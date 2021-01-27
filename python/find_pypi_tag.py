import os
import re
import requests
from packaging import version
import argparse

REPO_ROOT = os.path.join(os.path.dirname(os.path.realpath(__file__)), '..')


def parse_pypi_version(pypi=False):
    if pypi:
        response = requests.get('https://pypi.org/pypi/openstudio/json')
    else:
        response = requests.get('https://test.pypi.org/pypi/openstudio/json')
    response.raise_for_status()
    data = response.json()
    releases = [version.parse(v) for v in data['releases'].keys()]
    return releases


def parse_cmake_version_info():

    with open(os.path.join(REPO_ROOT, 'CMakeLists.txt'), 'r') as f:
        content = f.read()

    no_comments_lines = []
    for line in content.splitlines():
        line_cleaned = line.strip().split('#')[0]
        if line_cleaned:
            no_comments_lines.append(line_cleaned)
    content = "\n".join(no_comments_lines)
    m = re.search(r'project\(OpenStudio VERSION (\d+\.\d+\.\d+)\)', content)
    v = ''
    if m:
        v = m.groups()[0]

    m = re.search(r'set\(PROJECT_VERSION_PRERELEASE \"(.*?)\"\)', content)
    pre_release = ''
    if m:
        pre_release = m.groups()[0]
        v += f".{pre_release}"

    return version.Version(v)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description="Find the right version from pypi/testpypi")

    parser.add_argument("--pypi", default=False,
                        action='store_true',
                        help="Check pypi instead of testpypi")

    parser.add_argument("--current", default=False,
                        action='store_true',
                        help="Check current version instead of incrementing by one")
                        
    args = parser.parse_args()
    current_v = parse_cmake_version_info()
    releases = parse_pypi_version(pypi=args.pypi)

    matched_releases = [v for v in releases
                        if v.base_version == current_v.base_version]

    new_v = current_v.base_version
    if matched_releases:
        max_v = max(matched_releases)
        if max_v.pre:
            pre_iden, pre_v = max_v.pre
            if args.current:
                new_v += f"{pre_iden}{pre_v}"
            else:
                new_v += f"{pre_iden}{pre_v + 1}"
        else:
            new_v += ".post1"
    else:
        new_v = str(current_v)

    new_v = version.Version(new_v)
    print(new_v, end="")
