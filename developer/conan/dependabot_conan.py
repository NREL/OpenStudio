import re
import shlex
import subprocess
from pathlib import Path
from typing import List, Optional

from dateutil import parser
from packaging import version
from rich import print

RE_CONAN = re.compile(
    r"(?P<package>\w+)/(?P<version>\d+\.\d+(?:\.\w+)?)"
    r"(?:@)?(?P<user>\w+)?(?:/)?(?P<channel>\w+)?"
    r"#?(?P<rev>\w+)?"
)


def locate_conan_files(base_dir: Path, include_cmake_files: Optional[bool] = False) -> List[Path]:
    files = list(base_dir.glob("**/conanfile.py")) + list(base_dir.glob("**/conanfile.txt"))
    if include_cmake_files:
        cmake_files = list(base_dir.glob("**/*.cmake")) + list(base_dir.glob("**/CMakeLists.txt"))
        for cmake_file in cmake_files:
            print(f"reading {cmake_file}")
            with open(cmake_file, "r") as f:
                content = f.read()
            if "conan_cmake_" in content:
                files.append(cmake_file)

    if not files:
        raise IOError(f"Found zero conanfile.py/conanfile.txt in {base_dir}")
    print(files)
    return files


class RemoteInfo:
    # RE_REMOTE = re.compile(
    #    r'(?P<name>\w+): (?P<url>(?:https?://[a-z0-9.\-/]+)) '
    #    r'\[Verify SSL: (?P<ssl>(?:True|False))\]'
    # )

    RE_REMOTE = re.compile(r"(?P<name>\w+): (?P<url>.+) \[Verify SSL: (?P<ssl>(?:True|False))\]")

    @classmethod
    def from_conan_str(cls, line: str):
        d = cls.RE_REMOTE.match(line).groupdict()
        return RemoteInfo(**d)

    def __init__(self, name: str, url: str, ssl: bool):
        self.name = name
        self.url = url
        self.ssl = ssl

    def __repr__(self):
        return str(self.__dict__)

    def __str__(self):
        return f"Remote '{self.name}'"


# Forward declare for type hint of the factory method
class PackageInfo:
    pass


