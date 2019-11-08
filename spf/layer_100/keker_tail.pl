#!/usr/bin/perl -T

use strict;
use warnings;
use Getopt::Long;

my $num_lines   = 10;
my $word_mode;
GetOptions ("n=i" => \$num_lines,
            "w"   => \$word_mode)
or die("Error in command line arguments\n");

if ($word_mode) {
  print "word mode\n";
}
else {
  print "line mode\n";
}

