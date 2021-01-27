# OpenStudio Version 3.2.0

_Release Notes - **TDB**/2021_

These release notes describe version 3.2.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Sciences Center, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog

# Overview
As of April 2020, development and distribution of the OpenStudioApplication and the SketchUp plugin have transitioned to the OpenStudio Coalition, who is independently managing and distributing the software through its own [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) repository. The OpenStudio SDK is continuing to be actively developed and distributed by NREL and is released two times per year, through a spring and a fall release.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.2.0**__
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

OpenStudio SDK 3.2.0 is supported on 64-bit Windows 7 – 10, OS X 10.14 – 10.15, and Ubuntu 18.04.**TODO: update to 20.04?**

OpenStudio SDK 3.2.0 supports [EnergyPlus Release 9.4](https://github.com/NREL/EnergyPlus/releases/tag/v9.4.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio SDK 3.2.0.

OpenStudio SDK 3.2.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio SDK 3.2.0.

OpenStudio SDK 3.x now uses **Ruby 2.5.5** as the supported version (2.2.4 previously). **TODO: update to 2.7.2?**

As usual, you can refer to the **[OpenStudio SDK Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2019 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply rerun the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user- documentation/help/troubleshooting/.

# OpenStudio SDK: Changelog

The 3.2.0 is a major release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2020-10-16..2021-04-15+).
**TODO: update link**

## New Features, Major Fixes and API-breaking changes

A number of new features and API-breaking changes have been implemented in OpenStudio SDK 3.2.0:

* [#4155](https://github.com/NREL/OpenStudio/pull/4155) - Wrap `Humidifier:Stream:Gas` in OS SDK
* [#4157](https://github.com/NREL/OpenStudio/pull/4157) - Wrap `ZoneHVAC:CoolingPanel:RadiantConvective:Water` in OS SDK
* [#4192](https://github.com/NREL/OpenStudio/pull/4192) - Wrap `Generator:WindTurbine` in OS SDK
* [#4193](https://github.com/NREL/OpenStudio/pull/4193) - Wrap `Fan:ComponentModel` in OS SDK
* [#4194](https://github.com/NREL/OpenStudio/pull/4194) - Wrap `PhotovoltaicPerformance:Sandia` in OS SDK
    * The recommended instantiation is via the factory methods `GeneratorPhotovoltaic::sandia(const Model&)` or `GeneratorPhotovoltaic::fromSandiaDatabase(const Model&, const std::string& sandiaModulePerformanceName)` where `sandiaModulePerformanceName` is one of keys in the now-embedded Sandia database. Please use the `PhotovoltaicPerformanceSandia::sandiaModulePerformanceNames()` static method to lookup the valid names.
* [#4180](https://github.com/NREL/OpenStudio/pull/4180) - Fixes [#4175](https://github.com/NREL/OpenStudio/issues/4175), issues with ft for ElectricLoadCenter:Distribution
    * `ElectricLoadCenterDistribution` objects are now possible with no generators / transformers
    * A default `ElectricLoadCenterDistribution` is no longer instantiated in the constructor of children objects: `GeneratorFuelCell`, `GeneratorMicroTurbine`, `GeneratorPhotovoltaic`, `GeneratorPVWatts

**TODO**: Python


## Minor changes:

* [#4188](https://github.com/NREL/OpenStudio/pull/4188) - Fix #4187 - speed up `IdfFile::save` (and derived, such as `Model::save`) and general serialization to string by using '\n' instead of `std::endl`


## Minor Bug Fixes

**TODO**


### OpenStudio Standards vX.Y.Z

**TODO**

### OpenStudio Server vX.Y.Z


**TODO**


## Issue Statistics Since Previous Release

**TODO**
