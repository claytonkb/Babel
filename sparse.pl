#!/usr/bin/perl.exe -w

# s-expression parser (sparse) - A low-level "assembler" for Babel constructs

use strict;
use feature 'state';
no warnings 'recursion'; # Get "deep recursion" on large .pb files otherwise
use Hash::Pearson16;
use Carp;

use YAML::XS;
use Data::Dumper;

my @asm_file;
my $asm_file;
my $obj_out = [];

my $MWORD_SIZE = 4;
my $HASH_SIZE = 16/$MWORD_SIZE;

my $proj_name = $ARGV[0];

my $section_name    = qr/[_A-Za-z][_A-Za-z0-9]*/;
my $sigil_re        = qr/[\*\$&%!#@]/;
my $array_begin     = qr/[\[\({<]/;
my $array_end       = qr/[\]\)}>]/;
my $non_array       = qr/[^\[\({<\]\)}>]/;

my $sections = {};

while($#ARGV>-1){
    $asm_file = shift @ARGV;
    open ASM_FILE, $asm_file;
    push @asm_file, <ASM_FILE>;
    close ASM_FILE;
}

#Section:       ( z <section-name> ... )
#Values:        ( q ... )
#Pointers:      ( x ... )
#List:          ( k ... )
#Tagged-list:   ( j ... )

$asm_file = clean     ( \@asm_file );
            $sections = balanced_parse( \$asm_file );
            translate($sections);
            #assemble  ( $sections, $asm_file );
            #linkit    ( $sections, 'root', $obj_out, 0 );
            #send_obj  ( $proj_name, $obj_out, $sections);

print Dumper($sections);

#########################################################################
#
# CLEANER
#
#########################################################################

#
#
sub clean{
    my $asm_file = shift;
    remove_comments($asm_file);
    remove_ws      ($asm_file);
    return join ' ', @{$asm_file};
}

#
#
sub remove_comments{

    my $asm_file = shift;

    foreach my $line (@{$asm_file}){
        $line =~ s/--.*$//; #Strips a line-comment
    }

}

#
#
sub remove_ws{

    my $asm_file = shift;
    my @asm_file = @{$asm_file};

    chomp for @asm_file;

    splice(@{$asm_file});

    for(@asm_file){
        unless(/^\s*$/){
            push @{$asm_file}, $_;
        }
    }

}

#########################################################################
#
# BALANCED EXPRESSION PARSER
#
#########################################################################

# ( a [ b c { d e f } g ] h i ( j ) k )
sub balanced_parse{

    my $string      = shift;
    my $expression  = [];

    clean_string($string);

    my ($non_array_context) = ${$string} =~ /^($non_array*)/;

    if(defined $non_array_context 
            and length($non_array_context) > 0){
#            print "non_array_context .${$string}.\n";
        ${$string} = substr(${$string}, length($non_array_context));
        clean_string(\$non_array_context);
        push @{$expression}, $non_array_context;
        return $expression;
    }

    if( not ${$string} =~ /^($array_begin)/ ){
        print ".${$string}.\n";
        die;
    }

    my ($paren) = ${$string} =~ /^($array_begin)/;
    #push @{$expression}, get_paren_type($paren);

    ${$string} = substr(${$string}, 1);

    my $paren_type = $1;

    begin_balanced:
        clean_string($string);
        ($non_array_context) = ${$string} =~ /^($non_array*)/;

        if(defined $non_array_context 
                and length($non_array_context) > 0){
#            print "non_array_context .${$string}.\n";
            ${$string} = substr(${$string}, length($non_array_context));
            clean_string(\$non_array_context);

            push @{$expression}, split(/\s+/, $non_array_context);
        }

        if(${$string} =~ /^$array_begin/){
#            print "array_begin .${$string}.\n";
            #${$string} = substr(${$string}, 1);
            push @{$expression}, balanced_parse($string, $expression);
            goto begin_balanced;
        }
        elsif(${$string} =~ /^($array_end)/){
#            print "array_end .${$string}.\n";
            ${$string} = substr(${$string}, 1);
            die if not is_matching_paren($paren_type, $1);
            return $expression;
        }
        else{
            die;
            #reached the end-of-string, syntax error
        }

}

sub clean_string{
    my $string = shift;
    carp if(!ref($string));

    ${$string} =~ s/^\s*//;
    ${$string} =~ s/\s*$//;
}

sub get_paren_type {
    for(shift){
        /\(/ and return "LIST_PAREN";
        /\)/ and return "LIST_PAREN";
        /\[/ and return "ARRAY_PAREN";
        /\]/ and return "ARRAY_PAREN";
        /{/  and return "CODE_LIST_PAREN";
        /}/  and return "CODE_LIST_PAREN";
        /</  and return "BRACKET_PAREN";
        />/  and return "BRACKET_PAREN";
    }
}

sub is_matching_paren{
    my ($open_paren, $close_paren) = @_;
    return 1 if (get_paren_type($open_paren) eq get_paren_type($close_paren));
    return 0;
}

#########################################################################
#
# TRANSLATOR
#
# Evaluates each token according to its syntactical type
#
#########################################################################

sub translate{

    my $tree = shift;
    my $eval_string;

    foreach my $branch (@{$tree}){

        if(ref($branch) eq "ARRAY"){
            translate($branch);
        }
        elsif(ref($branch) eq ""){

            if($branch =~ /^(\s*-?[1-9][0-9]*)$/ or $branch =~ /^(\s*0+)[^x]$/ ){
                #$branch =~ s/^\s*(-?[0-9]+)//;
                $branch = 0+$1;
            }
            elsif($branch =~ /^(\s*0x[A-Fa-f0-9]+)$/){
                #$branch =~ s/^\s*0x([A-Fa-f0-9]+)//;
                $branch = hex($1);
            }
            elsif($branch =~ /^\s*"[^"]*"$/){
                $eval_string = "\$branch = $branch;";
                die unless eval($eval_string);
                $branch = "\"$branch\"";
            }
            elsif($branch =~ /^\s*'([^']*)'$/ or $branch =~ /^$section_name$/ ){
                #$branch = "$1";
                #Do nothing
            }
            else{
                print "Error: .$branch.\n" and die;
            }
        
        }
        else{
            print "ref type: " . ref($branch) . "\n";
            die;
        }

    }

}


