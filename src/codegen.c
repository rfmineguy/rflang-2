#include "codegen.h"
#include <string.h>

// NOTE: All code generated will be in x86_64 nasm assembly

static char STACK_FRAME_BEGIN[100] = "push rbp\nmov rbp, rsp\n\n";
static char STACK_FRAME_END[100]   = "mov rsp, rbp\npop rbp\n";

void codegen_program(program_t* prog, FILE* f) {
  for (int i = 0; i < prog->use_list_count; i++) {
    codegen_use(prog->use_list[i], f); 
  }

  codegen_entry(f);
  
  for (int i = 0; i < prog->func_list_count; i++) {
    codegen_func(prog->func_list[i], f);
  }
}

void codegen_use(use_t* use, FILE* f) {
  char use_label[100] = {0};
  strncat(use_label, "\%include \"", 100);
  strncat(use_label, use->name, 100);
  strncat(use_label, "\"\n", 100);
  fwrite(use_label, 1, strlen(use_label), f);
}

void codegen_func(func_t* func, FILE* f) {
  char func_label[100] = {0};
  char ret[20] = {0};
  strncat(func_label, func->decl->name, 100);
  strncat(func_label, ":\n", 100);
  fwrite(func_label, 1, strlen(func_label), f);
  fwrite(STACK_FRAME_BEGIN, 1, strlen(STACK_FRAME_BEGIN), f);

  fwrite(STACK_FRAME_END, 1, strlen(STACK_FRAME_END), f);
  int ret_bytes = func->decl->params->params_count * 8;
  snprintf(ret, 20, "ret %d\n\n", ret_bytes);
  fwrite(ret, 1, strlen(ret), f);
}

void codegen_entry(FILE* f) {
  fputs("SECTION .data\n", f);
  fputs("SECTION .text\n", f);
  fputs("global _start\n", f);
  fputs("_start:\n", f);
  fputs("\tcall main\n", f);
  fputs("\tcall _end\n", f);
  fputs("_end:\n", f);
  fputs("\tmov rax, 0x3C\n", f);
  fputs("\tsyscall\n", f);
}
