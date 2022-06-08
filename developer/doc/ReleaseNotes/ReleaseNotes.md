# OpenStudio Version 3.4.1

_Release Notes_ -  _TDB_

These release notes describe version 3.4.1 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Sciences Center, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog

# Overview
As of April 2020, development and distribution of the OpenStudioApplication and the SketchUp plugin have transitioned to the OpenStudio Coalition, who is independently managing and distributing the software through its own [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) repository. The OpenStudio SDK is continuing to be actively developed and distributed by NREL and is released two times per year, through a spring and a fall release.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.4.1**__
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

OpenStudio SDK 3.4.1 is supported on 64-bit Windows 7 – 10, OS X 10.15, and Ubuntu 18.04, 20.04

OpenStudio SDK 3.4.1 supports [EnergyPlus Release 22.1.0](https://github.com/NREL/EnergyPlus/releases/tag/v22.1.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio SDK 3.4.1.

OpenStudio SDK 3.4.1 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio SDK 3.4.1.

As usual, you can refer to the **[OpenStudio SDK Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/.

# OpenStudio SDK: Changelog

The 3.4.1 is a minor release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2021-11-05..2022-04-26+).


## Python Bindings

As of OpenStudio SDK 3.2.0, Python bindings are officially supported and distributed through Python Package Index (PyPI). To install, users will need to have Python3 installed along with pip and simply run the following command in a terminal window.

`pip install openstudio==3.4.1`

**New in 3.4.1: Python bindings for macOS arm64 (M1 mac) are now provided on Pypi**

Please refer to the [Python Bindings Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Python-Binding-Version-Compatibility-Matrix) for more information.

Please see [openstudio on PyPi](https://pypi.org/project/openstudio/) for further instructions on how to install. Users can also visit the test channel at [openstudio on TestPyPi](https://test.pypi.org/project/openstudio/) to install development bindings.

## New Features, Major Fixes and API-breaking changes

* [#4589](https://github.com/NREL/OpenStudio/pull/4589) - Switch to E+ Space FT by default
    * OpenStudio SDK 3.3.0 added support to the new EnergyPlus 9.6.0 [space concept](https://github.com/NREL/EnergyPlus/blob/develop/design/FY2021/NFP-Spaces.md). This was an opt-in feature, as it was disabled by default.
    * This is now enabled by default. You can opt-out by by manually calling `ForwardTranslator::setExcludeSpaceTranslation(true)` before calling `ForwardTranslator::translateModel(Model&)` or passing the CLI argument `--no-space-translation` to the `run` method (eg: `openstudio run --no-space-translation -w workflow.osw`)
    * As a reminder, a few limitations due to DaylightingControls/IlluminanceMap exist: in case one of these objects is found in a ThermalZone, spaces will be combined for that ThermalZone (historical behavior)

## Minor changes and bug fixes


### OpenStudio Standards vXXX


### OpenStudio Server v3.4.1


## Issue Statistics Since Previous Release

