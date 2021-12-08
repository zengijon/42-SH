from argparse import ArgumentParser
from pathlib import Path
from difflib import unified_diff

import subprocess as sp
import termcolor
import yaml

TEST_OK = f"\n[ {termcolor.colored('OK', 'green')} ]"
TEST_KO = f"\n[ {termcolor.colored('KO', 'red')} ]"


def my_diff(expected: str, actual: str):
    expected_lines = expected.splitlines(keepends=True)
    actual_lines = actual.splitlines(keepends=True)
    return "".join(unified_diff(expected_lines, actual_lines, fromfile='expected', tofile='actual'))


def running(shell: str, stdin: str) -> sp.CompletedProcess:
    return sp.run([shell], input=stdin, capture_output=True, text=True)


def check_output(expected: sp.CompletedProcess, actual: sp.CompletedProcess):
    assert expected.returncode == actual.returncode, \
        f"Exit with return code -> {actual.returncode}, and -> {expected.returncode} was expected"
    assert expected.stdout == actual.stdout, \
        f"Stdout is not the same\n{my_diff(expected.stdout, actual.stdout)}"
    assert expected.stderr == actual.stderr, \
        f"Stderr is not the same\n{my_diff(expected.stderr, actual.stderr)}"


if __name__ == "__main__":
    parser = ArgumentParser("TestSuite")
    parser.add_argument("--binary", required=True, type=Path)
    arg = parser.parse_args()

    path_42sh = arg.binary.absolute()
    print(f"We are testing the following binary -> {path_42sh}")

    with open("tests_def.yml", "r") as our_yaml:
        tests_list = list(yaml.safe_load(our_yaml))

    for test in tests_list:
        our_input = test["input"]
        name = test["name"]

        process_dash = running("dash", our_input)
        process_42sh = running(path_42sh, our_input)

        try:
            check_output(process_dash, process_42sh)
        except AssertionError as err:
            print(f"{TEST_KO} {name}\n{err}")
        else:
            print(f"{TEST_OK} {name}")
