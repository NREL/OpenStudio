# OpenStudio Version 3.1.0

_Release Notes - MM/DD/YYYY_

These release notes describe version 3.1.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Systems, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog


# Overview

Following an important announcement related to future development of the OpenStudio Application released in September 2019 ([A Shift in BTO’s BEM Strategy: A New Future for the OpenStudio Application](https://www.openstudio.net/new-future-for-openstudio-application)), **this release marks the first installment of the separated OpenStudio SDK from the Application.**
Going forward, the SDK will continue to be distributed and released two times per year, in April and October. Development of the OpenStudioApplication has been moved to its own [NREL/OpenStudioApplication](https://github.com/NREL/OpenStudioApplication/) GitHub.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.1.0**__
- EnergyPlus
- Command Line Interface (CLI)
- Radiance
- Ruby API
- C++ SDK

**Note** that [PAT](https://github.com/NREL/OpenStudio-PAT) is not included in either the SDK or Application installers. You will need to install PAT separately which is distributed on the [OpenStudio-PAT](https://github.com/NREL/OpenStudio-PAT) GitHub page.

# Where to Find OpenStudio Documentation

- OpenStudio release documentation, including these release notes, tutorials, and other user documentation, is available at [https://www.openstudio.net/](https://www.openstudio.net/.)
- C++ API documentation is available at [https://openstudio-sdk-documentation.s3.amazonaws.com/index.html](https://openstudio-sdk-documentation.s3.amazonaws.com/index.html)
- Measure development documentation is available at [http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/](http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/ )
- A roadmap for planned features is available at http://nrel.github.io/OpenStudio-user-documentation/getting_started/roadmap/.

# Installation Notes

OpenStudio 3.1.0 is supported on 64-bit Windows 7 – 10, OS X 10.12 – 10.15, and Ubuntu 18.04.

OpenStudio 3.1.0 supports [EnergyPlus Release 9.3](https://github.com/NREL/EnergyPlus/releases/tag/v9.3.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio 3.1.0.

OpenStudio 3.1.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio 3.1.0.

OpenStudio 3.x now uses **Ruby 2.5.5** as the supported version (2.2.4 previously).

As usual, you can refer to the **[OpenStudio Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-Version-Compatibility-Matrix)** for more information.

## Installation Steps

- Download and install OpenStudio SDK and/or [OpenStudioApplication]( depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2019 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply rerun the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user- documentation/help/troubleshooting/.

# OpenStudio SDK: Changelog

The 3.1.0 is a minor release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here **TODO**](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2020-02-25..2020-MM-DD+).


## New Features, Major Fixes and API-breaking changes

A number of API-breaking changes have been implemented in OpenStudio 3.1.0:

* [#3960](https://github.com/NREL/OpenStudio/pull/3960) - Added support for building the C# bindings via `dotnet` CLI, including on Unix platforms.
* [#3959](https://github.com/NREL/OpenStudio/pull/3959) - Also included some improvements in the generated C# bindings by reducing build warnings and properly exposing some types via SWIG
    * `ScheduleTypeKey` (which is normally only use by OpenStudio internals in ScheduleTypeRegistry checks) previously mapped to a `std::pair<std::string, std::string>` which was SWIGed in ruby as an Array of strings of size two, but improperly exposed in C#. It now uses a dedicated helper class with two methods `ScheduleTypeKey::className()` and `ScheduleTypeKey::scheduleDisplayName()`
* [#4050](https://github.com/NREL/OpenStudio/pull/4050) - Fix #3921 - some children of RefrigerationSystem can be added several times and produce a fatal when System is removed.
    * `RefrigerationSystem` will now be enforcing unicity for all children. This was already the case for a few of the child objects such as Case and WalkIns, it is now the case for all objects (`RefrigerationCondenserCascade`, `RefrigerationSubcoolerMechanical` and `RefrigerationSubcoolerLiquidSuction`, etc.). What this means is that setter methods (or add methods in case of a list) will remove the child for any current RefrigerationSystem it is on first.

## Minor changes:

* [#XXX](https://github.com/NREL/OpenStudio/pull/XXX) - Description


## Minor Bug Fixes


* [#XXXX](https://github.com/NREL/OpenStudio/issues/XXXX) - Description


### OpenStudio Standards

### OpenStudio Server

### Other?


## Issue Statistics Since Previous Release

* XX new issues were filed since the 3.0.0 release of OpenStudio (not including opened pull requests)

* XX issues were closed since the 3.0.0 release of OpenStudio (not including closed pull requests).

