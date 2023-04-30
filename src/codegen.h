#ifndef CODEGEN_H
#define CODEGEN_H
#include "parser2.h"
#include <stdio.h>
  
void codegen_select(program_t*, FILE*, const char*);

void codegen_program(program_t*, FILE*);
void codegen_use(use_t*, FILE*);
void codegen_func(func_t*, FILE*);

void codegen_entry(FILE*);

#endif
