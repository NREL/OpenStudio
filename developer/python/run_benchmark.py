#!/usr/bin/env python3

"""Run and plot results for a benchmark.

Usage:
  run_benchmark.py [--stacked]
                  [--products_dir=<prod_dir>]
                  [--relative]
                  [--skip-run]
                  [--quiet]
                  [--prefix=<PREFIX>]
                  (BENCH_NAME | --all)
  run_benchmark.py [--products_dir=<prod_dir>] (-l | --list)
  run_benchmark.py (-h | --help)

Options:
  -h --help     Show this screen.
  -l --list     List available benchmark executables
  -q, --quiet   Don't show the plot (which is blocking), just save it
  --stacked     Use a stacked bar chart of cpu_time & real_time
  --skip-run    Do not rerun, just replot the csv
  --prefix=<PREFIX>
                Prepend 'prefix_' to the name of the csv/png
  --relative    Relative
  -p <prod_dir>, --products_dir==<prod_dir>
                Provide a path to the products directory

Example:
  run_benchmark.py bench_basics
  run_benchmark.py --prefix ori --quiet --all
"""

from docopt import docopt
import re
import shlex
import stat
import subprocess
import typing

from pathlib import Path
import glob as gb
import pandas as pd
import matplotlib.pyplot as plt


ROOT_DIR = Path(__file__).parent.absolute()


def get_branch_and_sha() -> [str, str]:
    """
    Get source directory from CMakeCache.txt, recompute branch and sha
    the CMakeCache.txt has the branch and sha, but it may be outdated (if cmake
    didn't rerun explicitly)
    """
    with open(ROOT_DIR / 'CMakeCache.txt', 'r') as f:
        content = f.read()

    source_dir = re.search(r'CMAKE_HOME_DIRECTORY:INTERNAL=(.*)',
                           content).groups()[0]

    cmd_branch = shlex.split(f'git --git-dir={source_dir}/.git '
                             'rev-parse --abbrev-ref HEAD')
    branch = subprocess.check_output(cmd_branch).decode().strip()
    cmd_sha = shlex.split(f'git --git-dir={source_dir}/.git '
                          'log --pretty=format:"%h" -n 1')
    sha = subprocess.check_output(cmd_sha).decode().strip()
    return branch, sha


def infer_products_dir() -> Path:
    """
    Locate the build/Products/ directory under the current root

    Returns:
    --------
    * products_dir (pathlib.Path): the path to the Products directory
    """
    matches = gb.glob(str(ROOT_DIR / '**/Products/benchmark'), recursive=True)
    matches = [x for x in matches if Path(x).is_dir()]
    if len(matches) == 0:
        raise IOError(
            "Couldn't locate build dir, looked everywhere for a 'Products'"
            "directory but came up empty. Have you built the project?")
    if len(matches) > 1:
        print("Found multiple 'Products' directories, returning the first ="
              f"'{matches[0]}'"
              f"Other potential matches: {matches[1:]}")
    return Path(matches[0])


def is_executable(path: Path) -> bool:
    mode = path.stat().st_mode
    executable = stat.S_IEXEC | stat.S_IXGRP | stat.S_IXOTH
    return (mode & executable)


def is_benchmark(path_exe: Path) -> bool:
    cmd = shlex.split(f'{path_exe} --help')
    try:
        output = subprocess.check_output(cmd)
        return output.decode('utf-8').strip().lower().startswith('benchmark')
    except subprocess.CalledProcessError:
        pass
    except OSError:
        pass

    return False


def list_benchmarks(products_dir: Path) -> list[str]:
    """
    Lists the benchmarks inside a Products directory by looking at any file
    starting with bench_*

    Returns:
    --------

    bench_names: the names of the executables
    """

    matches = products_dir.glob('*')
    matches = filter(is_executable, matches)
    matches = filter(is_benchmark, matches)
    return matches


def format_list_benchmark(benchmark_list: list) -> str:
    msg = "The following benchmark executables are available:"
    for b in benchmark_list:
        msg += f"\n* {b}"
    return msg


