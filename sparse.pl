#!/usr/bin/perl.exe -w

# s-expression parser (sparse) - A low-level "assembler" for Babel constructs

# TODO: Empty lists, bare values, catch non-existent symbols

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

#Values:        ( j ... )
#Pointers:      ( k ... )
#Tagged-List:   ( q ... )
#List:          ( x ... )
#Cosntant:      ( z ... )

$asm_file = clean     ( \@asm_file );
            $sections = balanced_parse( \$asm_file );
            tokenize($sections);
            my $obj = encode($sections);

#print Dumper($sections);
#print Dumper($obj);
#dump_obj($obj->{root}{code});

create_lst($proj_name, $obj->{root}{code}, $obj);
create_bbl($proj_name, $obj->{root}{code});

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

            push @{$expression}, tokenize($non_array_context);
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

#
#
sub tokenize{

    my $string = shift;
    my $eval_string = "";
    my $temp_string = "";

    my @token_list = ();

    $string =~ s/^\s*//;

    while(length($string) > 0){
        if($string =~ /^\s*"[^"]*"\s*/){
            $string =~ s/^\s*("[^"]*")\s*//;
            $eval_string = "\$temp_string = $1;";
            die unless eval($eval_string);
            push @token_list, "\"$temp_string\"";
        }
        elsif($string =~ /^\s*'[^']*'\s*/){
            $string =~ s/^\s*('[^']*')\s*//;
            push @token_list, "$1";
        }
        elsif($string =~ /^\s*[\S]+\s*/){
            $string =~ s/^\s*([\S]+)\s*//;
            push @token_list, "$1";
        }
        else{
            print "Error: .$string.\n" and die;
        }
        $string =~ s/^\s*//;
    }

    return @token_list;

}

#########################################################################
#
# ENCODER
#
#########################################################################

#
#
sub encode{

    my $parse_tree = shift;

    my $symbol_table = {};
    my $obj = {};

    # build symbol-table
    foreach my $named_section (@{$parse_tree}){
        $symbol_table->{$named_section->[0]} = $named_section->[1];
    }

    #print Dumper($symbol_table) and die;

    encode_section($symbol_table, $obj, 'root', 0);

    return $obj;

}

#
#
sub encode_section{

    my ($symbol_table, 
        $obj, 
        $section_name, 
        $offset) = @_;

    if(exists $obj->{$section_name}){
        return;
    }
    else{
        $obj->{$section_name}{code} = [];
        $obj->{$section_name}{addr} = $offset+1;
        encode_tree($symbol_table, $obj, $symbol_table->{$section_name}, $obj->{$section_name}{code}, $offset);
    }

}

#
#
sub encode_tree{

    my ($symbol_table, 
        $obj, 
        $symbol_section, 
        $obj_section, 
        $offset) = @_;

    if   ($symbol_section->[0] eq 'j'){ #values
        encode_values($symbol_table, $obj, $symbol_section, $obj_section);
    }
    elsif($symbol_section->[0] eq 'k'){ #pointers
        encode_pointers($symbol_table, $obj, $symbol_section, $obj_section, $offset);
    }
    elsif($symbol_section->[0] eq 'q'){ #tagged-list
        encode_tagged_list($symbol_table, $obj, $symbol_section, $obj_section, $offset);
    }
    elsif($symbol_section->[0] eq 'x'){ #list
        encode_list($symbol_table, $obj, $symbol_section, $obj_section, $offset);
    }
    elsif($symbol_section->[0] eq 'z'){ #constant
    }
    else{
        die;
    }

}

