#ifndef CODEGEN_x86_64_H
#define CODEGEN_x86_64_H
#include "parser2.h"

typedef struct {
  int scope_depth;
  char* code_segment_source; int currently_written_code;
  char* data_segment_source; int currently_written_data;
  // var_dict_t var_dictionary;
} x86_64_codegen_context;

void x86_64_codegen_entry_stub(x86_64_codegen_context*);

void x86_64_codegen_program     (program_t*, FILE*); 
void x86_64_codegen_use         (use_t* u, x86_64_codegen_context*);
void x86_64_codegen_asm_block   (asm_block_t*, x86_64_codegen_context*);
void x86_64_codegen_block       (block_t*, x86_64_codegen_context*);
void x86_64_codegen_func        (func_t*, x86_64_codegen_context*);
int  x86_64_codegen_func_decl   (func_decl_t*, x86_64_codegen_context*);
void x86_64_codegen_func_call   (func_call_t*, x86_64_codegen_context*);
void x86_64_codegen_strlit      (string_lit_t*, x86_64_codegen_context*);
void x86_64_codegen_var         (var_t*, x86_64_codegen_context*);
void x86_64_codegen_assign      (assign_t*, x86_64_codegen_context*);
void x86_64_codegen_return      (return_t*, x86_64_codegen_context*);
void x86_64_codegen_param_list  (param_list_t*, x86_64_codegen_context*);
void x86_64_codegen_arg_list    (arg_list_t*, x86_64_codegen_context*);
void x86_64_codegen_expression  (expression_t*, x86_64_codegen_context*);
void x86_64_codegen_statement   (statement_t*, x86_64_codegen_context*);
void x86_64_codegen_if          (if_t*, x86_64_codegen_context*);
void x86_64_codegen_condition   (condition_t*, x86_64_codegen_context*);

#endif
