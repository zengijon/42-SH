cmake_minimum_required(VERSION 3.19)
project(42sheeee C)
add_compile_options(-Wextra -Wall -pedantic -Werror)
set(CMAKE_C_STANDARD 99)


file(GLOB sources src/exec_builtins/exec_cmds.c src/exec_builtins/microshell.c src/memory/hmalloc.c src/memory/free_list.c src/exec_builtins/echo.c)


add_executable(42sheee ${sources})
target_link_libraries(42sheee PUBLIC -lcriterion)