#
#
sub encode_values{

    my ($symbol_table, 
        $obj, 
        $symbol_section, 
        $obj_section) = @_;

    my $eval_string;
    my @str_vec;

    my @values = @{$symbol_section};
    shift @values;

    my @value_list = ();

    foreach my $value (@values){
        if($value =~ /^(\s*-?[1-9][0-9]*)$/ or $value =~ /^(\s*0+)[^x]$/ ){
            #$branch =~ s/^\s*(-?[0-9]+)//;
            push @value_list, 1+$1-1; #FORCE Perl to treat it as numeric
        }
        elsif($value =~ /^(\s*0x[A-Fa-f0-9]+)$/){
            #$branch =~ s/^\s*0x([A-Fa-f0-9]+)//;
            push @value_list, hex($1);
        }
        elsif($value =~ /^\s*"[^"]*"$/){
            $eval_string = "\$value = $value;";
            die unless eval($eval_string);
            $value = "\"$value\"";
            @str_vec = str2vec($value);
            push @value_list, @str_vec;
        }
        elsif($value =~ /^\s*'([^']*)'$/){
            @str_vec = str2vec($value);
            push @value_list, @str_vec;
        }
        elsif($value =~ /^($section_name)$/){
            my $label = $1;
            unless(exists $obj->{$label}){
                encode_section($symbol_table, $obj, $label, 0);
            }
            my @temp = @{$obj->{$label}{code}};
            shift @temp;
            push @value_list, @temp; #$obj_section->[$first_pointer+$i] = $obj->{$pointer}{addr};            
        }
        else{
            print "Error: .$value.\n" and die;
        }            
    }

    unshift @value_list, $MWORD_SIZE*($#value_list+1);

    push @{$obj_section}, @value_list;

}

