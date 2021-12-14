from argparse import ArgumentParser
from pathlib import Path
from difflib import unified_diff

import subprocess as sp
from typing import cast
import termcolor
import yaml

basic_files = ["if_basic_tests.yml", "cmd_var.yml", "cd_tests_basic.yml", "cmd_for_tests.yml", "cmd_pipe.yml", "function_basic.yml", "pipe_basic_tests.yml", "exit.yml", "export.yml"]
error_files = ["if_error_tests.yml", "cmd_error_var.yml"]
echo_files = ["cmd_echo_tests.yml"]
script_files = ["shell_script/cmd_var/arg_basics.sh", "shell_script/cmd_var/nega.sh", "shell_script/cmd_var/IFS_1.sh", "shell_script/complex_script/pipe_if_for.sh", 
                "shell_script/complex_script/pipe_if_for_2.sh", "shell_script/complex_script/pipe_if_for_3.sh", "shell_script/redir/pipe.sh", "shell_script/redir/redir_basics.sh", "shell_script/for/simple_for_1.sh"]

TEST_OK = f"[ {termcolor.colored('OK', 'green')} ]"
TEST_KO = f"[ {termcolor.colored('KO', 'red')} ]"
RED_DOT = f"{termcolor.colored('*', 'red')}"

ACTUAL = f"{termcolor.colored('---ACTUAL---', 'blue')}"
EXPECTED = f"{termcolor.colored('---EXPECTED---', 'magenta')}"


def check_stderr(stderr_output: str):
    length = len(stderr_output)
    if (length < 6):
        return False
    to_compare = ""
    compared = "42sh:"
    i = 0
    while (i < 6):
        to_compare += stderr_output[i]
        i += 1
    return True


def my_diff(expected: str, actual: str):
    expected_lines = expected.splitlines(keepends=True)
    actual_lines = actual.splitlines(keepends=True)
    return "".join(unified_diff(expected_lines, actual_lines, fromfile='expected', tofile='actual'))


def running(shell: str, stdin: str) -> sp.CompletedProcess:
    return sp.run([shell], input=stdin, capture_output=True, text=True, timeout=1, shell=True)


def running_process(shell: str, stdin: str) -> sp.CompletedProcess:
    return sp.run([shell, "-c", stdin], capture_output=True, text=True, timeout=1)


def my_check_output(expected: sp.CompletedProcess, actual: sp.CompletedProcess, flag: int, name: str, input: str):
    if (expected.returncode != actual.returncode):
        flag += 1
    if (expected.stdout != actual.stdout):
        flag += 1

    if (flag == 0):
        print(f"{TEST_OK} {name}\n")
    else:
        print(f"{TEST_KO} {name}\n")
        print(f"INPUT: {input}")
        if (expected.returncode != actual.returncode):
            print(
                f"\n{RED_DOT} Exit with return code -> {actual.returncode}, and -> {expected.returncode} was expected")
        if (expected.stdout != actual.stdout):
            print(f"\n{RED_DOT} Stdout is not the same\n{ACTUAL}\n{actual.stdout}\n{EXPECTED}\n{expected.stdout}")


if __name__ == "__main__":
    parser = ArgumentParser("TestSuite")
    parser.add_argument("--binary", required=True, type=Path)
    parser.add_argument("--type", required=True, type=str)
    arg = parser.parse_args()
    path_42sh = arg.binary.absolute()
    type_of_test = arg.type
    print(f"We are testing the following binary -> {path_42sh}")

    if (type_of_test == "basic"):
        tests_files = basic_files

    elif (type_of_test == "error"):
        tests_files = error_files

    elif (type_of_test == "script"):
        tests_files = script_files

    elif (type_of_test == "echo"):
        tests_files = echo_files
    else:
    	tests_files = []
    	print("Wrong Type !!\n")

    cat = ""



    for file_test in tests_files:

        if type_of_test == "script":
            list_path = file_test.split("/")
            name_script = list_path[len(list_path) - 1]

            flag_timeout = 0

            try:
                process_dash = running("dash", file_test)
            except sp.TimeoutExpired:
                flag_timeout += 1

            try:
                process_42sh = sp.run(['./42sh', file_test], capture_output=True, text=True, timeout=1)
            except sp.TimeoutExpired:
                flag_timeout += 1

            if flag_timeout == 1:
                print(f"{TEST_KO} {name_script}\n One of them timed out...\n")
            elif flag_timeout == 2:
                print(f"{TEST_OK} TIME OUT : {name_script}")
            else:
                my_check_output(process_dash, process_42sh, 0, name_script, file_test)

        else:


            with open(file_test, "r") as our_yaml:
                tests_list = list(yaml.safe_load(our_yaml))

            for test in tests_list:
                flag_timeout = 0
                if cat != test["category"]:
                    cat = test["category"]
                    print(f"\n============{cat}============")

                our_input = test["input"]
                name = test["name"]

                try:
                    process_dash = running("dash", our_input)
                except sp.TimeoutExpired:
                    flag_timeout += 1

                try:
                    process_42sh = running_process(path_42sh, our_input)
                except sp.TimeoutExpired:
                    flag_timeout += 1

                if flag_timeout == 1:
                    print(f"{TEST_KO} {name}\n")
                elif flag_timeout == 2:
                    print(f"{TEST_OK} {name}")
                else:
                    my_check_output(process_dash, process_42sh, 0, name, our_input)
