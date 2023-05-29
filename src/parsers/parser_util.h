#ifndef PARSER_BASE_H
#define PARSER_BASE_H

typedef struct program_t    program_t;
typedef struct use_t        use_t;
typedef struct asm_block_t  asm_block_t;
typedef struct block_t      block_t;
typedef struct func_t       func_t;
typedef struct func_decl_t  func_decl_t;
typedef struct func_call_t  func_call_t;
typedef struct var_t        var_t;
typedef struct string_lit_t string_lit_t;
typedef struct assign_t     assign_t;
typedef struct return_t     return_t;
typedef struct param_list_t param_list_t;
typedef struct arg_list_t   arg_list_t;
typedef struct expression_t expression_t;
typedef struct statement_t  statement_t;
typedef struct if_t         if_t;
typedef struct while_t      while_t;
typedef struct condition_t  condition_t;

typedef enum {
  X86_32_LINUX_ASM = 142, X86_64_LINUX_ASM, ARM64_ASM  // the order here needs to match the order of the normal token_type_t enum
} asm_type_t;
typedef enum {
  EXPR_NUM = 0, EXPR_STRING, EXPR_COMPOUND
} expr_type_t;
typedef enum {
  ASSIGN_LHS_VAR     = 1,
  ASSIGN_LHS_ID      = 2,
  ASSIGN_RHS_STR_LIT = 4,
  ASSIGN_RHS_EXPR    = 8,
} assign_type_t;
struct program_t {
  use_t**     use_list;
  int         use_list_count, use_list_max;
  func_t**    func_list;
  int         func_list_count, func_list_max;
};
struct use_t {
  char name[30];
};
struct block_t {
  statement_t** statements;
  int statement_count;
};
struct asm_block_t {
  char* asm_source_code_begin;
  char* asm_source_code_end;
  asm_type_t asm_type;
};
struct func_t {
  int          has_return_type;
  token_type_t return_type;
  const char*  return_type_str;
  func_decl_t* decl;
  block_t*     block;
};
struct func_decl_t {
  char name[30];
  param_list_t* params;
};
struct func_call_t {
  char name[30];
  arg_list_t* args;
};
struct var_t {
  char name[30];
  const char* type_name;
  int type;
  int indirection;
};
struct string_lit_t {
  char* str_lit_begin;
  char* str_lit_end;
};
struct assign_t {
  assign_type_t type;
  union {
    var_t* var;
    char id[30];
  } left_hand_side;
  union {
    expression_t* expr;
    string_lit_t* str_lit;
  } right_hand_side;
};
struct return_t {
  expression_t* expr;
};
struct param_list_t {
  var_t** params;
  int params_count;
};
struct expression_t {
  expr_type_t type;             // single value (number or id), or composed expression (expr '+' expr)
  union {
    token_type_t operation;
    int i;
    char s[30];
  } value;
  expression_t *left, *right; // ?
};
struct condition_t {
  token_type_t comparison;
  expression_t *left, *right;
};
struct arg_list_t {
  expression_t** args;
  int arg_count;
};
struct statement_t {
  if_t* iff;
  return_t* ret;
  func_call_t* func_call;
  assign_t* assign;
  asm_block_t* asm_block;
  while_t* whle;
};
struct if_t {
  expression_t* condition;
  block_t* block;
};
struct while_t {
  expression_t* condition;
  block_t* block;
};

#endif
