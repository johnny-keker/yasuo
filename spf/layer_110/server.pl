#!/usr/bin/perl

use strict;
use warnings qw(FATAL all);
use threads;
use IO::Socket::INET;

my ($port) = shift =~ /(\d+)/ or die "Usage ./server port\n";

my $server = IO::Socket::INET->new(
  LocalPort => $port, 
  Proto => 'tcp', 
  Listen => SOMAXCONN					# max client buf size
) or die "Can't connect to server: $!\n";

print "Listening on $port\n";

while (my $client = $server->accept()) {
  my $pid = fork();
  die if not defined $pid;
  if (not $pid) {
    my $request = "";
    while (<$client>) {
      $request .= $_;
      last if $request =~ /\r\n\r\n$/;
    }

    my @dirs = split /\r\n/, $request;

    for my $dir (@dirs) {
      if (opendir my $dirent, $dir) {
        my @files = readdir $dirent;
        closedir $dirent;

        print $client "dir-'$dir'\n" . join("\n", @files) . "\n\n";
      }
      else {
        print $client "Can't open $dir: $!\n\n";
      }
    }
    print $client "\r\n";
    close $client;
  }
}
