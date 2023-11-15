import pytest

from workflow_helpers import run_workflow


@pytest.mark.parametrize(
    "is_labs",
    [pytest.param(True, id="labs"), pytest.param(False, id="classic")],
)
def test_outdated_reportingmeasure(osclipath, is_labs: bool):
    suffix = "labs" if is_labs else "classic"
    runDir, r = run_workflow(
        osclipath=osclipath,
        base_osw_name="outdatedreportingmeasure.osw",
        suffix=suffix,
        is_labs=is_labs,
        verbose=False,
        debug=True,
        post_process_only=True,
    )
    r.check_returncode()
    assert r.returncode == 0
