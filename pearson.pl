#!/usr/bin/perl.exe -w

use strict;
use feature 'state';
no warnings 'recursion'; # Get "deep recursion" on large .pb files otherwise
use Hash::Pearson16;

use YAML::XS;
use Data::Dumper;

my @my_hash = Hash::Pearson16::pearson16_hash(shift);
my $j;
for($j=$#my_hash;$j>=0;$j--){
    printf("%02x", $my_hash[$j]);
}
print "\n";

