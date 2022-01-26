#!/usr/bin/python

filename="r344.txt"

buffer = "A" * 12000

textfile = open(filename , 'w')
textfile.write(buffer)
textfile.close()


