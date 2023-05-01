section .data
str: db 0x48, 0x69, 0x6B, 0x61, 0x67, 0x66, 0x6A, 0x61, 0x6B, 0x68, 0x64, 0x0A
section .text
%include "io"
%include "stdlib"
global add
add:
push rbp
mov rbp, rsp
mov rsp, rbp
pop rbp
ret 24
global sub
sub:
push rbp
mov rbp, rsp
mov rsp, rbp
pop rbp
ret 16
global main
main:
push rbp
mov rbp, rsp
mov rsp, rbp
pop rbp
ret 16