class PackageInfo:
    @staticmethod
    def from_str(line: str) -> Optional[PackageInfo]:
        if m := RE_CONAN.search(line):
            # print(f"Trying to construct PackageInfo from {line}")
            d = m.groupdict()
            p = PackageInfo(**d)
            return p

    def __init__(
        self,
        package: str,
        version: str,
        user: Optional[str],
        channel: Optional[str],
        rev: Optional[str],
    ):
        self.package = package
        self.version = version
        self.user = user
        self.channel = channel
        self.rev = rev

        self.last_known_version = self.version
        self.last_known_v_remote = None
        self.last_known_rev = None
        self.last_revs_info = None
        self.need_update = None

        self.force_version = None

    def __repr__(self):
        d = {}
        for k, v in self.__dict__.items():
            if v is not None:  # and k != 'last_known_version':
                d[k] = v
        return f"PackageInfo: {d}"

    def __str__(self):
        s = f"{self.package}/{self.version}"
        if self.user:
            s += f"@{self.user}/{self.channel}"
        if self.rev:
            s += f"#{self.rev}"
        return s

    def _lookup_query(self, version: Optional[str] = None):
        s = f"{self.package}/"
        if version:
            s += f"{version}@"
            if self.user:
                s += f"{self.user}/{self.channel}"
        else:
            s += "*"
            if self.user:
                s += f"@{self.user}/{self.channel}"
        return s

    def _lookup_all_v(self, remote: RemoteInfo) -> List[str]:
        query = self._lookup_query(version=None)
        cmd = f"conan search -r {remote.name} {query} --raw"
        # print(cmd)
        r = subprocess.check_output(shlex.split(cmd))
        # print(r.decode().splitlines())

        # Filter out the cci.DATE stuff
        known_versions = [x for x in r.decode().splitlines() if "cci." not in x]

        known_versions = [
            m.version
            for x in known_versions
            if (m := PackageInfo.from_str(x)) is not None and ((self.user is None) == (m.user is None))
        ]

        # Filter prereleases
        known_versions = list(filter(lambda v: not version.parse(v).is_prerelease, known_versions))

        # Force a version
        if self.force_version:
            known_versions = [x for x in known_versions if self.force_version in x]

        known_versions.sort(key=lambda v: version.parse(v))
        return known_versions

    def _lookup_last_v(self, remotes: List[RemoteInfo]) -> str:
        """
        Search conan center remote for the latest version of a package
        I'm deliberately returning a str and not a Strict/LooseVersion
        to avoid missing the minor when it's zero
        """
        found = False
        query = self._lookup_query(version=None)
        for remote in remotes:
            known_versions = self._lookup_all_v(remote=remote)
            if not known_versions:
                # print(f"{query} not found in {remote}")
                continue

            # print(f"{query} found in {remote}")
            found = True
            last_v_str = known_versions[-1]
            if version.parse(last_v_str) >= version.parse(self.last_known_version):
                # print(f"FOUND {last_v_str} in {remote}")
                self.last_known_version = last_v_str
                self.last_known_v_remote = remote
        if not found:
            raise ValueError(f"Could not find {query} in any of the remotes: {remotes}")
        return self.last_known_version

    def _lookup_all_revs_for_version(self, version: Optional[str] = None) -> str:
        if version is None:
            version = self.version

        query = self._lookup_query(version=version)

        cmd = f"conan search -r {self.last_known_v_remote.name} " f"{query} -rev --raw"
        print(cmd)
        r = subprocess.check_output(shlex.split(cmd))

        revs_info = {}
        for rev_and_date in r.decode().splitlines():
            rev, date = rev_and_date.split("(")
            rev = rev.strip()
            d = date.split(")")[0]
            d = parser.parse(d)

            revs_info[rev] = d
        return revs_info

    def check_updates(self, remotes: List[RemoteInfo]) -> bool:
        if not remotes:
            raise ValueError("remotes must be a list of RemoteInfo!")

        self.need_update = False
        self._lookup_last_v(remotes=remotes)
        last_v = version.parse(self.last_known_version)
        cur_v = version.parse(self.version)

        if self.rev:
            self.last_revs_info = self._lookup_all_revs_for_version(version=self.last_known_version)
            self.last_known_rev = max(self.last_revs_info, key=lambda k: self.last_revs_info[k])

        if cur_v > last_v:
            # Not expected, though that might be possible
            # (if we do a new version on a fork)
            raise ValueError(f"Package {self}: {cur_v=} > {last_v=}")

        elif cur_v == last_v:
            if self.rev is None:
                print(
                    f"\n:white_check_mark: [green]Package {self} is using " "the latest version and has no revision[/]"
                )
                # No-op!
                return False
            elif self.rev == self.last_known_rev:
                print(f"\n:white_check_mark: [green]Package {self} is using " "the latest version and revision[/]")
                # No-op!
                return False
            else:
                print(
                    f"\n:collision: Package [yellow bold]{self}[/] is using "
                    "the latest version but an outdated revision on "
                    f"{self.last_known_v_remote}"
                )
                if self.rev in self.last_revs_info:
                    cur_rev_date = self.last_revs_info[self.rev]
                    last_rev_date = self.last_revs_info[self.last_known_rev]
                    dt = last_rev_date - cur_rev_date
                    print(
                        f"Current revision {self.rev} is outdated by "
                        f"{dt.days} days. "
                        f"New rev {self.last_known_rev} was released "
                        f"on {last_rev_date.date().isoformat()}"
                    )

        elif cur_v < last_v:
            print(
                f"\n:cross_mark: Package [red bold]{self}[/] is using {cur_v} "
                f"when {last_v} is available on {self.last_known_v_remote}"
            )

        self.need_update = True
        return True

    def get_replacement(self):
        new_p = PackageInfo(
            package=self.package,
            version=self.last_known_version,
            user=self.user,
            channel=self.channel,
            rev=self.last_known_rev,
        )
        return f"{self}", f"{new_p}"


