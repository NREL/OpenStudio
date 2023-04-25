import argparse
from pathlib import Path
import subprocess


def validate_file(arg):
    if (filepath := Path(arg)).is_file():
        return filepath
    else:
        raise FileNotFoundError(arg)

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Run a logger test.")
    parser.add_argument(
        "os_cli_path", type=validate_file, help="Path to the OS CLI"
    )
    parser.add_argument('--labs', action='store_true')
    parser.add_argument(
        "logger_file", type=validate_file, help="Path to the logger test file to run"
    )
    args = parser.parse_args()
    print(args)

    command = [str(args.os_cli_path)]
    if args.labs:
        command.append("labs")
    if (ext := args.logger_file.suffix) == '.py':
        if not args.labs:
            raise ValueError("When supplying a .py file, you must pass --labs")
        command.append("execute_python_script")
    elif ext == '.rb':
        command.append("execute_ruby_script")
    else:
        raise ValueError(f"logger_file should have a .rb or .py extension, not {ext}")

    command.append(str(args.logger_file))
    print(f"Running: {' '.join(command)}")
    r = subprocess.check_output(command, encoding='utf-8')
    lines = r.splitlines()

    # Pop the labs box
    i_warn = 0
    for i, line in enumerate(lines):
        if "The `labs` command is experimental - Do not use in production" in line:
            i_warn = i
            break
    lines = lines[:(i_warn - 1)] + lines[(i_warn + 2):]

    for i, line in enumerate(lines):
        print(i, line)

    if (n := len(lines)) != 3:
        raise IOError(f"Expected 3 lines, got {n}")

    # Ruby when called this way has the openstudio logger messages first instead of last, so just sort
    lines.sort()
    assert lines == ['LOGGER - STDOUT Error', 'LOGGER - STDOUT Warn', '[test] <1> Error']