#########################################################################
#
# ASSEMBLER
#
#########################################################################

#( z code ... )
#( z data ... )
#( z stack ... )
#( z root code data stack )
#
#( z root ( x 1 2 3 ) )

#
#
sub assemble{

    my $sections = shift;
    my $asm_file = shift;

    while(1){

        # return if EOF
        return if $asm_file =~ /^\s*$/;

        # if "( z ...
        if   ($asm_file =~ s/^\s*\(\s*z\s*//){
            assemble_section($sections, \$asm_file);
        }
        else{
            # die if not "( z ...
            assembly_error("assemble", $asm_file);
        }

    }

}

#
#
sub assemble_section{

    my $sections = shift;
    my $asm_file = shift;

    my $section_name = undef;

    # get section name
    ($section_name) = $$asm_file =~ /^\s*([_A-Za-z][_A-Za-z0-9]*)\s*/;
    if( defined $section_name ){
        $$asm_file =~ s/^\s*([_A-Za-z][_A-Za-z0-9]*)\s*//;
    }
    else{
        assembly_error("assemble_section(1)", $$asm_file);
    }

    if(exists $sections->{$section_name}){
        print STDERR "Warning: duplicate section $section_name\n";
    }

    $sections->{$section_name} = [];

    assemble_generic($sections->{$section_name}, $asm_file);

    unless($$asm_file =~ s/^\s*\)\s*//){
        assembly_error("assemble_section(3)", $$asm_file);
    }

}

#
#
sub assemble_generic{

    my $section = shift;
    my $asm_file = shift;

    # switch on [xqkj]
    if   ($$asm_file =~ s/^\s*\(\s*x\s*//){
        push @{$section}, ["x", assemble_values($asm_file)];
    }
    elsif($$asm_file =~ s/^\s*\(\s*q\s*//){
        push @{$section}, ["q", assemble_pointers($asm_file)];
    }
    elsif($$asm_file =~ s/^\s*\(\s*k\s*//){
        push @{$section}, ["k", assemble_list($asm_file)];
    }
    elsif($$asm_file =~ s/^\s*\(\s*j\s*//){
        push @{$section}, ["j", assemble_tag_list($asm_file)];
    }
    else{
        # die if not "( [xqkj] ...
        assembly_error("assemble_section(2)", $$asm_file);
    }

}

#
#
sub assemble_values{

    my $asm_file = shift;

    my $token;
    my $values = [];

    while(1){

        $token = is_numeric($asm_file);
        if(defined $token){
            push @{$values}, 0+$token;
            next;
        }

        $token = is_string($asm_file);
        if(defined $token){
            push @{$values}, "$token";
            next;
        }

        if($$asm_file =~ s/^\s*\)\s*//){
            return $values;
        }
        else{
            assembly_error("assemble_values", $$asm_file);
        }

    }

}

#
#
sub is_numeric{

    my $string = shift; #substr($section->{src}, $section->{ptr});

    if($$string =~ /^(\s*-?[1-9][0-9]*)/ or $$string =~ /^(\s*0+)[^x]/ ){
        $$string =~ s/^\s*(-?[0-9]+)//;
        return 0+$1;
    }

    if($$string =~ /^(\s*0x[A-Fa-f0-9]+)/){
        #$section->{ptr} += length $1;
        $$string =~ s/^\s*0x([A-Fa-f0-9]+)//;
        return hex($1);
    }

    return undef;

}

#
#
sub is_string{

    my $string = shift;

    if($$string =~ /^(\s*"[^"]*")/){
        $$string =~ s/^\s*"([^"]*)"//;
        return "$1";
    }
    elsif($$string =~ /^(\s*'[^']*')/){
        $$string =~ s/^\s*'([^']*)'//;
        return "$1";
    }

    return undef;

}

