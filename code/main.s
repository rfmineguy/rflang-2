SECTION .data
str:  db "Hello World", 0ah, 0h
str2: db "This is another string", 0ah, 0h
str3: db 0ah, 0h

SECTION .text
extern putc
extern puts
extern strlen
global _start
_start:
  call main
  call _end

main:
  push str2
  call strlen
  mov rdi, rax

  push str2
  call puts
  
  push str3
  call puts
  ret

_end:
  mov rax, 0x3C
  syscall
  ret

