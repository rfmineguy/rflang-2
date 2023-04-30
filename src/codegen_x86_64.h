#ifndef CODEGEN_x86_64_H
#define CODEGEN_x86_64_H
#include "parser2.h"

void x86_64_codegen_entry_stub(FILE*);

void x86_64_codegen_program(program_t*, FILE*); 
void x86_64_codegen_use(use_t* u, FILE* f);
void x86_64_codegen_asm_block(asm_block_t*, FILE*);
void x86_64_codegen_block(block_t*, FILE*);
void x86_64_codegen_func(func_t*, FILE*);
int  x86_64_codegen_func_decl(func_decl_t*, FILE*);
void x86_64_codegen_func_call(func_call_t*, FILE*);
void x86_64_codegen_var(var_t*, FILE*);
void x86_64_codegen_assign(assign_t*, FILE*);
void x86_64_codegen_return(return_t*, FILE*);
void x86_64_codegen_param_list(param_list_t*, FILE*);
void x86_64_codegen_arg_list(arg_list_t*, FILE*);
void x86_64_codegen_expression(expression_t*, FILE*);
void x86_64_codegen_statement(statement_t*, FILE*);
void x86_64_codegen_if(if_t*, FILE*);
void x86_64_codegen_condition(condition_t*, FILE*);

#endif
