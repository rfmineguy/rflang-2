#ifndef TOKEN_H
#define TOKEN_H

// token_type_t
typedef enum {
  T_INVALID = -2,
  T_UNKNOWN = -1,
  T_NL = 0, T_SPC, T_TAB,
  T_FN = 10, T_PROC,
  T_IF = 20, T_FOR, T_WHILE,
  T_ASM = 30,
  T_RB = 40, T_LB,
  T_RP = 50, T_LP,
  T_EQ = 60, T_NOT,
  T_COLON = 70, T_SEMICOLON, T_COMMA,

  T_ARROW = 80, T_USE, T_RETURN,
  
  // Types
  T_INT = 90, T_SHT, T_CHR, T_DBL, T_FLT, T_BOOL,

  // Comparison
  T_GT = 100, T_LT,
  
  // Quotes
  T_DQT = 110, T_SQT,
  
  // Escape
  T_BACKSLASH = 120,

  // Binary ops
  T_PLUS = 130, T_MINUS, T_MUL, T_DIV, T_MOD,

  // Other
  T_NUM = 140,
  T_ID,
  T_X86_32_LINUX,
  T_X86_64_LINUX,
  T_ARM64,
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
  union {
    char s[30];
    int i;
  } value;
} token_t;

const char* token_type_stringify(token_type_t);

#endif
