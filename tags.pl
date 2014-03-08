# tags.pl
# Generates src/tags.h and src/tags.c

use strict;
use Hash::Pearson16;

my @hash;
my $MWORD_SIZE = 4;
my $HASH_SIZE = 16/$MWORD_SIZE;

my $tags = 
    {   "BABEL_TAG_NIL"                 => "/babel/tag/nil", 
        "BABEL_TAG_INTERP_NIL"          => "nil", 
        "BABEL_TAG_REF"                 => "/babel/tag/ref", 
        "BABEL_TAG_PURE_REF"            => "/babel/tag/pure_ref",
        "BABEL_TAG_HASH_TABLE"          => "/babel/tag/hash_table",
        "BABEL_TAG_HASH_TABLE_ENTRY"    => "/babel/tag/hash_table_entry",
        "BABEL_TAG_DIRECTORY"           => "/babel/tag/directory",
        "BABEL_TAG_BABEL_STRING"        => "/babel/tag/babel_string",
        "BABEL_TAG_CSTRING"             => "/babel/tag/cstring",
        "BABEL_TAG_STRING_ARRAY"        => "/babel/tag/string_array",
        "BABEL_TAG_STRING_LIST"         => "/babel/tag/string_list",
        "BABEL_TAG_NUMERIC"             => "/babel/tag/numeric",
        "BABEL_TAG_ARRAY8"              => "/babel/tag/array8",
        "BABEL_TAG_ARRAY1"              => "/babel/tag/array1",
        "BABEL_TAG_BVM"                 => "/babel/tag/bvm",    
        "BABEL_TAG_SYM_BVM"             => "/babel/tag/sym_bvm",    
        "BABEL_TAG_HIBER_BVM"           => "/babel/tag/hiber_bvm",    
        "BABEL_TAG_SPARSE_BVM"          => "/babel/tag/sparse_bvm",    
        "BABEL_TAG_READY_BVM"           => "/babel/tag/ready_bvm",    
        "BABEL_TAG_BVM_CODE"            => "/babel/tag/bvm_code",
        "BABEL_TAG_BVM_STACK"           => "/babel/tag/bvm_stack",    
        "BABEL_TAG_BVM_RSTACK"          => "/babel/tag/bvm_rstack",    
        "BABEL_TAG_BVM_JUMP_TABLE"      => "/babel/tag/bvm_jump_table",    
        "BABEL_TAG_BVM_SYM_TABLE"       => "/babel/tag/bvm_sym_table",
        "BABEL_TAG_BVM_STEPS"           => "/babel/tag/bvm_steps",
        "BABEL_TAG_EVAL"                => "/babel/tag/eval",
        "BABEL_TAG_LOOP"                => "/babel/tag/loop",
        "BABEL_TAG_TIMES"               => "/babel/tag/times",
        "BABEL_TAG_IFTE"                => "/babel/tag/ifte",
        "BABEL_TAG_EACH"                => "/babel/tag/each",
        "BABEL_TAG_EACHAR"              => "/babel/tag/eachar",
        "BABEL_TAG_WHILE"               => "/babel/tag/while",
        "BABEL_TAG_NEST"                => "/babel/tag/nest",
        "BABEL_TAG_ALT"                 => "/babel/tag/alt",
        "BABEL_TAG_SEQ"                 => "/babel/tag/seq",
        "BABEL_TAG_PASS"                => "/babel/tag/pass",
        "BABEL_TAG_FAIL"                => "/babel/tag/fail",
        "BABEL_TAG_TRUE"                => "/babel/tag/true",
        "BABEL_TAG_FALSE"               => "/babel/tag/false",
        "BABEL_TAG_UNDEF"               => "/babel/tag/undef",
        "BABEL_TAG_UNEXIST"             => "/babel/tag/unexist",
        "BABEL_TAG_NAN"                 => "/babel/tag/nan",
        "BABEL_TAG_LET"                 => "/babel/tag/let",
        "BABEL_TAG_REF_HASH"            => "/babel/tag/ref_hash",
        "BABEL_TAG_REF_STRING"          => "/babel/tag/ref_string",
        "BABEL_TAG_REF_ARRAY"           => "/babel/tag/ref_array",
        "BABEL_TAG_REF_LIST"            => "/babel/tag/ref_list",
        "BABEL_TAG_COND"                => "/babel/tag/cond",
        "BABEL_TAG_PACMAN"              => "/babel/tag/pacman",
        "BABEL_TAG_COMPAT"              => "WINDOWS;32", };


#########################################################################
#
# tags.h
#
#########################################################################
    
open H_FILE,   ">src/tags.h" or die "Couldn't create src/tags.h: $!";

print H_FILE <<END_QUOTE;
// tags.h
//

#ifndef TAGS_H
#define TAGS_H

#include "babel.h"

END_QUOTE

for my $tag (keys %{$tags}){

    print H_FILE "extern const mword ${tag}[HASH_SIZE];\n";

}

print H_FILE <<END_QUOTE;

#endif //TAGS_H

// Clayton Bauman 2013

END_QUOTE


close H_FILE;

#########################################################################
#
# tags.c
#
#########################################################################

open C_FILE,   ">src/tags.c" or die "Couldn't create src/tags.c: $!";

print C_FILE <<END_QUOTE;
// tags.c
//

#include "tags.h"

END_QUOTE

for my $tag (keys %{$tags}){

    @hash = bytes_to_mwords(Hash::Pearson16::pearson16_hash($tags->{$tag}));

    print C_FILE "const mword ${tag}[] = { ";

    for(@hash){
        printf   C_FILE ("0x%08x, ", $_ & 0xffffffff);
    }

    print C_FILE "};\n";

}

print C_FILE <<END_QUOTE;

// Clayton Bauman 2013

END_QUOTE

close C_FILE;

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

