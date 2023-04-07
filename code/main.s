SECTION .text
add__int_int_short:
  push rbp
  mov rbp, rsp
  mov rax, 0
  add rax, dword [rbp + 8]
  add rax, dword [rbp + 16]
  add rax, word  [rbp + 24]
  mov dword [rbp - 8], rax
  mov rsp, rbp
  pop rbp

sub__int_int:
  push rbp
  mov rbp, rsp
  mov rax, 0
  mov rsp, rbp
  pop rbp

global _start
_start:
  call main
  call _end

main:
  ret

_end:
  mov rax, 0x3c
  mov rdi, 0
  syscall
  ret

