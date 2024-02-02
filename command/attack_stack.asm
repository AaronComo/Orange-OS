global _start 
section .text

_start:
    pushad 
    jmp get_message 
main:
    pop edx 
    xor eax, eax 
    int 0x90 
    popad 
    mov ebx, 0x00000000 
loop:
    jmp loop 
get_message:
    call main 
msg:
    db "You are overflowed!", 0xA, 0x0