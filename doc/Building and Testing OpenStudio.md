Process of Building and Testing OpenStudio for Release
======================================================
Configured build environments: https://github.com/NREL/OpenStudio/wiki/Configuring-OpenStudio-Build-Environments
- 64-bit Windows environment
- 64-bit Ubuntu environment
- 64-bit OS X environment
- Git
- VMWare
- IncrediBuild
- S3
- SketchUp

Initial Steps
=============
If internal to NREL, connect to developer VPN (avoids certificate warnings, increases speed of uploads).

With Git, merge `develop` into `iteration`.

Builds
======

Ubuntu
------
With Git, pull iteration branch.

In a command window:

	cd openstudio/build
	ccmake ..

In CMake check the following:

- BUILD\_PACKAGE
- BUILD\_SWIG
- CMAKE\_BUILD\_TYPE = Release
- CMAKE\_BINARY\_DEB

In CMake type the following:

	c
	g

In a command window:

	make –j8
	make package_deb

Copy .deb package from VM to Windows

Windows 64-bit
--------------
With Git, pull iteration branch.

In CMake, select current 64-bit compiler

In CMake check the following:

- BUILD\_CSHARP\_BINDINGS
- BUILD\_DOCUMENTATION
- BUILD\_PACKAGE
- BUILD\_SWIG
- BUILD\_TESTING

Press `Configure` and `Generate` in CMake

In Visual Studio:

- Open OpenStudio.sln
- Select Release Solution Configuration
- Build OpenStudio until "configuring done", and "generating done"
- Cancel the build
- Open OpenStudioCore.sln
- Select Release Solution Configuration
- Build OpenStudioCore with IncrediBuild

Windows 32-bit
--------------
With Git, pull iteration branch.

In CMake, select current 32-bit compiler

In CMake check the following:

- BUILD\_CSHARP\_BINDINGS
- BUILD\_PACKAGE
- BUILD\_SWIG

Press `Configure` and `Generate` in CMake

In Visual Studio:

- Open OpenStudio.sln
- Select Release Solution Configuration
- Build OpenStudio until "configuring done", and "generating done"
- Cancel the build
- Open OpenStudioCore.sln
- Select Release Solution Configuration
- Build OpenStudioCore with IncrediBuild

Mac
---
With Git, pull iteration branch.

In a command window:

	cd openstudio/build
	ccmake ..

In CMake check the following:

- BUILD\_PACKAGE

In CMake type the following:

	t

In CMake **uncheck** the following:

- CPACK\_BINARY\__FOO_ (uncheck all)

In CMake **check** the following:

- CPACK\_BINARY\_PACKAGEMAKER

In CMake type the following:

	c
	g

In a command window:

	make package –j8
	CTRL + c
	⌘ + q (to quit a Mac app)

Copy build to VM's share folder

GitHub Releases
===============
https://github.com/NREL/OpenStudio/releases

- Select "Draft a new release"
- Set tag version = vX.Y.Z
- Set Target = `iteration`
- Set Release title = OpenStudio vX.Y.Z
- Check "This is a pre-release" for an iteration build
- Select "Save draft"
- Publish release
- Attach binaries by drag and drop (tip: Save draft after each binary successfully added)
- Select "Publish release"

OpenStudio.net
==============
In S3:

- In `openstudio-builds` bucket, make directory for new builds (X.Y.Z)
- Drag and drop the builds into above bucket
- Right-click on the newly uploaded files, and `Generate Web URLs`

On [OpenStudio.net](https://www.openstudio.net/):

- Select `Edit OpenStudio Release Links`
- Update `Current Release Version` or `Develop Release Version` (depending on whether a major or iteration build is being done)
- Replace the S3 build URLs with those generated above

Documentation
=============
In CMake check the following:

- BUILD\_DOCUMENTATION

In Visual Studio:

- Open OpenStudio.sln
- Build OpenStudio until "configuring done", and "generating done"
- Cancel the build
- Open OpenStudioCore.sln
- **Without** IncrediBuild, build ALL\_DOXYGEN
- **Without** IncrediBuild, build ALL\_RDOC

In folder `C:\Git\OpenStudio_1\build\OSCore-prefix\src\OSCore-build\doc`

- Extract zip OpenStudio-X.Y.Z-doc.zip to a similarly named directory

In S3:

- Drag extracted directory into S3 directory `openstudio-sdk-documentation/cpp`

For major releases, delete the content of `openstudio-sdk-documentation/cpp/latest` and drag in the contents of `C:\Git\OpenStudio_1\build\OSCore-prefix\src\OSCore-build\doc\OpenStudio-X.Y.Z-doc`

In folder `C:\Git\OpenStudio_1\build\OSCore-prefix\src\OSCore-build\ruby\rdocify`

- Extract zip OpenStudio-X.Y.Z-rdoc.zip to a similarly named folder

In S3:

- Drag extracted directory into S3 directory `openstudio-sdk-documentation/ruby`
- From `openstudio-sdk-documentation` bucket, download index.html and edit to new release numbers (23 changes)
- Test at https://openstudio-sdk-documentation.s3.amazonaws.com/index.html

OSVersion Testing
=================

In folder `C:\Git\OpenStudio_1\build\OSCore-prefix\src\OSCore-build\Products\Release`

- Open cmd prompt
- Drag and drop `openstudio_osversion_tests.exe` onto the prompt, then run
- Make sure everything passes

Sanity Testing Release Builds
=============================

TBD

Version Update
==============

- Copy directory X.Y.Z to `C:\Git\OpenStudio_1\openstudiocore\resources\osversion` (new folder, 3 files)
- In `C:\Git\OpenStudio_1` update `CMakeLists.txt` version (1 line)
- In `C:\Git\OpenStudio_1\openstudiocore\resources` update `CMakeLists.txt` version (3 lines, 1 location)
- In `C:\Git\OpenStudio_1\openstudiocore\resources\model` update `OpenStudio.idd` version (1 line)
- In `C:\Git\OpenStudio_1\openstudiocore\src\osversion` update `VersionTranslator.cpp` version

At https://github.com/NREL/OpenStudio

	t
	VersionTranslator.cpp

- Select "History", see edits if needed (usually use `defaultUpdate`, 2 lines)

With Git, commit above files (Commit Message = `Updating version to X.Y.Z`)

AMI BUILD
=========
With Git, pull OpenStudio-server master

- Update version in `C:\Git\OpenStudio-server\CHANGELOG.md`
- Update version in `C:\Git\OpenStudio-server\vagrant\chef\roles\openstudio.rb`
- Update version in `C:\Git\OpenStudio-server\server\lib\openstudio_server\version.rb`

With Git, commit above files (Commit Message = `Bump version`)

In a command window:

- In `C:\Git\OpenStudio-server`, run `rake release`

Compatibility Matrix
====================
On https://github.com/NREL/OpenStudio/wiki/OpenStudio-Version-Compatibility-Matrix

- Select `Edit` and add a new row of information
