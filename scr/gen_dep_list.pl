#!/usr/bin/perl

use strict;
use warnings;
use autodie qw(:all);

@ARGV=glob("*.i");
my $f="tmp/all_deps.mk";

open(STDOUT,">$f.new");

my %seen;
while(<>){
  next unless s{^#\s+\d+\s+}{};
  next if $seen{$_};
  print;
};

close(STDOUT);
mkdir("tmp") unless -d "tmp";
rename("$f","$f.old") if -e "$f";
rename("$f.new","$f");

