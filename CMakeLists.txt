cmake_minimum_required(VERSION 3.19)
project(42sheeee C)
add_compile_options(-Wextra -Wall -pedantic -Werror)
set(CMAKE_C_STANDARD 99)


file(GLOB sources src/exec/exec.c src/exec_builtins/echo.c src/exec_builtins/microshell.c src/exec_builtins/exec_cmds.c src/exec_builtins/exec_cmds.c src/test/test_parser/print_parser.c src/builtin/echo/echo.c src/main/main.c src/utils/*.c src/lexer/lexer.c src/lexer/token.c src/lexer/utils.c src/memory/hmalloc.c src/memory/free_list.c src/parser/parser.c)

add_executable(42sheee ${sources})
target_link_libraries(42sheee PUBLIC)