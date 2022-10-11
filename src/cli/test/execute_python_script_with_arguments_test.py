import sys
from pathlib import Path

argv0 = sys.argv[0]
thisFile = Path(__file__)
# In Python, argv[0] is the name of the program
if argv0 != thisFile.name:
    raise ValueError(f"Wrong sys.argv[0] / program name:  sys.argv[0]={argv0}, __file__={thisFile}")

if len(sys.argv) != 3:
    raise ValueError("You must supply two arguments to this script")

print(f"Hello from {sys.argv[1]}, at {sys.argv[2]}")
