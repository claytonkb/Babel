#!/usr/bin/perl.exe -w

# s-expression parser (sparse) - A low-level "assembler" for Babel constructs

# TODO: Empty lists, bare values, catch non-existent symbols
# FIXME: Quotes not being properly stripped off in quotes in lists...

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

my $sections = [];
my @asm_files;

while($#ARGV>-1){
    $asm_file = shift @ARGV;
    open ASM_FILE, $asm_file;
    $asm_file = [<ASM_FILE>];
    push @asm_files, $asm_file;
    close ASM_FILE;
}

#$sections = balanced_parse( clean( $asm_files[0] ) );

for $asm_file (@asm_files){
    $asm_file = clean( $asm_file );
    #$sections = balanced_parse( \$asm_file );
    my $parsed = balanced_parse( \$asm_file );
    splice( @{$sections}, 0, 0, @{$parsed} );
}

#print Dumper($sections) and die;

tokenize($sections);
my $obj = encode($sections);

#print Dumper($obj) and die;
#dump_obj($obj->{root}{code});

create_lst($proj_name, $obj);
create_bbl($proj_name, $obj);
create_c  ($proj_name, $obj);

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
#    $obj->{code} = [];

    # build symbol-table
    foreach my $named_section (@{$parse_tree}){
        $symbol_table->{$named_section->[0]} = $named_section->[1];
    }

    #print Dumper($symbol_table) and die;

    my $offset = 0;

    #$obj->{code}{'root'} = [];
    return encode_section($symbol_table, {}, 'root', \$offset);

    #return $obj;

}

#
#
sub encode_section{

    my ($symbol_table, 
        $addr_lut, 
        $section_name, 
        $offset) = @_;

    if($section_name eq "nil"){ # If nil is reference prior to creation of the first list...
        return create_nil($addr_lut, $offset);
    }

    $addr_lut->{$section_name} = ($$offset+1) * $MWORD_SIZE;

    return encode_tree($symbol_table, $addr_lut, $section_name, $offset, $symbol_table->{$section_name});

}

#
#
sub encode_tree{

    my ($symbol_table, 
        $addr_lut,
        $section_name, 
        $offset,
        $sub_tree) = @_;

    if   ($sub_tree->[0] eq 'val'){ #values
        return encode_values        ($sub_tree, $offset, 0);
    }
    elsif($sub_tree->[0] eq 'ptr'){ #pointers
        return encode_pointers      ($symbol_table, $addr_lut, $section_name, $offset, $sub_tree);
    }
    elsif($sub_tree->[0] eq 'tlist'){ #tagged-list
        return encode_tagged_list   ($symbol_table, $addr_lut, $section_name, $offset, $sub_tree);
    }
    elsif($sub_tree->[0] eq 'list'){ #list
        return encode_list          ($symbol_table, $addr_lut, $section_name, $offset, $sub_tree);
    }
    elsif($sub_tree->[0] eq 'hash'){ #hash
        return encode_hash          ($sub_tree, $offset, 0);
    }
    elsif($sub_tree->[0] eq 'oper'){ #operator
        return encode_values        ($sub_tree, $offset, 0);
    }
    elsif($sub_tree->[0] eq 'ref'){ #operator
        return encode_ref   ($symbol_table, $addr_lut, $section_name, $offset, $sub_tree);
    }
    else{
        print "Unrecognized list type: $symbol_table->{$section_name}->[0]\n" and die;
    }

}

#
#
sub encode_values{

    my ($sub_tree, 
        $offset,
        $single_mode) = @_;

    my $eval_string;
    my @str_vec;

    my @values = @{$sub_tree};
    shift @values;

    #$$offset += ($#values+2);
    $$offset += 1;

    my $value_list = [];

    foreach my $value (@values){

        if($value =~ /^(\s*-?[1-9][0-9]*)$/ or $value =~ /^(\s*0+)[^x]$/ ){
            push @{$value_list}, 1+$1-1; #FORCE Perl to treat it as numeric
            $$offset += 1;
        }
        elsif($value =~ /^(\s*0x[A-Fa-f0-9]+)$/){
            push @{$value_list}, hex($1);
            $$offset += 1;
        }
        elsif($value =~ /^\s*"[^"]*"$/){

            $eval_string = "\$value = $value;";
            die unless eval($eval_string);
            #$value = "\"$value\"";

            @str_vec = str2vec($value);
            push @{$value_list}, @str_vec;
            $$offset += ($#str_vec+1);
            
        }
        elsif($value =~ /^\s*'([^']*)'$/){
            @str_vec = str2vec($1);
            push @{$value_list}, @str_vec;
            $$offset += ($#str_vec+1);
        }
        elsif($value =~ /^($section_name)$/){
            die;
        }
        else{
            print "Error: .$value.\n" and die;
        }

        last if $single_mode;

    }

    unshift @{$value_list}, $MWORD_SIZE * ($#{$value_list}+1);

    return $value_list;

}


