#include "string_util.h"
#include <math.h>
#include <ctype.h>
#include <stdlib.h>

int str_to_int(char* str) {
  int result = strtol(str, &(str + strlen(str)), 10);
  return result;
}
