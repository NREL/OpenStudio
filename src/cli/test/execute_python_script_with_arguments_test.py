import sys
from pathlib import Path

# TODO: __file__ not working yet, since I just exec the content of the file
# In Python, argv[0] is the name of the program
# if sys.argv[0] != Path(__file__).name:
#    raise ValueError("Wrong sys.argv[0] / program name")

if len(sys.argv) != 3:
    raise ValueError("You must supply two arguments to this script")

print(f"Hello from {sys.argv[1]}, at {sys.argv[2]}")