#
#
sub encode_hash{

    my ($sub_tree, 
        $offset) = @_;

    my $eval_string;
    my @str_vec;

    my @values = @{$sub_tree};
    shift @values;

    #$$offset += ($#values+2);
    $$offset += 1;

    my $value_list = [];
    my $value = $values[0];
    my @hash = ();

    if($value =~ /^\s*"[^"]*"$/){

        $eval_string = "\$value = $value;";
        die unless eval($eval_string);

        @hash = bytes_to_mwords(Hash::Pearson16::pearson16_hash($value));
        push @{$value_list}, @hash;
        $$offset += ($#hash+1);
        
    }
    elsif($value =~ /^\s*'([^']*)'$/){

        @hash = bytes_to_mwords(Hash::Pearson16::pearson16_hash($1));
        push @{$value_list}, @hash;
        $$offset += ($#hash+1);
        
    }
    else{
        print "Error: .$value.\n" and die;
    }

    unshift @{$value_list}, $MWORD_SIZE * ($#{$value_list}+1);

    return $value_list;

}


#
#
sub encode_pointers{

    my ($symbol_table, 
        $addr_lut,
        $section_name, 
        $offset,
        $sub_tree) = @_;

    my $encoded;

    my @pointers = @{$sub_tree};
    shift @pointers;

    $$offset += ($#pointers+2);

    my $pointer_list = [];
    push @{$pointer_list}, -1*$MWORD_SIZE*($#pointers+1);

    for(0..$#pointers){
        push @{$pointer_list},0xdeadbeef;
    }

    my $i = 1;
    foreach my $pointer (@pointers){

        if(ref($pointer) eq ""){
        
            if(is_value($pointer)){
                $pointer_list->[$i] = $MWORD_SIZE*($$offset+1);
                $encoded = encode_values(["val", $pointer], $offset, 1);
                push (@{$pointer_list}, $_) for (@{$encoded});
                #printf("A:%x ", $$offset*$MWORD_SIZE);
            }
            else{
                #printf("B:%x ", $$offset*$MWORD_SIZE);
                unless(exists $addr_lut->{$pointer}){
                    $encoded = encode_section($symbol_table, $addr_lut, $pointer, $offset);
                    push (@{$pointer_list}, $_) for (@{$encoded});
                }
                $pointer_list->[$i] = $addr_lut->{$pointer};
            }
        
        }
        else{
            #printf("C:%x ", $$offset*$MWORD_SIZE);
            $pointer_list->[$i] = $MWORD_SIZE*($$offset+1);
            $encoded = encode_tree($symbol_table, $addr_lut, $section_name, $offset, $pointer);
            push (@{$pointer_list}, $_) for (@{$encoded});
        }
        $i++;

    }

    return $pointer_list;

}

#
#
sub encode_list{

    my ($symbol_table, 
        $addr_lut,
        $section_name, 
        $offset,
        $sub_tree) = @_;

    my $encoded;
    my ($car, $cdr);

    my @elements = @{$sub_tree};
    shift @elements;

    #$$offset += ($#elements+2);

    my $element_list = [];
    #push @{$element_list}, -1*$MWORD_SIZE*($#elements+1);

    foreach my $element (@elements){

        $$offset += 3;
        push @{$element_list}, -2*$MWORD_SIZE;
        for(1..2){
            push @{$element_list}, 0xdeadbeef;
        }

        $car = $#{$element_list}-1;
        $cdr = $#{$element_list};

        if(ref($element) eq ""){

            if(is_value($element)){
                $element_list->[$car] = $MWORD_SIZE*($$offset+1);
                $encoded = encode_values(["val", $element], $offset, 1);
                push (@{$element_list}, $_) for (@{$encoded});
                #printf("A:%x ", $$offset*$MWORD_SIZE);
            }
            else{
                unless(exists $addr_lut->{$element}){
                    $encoded = encode_section($symbol_table, $addr_lut, $element, $offset);
                    push (@{$element_list}, $_) for (@{$encoded});
                }
                $element_list->[$car] = $addr_lut->{$element};
            }

        }
        else{
            $element_list->[$car] = $MWORD_SIZE*($$offset+1);
            $encoded = encode_tree($symbol_table, $addr_lut, $section_name, $offset, $element);
            push (@{$element_list}, $_) for (@{$encoded});
        }
        $element_list->[$cdr] = ($$offset+1) * $MWORD_SIZE;

    }

    if(!exists $addr_lut->{nil}){
        $encoded = create_nil($addr_lut, $offset);
        push (@{$element_list}, $_) for (@{$encoded});
    }

    $element_list->[$cdr] = $addr_lut->{nil};

    return $element_list;

}

#
#
sub encode_tagged_list{

    my ($symbol_table, 
        $addr_lut,
        $section_name, 
        $offset,
        $sub_tree) = @_;

    my $encoded;
    my ($car, $cdr);

    my @elements = @{$sub_tree};
    shift @elements;

    my $element_list = [];

    my $plain_tag = shift @elements;
    my $tag;

    push @{$element_list}, 0;

    if($plain_tag =~ /^\s*"([^"]*)"$/ or $plain_tag =~ /^\s*'([^']*)'$/){
        push @{$element_list}, bytes_to_mwords(Hash::Pearson16::pearson16_hash($1));
    }
    else{
        print "Error: .$plain_tag.\n" and die;
    }

    $$offset += 5;

    foreach my $element (@elements){

        $$offset += 3;
        push @{$element_list}, -2*$MWORD_SIZE;
        for(1..2){
            push @{$element_list}, 0xdeadbeef;
        }

        $car = $#{$element_list}-1;
        $cdr = $#{$element_list};

        if(ref($element) eq ""){
            if(is_value($element)){
                $element_list->[$car] = $MWORD_SIZE*($$offset+1);
                $encoded = encode_values(["val", $element], $offset, 1);
                push (@{$element_list}, $_) for (@{$encoded});
                #printf("A:%x ", $$offset*$MWORD_SIZE);
            }
            else{
                unless(exists $addr_lut->{$element}){
                    $encoded = encode_section($symbol_table, $addr_lut, $element, $offset);
                    push (@{$element_list}, $_) for (@{$encoded});
                }
                $element_list->[$car] = $addr_lut->{$element};
            }
        }
        else{
            $element_list->[$car] = $MWORD_SIZE*($$offset+1);
            $encoded = encode_tree($symbol_table, $addr_lut, $section_name, $offset, $element);
            push (@{$element_list}, $_) for (@{$encoded});
        }
        $element_list->[$cdr] = ($$offset+1) * $MWORD_SIZE;

    }

    if(!exists $addr_lut->{nil}){
        $encoded = create_nil($addr_lut, $offset);
        push (@{$element_list}, $_) for (@{$encoded});
    }

    $element_list->[$cdr] = $addr_lut->{nil};

    return $element_list;

}


