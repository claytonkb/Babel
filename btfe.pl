#!/usr/bin/perl.exe -w

#Babel Temporary Front-End

package Babel::TFE;

use strict;
use feature 'state';
no warnings 'recursion'; # Get "deep recursion" on large .pb files otherwise
use Hash::Pearson16;

use YAML::XS;
use Data::Dumper;

my @asm_file;
my $asm_file;
my $obj_out = [];

#my $MWORD_SIZE = 8;    # 64-bit mode
my $MWORD_SIZE = 4;
my $HASH_SIZE = 16/$MWORD_SIZE;

my $proj_name = $ARGV[0];

#my $section_name = qr/[_A-Za-z\+-<>=!~#%\.\$\?\/\|^][_A-Za-z0-9\+-<>=!~#%\.\$\?\/\|^]*/;

my $section_name = qr/[_A-Za-z][_A-Za-z0-9]*/;
my $sigil_re     = qr/[\*\$&%!]/;

#unshift @ARGV, "pb/opcodes.pb";

my $sections = {};

#while($#ARGV>-1){
    $asm_file = shift @ARGV;
    open ASM_FILE, $asm_file;
#    push @asm_file, <ASM_FILE>;
    @asm_file = <ASM_FILE>;
    close ASM_FILE;
#}

$asm_file = \@asm_file;

my $first_line = 0+discard_null_context($asm_file, 0);

my $left_edge = get_dent($asm_file->[$first_line]);

#my $size_asm_file = $#{$asm_file};

