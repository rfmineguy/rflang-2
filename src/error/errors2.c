#include "errors2.h"

const char* error_type_str(error_type_t type) {
  switch (type) {
    case E_MISSING_SEMICOLON: return "Missing semicolon";
    case E_MISSING_USE_PATH: return "Missing path for use";
    case E_MISSING_LB: return "Missing '{' for block";
    case E_STATEMENT: return "Statement error";
    case E_MISSING_FUNC_RETURN_TYPE: return "Function missing return type";
    case E_MISSING_ASSIGN_EQ: return "Assignment missing '='";
    default: return "error_type_str(...)";
  }
}

error_context_t error_context_new() {
  error_context_t ctx = {0};
  ctx.error_ll = NULL;
  return ctx;
}

void error_context_free(error_context_t ctx) {
  error_t* curr = ctx.error_ll;
  while (curr != NULL) {
    error_t* temp = curr;
    curr = curr->next;
    free(temp);
    temp = NULL;
  }
}

error_t error_new_int(error_type_t type, token_t t, const char* debug_function, int debug_line_number) {//const char* line_start, const char* filename, int line, int col) {
  error_t e = {0};
  e.type = type;
  e.token = t;
  e.line_number = t.loc.line;
  e.col_number = t.loc.column;
  e.debug_line_number = debug_line_number;
  e.debug_function = debug_function;
  // e.filename = filename;
  // e.line_start = line_start;
  // e.line_number = line;
  // e.col_number  = col;
  e.next = NULL;
  return e;
}

void error_push(error_context_t* ctx, error_t e) {
  error_t* curr = ctx->error_ll;
  if (!curr) {
    ctx->error_ll = malloc(sizeof(error_t));
    *ctx->error_ll = e;
    return;
  }
  while (curr->next != NULL) {
    curr = curr->next;
  }
  curr->next = malloc(sizeof(error_t));
  *curr->next = e;
  // error_show(e);
}

void error_show_all(error_context_t* ctx) {
  fprintf(stderr, "==============\n");
  fprintf(stderr, "Errors below: \n");
  fprintf(stderr, "==============\n");
  error_t* curr = ctx->error_ll;
  while (curr != NULL) {
    error_show(*curr);
    curr = curr->next;
  }
}

void error_show(error_t err) {
  fprintf(stderr, "[%d, %s] ERROR : [%s, %d:%d] <%s>: ", err.debug_line_number, err.debug_function, err.filename, err.line_number, err.col_number, error_type_str(err.type));
  // fprintf(stderr, "line_start: %p, %c\n", err.token.loc.line_start);
  char* cur = (char*) err.token.loc.line_start;
  while (*cur != '\n' && *cur != '\0') {
    fputc(*cur, stderr);
    cur++;
  }
  printf("\n");
}
