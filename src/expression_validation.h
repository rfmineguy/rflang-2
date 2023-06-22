#ifndef EXPRESSION_VALIDATION_H
#define EXPRESSION_VALIDATION_H
#include "tokenizer3.h"
#include "error/errors2.h"

int expr_validate(const char* expr);
int validate_expression(const tokenizer3_t*, error_context_t*, int*);

#endif
