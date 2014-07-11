Continuous Integration is facilitated through the use of the [decent_ci](https://github.com/lefticus/decent_ci) system.

The files `.decent_ci.yaml`, `.decent_ci-Linux.yaml`, `.decent_ci-Windows.yaml` and `.decent_ci-MacOS.yaml` in the root
folder configure the build system. 

For building OpenStudio, you need to set up the requirements for decent_ci, and follow its instructions here:

https://github.com/lefticus/decent_ci/blob/master/README.md

Additionally, OpenStudio requires that the following packages be installed

## MacOS

 * Official build of Qt. http://qt-project.org/downloads
 * PackageMaker - Almost impossible to find, have fun

## Windows

 * Official build of Qt. Both 32bit and 64bit versions: http://qt-project.org/downloads
 * Visual Studio 2013
 * nsis

## Linux 

 * Official builds of Qt, openssl and Boost: `sudo apt-get install libboost-all-dev qtbase5-dev libssl-dev libqt5webkit5-dev`
