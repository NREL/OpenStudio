########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

import sys
import unittest

from packaging import version

sys.path.append("..")
from find_pypi_tag import compute_appropriate_version


class TestFindTestPyPiTag(unittest.TestCase):
    def compare(self, releases: list[str], v: str, expected_v: str):
        """
        Test helper, to avoid repeating myself
        """
        new_v = compute_appropriate_version(
            current_v=version.parse(v), releases=[version.parse(v) for v in releases], current=False
        )
        self.assertEqual(version.parse(expected_v), new_v)

    def test_when_not_exist_with_prereleasetag(self):
        self.compare(releases=["3.2.0"], v="3.2.1-alpha", expected_v="3.2.1a0")

    def test_when_not_exist_no_prereleasetag(self):
        self.compare(releases=["3.2.0"], v="3.2.1", expected_v="3.2.1")

    def test_when_exist_with_prereleasetag(self):
        self.compare(releases=["3.2.1"], v="3.2.1-alpha", expected_v="3.2.1a0")

        self.compare(releases=["3.2.1", "3.2.1a0"], v="3.2.1-alpha", expected_v="3.2.1a1")

        self.compare(releases=["3.2.1", "3.2.1a0", "3.2.1a1"], v="3.2.1-alpha", expected_v="3.2.1a2")

        # Different prerelease tag
        self.compare(releases=["3.2.1", "3.2.1a0", "3.2.1a1"], v="3.2.1-beta", expected_v="3.2.1b0")

        self.compare(releases=["3.2.1", "3.2.1a0", "3.2.1a1", "3.2.1b0"], v="3.2.1-beta", expected_v="3.2.1b1")

        self.compare(releases=["3.2.1", "3.2.1a20", "3.2.1b0", "3.2.1b10"], v="3.2.1-beta", expected_v="3.2.1b11")

    def test_with_rctag(self):
        self.compare(releases=["3.2.0", "3.2.1a1"], v="3.2.1-rc1", expected_v="3.2.1rc1")

        # Here I actually expect a post release to be appended
        self.compare(releases=["3.2.0", "3.2.1a1", "3.2.1rc1"], v="3.2.1-rc1", expected_v="3.2.1rc1.post0")

        self.compare(releases=["3.2.0", "3.2.1a1", "3.2.1rc1"], v="3.2.1-rc2", expected_v="3.2.1rc2")

        # Now we release official
        self.compare(releases=["3.2.0", "3.2.1a1", "3.2.1rc1"], v="3.2.1", expected_v="3.2.1")

    def test_when_exist_no_prereleasetag(self):
        self.compare(releases=["3.2.1", "3.2.1a1"], v="3.2.1", expected_v="3.2.1post0")


class TestFindTestPyPiTagCurrent(unittest.TestCase):
    def compare(self, releases: list[str], v: str, expected_v: str):
        """
        Test helper, to avoid repeating myself
        """
        new_v = compute_appropriate_version(
            current_v=version.parse(v), releases=[version.parse(v) for v in releases], current=True
        )
        self.assertEqual(version.parse(expected_v), new_v)

    def test_when_not_exist_with_prereleasetag(self):
        self.compare(releases=["3.2.0"], v="3.2.1-alpha", expected_v="3.2.1a0")

    def test_when_not_exist_no_prereleasetag(self):
        self.compare(releases=["3.2.0"], v="3.2.1", expected_v="3.2.1")

    def test_when_exist_with_prereleasetag(self):
        self.compare(releases=["3.2.1"], v="3.2.1-alpha", expected_v="3.2.1a0")

        self.compare(releases=["3.2.1", "3.2.1a0"], v="3.2.1-alpha", expected_v="3.2.1a0")

        self.compare(releases=["3.2.1", "3.2.1a0", "3.2.1a1"], v="3.2.1-alpha", expected_v="3.2.1a1")

        # Different prerelease tag
        self.compare(releases=["3.2.1", "3.2.1a0", "3.2.1a1"], v="3.2.1-beta", expected_v="3.2.1b0")

        self.compare(releases=["3.2.1", "3.2.1a0", "3.2.1a1", "3.2.1b0"], v="3.2.1-beta", expected_v="3.2.1b0")

        self.compare(releases=["3.2.1", "3.2.1a20", "3.2.1b0", "3.2.1b10"], v="3.2.1-beta", expected_v="3.2.1b10")

    def test_with_rctag(self):
        self.compare(releases=["3.2.0", "3.2.1a1"], v="3.2.1-rc1", expected_v="3.2.1rc1")

        # Here I actually expect a post release to be appended
        self.compare(releases=["3.2.0", "3.2.1a1", "3.2.1rc1"], v="3.2.1-rc1", expected_v="3.2.1rc1")

        self.compare(releases=["3.2.0", "3.2.1a1", "3.2.1rc1"], v="3.2.1-rc2", expected_v="3.2.1rc2")

        # Now we release official
        self.compare(releases=["3.2.0", "3.2.1a1", "3.2.1rc1"], v="3.2.1", expected_v="3.2.1")

    def test_when_exist_no_prereleasetag(self):
        self.compare(releases=["3.2.1", "3.2.1a1"], v="3.2.1", expected_v="3.2.1")
