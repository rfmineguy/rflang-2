cmake_minimum_required(VERSION 3.20)

project(rfc LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -Wall")

file (COPY code DESTINATION ${CMAKE_BINARY_DIR})

add_executable(rfc
  src/main.c
  # src/parser.c
  # src/token_vector.c
  # src/tokenizer.c
  src/parser2.c
  src/token.c
  src/tokenizer2.c)
