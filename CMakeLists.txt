#
# CMAKE SETUP
#
cmake_minimum_required(VERSION 3.20)

project(rfc LANGUAGES C)
project(rfc-tests LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -Wall") # -fsanitize=address")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

# file (COPY vim-syntax/rf.vim DESTINATION )
# file (COPY code DESTINATION ${CMAKE_BINARY_DIR})

#
# LIBRARIES
#
include (FetchContent)
FetchContent_Declare(gtest  GIT_REPOSITORY https://github.com/google/googletest.git GIT_TAG v1.13.0)
FetchContent_MakeAvailable(gtest)

## ============================================
##   Source files
## ============================================
set (SOURCES 
  src/token.c
  src/compile.c
  src/args2.c
  src/file_util.c
  src/logging/logger_w_channels.c
  src/parsers/parser3.c
  src/parsers/parse_tree.c
  src/expression_validation.c
  src/codegen/codegen.c
  src/codegen/codegen_x86_64.c
  src/error/errors2.c
  src/tokenizer3.c
  src/ht/chain_ht_str_vardat.c
  src/ht/chain_ht_str_module.c
  src/ht/chain_ht_str_symbol.c
  src/analysis.c)

## ============================================
##   Testing binary
## ============================================
set(TESTS ON)
if (TESTS)
add_executable(rfc-tests
  tests/test.cpp
  ${SOURCES})

target_link_libraries(rfc-tests PRIVATE m gtest_main) #argparse)
target_include_directories(rfc-tests PRIVATE ${argparse_SOURCE_DIR} ${gtest_SOURCE_DIR})
endif()
## ============================================

add_executable(rfc
  src/main.c
  ${SOURCES})

target_link_libraries(rfc PRIVATE m) 
target_include_directories(rfc PRIVATE src/)

install(TARGETS rfc)
install(FILES vim-syntax/rf.vim DESTINATION ~/.vim/syntax/)
install(FILES vim-syntax/ftdetect/rf.vim DESTINATION ~/.vim/ftdetect/)
install(DIRECTORY rfcstd DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}) #/usr/local/include/stdlib
