$uitxt = "ui.txt";

my $junk = "A" x 5000 ; 

open(myfile,">$uitxt") ;
print myfile $junk;
