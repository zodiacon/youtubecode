.code

includelib user32

MessageBoxA proto

MB_ICONINFORMATION equ 40h

main proc
sub rsp, 40
xor rcx,rcx
mov rdx, offset text
mov r8, offset caption
mov r9, MB_ICONINFORMATION
call MessageBoxA
add rsp, 40
ret
main endp

.data

caption db "Hello Assembly!", 0
text db "This is a nice assembly program!"

end
