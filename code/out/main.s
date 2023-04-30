%include io%include rfadd:
push rbp
mov rbp, rsp
<ret goes here>
mov rsp, rbp
pop rbp
ret 24
sub:
push rbp
mov rbp, rsp
<ret goes here>
mov rsp, rbp
pop rbp
ret 16
putc:
push rbp
mov rbp, rsp
<ret goes here>
mov rsp, rbp
pop rbp
ret 8
puts:
push rbp
mov rbp, rsp
<ret goes here>
mov rsp, rbp
pop rbp
ret 8
main:
push rbp
mov rbp, rsp
<ret goes here>
mov rsp, rbp
pop rbp
ret 16
