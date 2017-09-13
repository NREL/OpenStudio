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


Release Notes
=============
If this is a major release, release notes must be written.

- Contact each OpenStudio developer and get from them a list of features and bug fixes which they feel should be highlighted.
- In the folder doc\ReleaseNotes copy the most recent release notes, and update the name to reflect the correct version, and date.
- Incorporate the developers notes previously obtained.
- Have the release notes reviewed for accuracy.
- Generate a PDF from the word doc.
- Update openstudiocore\cmakelists.txt to package the latest release notes pdf.
- With Git, commit above files (Commit Message = `Updating release notes X.Y.Z`) to the develop branch
- Generate a markdown document from the word doc.


Updating HVAC lib
=================
If this is a major release, the HVAC library must be updated.

Run `developer/ruby/UpdateHVACLibrary.rb` (ruby -I E:\Git\OS1\build\Products\ruby\Release UpdateHVACLibrary.rb)

Commit the updated files to develop.


Updating EnergyPlus
===================

If a new version of EnergyPlus is to be incorporated into OpenStudio
- Upload the 3 versions (Windows 64, Ubuntu, and Linux) to S3's folder `openstudio-resources/dependencies`
- In the ./openstudiocore folder, update `CMakeLists.txt` following
	- ENERGYPLUS_VERSION_MAJOR
	- ENERGYPLUS_VERSION_MINOR
	- ENERGYPLUS_VERSION_PATCH
	- ENERGYPLUS_BUILD_SHA
	- ENERGYPLUS_EXPECTED_HASH (In 3 places: Win 32, Win 64, Darwin, and Linux)

Note: use HashTab, or similar, to determine the MD5 hash value for each file referenced above.


Updating OpenStudio Server used by PAT
======================================

If there is a new server build, bump the server SHA (both for Mac and Windows) in manifest.json in the root folder of PAT.


Updating PAT
==========================

Bump the PAT SHA in OpenStudio's openstudiocore/pat/CMakeLists.txt.


Initial Steps
=============
If internal to NREL, connect to developer VPN (avoids certificate warnings, increases speed of uploads).

- With Git, merge `develop` into `iteration`.

If this is a major release

- With Git, merge `develop` into `master`.


DView Builds
======

Ubuntu
------
With Git, pull `iteration` branch.

Ensure the correct version of wxWidgets is installed (currently 3.1.0, us "apt-get install libwxgtk3.0-dev")

In a command window:

    cd openstudio/buildMeta
	ccmake ../

In CMake check the following:

- BUILD\_DVIEW
- CMAKE\_BUILD\_TYPE = Release

In CMake type the following:

	c
	g

In a command window:

	make DView

Compress folder DView-install with DView, and upload to S3.

Windows 64-bit
--------------
With Git, pull `iteration` branch.

Ensure the correct version of wxWidgets is installed (currently 3.1.0)

In CMake, select current 64-bit compiler

Point CMake to the top folder for source, and buildMeta folder for binaries

In CMake check the following:

- BUILD\_DVIEW 

Press `Configure` and `Generate` in CMake

In Visual Studio:

- Open OpenStudioMeta.sln
- Select **Release** Solution Configuration

Compress folder DView-install with DView, and upload to S3.

Mac
---
With Git, pull `iteration` branch.

Ensure the correct version of wxWidgets is installed (currently 3.1.0)

In a command window:

	cd openstudio/buildMeta
	ccmake ../

In CMake check the following:

- BUILD\_DVIEW
- CMAKE\_BUILD\_TYPE = Release

In CMake type the following:

	c
	g

In a command window:
```bash
make DView –j16
# When done:
⌘ + q (to quit a Mac app)
```

Locate the folder DView-install, right click and use Mac's compression algorithm. Upload to S3.

Update all DView MD5 values in openstudiocore/cmakelists.txt and upload to OpenStusio's GitHub repository.

OpenStudio 2 Builds
======

Note: If the desire is to build OpenStudio 1.x, refer to the earlier version of this document dated 12/9/2016.

Ubuntu
------
With Git, pull `iteration` branch.

In a command window:

    cd openstudio/build
	ccmake ../openstudiocore

In CMake check the following:

- BUILD\_DVIEW
- BUILD\_OS\_APP
- BUILD\_PACKAGE
- CMAKE\_BUILD\_TYPE = Release

In CMake **uncheck** the following:

- CPACK_BINARY_FOO (uncheck all)
- CPACK_SOURCE_FOO (uncheck all)

In CMake **check** the following:

- CPACK\_BINARY\_DEB

In CMake type the following:

	c
	g

In a command window:

	make –j16 (16 indicates the number of cores used, the max number allowed by VMware 12)
	make package

Copy .deb package from VM to Windows

Windows 64-bit
--------------
With Git, pull `iteration` branch.

In CMake, select current 64-bit compiler

