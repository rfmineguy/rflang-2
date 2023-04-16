cmake_minimum_required(VERSION 3.20)

project(rfc LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -Wall")

file (COPY code DESTINATION ${CMAKE_BINARY_DIR})

include (FetchContent)
FetchContent_Declare(
  argparse
  GIT_REPOSITORY https://github.com/cofyc/argparse.git
  GIT_TAG v1.1.0
)
FetchContent_GetProperties(argparse)
if (NOT argparse_POPULATED)
  FetchContent_Populate(argparse)
  message(${argparse_SOURCE_DIR})
  add_subdirectory(${argparse_SOURCE_DIR})
endif()

add_executable(rfc
  src/main.c
  # src/parser.c
  # src/token_vector.c
  # src/tokenizer.c
  src/parser2.c
  src/token.c
  src/codegen.c
  src/tokenizer2.c)

target_link_libraries(rfc PRIVATE argparse)

target_include_directories(rfc PRIVATE
  ${argparse_SOURCE_DIR}
)

# cd buid; sudo make install
install(TARGETS rfc)
