#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "parser2.h"
#include "chaining_ht.h"

/* NOTES:
 *  - analysis will include the following features
 *       1. type checking
 *       2. symbol resolution
 */

typedef struct {
  chaining_ht_t ht;
} analyze_context_t;

int analyze_program(program_t*);
int analyze_use(use_t*, analyze_context_t);

#endif
