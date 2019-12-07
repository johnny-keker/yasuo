#!/usr/bin/perl -T


use strict;
use warnings qw(FATAL all);
use IO::Socket::UNIX;
use English;

$ENV{'PATH'} = '/usr/bin';
$ENV{CDPATH}="";
$ENV{ENV}="";
$ENV{BASH_ENV} = '/usr/share/Modules/init/bash';

use constant SYSTEM_INFO_FMT => 'i<i<i<d<d<d<d<';
use constant SOCKET_PATH => 'LAB_SOCK';
use sigtrap 'handler' => \&signal_handler, qw(HUP INT TERM USR1 USR2);

# get socket name
my $sockname = shift @ARGV;

unlink SOCKET_PATH;

my $server = IO::Socket::UNIX->new(
  Type => SOCK_STREAM(), Local => SOCKET_PATH, Listen => 1
) or die "!<...failed-to-create-a-socket-$!...>!\n";

# logging
my $gid = (split ' ', $GID)[0];
print "<...initializing-server...>\n\n";
print "<...start-parameters...>\n";
print "    <...pid=$PID...>\n";
print "    <...uid=$UID...>\n";
print "    <...gid=$gid...>\n";
print "<...start-parameters...>\n\n";
# logging

# init server parameters
my ($s_time, $r_time) = (time(), time());
my ($l1, $l5, $l15) = (0, 0, 0);

while (my $clt = $server->accept()) {
  $r_time = time() - $s_time;
  my $loads = `uptime | awk '{print \$(NF-2)\$(NF-1)\$NF}'`;
	$loads =~ /(\d+\.\d+),(\d+\.\d+),(\d+\.\d+)/;
  my $state = pack SYSTEM_INFO_FMT, $PID, $UID, $gid, $r_time, $1, $2, $3;

  $clt->print($state);
  $clt->close();
}

sub signal_handler {
  $r_time = time() - $s_time;
  my $loads = `uptime | awk '{print \$(NF-2)\$(NF-1)\$NF}'`;
	$loads =~ /(\d+\.\d+),(\d+\.\d+),(\d+\.\d+)/;

  print "<...received-$_[0]...>\n";
  print "    <...time=$r_time...>\n";
  print "    <...pid=$PID...>\n";
  print "    <...uid=$UID...>\n";
  print "    <...gid=$gid...>\n";
  print "    <...ave-sys-load-1min=$1...>\n";
  print "    <...ave-sys-load-2min=$2...>\n";
  print "    <...ave-sys-load-15min=$3...>\n";
  print "<...terminating-server...>\n\n";
  exit(0);
}