#
#
sub encode_ref{

    my ($symbol_table, 
        $addr_lut,
        $section_name, 
        $offset,
        $sub_tree) = @_;

    my $encoded;
    my ($car, $cdr);

    my @elements = @{$sub_tree};
    shift @elements;

    my @list = @elements;
    unshift(@list, "list");

#    print "@list\n" and die;

    my $element_list = [];

    push @{$element_list}, 0;
    push @{$element_list}, bytes_to_mwords(Hash::Pearson16::pearson16_hash("/babel/tag/ref"));
    push @{$element_list}, -2 * $MWORD_SIZE;
    push @{$element_list}, 0xdeadbeef;
    push @{$element_list}, 0xdeadbeef;

    $car = $$offset+5;
    $cdr = $$offset+6;

    $$offset += 7;
    my $offset_save = $$offset;

    $encoded = encode_list($symbol_table, $addr_lut, "${section_name}_ATTACHED_LIST", $offset, \@list);
    push (@{$element_list}, $_) for (@{$encoded});

    $element_list->[$car] = $offset_save * $MWORD_SIZE;#$addr_lut->{"${section_name}_ATTACHED_LIST"};

    if(!exists $addr_lut->{nil}){
        $encoded = create_nil($addr_lut, $offset);
        push (@{$element_list}, $_) for (@{$encoded});
    }

    $element_list->[$cdr] = $addr_lut->{nil};

    return $element_list;

}


#
#
sub is_value{

    my $value = shift;

    if( $value =~ /^(\s*-?[1-9][0-9]*)$/ or 
        $value =~ /^(\s*0+)[^x]$/ or
        $value =~ /^(\s*0x[A-Fa-f0-9]+)$/ or
        $value =~ /^\s*"[^"]*"$/ or
        $value =~ /^\s*'([^']*)'$/){
        return 1;
    }

    return 0;

}


#
#
sub create_nil{

    my ($addr_lut, $offset) = @_;

    $addr_lut->{nil} = ($$offset+1) * $MWORD_SIZE;
    $$offset += 8;

    return [ 0, 0x3023f4e7, 0x8c2f644d, 0x71cf647b, 0xe974b23a, -2*$MWORD_SIZE, $addr_lut->{nil}, $addr_lut->{nil} ];

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
#    my $obj = shift;
    my $i = 0;

    open LST_FILE, ">${lst_file}.lst" or die "Couldn't create .lst: $!";

    for my $word (@{$obj_root}) {
        printf LST_FILE ("%04x %08x\n", $MWORD_SIZE * $i++, $word);
    }

#    foreach (sort keys %{$obj}){
#        if(defined $obj->{addr}{$_}){
##            my $op = $_;
##            $op =~ s/%/%%/g;
###            printf("$op\n");
#            printf LST_FILE ("%08x $_\n", $obj->{addr}{$_});
#        }
#    }
#
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

    my $c_file = shift;
    my $obj_root = shift;

    open C_FILE,   ">${c_file}.c" or die "Couldn't create .c: $!";
    printf C_FILE ("#define BBL_SIZE %d\nunsigned bbl[BBL_SIZE] = {", $#{$obj_root} + 1);

    my $i = 0;
    for(@{$obj_root}){
        if($i++ % 8 == 0){
            print C_FILE "\n    ";
        }
        printf   C_FILE ("0x%08x, ", $_ & 0xffffffff);
        #$i++;
    }

    print C_FILE "\n};\n";
    close C_FILE;

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



