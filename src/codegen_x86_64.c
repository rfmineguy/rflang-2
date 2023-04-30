#include "codegen_x86_64.h"

void x86_64_codegen_program(program_t* p, FILE* f) {
  for (int i = 0; i < p->use_list_count; i++) {
    x86_64_codegen_use(p->use_list[i], f);
  }
  for (int i = 0; i < p->func_list_count; i++) {
    x86_64_codegen_func(p->func_list[i], f);
  }
  printf("Codegened x86-64\n");
}

void x86_64_codegen_use(use_t* u, FILE* f) {
  fprintf(f, "%%include %s", u->name);
}

void x86_64_codegen_func(func_t* func, FILE* f) {
  int pop_size = x86_64_codegen_func_decl(func->decl, f);

  x86_64_codegen_block(func->block, f);

  fprintf(f, "ret %d\n", pop_size * 8);
}

int x86_64_codegen_func_decl(func_decl_t* decl, FILE* f) {
  fprintf(f, "%s:\n", decl->name);
  return decl->params->params_count;
}

void x86_64_codegen_block(block_t* block, FILE* f) {
  fprintf(f, "push rbp\n");
  fprintf(f, "mov rbp, rsp\n");

  for (int i = 0; i < block->statement_count; i++) {
    x86_64_codegen_statement(block->statements[i], f);
  }

  fprintf(f, "mov rsp, rbp\n");
  fprintf(f, "pop rbp\n");
}

void x86_64_codegen_statement(statement_t* stmt, FILE* f) {
  if (stmt->iff) {
    x86_64_codegen_if(stmt->iff, f);
    return;
  }
  if (stmt->ret) {
    x86_64_codegen_return(stmt->ret, f);
    return;
  }
  if (stmt->func_call) {
    x86_64_codegen_func_call(stmt->func_call, f);
    return;
  }
  if (stmt->assign) {
    x86_64_codegen_assign(stmt->assign, f);
    return;
  }
  if (stmt->asm_block) {
    x86_64_codegen_asm_block(stmt->asm_block, f);
    return;
  }
}

void x86_64_codegen_if(if_t* iff, FILE* f) {
  fprintf(f, "<if goes here>\n");
}

void x86_64_codegen_return(return_t* ret, FILE* f) {
  fprintf(f, "<ret goes here>\n");
}

void x86_64_codegen_func_call(func_call_t* func_call, FILE* f) {
  fprintf(f, "<func_call goes here>\n");
}

void x86_64_codegen_assign(assign_t* assign, FILE* f) {
  fprintf(f, "<assign goes here>\n");
}

// NOTE: Conditionally generate this one based on platform
void x86_64_codegen_asm_block(asm_block_t* asm_block, FILE* f) {
  int length = asm_block->asm_source_code_end - asm_block->asm_source_code_begin;
  fprintf(f, "%.*s\n", length - 1, asm_block->asm_source_code_begin);
}
