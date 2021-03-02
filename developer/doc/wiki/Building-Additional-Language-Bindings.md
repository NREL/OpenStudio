> [Wiki](Home) ▸ **Building Additional Language Bindings**

# Java Bindings

Have a proper [build environment](https://github.com/NREL/OpenStudio/wiki/Configuring-OpenStudio-Build-Environments) set up for normal OpenStudio builds for the architecture and platform you are interested in.


## Install Java JRE SDK

### Windows

Install the [appropriate JDK](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html) (32bit / 64bit) 

When configuring OpenStudio with CMake, be sure to choose the appropriate compiler "Visual Studio 12 2013 (Win64)" for 64bit, "Visual Studio 12 2013" for 32bit.

You may install both 32bit and 64bit JDKs, the build system will automatically find the appropriate one.

### Linux

sudo apt-get install openjdk-7-jdk

### MacOS

Install the [appropriate JDK](http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html)

## Configure

 * Enable `BUILD_JAVA_BINDINGS` in CMake
 * Enable `BUILD_SWIG` to make sure you get the most appropriate version of SWIG for the bindings you are building
 * Make sure `BUILD_PACKAGE` is enabled if you want an installer package to be produced.
 * Consider enabling `MAXIMIZE_CPU_USAGE` to fully utilize the CPU (and use more memory).


## Build

Build as normal, the bindings will be in a `Products/Java` subfolder.

## Testing

```sh
cd OSCore-prefix/src/OSCore-build
ctest -R Java
```


## More Information

https://github.com/NREL/OpenStudio/tree/develop/openstudiocore/java

--------------------------------------------------------------

# C# / .NET Bindings

Have a proper [build environment](https://github.com/NREL/OpenStudio/wiki/Configuring-OpenStudio-Build-Environments) set up for normal OpenStudio builds for the architecture and platform you are interested in.

*Only windows is supported, the C++ half of the bindings will build on all platforms, but the C# half will only build on Windows right now*

## Configure

 * Enable `BUILD_CSHARP_BINDINGS` in CMake
 * Enable `BUILD_SWIG` to make sure you get the most appropriate version of SWIG for the bindings you are building
 * Make sure `BUILD_PACKAGE` is enabled if you want an installer package to be produced.
 * Consider enabling `MAXIMIZE_CPU_USAGE` to fully utilize the CPU (and use more memory).


## Build

Build as normal, the bindings will be in a `Products/CSharp` subfolder.

## More Information

https://github.com/NREL/OpenStudio/tree/develop/openstudiocore/csharp


--------------------------------------------------------------

# JavaScript

JavaScript is supported via V8. You can choose to either build for V8 directly or as a Node.js module (probably the most likely option).

Have a proper [build environment](https://github.com/NREL/OpenStudio/wiki/Configuring-OpenStudio-Build-Environments) set up for normal OpenStudio builds for the architecture and platform you are interested in.

*Only MacOS and Linux are supported.*

## Configure

 * Enable `BUILD_V8_BINDINGS` in CMake
     * This will automatically enable `BUILD_NODE` and `BUILD_NODE_MODULES` which builds a local, appropriate, version of Node.js and modules which can run against it.
     * This is the only fully supported configuration
 * Enable `BUILD_SWIG` to make sure you get the most appropriate version of SWIG for the bindings you are building
 * Make sure `BUILD_PACKAGE` is enabled if you want an installer package to be produced.
 * Consider enabling `MAXIMIZE_CPU_USAGE` to fully utilize the CPU (and use more memory).


## Build

Build as normal, the bindings will be in a `Products/V8` subfolder.

## Testing

```sh
cd OSCore-prefix/src/OSCore-build
ctest -R V8
```

## More Information

https://github.com/NREL/OpenStudio/tree/develop/openstudiocore/v8-node


-------------------------------------------------------------

# Python

Have a proper [build environment](https://github.com/NREL/OpenStudio/wiki/Configuring-OpenStudio-Build-Environments) set up for normal OpenStudio builds for the architecture and platform you are interested in.

**Packaging is untested on all platforms**

## Install Python

### Linux

```sh
sudo apt-get install python2.7-dev
```

### Windows 

https://www.python.org/downloads/ Allow the installer to add python to the path. *Only Python 2.7 is tested currently. Only 32bit is tested currently. Only Release is tested.*

To use the Python bindings from your build directory (e.g. C:\openstudio-python\build) copy all the OpenStudio dlls (along with dependencies) to C:\openstudio-python\build\OSCore-prefix\src\OSCore-build\Products\python\Release then set the following environment variable:

```sh
set PYTHONPATH = C:\openstudio-python\build\OSCore-prefix\src\OSCore-build\Products\python;C:\openstudio-python\build\OSCore-prefix\src\OSCore-build\Products\python\Release;C:\openstudio-python\openstudiocore\python;
```

### MacOS

Python should already be installed.

## Configure

 * Enable `BUILD_PYTHON_BINDINGS` in CMake
 * Enable `BUILD_SWIG` to make sure you get the most appropriate version of SWIG for the bindings you are building
 * Consider enabling `MAXIMIZE_CPU_USAGE` to fully utilize the CPU (and use more memory).

## Build

Build as normal, the bindings will be in a `Products/Python` subfolder

```sh
cd OSCore-prefix/src/OSCore-build
ctest -R Python
```

## More Information

https://github.com/NREL/OpenStudio/tree/develop/openstudiocore/python
