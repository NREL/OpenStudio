import json
import shutil
import subprocess
from pathlib import Path
from typing import Tuple


def run_workflow(
    osclipath: Path,
    base_osw_name: str,
    suffix: str,
    is_labs: bool,
    verbose: bool = False,
    debug: bool = False,
    post_process_only: bool = False,
) -> Tuple[Path, "subprocess.CompletedProcess[str]"]:
    """
    Run a workflow file with the OS CLI, taking a base OSW and using a unique run_directory via the suffix

    Notes
    -----
    The function assumes the current working directory is where the original OSW is.

    Parameters
    ----------
    osclipath : Path
        The path to the OS CLI executable.

    base_osw_name : str
        The name of the base OSW (OpenStudio Workflow) file.

    suffix : str
        The suffix to be appended to the base OSW name for the new workflow file.

    is_labs : bool
        A flag indicating whether to run in OpenStudio Labs mode, or Classic mode.

    verbose : bool, optional
        If True, passes the high level --verbose flag.

    debug : bool, optional
        If True, enables debugging output in the run subcommand via --debug

    post_process_only : bool, optional
        If True, runs the workflow in --postprocess_only mode.

    Returns
    -------
    Tuple[Path, 'subprocess.CompletedProcess[str]']
        A tuple containing the path to the run directory and a CompletedProcess object
        representing the result of the subprocess execution.

    Raises
    ------
    AssertionError
        If the specified base OSW file is not found.


    Example
    -------
    >>> oscli_path = Path('/path/to/oscli')
    >>> base_osw = 'example.osw'
    >>> suffix = 'run1'
    >>> labs_mode = True
    >>> run_dir, process_result = run_workflow(oscli_path, base_osw, suffix, labs_mode, verbose=True)
    """
    base_osw_path = Path(base_osw_name).resolve()
    assert base_osw_path.is_file(), f"{base_osw_path=} is not found"

    osw = json.loads(base_osw_path.read_text())
    osw_path = base_osw_path.parent / f"{base_osw_path.stem}_{suffix}.osw"
    runDir = base_osw_path.parent / f"run_{osw_path.stem}"
    osw["run_directory"] = str(runDir)
    if runDir.is_dir():
        shutil.rmtree(runDir)
    runDir.mkdir(exist_ok=False)
    with open(osw_path, "w") as f:
        json.dump(osw, fp=f, indent=2, sort_keys=True)

    command = [str(osclipath)]
    if not is_labs:
        command.append("classic")
    if verbose:
        command.append("--verbose")
    command.append("run")
    if post_process_only:
        # Fake having an in.idf or it won't run in the "classic" subcommand
        # Doing it for labs too so that it's less confusing
        with open(runDir / "in.idf", "w") as f:
            f.write("Building,;")
        command.append("--postprocess_only")
    if debug:
        command.append("--debug")
    command += ["-w", str(osw_path)]
    # print(f"Running command: {' '.join(command)}")
    return runDir, subprocess.run(command, capture_output=True, encoding="utf-8")
