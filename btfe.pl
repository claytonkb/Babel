#!/usr/bin/perl.exe -w

#Babel Front-End in Perl
package Babel::FrontEnd;

use strict;
use Carp;
use feature 'state';
no warnings 'recursion'; # Get "deep recursion" on large .pb files otherwise
use Hash::Pearson16;

use YAML::XS;
use Data::Dumper;
#use File::Spec;

my @asm_file;
my $asm_file;
my $obj_out = [];

#my $MWORD_SIZE = 8;    # 64-bit mode
my $MWORD_SIZE = 4;
my $HASH_SIZE = 16/$MWORD_SIZE;

my $proj_name = $ARGV[0];

#my $section_name = qr/[_A-Za-z\+-<>=!~#%\.\$\?\/\|^][_A-Za-z0-9\+-<>=!~#%\.\$\?\/\|^]*/;

my $section_name   = qr/[_A-Za-z][_A-Za-z0-9]*/;

#my $section_name   = qr/ (?: \/ | \.\/ | \.\.\/ ) 
#                         (?: (?: [^\/\*\$&%!#@] | \\[^\/] )+ \/? )+ /x;
my $sigil_re       = qr/[\*\$&%!#@]/;
my $array_begin  = qr/[\[\({<]/;
my $array_end    = qr/[\]\)}>]/;
my $non_array    = qr/[^\[\({<\]\)}>]/;

#unshift @ARGV, "pb/opcodes.pb";

my $sections = {};

#while($#ARGV>-1){
    $asm_file = shift @ARGV;
    open ASM_FILE, $asm_file;
#    push @asm_file, <ASM_FILE>;
    @asm_file = <ASM_FILE>;
    close ASM_FILE;
#}

$sections = parse_ntb_file(\@asm_file);

balanced_parse_all_sections($sections);

translate_all_sections($sections);

#chomp foreach (@asm_file);
#$asm_file = join ' ', @asm_file;
#clean_string(\$asm_file);
#$sections = balanced_parse(\$asm_file);

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
# NESTED TEXT BLOCK PARSER
#
#########################################################################

# Each section can, recursively, have a null-context that terminates EITHER
# with further section-definitions or a balanced-expression

sub parse_ntb_file{

    my $ntb_file = shift;

    my $first_line = discard_null_context($ntb_file, 0);

    return if $first_line < 0;

    #trim off the null-context
    @{$ntb_file} = @{$ntb_file}[$first_line..$#{$ntb_file}];

    parse_ntb($ntb_file);

}

sub parse_ntb{

    my $ntb_file = shift;
    my $sections = {};

    my $left_edge = get_dent($ntb_file->[0]);

    my $test_line = undent($ntb_file->[0],$left_edge);

    if(is_label($test_line)){
        gather_sections( $ntb_file, 0, $left_edge, $sections );
        foreach my $section ( keys %{$sections} ){
            my $parsed_sections = parse_ntb($sections->{$section}{text});
            foreach my $parsed_section (keys %{$parsed_sections}){
                $sections->{$section}{$parsed_section} = $parsed_sections->{$parsed_section};
            }
        }
    }
    else{
        gather_text( $ntb_file, 0, $left_edge, $sections );
    }

    return $sections;

}

sub gather_sections{ 
    
    my ($ntb_file, $current_line, $left_edge, $sections) = @_;

    my ($label, $sigil, $remainder);
    my $next_line;
    my $dent_left_edge;

    gather_next_section:

         return (-1, 0) if $current_line > $#{$ntb_file};

#        die if ( !is_label( undent( $ntb_file->[$current_line], $left_edge) ) );

        ($label, $sigil, $remainder) = get_label( undent( $ntb_file->[$current_line], $left_edge ) );

#        print "$label\n" if defined $label;
#        print "$sigil\n" if defined $sigil;

#        if($sigil eq "QUOTE_BLOCK"){
#            print "$label\n" if defined $label;
#            print "$sigil\n" if defined $sigil;
#            die;
#        }

        unless($label eq "" and $sigil eq "QUOTE_BLOCK"){
            $sections->{$label}{sigil} = $sigil if defined $sigil;
            if(defined $remainder and !is_empty($remainder) ){
                die if $sigil eq "QUOTE_BLOCK";
                push(@{$sections->{$label}{text}}, $remainder);
            }
        }

        $next_line = find_next_non_empty_line( $ntb_file, $current_line+1 );
        return (-1, 0) if($next_line < 0);

        $dent_left_edge = get_dent( $ntb_file->[$next_line] );

        if($dent_left_edge == $left_edge){
            $current_line = $next_line;
            goto gather_next_section;
        }

        return (-1, 0) if $dent_left_edge < $left_edge;

        $current_line = $next_line;

        gather_next_line:
            unless($label eq "" and $sigil eq "QUOTE_BLOCK"){
                push @{$sections->{$label}{text}}, undent( $ntb_file->[$current_line], $dent_left_edge );
            }
            
            if($current_line < $#{$ntb_file}){
                if( ( get_dent( $ntb_file->[$current_line+1] ) >= $dent_left_edge )
                    or is_empty( $ntb_file->[$current_line+1] ) ){

                    $current_line++;
                    goto gather_next_line;

                }
            }

        return (-1, 0) if $current_line == $#{$ntb_file}; #EOF

        $current_line = find_next_non_empty_line( $ntb_file, $current_line+1 );
        return (-1, 0) if $current_line < 0;
        goto gather_next_section;

}

sub gather_text{

    my ($ntb_file, $current_line, $left_edge, $sections) = @_;
#    my $next_left_edge;

#    while(  $current_line <= $#{$ntb_file}
#            and ( (   get_dent( $ntb_file->[$current_line] ) >= $left_edge )
#                or    is_empty( $ntb_file->[$current_line] ) ) ){
#            push @{$sections->{text}}, undent( $ntb_file->[$current_line], $left_edge );
#            $current_line++;
#    }
#
#    if($current_line > $#{$ntb_file}){ #EOF
#        print "hereA\n";
#        return (-1, 0);
#    }
#    else{
#        print "hereB\n";
#        return ($current_line, get_dent($ntb_file->[$current_line]) );
#    }
    $sections->{leaf} = 1;

    gather_next_line:
        push @{$sections->{text}}, undent( $ntb_file->[$current_line], $left_edge );
        
        if($current_line < $#{$ntb_file}){
            if( ( get_dent( $ntb_file->[$current_line+1] ) >= $left_edge )
                or is_empty( $ntb_file->[$current_line+1] ) ){

                $current_line++;
                goto gather_next_line;

            }
        }

    return (-1, 0) if $current_line == $#{$ntb_file}; #EOF

}


sub gather_comment{ 

    my ($ntb_file, $current_line, $left_edge) = @_;

    while(  $current_line <= $#{$ntb_file}
            and ( (   get_dent( $ntb_file->[$current_line] ) >= $left_edge )
                or    is_empty( $ntb_file->[$current_line] ) ) ){
        $current_line++;
    }

    if($current_line == $#{$ntb_file}){ #EOF
        return (-1, 0);
    }
    else{
        return ($current_line, get_dent($ntb_file->[$current_line]) );
    }

}

sub find_next_non_empty_line{

    my $ntb_file = shift;
    my $current_line = shift;

    return -1 if $current_line > $#{$ntb_file};

    while( ( is_empty( $ntb_file->[$current_line] ) 
            or is_comment($ntb_file->[$current_line]) )
            and $current_line < $#{$ntb_file} ){
        $current_line++;
    }

    if($current_line == $#{$ntb_file}){
        return -1;
    }

    return $current_line;

}

sub is_empty{ #tabs show up as non-empty!!! 

    my $line = shift;
    carp "blah" if not defined $line;

    return $line =~ /^ *$/;

}

sub is_comment{

    my $line = shift;
    carp "blah" if not defined $line;

    return $line =~ /^ *--/;

}

sub get_dent{

    my $line = shift;
    if(not defined $line){
        carp "blah";
    }
    $line =~ /( *)[^ ]/;
    return length($1) if defined $1;
    return 0;

}

sub get_label{

    my $line = shift;

    my ($label, $sigil, $remainder) 
        = $line =~ /^($section_name?)($sigil_re?) *:(.*)/;

    $sigil = sigil_type($sigil) if defined $sigil;

    return ($label, $sigil, $remainder);

}


sub discard_null_context{

    my $ntb_file = shift;
    my $current_line = shift;

    while(  ( $current_line <= $#{$ntb_file} )
            and ( $ntb_file->[$current_line] !~ /^---/ ) ){
        $current_line++;
    }

    $current_line = find_next_non_empty_line( $ntb_file, $current_line+1 );

    return $current_line;

}

sub is_comment_block_label{
    my $line = shift;
    return 1 if $line =~ /^# *:/;
    return 0;
}

sub is_label{

    my $line = shift;
    return 1 if $line =~ /^${section_name}?$sigil_re? *:/;
    return 0;

}

sub undent{

    my $line = shift;
    my $dent = shift;

    return $line if is_empty( $line );
    return substr( $line, $dent, length($line) );

}

sub sigil_type {
    for(shift){
        /\*/ and return "LEX_SUB";
        /\$/ and return "INDIRECT_REF";
        /&/  and return   "DIRECT_REF";
        /%/  and return "HASH_REF";
        /!/  and return "AUTO_EVAL";
        /#/  and return "QUOTE_BLOCK";
        /@/  and return "LOCATION_LABEL";
    }
}

sub balanced_parse_all_sections{

    my $sections = shift;

    if(exists $sections->{leaf}){       
        my $temp = join ' ', @{$sections->{text}};
        $temp =~ tr{\n}{ }; #FIXME: This may be unnecessary... there's a bug here
        #print "$temp\n";
        $sections->{parsed} = balanced_parse( \$temp ) if $sections->{sigil} ne "QUOTE_BLOCK";
    }
    else{
        foreach my $section (keys %{$sections}){
            next if $section eq "text";
            next if $section eq "sigil";
            balanced_parse_all_sections($sections->{$section});
        }
    }
    
}

sub join_lines { # need to handle line comments and quote-blocks properly
    my $lines = shift;
    $lines = join ' ', @{$lines};
    my ($temp) = $lines =~ s/\n/ /g;
    return $temp;
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
    push @{$expression}, get_paren_type($paren);

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
            push @{$expression}, $non_array_context;
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

sub is_matching_paren{
    my ($open_paren, $close_paren) = @_;
    return 1 if (get_paren_type($open_paren) eq get_paren_type($close_paren));
    return 0;
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

#########################################################################
#
# TRANSLATOR
#
#########################################################################

sub translate_all_sections{

    my $sections = shift;

    if(exists $sections->{leaf}){
        if($sections->{sigil} ne "QUOTE_BLOCK"){
            $sections->{translated} = translate_balanced_expression($sections->{parsed});
        }
    }
    else{
        foreach my $section (keys %{$sections}){
            next if $section eq "text";
            next if $section eq "sigil";
            translate_all_sections($sections->{$section});
        }
    }

}

sub translate_balanced_expression{

    my $expression = shift;
    my $result_expression = [];

    foreach my $element (@{$expression}){
        if( $element eq "LIST_PAREN" ){
            push @{$result_expression}, "LIST";
            next;
        }
        if( $element eq "ARRAY_PAREN" ){
            push @{$result_expression}, "ARRAY";
            next;
        }
        if( $element eq "CODE_LIST_PAREN" ){
            push @{$result_expression}, "CODE_LIST";
            next;
        }
        if( $element eq "BRACKET_PAREN" ){
            push @{$result_expression}, "BRACKET";
            next;
        }

        if(ref($element) eq 'ARRAY'){
            push @{$result_expression}, translate_balanced_expression($element);
        }
        else{
            my $translated_expression = translate($element);
            push(@{$result_expression},$_) foreach(@{$translated_expression});
        }
    }

    return $result_expression;

}

sub translate{

    my $string = shift;
    my $chunks = [];
#    my $string = $section->{text};

    my $chunk = get_next_chunk(\$string);

    while(defined $chunk){
        push @{$chunks}, $chunk;
        $chunk = get_next_chunk(\$string);
    }

    return $chunks;

}

sub get_next_chunk{

    my $string = shift;
    my $chunk;

    retry_get_next_chunk:

        clean_string($string);

        $chunk = chunk_get_label(${$string});
        if(defined $chunk){
            ${$string} = substr(${$string}, length($chunk));
            my ($label, $sigil) = $chunk =~ /($section_name)($sigil_re?)/;

            if(defined $sigil and length($sigil) > 0){
                $sigil = sigil_type($sigil);
            }
            else{
                $sigil = "NONE";
            }
            return ["LABEL", $label, $sigil];
        }

        $chunk = is_string(${$string});
        if(defined $chunk){
            ${$string} = substr(${$string}, length($chunk));
            eval("\$chunk = $chunk;");
            return ["STRING", "$chunk"];
        }

        $chunk = is_numeric(${$string});
        if(defined $chunk){
            ${$string} = substr(${$string}, length($chunk));
            return ["NUMERIC", 0+$chunk];
        }

        if(${$string} =~ /(--.*\n)/){ # discard line comments
            ${$string} = substr(${$string}, length($1));
            goto retry_get_next_chunk;
        }

        return undef;
    
}

sub chunk_get_label{

    my $string = shift;

#my $section_name = qr/[_A-Za-z][_A-Za-z0-9]*/;
#my $sigil_re     = qr/[\*\$&%!#]/;

    my ($label, $sigil) = $string =~ /^(${section_name})($sigil_re?)/;

    return "$label$sigil" if defined $sigil;
    return $label if defined $label;

    return undef;

}

sub is_numeric{

    my $string = shift;

    if($string =~ /^0+\b/){
        return 0;
    }

    if($string =~ /^0x([A-Fa-f0-9]+)\b/){
        return hex($1);
    }

    if($string =~ /^0b([01]+)\b/){
        return 0+bin2dec($1);
    }

    if( $string =~ /^(-?[1-9][0-9]*)\b/ ){
        return 0+$1;
    }

    return undef;

}

sub bin2dec { return unpack("N", pack("B32", substr("0" x 32 . shift, -32))) }

#my $str = '" this is a \" quoted string"';
#if ($str =~ $RE{quoted}) {
#  # do something
#}
sub is_string{

    my $string = shift;

    #/"(?:[^\\"]|\\.)*"/
    #/'(?:[^\\']|\\.)*'/

    if($string =~ /^("(?:[^\\"]|\\.)*")/){
        return "$1";
    }
    elsif($string =~ /^('(?:[^\\']|\\.)*')/){
        return "$1";
    }

    return undef;

}

#########################################################################
#
# DISAMBIGUATION (LEAF/INTERIOR)
#
#########################################################################

# for each array
#     - if it has a number or string in it, it's a leaf-array
#     - if it has any label with a sigil other than * in it, it's an interior-array
#     - if still unresolved
#       - recurse into each label
#       - unless EVERY label is a lex-sub'ing leaf-array, this is an interior-array
#     - any arrays which have not been disambiguated by the above rules are
#       interior arrays

#########################################################################
#
# LEXICAL SUBSTITUTION
#
#########################################################################



#########################################################################
#
# REFERENCE CONNECTION (DIRECT/INDIRECT)
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


