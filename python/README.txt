Example for launching example

Note: the *.py files are generated in <build>/python_wrapper/generated_sources while the library files are in <build>/Products/python.
When installing, they will end up in the same directory. You can either go symlink the generated_sources/*.py to Products/python, or specify both
in PYTHONPATH

On Linux:
PYTHONPATH=~/openstudio/build/Products/python/:~/openstudio/build/python_wrapper/generated_sources/ python testpath.py

On Windows:
set PYTHONPATH=\working\openstudio\build\OSCore-prefix\src\OSCore-build\Products\python\Release
python testpath.py
