#!/usr/bin/perl.exe -w

use strict;

use Data::Dumper;

my $file_name = shift;

open FILE, $file_name;
my @file = <FILE>;
close FILE;

chomp for @file;

my $i=0;
while($i<$#file){
    printf("%07d %s\n", $i+1, $file[$i]);
    $i = $i + (abs(hex2int($file[$i])) / 4) + 1;
}

sub hex2int { return unpack('l', pack('L', hex(shift))) }

