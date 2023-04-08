SECTION .data
str:  db "Hello World", 0ah, 0h
str2: db "This is another string", 0h
str3: db 0h

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
  ret

_end:
  mov rax, 0x3C
  syscall
  ret

