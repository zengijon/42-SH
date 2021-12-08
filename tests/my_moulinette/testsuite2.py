from argparse import ArgumentParser
from pathlib import Path
from difflib import unified_diff

import subprocess as sp
from typing import cast
import termcolor
import yaml

basic_files = ["if_basic_tests.yml", "cmd_var.yml"]
error_files = ["if_error_tests.yml", "cmd_error_var.yml"]
hard_files = []
script_files = ["shell_script/cmd_var/arg_basics.sh", "shell_script/cmd_var/nega.sh", "shell_script/cmd_var/IFS_1.sh"]

TEST_OK = f"[ {termcolor.colored('OK', 'green')} ]"
TEST_KO = f"[ {termcolor.colored('KO', 'red')} ]"

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
    return to_compare == compared

def my_diff(expected: str, actual: str):
    expected_lines = expected.splitlines(keepends=True)
    actual_lines = actual.splitlines(keepends=True)
    return "".join(unified_diff(expected_lines, actual_lines, fromfile='expected', tofile='actual'))


def running(shell: str, stdin: str) -> sp.CompletedProcess:
    return sp.run([shell], input=stdin, capture_output=True, text=True)


def running_process(shell: str, stdin: str) -> sp.CompletedProcess:
    return sp.run([shell, "-c", stdin], capture_output=True, text=True)


def my_check_output(expected: sp.CompletedProcess, actual: sp.CompletedProcess, flag: int, name: str, input: str):
    if (expected.returncode != actual.returncode):
        flag += 1
    if (expected.stdout != actual.stdout):
        flag += 1
    if (expected.stderr != actual.stderr):
        flag += 1

    if (flag == 0):
        print(f"{TEST_OK} {name}")
    else:
        print(f"{TEST_KO} {name}\n")
        print(f"INPUT: {input}")
        if (expected.returncode != actual.returncode):
            print(
                f"\n{RED_DOT} Exit with return code -> {actual.returncode}, and -> {expected.returncode} was expected")
        if (expected.stdout != actual.stdout):
            print(f"\n{RED_DOT} Stdout is not the same\n{ACTUAL}\n{actual.stdout}\n{EXPECTED}\n{expected.stdout}")
        if (expected.stderr != ""):  # PR VOIR SI UN MSG DERREUR EST ATTENDU
            if (check_stderr(actual.stderr) == False):  # CHECK SI LE MSG DERREUR COMMENCE PAR '42sh:'
                print(f"\n{RED_DOT} Stderr is not the same\n {ACTUAL}\n{actual.stderr}\n{EXPECTED}\n"
                      )


if __name__ == "__main__":
    parser = ArgumentParser("TestSuite")
    parser.add_argument("--binary", required=True, type=Path)
    arg = parser.parse_args()

    path_42sh = arg.binary.absolute()
    print(f"We are testing the following binary -> {path_42sh}")

    if (type_of_test == "basic"):
        tests_files = basic_files

    if (type_of_test == "error"):
        tests_files = error_files

    if (type_of_test == "script"):
        tests_files = script_files

    if (type_of_test == "hard"):
        tests_files = hard_files

        #input_42sh = "-c "
        #input_42sh = "\"" + our_input + "\""
        print(our_input)
        #res = str(path_42sh) + " " + input_42sh

        process_42sh = running_process(path_42sh, our_input)

        with open(file_test, "r") as our_yaml:
            tests_list = list(yaml.safe_load(our_yaml))

        for test in tests_list:
            if cat != test["category"]:
                cat = test["category"]
                print(f"\n============{cat}============")

            our_input = test["input"]
            name = test["name"]

            process_dash = running("dash", our_input)
            if type_of_test == "script":
                process_42sh = running(path_42sh, our_input)
            else:
                process_42sh = running_process(path_42sh, our_input)
            my_check_output(process_dash, process_42sh, 0, name, our_input)
