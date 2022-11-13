#!/usr/bin/perl

use strict;
use warnings;
use autodie qw(:all);

@ARGV=qw(errno.cc.i);
my $f="errno.list.cc";
open(STDIN,"</dev/null");
open(STDOUT,">$f.new");
print "#define error_msg(name,code)\n";

while(<>){
  last if /ERRNO_H\s*$/;
};
my %seen;
my @errs;

while(<>){
  next if /^#define EXIT/;
  next if /^#define EOF/;
  next unless /^#define E/;
  @_=split;
  shift @_;
  push(@errs,\@_);
};
for(@errs){
  @_=@{$_};
  $_="error_msg($_[0],$_[1])\n";
  print;
};
while(<>){
};

close(STDOUT);
mkdir("tmp") unless -d "tmp";
rename($f,"tmp/$f.old") if -e $f;
rename("$f.new","$f");

