import json
import shutil
import subprocess
from enum import Enum
from pathlib import Path

import pytest


def run_workflow(osclipath: Path, suffix: str, is_labs: bool, verbose: bool = False, debug: bool = False):
    base_osw_path = Path("with_analysis.osw").resolve()
    assert base_osw_path.is_file(), f"{base_osw_path=} is not found"

    osw = json.loads(base_osw_path.read_text())
    osw_path = base_osw_path.parent / f"with_analysis_{suffix}.osw"
    runDir = base_osw_path.parent / f"run_{suffix}"
    osw["run_directory"] = str(runDir)
    if runDir.is_dir():
        shutil.rmtree(runDir)
    runDir.mkdir(exist_ok=False)
    with open(osw_path, "w") as f:
        json.dump(osw, fp=f, indent=2, sort_keys=True)

    # Fake having an in.idf or it won't run in the "classic" subcommand
    # Doing it for labs too so that it's less confusing
    with open(runDir / "in.idf", "w") as f:
        f.write("Building,;")
    command = [str(osclipath)]
    if not is_labs:
        command.append("classic")
    if verbose:
        command.append("--verbose")
    command += ["run", "--postprocess_only"]
    if debug:
        command.append("--debug")
    command += ["-w", str(osw_path)]
    # print(f"Running command: {' '.join(command)}")
    lines = subprocess.check_output(command, encoding="utf-8").splitlines()
    return runDir


# Only want to run the CLI call ONCE
@pytest.fixture(scope="module", params=[True, False], ids=["labs", "classic"])
def runWorkflow(osclipath, request):
    is_labs = request.param
    suffix = "labs" if is_labs else "classic"
    return run_workflow(osclipath=osclipath, suffix=suffix, is_labs=is_labs, verbose=False, debug=False), is_labs


def test_run_with_analysis(runWorkflow):
    runDir, _ = runWorkflow
    assert runDir.exists()
    measure_attributes_path = runDir / "measure_attributes.json"
    assert measure_attributes_path.is_file()
    results_path = runDir / "results.json"
    assert results_path.is_file()
    objectives_path = runDir / "objectives.json"
    assert objectives_path.is_file()

    measure_attributes = json.loads(measure_attributes_path.read_text())
    assert measure_attributes == {
        "FakeReport": {"applicable": True, "net_site_energy": 167.1, "something_with_invalid_chars": 1}
    }

    results = json.loads(results_path.read_text())
    assert results == {"FakeReport": {"applicable": True, "net_site_energy": 167.1, "something_with_invalid_chars": 1}}

    objectives = json.loads(objectives_path.read_text())
    assert objectives == {
        "objective_function_1": 167.1,
        "objective_function_3": 1.7976931348623157e308,
        "objective_function_group_1": 1.0,
        "objective_function_group_3": None,
        "objective_function_target_1": 0.0,
        "objective_function_target_3": None,
        "scaling_factor_1": 1.0,
        "scaling_factor_3": None,
    }

    data_point_out_path = runDir / "data_point_out.json"
    assert data_point_out_path.is_file()
    data_point_out = json.loads(data_point_out_path.read_text())
    assert data_point_out == {
        "FakeReport": {"applicable": True, "net_site_energy": 167.1, "something_with_invalid_chars": 1}
    }


def test_all_expected_files_in_run_dir(runWorkflow):
    expected_files_in_run_dir = {
        "data_point.zip",
        "finished.job",
        "in.idf",
        "measure_attributes.json",
        "objectives.json",
        "results.json",
        "run.log",
        "started.job",
        "data_point_out.json",
    }
    runDir, _ = runWorkflow
    assert set([x.name for x in runDir.glob("*")]) == expected_files_in_run_dir


# Don't want to import openstudio python bindings just for this so redefine the enum
class LogLevel(Enum):
    Trace = -3
    Debug = -2
    Info = -1
    Warn = 0
    Error = 1
    Fatal = 2


def _get_search_str(log_level: LogLevel, is_labs: bool) -> str:
    if is_labs:
        return f"<{log_level.value}>"
    else:
        return f"{log_level.name.upper()}]"


def _get_lines_with_log_level(run_log: str, log_level: LogLevel, is_labs: bool):
    search_str = _get_search_str(log_level=log_level, is_labs=is_labs)
    return [x for x in run_log.splitlines() if search_str in x]


def count_msg_with_log_level(run_log: str, log_level: LogLevel, is_labs: bool) -> int:
    return run_log.count(_get_search_str(log_level=log_level, is_labs=is_labs))


def test_run_log(runWorkflow):
    runDir, is_labs = runWorkflow

    # Test run.log for #5030
    run_log_path = runDir / "run.log"
    assert run_log_path.is_file()
    run_log = run_log_path.read_text()
    assert len(run_log.splitlines()) == 3
    assert "No valid weather file defined in either the osm or osw." in run_log
    assert "No results for objective function IsNonExisting.NonExisting" in run_log
    assert "runner.registerWarning called" in run_log

    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Trace, is_labs=is_labs) == 0
    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Debug, is_labs=is_labs) == 0
    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Info, is_labs=is_labs) == 0
    assert 1 < count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Warn, is_labs=is_labs) < 4
    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Error, is_labs=is_labs) == 0
    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Fatal, is_labs=is_labs) == 0


@pytest.mark.parametrize(
    "is_labs",
    [pytest.param(True, id="labs"), pytest.param(False, id="classic")],
)
def test_run_log_debug(osclipath, is_labs: bool):
    suffix = "labs" if is_labs else "classic"
    suffix += "_debug"
    runDir = run_workflow(osclipath=osclipath, suffix=suffix, is_labs=is_labs, verbose=False, debug=True)
    assert runDir.is_dir()
    run_log_path = runDir / "run.log"
    assert run_log_path.is_file()
    run_log = run_log_path.read_text()
    assert "No results for objective function IsNonExisting.NonExisting" in run_log
    assert "runner.registerWarning called" in run_log
    assert 50 < len(run_log.splitlines()) < 100

    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Trace, is_labs=is_labs) == 0
    assert 30 < count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Debug, is_labs=is_labs) < 40
    assert 20 < count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Info, is_labs=is_labs) < 50
    assert 1 < count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Warn, is_labs=is_labs) < 4
    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Error, is_labs=is_labs) == 0
    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Fatal, is_labs=is_labs) == 0


@pytest.mark.parametrize(
    "is_labs",
    [pytest.param(True, id="labs"), pytest.param(False, id="classic")],
)
def test_run_log_toplevel_verbose(osclipath, is_labs: bool):
    suffix = "labs" if is_labs else "classic"
    suffix += "_toplevel_verbose"
    runDir = run_workflow(osclipath=osclipath, suffix=suffix, is_labs=is_labs, verbose=True, debug=False)
    assert runDir.is_dir()
    run_log_path = runDir / "run.log"
    assert run_log_path.is_file()
    run_log = run_log_path.read_text()
    assert "No results for objective function IsNonExisting.NonExisting" in run_log
    assert "runner.registerWarning called" in run_log
    if not is_labs:
        pytest.skip("classic (Ruby) CLI just ignored the top level --verbose flag for run.log")
    assert 50 < len(run_log.splitlines()) < 100

    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Trace, is_labs=is_labs) == 0
    assert 30 < count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Debug, is_labs=is_labs) < 40
    assert 20 < count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Info, is_labs=is_labs) < 50
    assert 1 < count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Warn, is_labs=is_labs) < 4
    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Error, is_labs=is_labs) == 0
    assert count_msg_with_log_level(run_log=run_log, log_level=LogLevel.Fatal, is_labs=is_labs) == 0
