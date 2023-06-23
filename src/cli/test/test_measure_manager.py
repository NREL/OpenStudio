"""Tests for the CLI's MeasureManagerServer.

Calls the CLI to serve the MeasureManagerServer and uses python to send GET/POST requests

Example Usage:

```
pytest --verbose --tb=short -rfEsxX --durations=0 test_measure_manager.py \
       --os-cli-path=~/Software/Others/OS-build/Products/openstudio
```
"""
import errno
import shutil
import signal
import socket
import subprocess
import time
from contextlib import closing
from copy import deepcopy
from pathlib import Path
from typing import Any, Dict, Tuple
from urllib.parse import urljoin

import pytest
import requests

HOST = "localhost"
DEFAULT_MEASURES_DIR = Path("~/OpenStudio/Measures").expanduser()

BASE_INTERNAL_STATE: Dict[str, Any] = {
    "idf": [],
    "measure_info": None,
    "measures": None,
    "my_measures_dir": str(DEFAULT_MEASURES_DIR),
    "osm": [],
    "status": "running",
}

BCL_DIR = Path("~/BCL").expanduser()
# This is 'Test Measure Recursive Folders'
TEST_BCL_MEASURE_UID = "b79e1024-8a7a-4c9d-91bf-6c11de58f4a9"
# Change R-value of Insulation Layer for Construction By a Specified Percentage
TEST_BCL_MEASURE_MODEL_DEPENDENT = "7ee248fd-052a-48d6-a8eb-b1fc6a0dc348"


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


def launch_measure_manager_client(osclipath: Path) -> Tuple[subprocess.Popen, int]:
    port = get_open_port_for_serving()
    base_url = get_url(port=port)
    proc = subprocess.Popen([str(osclipath), "labs", "measure", "-s", str(port)])
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
def measure_manager_client(osclipath):
    """Launches a measure manager server and waits for it to be ready, then destroys at end."""
    proc, port = launch_measure_manager_client(osclipath=osclipath)
    yield MeasureManagerClient(port=port)
    proc.send_signal(signal.SIGINT)
    proc.kill()


# TODO: probably would be easier to just locate openstudio python bindings
def _write_model_cli(osclipath: Path, osm_path: Path):
    subprocess.check_output([str(osclipath), "-e", f"m = OpenStudio::Model::Model.new; m.save('{osm_path}',  true)"])
    # model = openstudio.model.Model()
    # model.save(str(osm_path), True)


def _write_example_model_cli(osclipath: Path, osm_path: Path):
    subprocess.check_output([str(osclipath), "-e", f"m = OpenStudio::Model::exampleModel; m.save('{osm_path}',  true)"])
    # model = openstudio.model.exampleModel()
    # model.save(str(osm_path), True)


