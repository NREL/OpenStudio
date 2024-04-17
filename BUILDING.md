# Building with conan v2

Check you have `conan >= 2`, and add the `nrel-v2` remote to grab `ruby` and `swig/4.1.1`.

```shell
conan --version
conan remote add -f nrel-v2 http://conan.openstudio.net/artifactory/api/conan/conan-v2
```

## Install the conan dependencies into a build folder

```shell
conan install . --output-folder=../OS-build-release --build=missing -c tools.cmake.cmaketoolchain:generator=Ninja -s compiler.cppstd=20 -s build_type=Release
```

You can also do another configuration, such as `Debug`, `RelWithDeb`, etc

```shell
conan install . --output-folder=../OS-build --build=missing -c tools.cmake.cmaketoolchain:generator=Ninja -s compiler.cppstd=20 -s build_type=Debug
```

You'll have the `conan-release` and `conan-debug` CMake Presets in the root folder. Do `cmake --list-presets` to list the available presets (which are in `CMakeUserPresets.json`)

*Side note:* If you want a specific configure option (such as a build folder in Release mode where you build with `-DBUILD_CSHARP_BINDINGS:BOOL=ON`) in a different build dir for the same source dir, you probably will need to go and edit the `<build_dir>/CMakePresets.json` to rename it to another configuration name so it does not clash with the preset `conan-release`

## CMake Configure and build

### With presets

#### Why are Presets recommended

The recommend flow is to use [CMake Presets](https://cmake.org/cmake/help/latest/manual/cmake-presets.7.html).

If you notice the messages printed to the console when you run the `conan install` command from above,
you'll see that the `conanfile` actually defines common build variables for you already,
such as defining which `CPack` Generators to use depending on your target platform, trying to infer the `Python_ROOT_DIR` etc

<pre><font color="#75507B"><b>conanfile.py:</b></font> <font color="#75507B"><b>Calling generate()</b></font>
conanfile.py: Generators folder: /path/to/OS-build-release
conanfile.py: Setting PYTHON_VERSION and Python_ROOT_DIR from your current python: 3.8.13, &apos;/home/julien/.pyenv/versions/3.8.13&apos;
conanfile.py: CMakeToolchain generated: conan_toolchain.cmake
conanfile.py: Preset &apos;conan-release&apos; added to CMakePresets.json. Invoke it manually using &apos;cmake --preset conan-release&apos; if using CMake&gt;=3.23
conanfile.py: If your CMake version is not compatible with CMakePresets (&lt;3.23) call cmake like:
    cmake &lt;path&gt; -G Ninja -DCMAKE_TOOLCHAIN_FILE=/path/to/OS-build-release/conan_toolchain.cmake \
      -DBUILD_CLI=ON -DBUILD_RUBY_BINDINGS=ON -DBUILD_PYTHON_BINDINGS=ON \
      -DBUILD_PYTHON_PIP_PACKAGE=OFF -DBUILD_TESTING=ON -DBUILD_BENCHMARK=ON \
      -DCPACK_BINARY_TGZ=ON -DCPACK_BINARY_IFW=OFF -DCPACK_BINARY_DEB=ON -DCPACK_BINARY_NSIS=OFF \
      -DCPACK_BINARY_RPM=OFF -DCPACK_BINARY_STGZ=OFF -DCPACK_BINARY_TBZ2=OFF \
      -DCPACK_BINARY_TXZ=OFF -DCPACK_BINARY_TZ=OFF \
      -DPYTHON_VERSION=3.8.13 -DPython_ROOT_DIR=/home/julien/.pyenv/versions/3.8.13 \
      -DCMAKE_POLICY_DEFAULT_CMP0091=NEW -DCMAKE_BUILD_TYPE=Release
conanfile.py: CMakeToolchain generated: CMakePresets.json
conanfile.py: CMakeToolchain generated: ../OpenStudio/CMakeUserPresets.json
<font color="#75507B"><b>conanfile.py:</b></font> <font color="#75507B"><b>Generating aggregated env files</b></font>
conanfile.py: Generated aggregated env files: [&apos;conanbuild.sh&apos;, &apos;conanrun.sh&apos;]
<font color="#4E9A06"><b>Install finished successfully</b></font>
</pre>

**Note that this is also supported by Visual Studio (MSVC)**.

#### Configure and build with Presets

Run these commands from the **source** directory (`OpenStudio/`).

```shell
cmake --preset conan-release [any_futher_configuration_options]
```

Example:

```
cmake --preset conan-release \
  -DBUILD_PYTHON_BINDINGS:BOOL=ON -DBUILD_PYTHON_PIP_PACKAGE:BOOL=ON \
  -DPYTHON_VERSION=3.8.13 -DPython_ROOT_DIR:PATH=$HOME/.pyenv/versions/3.8.13/ \
  -DBUILD_CSHARP_BINDINGS:BOOL=ON
```

Building

```
cmake --build --preset conan-release
```

### Atlernatively: manual CMake

First, go to the **build** directory, and **activate the conan build environment**.

```shell
cd ../OS-build-release
# Unix
. ./conanbuild.sh
# Windows
call conanbuild.bat
```

Still in the build directory, run cmake, but do pass the `CMAKE_TOOLCHAIN_FILE`

```
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE:STRING=Release \
  -DBUILD_TESTING:BOOL=ON -DCPACK_BINARY_TGZ:BOOL=ON -DCPACK_BINARY_DEB:BOOL=ON \
  -DCPACK_BINARY_IFW:BOOL=OFF -DCPACK_BINARY_NSIS:BOOL=OFF -DCPACK_BINARY_RPM:BOOL=OFF -DCPACK_BINARY_STGZ:BOOL=OFF \
  -DCPACK_BINARY_TBZ2:BOOL=OFF -DCPACK_BINARY_TXZ:BOOL=OFF -DCPACK_BINARY_TZ:BOOL=OFF \
  -DBUILD_PYTHON_BINDINGS:BOOL=ON -DBUILD_PYTHON_PIP_PACKAGE:BOOL=ON \
  -DPYTHON_VERSION=3.8.13 -DPython_ROOT_DIR:PATH=/home/julien/.pyenv/versions/3.8.13/  \
  -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON \
  ../OpenStudio
```

You can deactivate now if you want

```
# Unix
. ./deactivate_conanbuild.sh
# Windows
call deactivate_conanbuild.bat
```

# Full Example

```
git clone git@github.com/NREL/OpenStudio.git
cd OpenStudio
conan install . --output-folder=../OS-build-release --build=missing -c tools.cmake.cmaketoolchain:generator=Ninja -s compiler.cppstd=20 -s build_type=Release
cmake --preset conan-release
cmake --build --preset conan-release
```

# Updating the Conan Lockfile

If you want to update a dependency in the `conan.lock`, just delete the line, and use this:


```shell
conan install . --output-folder=../OS-build-release --build=missing \
   -c tools.cmake.cmaketoolchain:generator=Ninja -s compiler.cppstd=20 -s build_type=Release \
   --lockfile-partial --lockfile-out=conan.lock
```
