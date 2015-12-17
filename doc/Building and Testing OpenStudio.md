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
- MarkdownPad 2

Initial Steps
=============
If internal to NREL, connect to developer VPN (avoids certificate warnings, increases speed of uploads).

With Git, merge `develop` into `iteration`.

Release Notes
=============
If this is a major release, release notes must be written.

- Contact each OpenStudio developer and get from them a list of features and bug fixes which they feel should be highlighted.
- In the folder doc\ReleaseNotes copy the most recent release notes, and update the name to reflect the correct version, and date.
- Incorporate the developers notes previously obtained.
- Have the release notes reviewed for accuracy.
- Generate a PDF from the word doc.
- Generate a markdown document from the word doc.


Update CHANGELOG.md
=================

### Set the desired stats date range
- Open GitHubIssuesStats.rb, and adjust date range

### Get the stats
Open Git Bash

In the Git Bash window, type the following

- cd to the root folder of your local Git OpenStudio repository
- cd developer/ruby
- gem install github_api
- ruby GitHubIssuesStats.rb

### Pipe the stats out to a text file
In the Git Bash window, type the following

- ruby GitHubIssuesStats.rb > out.txt

Open out.txt, and paste data into CHANGELOG.md

Commit CHANGELOG.md


Builds
======

Ubuntu
------
With Git, pull `iteration` branch.

In a command window:

    cd openstudio
    mkdir build
	cd build
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

	make –j8 (8 indicates the number of cores used)
	make package_deb

Copy .deb package from VM to Windows

Windows 64-bit
--------------
With Git, pull `iteration` branch.

In CMake, select current 64-bit compiler

In CMake check the following:

- BUILD\_CSHARP\_BINDINGS
- BUILD\_PACKAGE
- BUILD\_SWIG
- BUILD\_TESTING

Press `Configure` and `Generate` in CMake

In Visual Studio:

- Open OpenStudio.sln
- Select Release Solution Configuration
- Build OpenStudio until "configuring done", and "generating done"
- Cancel the build (tip: set up [Kill Build Shortcut](https://github.com/NREL/OpenStudio/wiki/Suggested-Visual-Studio-2013-Configuration#kill-build-shortcut))
- Open OpenStudioCore.sln
- Select Release Solution Configuration
- Build OpenStudioCore with IncrediBuild
- Open OpenStudio.sln
- Build PACKAGE

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
- Open OpenStudio.sln
- Build PACKAGE

Mac
---
With Git, pull `iteration` branch.

In a command window:

	cd openstudio
    mkdir build
    cd build
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

```bash
make package –j8
# When done:
⌘ + q (to quit a Mac app)
```

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

Press `Configure` and `Generate` in CMake

In Visual Studio:

- Open OpenStudio.sln
- Build OpenStudio until "configuring done", and "generating done"
- Cancel the build
- Open OpenStudioCore.sln
- **Without** IncrediBuild, build ALL\_DOXYGEN
- **Without** IncrediBuild, build ALL\_RDOC

In folder `build\OSCore-prefix\src\OSCore-build\doc`

- Extract zip OpenStudio-X.Y.Z-doc.zip to a similarly named directory

In S3:

- Drag extracted directory into S3 directory `openstudio-sdk-documentation/cpp`

For major releases, delete the content of `openstudio-sdk-documentation/cpp/latest` and drag in the contents of `build\OSCore-prefix\src\OSCore-build\doc\OpenStudio-X.Y.Z-doc`

In folder `build\OSCore-prefix\src\OSCore-build\ruby\rdocify`

- Extract zip OpenStudio-X.Y.Z-rdoc.zip to a similarly named folder

In S3:

- Drag extracted directory into S3 directory `openstudio-sdk-documentation/ruby`
- From `openstudio-sdk-documentation` bucket, download index.html and edit to new release numbers (23 changes)
- Test at https://openstudio-sdk-documentation.s3.amazonaws.com/index.html

OSVersion Testing
=================

In folder `build\OSCore-prefix\src\OSCore-build\Products\Release`

- Open cmd prompt
- Drag and drop `openstudio_osversion_tests.exe` onto the prompt, then run
- Make sure everything passes

Sanity Testing Release Builds
=============================

### Ubuntu
- On a clean Ubuntu VM, install the current version of OpenStudio
- Open OpenStudio, and make a model
- Open PAT, make a project, and select the model above as your baseline model

### Mac
- On a clean Mac VM, install the current version of SketchUp and OpenStudio
- Open SketchUp, and make and save a model
- Open OpenStudio, and open the model above
- Open PAT, make a project, and select the model above as your baseline model

### 32-bit Windows
- On a clean Windows VM, install the current 32-bit version of SketchUp and the current 32-bit version of OpenStudio
- Open SketchUp, and make and save a model
- Open OpenStudio, and open the model above
- Open PAT, make a project, and select the model above as your baseline model

### 64-bit Windows
- On a clean Windows VM, install the current 64-bit version of SketchUp and the current 64-bit version of OpenStudio
- Open SketchUp, and make and save a model
- Open OpenStudio, and open the model above
- Open PAT, make a project, and select the model above as your baseline model

### Tests to run
- Test running PAT on the cloud on at least one platform
- Test BCL downloads
- Test user scripts in SketchUp
- Test Apply Measures Now
- Run simulation in OpenStudio
- Run simulation in PAT
- Test opening results in ResultsViewer

OpenStudio News Update
======================

If this version is a major release

- At https://www.openstudio.net/ click "Add Announcement" and publish a release note.

Version Update
==============

- In `build\OSCore-prefix\src\OSCore-build\resources\osversion` copy directory X.Y.Z to `openstudiocore\resources\osversion` (new folder, 3 files)
- In the top level of your OpenStudio folder, update `CMakeLists.txt` version (1 line)
- In `openstudiocore\resources` update `CMakeLists.txt` version (3 lines, 1 location)
- In `openstudiocore\resources\model` update `OpenStudio.idd` version (1 line)
- In `openstudiocore\src\osversion` update `VersionTranslator.cpp` version

At https://github.com/NREL/OpenStudio/blob/develop/openstudiocore/src/osversion/VersionTranslator.cpp

- Select "History", see edits if needed (usually use `defaultUpdate`, 2 lines)

With Git, commit above files (Commit Message = `Updating version to X.Y.Z`)

AMI BUILD
=========
On first use, install the required gems using a command prompt, with Ruby in the PATH.

- In the top level of your OpenStudio-server folder, run

```ruby
gem install bundler
bundler install
```

With Git, pull OpenStudio-server master

- Update version in `server\lib\openstudio_server\version.rb`
- Update version in `CHANGELOG.md`
- Update version in `vagrant\chef\roles\openstudio.rb`

With Git, commit above files (Commit Message = `Bump version`)

In a command window:

- In the top level of your OpenStudio-server folder, run `rake release`

Compatibility Matrix
====================
On https://github.com/NREL/OpenStudio/wiki/OpenStudio-Version-Compatibility-Matrix

- Select `Edit` and add a new row of information
