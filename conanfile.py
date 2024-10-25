import sys
from pathlib import Path

from conan import ConanFile
from conan.tools.apple import is_apple_os
from conan.tools.cmake import CMakeToolchain

required_conan_version = ">=2.0"


class OpenStudioBuildRecipe(ConanFile):
    name = "openstudio"
    description = "Build recipe for OpenStudio SDK which install necessary conan dependencies"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps"  # CMakeToolchain explicitly instantiated

    options = {
        "with_ruby": [True, False],
        "with_python": [True, False],
        "with_csharp": [True, False],
        "with_testing": [True, False],
        "with_benchmark": [True, False],
    }
    default_options = {
        "with_ruby": True,
        "with_python": True,
        "with_csharp": False,
        "with_testing": True,
        "with_benchmark": True,
    }

    def requirements(self):
        if self.options.with_ruby:
            self.requires("ruby/3.2.2")

        if is_apple_os(self):
            self.requires(
                "boost/1.79.0", force=True, options={"visibility": "global"}
            )  # f664bfe40e2245fa9baf1c742591d582
        else:
            self.requires("boost/1.79.0", force=True)
        self.requires("pugixml/1.12.1")  # 5a39f82651eba3e7d6197903a3202e21
        self.requires(
            "libxml2/[<2.12.0]"
        )  # deprecated xmlSubstituteEntitiesDefault and co https://github.com/GNOME/libxml2/commit/db8b9722cb2a1f7dca7374ec38ecaa4936ab3869
        self.requires("libxslt/1.1.37")  # 9085031f5b9b2bb328ad615cd1bf1282
        self.requires("jsoncpp/1.9.5")  # 536d080aa154e5853332339bf576747c
        self.requires("fmt/9.1.0")  # 811e918ca4b4e0b9ddd6d5a2883efa82
        self.requires("sqlite3/3.38.5")  # 010911927ce1889b5cf824f45e7cd3d2
        self.requires("cpprestsdk/2.10.19")  # df2f6ac88e47cadd9c9e8e0971e00d89
        self.requires("websocketpp/0.8.2")  # 3fd704c4c5388d9c08b11af86f79f616
        self.requires("geographiclib/1.52")  # 76536a9315a003ef3511919310b2fe37
        self.requires("swig/4.1.1")  # Pending https://github.com/conan-io/conan-center-index/pull/19058
        self.requires("tinygltf/2.5.0")  # c8b2aca9505e86312bb42aa0e1c639ec
        self.requires("cli11/2.3.2")  # 8ccdf14fb1ad53532d498c16ae580b4b

        self.requires(
            "minizip/1.2.13"
        )  # 0b5296887a2558500d0323c6c94c8d02 # This depends on zlib, and basically patches it
        self.requires(
            "zlib/[>=1.2.11 <2]"
        )  # 3b9e037ae1c615d045a06c67d88491ae # Also needed, so we can find zlib.h and co (+ pinning exactly is good)
        self.requires("openssl/[>=3 <4]")

        if self.options.with_testing:
            self.requires("gtest/1.14.0")
        if self.options.with_benchmark:
            self.requires("benchmark/1.8.3")

    # Let people provide their own CMake for now
    # def build_requirements(self):
    #     self.tool_requires("cmake/3.29.0")

    def generate(self):
        tc = CMakeToolchain(self)

        tc.cache_variables["BUILD_CLI"] = bool(self.options.with_ruby)
        tc.cache_variables["BUILD_RUBY_BINDINGS"] = bool(self.options.with_ruby)
        tc.cache_variables["BUILD_PYTHON_BINDINGS"] = bool(self.options.with_python)
        tc.cache_variables["BUILD_CSHARP_BINDINGS"] = bool(self.options.with_csharp)
        # tc.cache_variables["BUILD_NUGET_PACKAGE"] = False
        # tc.cache_variables["BUILD_PYTHON_PIP_PACKAGE"] = False

        tc.cache_variables["BUILD_TESTING"] = bool(self.options.with_testing)
        tc.cache_variables["BUILD_BENCHMARK"] = bool(self.options.with_benchmark)

        tc.cache_variables["CPACK_BINARY_TGZ"] = True
        tc.cache_variables["CPACK_BINARY_IFW"] = False
        tc.cache_variables["CPACK_BINARY_DEB"] = False
        if self.settings.build_type == "Release":
            if is_apple_os(self) or self.settings.os == "Windows":
                tc.cache_variables["CPACK_BINARY_IFW"] = True
            else:
                tc.cache_variables["CPACK_BINARY_DEB"] = True
        tc.cache_variables["CPACK_BINARY_NSIS"] = False
        tc.cache_variables["CPACK_BINARY_RPM"] = False
        tc.cache_variables["CPACK_BINARY_STGZ"] = False
        tc.cache_variables["CPACK_BINARY_TBZ2"] = False
        tc.cache_variables["CPACK_BINARY_TXZ"] = False
        tc.cache_variables["CPACK_BINARY_TZ"] = False

        if self.options.with_python:
            v = sys.version_info
            if (v.major, v.minor) == (3, 12):
                python_version = f"{v.major}.{v.minor}.{v.micro}"
                self.output.info(
                    f"Setting PYTHON_VERSION and Python_ROOT_DIR from your current python: {python_version}, '{sys.base_prefix}'"
                )
                tc.cache_variables["PYTHON_VERSION"] = python_version
                tc.cache_variables["Python_ROOT_DIR"] = str(Path(sys.base_prefix))
            else:
                self.output.warning(
                    "Your current python is not in the 3.12.x range, which is what we target.\n"
                    "You'll need to pass it properly when configuring CMake\n"
                    "via -DPYTHON_VERSION:STRING='3.12.xx' and -DPython_ROOT_DIR:PATH='/path/to/python3.12/'"
                )
        tc.generate()
