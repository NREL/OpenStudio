import json
import subprocess
from pathlib import Path

import pytest


@pytest.mark.parametrize(
    "is_labs",
    [pytest.param(True, id="labs"), pytest.param(False, id="classic")],
)
def test_run_with_analysis(osclipath, is_labs: bool):
    osw_path = Path("with_analysis.osw").resolve()
    assert osw_path.is_file(), f"{osw_path=} is not found"

    command = [str(osclipath)]
    if not is_labs:
        command.append("classic")
    command += ["run", "-w", str(osw_path)]
    lines = subprocess.check_output(command, encoding="utf-8").splitlines()

    runDir = Path("./run")
    assert runDir.exists()
    measure_attributes_path = runDir / "measure_attributes.json"
    assert measure_attributes_path.is_file()
    results_path = runDir / "results.json"
    assert results_path.is_file()
    objectives_path = runDir / "objectives.json"
    assert objectives_path.is_file()

    measure_attributes = json.loads(measure_attributes_path.read_text())
    assert measure_attributes == {
        "StandardReports": {"applicable": True, "net_site_energy": 167.1, "something_with_invalid_chars": 1}
    }

    results = json.loads(results_path.read_text())
    assert results == {
        "StandardReports": {"applicable": True, "net_site_energy": 167.1, "something_with_invalid_chars": 1}
    }

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
