import argparse
from pathlib import Path


def validate_file(arg):
    if (filepath := Path(arg)).is_file():
        return filepath
    else:
        raise FileNotFoundError(arg)

def validate_xml_file(arg):
    filepath = validate_file(arg)
    if (filepath.suffix != '.xml'):
        raise FileNotFoundError(f"{arg} is not a .xml file")
    return filepath

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="The Python help description.")
    parser.add_argument("-x,--xml", metavar='FILE', required=True, type=validate_xml_file, help="HPXML file")
    args = parser.parse_args()
    print(args)
