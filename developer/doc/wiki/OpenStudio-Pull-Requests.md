## Pull Request Nutshell Rules

* Keep up-to-date with develop
* Build testing and pass all CTests
* Build package and pass sanity tests

To submit a PR, you must first ensure that your feature branch is up-to-date with develop, has no conflicts with develop, introduces no test failures, and that a package can be successfully built, installed, and run.

## Merge in develop

Immediately prior to submitting a PR, merge develop into your feature branch and ensure your branch has no conflicts and that it is not behind develop.

## Package Dependencies

Ubuntu packages have the fewest dependency requirements, followed by Mac packages, and finally Windows packages which have the most requirements.

Common across all operating systems is the requirement that you build testing, and then run and pass all tests prior to submitting a PR.

Mac adds the requirement of building PAT.

Windows also adds the requirement of building PAT, as well as the requirement of being able to build C# bindings (this is a common area for build failures and needs to be verified prior to submitting a PR).

When building Ubuntu packages, you need to set the build flags as follows:
* BUILD_DVIEW = ON
* BUILD_OS_APP = ON
* BUILD_PACKAGE = ON
* BUILD_TESTING = ON
* CMAKE_BUILD_TYPE = Release
* CPACK_BINARY_DEB = ON

When building Mac packages, you need to set the build flags as follows:
* BUILD_DVIEW = ON
* BUILD_OS_APP = ON
* BUILD_PACKAGE = ON
* BUILD_TESTING = ON
* BUILD_PAT = ON
* CMAKE_BUILD_TYPE = Release
* CPACK_BINARY_IFW = ON

When building Windows packages, you need to set the build flags as follows:
* BUILD_DVIEW = ON
* BUILD_OS_APP = ON
* BUILD_PACKAGE = ON
* BUILD_TESTING = ON
* BUILD_PAT = ON
* BUILD_CSHARP_BINDINGS = ON

Windows builds need to be built with Microsoft Visual Studio 2013 Service Pack 5. Use Visual Studio to set the build type to “Release|x64” and then build “PACKAGE”.

## CTest

In all cases testing needs to be built, and in testing results need to be included with your PR. Testing can be invoked simply by running CTest from a command prompt in the build directory.

## Sanity Testing

Sanity testing involves the following:
* Install package
* Verify operation by running a model

## Continuous Integration

OpenStudio CI will attempt to build and test your pull request once created. Learn more about [OpenStudio CI](http://https://github.com/NREL/OpenStudio/wiki/OpenStudio-Continuous-Integration).
