#ifndef CODEGEN_H
#define CODEGEN_H
#include "../parsers/parser3.h"
#include <stdio.h>
  
void codegen_select(module_t*, FILE*, const char*);

void codegen_module(module_t*, FILE*);
void codegen_use(use_t*, FILE*);
void codegen_func(func_t*, FILE*);

void codegen_entry(FILE*);

#endif
