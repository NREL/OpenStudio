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

You can pass `--update` to the above command so make it look for newest revisions in the remotes (probably use conancenter first in line)


---

**If you want to pull the latest revision from the remote, ignoring your local cache**, instead of actually installing packages which takes a while, you probably want to do this instead.

While doing so, I suggest you make the conancenter remote first in line, so it doesn't find the revision from the NLR Artifactory which are likely outdated.

You'll need to remove conan.lock first, or maybe keep only a couple of requires references in there (eg: libxslt/libxml2 to help it resolve conflicts)

```shell
conan lock create conanfile.py --update --lockfile=conan.lock --lockfile-out=conan.lock
```

## Running Tests

After building, you can run the test suite using CTest:

```shell
cd ../OS-build-release
ctest --output-on-failure
```

### Test Filtering

OpenStudio tests are organized with labels for selective execution:

**Run tests excluding network-dependent tests** (useful for offline work or unreliable connections):
```shell
ctest -LE network --output-on-failure
```

**Run only specific test categories**:
```shell
# Bundle/gem tests
ctest -L bundle --output-on-failure

# CLI tests
ctest -L cli --output-on-failure

# Geometry tests
ctest -L geometry --output-on-failure

# Previously flaky tests (for verification)
ctest -L flaky --output-on-failure
```

**Run a specific test by name**:
```shell
ctest -R "ModelFixture.Space_Convexity" --output-on-failure
```

### Handling Flaky Tests

Some tests may occasionally fail due to network issues or race conditions. Use the `--repeat` flag:

```shell
# Repeat until the test passes (max 10 times)
ctest -R "test_bundle" --repeat until-pass:10 --output-on-failure
```

### Troubleshooting Bundle Tests

Bundle tests require network access to rubygems.org. If they fail:

1. **Check network connectivity**: Ensure you can reach `rubygems.org`
2. **Skip network tests**: Use `ctest -LE network` to skip them
3. **Configure Bundler retries**: The project includes `.bundle/config` with retry settings
4. **Increase timeout**: Set `BUNDLE_TIMEOUT` environment variable:
   ```shell
   export BUNDLE_TIMEOUT=120
   ctest -R "test_bundle" --output-on-failure
   ```

### Test Utility Scripts

For CI and development workflows, use the test utilities:

```shell
# Source the utility functions
source ci/test_utils.sh

# Show available functions
show_test_utils_help

# Run tests offline (excluding network tests)
run_tests_offline

# Run network tests with safety checks
run_network_tests_safe

# Verify flaky tests are fixed (run 20 times)
run_flaky_tests 20

# Run a specific test with retry logic
run_test_with_retry "ModelFixture.Building_Clone" 3
```

### Parallel Test Execution

To speed up testing, run tests in parallel:

```shell
ctest -j8 --output-on-failure  # Run with 8 parallel jobs
```

**Note**: Some tests may interfere with each other when run in parallel. If you encounter issues, run tests serially or use labels to isolate problematic tests.

## Known issues with the Conan `ruby/3.2.2` package

* `lib/ruby/3.2.0/rubygems.rb` in `ruby/3.2.2#e349279c358fd8f54d83446a3af8ecfe` replaces upstream's `autoload :SpecFetcher` with eager `require`s (e.g. `rubygems/spec_fetcher`) before `Gem::Specification.load_defaults` runs. This loads stdlib `securerandom` (0.2.2) first, so `Bundler.setup` fails with `You have already activated securerandom 0.2.2, but your Gemfile requires securerandom 0.4.1` when a `Gemfile.lock` pins a newer version. The fix belongs in the Conan recipe (restore the `autoload`, or run `load_defaults` first); see #5666.
* Portable macOS Ruby tarballs (e.g. `ruby-3.2.2-darwin-arm64.tar.gz`) must be created with `COPYFILE_DISABLE=1 tar -czf ...` to avoid `._*` AppleDouble files. Check with `tar -tzf <archive> | grep -E '(^|/)\._'`.
