%include iomain:
push rbp
mov rbp, rsp
<assign goes here>
<ret goes here>
mov rsp, rbp
pop rbp
ret 16
add:
push rbp
mov rbp, rsp
<ret goes here>
mov rsp, rbp
pop rbp
ret 16
