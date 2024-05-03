import argparse
import subprocess
from pathlib import Path
from typing import Optional

THIS_DIR = Path(__file__).parent.absolute()
MINITEST_FAIL_FILE = THIS_DIR / "minitest_fail.rb"
MINITEST_OK_FILE = THIS_DIR / "minitest_ok.rb"
AT_EXIT_FILE = THIS_DIR / "at_exit.rb"
ALL_FILES = [MINITEST_FAIL_FILE, MINITEST_OK_FILE, AT_EXIT_FILE]


def run_file(
    ruby_or_cli_path: str,
    filepath: Path,
    expected_return_code: int,
    expected_stdout_message: Optional[str],
    verbose: bool = True,
):

    command = [str(ruby_or_cli_path), str(filepath)]

    if verbose:
        print(f"Running: `{' '.join(command)}`")

    r = subprocess.run(
        command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, encoding="utf-8", universal_newlines=True
    )
    ret_code = r.returncode
    if verbose:
        print(f"return code: {ret_code}")
        print(f"stdout:\n```\n{r.stdout}\n```")
        print(f"stderr:\n```\n{r.stderr}\n```")

    assert ret_code == expected_return_code, f"Expected the return code to be {expected_return_code}, got {ret_code}"
    if expected_stdout_message is not None:
        assert expected_stdout_message in r.stdout, f"stdout appears to be missing the minitest output.\n\n{r.stdout}"


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run a logger test.")
    parser.add_argument("ruby_or_cli_path", type=str, help="Path to the OS CLI or the System Ruby")
    args = parser.parse_args()

    for fp in ALL_FILES:
        if not fp.is_file():
            raise ValueError(f"{fp} does not exist")

    run_file(
        ruby_or_cli_path=args.ruby_or_cli_path,
        filepath=AT_EXIT_FILE,
        expected_return_code=12,
        expected_stdout_message=None,
    )

    run_file(
        ruby_or_cli_path=args.ruby_or_cli_path,
        filepath=MINITEST_OK_FILE,
        expected_return_code=0,
        expected_stdout_message="0 failures",
    )

    run_file(
        ruby_or_cli_path=args.ruby_or_cli_path,
        filepath=MINITEST_FAIL_FILE,
        expected_return_code=1,
        expected_stdout_message="1 failures",
    )

    r = subprocess.run(
        [str(args.ruby_or_cli_path), "-e", "at_exit { exit 12 }"],
        stdout=subprocess.PIPE, stderr=subprocess.PIPE, encoding="utf-8", universal_newlines=True
    )
    assert r.returncode == 12
