# Building with conan v2

Check you have conan v2, and add the nrel-v2 remote to grab ruby and swig/4.1.1

```shell
conan --version
conan remote add -f nrel-v2 http://conan.openstudio.net/artifactory/api/conan/conan-v2
```


```shell
conan install . --output-folder=../OS-build-release-v2 --build=missing -c tools.cmake.cmaketoolchain:generator=Ninja -s compiler.cppstd=20 -s build_type=Release
cd ../OS-build-release-v2
. ./conanbuild.sh

# Run cmake, but do pass the CMAKE_TOOLCHAIN_FILE
#/OS-build-release-v2 %
cmake -G Ninja -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE:STRING=Release \
  -DBUILD_TESTING:BOOL=ON -DCPACK_BINARY_TGZ:BOOL=ON -DCPACK_BINARY_DEB:BOOL=ON \
  -DCPACK_BINARY_IFW:BOOL=OFF -DCPACK_BINARY_NSIS:BOOL=OFF -DCPACK_BINARY_RPM:BOOL=OFF -DCPACK_BINARY_STGZ:BOOL=OFF \
  -DCPACK_BINARY_TBZ2:BOOL=OFF -DCPACK_BINARY_TXZ:BOOL=OFF -DCPACK_BINARY_TZ:BOOL=OFF \
  -DBUILD_PYTHON_BINDINGS:BOOL=ON -DBUILD_PYTHON_PIP_PACKAGE:BOOL=ON \
  -DPYTHON_VERSION=3.8.13 -DPython_ROOT_DIR:PATH=/home/julien/.pyenv/versions/3.8.13/  \
  -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON \
  ../OpenStudio

# You can deactivate now if you want
. ./deactivate_conanbuild.sh
```

Note that is also makes cmake presets, at root of OpenStudio, you can also try `cmake --list-presets`. To configure `cmake --preset conan-release`; to build `cmake --build --preset conan-release`.


If you want to update a dependency in the conan.lock, just delete the line, and use this:


```shell
conan install . --output-folder=../OS-build-release-v2 --build=missing -c tools.cmake.cmaketoolchain:generator=Ninja -s compiler.cppstd=20 -s build_type=Release --lockfile-partial --lockfile-out=conan.lock
```
