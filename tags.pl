# tags.pl
# Generates src/tags.h

use strict;
use Hash::Pearson16;

my @hash;
my $MWORD_SIZE = 4;
my $HASH_SIZE = 16/$MWORD_SIZE;

my $tags = 
    {   "BABEL_TAG_NIL"         => "/babel/tag/nil", 
        "BABEL_TAG_REF"         => "/babel/tag/ref", 
        "BABEL_TAG_PURE_REF"    => "/babel/tag/pure_ref",
        "BABEL_TAG_HASH_TABLE"  => "/babel/tag/hash_table",
        "BABEL_TAG_HASH_TABLE_ENTRY"  => "/babel/tag/hash_table_entry",
        "BABEL_TAG_BVM"         => "/babel/tag/bvm",    };

open C_FILE,   ">src/tags.h" or die "Couldn't create src/tags.h: $!";

for my $tag (keys %{$tags}){

    print C_FILE "#define $tag { ";

    @hash = bytes_to_mwords(Hash::Pearson16::pearson16_hash($tags->{$tag}));

    for(@hash){
        printf   C_FILE ("0x%08x, ", $_ & 0xffffffff);
    }

    print C_FILE "}\n";

}

print C_FILE "\n";
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

