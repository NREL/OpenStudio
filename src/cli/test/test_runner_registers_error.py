import subprocess
from pathlib import Path

import pytest

from workflow_helpers import run_workflow


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
