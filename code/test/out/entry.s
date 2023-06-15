section .data
section .text
global exit
exit:
push rbp
mov rbp, rsp
    mov rax, 0x3c
    mov rdi, [rbp + 8]
    syscall
 mov rsp, rbp
pop rbp
ret 8
extern main
global _start
_start:
push rbp
mov rbp, rsp
    call main
    call exit
 mov rsp, rbp
pop rbp
ret 0
