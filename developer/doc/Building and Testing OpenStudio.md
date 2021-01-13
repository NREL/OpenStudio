Process of Building and Testing OpenStudio
======================================================
# Contents
- [Windows](#Windows%20Build%20Environment)
- [Linux](#Linux%20Build%20Environment)
- [Mac](#Mac%20Build%20Environment)
- [Unit Testing](#Unit%20Testing)
- [Regression Testing](#Regression%20Testing)

## Windows Build Environment
64-bit Windows 7 – 10
 
**Required**
* [Visual Studio 2019](https://visualstudio.microsoft.com/vs/) (Community versions is free)
* [CMake](https://cmake.org/download/)(v3.15 or newer)
* [conan](https://conan.io/) (v1.28 or newer)  You need to install Python first, and once that is installed, open a terminal and run `pip install conan` to install for that user, or `sudo pip install conan` to install globally for all users.  Verify conan has been installed by running `conan -v` 

**Optional**
* [nuget.exe](https://www.nuget.org/downloads) This is for creating nuget packages. You can skip this if you are not planning on building this component. It is disabled by default 
* [QtIFW](https://download.qt.io/official_releases/qt-installer-framework/3.2.2/QtInstallerFramework-win-x86.exe) This for creating binary installer packages. 
* [Doxygen v1.8.8](http://ftp.stack.nl/pub/users/dimitri/doxygen-1.8.8-setup.exe) For building docs. 

For installed dependencies such as (e.g. cmake, nuget, QtIFW) be sure to include these to your PATH. Usually this is done automatically via installer but not always. 

When installing Visual Studio Community 2019, you will be prompted to select which components to install. Below is a link to the `.vsconfig` that we use on the OpenStudio CI build agents. This is using Visual Studio 2019 Community Edition v16.7.1

Download VS Config: [vsconfig](https://openstudio-dependencies.s3.amazonaws.com/visual-studio-2019/install-options/config/.vsconfig) 

You can import this file using the VS Installer:  

![vs-2019-installer](https://openstudio-dependencies.s3.amazonaws.com/visual-studio-2019/install-options/img/vsinstaller.png)

Once all the dependencies are installed, set up the build using CMake. The example below is for a headless build using PowerShell. You can also use Visual Studio and select new CMake project and navigate to the root of the openStudio clone repo. 

This setting below is for a `Release` build of OpenStudio.  Note, you can customize the build options by toggling the ON/OFF cmake settings that you do not need. Here is an example of a CMake setup that builds openstudio, csharp, documentation, testing and creates installer packages. 

Make sure you create a `build` directory in the base repo before running these cmake configure commands. 

`cmake -G "Visual Studio 16 2019" -A x64 -DBUILD_CSHARP_BINDINGS=ON -DBUILD_DOCUMENTATION=ON -DBUILD_TESTING=ON -DBUILD_PACKAGE=ON -DCMAKE_BUILD_TYPE=Release -DCPACK_BINARY_IFW=ON -DCPACK_BINARY_TGZ=ON ../`

Here is an example of a debug build with the same settings as above. This is useful if using the Visual Studio debugger tools.   

`cmake -G "Visual Studio 16 2019" -A x64 -DBUILD_CSHARP_BINDINGS=ON -DBUILD_DOCUMENTATION=ON -DBUILD_TESTING=ON -DBUILD_PACKAGE=OFF -DCMAKE_BUILD_TYPE=Debug ../`

Once the CMake configure step completes, you can build your target. For example, you can build the target `package` which builds and then uses QTIFW to create the binary installer packages.  

`$ cmake --build . --config Release --target package`

If you want to skip the installer, you can also just build openstudio.

`$ cmake --build . --config Release --target openstudio`


## Linux Build Environment
Ubuntu 18.04

**Required**

```shell
 sudo apt install build-essential git cmake-curses-gui cmake-gui libssl-dev libxt-dev \
    libncurses5-dev libgl1-mesa-dev autoconf libexpat1-dev libpng-dev libfreetype6-dev \
    libdbus-glib-1-dev libglib2.0-dev libfontconfig1-dev libxi-dev libxrender-dev \
    libgeographic-dev libicu-dev chrpath bison libffi-dev libgdbm-dev libqdbm-dev \
    libreadline-dev libyaml-dev libharfbuzz-dev libgmp-dev patchelf python-pip
```
* [conan](https://conan.io/) (v1.28 or newer)  You need to install Python first, and once that is installed, open a terminal and run `pip install conan` to install for that user, or `sudo pip install conan` to install globally for all users.  Verify conan has been installed by running `conan -v`
Other dependencies: 

* cmake - The cmake version that ships via apt package manager for Ubuntu 18.04 is < 3.15, so run the commands below to build and install cmake for newer versions. 

```shell
CMAKE_VERSION=3.15.0 # Update with latest by checking https://cmake.org/download/
wget https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION.tar.gz
tar xfz cmake-$CMAKE_VERSION.tar.gz
cd cmake-$CMAKE_VERSION
./configure
make -j$(nproc)
sudo make install
```
  
**Optional**
* [QtIFW](https://download.qt.io/official_releases/qt-installer-framework/3.2.2/QtInstallerFramework-linux-x64.run)


After you install the required dependencies, you can clone the repository and run cmake setup and build a target. Below is an example of cloning the repo at develop branch and running a basic cmake config and building the target package. 

```shell
git clone https://github.com/NREL/OpenStudio  
cd OpenStudio  
mkdir build 
cd build  
cmake -DBUILD_TESTING=ON -DBUILD_PACKAGE=ON -DCMAKE_BUILD_TYPE=Release -DCPACK_BINARY_DEB=ON -DCPACK_BINARY_TGZ=ON -DCPACK_BINARY_TZ=OFF ../  
make -j $(nproc) package
``` 
## Mac Build Environment
#### 10.14 and higher are supported

**Required**
* [Xcode](https://developer.apple.com/support/xcode/)
* [CMake](https://cmake.org/download/)
* [conan](https://conan.io/) (version 1.28 or newer)  You need to install python (install version 3 or greater) and then in a terminal run `pip install conan` or `sudo pip install conan`, and make sure that your python bin dir is in your path. After that the rest of it should be automatic.

***Optional***
* [QtIFW](https://download.qt.io/official_releases/qt-installer-framework/3.2.2/QtInstallerFramework-mac-x64.dmg)
* [Doxygen v1.8.8](http://ftp.stack.nl/pub/users/dimitri/Doxygen-1.8.8.dmg)

After you install the required dependencies, you can clone the repository and run cmake setup and build a target. Below is an example of cloning the repo at develop branch and running a basic cmake config and building the target package. 

```shell
git clone https://github.com/NREL/OpenStudio
cd OpenStudio
mkdir build 
cd build  
cmake -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 -DBUILD_TESTING=ON -DBUILD_PACKAGE=ON -DCMAKE_BUILD_TYPE=Release  -DCPACK_BINARY_IFW=ON ../
make -j $(nproc) package
```

## Unit Testing

Provided you set the cmake option `-DBUILD_TESTING=ON` during your cmake config (see examples above),  you can use `ctest` to run the unit test suites.  This works on all platforms. Below are some examples of how to use ctest. 

`ctest -T test --no-compress-output` 

You can also filter by type of tests you want to run (e.g.) 

`ctest -T test --no-compress-output -R IddFixture*` 

You can also speed up the unit tests by running in parallel by using the `-j` flag for num cores

e.g. on a 4 core system
`ctest -T test -j 4 --no-compress-output` 

## Regression Testing

Please consult https://github.com/NREL/OpenStudio-resources for all SDK regression testing. 
