_start:
  call main

main:
  ret

add:
  push rbp
  mov rbp, rsp
  ;... 
  mov rsp, rbp
  pop rbp
  ret

putc:
  push rbp
  mov rbp, rsp
    

  mov rsp, rbp
  pop rbp
  ret

_end:

