#ifndef COMPILE_H
#define COMPILE_H
#include "args2.h"
#include "parsers/parser3.h"
#include "ht/chain_ht_str_module.h"
#include "ht/chain_ht_str_symbol.h"
#include <limits.h>

int         compile_all(args2_t);

void        parse_file_rec(const char*, args2_t*, chaining_ht_str_module_t);
module_t*   parse_file(const char*, args2_t*);

void        analyze_file_rec(const char*, args2_t*, chaining_ht_str_module_t);

//TEMP
void        analyze_module_rec(const char*, args2_t*, chaining_ht_str_module_t, chaining_ht_str_symbol_t);
void        analyze_module_temp(module_t*, chaining_ht_str_symbol_t);
#endif