Point CMake to the openstudiocore folder for source, and build folder for binaries

In CMake check the following:

- BUILD\_CSHARP\_BINDINGS
- BUILD\_DOCUMENTATION
- BUILD\_DVIEW 
- BUILD\_OS\_APP
- BUILD\_PACKAGE
- BUILD\_PAT
- BUILD\_TESTING

In CMake **uncheck** the following:

- CPACK_BINARY_FOO (uncheck all)
- CPACK_SOURCE_FOO (uncheck all)


Press `Configure` and `Generate` in CMake

In Visual Studio:

- Open OpenStudio.sln
- Select **Release** Solution Configuration
- Build ALL_BUILD with IncrediBuild
- Build PACKAGE

Mac
---
With Git, pull `iteration` branch.

In a command window:

	cd openstudio/build
	ccmake ../openstudiocore

In CMake check the following:

- BUILD\_DVIEW
- BUILD\_OS\_APP
- BUILD\_PACKAGE
- BUILD\_PAT
- CMAKE\_BUILD\_TYPE = Release

In CMake **uncheck** the following:

- CPACK_BINARY_FOO (uncheck all)
- CPACK_SOURCE_FOO (uncheck all)

In CMake **check** the following:

- CPACK\_BINARY\_IFW


In CMake type the following:

	c
	g

In a command window:

```bash
make package –j16
# When done:
⌘ + q (to quit a Mac app)
```

Locate the package, right click and use Mac's compression algorithm.

Copy build to VM's share folder

**Note:** The package will appear to be an empty folder in the build directory, when in fact the package is in OpenStudio/build/_CPack_Packages/Darwin/IFW and it's file name will have no file type extension.

**Note:** Making a build while on NREL's network (even the Dev VPN) and using a VMWare Mac VM will often fail when cmake attempts to download from GitHub the PAT SHA-specific zip file, meaning the developer will need to do this step manually. Example: https://github.com/NREL/OpenStudio-PAT/archive/f942affb1897678c4dc17a137fcf338e00da2cfb.zip. The contents off the above zip file's parent directory will need to be deleted prior to attempting again to build the package.

OSVersion Testing
=================

In folder `build\Products\Release`

- Open cmd prompt
- Drag and drop `openstudio_osversion_tests.exe` onto the prompt, then run
- Make sure everything passes

Sanity Testing Release Builds
=============================

### Ubuntu
- On a clean Ubuntu VM, install the current version of OpenStudio
- Locate OpenStudioApp with "which OpenStudioApp", and navigate to its folder
- ./OpenStudioApp, and make a model

### Mac
- On a clean Mac VM, install the current version of SketchUp and OpenStudio
- Open SketchUp, and make and save a model.  Ensure that plug-in loads with Extensions Policy = "Identified Extensions Only".
- Open OpenStudio, and open the model above
- Open PAT, make a project, and select the model above as your baseline model

### 64-bit Windows
- On a clean Windows VM, install the current 64-bit version of SketchUp and the current 64-bit version of OpenStudio
- Open SketchUp, and make and save a model.  Ensure that plug-in loads with Extensions Policy = "Identified Extensions Only".
- Open OpenStudio, and open the model above
- Open PAT, make a project, and select the model above as your baseline model

### Tests to run
- Test prior version of OpenStudio model
- Test prior version of PAT project
- Use SketchUp to make a model on Windows 64 bit, and Mac
- Test running PAT on the cloud on at least one platform
- Test BCL downloads
- Test user scripts in SketchUp
- Test Apply Measures Now
- Run simulation in OpenStudio
- Run simulation in PAT
- Test opening results in ResultsViewer


GitHub Releases
===============
https://github.com/NREL/OpenStudio/releases

- Select "Draft a new release"
- Set tag version = vX.Y.Z
- If this is a major release
	- Set Target = `master`
- Else If this is an iteration build
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


Verification of Posted Software
===============================

Download all posted software from both GitHub and S3. Verify that each downloaded files' MD5 hash matches their respective MD5 hash of the original, tested files from the build machines.


Documentation
=============

In Visual Studio:

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


OpenStudio News Update
======================

If this is a major release, a news update must be written

- At https://www.openstudio.net/ click "Add Announcement" and publish a release note.

OpenStudio-Web Update
=====================

If this is a major release, OpenStudio-web must be updated

In the docroot folder\

- open update.html and update its build version, sha, and date (2 places)


Final Steps
=============
- With Git, switch to `develop`.


Update CHANGELOG.md
=================

### Set the desired stats date range
In folder `developer\ruby`
- Open GitHubIssueStats.rb, and adjust date range

### Get the stats
In folder `developer\ruby`, open Git Bash and type the following

	gem install github_api
	ruby GitHubIssueStats.rb
	ruby GitHubIssueStats.rb > out.txt

