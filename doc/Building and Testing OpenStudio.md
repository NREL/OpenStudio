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
- With Git, commit above files (Commit Message = `Updating release notes X.Y.Z`) to the develop branch
- Generate a markdown document from the word doc.


Initial Steps
=============
If internal to NREL, connect to developer VPN (avoids certificate warnings, increases speed of uploads).

- With Git, merge `develop` into `iteration`.

If this is a major release

- With Git, merge `develop` into `master`.


Updating EnergyPlus
===================

If a new version of EnergyPlus is to be incorporated into OpenStudio
- Upload the 4 versions (Windows 64, Windows 32, Ubuntu, and Linux) to S3's folder `openstudio-resources/dependencies`
- In the top level of your OpenStudio folder, update `CMakeLists.txt` following
	- ENERGYPLUS_VERSION_MAJOR
	- ENERGYPLUS_VERSION_MINOR
	- ENERGYPLUS_VERSION_PATCH
	- ENERGYPLUS_BUILD_SHA
	- ENERGYPLUS_EXPECTED_HASH (In 4, perhaps 5 places: Win 32, Win 64, Darwin, Linux, and maybe Redhat)

Note: use HashTab, or similar, to determine the MD5 hash value for each file referenced above.


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
- CPACK\_BINARY\_DEB

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

OpenStudio 2 Windows 64-bit
--------------
With Git, pull `os_2_0_develop` branch.

In CMake, select current 64-bit compiler

In CMake check the following:

- BUILD\_PACKAGE
- BUILD\_SWIG

Press `Configure` and `Generate` in CMake

Check advanced, check grouped

In CMake **uncheck** the following:

- CPACK_BINARY_FOO (uncheck all)
- CPACK_SOURCE_FOO (uncheck all)

In CMake **check** the following:

- CPACK_BINARY_ZIP

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


In CMake **enter** the following for CMAKE\_BUILD\_TYPE:

- `Release`


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
- Open SketchUp, and make and save a model.  Ensure that plug-in loads with Extensions Policy = "Identified Extensions Only".
- Open OpenStudio, and open the model above
- Open PAT, make a project, and select the model above as your baseline model

### 32-bit Windows
- On a clean Windows VM, install the current 32-bit version of SketchUp and the current 32-bit version of OpenStudio
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
- Use SketchUp to make a model on Windows 32 bit, Windows 64 bit, and Mac
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

- In `build\OSCore-prefix\src\OSCore-build\resources\osversion` copy directory X.Y.Z to `openstudiocore\resources\osversion` (new folder, 3 files)
- In the top level of your OpenStudio folder, update `CMakeLists.txt` version to X.Y.Z+1 (1 line)
- In `openstudiocore\resources` update `CMakeLists.txt` version to X.Y.Z (3 lines, 1 location)
- In `openstudiocore\resources\model` update `OpenStudio.idd` version to X.Y.Z+1 (1 line)
- In `openstudiocore\src\osversion` update `VersionTranslator.cpp` version to X.Y.Z+1 in first location, and X.Y.Z in second location

At https://github.com/NREL/OpenStudio/blob/develop/openstudiocore/src/osversion/VersionTranslator.cpp

- Select "History", see edits if needed (usually use `defaultUpdate` in first location, 1 line for each of 2 locations)

With Git, commit above files (Commit Message = `Updating version to X.Y.Z+1`) to the develop branch

SketchUp Extension Signature
=========
If either of the files '/openstudiocore/ruby/openstudio/sketchup_plugin/OpenStudio.rb' or '/openstudiocore/ruby/openstudio/sketchup_plugin/Startup.rb' change, then the SketchUp Extension Signature in '/openstudiocore/ruby/openstudio/sketchup_plugin/OpenStudio.hash' must be updated.  To do this:

- Build an OpenStudio package
- Copy all the files in '\Ruby\Plugins' to another directory (you should see OpenStudio.rb and an OpenStudio folder)
- Delete the 'OpenStudio.hash' file in the copied directory
- Zip the OpenStudio.rb and OpenStudio folders (with OpenStudio.rb being at the top level of the zip archive)
- Change the extension of the zip file from .zip to .rbz
- Login to https://extensions.sketchup.com/en/developer_center/extension_signature as openstudio@nrel.gov
- Drag the .rbz file into the upload box, upload the file, push the 'Sign The Extension' button (do not select any options to encrypt the ruby files)
- Download the signed .rbz file, extract all the files
- Copy the new OpenStudio.hash file over the existing one in the source tree, commit the new OpenStudio.hash file
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

- Update version in `server\lib\openstudio_server\version.rb`: index the minor version for an OpenStudio quarterly release, index the dot version for an OpenStudio iteration release.
- Update version_ext to "-OS.x.y.z.sha"
- Update version in `CHANGELOG.md` to be version + version_ext above, update OpenStudio to include its SHA
- Update version and version_revision with the openstudio SHA in `vagrant\chef\roles\openstudio.rb`

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

With Git, commit Dockerfile (Commit Message = Bump OpenStudio version to OS-x.y.z.sha)
 
Verify master branch built successfully at https://hub.docker.com/r/nrel/openstudio/builds/

In a command window:
- In the top level of your docker-openstudio folder, run `git tag -a X.Y.Z` and annotate with 'X.Y.Z Release'
