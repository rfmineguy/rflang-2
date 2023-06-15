section .data
section .text
global add
add:
push rbp
mov rbp, rsp
mov rsp, rbp
pop rbp
ret 16
global pow
pow:
push rbp
mov rbp, rsp
mov rsp, rbp
pop rbp
ret 16
global main
main:
push rbp
mov rbp, rsp
call function
mov rsp, rbp
pop rbp
ret 16
global func
func:
push rbp
mov rbp, rsp
mov rsp, rbp
pop rbp
ret 16
