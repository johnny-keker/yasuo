#!/usr/bin/perl -T

use strict;
use warnings;
use Getopt::Long;

my $num_lines = 10;
my $word_mode;

# cmd line args parsing
GetOptions ("n=i" => \$num_lines,
            "w"   => \$word_mode)
or die("Error in command line arguments\n");

if ($word_mode) {

}
else {
  my @lines;
  my $processed = 0;
  while (my $line = <ARGV>) {
    if ($processed < $num_lines) {
      push (@lines, $line);
      $processed++;
    }
    else {
      shift @lines;
      push (@lines, $line);
    }
  }
  foreach my $line (@lines) {
    print $line;
  }
}