@{$asm_file} = @{$asm_file}[$first_line..$#asm_file];

parse_ntb( $asm_file, $left_edge, $sections );

print Dumper($sections);
die;

#                    section_parse   ( $sections->{$_}) for (keys %{$sections});
#
##                    assemble        ( $sections, $sections->{'main'} );
##                    linkit          ( $sections, 'main', $obj_out, 0 );
#
#                    assemble        ( $sections, $sections->{'root'} );
#                    linkit          ( $sections, 'root', $obj_out, 0 );
#
#                    send_obj        ( $proj_name, $obj_out, $sections);
#
#print Dumper($sections);

#########################################################################
#
# PRE-PARSER
#
#########################################################################

sub parse_ntb{

    my $ntb_file  = shift;
    my $left_edge = shift;
    my $sections  = shift;
    my $current_line = 0;
    my ($label, $sigil, $remainder);
    my $next_left_edge = $left_edge;
    my $next_line;

    while( $next_left_edge == $left_edge ){

        # FIXME: Detect and report syntax errors early...
        ($label, $sigil, $remainder) = get_label(undent($ntb_file->[$current_line],$left_edge));

        $next_line = find_next_non_empty_line( $ntb_file, $current_line+1 );

            if( is_label( undent( $ntb_file->[$current_line], $left_edge) ) ){


            $sections->{$label} = {};
            $sections->{$label}{sigil} = $sigil if defined $sigil;
            push(@{$sections->{$label}{text}}, $remainder) if defined $remainder;

            print "asdf: $current_line\n";

            $next_left_edge = get_dent( $ntb_file->[$current_line] );

            if($next_left_edge > $left_edge){
                print "A\n";
                ($current_line,$next_left_edge) = parse_ntb( [ @{$ntb_file}[$current_line .. $#{$ntb_file}] ], $next_left_edge, $sections->{$label} );

                print "$next_left_edge\n";
                print "$left_edge\n";

                if(($next_left_edge == $left_edge) 
                        and ($current_line < $#{$ntb_file}-2)){
                    $current_line++;
                    print "B\n";
                    die;
                    next;
                }
                else{
                    print "C\n";
                    return ( $current_line, get_dent( $ntb_file->[$current_line] ) );
                }
            }
            elsif($next_left_edge < $left_edge){
                print "D\n";
                return ( $current_line, $next_left_edge );
            }
            else{ #($next_left_edge == $left_edge)
                print "E\n";
                if($current_line < $#{$ntb_file}){
                    print "F\n";
                    $current_line++;
                    next;
                }
                else{
                    print "G\n";
                    return ( $current_line, get_dent( $ntb_file->[$current_line] ) );
                }
            }

        }
        else{
            print "H\n";
            
        }

    }

}

sub gather_text{

    my $ntb_file  = shift;
    my $left_edge = shift;
    my $sections  = shift;
    my $current_line = 0;
    my ($label, $sigil, $remainder);
    my $next_left_edge;

    while( $current_line <= $#{$ntb_file} 
            and ( (get_dent($ntb_file->[$current_line]) >= $left_edge)
            or is_empty($ntb_file->[$current_line]))){
        print "I\n";
        if(is_empty($ntb_file->[$current_line])){
            push @{$sections->{text}}, $ntb_file->[$current_line],$left_edge;
        }
        else{
            push @{$sections->{text}}, undent($ntb_file->[$current_line],$left_edge);
        }
        $current_line++;
    }

    print "zxcv: $ntb_file->[$current_line]\n";

    return ( $current_line, get_dent( $ntb_file->[$current_line] ) );

}

sub find_next_non_empty_line{

    my $ntb_file = shift;
    my $current_line = shift;

    while( is_empty( $ntb_file->[$current_line] ) ){
        $current_line++;
    }

    return $current_line;

}

#sub get_next_left_edge{
#
#    my $ntb_file = shift;
#    my $current_line = shift;
#    my $left_edge = shift;
#
#    while( is_empty( $ntb_file->[$current_line] ) ){
#        $current_line++;
#    }
#    die;
#
#    return get_dent($ntb_file->[$current_line]);
#
#}

sub is_empty{ #tabs show up as non-empty ... 

    my $line = shift;
    return $line =~ /^ *$/;

}

sub get_dent{

    my $line = shift;
    $line =~ /( *)\S/;
    return length($1) if defined $1;
    return 0;

}

sub get_label{

    my $line = shift;

    my ($label, $sigil, $remainder) 
        = $line =~ /^($section_name)($sigil_re?) *:(.*)/;

    $sigil = sigil_type($sigil) if defined $sigil;

    return ($label, $sigil, $remainder);

}


sub discard_null_context{

    my $ntb_file = shift;
    my $current_line = shift;
    my $left_edge = 0;

    while( !is_label( undent( $ntb_file->[$current_line], $left_edge) ) ){
        $current_line++;
    }

    return $current_line;

}

sub is_label{

    my $line = shift;
    return 1 if $line =~ /^${section_name}$sigil_re? *:/;
    return 0;

}

sub undent{

    my $line = shift;
    my $dent = shift;

    return substr( $line, $dent, length($line) );

}

#########################################################################
#
# PARSER (SEE BELOW)
#
#########################################################################

#########################################################################
#
# ASSEMBLER
#
#########################################################################

# Choose whether to assemble_leaf or assemble_interior...
# This function should be re-entrant
sub assemble{

    my $sections     = shift;
    my $this_section = shift;

    return if $this_section->{asmd};

    if($this_section->{obj}[0] eq "LEAF_ARR"){
        push @{$this_section->{bin}}, 0;
        $this_section->{bin}[0] = assemble_leaf($sections, $this_section) * $MWORD_SIZE;
    }
    elsif($this_section->{obj}[0] eq "HASH_REF"){
#        push    @{$this_section->{bin}}, @{$this_section->{obj}};
#        shift   @{$this_section->{bin}};
#        unshift @{$this_section->{bin}}, 0;
        assemble_hash_ref($this_section);
    }
#    elsif($this_section->{obj}[0] eq "ALIAS"){
#    }
    else{
        push @{$this_section->{bin}}, 0;
        $this_section->{bin}[0] = (-1 * assemble_interior($sections, $this_section) * $MWORD_SIZE);
    }

    $this_section->{asmd} = 1;

}

sub assemble_hash_ref{

#    my $sections     = shift;
    my $this_section = shift;

    push    @{$this_section->{bin}}, @{$this_section->{obj}};
    shift   @{$this_section->{bin}};
    unshift @{$this_section->{bin}}, 0;

}

sub assemble_leaf {

    my $sections     = shift;
    my $this_section = shift;

    my $count = 0;
    my @str_vec;

    for my $elem (@{$this_section->{obj}}){

        next unless ref($elem) eq "ARRAY"; # The first element in the list is a scalar; ignore

        if($elem->[0] eq "NUMERIC"){
            push @{$this_section->{bin}}, $elem->[1];
            $count++;
            next;
        }

        if($elem->[0] eq "STRING"){
            @str_vec = str2vec($elem->[1]);
#            print STDERR "\@str_vec: @str_vec\n";
            $count += ($#str_vec + 1);
            push @{$this_section->{bin}}, @str_vec;
            next;
        }

        if($elem->[0] eq "ADDR_LABEL"){
            assemble($sections, $sections->{$elem->[1]});
            push @{$this_section->{bin}}, "$elem->[1]";
            $count++;
            next;
        }

#        if($elem->[0] eq "EXP_LABEL"){
#
#            if($sections->{$elem->[1]}{obj}[0] ne "LEAF_ARR"){
#                print "Expected leaf array\n";
#                die;
#            }
#
#            unless($sections->{$elem->[1]}{asmd}){
#                assemble_leaf($sections, $sections->{$elem->[1]});
#            }
#
#            push @{$this_section->{bin}}, @{$sections->{$elem->[1]}{bin}};
#            $count += ($#{$sections->{$elem->[1]}{bin}} + 1);
#
#            next;
#
#        }

    }

    $this_section->{asmd} = 1;
    return $count;

}

sub assemble_interior {

    my $sections     = shift;
    my $this_section = shift;
    my @str_vec;
    my $length;

    my $count = 0;
    state $anon_section_num = 0;

    my $elem;
    my $anon_section;

    $this_section->{asmd} = 1;

    for $elem (@{$this_section->{obj}}){

        next unless ref($elem) eq "ARRAY"; # The first element in the list is a scalar; ignore

        if($elem->[0] eq "INTERIOR_ARR"){

            $anon_section = "ANON_$anon_section_num";
            $anon_section_num++;

            $sections->{$anon_section}{obj} = $elem;

            push @{$sections->{$anon_section}{bin}}, 0;
            $sections->{$anon_section}{bin}[0] 
                = (-1 * assemble_interior($sections, $sections->{$anon_section}) * $MWORD_SIZE);

            push @{$this_section->{bin}}, $anon_section;

            $count++;
            next;

        }

        if(
            $elem->[0] eq "NUMERIC"
                or
            $elem->[0] eq "STRING"
        ){
            $elem = ["LEAF_ARR", $elem];
        }

        if( $elem->[0] eq "HASH_REF" ){
            push    @{$this_section->{bin}}, 0;
            push    @{$this_section->{bin}}, $elem->[1];
            push    @{$this_section->{bin}}, $elem->[2];
            push    @{$this_section->{bin}}, $elem->[3];
            push    @{$this_section->{bin}}, $elem->[4];
        }
        
        if($elem->[0] eq "LEAF_ARR"){

            $anon_section = "ANON_$anon_section_num";
            $anon_section_num++;

            $sections->{$anon_section}{obj} = $elem;

            push @{$sections->{$anon_section}{bin}}, 0;
            $sections->{$anon_section}{bin}[0] 
                = assemble_leaf($sections, $sections->{$anon_section})  * $MWORD_SIZE;

            push @{$this_section->{bin}}, $anon_section;

            $anon_section_num++;
            $count++;
            next;

        }

        if($elem->[0] eq "HASH_REF"){

            $anon_section = "ANON_$anon_section_num";
            $anon_section_num++;

            $sections->{$anon_section}{obj} = $elem;

            assemble_hash_ref($sections->{$anon_section});

#            push @{$sections->{$anon_section}{bin}}, 0;
#            $sections->{$anon_section}{bin}[0] 
#                = assemble_leaf($sections, $sections->{$anon_section})  * $MWORD_SIZE;
#
            push @{$this_section->{bin}}, $anon_section;

            $anon_section_num++;
            $count++;
            next;

        }

        if($elem->[0] eq "LABEL"){

            assemble($sections, $sections->{$elem->[1]});
            push @{$this_section->{bin}}, "$elem->[1]";
            $count++;
            next;

        }

#        if($elem->[0] eq "EXP_LABEL"){
#
#            if($sections->{$elem->[1]}{obj}[0] ne "INTERIOR_ARR"){
#                print "Expected interior array\n";
#                die;
#            }
#
#            assemble($sections, $sections->{$elem->[1]});
#            my @temp = @{$sections->{$elem->[1]}{bin}};
#            push @{$this_section->{bin}}, splice(@temp, 1);
#            $count += abs($sections->{$elem->[1]}{bin}[0]) / $MWORD_SIZE;
#            next;
#
#        }

    }

    return $count;

}


#########################################################################
#
# LINKER
#
#########################################################################

sub linkit{

    my $sections = shift;
    my $curr_section = shift;
    my $obj_out = shift;
    my $curr_ptr = shift;
#    my $end_ptr = $curr_ptr + abs($sections->{$curr_section}{bin}[0])/$MWORD_SIZE + 1;
    my $end_ptr;

#    if(! defined $sections->{$curr_section}{bin}[0]){
#        print "$curr_section\n";
#    }

    if($sections->{$curr_section}{bin}[0] != 0){
        $end_ptr = $curr_ptr + abs($sections->{$curr_section}{bin}[0])/$MWORD_SIZE + 1;
    }
    else{
        $end_ptr = $curr_ptr + $HASH_SIZE + 1;
    }

    if(!defined $sections->{$curr_section}{obj_ptr}){
        $sections->{$curr_section}{obj_ptr} = $curr_ptr+1;
    }
    else{
        return $curr_ptr;
    }

    for (@{$sections->{$curr_section}{bin}}){
        if($_ =~ /^(\s*-?[1-9][0-9]*)/ or $_ =~ /^(\s*0+)[^x]/ or $_ eq '0' ){
            $obj_out->[$curr_ptr] = $_;
            $curr_ptr++;
            next;
        }
        #print("$_\n");
        # if a name, recurse linkit
#        if( /[_A-Za-z][_A-Za-z0-9]*/ ){
        if( "$_" =~ /$section_name/ ){
            if(!exists $sections->{$_}){
                print "Unresolved symbol: $_\n";
                die;
            }
            $end_ptr = linkit($sections, $_, $obj_out, $end_ptr);
            $obj_out->[$curr_ptr] = $sections->{$_}{obj_ptr} * $MWORD_SIZE;
        }
        else{
            $obj_out->[$curr_ptr] = $_;
        }
#        printf("%02x %08x\n", $curr_ptr, $obj_out->[$curr_ptr]);
        $curr_ptr++;
    }

    return $end_ptr;

}

#########################################################################
#
# OUTPUT
#
#########################################################################

sub send_obj{

    my $proj_name = shift;
    my $obj_out = shift;
    my $sections = shift;

    open C_FILE,   ">${proj_name}.c";
    open LST_FILE, ">${proj_name}.lst";
    open BBL_FILE, ">${proj_name}.bbl";
    binmode BBL_FILE;

    printf C_FILE ("#define BBL_SIZE %d\nunsigned bbl[BBL_SIZE] = {", $#{$obj_out} + 1);

    my $i = 0;
    for(0..$#{$obj_out}){
        if($i % 8 == 0){
            print C_FILE "\n    ";
        }
        printf   C_FILE ("0x%08x, ",                      $obj_out->[$_] & 0xffffffff);
        printf LST_FILE ("%04x %08x\n", $_ * $MWORD_SIZE, $obj_out->[$_] & 0xffffffff);
        print  BBL_FILE pack("V",  $obj_out->[$_]);
        $i++;
    }

    print C_FILE "\n};\n";

    for(keys %{$sections}){
        next if /^ANON_/;
        if(defined $sections->{$_}{obj_ptr}){
            my $op = $_;
            $op =~ s/%/%%/g;
#            printf("$op\n");
            printf LST_FILE ("%08x $op\n", $sections->{$_}{obj_ptr} * $MWORD_SIZE);
        }
    }

    close LST_FILE;
    close BBL_FILE;

}



#########################################################################
#
# PARSER
#
#########################################################################

sub section_parse{

    my $section = shift;
    my $new_array_ref;
    my $token;
    my $sigil;
    my @hash;

    my $src = $section->{src};
    $section->{obj} = [];

#    my $current_context;

    $token = is_numeric($section);
    if(defined $token){
        $section->{obj} = ["LEAF_ARR", ["NUMERIC", 0+$token]];
        # TODO: write is_whitespace() to check for syntax errors
        return;
    }

    $token = is_string($section);
    if(defined $token){
        $section->{obj} = ["LEAF_ARR", ["STRING", "$token"]];
        # TODO: write is_whitespace() to check for syntax errors
        return;
    }

    ($token, $sigil) = is_label_sigil($section);
    if(defined $token){
        if(    $sigil eq "LEX_SUB"      
            or $sigil eq "DIRECT_REF"   
            or $sigil eq "INDIRECT_REF" 
            or $sigil eq "AUTO_EVAL" ){
            print "Syntax error\n";
            die;
        }
        elsif($sigil eq  "HASH_REF"){
            @hash = bytes_to_mwords(Hash::Pearson16::pearson16_hash($token));
            $section->{obj} = ["HASH_REF", @hash];
        }
        else{
            print "Syntax error\n";
            die;
#            $section->{obj} = ["ALIAS", $token];
        }
        return;
    }

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

    if(is_interior_array_begin($section)){
#        $section->{typ} = "interior_arr";
        $section->{obj} = ["INTERIOR_ARR"];
        interior_array($section, $section->{obj});
        return;
    }

    if(is_leaf_array_begin($section)){
#        $section->{typ} = "leaf_arr";
        $section->{obj} = ["LEAF_ARR"];
        leaf_array($section, $section->{obj});
        return;
    }

    if(is_list_begin($section)){
#        die "list detected\n";
        $section->{obj} = ["INTERIOR_ARR"];
        list_interior_array($section, $section->{obj},0);
        return;
    }

    print "Syntax error\n";
    die;

}

sub leaf_array{

    my $section = shift;
    my $obj = shift;

#    print Dumper($src), die;

#    my $current_context;

    my $label;
    my $token;

    while(1){

        if(is_leaf_array_end($section)){
            last;
        }

#        $label = is_expanded_label($section);
#        if(defined $label){
#            push @{$obj}, ["EXP_LABEL", "$label"];
#            next;
#        }

#        $label = is_addr_label($section);
#        if(defined $label){
#            push @{$obj}, ["ADDR_LABEL", "$label"];
#            next;
#        }

        $token = is_numeric($section);
        if(defined $token){
            push @{$obj}, ["NUMERIC", 0+$token];
            next;
        }

        $token = is_string($section);
        if(defined $token){
            $token = '$token = "' . $token . '"';
            eval($token);
            push @{$obj}, ["STRING", "$token"];
            next;
        }

        #       $current_context = current_context();
        #die "Syntax error: $current_context\n";
        print "Syntax error: " . substr($section->{src}, $section->{ptr}) . "\n";
        print Dumper($section);
        die;

    }

}

sub interior_array{

    my $section = shift;
    my $obj = shift;
    my $token;

#    my $current_context;
    my $new_array_ref;

    my $label;
    my @hash;
    my $sigil;

    while(1){

        if(is_interior_array_end($section)){
            last;
        }

        if(is_interior_array_begin($section)){
            $new_array_ref = ["INTERIOR_ARR"];
            interior_array($section, $new_array_ref);
            push @{$obj}, $new_array_ref;
            next;
        }

        if(is_leaf_array_begin($section)){
            $new_array_ref = ["LEAF_ARR"];
            leaf_array($section, $new_array_ref);
            push @{$obj}, $new_array_ref;
            next;
        }

        if(is_list_begin($section)){
    #        die "list detected\n";
            $new_array_ref = ["INTERIOR_ARR"];
            list_interior_array($section, $new_array_ref,0);
            push @{$obj}, $new_array_ref;
            next;
        }


        ($token, $sigil) = is_label_sigil($section);
        if(defined $token){
            if(    $sigil eq "LEX_SUB"      
                or $sigil eq "DIRECT_REF"   
                or $sigil eq "INDIRECT_REF" 
                or $sigil eq "AUTO_EVAL" ){
                print "Syntax error\n";
                die;
            }
            elsif($sigil eq  "HASH_REF"){
                @hash = bytes_to_mwords(Hash::Pearson16::pearson16_hash($token));
                push @{$obj}, ["HASH_REF", @hash];
            }
            else{
                push @{$obj}, ["LABEL", "$token"];
            }
            next;
        }

        $token = is_string($section);
        if(defined $token){
            $token = '$token = "' . $token . '"';
            eval($token);
            push @{$obj}, ["STRING", "$token"];
            next;
        }

        $token = is_numeric($section);
        if(defined $token){
            push @{$obj}, ["NUMERIC", 0+$token];
            next;
        }

        @hash = is_hash_ref($section);
        if($#hash > -1){
            push @{$obj}, ["HASH_REF", @hash];
#my $j;
#for($j=$#hash;$j>=0;$j--){
#    printf("%02x", $hash[$j]);
#}
#print "\n";
#die;
            next;
        }

        $label = is_label($section);
        if(defined $label){
            push @{$obj}, ["LABEL", "$label"];
            next;
        }

#        $label = is_expanded_label($section);
#        if(defined $label){
#            push @{$obj}, ["EXP_LABEL", "$label"];
#            next;
#        }

        print "Syntax error\n";
        die;

    }

}


sub list_interior_array{

    my $section = shift;
    my $obj = shift;
    my $quick_wrap = shift;
    my $token;

#    my $current_context;
    my $new_array_ref;

    my $label;

#    if(is_quick_wrap($section)){
#        $new_array_ref = ["INTERIOR_ARR"];
#        list_interior_array($section, $new_array_ref,1);
#        push @{$obj}, $new_array_ref;
#        goto DONE;
#    }
    
    if(is_list_begin($section)){
        $new_array_ref = ["INTERIOR_ARR"];
        list_interior_array($section, $new_array_ref,0);
        push @{$obj}, $new_array_ref;
        goto DONE;
    }

    if(is_interior_array_begin($section)){
        $new_array_ref = ["INTERIOR_ARR"];
        interior_array($section, $new_array_ref);
        push @{$obj}, $new_array_ref;
        goto DONE;
    }

    if(is_leaf_array_begin($section)){
        $new_array_ref = ["LEAF_ARR"];
        leaf_array($section, $new_array_ref);
        push @{$obj}, $new_array_ref;
        goto DONE;
    }

    my @hash = is_hash_ref($section);
    if($#hash > -1){
        push @{$obj}, ["HASH_REF", @hash];
#my $j;
#for($j=$#hash;$j>=0;$j--){
#    printf("%02x", $hash[$j]);
#}
#print "\n";
#die;
        goto DONE;
    }

    $token = is_numeric($section);
    if(defined $token){
        push @{$obj}, ["NUMERIC", 0+$token];
        goto DONE;
    }

    $label = is_label($section);
    if(defined $label){
        push @{$obj}, ["LABEL", "$label"];
        goto DONE;
    }

#    $label = is_expanded_label($section);
#    if(defined $label){
#        push @{$obj}, ["EXP_LABEL", "$label"];
#        goto DONE;
#    }

    $token = is_string($section);
    if(defined $token){
        $token = '$token = "' . $token . '"';
        eval($token);
        push @{$obj}, ["STRING", "$token"];
        goto DONE;
    }

DONE:

    if(is_list_end($section) or $quick_wrap ){
        push @{$obj}, ["LABEL","nil"];
        return;
    }

    $new_array_ref = ["INTERIOR_ARR"];
    list_interior_array($section, $new_array_ref,0);
    push @{$obj}, $new_array_ref;

}

sub is_leaf_array_begin{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#    $string =~ /^(\s*\{)/;
    
    if($string =~ /^(\s*\{)/){
        $section->{ptr} += length $1;
        return 1;
    }

    return 0;

}

sub is_leaf_array_end{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

    if($string =~ /^(\s*\})/){
        $section->{ptr} += length $1;
        return 1;
    }

    return 0;
}

sub is_interior_array_begin{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#    $string =~ /^(\s*\()/;
    
    if($string =~ /^(\s*\[)/){
        $section->{ptr} += length $1;
        return 1;
    }

    return 0;

}

sub is_interior_array_end{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#    $string =~ /^(\s*\))/;
    
    if($string =~ /^(\s*\])/){
        $section->{ptr} += length $1;
        return 1;
    }

    return 0;

}

#sub is_expanded_label{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
##    $string =~ /^(\s*\*[A-Za-z_][A-Za-z_0-9]*)/;
#    
##    if($string =~ /^(\s*\*[A-Za-z_][A-Za-z_0-9]*)/){
#    if($string =~ /^(\s*\*$section_name)/){
#        $section->{ptr} += length $1;
##        $string =~ /^\s*\*([A-Za-z_][A-Za-z_0-9]*)/;
#        $string =~ /^\s*\*($section_name)/;
#        return $1;
#    }
#
#    return undef;
#
#}

#sub is_addr_label{
#
#    my $section = shift;
#
#    my $string = substr($section->{src}, $section->{ptr});
#
##    $string =~ /^(\s*\*[A-Za-z_][A-Za-z_0-9]*)/;
#    
##    if($string =~ /^(\s*&[A-Za-z_][A-Za-z_0-9]*)/){
#    if($string =~ /^(\s*&$section_name)/){
#        $section->{ptr} += length $1;
##        $string =~ /^\s*&([A-Za-z_][A-Za-z_0-9]*)/;
#        $string =~ /^\s*&($section_name)/;
#        return $1;
#    }
#
#    return undef;
#
#}

sub is_label_sigil{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});
    my $sigil = "";
    my $label;

#   $string =~ /^(\s*[A-Za-z_][A-Za-z_0-9]*)/;
    
#    if($string =~ /^(\s*[A-Za-z_][A-Za-z_0-9]*)/){
    if($string =~ /^(\s*${section_name}$sigil_re?)/){

#        print "$1\n";

        $section->{ptr} += length $1;

        $string =~ /^\s*($section_name)($sigil_re?)/;
        $label = $1;

        $sigil = sigil_type($2) if defined $2;


        return ($label, $sigil);

    }

    return undef;

}

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

sub is_hash_ref{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#   $string =~ /^(\s*[A-Za-z_][A-Za-z_0-9]*)/;
#   print substr($string,0,10) . "\n";

#    if($string =~ /^(\s*[A-Za-z_][A-Za-z_0-9]*&)/){
    if($string =~ /^(\s*$section_name&)/){
        $section->{ptr} += length $1;
#        $string =~ /^\s*([A-Za-z_][A-Za-z_0-9]*)&/;
        $string =~ /^\s*($section_name)&/;
        return bytes_to_mwords(Hash::Pearson16::pearson16_hash($1));
    }

    return ();

}

sub bytes_to_mwords{

    my @bytes = @_;
    my @mwords;
    my $temp=0;

    for(my $i=0; $i<=$#bytes; $i++){
#        printf("%02x ", $bytes[$i]);
        if( (($i % $MWORD_SIZE) == 0) and ($i != 0) ){
            push @mwords, $temp;
            $temp=0;
        }
        $temp = $temp | ( $bytes[$i] << (($i % $MWORD_SIZE) * 8) ); #8 bits per byte
    }
    push @mwords, $temp;
#    printf("%08x ", $_) for @mwords;
#    die;

    return @mwords;

}

sub is_numeric{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

    if($string =~ /^(\s*-?[1-9][0-9]*)/ or $string =~ /^(\s*0+)[^x]/ ){
        $section->{ptr} += length $1;
        $string =~ /^\s*(-?[0-9]+)/;
        return 0+$1;
    }

    if($string =~ /^(\s*0x[A-Fa-f0-9]+)/){
        $section->{ptr} += length $1;
        $string =~ /^\s*0x([A-Fa-f0-9]+)/;
        return hex($1);
    }

    if($string =~ /^(\s*0\b)/){
        $section->{ptr} += length $1;
        return 0;
    }

    return undef;

}

sub is_string{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#    $string =~ /^(\s*"[^"]*")/;
    
    if($string =~ /^(\s*"[^"]*")/){
        $section->{ptr} += length $1;
        $string =~ /^\s*"([^"]*)"/;
        return "$1";
    }
    elsif($string =~ /^(\s*'[^']*')/){
        $section->{ptr} += length $1;
        $string =~ /^\s*'([^']*)'/;
        return "$1";
    }

    return undef;

}

sub is_quick_wrap{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#    $string =~ /^(\s*\{)/;
    
    if($string =~ /^(\s*`)/){
        $section->{ptr} += length $1;
        return 1;
    }

    return 0;
}

sub is_list_begin{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#    $string =~ /^(\s*\{)/;
    
    if($string =~ /^(\s*\()/){
        $section->{ptr} += length $1;
        return 1;
    }

    return 0;

}

sub is_list_end{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#    $string =~ /^(\s*\{)/;
    
    if($string =~ /^(\s*\))/){
        $section->{ptr} += length $1;
        return 1;
    }

    return 0;

}

sub sigil_type {
    for(shift){
        /\*/ and return "LEX_SUB";
        /\$/ and return   "DIRECT_REF";
        /&/  and return "INDIRECT_REF";
        /%/  and return "HASH_REF";
        /!/  and return "AUTO_EVAL";
    }
}

sub str2vec {

    my $str = shift;

    my $index = 0;
    my $my_len;
    my @vec;

    while(1){
        $my_len = length( substr( $str, $index ) );
        if($my_len == 0){
#            push @vec, 0xffffffff;
            push @vec, 0x00000000;
            last;
        }
        elsif($my_len == 1){
            push @vec, ord( substr( $str, $index, 1 ) );
            push @vec, 0xffffff00;
#            push @vec, 0x000000ff;
            last;
        }
        elsif($my_len == 2){
            push @vec, 
                (
                    ord( substr( $str, $index, 1 ) ) 
                    |
                    (ord( substr( $str, $index+1, 1 ) ) << 8)
                )
                ;
            push @vec, 0xffff0000;
#            push @vec, 0x0000ffff;
            last;
        }
        elsif($my_len == 3){
            push @vec, 
                (
                    ord( substr( $str, $index, 1 ) ) 
                    |
                    (ord( substr( $str, $index+1, 1 ) ) << 8)
                    |
                    (ord( substr( $str, $index+2, 1 ) ) << 16)
                )
                ;
            push @vec, 0xff000000;
#            push @vec, 0x00ffffff;
            last;
        }
        else{
            push @vec, 
                (
                    ord( substr( $str, $index, 1 ) ) 
                    |
                    (ord( substr( $str, $index+1, 1 ) ) << 8)
                    |
                    (ord( substr( $str, $index+2, 1 ) ) << 16)
                    |
                    (ord( substr( $str, $index+3, 1 ) ) << 24)
                )
                ;
            $index+=4;
        }

    }

    return @vec;

}


