import re
import shlex
import subprocess

# StrictVersion would work for except openssl which has 1.1.1m for eg
from distutils.version import LooseVersion, StrictVersion
from pathlib import Path
from typing import List, Optional, Tuple

from dateutil import parser
from rich import print

RE_CONAN = re.compile(
    r"(?P<package>\w+)/(?P<version>\d+\.\d+(?:\.\w+)?)#(?P<rev>\w+)"
)

# Change to None if ok to go to any version
PIN_OPENSSL_VERSION = "1.1.1"


def locate_conaninstall_cmake() -> Path:
    root_dir = Path(__file__).parent.parent.parent.resolve()

    conan_install = root_dir / "ConanInstall.cmake"
    if not conan_install.exists():
        raise IOError(f"File {conan_install} does not exist")

    return conan_install


def parse_conanfile(filepath: Path) -> List[dict]:
    with open(filepath, "r") as f:
        content = f.read()
    lines = content.splitlines()

    matches = []
    for line in lines:
        if m := RE_CONAN.search(line):
            matches.append(m.groupdict())

    if len(matches) != 16:
        # Update this number if you add/remove a dependency
        raise ValueError(f"Found {len(matches)} packages when I expected 16")

    return matches


def lookup_remote_names() -> dict:
    cci = None
    nrel = None
    r = subprocess.check_output(shlex.split("conan remote list"))
    for line in r.decode().splitlines():
        remote, url = line.split(" ")[:2]
        remote = remote.replace(":", "").strip()
        url = url.strip()
        if "conan.openstudio.net" in url:
            nrel = remote
        if "center.conan.io" in url:
            cci = remote

    if cci is None:
        raise ValueError("Could not find a remote name for center.conan.io")
    if nrel is None:
        raise ValueError(
            "Could not find a remote name for conan.openstudio.net"
        )

    return {"cci": cci, "nrel": nrel}


def lookup_last_v(package: str, remote: Optional[str] = "cci") -> str:
    """
    Search conan center remote for the latest version of a package
    I'm deliberately returning a str and not a Strict/LooseVersion
    to avoid missing the minor when it's zero
    """
    r = subprocess.check_output(
        shlex.split(f"conan search -r {remote} {package} --raw")
    )
    known_versions = [x.split("/")[1] for x in r.decode().splitlines()]
    known_versions = [x for x in known_versions if "cci" not in x]

    if package == "openssl":
        if PIN_OPENSSL_VERSION is not None:
            known_versions = [
                x for x in known_versions if PIN_OPENSSL_VERSION in x
            ]
        known_versions.sort(key=LooseVersion)
    else:
        known_versions.sort(key=StrictVersion)

    return known_versions[-1]


def lookup_all_revs_for_version(
    package: str, version: str, remote: Optional[str] = "cci"
) -> str:
    r = subprocess.check_output(
        shlex.split(
            f"conan search -r {remote} {package}/{version}@ -rev --raw"
        )
    )
    revs_info = {}
    for rev_and_date in r.decode().splitlines():
        rev, date = rev_and_date.split("(")
        rev = rev.strip()
        d = date.split(")")[0]
        d = parser.parse(d)

        revs_info[rev] = d
    return revs_info


def find_update_for_package(
    package: str,
    current_version: str,
    current_rev: str,
    remote: Optional[str] = "cci",
) -> Tuple[str, str]:

    last_v_str = lookup_last_v(package=package, remote=remote)
    if package == "openssl":
        cur_v = LooseVersion(current_version)
        last_v = LooseVersion(last_v_str)
    else:
        cur_v = StrictVersion(current_version)
        last_v = StrictVersion(last_v_str)

    revs_info = lookup_all_revs_for_version(
        package=package, version=last_v_str, remote=remote
    )
    last_rev = max(revs_info, key=lambda k: revs_info[k])

    if cur_v > last_v:
        # Not expected, though that might be possible
        # (if we do a new version on a fork)
        raise ValueError(f"Package {package}: {cur_v=} > {last_v=}")

    elif cur_v == last_v:
        if last_rev == current_rev:
            print(
                f"\n:white_check_mark: [green]Package {package} is using "
                "the latest version and revision: "
                f"{package}/{last_v_str}#{last_rev}[/]"
            )
            # No-op!
            return None
        else:
            print(
                f"\n:collision: Package [yellow bold]{package}[/] is using "
                f"the latest version ({last_v_str}), "
                "but an outdated revision"
            )
            if current_rev in revs_info:
                cur_rev_date = revs_info[current_rev]
                last_rev_date = revs_info[last_rev]
                dt = last_rev_date - cur_rev_date
                print(
                    f"Current revision {current_rev} is outdated by "
                    f"{dt.days} days. New rev {last_rev} was released "
                    f"on {last_rev_date.date().isoformat()}"
                )

    elif cur_v < last_v:
        print(
            f"\n:cross_mark: Package [red bold]{package}[/] is using {cur_v} "
            f"when {last_v} is available"
        )

    return last_v_str, last_rev


def update_conanfile(filepath: Path, updates: dict):
    with open(filepath, "r") as f:
        content = f.read()
    for d in updates.values():
        content = content.replace(d["from"], d["to"])

    with open(filepath, "w") as f:
        f.write(content)

    print("\n:+1: [bold green]Updated ConanInstall.cmake[/]")


if __name__ == "__main__":

    remote_names = lookup_remote_names()
    conan_install_cmake = locate_conaninstall_cmake()
    matches = parse_conanfile(filepath=conan_install_cmake)

    need_updates = {}
    for m in matches:
        p = m["package"]
        cur_v_str = m["version"]
        cur_rev = m["rev"]

        ret = find_update_for_package(
            package=p, current_version=cur_v_str, current_rev=cur_rev
        )
        if ret is not None:
            last_v_str, last_rev = ret
            need_updates[p] = {
                "from": f"{p}/{cur_v_str}#{cur_rev}",
                "to": f"{p}/{last_v_str}#{last_rev}",
            }
    if need_updates:
        print(
            "\n\n:fire: :fire_engine: "
            f"[bold cyan]{len(need_updates)} packages need updates[/]\n"
        )
        update_conanfile(filepath=conan_install_cmake, updates=need_updates)
        exit(1)
    exit(0)
