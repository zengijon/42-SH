from argparse import ArgumentParser
from pathlib import Path
from difflib import unified_diff

import subprocess as sp
from typing import cast
import termcolor
import yaml

TEST_OK = f"[ {termcolor.colored('OK', 'green')} ]"
TEST_KO = f"[ {termcolor.colored('KO', 'red')} ]"

ACTUAL = f"{termcolor.colored('---ACTUAL---', 'blue')}"
EXPECTED = f"{termcolor.colored('---EXPECTED---', 'magenta')}"


def my_diff(expected: str, actual: str):
    expected_lines = expected.splitlines(keepends=True)
    actual_lines = actual.splitlines(keepends=True)
    return "".join(unified_diff(expected_lines, actual_lines, fromfile='expected', tofile='actual'))


def running(shell: str, stdin: str) -> sp.CompletedProcess:
    return sp.run([shell], input=stdin, capture_output=True, text=True)

def running_process(shell: str, stdin: str) -> sp.CompletedProcess:
    return sp.run([shell, "-c",stdin], capture_output=True, text=True)

def check_output(expected: sp.CompletedProcess, actual: sp.CompletedProcess):
    assert expected.stdout == actual.stdout, \
        f"Stdout is not the same\n{ACTUAL}\n{actual.stdout}\n{EXPECTED}\n{expected.stdout}"
    assert expected.stderr == actual.stderr, \
        f"Stderr is not the same\n {ACTUAL}\n{actual.stderr}\n{EXPECTED}\n"


if __name__ == "__main__":
    parser = ArgumentParser("TestSuite")
    parser.add_argument("--binary", required=True, type=Path)
    arg = parser.parse_args()

    path_42sh = arg.binary.absolute()
    print(f"We are testing the following binary -> {path_42sh}")

    with open("tests_def.yml", "r") as our_yaml:
        tests_list = list(yaml.safe_load(our_yaml))

    cat_list = ["ECHO BASIC TESTS"]
    i = 0
    print(f"============ECHO TESTS============")
    for test in tests_list:
        cat = test["category"]
        if (cat_list[i] != cat):
            print(f"============{cat}============")
            cat_list.append(cat)
            i += 1

        our_input = test["input"]
        name = test["name"]

        process_dash = running("dash", our_input)

        #input_42sh = "-c "
        #input_42sh = "\"" + our_input + "\""
        print(our_input)
        #res = str(path_42sh) + " " + input_42sh

        process_42sh = running_process(path_42sh, our_input)

        try:
            check_output(process_dash, process_42sh)
        except AssertionError as err:
            print(f"{TEST_KO} {name}\n{err}")
        else:
            print(f"{TEST_OK} {name}")
