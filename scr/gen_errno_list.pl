#!/usr/bin/perl

use strict;
use warnings;
use autodie qw(:all);
use Data::Dumper;

@ARGV=qw(errno.cc.i);
my $f="tmp/errno.list.cc";
open(STDIN,"</dev/null");
open(STDERR,">$f.trash");

my @errs;
print STDERR "starting read\n";
while(<>){
  print STDERR;
  next unless s/^#define\s*//;
  next unless m{^E};
  next if m{^E(XIT|OF)};
  next unless @_ =  m{^(E[A-Z0-9]+)\s+(\d+)};
  push(@errs,[@_]);
  print STDERR "found one\n";
};
print STDERR "finished read\n";
mkdir("tmp") unless -d "tmp";
open(STDOUT,">$f.new");
print "#ifndef error_msg\n";
print "#define error_msg(name,code)\n\n";
print "#endif\n";
print "\n\n";
print STDERR "\n\n";
for(@errs){
  @_=@{$_};
  print "error_msg($_[0],$_[1])\n";
};
close(STDOUT);
close(STDERR);
system("mv $f $f.old") if -e $f;
system("cp $f.new $f");
