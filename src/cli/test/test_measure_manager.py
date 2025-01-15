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

BASE_INTERNAL_STATE_CLASSIC: Dict[str, Any] = {
    "status": "running",
    "my_measures_dir": DEFAULT_MEASURES_DIR.as_posix() + "/",
    "osms": [],
    "measures": [],
    "measure_info": [],
}

BASE_INTERNAL_STATE_LABS: Dict[str, Any] = {
    "idfs": [],
    "measure_info": [],
    "measures": [],
    "my_measures_dir": DEFAULT_MEASURES_DIR.as_posix(),
    "osms": [],
    "status": "running",
}

BCL_DIR = Path("~/BCL").expanduser()
# This is 'Test Measure Recursive Folders'
TEST_BCL_MEASURE_UID = "b79e1024-8a7a-4c9d-91bf-6c11de58f4a9"


def get_url(port: int):
    return f"http://{HOST}:{port}"


class MeasureManagerClient(requests.Session):
    """Stores the base url in the class so we don't repeat ourselves."""

    def __init__(self, port, is_classic=False):
        self.base_url = get_url(port=port)
        self.is_classic = is_classic
        self.base_state = deepcopy(BASE_INTERNAL_STATE_CLASSIC if is_classic else BASE_INTERNAL_STATE_LABS)
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
        assert self.internal_state() == self.base_state

    def prime_a_new_my_measures_dir_with_a_single_measure(self, my_measures_dir: Path) -> Path:
        """Calls /set then /create_measure and return the new measure path under it."""
        my_measures_dir.mkdir(parents=True)

        r = self.post("/set", json={"my_measures_dir": str(my_measures_dir)})
        r.raise_for_status()
        assert self.internal_state()["my_measures_dir"] == my_measures_dir.as_posix()

        measure_dir = my_measures_dir / "MyMeasure"
        data = {
            # "name": "name",
            "measure_dir": str(measure_dir),
            "display_name": "A ModelMeasure that depends on model",
            "class_name": "ModelDependentMeasure",
            "taxonomy_tag": "taxonomy_tag",
            "measure_type": "ModelMeasure",
            "measure_language": "Ruby",
            "description": "This is the description",
            "modeler_description": "This is the modeler description",
        }
        r = self.post(url="/create_measure", json=data)
        r.raise_for_status
        assert measure_dir.is_dir()
        return measure_dir

@pytest.fixture
def expected_internal_state(use_classic_cli):
    return deepcopy(BASE_INTERNAL_STATE_CLASSIC if use_classic_cli else BASE_INTERNAL_STATE_LABS)


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


def launch_measure_manager_client(osclipath: Path, use_classic_cli: bool) -> Tuple[subprocess.Popen, int]:
    port = get_open_port_for_serving()
    base_url = get_url(port=port)
    cmd_args = [str(osclipath)]
    if use_classic_cli:
        cmd_args.append("classic")
    cmd_args += ["measure", "-s", str(port)]
    print(cmd_args)
    proc = subprocess.Popen(cmd_args)
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
def measure_manager_client(osclipath, use_classic_cli):
    """Launches a measure manager server and waits for it to be ready, then destroys at end."""
    # If you want to attach to your debug CLI session you have launched in a debugger, just do this isntead
    # yield MeasureManagerClient(port=8094)
    # return
    # Regular
    proc, port = launch_measure_manager_client(osclipath=osclipath, use_classic_cli=use_classic_cli)
    yield MeasureManagerClient(port=port, is_classic=use_classic_cli)
    # if sys.platform == "win32":
    #     proc.send_signal(signal.CTRL_C_EVENT)
    # else:
    #     proc.send_signal(signal.SIGINT)
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


def test_set_measures_dir(measure_manager_client, expected_internal_state, tmp_path):
    my_measures_dir = tmp_path / 'Measures'

    r = measure_manager_client.post("/set", json={"BAD": str(my_measures_dir)})
    # The Classic CLI returns 200 even though it didn't set anything, the C++ error handling is better
    if measure_manager_client.is_classic:
        assert r.status_code == 200
        assert not r.json()
    else:
        assert r.status_code == 400
        assert r.json() == "Missing the my_measures_dir in the post data"
    assert measure_manager_client.internal_state() == expected_internal_state

    # When the measure directory does not exist, the C++ version catches it
    assert not my_measures_dir.is_dir()
    r = measure_manager_client.post("/set", json={"my_measures_dir": str(my_measures_dir)})
    if measure_manager_client.is_classic:
        assert r.status_code == 200
        assert not r.json()
        expected_internal_state["my_measures_dir"] = my_measures_dir.as_posix()
        assert measure_manager_client.internal_state() == expected_internal_state
    else:
        assert r.status_code == 400
        assert "is a not a valid directory" in r.text
        assert measure_manager_client.internal_state() == expected_internal_state

    my_measures_dir.mkdir(parents=True)

    r = measure_manager_client.post("/set", json={"my_measures_dir": str(my_measures_dir)})
    r.raise_for_status()
    expected_internal_state["my_measures_dir"] = my_measures_dir.as_posix()
    assert measure_manager_client.internal_state() == expected_internal_state


