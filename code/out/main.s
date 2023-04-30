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
mov rsp, rbp
pop rbp
ret 8
global main
main:
push rbp
mov rbp, rsp
mov rsp, rbp
pop rbp
ret 16
