cmake_minimum_required(VERSION 3.19)
project(42sheeee C)
add_compile_options(-Wextra -Wall -pedantic -Werror)
set(CMAKE_C_STANDARD 99)


file(GLOB sources src/test_parser/*.c src/lexer/lexer.c src/lexer/token.c src/lexer/utils.c src/memory/hmalloc.c src/memory/free_list.c src/parser/*.c)


add_executable(42sheee ${sources})
target_link_libraries(42sheee PUBLIC)