Open out.txt, and paste data into CHANGELOG.md

Commit GitHubIssueStats.rb and CHANGELOG.md to the develop branch


Version Update
==============

The current version (X.Y.Z) being built, and the updated version (X.Y.Z+1) for the upcoming iteration need to be correctly incorporated into their respective documents.

- In the top level of your OpenStudio folder, update `CMakeLists.txt` version to X.Y.Z+1 (3 lines), e.g. `set(CMAKE_VERSION_MAJOR 2)`, `set(CMAKE_VERSION_MINOR 0)`, `set(CMAKE_VERSION_PATCH 1)`
- In the openstudiocore level of your OpenStudio folder, update `CMakeLists.txt` version to X.Y.Z+1 (1 line), e.g. `project(OpenStudio VERSION 2.0.1)`
- Copy file `openstudiocore\resources\model\OpenStudio.idd` to `openstudiocore\src\utilities\idd\versions\x_Y_Z` (new folder)
- In `openstudiocore\resources\model` update `OpenStudio.idd` version to X.Y.Z+1 (1 line)
- In `openstudiocore\src\osversion` update `VersionTranslator.cpp` version to X.Y.Z+1 in first location, and X.Y.Z in second location

At https://github.com/NREL/OpenStudio/blob/develop/openstudiocore/src/osversion/VersionTranslator.cpp

- Select "History", see edits if needed (usually use `defaultUpdate` in first location, 1 line for each of 2 locations)

With Git, commit above files (Commit Message = `Update version to X.Y.Z+1`) to the develop branch

SketchUp Extension Signature
=========
Check if either file below has been updated:
- `openstudiocore/ruby/openstudio/sketchup_plugin/OpenStudio.rb` (Note: not valid in OS2.x)
- `openstudiocore/sketchup_plugin/plugin/OpenStudio/Startup.rb` 

If either file was updated, the SketchUp Extension Signature must be updated in
- `openstudiocore/sketchup_plugin/plugin/OpenStudio/OpenStudio.hash`
 
To do this: (Note: not valid in OS2.x)
- Build an OpenStudio package
- Zip the contents (OpenStudio.rb and OpenStudio folder) in `build\_CPack_Packages\win64\NSIS\OpenStudio-x.y.z.sha-Win64\Ruby\Plugins`
- Change the extension of the zip file from .zip to .rbz
- Login to https://extensions.sketchup.com/en/developer_center/extension_signature as openstudio@nrel.gov
- Drag the .rbz file into the upload box, upload the file, click 'Sign The Extension' (do not select any options to encrypt the ruby files)
- Download the signed .rbz file, and extract the files over the originals in `build\_CPack_Packages\win64\NSIS\OpenStudio-x.y.z.sha-Win64\Ruby\Plugins`
- Copy the new OpenStudio.hash file over the existing one in `openstudiocore/ruby/openstudio/sketchup_plugin`, and commit the new OpenStudio.hash file
- Rebuild the OpenStudio package

AMI BUILD
=========
On first use, install the required gems using a command prompt, with Ruby in the PATH.

- In the top level of your OpenStudio-server folder, run

```ruby
gem install bundler
bundler install
```

With Git, make feature branch of OpenStudio-server develop, and name it "OS.x.y.z.sha"

- In `server\lib\openstudio_server\version.rb`, update VERSION_EXT to `-OS.x.y.z.sha` (ex: `-OS.1.12.2.462ae9e746`).
- In `CHANGELOG.md`, add new server and openstudio versions (ex: `Version 1.19.1-OS.1.12.2.462ae9e746`, `OpenStudio 1.12.2.462ae9e746`).
- In `vagrant\chef\roles\openstudio.rb`, update version and version_revision (ex: `1.12.2`, `462ae9e746`) 

With Git, commit above files to new branch (Commit Message = `Bump OpenStudio version to OS.x.y.z.sha`)
With Git, submit pull request

In a command window:

- In the top level of your OpenStudio-server folder, run `rake release`

Compatibility Matrix
====================
On https://github.com/NREL/OpenStudio/wiki/OpenStudio-Version-Compatibility-Matrix

- Select `Edit` and add a new row of information


Docker Image
============

In the top level of your docker-openstudio folder, modify `Dockerfile`

- Update OPENSTUDIO_VERSION with current version (X.Y.Z)
- Update OPENSTUDIO_SHA with current SHA
- (optional) test that the new Dockerfile works by running `docker build -t test-openstudio .` after starting the docker-machine to make sure that it completes successfully

With Git, commit Dockerfile (Commit Message = Bump OpenStudio version to OS.x.y.z.sha)
 
Verify master branch built successfully at https://hub.docker.com/r/nrel/openstudio/builds/

In a command window:
- In the top level of your docker-openstudio folder, run `git tag -a X.Y.Z` and annotate with 'X.Y.Z Release'