############################
#
sub assemble_pointers{

    # switch on [zxqkj]
    if   ($asm_file =~ s/^\s*\(\s*z\s*//){
        assemble_section($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*x\s*//){
        assemble_values($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*q\s*//){
        assemble_pointers($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*k\s*//){
        assemble_list($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*j\s*//){
        assemble_tag_list($sections, $asm_file);
    }
    else{
        # die if not "( [zxqkj] ...
        print substr( $asm_file, 0, 60 ) . "\n";
        die;
    }

}

#
#
sub assemble_list{

    # switch on [zxqkj]
    if   ($asm_file =~ s/^\s*\(\s*z\s*//){
        assemble_section($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*x\s*//){
        assemble_values($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*q\s*//){
        assemble_pointers($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*k\s*//){
        assemble_list($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*j\s*//){
        assemble_tag_list($sections, $asm_file);
    }
    else{
        # die if not "( [zxqkj] ...
        print substr( $asm_file, 0, 60 ) . "\n";
        die;
    }

}

#
#
sub assemble_tag_list{

    # switch on [zxqkj]
    if   ($asm_file =~ s/^\s*\(\s*z\s*//){
        assemble_section($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*x\s*//){
        assemble_values($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*q\s*//){
        assemble_pointers($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*k\s*//){
        assemble_list($sections, $asm_file);
    }
    elsif($asm_file =~ s/^\s*\(\s*j\s*//){
        assemble_tag_list($sections, $asm_file);
    }
    else{
        # die if not "( [zxqkj] ...
    }

}

sub assembly_error{
    my ($sub, $asm_file) = @_;
    print "Error in $sub: " . substr( $asm_file, 0, 60 ) . "\n";
    die;
}



#
#
#
#sub send_obj{
#
#    my $proj_name = shift;
#    my $obj_out = shift;
#    my $sections = shift;
#
#    open C_FILE,   ">${proj_name}.c";
#    open LST_FILE, ">${proj_name}.lst";
#    open BBL_FILE, ">${proj_name}.bbl";
#    binmode BBL_FILE;
#
#    printf C_FILE ("#define BBL_SIZE %d\nunsigned bbl[BBL_SIZE] = {", $#{$obj_out} + 1);
#
#    my $i = 0;
#    for(0..$#{$obj_out}){
#        if($i % 8 == 0){
#            print C_FILE "\n    ";
#        }
#        printf   C_FILE ("0x%08x, ",                      $obj_out->[$_] & 0xffffffff);
#        printf LST_FILE ("%04x %08x\n", $_ * $MWORD_SIZE, $obj_out->[$_] & 0xffffffff);
#        print  BBL_FILE pack("V",  $obj_out->[$_]);
#        $i++;
#    }
#
#    print C_FILE "\n};\n";
#
#    for(keys %{$sections}){
#        next if /^ANON_/;
#        if(defined $sections->{$_}{obj_ptr}){
#            my $op = $_;
#            $op =~ s/%/%%/g;
##            printf("$op\n");
#            printf LST_FILE ("%08x $op\n", $sections->{$_}{obj_ptr} * $MWORD_SIZE);
#        }
#    }
#
#    close LST_FILE;
#    close BBL_FILE;
#
#}
#
#
#sub linkit{
#
#    my $sections = shift;
#    my $curr_section = shift;
#    my $obj_out = shift;
#    my $curr_ptr = shift;
##    my $end_ptr = $curr_ptr + abs($sections->{$curr_section}{bin}[0])/$MWORD_SIZE + 1;
#    my $end_ptr;
#
##    if(! defined $sections->{$curr_section}{bin}[0]){
##        print "$curr_section\n";
##    }
#
#    if($sections->{$curr_section}{bin}[0] != 0){
#        $end_ptr = $curr_ptr + abs($sections->{$curr_section}{bin}[0])/$MWORD_SIZE + 1;
#    }
#    else{
#        $end_ptr = $curr_ptr + $HASH_SIZE + 1;
#    }
#
#    if(!defined $sections->{$curr_section}{obj_ptr}){
#        $sections->{$curr_section}{obj_ptr} = $curr_ptr+1;
#    }
#    else{
#        return $curr_ptr;
#    }
#
#    for (@{$sections->{$curr_section}{bin}}){
#        if($_ =~ /^(\s*-?[1-9][0-9]*)/ or $_ =~ /^(\s*0+)[^x]/ or $_ eq '0' ){
#            $obj_out->[$curr_ptr] = $_;
#            $curr_ptr++;
#            next;
#        }
#        #print("$_\n");
#        # if a name, recurse linkit
##        if( /[_A-Za-z][_A-Za-z0-9]*/ ){
#        if( "$_" =~ /$section_name/ ){
#            if(!exists $sections->{$_}){
#                print "Unresolved symbol: $_\n";
#                die;
#            }
#            $end_ptr = linkit($sections, $_, $obj_out, $end_ptr);
#            $obj_out->[$curr_ptr] = $sections->{$_}{obj_ptr} * $MWORD_SIZE;
#        }
#        else{
#            $obj_out->[$curr_ptr] = $_;
#        }
##        printf("%02x %08x\n", $curr_ptr, $obj_out->[$curr_ptr]);
#        $curr_ptr++;
#    }
#
#    return $end_ptr;
#
#}
#
## Choose whether to assemble_leaf or assemble_interior...
## This function should be re-entrant
#sub assemble{
#
#    my $sections     = shift;
#    my $this_section = shift;
#
#    return if $this_section->{asmd};
#
#    if($this_section->{obj}[0] eq "LEAF_ARR"){
#        push @{$this_section->{bin}}, 0;
#        $this_section->{bin}[0] = assemble_leaf($sections, $this_section) * $MWORD_SIZE;
#    }
#    elsif($this_section->{obj}[0] eq "HASH_REF"){
##        push    @{$this_section->{bin}}, @{$this_section->{obj}};
##        shift   @{$this_section->{bin}};
##        unshift @{$this_section->{bin}}, 0;
#        assemble_hash_ref($this_section);
#    }
#    else{
#        push @{$this_section->{bin}}, 0;
#        $this_section->{bin}[0] = (-1 * assemble_interior($sections, $this_section) * $MWORD_SIZE);
#    }
#
#    $this_section->{asmd} = 1;
#
#}
#
#sub assemble_hash_ref{
#
##    my $sections     = shift;
#    my $this_section = shift;
#
#    push    @{$this_section->{bin}}, @{$this_section->{obj}};
#    shift   @{$this_section->{bin}};
#    unshift @{$this_section->{bin}}, 0;
#
#}
#
#sub assemble_leaf {
#
#    my $sections     = shift;
#    my $this_section = shift;
#
#    my $count = 0;
#    my @str_vec;
#
#    for my $elem (@{$this_section->{obj}}){
#
#        next unless ref($elem) eq "ARRAY"; # The first element in the list is a scalar; ignore
#
#        if($elem->[0] eq "NUMERIC"){
#            push @{$this_section->{bin}}, $elem->[1];
#            $count++;
#            next;
#        }
#
#        if($elem->[0] eq "STRING"){
#            @str_vec = str2vec($elem->[1]);
##            print STDERR "\@str_vec: @str_vec\n";
#            $count += ($#str_vec + 1);
#            push @{$this_section->{bin}}, @str_vec;
#            next;
#        }
#
#        if($elem->[0] eq "ADDR_LABEL"){
#            assemble($sections, $sections->{$elem->[1]});
#            push @{$this_section->{bin}}, "$elem->[1]";
#            $count++;
#            next;
#        }
#
##        if($elem->[0] eq "EXP_LABEL"){
##
##            if($sections->{$elem->[1]}{obj}[0] ne "LEAF_ARR"){
##                print "Expected leaf array\n";
##                die;
##            }
##
##            unless($sections->{$elem->[1]}{asmd}){
##                assemble_leaf($sections, $sections->{$elem->[1]});
##            }
##
##            push @{$this_section->{bin}}, @{$sections->{$elem->[1]}{bin}};
##            $count += ($#{$sections->{$elem->[1]}{bin}} + 1);
##
##            next;
##
##        }
#
#    }
#
#    $this_section->{asmd} = 1;
#    return $count;
#
#}
#
#sub assemble_interior {
#
#    my $sections     = shift;
#    my $this_section = shift;
#    my @str_vec;
#    my $length;
#
#    my $count = 0;
#    state $anon_section_num = 0;
#
#    my $elem;
#    my $anon_section;
#
#    $this_section->{asmd} = 1;
#
#    for $elem (@{$this_section->{obj}}){
#
#        next unless ref($elem) eq "ARRAY"; # The first element in the list is a scalar; ignore
#
#        if($elem->[0] eq "INTERIOR_ARR"){
#
#            $anon_section = "ANON_$anon_section_num";
#            $anon_section_num++;
#
#            $sections->{$anon_section}{obj} = $elem;
#
#            push @{$sections->{$anon_section}{bin}}, 0;
#            $sections->{$anon_section}{bin}[0] 
#                = (-1 * assemble_interior($sections, $sections->{$anon_section}) * $MWORD_SIZE);
#
#            push @{$this_section->{bin}}, $anon_section;
#
#            $count++;
#            next;
#
#        }
#
#        if(
#            $elem->[0] eq "NUMERIC"
#                or
#            $elem->[0] eq "STRING"
#        ){
#            $elem = ["LEAF_ARR", $elem];
#        }
#
#        if( $elem->[0] eq "HASH_REF" ){
#            push    @{$this_section->{bin}}, 0;
#            push    @{$this_section->{bin}}, $elem->[1];
#            push    @{$this_section->{bin}}, $elem->[2];
#            push    @{$this_section->{bin}}, $elem->[3];
#            push    @{$this_section->{bin}}, $elem->[4];
#        }
#        
#        if($elem->[0] eq "LEAF_ARR"){
#
#            $anon_section = "ANON_$anon_section_num";
#            $anon_section_num++;
#
#            $sections->{$anon_section}{obj} = $elem;
#
#            push @{$sections->{$anon_section}{bin}}, 0;
#            $sections->{$anon_section}{bin}[0] 
#                = assemble_leaf($sections, $sections->{$anon_section})  * $MWORD_SIZE;
#
#            push @{$this_section->{bin}}, $anon_section;
#
#            $anon_section_num++;
#            $count++;
#            next;
#
#        }
#
#        if($elem->[0] eq "HASH_REF"){
#
#            $anon_section = "ANON_$anon_section_num";
#            $anon_section_num++;
#
#            $sections->{$anon_section}{obj} = $elem;
#
#            assemble_hash_ref($sections->{$anon_section});
#
##            push @{$sections->{$anon_section}{bin}}, 0;
##            $sections->{$anon_section}{bin}[0] 
##                = assemble_leaf($sections, $sections->{$anon_section})  * $MWORD_SIZE;
##
#            push @{$this_section->{bin}}, $anon_section;
#
#            $anon_section_num++;
#            $count++;
#            next;
#
#        }
#
#        if($elem->[0] eq "LABEL"){
#
#            assemble($sections, $sections->{$elem->[1]});
#            push @{$this_section->{bin}}, "$elem->[1]";
#            $count++;
#            next;
#
#        }
#
##        if($elem->[0] eq "EXP_LABEL"){
##
##            if($sections->{$elem->[1]}{obj}[0] ne "INTERIOR_ARR"){
##                print "Expected interior array\n";
##                die;
##            }
##
##            assemble($sections, $sections->{$elem->[1]});
##            my @temp = @{$sections->{$elem->[1]}{bin}};
##            push @{$this_section->{bin}}, splice(@temp, 1);
##            $count += abs($sections->{$elem->[1]}{bin}[0]) / $MWORD_SIZE;
##            next;
##
##        }
#
#    }
#
#    return $count;
#
#}
#
#
#sub get_sections{
#
#    my $asm_file = shift;
#    my $sections;
#    my $current_section;
#    my $match;
#    my $current_sigil;
#
#    #Create root section
#    $sections->{'root'}{src} = [];
#    $sections->{'root'}{ptr} = 0;
#    $sections->{'root'}{asmd} = 0;
#    $sections->{'root'}{bin_ptr} = undef;
#    $sections->{'root'}{bin} = [];
##    push @{$sections->{'nil'}{src}}, "[nil nil]";
#                                              #stack ustack rstack jump  sym  TID       argv  steps   advance
#    push @{$sections->{'root'}{src}}, "[ main nil    nil    nil    nil   nil  thread_id nil   steps   advance_type ]";
#
#    $sections->{'thread_id'}{src} = [];
#    $sections->{'thread_id'}{ptr} = 0;
#    $sections->{'thread_id'}{asmd} = 0;
#    $sections->{'thread_id'}{bin_ptr} = undef;
#    $sections->{'thread_id'}{bin} = [];
##    push @{$sections->{'nil'}{src}}, "[nil nil]";
#                                              #stack ustack rstack jump  sym  TID argv steps advance
#    push @{$sections->{'thread_id'}{src}}, "{0}";
#
#    $sections->{'steps'}{src} = [];
#    $sections->{'steps'}{ptr} = 0;
#    $sections->{'steps'}{asmd} = 0;
#    $sections->{'steps'}{bin_ptr} = undef;
#    $sections->{'steps'}{bin} = [];
##    push @{$sections->{'nil'}{src}}, "[nil nil]";
#                                              #stack ustack rstack jump  sym  TID argv steps advance
#    push @{$sections->{'steps'}{src}}, "{-1}";
#
#    $sections->{'advance_type'}{src} = [];
#    $sections->{'advance_type'}{ptr} = 0;
#    $sections->{'advance_type'}{asmd} = 0;
#    $sections->{'advance_type'}{bin_ptr} = undef;
#    $sections->{'advance_type'}{bin} = [];
##    push @{$sections->{'nil'}{src}}, "[nil nil]";
#                                              #stack ustack rstack jump  sym  TID argv steps advance
#    push @{$sections->{'advance_type'}{src}}, "{0}";
#
#
#    # Create nil for use by lists
#    $sections->{'nil'}{src} = [];
#    $sections->{'nil'}{ptr} = 0;
#    $sections->{'nil'}{asmd} = 0;
#    $sections->{'nil'}{bin_ptr} = undef;
#    $sections->{'nil'}{bin} = [];
##    push @{$sections->{'nil'}{src}}, "[nil nil]";
#    push @{$sections->{'nil'}{src}}, "nil&";
#
#    for(@{$asm_file}){
##        if(/^([a-zA-Z0-9_]+):/){
#        if(/^($section_name)(${sigil_re}?):/){
#
#            $current_section = $1;
#            $current_sigil   = defined $2 ? $2 : undef;
#
#            $sections->{$current_section}{src}     = [];
#            $sections->{$current_section}{ptr}     = 0;
#            $sections->{$current_section}{asmd}    = 0;
#            $sections->{$current_section}{bin_ptr} = undef;
#            $sections->{$current_section}{bin}     = [];
#            $sections->{$current_section}{sigil}   = "DIRECT_REF";
#
#            if(defined $current_sigil){
#                for($current_sigil){
#                    /\*/ and $sections->{$current_section}{sigil} = "LEX_SUB";
#                    /\$/ and $sections->{$current_section}{sigil} = "DIRECT_REF";
#                    /&/  and $sections->{$current_section}{sigil} = "INDIRECT_REF";
#                    /%/  and $sections->{$current_section}{sigil} = "HASH_REF";
#                    /!/  and $sections->{$current_section}{sigil} = "AUTO_EVAL";
#                }
#            }
#
##            if(/\@/){
##                $sections->{$current_section}{nest} = 0;
##            }
##            else{
##                $sections->{$current_section}{nest} = 1;
##            }
#
##            ($match) = $_ =~ /^[a-zA-Z0-9_]+:(.+)$/;
#            ($match) = $_ =~ /^${section_name}${sigil_re}?:(.+)$/;
#            if(defined $match){
#                push @{$sections->{$current_section}{src}}, $match;
#            }
#            next;
#
#        }
#        push @{$sections->{$current_section}{src}}, $_;
#    }
#
#    for(keys %{$sections}){
#        $sections->{$_}{src} = join '', @{$sections->{$_}{src}};
#
##        next if $_ eq 'root';
##        next if $_ eq 'thread_id';
##        next if $_ eq 'steps';
##        next if $_ eq 'advance_type';
##        next if $_ eq 'nil';
##        next if $_ eq 'main';
#
##        next unless $sections->{$_}{nest};
##
##        print "$_ ";
#
##        $sections->{$_}{src} = '[ ' . $sections->{$_}{src} . ' ]';
#    }
#
#    return $sections;
#
#}
#
#
#
###################################################
## Section parser
###################################################
#
#sub section_parse{
#
#    my $section = shift;
#    my $new_array_ref;
#    my $token;
#
#    my $src = $section->{src};
#    $section->{obj} = [];
#
##    my $current_context;
#
#    $token = is_numeric($section);
#    if(defined $token){
#        $section->{obj} = ["LEAF_ARR", ["NUMERIC", 0+$token]];
#        # TODO: write is_whitespace() to check for syntax errors
#        return;
#    }
#
#    $token = is_string($section);
#    if(defined $token){
#        $section->{obj} = ["LEAF_ARR", ["STRING", "$token"]];
#        # TODO: write is_whitespace() to check for syntax errors
#        return;
#    }
#
#    my @hash = is_hash_ref($section);
#    if($#hash > -1){
#        $section->{obj} = ["HASH_REF", @hash];
##        push @{$obj}, ["HASH_REF", @hash];
##my $j;
##for($j=$#hash;$j>=0;$j--){
##    printf("%08x", $hash[$j]);
##}
##print "\n";
##die;
#        return;
#    }
#
#    if(is_interior_array_begin($section)){
##        $section->{typ} = "interior_arr";
#        $section->{obj} = ["INTERIOR_ARR"];
#        interior_array($section, $section->{obj});
#        return;
#    }
#
#    if(is_leaf_array_begin($section)){
##        $section->{typ} = "leaf_arr";
#        $section->{obj} = ["LEAF_ARR"];
#        leaf_array($section, $section->{obj});
#        return;
#    }
#
#    if(is_list_begin($section)){
##        die "list detected\n";
#        $section->{obj} = ["INTERIOR_ARR"];
#        list_interior_array($section, $section->{obj},0);
#        return;
#    }
#
#    print "Syntax error\n";
#    die;
#
#}
#
#sub leaf_array{
#
#    my $section = shift;
#    my $obj = shift;
#
##    print Dumper($src), die;
#
##    my $current_context;
#
#    my $label;
#    my $token;
#
#    while(1){
#
#        if(is_leaf_array_end($section)){
#            last;
#        }
#
##        $label = is_expanded_label($section);
##        if(defined $label){
##            push @{$obj}, ["EXP_LABEL", "$label"];
##            next;
##        }
#
##        $label = is_addr_label($section);
##        if(defined $label){
##            push @{$obj}, ["ADDR_LABEL", "$label"];
##            next;
##        }
#
#        $token = is_numeric($section);
#        if(defined $token){
#            push @{$obj}, ["NUMERIC", 0+$token];
#            next;
#        }
#
#        $token = is_string($section);
#        if(defined $token){
#            $token = '$token = "' . $token . '"';
#            eval($token);
#            push @{$obj}, ["STRING", "$token"];
#            next;
#        }
#
#        #       $current_context = current_context();
#        #die "Syntax error: $current_context\n";
#        print "Syntax error: " . substr($section->{src}, $section->{ptr}) . "\n";
#        print Dumper($section);
#        die;
#
#    }
#
#}
#
#sub interior_array{
#
#    my $section = shift;
#    my $obj = shift;
#    my $token;
#
##    my $current_context;
#    my $new_array_ref;
#
#    my $label;
#    my @hash;
#
#    while(1){
#
#        if(is_interior_array_end($section)){
#            last;
#        }
#
#        if(is_interior_array_begin($section)){
#            $new_array_ref = ["INTERIOR_ARR"];
#            interior_array($section, $new_array_ref);
#            push @{$obj}, $new_array_ref;
#            next;
#        }
#
#        if(is_leaf_array_begin($section)){
#            $new_array_ref = ["LEAF_ARR"];
#            leaf_array($section, $new_array_ref);
#            push @{$obj}, $new_array_ref;
#            next;
#        }
#
#        if(is_list_begin($section)){
#    #        die "list detected\n";
#            $new_array_ref = ["INTERIOR_ARR"];
#            list_interior_array($section, $new_array_ref,0);
#            push @{$obj}, $new_array_ref;
#            next;
#        }
#
#        @hash = is_hash_ref($section);
#        if($#hash > -1){
#            push @{$obj}, ["HASH_REF", @hash];
##my $j;
##for($j=$#hash;$j>=0;$j--){
##    printf("%02x", $hash[$j]);
##}
##print "\n";
##die;
#            next;
#        }
#
#        $token = is_numeric($section);
#        if(defined $token){
#            push @{$obj}, ["NUMERIC", 0+$token];
#            next;
#        }
#
#        $label = is_label($section);
#        if(defined $label){
#            push @{$obj}, ["LABEL", "$label"];
#            next;
#        }
#
##        $label = is_expanded_label($section);
##        if(defined $label){
##            push @{$obj}, ["EXP_LABEL", "$label"];
##            next;
##        }
#
#        $token = is_string($section);
#        if(defined $token){
#            $token = '$token = "' . $token . '"';
#            eval($token);
#            push @{$obj}, ["STRING", "$token"];
#            next;
#        }
#
#        print "Syntax error\n";
#        die;
#
#    }
#
#}
#
#
#sub list_interior_array{
#
#    my $section = shift;
#    my $obj = shift;
#    my $quick_wrap = shift;
#    my $token;
#
##    my $current_context;
#    my $new_array_ref;
#
#    my $label;
#
##    if(is_quick_wrap($section)){
##        $new_array_ref = ["INTERIOR_ARR"];
##        list_interior_array($section, $new_array_ref,1);
##        push @{$obj}, $new_array_ref;
##        goto DONE;
##    }
#    
#    if(is_list_begin($section)){
#        $new_array_ref = ["INTERIOR_ARR"];
#        list_interior_array($section, $new_array_ref,0);
#        push @{$obj}, $new_array_ref;
#        goto DONE;
#    }
#
#    if(is_interior_array_begin($section)){
#        $new_array_ref = ["INTERIOR_ARR"];
#        interior_array($section, $new_array_ref);
#        push @{$obj}, $new_array_ref;
#        goto DONE;
#    }
#
#    if(is_leaf_array_begin($section)){
#        $new_array_ref = ["LEAF_ARR"];
#        leaf_array($section, $new_array_ref);
#        push @{$obj}, $new_array_ref;
#        goto DONE;
#    }
#
#    my @hash = is_hash_ref($section);
#    if($#hash > -1){
#        push @{$obj}, ["HASH_REF", @hash];
##my $j;
##for($j=$#hash;$j>=0;$j--){
##    printf("%02x", $hash[$j]);
##}
##print "\n";
##die;
#        goto DONE;
#    }
#
#    $token = is_numeric($section);
#    if(defined $token){
#        push @{$obj}, ["NUMERIC", 0+$token];
#        goto DONE;
#    }
#
#    $label = is_label($section);
#    if(defined $label){
#        push @{$obj}, ["LABEL", "$label"];
#        goto DONE;
#    }
#
##    $label = is_expanded_label($section);
##    if(defined $label){
##        push @{$obj}, ["EXP_LABEL", "$label"];
##        goto DONE;
##    }
#
#    $token = is_string($section);
#    if(defined $token){
#        $token = '$token = "' . $token . '"';
#        eval($token);
#        push @{$obj}, ["STRING", "$token"];
#        goto DONE;
#    }
#
#DONE:
#
#    if(is_list_end($section) or $quick_wrap ){
#        push @{$obj}, ["LABEL","nil"];
#        return;
#    }
#
#    $new_array_ref = ["INTERIOR_ARR"];
#    list_interior_array($section, $new_array_ref,0);
#    push @{$obj}, $new_array_ref;
#
#}
#
#sub is_leaf_array_begin{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
##    $string =~ /^(\s*\{)/;
#    
#    if($string =~ /^(\s*\{)/){
#        $section->{ptr} += length $1;
#        return 1;
#    }
#
#    return 0;
#
#}
#
#sub is_leaf_array_end{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
#    if($string =~ /^(\s*\})/){
#        $section->{ptr} += length $1;
#        return 1;
#    }
#
#    return 0;
#}
#
#sub is_interior_array_begin{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
##    $string =~ /^(\s*\()/;
#    
#    if($string =~ /^(\s*\[)/){
#        $section->{ptr} += length $1;
#        return 1;
#    }
#
#    return 0;
#
#}
#
#sub is_interior_array_end{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
##    $string =~ /^(\s*\))/;
#    
#    if($string =~ /^(\s*\])/){
#        $section->{ptr} += length $1;
#        return 1;
#    }
#
#    return 0;
#
#}
#
##sub is_expanded_label{
##
##    my $section = shift;
##
##    my $string = substr($section->{src}, $section->{ptr});
##
###    $string =~ /^(\s*\*[A-Za-z_][A-Za-z_0-9]*)/;
##    
###    if($string =~ /^(\s*\*[A-Za-z_][A-Za-z_0-9]*)/){
##    if($string =~ /^(\s*\*$section_name)/){
##        $section->{ptr} += length $1;
###        $string =~ /^\s*\*([A-Za-z_][A-Za-z_0-9]*)/;
##        $string =~ /^\s*\*($section_name)/;
##        return $1;
##    }
##
##    return undef;
##
##}
#
##sub is_addr_label{
##
##    my $section = shift;
##
##    my $string = substr($section->{src}, $section->{ptr});
##
###    $string =~ /^(\s*\*[A-Za-z_][A-Za-z_0-9]*)/;
##    
###    if($string =~ /^(\s*&[A-Za-z_][A-Za-z_0-9]*)/){
##    if($string =~ /^(\s*&$section_name)/){
##        $section->{ptr} += length $1;
###        $string =~ /^\s*&([A-Za-z_][A-Za-z_0-9]*)/;
##        $string =~ /^\s*&($section_name)/;
##        return $1;
##    }
##
##    return undef;
##
##}
#
#sub is_label{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
##   $string =~ /^(\s*[A-Za-z_][A-Za-z_0-9]*)/;
#    
##    if($string =~ /^(\s*[A-Za-z_][A-Za-z_0-9]*)/){
#    if($string =~ /^(\s*$section_name)/){
#        $section->{ptr} += length $1;
##        $string =~ /^\s*([A-Za-z_][A-Za-z_0-9]*)/;
#        $string =~ /^\s*($section_name)/;
#        return $1;
#    }
#
#    return undef;
#
#}
#
#sub is_hash_ref{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
##   $string =~ /^(\s*[A-Za-z_][A-Za-z_0-9]*)/;
##   print substr($string,0,10) . "\n";
#
##    if($string =~ /^(\s*[A-Za-z_][A-Za-z_0-9]*&)/){
#    if($string =~ /^(\s*$section_name&)/){
#        $section->{ptr} += length $1;
##        $string =~ /^\s*([A-Za-z_][A-Za-z_0-9]*)&/;
#        $string =~ /^\s*($section_name)&/;
#        return bytes_to_mwords(Hash::Pearson16::pearson16_hash($1));
#    }
#
#    return ();
#
#}
#
#sub bytes_to_mwords{
#
#    my @bytes = @_;
#    my @mwords;
#    my $temp=0;
#
#    for(my $i=0; $i<=$#bytes; $i++){
##        printf("%02x ", $bytes[$i]);
#        if( (($i % $MWORD_SIZE) == 0) and ($i != 0) ){
#            push @mwords, $temp;
#            $temp=0;
#        }
#        $temp = $temp | ( $bytes[$i] << (($i % $MWORD_SIZE) * 8) ); #8 bits per byte
#    }
#    push @mwords, $temp;
##    printf("%08x ", $_) for @mwords;
##    die;
#
#    return @mwords;
#
#}
#
#
#sub is_quick_wrap{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
##    $string =~ /^(\s*\{)/;
#    
#    if($string =~ /^(\s*`)/){
#        $section->{ptr} += length $1;
#        return 1;
#    }
#
#    return 0;
#}
#
#sub is_list_begin{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
##    $string =~ /^(\s*\{)/;
#    
#    if($string =~ /^(\s*\()/){
#        $section->{ptr} += length $1;
#        return 1;
#    }
#
#    return 0;
#
#}
#
#sub is_list_end{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
##    $string =~ /^(\s*\{)/;
#    
#    if($string =~ /^(\s*\))/){
#        $section->{ptr} += length $1;
#        return 1;
#    }
#
#    return 0;
#
#}
#
#
#sub str2vec {
#
#    my $str = shift;
#
#    my $index = 0;
#    my $my_len;
#    my @vec;
#
#    while(1){
#        $my_len = length( substr( $str, $index ) );
#        if($my_len == 0){
##            push @vec, 0xffffffff;
#            push @vec, 0x00000000;
#            last;
#        }
#        elsif($my_len == 1){
#            push @vec, ord( substr( $str, $index, 1 ) );
#            push @vec, 0xffffff00;
##            push @vec, 0x000000ff;
#            last;
#        }
#        elsif($my_len == 2){
#            push @vec, 
#                (
#                    ord( substr( $str, $index, 1 ) ) 
#                    |
#                    (ord( substr( $str, $index+1, 1 ) ) << 8)
#                )
#                ;
#            push @vec, 0xffff0000;
##            push @vec, 0x0000ffff;
#            last;
#        }
#        elsif($my_len == 3){
#            push @vec, 
#                (
#                    ord( substr( $str, $index, 1 ) ) 
#                    |
#                    (ord( substr( $str, $index+1, 1 ) ) << 8)
#                    |
#                    (ord( substr( $str, $index+2, 1 ) ) << 16)
#                )
#                ;
#            push @vec, 0xff000000;
##            push @vec, 0x00ffffff;
#            last;
#        }
#        else{
#            push @vec, 
#                (
#                    ord( substr( $str, $index, 1 ) ) 
#                    |
#                    (ord( substr( $str, $index+1, 1 ) ) << 8)
#                    |
#                    (ord( substr( $str, $index+2, 1 ) ) << 16)
#                    |
#                    (ord( substr( $str, $index+3, 1 ) ) << 24)
#                )
#                ;
#            $index+=4;
#        }
#
#    }
#
#    return @vec;
#
#}
#
#sub inskha{
#
#    my $hash_table  = shift;
#    my $key         = shift;
#    my $val         = shift;
#
#    my $result;
#    my $temp;
#    my $cons_side;
#
#    my $hash = 0;
#
#    if(is_nil($hash_table)){
#        $cons_side = _cxr1($hash,0);
#        $temp = new_hash_entry($hash, $key, $val, -1, 0);
#        $result = ();
#        $result->[$cons_side] = $temp;
#    }
#    else{
#        _rinskha($hash_table, $hash, $key, $val, 0);
#        $result = $hash_table;
#    }
#
#    return $result;
#
##    if($#{$hash_table} < 0){
##        my $cons_side = _cxr1($hash,0);
##        my $new_ref =   [ [ "HASH_REF", @{$hash} ],
##                        [ [ "INTERIOR_ARR", [$val]],
##                        [ [ "LEAF_ARR", ["STRING", "$key"]],
##                        [ [ "LEAF_ARR", ["NUMERIC", 0]], 
##                        ["LABEL","nil"] ]]]];
##    }
#
#}
#
##void _rinskha(mword *hash_table, mword *hash, mword *key, mword *val, mword level){
#sub rinskha{
#
#    my $hash_table = shift;
#    my $hash = shift;
#    my $key = shift;
#    my $val = shift;
#    my $level = shift;
#
#    my $temp;
#    my $cons_side  = _cxr1($hash,$level);
#    my $next_level = $hash_table->[$cons_side];
#
#    my $HASH_ENTRY_SIZE = 5;
#
##    // 1. cons_side = nil
##    //      insert
##    // 2. cons_side is inte AND size = 2
##    //      recurse
##    // 3. cons_side is inte AND size = HASH_ENTRY_SIZE
##    //      split and insert
##
##//    printf("%x\n",s(next_level));
#
#    if(is_nil($next_level)){
#
#        $hash_table->[$cons_side] = new_hash_entry($hash, $key, $val, -1, 0);
#
#    }
#    elsif(is_inte($next_level) && size($next_level) == 2){
#
#        _rinskha($hash_table->[$cons_side], $hash, $key, $val, $level+1);
#
#    }
#    elsif(is_inte($next_level) && size($next_level) == $HASH_ENTRY_SIZE){
#
#        my $colliding_hash_ref           = HASH_ENTRY_REF($next_level);
#        my $colliding_next_cons_side     = _cxr1($colliding_hash_ref,$level+1);
#
#        $temp = ();
#        $temp->[$colliding_next_cons_side] = $next_level;
#
#        $hash_table->[$cons_side]     = $temp;
#
#        _rinskha($hash_table->[$cons_side], $hash, $key, $val, $level+1);
#
#    }
#    else{
#        die;
#    }
#
#}
#
#sub new_hash_entry{
#}
#
#sub HASH_ENTRY_REF{
#}
#
#sub is_nil{
#
##    my $check = shift;
##
##    if(size($val) != 4){
##    }
##
##    my $nil = bytes_to_mwords(Hash::Pearson16::pearson16_hash("nil"));
#
#}
#
#sub is_inte{
#}
#
#sub is_href{
#}
#
#sub size{
#}
#
#sub _cxr1{
#
#    my $val = shift;
#    my $bit_place = shift;
#
#    return (($val & (1 << $bit_place)) >> $bit_place);
#
#}



