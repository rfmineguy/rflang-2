section .data
section .text
%include "io"
global add
add:
push rbp
mov rbp, rsp
mov rsp, rbp
pop rbp
ret 16
global main
main:
push rbp
mov rbp, rsp
push <arg #0>
push <arg #1>
call add
mov rsp, rbp
pop rbp
ret 0
