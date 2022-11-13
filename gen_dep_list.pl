#!/usr/bin/perl

use strict;
use warnings;
use autodie qw(:all);

@ARGV=glob("*.i");
my $f="all_deps.mk";

open(STDOUT,">$f.new");

while(<>){
  print;
  next unless s{^#\s+\d+\s+}{};
  print;
};

close(STDOUT);
rename("$f","$f.old") if -e "$f";
rename("$f.new","$f");

