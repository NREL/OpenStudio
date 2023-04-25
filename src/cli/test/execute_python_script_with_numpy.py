"""Test for importing a python package with a C extension, numpy here."""

import sys
from pathlib import Path


def test_import_numpy():
    """Common test function."""
    import numpy as np

    a = np.array([0, 1, 2, 3])
    assert a.sum() == 6


def test_with_sys_path_explicit(std_lib: Path):
    """Explicitly insert site-packages and lib-dynload in sys.path."""
    sys.path.insert(0, str(std_lib / "site-packages"))
    sys.path.insert(0, str(std_lib / "lib-dynload"))

    test_import_numpy()


def test_with_pythonpath_pythonhome():
    """Rely on passing PYTHONPATH/PYTHONHOME."""
    test_import_numpy()


if __name__ == "__main__":
    if len(sys.argv) == 2:
        std_lib = Path(sys.argv[1])
        print(std_lib)

        if std_lib.is_dir():
            test_with_sys_path_explicit(std_lib)
        else:
            raise ValueError(f"Python std_lib doesn't exist at {std_lib}")

    else:
        test_with_pythonpath_pythonhome()
