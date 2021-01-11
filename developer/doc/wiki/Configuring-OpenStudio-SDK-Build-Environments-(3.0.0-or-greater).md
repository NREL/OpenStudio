> [Wiki](Home) ▸ **Configuring OpenStudio Build Environments**

*Note* For instructions on building previous 2.X branches, please see [[Configuring OpenStudio Build Environments]].  This documentation refers to versions >= 3.0.0 and is easier to use due the using conan C++ package manager.  To build OpenStudio, most developers need only follow the [OpenStudio Core](#openstudio-core) instructions, this process will download all necessary dependencies.  Only developers needing to build OpenStudio dependencies need worry about the advanced [OpenStudio Dependencies](#openstudio-dependencies) instructions.

# Contents
- [Windows](#windows)
- [Linux](#linux)
- [Mac](#mac)

# OpenStudio SDK

This is the fastest, most reliable method of getting a working OpenStudio build.  These instructions assume that you have successfully [cloned the OpenStudio repository](Using-OpenStudio-with-Git-and-GitHub#cloning-the-repository-to-your-local-computer) already. For these instructions, we will refer to the root of the OpenStudio checkout as `.`.  Suggested build directory names are `.\build` or `.\core-build`, these names are ignored by git.*

## Windows
 
**Required**
* [Visual Studio 2019](https://visualstudio.microsoft.com/vs/) Community (free for open source development)
* [CMake 3.15 or greater](https://cmake.org/download/)
* [conan](https://conan.io/) (version 1.28 or newer)  You need to install python (install version 3 or greater) and then in a terminal run `pip install conan` or `sudo pip install conan`, and make sure that your python bin dir is in your path. After that the rest of it should be automatic.

**Optional**
* [nuget.exe](https://www.nuget.org/downloads) This is for creating nuget packages. You can skip this if you are not planning on building this component by turning off cmake setting. `-DBUILD_NUGET_PACKAGE=OFF` 
* [QtIFW](https://download.qt.io/official_releases/qt-installer-framework/3.2.2/QtInstallerFramework-win-x86.exe) This for creating binary installer packages. You can skip this by disabling this via cmake setting `-DCPACK_BINARY_IFW=OFF`
* [Doxygen v1.8.8](http://ftp.stack.nl/pub/users/dimitri/doxygen-1.8.8-setup.exe) You can skip this by turning off `-DBUILD_DOCUMENTATION=OFF`

For any installed dep (e.g. cmake, nuget QtIFW) be sure to include these to your PATH. 

When installing Visual Studio Community 2019, you will be prompted to select which components to install. Below is a link to the `.vsconfig` that we use on the OpenStudio CI build agents. This is using Visual Studio 2019 Community Edition v16.7.1

VS Config: [.vsconfig](https://openstudio-dependencies.s3.amazonaws.com/visual-studio-2019/install-options/config/.vsconfig) 

You can import this file using the VS Installer:  

![vs-2019-installer](https://openstudio-dependencies.s3.amazonaws.com/visual-studio-2019/install-options/img/vsinstaller.png)

Once all the dependencies are installed, you go then set up the build using CMake. The example below is for a headless build using PowerShell. 

This setting below is for a `Release` build of OpenStudio.  Note, you can customize the build options by toggling the ON/OFF cmake settings that you do not need. Here is an example of a CMake setup that builds openstudio, csharp, documentation, testing and creates installer packages. 

`cmake -G "Visual Studio 16 2019" -A x64 -DBUILD_CSHARP_BINDINGS=ON -DBUILD_RADIANCE=OFF -DBUILD_DOCUMENTATION=ON -DBUILD_TESTING=ON -DBUILD_PACKAGE=ON -DCMAKE_BUILD_TYPE=Release -DCPACK_BINARY_DEB=OFF -DCPACK_BINARY_IFW=ON -DCPACK_BINARY_NSIS=OFF -DCPACK_BINARY_RPM=OFF -DCPACK_BINARY_STGZ=OFF -DCPACK_BINARY_TBZ2=OFF -DCPACK_BINARY_TGZ=ON -DCPACK_BINARY_TXZ=OFF -DCPACK_BINARY_TZ=OFF -DBUILD_NUGET_PACKAGE=OFF ../`

Here is an example of a debug build with the same settings as above. This is useful if using the Visual Studio debugger tools.   

`cmake -G "Visual Studio 16 2019" -A x64 -DBUILD_CSHARP_BINDINGS=ON -DBUILD_RADIANCE=OFF -DBUILD_DOCUMENTATION=ON -DBUILD_TESTING=ON -DBUILD_PACKAGE=ON -DCMAKE_BUILD_TYPE=Debug -DCPACK_BINARY_DEB=OFF -DCPACK_BINARY_IFW=ON -DCPACK_BINARY_NSIS=OFF -DCPACK_BINARY_RPM=OFF -DCPACK_BINARY_STGZ=OFF -DCPACK_BINARY_TBZ2=OFF -DCPACK_BINARY_TGZ=ON -DCPACK_BINARY_TXZ=OFF -DCPACK_BINARY_TZ=OFF -DBUILD_NUGET_PACKAGE=OFF ../`

Once the CMake configure step completes, you can build your target. In these cases, you can build the target `package` which builds and then uses QTIFW to build create the binary installer.  

`$ cmake --build . --config Release --target package`

If you want to skip the installer, you can also just build openstudio.

`$ cmake --build . --config Release --target openstudio`


## Linux

#### Ubuntu Bionic 18.04 (primary version of Linux supported).  To install build dependencies:

```shell
 sudo apt install build-essential git cmake-curses-gui cmake-gui libssl-dev libxt-dev \
    libncurses5-dev libgl1-mesa-dev autoconf libexpat1-dev libpng-dev libfreetype6-dev \
    libdbus-glib-1-dev libglib2.0-dev libfontconfig1-dev libxi-dev libxrender-dev \
    libgeographic-dev libicu-dev chrpath bison libffi-dev libgdbm-dev libqdbm-dev \
    libreadline-dev libyaml-dev libharfbuzz-dev libgmp-dev patchelf python-pip
```
Other dependencies: 
* [QtIFW](https://download.qt.io/official_releases/qt-installer-framework/3.2.2/QtInstallerFramework-linux-x64.run)
* [conan](https://conan.io/) (version 1.28 or newer)  You need to install python (install version 3 or greater) and then in a terminal run `pip install conan` or `sudo pip install conan`, and make sure that your python bin dir is in your path. After that the rest of it should be automatic.
    
#### Ubuntu 16.04 (not recommended) 
On Ubuntu Xenial 16.04, add a PPA that supports GCC-7 and configure to use GCC 7

```shell
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt update
sudo apt install gcc-7 g++-7
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 60 --slave /usr/bin/g++ g++ /usr/bin/g++-5
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 40 --slave /usr/bin/g++ g++ /usr/bin/g++-7
sudo update-alternatives --config gcc
g++ -v
# => 7.3.0
```

Update HarfBuzz

```shell
sudo apt remove libharfbuzz-dev
sudo apt-add-repository ppa:jonathonf/texlive
sudo apt update
sudo apt install libharfbuzz-dev
```

Install other dependencies

    sudo apt install libnssn3 libxtst6

----

Install CMake

```shell
CMAKE_VERSION=3.15.0 # Update with latest by checking https://cmake.org/download/
wget https://github.com/Kitware/CMake/releases/download/v$CMAKE_VERSION/cmake-$CMAKE_VERSION.tar.gz
tar xfz cmake-$CMAKE_VERSION.tar.gz
cd cmake-$CMAKE_VERSION
./configure
make -j$(nproc)
sudo make install
```
    
Not required but to install Ruby 2.5:

```shell
sudo apt-add-repository ppa:brightbox/ruby-ng
sudo apt-get update
sudo apt-get install ruby2.5 ruby2.5-dev
```

Other 16.04 dependencies: 
* [QtIFW](https://download.qt.io/official_releases/qt-installer-framework/3.2.2/QtInstallerFramework-linux-x64.run)
* [conan](https://conan.io/) (version 1.28 or newer)  You need to install python (install version 3 or greater) and then in a terminal run `pip install conan` or `sudo pip install conan`, and make sure that your python bin dir is in your path. After that the rest of it should be automatic.

Then you can clone the repository and run cmake setup. 

e.g. That builds all and create a binary .deb installer package

`$ git clone https://github.com/NREL/OpenStudio`  
`$ cd OpenStudio`  
`$ mkdir build`  
`$ cd build `  
`$ cmake -DBUILD_TESTING=ON -DBUILD_PACKAGE=ON -DCMAKE_BUILD_TYPE=Release -DCPACK_BINARY_DEB=ON -DCPACK_BINARY_IFW=OFF -DCPACK_BINARY_NSIS=OFF -DCPACK_BINARY_RPM=OFF -DCPACK_BINARY_STGZ=OFF -DCPACK_BINARY_TBZ2=OFF -DCPACK_BINARY_TGZ=ON -DCPACK_BINARY_TXZ=OFF -DCPACK_BINARY_TZ=OFF ../`  
`$ make -j <num_of_cores> package `  


## Mac
Install:
* [Xcode](https://developer.apple.com/support/xcode/)
* [CMake](https://cmake.org/download/).
* [QtIFW](https://download.qt.io/official_releases/qt-installer-framework/3.2.2/QtInstallerFramework-mac-x64.dmg)
* [conan](https://conan.io/) (version 1.28 or newer)  You need to install python (install version 3 or greater) and then in a terminal run `pip install conan` or `sudo pip install conan`, and make sure that your python bin dir is in your path. After that the rest of it should be automatic.

Clone the OpenStudio repository, launch the CMake GUI, and select the source and build directories.  Configure build options and press generate. After generating, change directories into the build directory and run `make -j$(nproc)`.

`$ git clone https://github.com/NREL/OpenStudio`  
`$ cd OpenStudio`  
`$ mkdir build`  
`$ cd build `  
`$ cmake -DCMAKE_OSX_DEPLOYMENT_TARGET=10.12 -DBUILD_TESTING=ON -DBUILD_PACKAGE=ON -DCMAKE_BUILD_TYPE=Release -DCPACK_BINARY_DEB=OFF -DCPACK_BINARY_IFW=ON -DCPACK_BINARY_NSIS=OFF -DCPACK_BINARY_RPM=OFF -DCPACK_BINARY_STGZ=OFF -DCPACK_BINARY_TBZ2=OFF -DCPACK_BINARY_TGZ=ON -DCPACK_BINARY_TXZ=OFF -DCPACK_BINARY_TZ=OFF ../`  
`$ make -j <num_of_cores> package`  



### Building C# Bindings (Windows only)
*NEEDS UPDATE:*
Ensure that you have the [.NET Framework 3.5 SP1](http://www.microsoft.com/en-us/download/details.aspx?id=22) installed


### Building Documentation

#### Windows
Install [Doxygen v1.8.8](http://ftp.stack.nl/pub/users/dimitri/doxygen-1.8.8-setup.exe)

(The searchengine feature in >=1.8.9 is broken)

Install [Graphviz v2.38](http://www.graphviz.org/pub/graphviz/stable/windows/graphviz-2.38.msi)

#### Mac
Download [Doxygen v1.8.8](http://ftp.stack.nl/pub/users/dimitri/Doxygen-1.8.8.dmg) and drag it to Applications

(The searchengine feature in >=1.8.9 is broken)

Install [Graphviz v2.36.0](http://www.graphviz.org/pub/graphviz/stable/macos/mountainlion/graphviz-2.36.0.pkg)


---

# OpenStudio Dependencies

Most developers do not have to worry about building OpenStudio dependencies, the correct dependencies will be downloaded as needed when building the [OpenStudio Core](#openstudio-core) project.  Only developers needing to update the version of OpenStudio dependencies or provide dependency builds for new platforms need to read this section.  After new OpenStudio dependencies are built they are uploaded to Amazon S3 and the `./CMakeLists.txt` file is updated to reference the new packages.

*Note that you will want to select `./meta` as the source directory.  Suggested build directory names are `build` or `super-build`, these names are ignored by git.*

Clone the repository, launch the CMake GUI, and select the source and build directories.  Configure build options and press generate.  Certain configuration options may require installation of additional dependencies, these are documented below.  After generating, open OpenStudioMeta.sln in the build directory with Visual Studio and choose Build Solution.  

### Build OpenStudio Gems

* Update the gems referenced in `\dependencies\ruby\Gemfile`.  
* Build the openstudio-gems target in the OpenStudioMeta.sln.
* Copy the openstudio-gems-YYYYMMDD.tar.gz file to S3:openstudio-resources/dependencies/. 
* Find the MD5 Hash (OSX: `md5 openstudio-gems-YYYYMMDD.tar.gz`, Windows: `md5sum openstudio-gems-YYYYMMDD.tar.gz`)
* Update filename and MD5 hash in openstudiocore/CMakeLists.txt

