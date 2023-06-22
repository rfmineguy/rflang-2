#include "token.h"

const char* token_type_stringify(token_type_t t) {
  switch (t) {
    case T_EXPR_FUNC_END:return "FuncEnd";
    case T_INVALID:     return "Invalid";
    case T_UNKNOWN:     return "Unknown";
    case T_NL:          return "NL";
    case T_SPC:         return "SPC";
    case T_TAB:         return "TAB";
    case T_FN:          return "FN";
    case T_PROC:        return "PROC";
    case T_IF:          return "IF";
    case T_FOR:         return "FOR";
    case T_WHILE:       return "WHILE";
    case T_ASM:         return "ASM";
    case T_RB:          return "RB";
    case T_LB:          return "LB";
    case T_RP:          return "RP";
    case T_LP:          return "LP";
    case T_EQ:          return "EQ";
    case T_NOT:         return "NOT";
    case T_COLON:       return "COLON";
    case T_SEMICOLON:   return "SEMI";
    case T_COMMA:       return "COMMA";
    case T_ARROW:       return "ARROW";
    case T_USE:         return "USE";
    case T_RETURN:      return "RETURN";
    case T_INT:         return "INT";
    case T_SHT:         return "SHT";
    case T_CHR:         return "CHR";
    case T_DBL:         return "DBL";
    case T_FLT:         return "FLT";
    case T_BOOL:        return "BOOL";
    case T_VOID:        return "VOID";
    case T_GT:          return "GT";
    case T_LT:          return "LT";
    case T_DQT:         return "DQT";
    case T_SQT:         return "SQT";
    case T_BACKSLASH:   return "BCKSLSH";
    case T_PLUS:        return "PL";
    case T_MINUS:       return "MN";
    case T_MUL:         return "MUL";
    case T_DIV:         return "DIV";
    case T_MOD:         return "MOD";
    case T_NUM:         return "NUM";
    case T_ID:          return "ID";
    case T_X86_32_LINUX:return "X86_32_LINUX";
    case T_X86_64_LINUX:return "X86_64_LINUX";
    case T_ARM64:       return "ARM64";
    case T_EOF:         return "EOF";
    default:            return "Defaulted";
  }
}
