#ifndef ERROR2_H
#define ERROR2_H
#include <stdlib.h>
#include <stdio.h>
#include "../tokenizer3.h"

/* Error format
 *   [file, line] <error type>: <line contents>
 *                        ^ <error description>
 * =================================================================================
 *   [src/main.c, 45] Error: a int = 5;
 *                          ^ <missing colon in variable declaration>
 */

/* Usage
 *
 * error_push(error_new(error_type, source, filename, line, col));
 */

typedef struct error_t error_t;

typedef enum {
  E_INVALID_START_TOKEN,
  E_GENERAL_EXPECT,
  E_MISSING_SEMICOLON,
  E_MISSING_USE_PATH,
  E_MISSING_LB,
  E_STATEMENT,
  E_MISSING_FUNC_RETURN_TYPE,
  E_MISSING_ASSIGN_EQ,
  E_MISMATCHED_LP,
  E_EXPECTED_TOKEN_GOT_TOKEN,
} error_type_t;

struct error_t {
  error_type_t type;
  const char* filename;
  const char* line_start;
  int line_number, col_number;
  int debug_line_number;
  const char* debug_function;
  union {
    struct {
      const char* expected_token_class;
      token_t got;
    } expect_got;
  } value;
  token_t token;
  error_t* next;
};

typedef struct {
  error_t* error_ll;
} error_context_t;

#define error_new(error_type, token) error_new_int(error_type, token, __FUNCTION__, __LINE__)

#define MAKE_GENERAL_EXPECT_ERROR(message, got_token) \
  (error_t) {\
    .type = E_GENERAL_EXPECT,\
    .value.expect_got.expected_token_class=(#message),\
    .value.expect_got.got=(got_token),\
    .debug_function=(__FUNCTION__),\
    .debug_line_number=(__LINE__),\
    .filename=NULL,\
    .next=NULL\
  }\


#define ERROR_EXPECT_BUT_GOT(expect_token_class, got_token) \
  (error_t) {\
    .type=E_EXPECTED_TOKEN_GOT_TOKEN,\
    .value.expect_got.expected_token_class=(expect_token_class),\
    .value.expect_got.got=(got_token),\
    .debug_function=(__FUNCTION__),\
    .debug_line_number=(__LINE__),\
    .filename=NULL,\
    .next=NULL\
  }\

const char* error_type_fmt_str(error_type_t);

error_context_t error_context_new();
void error_context_free(error_context_t);

error_t error_expect_but_got(token_type_t, token_t);
error_t error_unknown_token(token_type_t, token_t);

error_t error_new_int(error_type_t, token_t, const char*, int);
void error_push(error_context_t*, error_t);

void error_show_all(error_context_t*);
void error_show(error_t);

#endif
