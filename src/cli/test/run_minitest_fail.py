import argparse
import subprocess
from pathlib import Path

THIS_DIR = Path(__file__).parent.absolute()
MINITEST_FILE = THIS_DIR / "minitest_fail.rb"


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run a logger test.")
    parser.add_argument("ruby_or_cli_path", type=str, help="Path to the OS CLI or the System Ruby")
    args = parser.parse_args()

    if not MINITEST_FILE.is_file():
        raise ValueError(f"{MINITEST_FILE} does not exist")

    command = [str(args.ruby_or_cli_path), str(MINITEST_FILE)]

    print(f"Running: `{' '.join(command)}`")
    r = subprocess.run(
        command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, encoding="utf-8", universal_newlines=True
    )
    ret_code = r.returncode
    print(f"return code: {ret_code}")
    print(f"stdout:\n```\n{r.stdout}\n```")
    print(f"stderr:\n```\n{r.stderr}\n```")

    assert ret_code == 1, f"Expected the return code to be 1, got {ret_code}"
    assert "1 failures" in r.stdout, f"stdout appears to be missing the minitest output.\n\n{r.stdout}"
