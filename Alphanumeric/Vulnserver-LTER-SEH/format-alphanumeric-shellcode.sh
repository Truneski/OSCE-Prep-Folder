#!/bin/bash
sed 's/^.//' shellcode.txt > /tmp/shellcode1.txt
sed 's/.$//' /tmp/shellcode1.txt > /tmp/shellcode2.txt
tr -d '\n' < /tmp/shellcode2.txt

