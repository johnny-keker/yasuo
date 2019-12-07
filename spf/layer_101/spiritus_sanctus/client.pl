#!/usr/bin/perl -T

#use strict;
use warnings qw(FATAL all);
use IO::Socket::UNIX;

use constant SYSTEM_INFO_FMT => 'i<i<i<d<d<d<d<';
use constant SYSTEM_INFO_SIZE => length pack(SYSTEM_INFO_FMT, 0, 0, 0, 0, 0, 0);
use constant SOCKET_PATH => 'LAB_SOCK';

my $client = IO::Socket::UNIX->new(Type => SOCK_STREAM(), Peer => SOCKET_PATH)
  or die "!<...failed-to-connect-to-a-socket-$!...>!\n";

my $sys_info_buf = "";
read $client, $sys_info_buf, SYSTEM_INFO_SIZE;
my ($spid, $suid, $sgid, $rt, $sl1, $sl5, $sl15) = unpack SYSTEM_INFO_FMT, $sys_info_buf;

print "<...initializing-client...>\n\n";
print "<...connecting-to-socket...>\n\n";

print "<...received-data...>\n";
print "    <...time=$rt...>\n";
print "    <...pid=$spid...>\n";
print "    <...uid=$suid...>\n";
print "    <...gid=$sgid...>\n";
print "    <...ave-sys-load-1min=$sl1...>\n";
print "    <...ave-sys-load-2min=$sl5...>\n";
print "    <...ave-sys-load-5min=$sl15...>\n";
print "<...received-data...>\n\n";
print "<...terminating-client...>\n";
