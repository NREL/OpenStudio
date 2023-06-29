import pytest
from pathlib import Path

def validate_file(arg):
    if (filepath := Path(arg)).is_file():
        return filepath
    else:
        raise FileNotFoundError(arg)


def pytest_addoption(parser):
    parser.addoption(
        "--os-cli-path", type=validate_file, help="Path to the OS CLI" #, required=True
    )

@pytest.fixture
def osclipath(request):
    cli_path = request.config.getoption("--os-cli-path")
    if cli_path is None:
        raise ValueError("You must supply --os-cli-path [Path]")
    return cli_path