class ConanFileUpdater:
    __conan_center_url = "center.conan.io"
    all_remotes_known = None
    conan_center = None

    @classmethod
    def update_all_remotes_known(cls, force_update: Optional[bool] = False):
        if cls.all_remotes_known is None or force_update:
            print("Updating remotes")
            cls.all_remotes_known = []
            r = subprocess.check_output(shlex.split("conan remote list"))
            for line in r.decode().splitlines():
                cls.all_remotes_known.append(RemoteInfo.from_conan_str(line=line))
            print(f"Found {len(cls.all_remotes_known)} remotes:")
            for remote in cls.all_remotes_known:
                if cls.__conan_center_url in remote.url:
                    cls.conan_center = remote
                print(remote)

            if cls.conan_center is None:
                raise ValueError("Could not find any remote for conancenter: " f"{cls.__conan_center_url}")

    def __init__(self, filepath: Path):
        if not filepath.exists():
            raise ValueError("Conanfile {filepath} does not exist!")
        # Get all remotes if not done already
        self.update_all_remotes_known()
        self.package_remote_assignments = {}

        self.filepath = filepath
        self.packages = self._parse_conanfile()
        self.need_updates = 0

    def _parse_conanfile(self) -> List[dict]:
        with open(self.filepath, "r") as f:
            content = f.read()
        lines = content.splitlines()

        packages = []
        for line in lines:
            line = line.strip()
            if line.startswith("#"):
                continue
            if m := RE_CONAN.search(line):
                d = m.groupdict()
                p = PackageInfo(**d)
                # Dynamically add an attribute to track whether
                # we want to check all remotes or not
                p.use_all_remotes = False
                packages.append(p)

        return packages

    def flag_package_to_check_in_all_remotes(self, package_name: str):
        for p in self.packages:
            if p.package == package_name:
                p.use_all_remotes = True

    def force_package_version(self, package_name: str, version_contains: str):
        for p in self.packages:
            if p.package == package_name:
                p.force_version = version_contains

    def __lookup_package_updates(self):
        self.need_updates = 0
        for package in self.packages:
            remotes = [self.conan_center]
            if package.use_all_remotes:
                remotes = self.all_remotes_known
            for remote in remotes:
                if package.check_updates(remotes=remotes):
                    self.need_updates += 1

    def update_conanfile(self) -> bool:
        print("\n:crossed_fingers: [bold yellow]Checking " f"{self.filepath} for updates[/]")
        self.__lookup_package_updates()
        if self.need_updates == 0:
            print("\n:+1: [bold green]Everything up to date[/]")
            return False

        print("\n\n:fire: :fire_engine: " f"[bold cyan]{self.need_updates} packages need updates[/]\n")

        with open(self.filepath, "r") as f:
            content = f.read()
        for package in self.packages:
            if package.need_update:
                from_str, to_str = package.get_replacement()
                print(f"{from_str} => {to_str}")
                content = content.replace(from_str, to_str)

        with open(self.filepath, "w") as f:
            f.write(content)

        print(f"\n:+1: [bold green]Updated {self.filepath}[/]")
        return True


if __name__ == "__main__":
    base_dir = Path(__file__).resolve().parent.parent.parent

    conanfile = base_dir / "ConanInstall.cmake"

    conanfileupdater = ConanFileUpdater(filepath=conanfile)

    conanfileupdater.flag_package_to_check_in_all_remotes(package_name="openstudio_ruby")

    conanfileupdater.force_package_version(package_name="openssl", version_contains="1.1.1")

    if conanfileupdater.update_conanfile():
        exit(1)
    exit(0)
