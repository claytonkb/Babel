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

#Values:        ( j ... )
#Pointers:      ( k ... )
#Tagged-List:   ( q ... )
#List:          ( x ... )
#Cosntant:      ( z ... )

$asm_file = clean     ( \@asm_file );
            $sections = balanced_parse( \$asm_file );
        #translate($sections);
            my $obj = encode($sections);

            #assemble  ( $sections, $asm_file );
            #linkit    ( $sections, 'root', $obj_out, 0 );
            #send_obj  ( $proj_name, $obj_out, $sections);

print Dumper($sections);
#print Dumper($obj);
dump_obj($obj->{root});

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

##########################################################################
##
## TRANSLATOR
##
## Evaluates each token according to its syntactical type
##
##########################################################################
#
#sub translate{
#
#    my $tree = shift;
#    my $eval_string;
#
#    foreach my $branch (@{$tree}){
#
#        if(ref($branch) eq "ARRAY"){
#            translate($branch);
#        }
#        elsif(ref($branch) eq ""){
#
#            if($branch =~ /^(\s*-?[1-9][0-9]*)$/ or $branch =~ /^(\s*0+)[^x]$/ ){
#                #$branch =~ s/^\s*(-?[0-9]+)//;
#                $branch = 0+$1;
#            }
#            elsif($branch =~ /^(\s*0x[A-Fa-f0-9]+)$/){
#                #$branch =~ s/^\s*0x([A-Fa-f0-9]+)//;
#                $branch = hex($1);
#            }
#            elsif($branch =~ /^\s*"[^"]*"$/){
#                $eval_string = "\$branch = $branch;";
#                die unless eval($eval_string);
#                $branch = "\"$branch\"";
#            }
#            elsif($branch =~ /^\s*'([^']*)'$/ or $branch =~ /^$section_name$/ ){
#                #$branch = "$1";
#                #Do nothing
#            }
#            else{
#                print "Error: .$branch.\n" and die;
#            }
#        
#        }
#        else{
#            print "ref type: " . ref($branch) . "\n";
#            die;
#        }
#
#    }
#
#}

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

    encode_section($symbol_table, $obj, 'root');

    return $obj;

}

#
#
sub encode_section{

    my $symbol_table = shift;
    my $obj = shift;
    my $section_name = shift;

    if(exists $obj->{$section_name}){
        return;
    }
    else{
        $obj->{$section_name} = [];
        encode_tree($symbol_table, $obj, $symbol_table->{$section_name}, $obj->{$section_name});
    }

}

#
#
sub encode_tree{

    my ($symbol_table, $obj, $symbol_section, $obj_section) = @_;

    if   ($symbol_section->[0] eq 'j'){ #values
        encode_values($symbol_table, $obj, $symbol_section, $obj_section);
    }
    elsif($symbol_section->[0] eq 'q'){ #pointers
        encode_pointers($symbol_table, $obj, $symbol_section, $obj_section);
    }
    elsif($symbol_section->[0] eq 'k'){ #tagged-list
    }
    elsif($symbol_section->[0] eq 'x'){ #list
    }
    elsif($symbol_section->[0] eq 'z'){ #constant
    }
    else{
        die;
    }

}

sub encode_values{

    my ($symbol_table, $obj, $symbol_section, $obj_section) = @_;
    my $eval_string;
    my @str_vec;

    my @values = @{$symbol_section};
    shift @values;

    foreach my $value (@values){
        if($value =~ /^(\s*-?[1-9][0-9]*)$/ or $value =~ /^(\s*0+)[^x]$/ ){
            #$branch =~ s/^\s*(-?[0-9]+)//;
            push @{$obj_section}, 1+$1-1; #FORCE Perl to treat it as numeric
        }
        elsif($value =~ /^(\s*0x[A-Fa-f0-9]+)$/){
            #$branch =~ s/^\s*0x([A-Fa-f0-9]+)//;
            push @{$obj_section}, hex($1);
        }
        elsif($value =~ /^\s*"[^"]*"$/){
            $eval_string = "\$value = $value;";
            die unless eval($eval_string);
            $value = "\"$value\"";
            @str_vec = str2vec($value);
            push @{$obj_section}, @str_vec;
        }
        elsif($value =~ /^\s*'([^']*)'$/){
            @str_vec = str2vec($value);
            push @{$obj_section}, @str_vec;
        }
        elsif($value =~ /^$section_name$/){
        }
        else{
            print "Error: .$value.\n" and die;
        }            
    }

    unshift @{$obj_section}, -1*$MWORD_SIZE*($#{$obj_section}+1);

}

sub encode_pointers{

    my ($symbol_table, $obj, $symbol_section, $obj_section) = @_;
    my $eval_string;
    my @str_vec;

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


