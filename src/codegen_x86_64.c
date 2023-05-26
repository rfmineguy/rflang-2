#include "codegen_x86_64.h"
#include <stdlib.h>
#include <string.h>

#define WRITE_CODE(str, ...) \
  ctx->currently_written_code += snprintf(ctx->code_segment_source + ctx->currently_written_code, 1000, str, ##__VA_ARGS__)

#define WRITE_DATA(str, ...) \
  ctx->currently_written_data += snprintf(ctx->data_segment_source + ctx->currently_written_data, 1000, str, ##__VA_ARGS__)

void x86_64_codegen_program(program_t* p, FILE* f) {
  printf("Beginning codegen for x86_64-linux\n");
  x86_64_codegen_context ctx = {0};
  ctx.code_segment_source = calloc(1000, 1);
  ctx.data_segment_source = calloc(1000, 1);

  for (int i = 0; i < p->use_list_count; i++) {
    x86_64_codegen_use(p->use_list[i], &ctx);
  }
  for (int i = 0; i < p->func_list_count; i++) {
    x86_64_codegen_func(p->func_list[i], &ctx);
    x86_64_codegen_func(p->func_list[i], &ctx);
  }

  // generate the segments
  fprintf(f, "section .data\n");
  fprintf(f, "%s", ctx.data_segment_source);
  fprintf(f, "section .text\n");
  fprintf(f, "%s", ctx.code_segment_source);;

  free(ctx.data_segment_source);
  ctx.data_segment_source = NULL;
  free(ctx.code_segment_source);
  ctx.code_segment_source = NULL;

  printf("Ending codegen for x86_64-linux\n");
}

void x86_64_codegen_use(use_t* u, x86_64_codegen_context* ctx) {
  WRITE_CODE("%%include \"%s\"\n", u->name);
}

void x86_64_codegen_func(func_t* func, x86_64_codegen_context* ctx) {
  int pop_size = x86_64_codegen_func_decl(func->decl, ctx);

  x86_64_codegen_block(func->block, ctx);

  WRITE_CODE("ret %d\n", pop_size * 8);
}

int x86_64_codegen_func_decl(func_decl_t* decl, x86_64_codegen_context* ctx) {
  if (strcmp(decl->name, "_start") == 0) {
    WRITE_CODE("extern main\n");
  }
  WRITE_CODE("global %s\n", decl->name);
  WRITE_CODE("%s:\n", decl->name);
  return decl->params->params_count;
}

void x86_64_codegen_strlit(string_lit_t* str_lit, x86_64_codegen_context* ctx) {
  WRITE_DATA("str%d: db ", ctx->string_count); //str_lit->str_lit_end - str_lit->str_lit_begin, str_lit->str_lit_begin);
  ctx->string_count++;
  char* c = str_lit->str_lit_begin;
  for (; c <= str_lit->str_lit_end; c++) {
    if (*c == '\\') {
      c++;
      switch (*c) {
        case '0': WRITE_DATA("0x00"); break; // Null
        case 'n': WRITE_DATA("0x0A"); break; // New line
        case 'r': WRITE_DATA("0x0D"); break; // Carraige Return
        case 't': WRITE_DATA("0x09"); break; // Tab
        default:  WRITE_DATA("0xFE"); break; // Solid Square (Extended ascii)
      }
    }
    else {
      WRITE_DATA("0x%02X", *c);
    }
    WRITE_DATA(", ");
  }
  WRITE_DATA("0x00");
  WRITE_DATA("\n");
}

void x86_64_codegen_block(block_t* block, x86_64_codegen_context* ctx) {
  WRITE_CODE("push rbp\n");
  WRITE_CODE("mov rbp, rsp\n");

  for (int i = 0; i < block->statement_count; i++) {
    x86_64_codegen_statement(block->statements[i], ctx);
  }

  WRITE_CODE("mov rsp, rbp\n");
  WRITE_CODE("pop rbp\n");
}

void x86_64_codegen_statement(statement_t* stmt, x86_64_codegen_context* ctx) {
  if (stmt->iff) {
    x86_64_codegen_if(stmt->iff, ctx);
    return;
  }
  if (stmt->ret) {
    x86_64_codegen_return(stmt->ret, ctx);
    return;
  }
  if (stmt->func_call) {
    x86_64_codegen_func_call(stmt->func_call, ctx);
    return;
  }
  if (stmt->assign) {
    x86_64_codegen_assign(stmt->assign, ctx);
    return;
  }
  if (stmt->asm_block) {
    x86_64_codegen_asm_block(stmt->asm_block, ctx);
    return;
  }
}

void x86_64_codegen_if(if_t* iff, x86_64_codegen_context* ctx) {
  // fprintf(ctx->code_segment_source, "<if goes here>\n");
}

void x86_64_codegen_return(return_t* ret, x86_64_codegen_context* ctx) {
  // fprintf(ctx->code_segment_source, "<ret goes here>\n");
}

void x86_64_codegen_func_call(func_call_t* func_call, x86_64_codegen_context* ctx) {
  // sprintf(ctx->code_segment_source, "<func_call goes here>\n");
  for (int i = 0; i < func_call->args->arg_count; i++) {
    WRITE_CODE("push <arg #%d>\n", i);
  }
  WRITE_CODE("call %s\n", func_call->name);
}

void x86_64_codegen_assign(assign_t* assign, x86_64_codegen_context* ctx) {
  // sprintf(ctx->code_segment_source, "<assign goes here>\n");
  // NOTE: This is getting ahead of yourself, we need to register variables and their data in a hashtable before any of this
  if ((assign->type & ASSIGN_LHS_VAR) && (assign->type & ASSIGN_RHS_STR_LIT)) {
     x86_64_codegen_strlit(assign->right_hand_side.str_lit, ctx);
     WRITE_CODE("mov qword [rbp - %d], str%d\n", (ctx->var_count + 1) * 8, ctx->string_count);
     ctx->var_count++;
  }
}

void x86_64_codegen_asm_block(asm_block_t* asm_block, x86_64_codegen_context* ctx) {
  int length = asm_block->asm_source_code_end - asm_block->asm_source_code_begin;
  WRITE_CODE("%.*s\n", length - 1, asm_block->asm_source_code_begin);
}
