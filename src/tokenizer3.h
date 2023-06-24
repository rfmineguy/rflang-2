#ifndef TOKENIZER3_H
#define TOKENIZER3_H
#include <stdio.h>
#include "token.h"

// MACROS
#define LOC_FIELD(tokenizer, len) \
  .loc = (token_loc_t) {\
    .begin_index = tokenizer->cursor - tokenizer->source_code,\
    .length = len,\
    .line = tokenizer->line,\
    .column = tokenizer->col,\
    .line_start = tokenizer->line_start,\
    .file = tokenizer->source_filename }

#define TOK_CHECK_CH(ch, tp) \
  if (*t->cursor == ch) {\
    t->history[4] = (token_t) {.type = tp, LOC_FIELD(t, 1) };\
    t->cursor++;\
    t->col++;\
    return;\
  }

#define TOK_CHECK_STR(str, len, tp) \
  if (strncmp(t->cursor, str, len) == 0) {\
    t->history[4] = (token_t) {.type = tp, LOC_FIELD(t, len) };\
    t->cursor+=len;\
    t->col+=len;\
    return;\
  }

#define TOK_CHECK_EOF(tok) \
  if (t->cursor >= t->source_code + t->source_length) {\
    t->history[4] = (token_t) {.type = T_EOF, LOC_FIELD(t, 1) };\
    t->cursor++;\
    t->col++;\
    return;\
  }

typedef struct {
  int type; //float or integer
  union {
    int i;
    float f;
  } value;
  int length;
} number_t;

typedef struct {
  char* begin;
  char* end;
  int length;
} identifier_t;

typedef struct {
  char* source_code;
  const char* source_filename;
  const char* module_name;
  int source_length;

  char* cursor;
  const char* line_start;
  int col, line;

  int unknown_ch_warning;

  token_t history[5];     // [0-1]->prev, [2]->current, [3-4]->next
} tokenizer3_t;

tokenizer3_t tokenizer3_new(const char*);
tokenizer3_t tokenizer3_new_from_str(const char*);
void         tokenizer3_free(tokenizer3_t*);

void         tokenizer3_advance(tokenizer3_t*);
token_t      tokenizer3_get(tokenizer3_t*, int);
int          tokenizer3_expect_offset(tokenizer3_t*, int, token_type_t);

number_t     tokenizer3_as_number(tokenizer3_t*);
identifier_t tokenizer3_as_id(tokenizer3_t*);

void         tokenizer3_consume_comments(tokenizer3_t*);
void         tokenizer3_consume_whitespace(tokenizer3_t*);

void         tokenizer3_show_history(tokenizer3_t*);
void         tokenizer3_show_token_offset(tokenizer3_t*, int);
const char*  tokenizer3_get_token_offset_as_string(tokenizer3_t*, int);
void         tokenizer3_token_print(token_t, tokenizer3_t*);

void         tokenizer3_enable_unknown_ch_warning(tokenizer3_t*);
void         tokenizer3_disable_unknown_ch_warning(tokenizer3_t*);

#endif
