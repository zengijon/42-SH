cmake_minimum_required(VERSION 3.19)
project(42sh C)
add_compile_options(-Wextra -Wall -pedantic -Werror)
set(CMAKE_C_STANDARD 99)


file(GLOB sources src/redir/redir.c src/exec/*.c src/exec_builtins/builtins/alias.c src/exec_builtins/builtins/echo.c src/exec_builtins/microshell.c src/exec_builtins/exec_cmds.c src/exec_builtins/exec_cmds.c src/exec_builtins/builtins/my_cd.c src/test/test_parser/print_parser.c src/builtin/echo/echo.c src/main/main.c src/utils/*.c src/lexer/lexer.c src/lexer/token.c src/lexer/utils.c src/memory/hmalloc.c src/memory/free_list.c src/parser/parser.c src/utils/usefull_fonction.c src/utils/for_pwd.c src/utils/substit.c src/utils/substit.h src/exec_builtins/builtins/unset.c src/exec_builtins/builtins/my_exit.c src/exec_builtins/builtins/my_export.c)

#file(GLOB sources src/lexer/lexer.c src/lexer/token.c src/lexer/utils.c src/memory/hmalloc.c src/memory/free_list.c)

add_executable(42sh ${sources} )

target_link_libraries(42sh PUBLIC -fsanitize=address)