def _checksum_cli(osclipath: Path, osm_path: Path):
    r = subprocess.check_output(
        [str(osclipath), "-e", f"puts OpenStudio::checksum(OpenStudio::toPath('{osm_path}'))"], encoding="utf-8"
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
    my_measures_dir = str(Path("~/OpenStudio/Measures2").expanduser())
    r = measure_manager_client.post("/set", json={"my_measures_dir": my_measures_dir})
    r.raise_for_status()
    expected_internal_state["my_measures_dir"] = my_measures_dir
    assert measure_manager_client.internal_state() == expected_internal_state


# tmp_path is a built-in pytest fixture hich will provide a temporary directory unique to the test invocation,
# created in the base temporary directory.
def test_get_model(
    measure_manager_client: MeasureManagerClient,
    expected_internal_state: Dict[str, Any],
    tmp_path: Path,
    osclipath: Path,
):
    osm_path = tmp_path / "model.osm"
    osm_path2 = tmp_path / "model2.osm"

    _write_model_cli(osclipath=osclipath, osm_path=osm_path)
    model_checksum = _checksum_cli(osclipath=osclipath, osm_path=osm_path)
    _write_model_cli(osclipath=osclipath, osm_path=osm_path2)
    model_checksum2 = _checksum_cli(osclipath=osclipath, osm_path=osm_path2)
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
    _write_model_cli(osclipath=osclipath, osm_path=osm_path)
    model_checksum = _checksum_cli(osclipath=osclipath, osm_path=osm_path)

    r = measure_manager_client.post("/get_model", json={"osm_path": str(osm_path)})
    r.raise_for_status()
    assert r.json() == f"OK, loaded model with checksum {model_checksum}"
    expected_internal_state["osm"][0] = {
        "checksum": model_checksum,
        "osm_path": str(osm_path),
    }

    measure_manager_client.reset_and_assert_internal_state()


def test_download_bcl_measures(measure_manager_client: MeasureManagerClient, expected_internal_state: Dict[str, Any]):
    r = measure_manager_client.post(url="/download_bcl_measure", json={"": ""})
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
    measures = r.json()
    assert isinstance(measures, list)
    n_ori = len(measures)
    ori_measures = [x["name"] for x in measures]

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
    measures = r.json()
    assert isinstance(measures, list)
    n_new = len(measures)
    new_measures = [x["name"] for x in measures]
    assert ori_measures + ["test_measure_recursive_folders"] == new_measures
    assert n_ori + 1 == n_new


def test_update_measures(measure_manager_client: MeasureManagerClient, expected_internal_state: Dict[str, Any]):
    r = measure_manager_client.post(url="/update_measures")
    r.raise_for_status()


def test_compute_arguments_ruby(
    measure_manager_client: MeasureManagerClient,
    expected_internal_state: Dict[str, Any],
    tmp_path: Path,
    osclipath: Path,
):
    measure_dir = BCL_DIR / TEST_BCL_MEASURE_MODEL_DEPENDENT
    if measure_dir.exists():
        measure_version_dir = next(measure_dir.glob("*/measure.rb")).parent
    else:
        r = measure_manager_client.post(url="/download_bcl_measure", json={"uid": TEST_BCL_MEASURE_MODEL_DEPENDENT})
        r.raise_for_status()
        measure_data = r.json()
        measure_version_dir = measure_dir / measure_data["version_id"]
    assert measure_version_dir.exists()

    r = measure_manager_client.post(url="/compute_arguments", json={"": ""})
    assert r.status_code == 400
    assert r.json() == "The 'measure_dir' (string) must be in the post data"

    r = measure_manager_client.post(url=f"/compute_arguments", json={"measure_dir": "bogusdirectory"})
    assert r.status_code == 400
    assert r.json() == "Cannot load measure at 'bogusdirectory'"

    r = measure_manager_client.post(url="/compute_arguments", json={"measure_dir": str(measure_version_dir)})
    r.raise_for_status()

    osm_path = tmp_path / "model.osm"
    _write_example_model_cli(osclipath=osclipath, osm_path=osm_path)
    assert osm_path.is_file()
    r = measure_manager_client.post(
        url="/compute_arguments", json={"measure_dir": str(measure_version_dir), "osm_path": str(osm_path)}
    )
    r.raise_for_status()


def modify_python_measure_for_model_dependent_arg(measure_dir: Path):
    measure_py = measure_dir / "measure.py"
    if not measure_py.exists():
        raise ValueError(f"{measure_py} doesn't exist")

    with open(measure_py, "r") as f:
        lines = f.read().splitlines()

    in_block = False
    arg_line = -1
    for i, line in enumerate(lines):
        line = line.strip()
        if "def arguments" in line:
            in_block = True
        if not in_block:
            continue
        if line.startswith("args = "):
            arg_line = i
            break
    assert arg_line > 0

    indent: int = lines[arg_line].index("args")  # type: ignore

    prog = """
construction_handles = []
construction_display_names = []

for c in model.getConstructions():
    construction_handles.append(str(c.handle()))
    construction_display_names.append(c.nameString())

construction = openstudio.measure.OSArgument.makeChoiceArgument(
    'construction', construction_handles, construction_display_names, True
)
args.append(construction)"""

    for line in prog.splitlines():
        arg_line += 1
        if line:
            lines.insert(arg_line, " " * indent + line)
        else:
            lines.insert(arg_line, "")

    with open(measure_py, "w") as f:
        f.write("\n".join(lines) + "\n")


def test_compute_arguments_python(
    measure_manager_client: MeasureManagerClient,
    expected_internal_state: Dict[str, Any],
    tmp_path: Path,
    osclipath: Path,
):
    measure_dir = tmp_path / "new_measure"
    assert not measure_dir.is_dir()

    data = {
        # "name": "name",
        "measure_dir": str(measure_dir),
        "display_name": "display_name",
        "class_name": "class_name",
        "taxonomy_tag": "taxonomy_tag",
        "measure_type": "ModelMeasure",  # <=
        "measure_language": "Python",  # <=
        "description": "description",
        "modeler_description": "modeler_description",
    }
    r = measure_manager_client.post(url="/create_measure", json=data)
    r.raise_for_status
    assert measure_dir.is_dir()

    r = measure_manager_client.post(url="/compute_arguments", json={"measure_dir": str(measure_dir)})
    r.raise_for_status()

    modify_python_measure_for_model_dependent_arg(measure_dir=measure_dir)

    r = measure_manager_client.post(url="/compute_arguments", json={"measure_dir": str(measure_dir)})
    r.raise_for_status()

    osm_path = tmp_path / "model.osm"
    _write_example_model_cli(osclipath=osclipath, osm_path=osm_path)
    assert osm_path.is_file()
    r = measure_manager_client.post(
        url="/compute_arguments", json={"measure_dir": str(measure_dir), "osm_path": str(osm_path)}
    )
    r.raise_for_status()
    measure_info = r.json()
    arguments = measure_info["arguments"]
    assert len(arguments) == 2
    construction_arg = next(x for x in arguments if x["name"] == "construction")
    assert "choice_display_names" in construction_arg
    assert "choices_values" in construction_arg
    assert len(construction_arg["choice_display_names"]) > 5
    assert len(construction_arg["choice_display_names"]) == len(construction_arg["choices_values"])


def test_create_measure(
    measure_manager_client: MeasureManagerClient,
    expected_internal_state: Dict[str, Any],
    tmp_path: Path,
):
    data = {
        # "name": "name",
        "display_name": "display_name",
        "class_name": "class_name",
        "taxonomy_tag": "taxonomy_tag",
        "measure_type": "BadMeasureType",  # <=
        "measure_language": "BadMeasureLanguage",  # <=
        "description": "description",
        "modeler_description": "modeler_description",
    }
    r = measure_manager_client.post(url="/create_measure", json=data)
    assert not r.ok
    assert r.json() == "The 'measure_dir' (string) must be in the post data."

    measure_dir = tmp_path / "new_measure"
    data["measure_dir"] = str(measure_dir)
    assert not measure_dir.is_dir()

    # The enums are throwy! Make sure we handle that gracefully
    r = measure_manager_client.post(url="/create_measure", json=data)
    assert not r.ok
    assert (
        r.json()
        == "Couldn't convert 'BadMeasureType' to a MeasureType: Unknown OpenStudio Enum Value 'BADMEASURETYPE' for Enum MeasureType"
    )
    data["measure_type"] = "ModelMeasure"

    r = measure_manager_client.post(url="/create_measure", json=data)
    assert not r.ok
    assert (
        r.json()
        == "Couldn't convert 'BadMeasureLanguage' to a MeasureLanguage: Unknown OpenStudio Enum Value 'BADMEASURELANGUAGE' for Enum MeasureLanguage"
    )
    data["measure_language"] = "Python"

    r = measure_manager_client.post("/create_measure", json=data)
    r.raise_for_status()
    assert measure_dir.is_dir()

    measure_info = r.json()
    for k in data:
        if k in ["measure_type", "measure_language"]:
            continue
        if k == "taxonomy_tag":
            assert len(measure_info["tags"]) == 1
            measure_info["tags"][0] == data[k]
            continue
        assert k in measure_info, k
        assert measure_info[k] == data[k], f"{k}: {measure_info[k]} vs {data[k]}"
    assert next(x["value"] for x in measure_info["attributes"] if x["name"] == "Measure Type") == data["measure_type"]
    assert (
        next(x["value"] for x in measure_info["attributes"] if x["name"] == "Measure Language")
        == data["measure_language"]
    )

    # Should fail gracefully when directory exists already
    r = measure_manager_client.post("/create_measure", json=data)
    assert not r.ok
    assert "The directory already exists" in r.json()


def test_duplicate_measure(
    measure_manager_client: MeasureManagerClient,
    expected_internal_state: Dict[str, Any],
    tmp_path: Path,
):
    old_measure_dir = tmp_path / "old_measure"
    new_measure_dir = tmp_path / "new_measure_dir"

    assert not old_measure_dir.is_dir()
    assert not new_measure_dir.is_dir()

    data_ori = {
        "measure_dir": str(old_measure_dir),
        "display_name": "old_display_name",
        "class_name": "old_class_name",
        "taxonomy_tag": "old_taxonomy_tag",
        "measure_type": "ModelMeasure",
        "measure_language": "Python",
        "description": "old_description",
        "modeler_description": "old_modeler_description",
    }
    r = measure_manager_client.post(url="/create_measure", json=data_ori)
    r.raise_for_status()

    assert old_measure_dir.is_dir()
    assert not new_measure_dir.is_dir()

    data = {
        "old_measure_dir": str(old_measure_dir),
        "measure_dir": str(new_measure_dir),
        "display_name": "display_name",
        "class_name": "class_name",
        "taxonomy_tag": "taxonomy_tag",
        "measure_type": "EnergyPlusMeasure",
        # "measure_language": "Python",
        "description": "description",
        "modeler_description": "modeler_description",
    }
    r = measure_manager_client.post("/duplicate_measure", json=data)
    r.raise_for_status()
    measure_info = r.json()
    for k in data:
        if k in ["old_measure_dir", "measure_type", "measure_language"]:
            continue
        if k == "taxonomy_tag":
            assert len(measure_info["tags"]) == 1
            measure_info["tags"][0] == data[k]
            continue
        assert k in measure_info, k
        assert measure_info[k] == data[k], f"{k}: {measure_info[k]} vs {data[k]}"
    assert next(x["value"] for x in measure_info["attributes"] if x["name"] == "Measure Type") == data["measure_type"]
    assert (
        next(x["value"] for x in measure_info["attributes"] if x["name"] == "Measure Language")
        == data_ori["measure_language"]
    )
    assert old_measure_dir.is_dir()
    assert new_measure_dir.is_dir()
