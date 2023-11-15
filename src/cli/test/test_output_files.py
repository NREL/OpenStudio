import json
from pathlib import Path

import pytest

from workflow_helpers import run_workflow


@pytest.fixture(scope="module", params=[True, False], ids=["labs", "classic"])
def runWorkflow(osclipath, request):
    is_labs = request.param
    suffix = "labs" if is_labs else "classic"
    runDir, r = run_workflow(
        osclipath=osclipath,
        base_osw_name="in.osw",
        suffix=suffix,
        is_labs=is_labs,
        verbose=False,
        debug=False,
        post_process_only=True,
    )
    r.check_returncode()
    return runDir, is_labs


@pytest.mark.parametrize(
    "is_labs",
    [pytest.param(True, id="labs"), pytest.param(False, id="classic")],
)
def test_(osclipath, is_labs: bool):
    suffix = "labs" if is_labs else "classic"
    runDir, r = run_workflow(
        osclipath=osclipath,
        base_osw_name="in.osw",
        suffix=suffix,
        is_labs=is_labs,
        verbose=False,
        debug=True,
        post_process_only=True,
    )
    r.check_returncode()
    out_osw_path = Path(f"out_in_{suffix}.osw")
    assert out_osw_path.is_file()
    out = json.loads(out_osw_path.read_text())

    assert out["completed_status"] == "Success"
    assert out["current_step"] == 1

    EXPECTED_TOPLEVEL_KEYS = {
        "completed_at",
        "completed_status",
        "current_step",
        "file_paths",
        "hash",
        "measure_paths",
        "out_name",
        "run_directory",
        "started_at",
        "steps",
        "updated_at",
    }
    if is_labs:
        EXPECTED_TOPLEVEL_KEYS.add("run_options")
    assert out.keys() == EXPECTED_TOPLEVEL_KEYS

    assert len(out["steps"]) == 1
    step = out["steps"][0]
    step.keys() == {"arguments", "measure_dir_name", "result"}
    assert step["arguments"] == {}
    assert step["measure_dir_name"] == "FakeReport"

    step_result = step["result"]
    assert step_result.keys() == {
        "completed_at",
        "measure_class_name",
        "measure_display_name",
        "measure_name",
        "measure_taxonomy",
        "measure_type",
        "measure_uid",
        "measure_version_id",
        "measure_version_modified",
        "measure_xml_checksum",
        "started_at",
        "stderr",
        "stdout",
        "step_errors",
        "step_files",
        "step_final_condition",
        "step_info",
        "step_result",
        "step_values",
        "step_warnings",
    }

    assert step_result["step_result"] == "Success"

    assert len(step_result["step_info"]) == 1
    assert len(step_result["step_warnings"]) == 1
    assert not step_result["step_errors"]

    len(step_result["step_values"]) == 3
    assert {x["name"] for x in step_result["step_values"]} == {
        "fake_report",
        "net_site_energy",
        "something_with_invalid_chars",
    }
