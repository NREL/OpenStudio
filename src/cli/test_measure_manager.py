import errno
import shutil
import signal
import socket
import subprocess
import time
from contextlib import closing
from copy import deepcopy
from pathlib import Path
from typing import Tuple
from urllib.parse import urljoin

import pytest
import requests

HOST = "localhost"
OPENSTUDIO_CLI = Path("~/Software/Others/OS-build/Products/openstudio").expanduser()

BASE_INTERNAL_STATE = {
    "idf": [],
    "measure_info": None,
    "measures": None,
    "my_measures_dir": "",
    "osm": [],
    "status": "running",
}

BCL_DIR = Path("~/BCL").expanduser()
# This is 'Test Measure Recursive Folders'
TEST_BCL_MEASURE_UID = "b79e1024-8a7a-4c9d-91bf-6c11de58f4a9"


def get_url(port: int):
    return f"http://{HOST}:{port}"


class MeasureManagerClient(requests.Session):
    """Stores the base url in the class so we don't repeat ourselves."""

    def __init__(self, port):
        self.base_url = get_url(port=port)
        super().__init__()

    def request(self, method, url, *args, **kwargs):
        url = urljoin(self.base_url, url)
        return super().request(method, url, *args, **kwargs)

    def internal_state(self):
        r = self.get("/internal_state")
        r.raise_for_status()
        return r.json()

    def reset(self):
        r = self.post("/reset")
        r.raise_for_status()
        assert r.json() == "Resetting internal state"

    def reset_and_assert_internal_state(self):
        self.reset()
        assert self.internal_state() == BASE_INTERNAL_STATE


@pytest.fixture
def expected_internal_state():
    return deepcopy(BASE_INTERNAL_STATE)


def get_open_port_for_serving():
    """Find an unused port for serving the Measure Manager."""

    port = 8089
    is_open = False
    while not is_open:
        port += 1
        # print(f"Testing {port=}")
        with closing(socket.socket(socket.AF_INET, socket.SOCK_STREAM)) as sock:
            try:
                sock.bind((HOST, port))
                is_open = True
            except socket.error as e:
                if e.errno == errno.EADDRINUSE:
                    print(f"{port=} is already in use")
                else:
                    # something else raised the socket.error exception
                    raise e
    print(f"Using {port=}")
    return port


def launch_measure_manager_client() -> Tuple[subprocess.Popen, int]:
    port = get_open_port_for_serving()
    base_url = get_url(port=port)
    proc = subprocess.Popen([str(OPENSTUDIO_CLI), "labs", "measure", "-s", str(port)])
    ok = False
    while not ok:
        try:
            r = requests.get(base_url)
            ok = r.status_code == requests.codes.ok
            running = r.json()["status"] == "running"
            ok &= running
            # print(f"{ok=}, {running=}")
        except Exception as e:
            # print(f"{e.__repr__()}")
            time.sleep(0.1)

    r = requests.get(base_url)
    assert r.json()["status"] == "running"
    return proc, port


@pytest.fixture
def measure_manager_client():
    """Launches a measure manager server and waits for it to be ready."""
    proc, port = launch_measure_manager_client()
    yield MeasureManagerClient(port=port)
    proc.send_signal(signal.SIGINT)
    proc.kill()


# TODO: probably would be easier to just locate openstudio python bindings
def _write_model_cli(osm_path: Path):
    subprocess.check_output(
        [str(OPENSTUDIO_CLI), "-e", f"m = OpenStudio::Model::Model.new; m.save('{osm_path}',  true)"]
    )
    # model = openstudio.model.Model()
    # model.save(str(OSM_PATH), True)


def _checksum_cli(osm_path: Path):
    r = subprocess.check_output(
        [str(OPENSTUDIO_CLI), "-e", f"puts OpenStudio::checksum(OpenStudio::toPath('{osm_path}'))"], encoding="utf-8"
    )
    return r.splitlines()[0]
    # openstudio.checksum(openstudio.toPath(str(osm_path)))