#
#
sub encode_pointers{

    my ($symbol_table, 
        $obj, 
        $symbol_section, 
        $obj_section, 
        $offset) = @_;

    my @pointers = @{$symbol_section};
    shift @pointers;

    push @{$obj_section}, -1*$MWORD_SIZE*($#pointers+1);

    my $first_pointer = $#{$obj_section}+1;
    my $i = 0;

    #my @temp = (0) x $#pointers+1;
    #push @{$obj_section}, @temp;
    for(0..$#pointers){
        push @{$obj_section}, 0xdeadbeef;
    }

    foreach my $pointer (@pointers){

        if(ref($pointer) eq ""){
            #value =~ /^$section_name$/
            if(exists $obj->{$pointer}){
                $obj_section->[$first_pointer+$i] = $obj->{$pointer}{addr};
            }
            else{
                #$obj->{$pointer}{code} = [];
                #$obj->{$pointer}{addr} = $#{$obj_section}+1;
                #encode_tree($symbol_table, $obj, $symbol_table->{$pointer}, $obj->{$pointer}{code}, $#{$obj_section}+1);
                encode_section($symbol_table, $obj, $pointer, $offset+$#{$obj_section}+1);
                push @{$obj_section}, @{$obj->{$pointer}{code}};
                $obj_section->[$first_pointer+$i] = $obj->{$pointer}{addr};
            }
        }
        else{
            $obj_section->[$first_pointer+$i] = $offset+$#{$obj_section}+2;
            encode_tree($symbol_table, $obj, $pointer, $obj_section, $#{$obj_section}+1);
        }
        $i++;

    }

}

#
#
sub encode_list{

    my ($symbol_table, 
        $obj, 
        $symbol_section, 
        $obj_section, 
        $offset) = @_;

    my ($car, $cdr);

    my @elements = @{$symbol_section};
    shift @elements;

    #if $#elements=0

    foreach my $element (@elements){

        push @{$obj_section}, -2*$MWORD_SIZE;
        for(1..2){
            push @{$obj_section}, 0xdeadbeef;
        }

        $car = $#{$obj_section}-1;
        $cdr = $#{$obj_section};

        if(ref($element) eq ""){
            if(exists $obj->{$element}){
                $obj_section->[$car] = $obj->{$element}{addr};
            }
            else{
                encode_section($symbol_table, $obj, $element, $offset+$#{$obj_section}+1);
                push @{$obj_section}, @{$obj->{$element}{code}};
                $obj_section->[$car] = $obj->{$element}{addr};
            }
        }
        else{
            $obj_section->[$car] = $offset+$#{$obj_section}+2;
            encode_tree($symbol_table, $obj, $element, $obj_section, $offset);
        }
        $obj_section->[$cdr] = $offset+$#{$obj_section}+2;

    }

    if(!exists $obj->{nil}){
        create_nil($obj, $obj_section, $offset);
        #$obj_section->[$car] = $obj->{$element}{addr};
    }

    $obj_section->[$cdr] = $obj->{nil}{addr};

}

#
#
sub encode_tagged_list{

    my ($symbol_table, 
        $obj, 
        $symbol_section, 
        $obj_section, 
        $offset) = @_;

    my ($car, $cdr);

    my @elements = @{$symbol_section};
    shift @elements;
    my $plain_tag = shift @elements;
    my $tag;

    push @{$obj_section}, 0;

    if($plain_tag =~ /^\s*"([^"]*)"$/ or $plain_tag =~ /^\s*'([^']*)'$/){
        push @{$obj_section}, bytes_to_mwords(Hash::Pearson16::pearson16_hash($1));
    }
    else{
        print "Error: .$plain_tag.\n" and die;
    }

    foreach my $element (@elements){

        push @{$obj_section}, -2*$MWORD_SIZE;
        for(1..2){
            push @{$obj_section}, 0xdeadbeef;
        }

        $car = $#{$obj_section}-1;
        $cdr = $#{$obj_section};

        if(ref($element) eq ""){
            if(exists $obj->{$element}){
                $obj_section->[$car] = $obj->{$element}{addr};
            }
            else{
                encode_section($symbol_table, $obj, $element, $offset+$#{$obj_section}+1);
                push @{$obj_section}, @{$obj->{$element}{code}};
                $obj_section->[$car] = $obj->{$element}{addr};
            }
        }
        else{
            $obj_section->[$car] = $offset+$#{$obj_section}+2;
            encode_tree($symbol_table, $obj, $element, $obj_section, $offset);
        }
        $obj_section->[$cdr] = $offset+$#{$obj_section}+2;

    }

    if(!exists $obj->{nil}){
        create_nil($obj, $obj_section, $offset);
        #$obj_section->[$car] = $obj->{$element}{addr};
    }

    $obj_section->[$cdr] = $obj->{nil}{addr};

}

sub create_nil{

    my ($obj, $obj_section, $offset) = @_;
    #print Dumper($obj) and die;

    $obj->{nil}{addr} = $offset+$#{$obj_section}+2;

    #add nil
    #e974b23a 71cf647b 8c2f644d 3023f4e7
    push @{$obj_section}, (0, 0x3023f4e7, 0x8c2f644d, 0x71cf647b, 0xe974b23a, -2*$MWORD_SIZE, $offset+$#{$obj_section}+2, $offset+$#{$obj_section}+2);

}


#########################################################################
#
# FILE OUTPUT
#
#########################################################################

#
#
sub create_lst{

    my $lst_file = shift;
    my $obj_root = shift;
    my $obj = shift;
    my $i = 0;

    open LST_FILE, ">${lst_file}.lst" or die "Couldn't create .lst: $!";

    for my $word (@{$obj_root}) {
        printf LST_FILE ("%04x %08x\n", $i++, $word);
    }

    foreach (sort keys %{$obj}){
        if(defined $obj->{$_}{addr}){
#            my $op = $_;
#            $op =~ s/%/%%/g;
##            printf("$op\n");
            printf LST_FILE ("%08x $_\n", $obj->{$_}{addr});
        }
    }


    close LST_FILE;

}

#
#
sub create_bbl{

    my $bbl_file = shift;
    my $obj_root = shift;

    open BBL_FILE, ">${bbl_file}.bbl" or die "Couldn't create .bbl: $!";
    binmode BBL_FILE;

    for(@{$obj_root}){
        print  BBL_FILE pack("V",  $_);
    }

    close BBL_FILE;

}

#
#
sub create_c{

#    my $c_file = shift;
#    my $obj_root = shift;
#
#    open C_FILE,   ">${c_file}.c" or die "Couldn't create .c: $!";
#    printf C_FILE ("#define BBL_SIZE %d\nunsigned bbl[BBL_SIZE] = {", $#{$obj_out} + 1);
#
#    my $i = 0;
#    for(0..$#{$obj_out}){
#        if($i % 8 == 0){
#            print C_FILE "\n    ";
#        }
#        printf   C_FILE ("0x%08x, ",                      $obj_out->[$_] & 0xffffffff);
#        $i++;
#    }
#
#    print C_FILE "\n};\n";
#    close C_FILE;

}

#########################################################################
#
# UTILITIES
#
#########################################################################

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

sub dump_obj{

    my $obj = shift;
    my $i = 0;
    
    for my $word (@{$obj}) {
        printf("%04x %08x\n", $i++, $word);
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


