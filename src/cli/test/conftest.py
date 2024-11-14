from pathlib import Path

import pytest


def validate_file(arg):
    if (filepath := Path(arg).expanduser()).is_file():
        return filepath
    else:
        raise FileNotFoundError(arg)


def pytest_addoption(parser):
    parser.addoption("--os-cli-path", type=validate_file, help="Path to the OS CLI")  # , required=True
    parser.addoption("--use-classic", action="store_true", help="Force use the Classic CLI")

@pytest.fixture(scope="module")
def osclipath(request):
    cli_path = request.config.getoption("--os-cli-path")
    if cli_path is None:
        raise ValueError("You must supply --os-cli-path [Path]")
    return cli_path

@pytest.fixture(scope="module")
def use_classic_cli(request):
    use_classic = request.config.getoption("--use-classic")
    return use_classic
