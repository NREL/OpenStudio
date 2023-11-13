import json
import subprocess
from pathlib import Path

import pytest


@pytest.mark.parametrize(
    "is_labs",
    [pytest.param(True, id="labs"), pytest.param(False, id="classic")],
)
def test_run_with_analysis(osclipath, is_labs: bool):
    base_osw_path = Path("with_analysis.osw").resolve()
    assert base_osw_path.is_file(), f"{base_osw_path=} is not found"

    osw = json.loads(base_osw_path.read_text())
    suffix = "labs" if is_labs else "classic"
    osw_path = base_osw_path.parent / f"with_analysis_{suffix}.osw"
    runDir = base_osw_path.parent / f"run_{suffix}"
    osw["run_directory"] = str(runDir)
    runDir.mkdir(exist_ok=True)
    with open(osw_path, "w") as f:
        json.dump(osw, fp=f, indent=2, sort_keys=True)

    # Fake having an in.idf or it won't run in the "classic" subcommand, doing it for labs too so that it's less
    # confusing
    # if not is_labs:
    with open(runDir / "in.idf", "w") as f:
        f.write("Building,;")

    command = [str(osclipath)]
    if not is_labs:
        command.append("classic")
    command += ["run", "--postprocess_only", "-w", str(osw_path)]
    lines = subprocess.check_output(command, encoding="utf-8").splitlines()

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

    assert set([x.name for x in runDir.glob("*")]) == expected_files_in_run_dir
