########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

import os
import re
import requests
from packaging import version
import argparse
from typing import List

REPO_ROOT = os.path.join(os.path.dirname(os.path.realpath(__file__)), '../..')


def parse_pypi_version(pypi: bool = False):
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
        pre_release = m.groups()[0].strip()
        if pre_release:
            v += f".{pre_release}"

    return version.Version(v)


def compute_appropriate_version(current_v: version.Version,
                                releases: List[version.Version],
                                current: bool = False):
    """
    Args:
    ------

    * current_v (version.Version): the version parsed from CMakeLists.txt
    * releases (list[version.Version]): the known versions
    * pypi (bool): if true, this is a pypi (official) release,
        otherwise it's a testpypi release (pre-release)
    """
    is_pre_release = current_v.is_prerelease
    pre_iden = None
    pre_v = None
    is_offical = True

    # Start by filtering out the stuff that does not match the base version
    matched_releases = [v for v in releases
                        if v.base_version == current_v.base_version]

    if not is_pre_release:
        # Filter out prereleases
        matched_releases = [v for v in matched_releases if not v.is_prerelease]
    else:
        is_offical = False

        # If we're a pre-release, we only match prerelease with the same pre
        # identifier (eg: 'a', 'b', 'rc')
        pre_iden, pre_v = current_v.pre
        matched_releases = [v for v in matched_releases
                            if v.is_prerelease and v.pre[0] == pre_iden]
        if pre_iden == 'rc':
            # Treat rc as official
            is_offical = True
            # I match on the pre_v too
            matched_releases = [v for v in matched_releases
                                if v.pre[1] == pre_v]

    new_v = current_v.base_version
    if matched_releases:
        max_v = max(matched_releases)
        if is_offical:
            if is_pre_release:
                new_v += f"{pre_iden}{pre_v}"
            post_v = max_v.post
            if not post_v:
                if not current:
                    new_v += 'post0'
            elif current:
                new_v += f"post{post_v}"
            else:
                new_v += f"post{post_v + 1}"
        else:
            # We **know** there is a pre_v in both cases and the pre_iden
            # matches
            if not max_v.is_prerelease:
                raise ValueError("Unknow")
            _, max_pre_v = max_v.pre
            if current:
                new_v += f"{pre_iden}{max_pre_v}"
            else:
                new_v += f"{pre_iden}{max_pre_v + 1}"

    else:
        # Use as is
        new_v = str(current_v)

    new_v = version.Version(new_v)
    return new_v


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description="Find the right version from pypi/testpypi")

    parser.add_argument("--pypi", default=False,
                        action='store_true',
                        help="Check pypi instead of testpypi")

    # This is more for testing purposes...
    parser.add_argument("--current", default=False,
                        action='store_true',
                        help="Check current version (no incrementing +1)")

    args = parser.parse_args()
    current_v = parse_cmake_version_info()
    releases = parse_pypi_version(pypi=args.pypi)

    new_v = compute_appropriate_version(
        current_v=current_v,
        releases=releases,
        current=args.current
    )
    print(new_v, end="")
