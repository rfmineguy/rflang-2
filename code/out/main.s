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
