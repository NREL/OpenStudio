import json
import shutil
import subprocess
from pathlib import Path

import pytest


def run_workflow(
    osclipath: Path,
    base_osw_name: str,
    suffix: str,
    is_labs: bool,
    verbose: bool = False,
    debug: bool = False,
    post_process_only: bool = False,
):
    base_osw_path = Path(base_osw_name).resolve()
    assert base_osw_path.is_file(), f"{base_osw_path=} is not found"

    osw = json.loads(base_osw_path.read_text())
    osw_path = base_osw_path.parent / f"{base_osw_path.stem}_{suffix}.osw"
    runDir = base_osw_path.parent / f"run_{osw_path.stem}"
    osw["run_directory"] = str(runDir)
    if runDir.is_dir():
        shutil.rmtree(runDir)
    runDir.mkdir(exist_ok=False)
    with open(osw_path, "w") as f:
        json.dump(osw, fp=f, indent=2, sort_keys=True)

    command = [str(osclipath)]
    if not is_labs:
        command.append("classic")
    if verbose:
        command.append("--verbose")
    command.append("run")
    if post_process_only:
        # Fake having an in.idf or it won't run in the "classic" subcommand
        # Doing it for labs too so that it's less confusing
        with open(runDir / "in.idf", "w") as f:
            f.write("Building,;")
        command.append("--postprocess_only")
    if debug:
        command.append("--debug")
    command += ["-w", str(osw_path)]
    # print(f"Running command: {' '.join(command)}")
    return runDir, subprocess.run(command, capture_output=True, encoding="utf-8")


@pytest.mark.parametrize(
    "is_labs",
    [pytest.param(True, id="labs"), pytest.param(False, id="classic")],
)
def test_reportingmeasure(osclipath, is_labs: bool):
    suffix = "labs" if is_labs else "classic"
    runDir, r = run_workflow(
        osclipath=osclipath,
        base_osw_name="reportingmeasure.osw",
        suffix=suffix,
        is_labs=is_labs,
        verbose=False,
        debug=True,
        post_process_only=True,
    )
    assert r.returncode == 1
    assert "" != r.stdout
    if is_labs:
        assert (
            r.stderr
            == """Failed to run workflow. Last Error:
  Found error in state 'ReportingMeasures' with message: 'Measure 'ReportingMeasureRegistersError' reported an error with [runner.registerError called]'
"""
        )
    assert runDir.is_dir()
    with pytest.raises(subprocess.CalledProcessError):
        r.check_returncode()
    run_log_path = runDir / "run.log"
    assert run_log_path.is_file()
    run_log = run_log_path.read_text()
    assert "runner.registerError called" in run_log
    assert "runner.registerWarning called" not in run_log  # ReportingMeasures after the one that failed aren't run
    assert "Gathering reports" in run_log  # We still do Post Processing


@pytest.mark.parametrize(
    "is_labs",
    [pytest.param(True, id="labs"), pytest.param(False, id="classic")],
)
def test_modelmeasure(osclipath, is_labs: bool):
    suffix = "labs" if is_labs else "classic"
    runDir, r = run_workflow(
        osclipath=osclipath,
        base_osw_name="modelmeasure.osw",
        suffix=suffix,
        is_labs=is_labs,
        verbose=False,
        debug=True,
        post_process_only=False,
    )
    assert r.returncode == 1
    assert "" != r.stdout
    if is_labs:
        assert (
            r.stderr
            == """Failed to run workflow. Last Error:
  Found error in state 'OpenStudioMeasures' with message: 'Measure 'ModelMeasureRegistersError' reported an error with [runner.registerError called]'
"""
        )
    assert runDir.is_dir()
    with pytest.raises(subprocess.CalledProcessError):
        r.check_returncode()
    run_log_path = runDir / "run.log"
    assert run_log_path.is_file()
    run_log = run_log_path.read_text()
    assert "runner.registerError called" in run_log
    assert (
        "runner.registerWarning called" not in run_log
    )  # It just stop there, so no more measures after the one that failed aren't run
    assert "Gathering reports" not in run_log  # And we do not Post Processing
