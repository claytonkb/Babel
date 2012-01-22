#!/usr/bin/perl.exe -w

use strict;
use feature 'state';
no warnings 'recursion'; # Get "deep recursion" on large .pb files otherwise
use Hash::Pearson16;

#my @my_hash = Hash::Pearson16::pearson16_hash($ARGV[0]);
#my $j;
#for($j=0;$j<=$#my_hash;$j++){
#    printf("%02x", $my_hash[$j]);
#}
#print "\n";
#die;

use YAML::XS;
use Data::Dumper;

#open YAML_FILE, shift;
#my $yaml_file = join '', <YAML_FILE>;
#close YAML_FILE;

#my $instrs = Load($yaml_file); # Loads the YAML file to a Perl data structure...

my @asm_file;
my $asm_file;
my $obj_out = [];

#my $MWORD_SIZE = 8;    # 64-bit mode
my $MWORD_SIZE = 4;

my $proj_name = $ARGV[0];

while($#ARGV>-1){
    $asm_file = shift @ARGV;
    open ASM_FILE, $asm_file;
    push @asm_file, <ASM_FILE>;
    close ASM_FILE;
}

                    clean           ( \@asm_file );
my $sections =      get_sections    ( \@asm_file );
                    section_parse   ( $sections->{$_}) for (keys %{$sections});
#print Dumper($sections) and die;

                    assemble        ( $sections, $sections->{'main'} );
                    linkit          ( $sections, 'main', $obj_out, 0 );
                    send_obj        ( $proj_name, $obj_out, $sections);

#print Dumper($sections);

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
            printf LST_FILE ("%08x $_\n", $sections->{$_}{obj_ptr} * $MWORD_SIZE);
        }
    }

    close LST_FILE;
    close BBL_FILE;

}

sub clean{
    my $asm_file = shift;
    remove_comments($asm_file);
    remove_ws      ($asm_file);
}

sub linkit{

    my $sections = shift;
    my $curr_section = shift;
    my $obj_out = shift;
    my $curr_ptr = shift;
    my $end_ptr = $curr_ptr + abs($sections->{$curr_section}{bin}[0])/$MWORD_SIZE + 1;

    if(!defined $sections->{$curr_section}{obj_ptr}){
        $sections->{$curr_section}{obj_ptr} = $curr_ptr+1;
    }
    else{
        return $curr_ptr;
    }

    for (@{$sections->{$curr_section}{bin}}){
        # if a name, recurse linkit
        if( /[_A-Za-z][_A-Za-z0-9]*/ ){
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
    else{
        push @{$this_section->{bin}}, 0;
        $this_section->{bin}[0] = (-1 * assemble_interior($sections, $this_section) * $MWORD_SIZE);
    }

    $this_section->{asmd} = 1;

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

        if($elem->[0] eq "EXP_LABEL"){

            if($sections->{$elem->[1]}{obj}[0] ne "LEAF_ARR"){
                print "Expected leaf array\n";
                die;
            }

            unless($sections->{$elem->[1]}{asmd}){
                assemble_leaf($sections, $sections->{$elem->[1]});
            }

            push @{$this_section->{bin}}, @{$sections->{$elem->[1]}{bin}};
            $count += ($#{$sections->{$elem->[1]}{bin}} + 1);

            next;

        }

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

        if($elem->[0] eq "LABEL"){

            assemble($sections, $sections->{$elem->[1]});
            push @{$this_section->{bin}}, "$elem->[1]";
            $count++;
            next;

        }

        if($elem->[0] eq "EXP_LABEL"){

            if($sections->{$elem->[1]}{obj}[0] ne "INTERIOR_ARR"){
                print "Expected interior array\n";
                die;
            }

            assemble($sections, $sections->{$elem->[1]});
            my @temp = @{$sections->{$elem->[1]}{bin}};
            push @{$this_section->{bin}}, splice(@temp, 1);
            $count += abs($sections->{$elem->[1]}{bin}[0]) / $MWORD_SIZE;
            next;

        }

    }

    return $count;

}

sub remove_comments{

    my $asm_file = shift;

    foreach my $line (@{$asm_file}){
        $line =~ s/--.*$//; #Strips a line-comment
    }

}

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

sub get_sections{

    my $asm_file = shift;
    my $sections;
    my $current_section;
    my $match;

    # Create nil for use by lists
    $sections->{'nil'}{src} = [];
    $sections->{'nil'}{ptr} = 0;
    $sections->{'nil'}{asmd} = 0;
    $sections->{'nil'}{bin_ptr} = undef;
    $sections->{'nil'}{bin} = [];
    push @{$sections->{'nil'}{src}}, "[nil nil]";

    for(@{$asm_file}){
        if(/^([a-zA-Z0-9_]+):/){
            $current_section = $1;
            $sections->{$current_section}{src} = [];
            $sections->{$current_section}{ptr} = 0;
            $sections->{$current_section}{asmd} = 0;
            $sections->{$current_section}{bin_ptr} = undef;
            $sections->{$current_section}{bin} = [];

            ($match) = $_ =~ /^[a-zA-Z0-9_]+:(.+)$/;
            if(defined $match){
                push @{$sections->{$current_section}{src}}, $match;
            }
            next;

        }
        push @{$sections->{$current_section}{src}}, $_;
    }

    for(keys %{$sections}){
        $sections->{$_}{src} = join '', @{$sections->{$_}{src}};
    }

    return $sections;

}



##################################################
# Section parser
##################################################

sub section_parse{

    my $section = shift;
    my $new_array_ref;
    my $token;

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
        list_interior_array($section, $section->{obj});
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

        $label = is_expanded_label($section);
        if(defined $label){
            push @{$obj}, ["EXP_LABEL", "$label"];
            next;
        }

        $label = is_addr_label($section);
        if(defined $label){
            push @{$obj}, ["ADDR_LABEL", "$label"];
            next;
        }

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
            list_interior_array($section, $new_array_ref);
            push @{$obj}, $new_array_ref;
            next;
        }

        $label = is_label($section);
        if(defined $label){
            push @{$obj}, ["LABEL", "$label"];
            next;
        }

        $label = is_expanded_label($section);
        if(defined $label){
            push @{$obj}, ["EXP_LABEL", "$label"];
            next;
        }

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

        print "Syntax error\n";
        die;

    }

}


