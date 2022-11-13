#!/usr/bin/perl

use strict;
use warnings;
use autodie qw(:all);

@ARGV=qw(errno.cc.i);
my $f="errno.list.cc";
open(STDOUT,">$f.new");

while(<>){
};

close(STDOUT);
rename($f,"$f.old") if -e $f;
rename("$f.new","$f");

