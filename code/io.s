global putc
global puts
global strlen
SECTION .bss
char_buf: resb 1

SECTION .text
putc:
  push rbp
  mov rbp, rsp

  mov cl, byte [rbp + 16]
  mov [char_buf], cl
  mov rax, 0x01       ; write syscall
  mov rdi, 0x01       ; stdout=1
  mov rsi, char_buf   ; argument 1 as char
  mov rdx, 0x01       ; number of characters to write
  syscall

  mov rsp, rbp
  pop rbp
  ret 8

strlen:
  push rbp
  mov rbp, rsp
  mov rax, 0
  mov rsi, [rbp + 16]
strlen_loop:
  cmp byte [rsi], 0
  je strlen_end
  inc rax
  inc rsi
  jmp strlen_loop
strlen_end:
  mov rsp, rbp
  pop rbp
  ret 8

puts:
  push rbp
  mov rbp, rsp
  
  push qword [rbp + 16]
  call strlen
  mov rdx, rax

  mov rax, 0x01
  mov rdi, 0x01
  mov rsi, [rbp + 16]
  syscall

  mov rsp, rbp
  pop rbp
  ret 8
