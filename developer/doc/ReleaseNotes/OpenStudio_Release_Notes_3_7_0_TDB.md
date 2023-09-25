# OpenStudio Version 3.7.0

_Release Notes_ - _TBD_

These release notes describe version 3.7.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Sciences Center, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog

# Overview

As of April 2020, development and distribution of the OpenStudioApplication and the SketchUp plugin have transitioned to the OpenStudio Coalition, who is independently managing and distributing the software through its own [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) repository. The OpenStudio SDK is continuing to be actively developed and distributed by NREL and is released two times per year, through a spring and a fall release.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.7.0**__
- EnergyPlus
- Command Line Interface (CLI)
- Radiance
- Ruby API
- C++ SDK

**Note** that [PAT](https://github.com/NREL/OpenStudio-PAT) is not included in either the SDK or the OpenStudio Coalition's Application installers. You will need to install PAT separately which is distributed on the [OpenStudio-PAT](https://github.com/NREL/OpenStudio-PAT) GitHub page.

# Where to Find OpenStudio SDK Documentation

- OpenStudio SDK release documentation, including these release notes, tutorials, and other user documentation, is available at [https://www.openstudio.net/](https://www.openstudio.net/.)
- C++ API documentation is available at [https://openstudio-sdk-documentation.s3.amazonaws.com/index.html](https://openstudio-sdk-documentation.s3.amazonaws.com/index.html)
- Measure development documentation is available at [http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/](http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/ )
- A roadmap for planned features is available at http://nrel.github.io/OpenStudio-user-documentation/getting_started/roadmap/.

# Installation Notes

OpenStudio SDK 3.7.0 is supported on:

* 64-bit Windows 7 – 11
* macOS: 10.15+ x86_64, 12.1+ arm64
* Ubuntu: 20.04 x86_64, 22.04 x86_64, 22.04 arm64
* Centos7

OpenStudio SDK 3.7.0 supports [EnergyPlus Release TDB](https://github.com/NREL/EnergyPlus/releases/tag/v23.1.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio SDK 3.7.0.

OpenStudio SDK 3.7.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio SDK 3.7.0.

As usual, you can refer to the **[OpenStudio SDK Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2021-2022 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply re-run the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/.

# OpenStudio SDK: Changelog

The 3.7.0 is a **major** release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2022-09-26..2023-05-12+).


## Python Bindings

As of OpenStudio SDK 3.2.0, Python bindings are officially supported and distributed through Python Package Index (PyPI). To install, users will need to have Python3 installed along with pip and simply run the following command in a terminal window.

`pip install openstudio==3.7.0`

Please see [openstudio on PyPi](https://pypi.org/project/openstudio/) for further instructions on how to install. Users can also visit the test channel at [openstudio on TestPyPi](https://test.pypi.org/project/openstudio/) to install development bindings.

You can also refer to the [OpenStudio SDK Python Binding Version Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Python-Binding-Version-Compatibility-Matrix) to see the list of supported platforms and python versions.

## New Features, Major Fixes and API-breaking changes

* [#4827](https://github.com/NREL/OpenStudio/pull/4827) - #4748 #4817 - Validate BCLXML with schema when loading + make sorting of files in measure.xml consistent when saving
* [#4873](https://github.com/NREL/OpenStudio/pull/4873) - AirLoopHVACUnitarySystem set Method During XXX Operation
    * Fix #4695 - AirLoopHVACUnitarySystem: Supply Air Flow Rate Method During <XXX> Operation should be set via related setters/autosize
    * Breaks the return of `supplyAirFlowRateMethodDuringCoolingOperation` and `supplyAirFlowRateMethodDuringHeatingOperation`: now returns `std::string` instead of `boost::optional<std::string>`
    * Deprecates many set/reset methods
* [#4932](https://github.com/NREL/OpenStudio/pull/4932) - Support undisturbed ground temperature models on GroundHeatExchangerVertical
    * Fix #4930 - Support undisturbed ground temperature models on GroundHeatExchangerVertical
    * Update `GroundHeatExchanger:Vertical` to actually use the Ground Temeprature Model field
* [#4923](https://github.com/NREL/OpenStudio/pull/4923) - Fix #4692 - Modify `Model::load` to use the VersionTranslator instead of loading it assuming the version of the loaded OSM is the same as the current SDK version being used.

## Minor changes and bug fixes

* [#4884](https://github.com/NREL/OpenStudio/pull/4884) - Fix #4882 - Avoid a crash in createFluidProperties during FT that's due to Undefined Behavior
* [#4881](https://github.com/NREL/OpenStudio/pull/4881) - Adjust CMakeLists for Ubuntu 22.04 arm64
* [#4888](https://github.com/NREL/OpenStudio/pull/4888) - Fix #4887 -  CSharp workflow - macos-10.15 was removed by @jmarrec in https://github.com/NREL/OpenStudio/pull/488

**Full Changelog**: https://github.com/NREL/OpenStudio/compare/v3.6.0...v3.7.0

**New Contributors**:

### OpenStudio Standards vXXX


### OpenStudio Server vXXX


## Issue Statistics Since Previous Release

