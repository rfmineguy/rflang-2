#ifndef FILE_UTIL_H
#define FILE_UTIL_H
#include "args2.h"

int file_util_check_input_modules_exist(args2_t* args, const char** failed_file);
int file_util_stdlib_module_exists(const char* module_name, char* module_full_path, int max_length);
int file_util_local_module_exists(const char* module_name, char* module_full_path, int max_length);

#endif
