> [Wiki](Home) ▸ **Configuring OpenStudio Build Environments**

Build instructions have changed as of OpenStudio 2.0.0. For instructions on building the 1.14.X Long Term Support branch please see [[(v1.14.X-LTS)-Configuring-OpenStudio-Build-Environments]].  The new build system is much easier to get started with.  To build OpenStudio, most developers need only follow the [OpenStudio Core](#openstudio-core) instructions, this process will download all necessary dependencies.  Only developers needing to build OpenStudio dependencies need worry about the advanced [OpenStudio Dependencies](#openstudio-dependencies) instructions.

## Contents
**Recommended**: [Building OpenStudio Core](#openstudio-core)
- [Windows](#windows)
- [Linux](#linux)
- [Mac](#mac)

**Advanced**: [Building OpenStudio Dependencies](#openstudio-dependencies)
- [Build Ruby](#build-ruby)
- [Build OpenStudio Gems](#build-openstudio-gems)
- [Build Boost](#build-boost)
- [Build Qt](#build-qt)

# OpenStudio Core
This is the fastest, most reliable method of getting a working OpenStudio build.  These instructions assume that you have successfully [cloned the OpenStudio repository](Using-OpenStudio-with-Git-and-GitHub#cloning-the-repository-to-your-local-computer) already. For these instructions we will refer to the root of the OpenStudio checkout as `.`.

*Note that you will want to select `.\openstudiocore` as the source directory in CMake and not the root of the OpenStudio checkout as in previous versions of OpenStudio.  Suggested build directory names are `.\build` or `.\core-build`, these names are ignored by git.*

### Windows
Install Visual Studio 2013 Professional (paid) or [Community](https://www.visualstudio.com/en-us/news/releasenotes/vs2013-community-vs) (free for open source development), [Visual Studio 2013 Update 5](https://www.visualstudio.com/vs/older-downloads/), [CMake 3.7 or greater](https://cmake.org/download/), and [msysGit](http://msysgit.github.io/).  Look at the [Suggested Visual Studio 2013 Configuration](https://github.com/NREL/OpenStudio/wiki/Suggested-Visual-Studio-2013-Configuration) to configure Visual Studio. Set the `PreferredToolArchitecture` environment variable to `x64`, this will use 64 bit compiler tools and performs much better (you may still create 32 bit builds with the 64 bit compiler tools). _Note: Visual Studio 2013 Express does not have the 64-bit compiler tools and will not work._

Clone the OpenStudio repository, launch the CMake GUI, and select the source and build directories.  Configure build options and press generate.  Certain configuration options may require installation of additional dependencies, these are documented below.  After generating, open OpenStudio.sln in the build directory with Visual Studio and choose Build Solution.  

#### Building Documentation
Install [Doxygen v1.8.8](http://ftp.stack.nl/pub/users/dimitri/doxygen-1.8.8-setup.exe)

(The searchengine feature in >=1.8.9 is broken)

Install [Graphviz v2.38](http://www.graphviz.org/pub/graphviz/stable/windows/graphviz-2.38.msi)

#### Building C# Bindings
Ensure that you have the [.NET Framework 3.5 SP1](http://www.microsoft.com/en-us/download/details.aspx?id=22) installed

#### Building Installer Packages
Install [QtIFW](https://download.qt.io/official_releases/qt-installer-framework/2.0.3/QtInstallerFramework-win-x86.exe)

#### Building PAT
Install [Node](https://nodejs.org/dist/v6.9.2/win-x64/node.exe)

### Linux

**The official supported Distribution is Ubuntu Xenial (16.04 LTS)**. Build support for OpenStudio 2.x on Ubuntu Bionioc (18.04) is not provided (though it is possible to *install* OpenStudio 2.x on Bionic). Building with Ubuntu Trusty 14.04 will also work.

    sudo apt-get install dpkg-dev git cmake-curses-gui cmake-gui libssl-dev libxt-dev libncurses5-dev libgl1-mesa-dev autoconf libexpat1-dev libpng12-dev libfreetype6-dev libdbus-glib-1-dev libglib2.0-dev libfontconfig1-dev libxi-dev libxrender-dev libgeographic-dev chrpath byacc libicu-dev libgmp3-dev curl gpgv2 ccache

Install Ruby 2.2.4:

    curl -sSL https://get.rvm.io -o rvm.sh
    source ./rvm.sh
    rvm install 2.2.4
    /bin/bash --login
    rvm use 2.2.4
    gem install bundler -v "1.17.3"

Add `source ~/.rvm/scripts/rvm` to .bashrc

Install cmake from source:

    wget https://cmake.org/files/v3.7/cmake-3.7.1.tar.gz
    tar -xzf cmake-3.7.1.tar.gz
    cd cmake-3.7.1/
    ./configure
    make 
    sudo make install

Clone the OpenStudio repository, create a build directory, and run ccmake:

    git clone git@github.com:NREL/OpenStudio.git
    cd OpenStudio
    mkdir build
    cd build
    ccmake ../openstudiocore

Configure build options and generate.  Certain configuration options may require installation of additional dependencies, these are documented below.  After generating, change directories into the build directory and run `make`.  

#### Building Installer Packages
Install [QtIFW](https://download.qt.io/official_releases/qt-installer-framework/2.0.3/QtInstallerFramework-linux-x64.run)

### Mac
Install Xcode and [CMake](http://www.cmake.org/files/v3.0/cmake-3.0.1-Darwin64-universal.dmg).

Install openssl. On Mac OS X 10.11 and above the development headers are no longer included. 
```brew install openssl```.


Clone the OpenStudio repository, launch the CMake GUI, and select the source and build directories.  Configure build options and press generate. On Mac OS X 10.11 and above manually set the *advanced* cmake configuration variables related to openssl, including OPENSSL_CRYPTO_LIBRARY, OPENSSL_INCLUDE_DIR, and OPENSSL_SSL_LIBRARY.  Certain configuration options may require installation of additional dependencies, these are documented below.   After generating, change directories into the build directory and run `make`.

#### Building Documentation
Download [Doxygen v1.8.8](http://ftp.stack.nl/pub/users/dimitri/Doxygen-1.8.8.dmg) and drag it to Applications

(The searchengine feature in >=1.8.9 is broken)

Install [Graphviz v2.36.0](http://www.graphviz.org/pub/graphviz/stable/macos/mountainlion/graphviz-2.36.0.pkg)

#### Building Installer Packages
Install [QtIFW](https://download.qt.io/official_releases/qt-installer-framework/2.0.3/QtInstallerFramework-mac-x64.dmg)

#### Building PAT
Install [Node](https://nodejs.org/dist/v6.9.2/node-v6.9.2.pkg)

---

# OpenStudio Dependencies

Most developers do not have to worry about building OpenStudio dependencies, the correct dependencies will be downloaded as needed when building the [OpenStudio Core](#openstudio-core) project.  Only developers needing to update the version of OpenStudio dependencies or provide dependency builds for new platforms need to read this section.  After new OpenStudio dependencies are built they are uploaded to Amazon S3 and the `.\openstudiocore\CMakeLists.txt` file is updated to reference the new packages.

*Note that you will want to select `\openstudio` as the source directory.  Suggested build directory names are `build` or `super-build`, these names are ignored by git.*

Clone the repository, launch the CMake GUI, and select the source and build directories.  Configure build options and press generate.  Certain configuration options may require installation of additional dependencies, these are documented below.  After generating, open OpenStudioMeta.sln in the build directory with Visual Studio and choose Build Solution.  

### Build Ruby
Build the Ruby target in the OpenStudioMeta.sln.  Package the Ruby-install directory.

### Build OpenStudio Gems

* Update the gems referenced in `\dependencies\ruby\Gemfile`.  
* Build the openstudio-gems target in the OpenStudioMeta.sln.
* Copy the openstudio-gems-YYYYMMDD.tar.gz file to S3:openstudio-resources/dependencies/. 
* Find the MD5 Hash (OSX: `md5 openstudio-gems-YYYYMMDD.tar.gz`, Windows: `md5sum openstudio-gems-YYYYMMDD.tar.gz`)
* Update filename and MD5 hash in openstudiocore/CMakeLists.txt

### Build Boost
Build the Boost target in the OpenStudioMeta.sln. Package the Boost directory.

### Build Qt
Configure whether to build static or shared Qt binaries using the Qt5Static variable.  Build the Qt target in the OpenStudioMeta.sln. Package the Qt directory.
