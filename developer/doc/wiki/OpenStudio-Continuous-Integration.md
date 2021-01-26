OpenStudio's internally-hosted GitLab CI uploads packages of OpenStudio, OpenStudio-server, OpenStudio-PAT, DView to S3 and are available at the following folders:

## Uploaded CI Packages

* OpenStudio: https://openstudio-builds.s3.amazonaws.com/_CI/OpenStudio/
* OpenStudio-server: https://openstudio-builds.s3.amazonaws.com/_CI/OpenStudio-server/
* DView: https://openstudio-builds.s3.amazonaws.com/_CI/DView/

## CI Package Naming Convention

Package naming convention is as follows:
* OpenStudio: NAME-MAJOR.MINOR.PATCH.SHA-OPERATING_SYSTEM.FILE_TYPE
* OpenStudio-server: NAME-SHA-OPERATING_SYSTEM.FILE_TYPE
* DView: NAME-OPERATING_SYSTEM.FILE_TYPE

Package naming examples are as follows:
* OpenStudio-2.5.1.b5c323884e-Darwin.zip
* OpenStudio-2.5.1.b5c323884e-Linux.deb
* OpenStudio-2.5.1.b5c323884e-Windows.exe
* OpenStudio-server-0034828655-darwin.tar.gz
* OpenStudio-server-0034828655-win32.tar.gz
* DView-linux.tar.gz
* DView-osx.tar.gz
* DView-windows.zip

## CI Behavior

The CI behavior is unique for each repository, and is defined by each repository's file titled ".gitlab-ci.yml". CI stages which are prone to failures have a "retry" setting which will trigger on any noted failure in that stage.

The GitLab CI repositories each mirror their respective, public-facing GitHub repository. GitHub / GitLab repository is accomplished via polling which occurs every 5 minutes.

## Pull Requests

OpenStudio CI automatically builds and tests OpenStudio pull requests[OpenStudio pull requests](http://https://github.com/NREL/OpenStudio/wiki/OpenStudio-Pull-Requests).