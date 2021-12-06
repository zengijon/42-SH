from argparse import ArgumentParser
from pathlib import Path
from difflib import unified_diff

import subprocess as sp
from typing import cast
import termcolor
import yaml

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
    return to_compare == compared


def my_diff(expected: str, actual: str):
    expected_lines = expected.splitlines(keepends=True)
    actual_lines = actual.splitlines(keepends=True)
    return "".join(unified_diff(expected_lines, actual_lines, fromfile='expected', tofile='actual'))


def running(shell: str, stdin: str) -> sp.CompletedProcess:
    return sp.run([shell], input=stdin, capture_output=True, text=True)

def running_process(shell: str, stdin: str) -> sp.CompletedProcess:
    return sp.run([shell, "-c",stdin], capture_output=True, text=True)

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
            print(f"\n{RED_DOT} Exit with return code -> {actual.returncode}, and -> {expected.returncode} was expected")
        if (expected.stdout != actual.stdout):
            print(f"\n{RED_DOT} Stdout is not the same\n{ACTUAL}\n{actual.stdout}\n{EXPECTED}\n{expected.stdout}") 
        if (expected.stderr != ""): #PR VOIR SI UN MSG DERREUR EST ATTENDU
            if (check_stderr(actual.stderr) == False): #CHECK SI LE MSG DERREUR COMMENCE PAR '42sh:'
                print(f"\n{RED_DOT} Stderr is not the same\n {ACTUAL}\n{actual.stderr}\n{EXPECTED}\n"
)


#def check_output(expected: sp.CompletedProcess, actual: sp.CompletedProcess):
#    assert expected.returncode == actual.returncode, \
 #       f"Exit with return code -> {actual.returncode}, and -> {expected.returncode} was expected"
  #  assert expected.stdout == actual.stdout, \
   #     f"Stdout is not the same\n{ACTUAL}\n{actual.stdout}\n{EXPECTED}\n{expected.stdout}"
    #assert expected.stderr == actual.stderr, \
     #   f"Stderr is not the same\n {ACTUAL}\n{actual.stderr}\n{EXPECTED}\n"


if __name__ == "__main__":
    parser = ArgumentParser("TestSuite")
    parser.add_argument("--binary", required=True, type=Path)
    arg = parser.parse_args()

    path_42sh = arg.binary.absolute()
    print(f"We are testing the following binary -> {path_42sh}")

    with open("cmd_echo_tests.yml", "r") as our_yaml:
        tests_list = list(yaml.safe_load(our_yaml))

    cat_list = ["ECHO BASIC TESTS"]
    i = 0
    print(f"\n============ECHO BASIC TESTS============")
    for test in tests_list:
        cat = test["category"]
        if (cat_list[i] != cat):
            print(f"\n============{cat}============")
            cat_list.append(cat)
            i += 1

        our_input = test["input"]
        name = test["name"]

        process_dash = running("dash", our_input)

        #input_42sh = "-c "
        #input_42sh = "\"" + our_input + "\""
        #print(our_input)
        #res = str(path_42sh) + " " + input_42sh
        #print(f"\n LE PATH: {path_42sh}, -c <{our_input}>\n")
        process_42sh = running_process(path_42sh, our_input)

        my_check_output(process_dash, process_42sh, 0, name, our_input)