#ifndef TOKEN_H
#define TOKEN_H

// token_type_t
typedef enum {
  T_INVALID = -2,
  T_UNKNOWN = -1,
  T_NL, T_SPC, T_TAB,
  T_FN, T_PROC,
  T_IF, T_FOR, T_WHILE,
  T_ASM,
  T_RB, T_LB,
  T_RP, T_LP,
  T_EQ, T_NOT,
  T_COLON, T_SEMICOLON, T_COMMA,

  T_ARROW, T_USE, T_RETURN,
  
  // Types
  T_INT, T_SHT, T_CHR, T_DBL, T_FLT, T_BOOL,

  // Comparison
  T_GT, T_LT,
  
  // Quotes
  T_DQT, T_SQT,
  
  // Escape
  T_BACKSLASH,

  // Binary ops
  T_PLUS, T_MINUS, T_MUL, T_DIV, T_MOD,

  // Other
  T_NUM,
  T_ID,
  T_EOF
} token_type_t;

// token_loc_t
typedef struct {
  int begin_index, length;
  int line, column;
} token_loc_t;

// token_t
typedef struct {
  token_type_t type;
  token_loc_t loc;
} token_t;

const char* token_type_stringify(token_type_t);

#endif
