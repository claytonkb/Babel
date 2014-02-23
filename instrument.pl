#!/usr/bin/perl.exe -w

use strict;

use Data::Dumper;

my @files = `ls src`;

@files = grep(/\.c/, @files);

#my @terms = <DATA>;
#chomp for @terms;

foreach my $file (@files){

    open FILE, "src/$file";
    my @lines = <FILE>;
    #my $line = <FILE>;
    #print "$line\n\n";
    close FILE;

    for my $line (@lines){

        next if $line =~ /pearson16_byte_perm/;
        next if $line =~ /^\s*\/\//;

        $line =~ s/#\s*$/#\ntrace; \/\/INSTRUMENT\n/;
    
    }

    open FILE, ">changed/$file";
    print FILE $_ for(@lines);
    close FILE;

}


