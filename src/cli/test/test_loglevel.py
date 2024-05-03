import subprocess
from pathlib import Path
from typing import List

import pytest

LOG_LEVELS = ["Trace", "Debug", "Info", "Warn", "Error", "Fatal"]

RUBY_PROGRAM = (
    f"{LOG_LEVELS=}; LOG_LEVELS.each_with_index {{ |s, i| OpenStudio::logFree(OpenStudio::Trace + i, 'test', s) }}"
)
PYTHON_PROGRAM = (
    f"{LOG_LEVELS=}; [openstudio.logFree(openstudio.Trace + i, 'test', s) for i, s in enumerate(LOG_LEVELS)]"
)


def remove_classic_box(lines: List[str]) -> List[str]:
    """Pop the classic box."""
    i_warn = None
    for i, line in enumerate(lines):
        if "The `classic` command is deprecated and will be removed in a future release" in line:
            i_warn = i
            break
    if i_warn is not None:
        lines = lines[: (i_warn - 1)] + lines[(i_warn + 2) :]
    return lines


def get_loglevel_list_index(logLevel: str) -> int:
    return LOG_LEVELS.index(logLevel)


def get_loglevel_int_value(logLevel: str) -> int:
    return LOG_LEVELS.index(logLevel) - 3


@pytest.mark.parametrize("language", ["ruby", "python"])
@pytest.mark.parametrize("logLevel", LOG_LEVELS)
def test_loglevel(osclipath: Path, language: str, logLevel: str):
    list_index = get_loglevel_list_index(logLevel=logLevel)
    loglevel_int_value = get_loglevel_int_value(logLevel=logLevel)
    args = [str(osclipath), "--loglevel", logLevel]
    if language == "ruby":
        args += ["-e", RUBY_PROGRAM]
    else:
        args += ["-c", PYTHON_PROGRAM]
    lines = subprocess.check_output(args, encoding="utf-8").splitlines()
    lines = remove_classic_box(lines)
    if logLevel in ['Trace', 'Debug']:
        assert f"[openstudio.CLI] <-2> Setting Log Level to {logLevel} ({loglevel_int_value})" in lines
    for i, msgLevel in enumerate(LOG_LEVELS):
        msg = f"[test] <{get_loglevel_int_value(msgLevel)}> {msgLevel}"

        if i < list_index:
            assert msg not in lines
        else:
            assert msg in lines


@pytest.mark.parametrize(
    "language, is_labs",
    [
        ["ruby", False],
        ["ruby", True],
        # ["python", False], // Not possible
        ["python", True],
    ],
)
def test_run_logger_file(osclipath, language: str, is_labs: bool):
    command = [str(osclipath)]
    if not is_labs:
        command.append("classic")
    if language == "ruby":
        command.append("execute_ruby_script")
        command.append(Path(__file__).parent / "logger_test.rb")
    elif language == "python":
        command.append("execute_python_script")
        command.append(Path(__file__).parent / "logger_test.py")
    else:
        raise ValueError("Can only use ruby or python")

    r = subprocess.check_output(command, encoding="utf-8")
    lines = r.splitlines()
    lines = remove_classic_box(lines)

    # Ruby when called this way has the openstudio logger messages first instead of last, so just sort
    lines.sort()
    assert lines == ["LOGGER - STDOUT Error", "LOGGER - STDOUT Warn", "[test] <1> Error"]
