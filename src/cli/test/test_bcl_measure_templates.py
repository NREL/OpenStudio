"""Tests for the BCL Templates.

Calls the CLI to instantiate a given MeasureType/MeasureLanguage combination and runs the tests

Example Usage:

```
pytest --verbose --tb=short -rfEsxX --durations=0 test_bcl_measure_templates.py \
       --os-cli-path=~/Software/Others/OS-build/Products/openstudio
```
"""
import shlex
import shutil
import subprocess
from pathlib import Path
from typing import List
import os

import pytest

VERBOSE = True
MEASURE_LANGUAGES = ['Ruby', 'Python']
MEASURE_TYPES = ['ModelMeasure', 'EnergyPlusMeasure', 'ReportingMeasure']


def get_measure_dir(measure_type: str, measure_language: str) -> Path:
    return (Path('./bcl_measures') / f'test_{measure_language}_{measure_type}'.lower()).absolute()


def _run_subprocess_and_capture(cmd, env=None):
    try:
        o = subprocess.check_output(
                cmd, stderr=subprocess.PIPE,
                env=env,
                universal_newlines=True, encoding='utf-8',
            )
    except subprocess.CalledProcessError as e:
        print(f'exit code: {e.returncode}')
        print(f'stdout: {e.output}')
        print(f'stderr: {e.stderr}')
        raise e

    return o


def get_cmd(osclipath: Path, measure_type: str, measure_language: str) -> List[str]:
    return [
        str(osclipath), 'measure', 'new',
        '--class-name', f'MyExample{measure_language}{measure_type}',
        '--type', measure_type,
        '--language', measure_language,
        '--name', f'My {measure_language} {measure_type} Measure',
        '--description', 'This is my measure',
        '--modeler-description', 'This does complicated stuff',
        '--taxonomy-tag', 'Envelope.Form',
        # Output Directory
        str(get_measure_dir(measure_type=measure_type, measure_language=measure_language))
    ]


def run_pytest(osclipath: Path, measure_dir_path: Path, verbose: bool = False):
    python_test = next(measure_dir_path.glob('tests/*.py'))
    # test_cmd = ['pytest', str(python_test)]
    cmd = [str(osclipath), str(python_test)]
    if verbose:
        print("Running Pytest:")
        print(f"cmd = {shlex.join(cmd)}")
    output = _run_subprocess_and_capture(cmd) # env=dict(os.environ, PYTHONPATH=str(measure_dir_path))
    if verbose:
        print(output)


def run_minitest(osclipath: Path, measure_dir_path: Path, verbose: bool = False):
    # This runs style and rubocop, which I don't want
    # test_cmd = [str(osclipath), 'measure', '--run_tests', str(measure_dir_path)]
    ruby_test = next(measure_dir_path.glob('tests/*.rb'))
    cmd = [str(osclipath), str(ruby_test)]  # This is rather slow in Debug
    if verbose:
        print("Running MiniTest:")
        print(f"cmd = {shlex.join(cmd)}")

    output = _run_subprocess_and_capture(cmd)
    if verbose:
        print(output)


@pytest.mark.parametrize("measure_language", MEASURE_LANGUAGES)
@pytest.mark.parametrize("measure_type", MEASURE_TYPES)
def test_create_measure_and_run_tests(osclipath: Path, measure_type: str, measure_language: str, verbose: bool = VERBOSE):
    measure_dir_path = get_measure_dir(measure_type=measure_type, measure_language=measure_language)
    if measure_dir_path.is_dir():
        shutil.rmtree(measure_dir_path)

    cmd = get_cmd(osclipath=osclipath, measure_type=measure_type, measure_language=measure_language)

    if verbose:
        print(f"Instantiating new measure {measure_language} - {measure_type}:")
        print(f"cmd = {shlex.join(cmd)}")
    output = _run_subprocess_and_capture(cmd)
    if verbose:
        print(output)

    if measure_language == 'Ruby':
        run_minitest(osclipath=osclipath, measure_dir_path=measure_dir_path, verbose=verbose)
    else:
        run_pytest(osclipath=osclipath, measure_dir_path=measure_dir_path, verbose=verbose)


if __name__ == "__main__":
    import sys
    if len(sys.argv) > 1:
        pytest.main([__file__] + sys.argv[1:])
    else:
        pytest.main()