sub list_interior_array{

    my $section = shift;
    my $obj = shift;
    my $token;

#    my $current_context;
    my $new_array_ref;

    my $label;

    if(is_list_begin($section)){
        $new_array_ref = ["INTERIOR_ARR"];
        list_interior_array($section, $new_array_ref);
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

    $label = is_label($section);
    if(defined $label){
        push @{$obj}, ["LABEL", "$label"];
        goto DONE;
    }

    $label = is_expanded_label($section);
    if(defined $label){
        push @{$obj}, ["EXP_LABEL", "$label"];
        goto DONE;
    }

    $token = is_numeric($section);
    if(defined $token){
        push @{$obj}, ["NUMERIC", 0+$token];
        goto DONE;
    }

    $token = is_string($section);
    if(defined $token){
        $token = '$token = "' . $token . '"';
        eval($token);
        push @{$obj}, ["STRING", "$token"];
        goto DONE;
    }

DONE:

    if(is_list_end($section)){
        push @{$obj}, ["LABEL","nil"];
        return;
    }

    $new_array_ref = ["INTERIOR_ARR"];
    list_interior_array($section, $new_array_ref);
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

sub is_expanded_label{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#    $string =~ /^(\s*\*[A-Za-z_][A-Za-z_0-9]*)/;
    
    if($string =~ /^(\s*\*[A-Za-z_][A-Za-z_0-9]*)/){
        $section->{ptr} += length $1;
        $string =~ /^\s*\*([A-Za-z_][A-Za-z_0-9]*)/;
        return $1;
    }

    return undef;

}

sub is_addr_label{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#    $string =~ /^(\s*\*[A-Za-z_][A-Za-z_0-9]*)/;
    
    if($string =~ /^(\s*&[A-Za-z_][A-Za-z_0-9]*)/){
        $section->{ptr} += length $1;
        $string =~ /^\s*&([A-Za-z_][A-Za-z_0-9]*)/;
        return $1;
    }

    return undef;

}

sub is_label{

    my $section = shift;

    my $string = substr($section->{src}, $section->{ptr});

#   $string =~ /^(\s*[A-Za-z_][A-Za-z_0-9]*)/;
    
    if($string =~ /^(\s*[A-Za-z_][A-Za-z_0-9]*)/){
        $section->{ptr} += length $1;
        $string =~ /^\s*([A-Za-z_][A-Za-z_0-9]*)/;
        return $1;
    }

    return undef;

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

    return undef;

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