# tmp_path is a built-in pytest fixture hich will provide a temporary directory unique to the test invocation,
# created in the base temporary directory.
# @pytest.mark.skipif(measure_manager_client.is_classic, reason="Classic CLI does not have the POST /get_model")
def test_get_model(
    measure_manager_client: MeasureManagerClient,
    expected_internal_state: Dict[str, Any],
    tmp_path: Path,
    osclipath: Path,
):
    if measure_manager_client.is_classic:
        pytest.skip("Classic CLI does not have the POST /get_model")

    osm_path = tmp_path / "model.osm"
    osm_path2 = tmp_path / "model2.osm"

    _write_model_cli(osclipath=osclipath, osm_path=osm_path)
    model_checksum = _checksum_cli(osclipath=osclipath, osm_path=osm_path)
    _write_model_cli(osclipath=osclipath, osm_path=osm_path2)
    model_checksum2 = _checksum_cli(osclipath=osclipath, osm_path=osm_path2)
    r = measure_manager_client.post("/get_model", json={"osm_path": str(osm_path)})
    r.raise_for_status()
    assert r.json() == f"OK, loaded model with checksum {model_checksum}"

    expected_internal_state["osms"].append(
        {
            "checksum": model_checksum,
            "osm_path": osm_path.as_posix(),
        }
    )
    assert measure_manager_client.internal_state() == expected_internal_state

    r = measure_manager_client.post("/get_model", json={"osm_path": str(osm_path2)})
    r.raise_for_status()
    assert r.json() == f"OK, loaded model with checksum {model_checksum2}"

    expected_internal_state["osms"].append(
        {
            "checksum": model_checksum2,
            "osm_path": osm_path2.as_posix(),
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
    expected_internal_state["osms"][0] = {
        "checksum": model_checksum,
        "osm_path": osm_path.as_posix(),
    }

    measure_manager_client.reset_and_assert_internal_state()


def test_download_bcl_measures(measure_manager_client: MeasureManagerClient, expected_internal_state: Dict[str, Any]):
    r = measure_manager_client.post(url="/download_bcl_measure", json={"": ""})
    assert r.status_code == 400
    if measure_manager_client.is_classic:
        # {"backtrace": [webcrick...], "error": "Missing required argument 'uid'}
        assert "Missing required argument 'uid'" in r.text
    else:
        assert r.json() == "Missing the uid in the post data"

    r = measure_manager_client.post(url=f"/download_bcl_measure", json={"uid": "baduuid"})
    assert r.status_code == 400
    if measure_manager_client.is_classic:
        # {"backtrace": [webcrick...], "error": "Failed to download measure 'baduuid'"}
        assert "Failed to download measure 'baduuid'" in r.text
    else:
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
    if measure_manager_client.is_classic:
        measure_data = measure_data[0]
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


def test_update_measures(measure_manager_client: MeasureManagerClient, expected_internal_state: Dict[str, Any],
                         tmp_path: Path):
    measure_manager_client.prime_a_new_my_measures_dir_with_a_single_measure(my_measures_dir=tmp_path / 'Measures')

    r = measure_manager_client.post(url="/update_measures")
    r.raise_for_status()


def modify_measure_for_model_dependent_arg(measure_dir: Path, is_ruby: bool):
    ext = "rb" if is_ruby else "py"
    measure_file = measure_dir / f"measure.{ext}"
    if not measure_file.exists():
        raise ValueError(f"{measure_file} doesn't exist")

    with open(measure_file, "r") as f:
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

    if is_ruby:
        prog = """
construction_handles = []
construction_display_names = []

model.getConstructions.each do |c|
    construction_handles << c.handle.to_s
    construction_display_names << c.nameString
end
construction = OpenStudio::Measure::OSArgument.makeChoiceArgument(
    'construction', construction_handles, construction_display_names, true, true
)
construction.setDisplayName("Choose a Construction to Alter")
construction.setDescription("Pick a construction from model")
args << construction"""

    else:
        prog = """
construction_handles = []
construction_display_names = []

for c in model.getConstructions():
    construction_handles.append(str(c.handle()))
    construction_display_names.append(c.nameString())

construction = openstudio.measure.OSArgument.makeChoiceArgument(
    'construction', construction_handles, construction_display_names, True, True
)
construction.setDisplayName("Choose a Construction to Alter")
construction.setDescription("Pick a construction from model")
args.append(construction)"""

    for line in prog.splitlines():
        arg_line += 1
        if line:
            lines.insert(arg_line, " " * indent + line)
        else:
            lines.insert(arg_line, "")

    with open(measure_file, "w") as f:
        f.write("\n".join(lines) + "\n")


@pytest.mark.parametrize(
    "measure_language",
    ["Ruby", "Python"],
)
def test_compute_arguments(
    measure_language: str,
    measure_manager_client: MeasureManagerClient,
    expected_internal_state: Dict[str, Any],
    tmp_path: Path,
    osclipath: Path,
):
    is_ruby = measure_language == "Ruby"
    if measure_manager_client.is_classic and not is_ruby:
        pytest.skip("Python not available for the classic CLI")
    measure_dir = tmp_path / f"new_{measure_language}_measure"
    assert not measure_dir.is_dir()
    readme_out_path = measure_dir / "README.md"

    data = {
        # "name": "name",
        "measure_dir": str(measure_dir),
        "display_name": "A ModelMeasure that depends on model",
        "class_name": "ModelDependentMeasure",
        "taxonomy_tag": "taxonomy_tag",
        "measure_type": "ModelMeasure",  # <=
        "measure_language": measure_language,  # <=
        "description": "This is the description",
        "modeler_description": "This is the modeler description",
    }
    r = measure_manager_client.post(url="/create_measure", json=data)
    r.raise_for_status
    assert measure_dir.is_dir()

    # Test README.md ERB generation
    if is_ruby:
        assert readme_out_path.exists()
        readme_out_path.unlink()

    # breakpoint()
    r = measure_manager_client.post(url="/compute_arguments", json={"measure_dir": str(measure_dir)})
    r.raise_for_status()

    if is_ruby:
        assert readme_out_path.exists()
        readme_out_path.unlink()

    modify_measure_for_model_dependent_arg(measure_dir=measure_dir, is_ruby=is_ruby)

    r = measure_manager_client.post(url="/compute_arguments", json={"measure_dir": str(measure_dir)})
    r.raise_for_status()

    if is_ruby:
        assert readme_out_path.exists()
        readme_out_path.unlink()

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
    assert "choice_values" in construction_arg
    assert len(construction_arg["choice_display_names"]) > 5
    assert len(construction_arg["choice_display_names"]) == len(construction_arg["choice_values"])

    if is_ruby:
        assert readme_out_path.exists()
        readme_out_md = readme_out_path.read_text()
        assert "Choose a Construction to Alter" in readme_out_md


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
    if measure_manager_client.is_classic:
        # A really bad error message, because it fails in expand_path since measure_dir is nil
        assert "expand_path" in r.json()["backtrace"]
        assert "no implicit conversion of nil into String" in r.json()["error"]
    else:
        assert r.json() == "The 'measure_dir' (string) must be in the post data."

    measure_dir = tmp_path / "new_measure"
    data["measure_dir"] = measure_dir.as_posix()
    assert not measure_dir.is_dir()

    # The enums are throwy! Make sure we handle that gracefully
    r = measure_manager_client.post(url="/create_measure", json=data)
    assert not r.ok
    if measure_manager_client.is_classic:
        assert r.json()["error"] == "Unknown OpenStudio Enum Value 'BADMEASURETYPE' for Enum MeasureType"
    else:
        assert (
            r.json()
            == "Couldn't convert 'BadMeasureType' to a MeasureType: Unknown OpenStudio Enum Value 'BADMEASURETYPE' for Enum MeasureType"
        )
    data["measure_type"] = "ModelMeasure"

    r = measure_manager_client.post(url="/create_measure", json=data)
    assert not r.ok
    if measure_manager_client.is_classic:
        assert r.json()["error"] == "Unknown OpenStudio Enum Value 'BADMEASURELANGUAGE' for Enum MeasureLanguage"
    else:
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
    if measure_manager_client.is_classic:
        assert "exists but is not an empty directory" in r.json()["error"]
    else:
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
        "measure_dir": old_measure_dir.as_posix(),
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
        "old_measure_dir": old_measure_dir.as_posix(),
        "measure_dir": new_measure_dir.as_posix(),
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
