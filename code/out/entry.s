extern main
global _start
_start:
push rbp
mov rbp, rsp

  call main
  push rax
  call exit
  
mov rsp, rbp
pop rbp
ret 0
global exit
exit:
push rbp
mov rbp, rsp

  mov rax, 60
  mov rdi, 45
  syscall
  
mov rsp, rbp
pop rbp
ret 8
