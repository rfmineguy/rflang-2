section .data
section .text
global strlen
strlen:
push rbp
mov rbp, rsp
mov rsp, rbp
pop rbp
ret 8
global putc
putc:
push rbp
mov rbp, rsp



mov rsp, rbp
pop rbp
ret 8
global puts
puts:
push rbp
mov rbp, rsp

    push qword [rbp + 16]
    call strlen
    mov rdx, rax
    mov rax, 0x01
    mov rdi, 0x01
    mov rsi, [rbp + 16]
    syscall
  

mov rsp, rbp
pop rbp
ret 8