def test_default_internal_state(measure_manager_client, expected_internal_state):
    r = measure_manager_client.get("/internal_state")
    r.raise_for_status()
    assert r.json() == expected_internal_state
    # Equivalent
    assert measure_manager_client.internal_state() == expected_internal_state


def test_set_measures_dir(measure_manager_client, expected_internal_state):
    my_measures_dir = str(Path("~/OpenStudio/Measures").expanduser())
    r = measure_manager_client.post("/set", json={"my_measures_dir": my_measures_dir})
    r.raise_for_status()
    expected_internal_state["my_measures_dir"] = my_measures_dir
    assert measure_manager_client.internal_state() == expected_internal_state
    measure_manager_client.reset_and_assert_internal_state()


# tmp_path is a built-in pytest fixture hich will provide a temporary directory unique to the test invocation,
# created in the base temporary directory.
def test_get_model(measure_manager_client, expected_internal_state, tmp_path):
    osm_path = tmp_path / "model.osm"
    osm_path2 = tmp_path / "model2.osm"

    _write_model_cli(osm_path)
    model_checksum = _checksum_cli(osm_path)
    _write_model_cli(osm_path2)
    model_checksum2 = _checksum_cli(osm_path2)
    r = measure_manager_client.post("/get_model", json={"osm_path": str(osm_path)})
    r.raise_for_status()
    assert r.json() == f"OK, loaded model with checksum {model_checksum}"

    expected_internal_state["osm"].append(
        {
            "checksum": model_checksum,
            "osm_path": str(osm_path),
        }
    )
    assert measure_manager_client.internal_state() == expected_internal_state

    r = measure_manager_client.post("/get_model", json={"osm_path": str(osm_path2)})
    r.raise_for_status()
    assert r.json() == f"OK, loaded model with checksum {model_checksum2}"

    expected_internal_state["osm"].append(
        {
            "checksum": model_checksum2,
            "osm_path": str(osm_path2),
        }
    )
    assert measure_manager_client.internal_state() == expected_internal_state

    # Try to get model 1 again, it should just reload it
    r = measure_manager_client.post("/get_model", json={"osm_path": str(osm_path)})
    r.raise_for_status()
    assert r.json() == f"OK, loaded model with checksum {model_checksum}"

    # Overwrite model1 and try again, it should reload it
    _write_model_cli(osm_path)
    model_checksum = _checksum_cli(osm_path)

    r = measure_manager_client.post("/get_model", json={"osm_path": str(osm_path)})
    r.raise_for_status()
    assert r.json() == f"OK, loaded model with checksum {model_checksum}"
    expected_internal_state["osm"][0] = {
        "checksum": model_checksum,
        "osm_path": str(osm_path),
    }

    measure_manager_client.reset_and_assert_internal_state()


def test_download_bcl_measures(measure_manager_client, expected_internal_state):
    r = measure_manager_client.post(url=f"/download_bcl_measure", json={"": ""})
    assert r.status_code == 400
    assert r.json() == "Missing the uid in the post data"

    r = measure_manager_client.post(url=f"/download_bcl_measure", json={"uid": "baduuid"})
    assert r.status_code == 400
    assert r.json() == "Cannot find measure with uid='baduuid'"

    measure_dir = BCL_DIR / TEST_BCL_MEASURE_UID
    if measure_dir.exists():
        shutil.rmtree(measure_dir)

    # TODO: this should be a get!
    r = measure_manager_client.post("/bcl_measures")
    r.raise_for_status()
    ori_measures = r.json()

    r = measure_manager_client.post(url="/download_bcl_measure", json={"uid": TEST_BCL_MEASURE_UID})
    r.raise_for_status()
    measure_data = r.json()
    assert "name" in measure_data
    assert "display_name" in measure_data
    assert "version_id" in measure_data
    assert measure_data["name"] == "test_measure_recursive_folders"
    assert measure_data["display_name"] == "Test Measure Recursive Folders"

    assert measure_dir.exists()

    r = measure_manager_client.post("/bcl_measures")
    r.raise_for_status()
    new_measures = r.json()
    assert ori_measures + ["test_measure_recursive_folders"] == new_measures
