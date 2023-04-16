#ifndef CODE_GEN_H
#define CODE_GEN_H
#include "parser2.h"
#include <stdio.h>

void codegen_program(program_t*, FILE*);
void codegen_use(use_t*, FILE*);
void codegen_func(func_t*, FILE*);

void codegen_entry(FILE*);

#endif