def find_skip_rows(results_file: Path) -> int:
    """
    Open the result benchmark, and locate the actual start of the timings

    Returns:
    --------
    skiprows (int): the number of lines to skip when read_csv is called
    """
    skiprows = 0
    with open(results_file, 'r') as f:
        while ('name,iterations,' not in f.readline()):
            skiprows += 1

    return skiprows


def run_bench(bench_exe: Path,
              skip_run: bool = False,
              stacked: bool = False,
              prefix: typing.Optional[str] = None,
              quiet: bool = False):

    if not bench_exe.exists():
        msg = (f"Couldn't find a benchmark executable named {bench_exe} "
               f"in {products_dir}. ")
        msg += format_list_benchmark(list_benchmarks(products_dir))
        raise ValueError(msg)

    results_dir = root_dir / "results"
    results_dir.mkdir(exist_ok=True)

    bench_name = bench_exe.name
    if prefix:
        bench_name = f"{prefix}_{bench_name}"

    results_file = results_dir / f"{bench_name}.csv"

    if not skip_run:
        cmd = shlex.split(
            f'{bench_exe} --benchmark_out_format=csv'
            f' --benchmark_out="{results_file}"')
        subprocess.check_call(cmd)

        # Prepend branch + Sha
        branch, sha = get_branch_and_sha()
        with open(results_file, 'r') as original:
            data = original.read()
        with open(results_file, 'w') as modified:
            modified.write(f'Git Branch: {branch}\n')
            modified.write(f'Git SHA: {sha}\n')
            modified.write(data)

    df = pd.read_csv(results_file,
                     skiprows=find_skip_rows(results_file),
                     index_col=0)

    if df['error_occurred'].notnull().any():
        print("Some benchmarks seem to have failed...")
        print(f"{df.index[df['error_occurred'].notnull()].values}")
        print("continuining anyways")

    # Skip bigO/RMS if any
    df = df.loc[df['iterations'].notnull()]

    # Convert everything in ms
    for col in ['real_time', 'cpu_time']:
        df[col] = (df[[col, 'time_unit']].apply(
            lambda row: pd.to_timedelta(arg=row[0], unit=row[1]), axis=1)
                   .dt.total_seconds() * 1e3)

    if stacked:
        df['real_time'] = df['real_time'] - df['cpu_time']

    fig, ax = plt.subplots(figsize=(16, 9))

    df[['cpu_time', 'real_time']].plot(kind='barh', stacked=stacked, ax=ax)
    time_units = df['time_unit'].unique()
    ax.set_title(f"{bench_name} [ms]")
    if len(time_units) == 1:
        ax.set_ylabel(time_units[0])
    else:
        raise NotImplementedError(
            "There seems to be multiple time units, which I haven't "
            f"encountered before and therefore didn't treat: {time_units}"
        )
    fig.tight_layout()
    if not quiet:
        plt.show()
    fig.savefig(results_dir / f"{bench_name}.png", dpi=150)


if __name__ == '__main__':
    arguments = docopt(__doc__)
    # print(arguments)
    # exit(0)

    root_dir = Path(__file__).parent.absolute()
    products_dir = None
    if arguments['--products_dir']:
        products_dir = Path(arguments['--products_dir'])
    else:
        products_dir = infer_products_dir()

    if not products_dir.exists():
        raise IOError(f"Products directory {products_dir} does not exist")

    if arguments['--list']:
        benches = list_benchmarks(products_dir)
        benches = map(lambda p: p.relative_to(products_dir), benches)
        print(format_list_benchmark(benches))
        exit(0)

    benches = []
    if arguments['--all']:
        benches = list_benchmarks(products_dir)
    else:
        bench_name = arguments['BENCH_NAME']
        bench_exe = products_dir / bench_name
        benches = [bench_exe]

    for bench_exe in benches:
        run_bench(bench_exe=bench_exe, skip_run=arguments['--skip-run'],
                  stacked=arguments['--stacked'], prefix=arguments['--prefix'],
                  quiet=arguments['--quiet'])
