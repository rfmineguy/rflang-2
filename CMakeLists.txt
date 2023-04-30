#
# CMAKE SETUP
#
cmake_minimum_required(VERSION 3.20)

project(rfc LANGUAGES C)
project(rfc-tests LANGUAGES C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -ggdb -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ggdb")

# file (COPY code DESTINATION ${CMAKE_BINARY_DIR})

#
# LIBRARIES
#
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

FetchContent_Declare(
  gtest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.13.0
)
FetchContent_GetProperties(gtest)
if (NOT gtest_POPULATED)
  FetchContent_Populate(gtest)
  message(${gtest_SOURCE_DIR})
  add_subdirectory(${gtest_SOURCE_DIR})
endif()

set (SOURCES 
  src/parser2.c
  src/token.c
  src/codegen.c
  src/codegen_x86_64.c
  src/errors.c
  src/tokenizer2.c)

add_executable(rfc-tests
  tests/test.cpp
  ${SOURCES})

target_link_libraries(rfc-tests PRIVATE argparse m gtest_main)
target_include_directories(rfc-tests PRIVATE ${argparse_SOURCE_DIR} ${gtest_SOURCE_DIR})

add_executable(rfc
  src/main.c
  ${SOURCES})

target_link_libraries(rfc PRIVATE argparse m)
target_include_directories(rfc PRIVATE ${argparse_SOURCE_DIR})

# cd buid; sudo make install
install(TARGETS rfc)
