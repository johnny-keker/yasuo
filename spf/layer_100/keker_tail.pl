#!/usr/bin/perl -T

use strict;
use warnings qw(FATAL all);
use Getopt::Long;

my $num_lines = 10;
my $word_mode;
my $first_out = 0;

# cmd line args parsing
GetOptions ("n=i" => \$num_lines,
            "w"   => \$word_mode)
or die("Error in command line arguments\n");

foreach my $filename (@ARGV) {
  my $fh;
  if ($filename eq "-") {
    $fh = "STDIN";
  }
  else {
    open($fh, "<", $filename)
      or die("Cannot open $filename\n");
  }
  if (scalar @ARGV != 1) {
    if ($filename eq "-") {
      if ($first_out == 0) {
        print "==> standard input <==\n";
        $first_out++;
      }
      else {
        print "\n==> standard input <==\n";
      }
    }
    else {
      if ($first_out == 0) {
        print "==> $filename <==\n";
        $first_out++;
      }
      else {
        print "\n==> $filename <==\n";
      }
    }
  }
  if ($word_mode) {
    my @words;
    my $processed = 0;
    while (my $line = <$fh>) {
      my @c_words = split /\s+/, $line;
      foreach my $word (@c_words) {
        if ($processed < $num_lines) {
          push (@words, "$word ");
          $processed++;
        }
        else {
          shift @words;
          push @words, "$word ";
        }
      }
      my $last_word = pop @words;
      push @words, "$last_word\n";
      if (eof) {
        foreach my $word (@words) {
          print $word;
        }
        @words = ();
      }
    }
  }
  else {
    my @lines;
    my $processed = 0;
    while (my $line = <$fh>) {
      if ($processed < $num_lines) {
        push (@lines, $line);
        $processed++;
      }
      else {
        shift @lines;
        push (@lines, $line);
      }
      if (eof) {
        foreach my $line (@lines) {
          print $line;
        }
        @lines = ();
      }
    }
  }
}
