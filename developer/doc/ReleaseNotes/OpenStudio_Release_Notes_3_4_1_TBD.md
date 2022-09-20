# OpenStudio Version 3.4.1 (TDB)

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
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2021-2022 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply re-run the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/.

# OpenStudio SDK: Changelog

The 3.4.1 is a minor release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release (**NEEDS UPDATE**) [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2021-04-27..2022-09-26+).


## Python Bindings

As of OpenStudio SDK 3.2.0, Python bindings are officially supported and distributed through Python Package Index (PyPI). To install, users will need to have Python3 installed along with pip and simply run the following command in a terminal window.

`pip install openstudio==3.4.1`

Please see [openstudio on PyPi](https://pypi.org/project/openstudio/) for further instructions on how to install. Users can also visit the test channel at [openstudio on TestPyPi](https://test.pypi.org/project/openstudio/) to install development bindings.

You can also refer to the [OpenStudio SDK Python Binding Version Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Python-Binding-Version-Compatibility-Matrix) to see the list of supported platforms and python versions.

## New Features, Major Fixes and API-breaking changes

* [#4616](https://github.com/NREL/OpenStudio/pull/4616) - Addresses #4611, allow non-Quadratic curves for the EIR-f-PLR for the Chiller:Electric:EIR object
    * `Chiller:Electric:EIR` has a few API-breaking changes related to its Curves. The types for the constructor, getters and setters used to be explicit (eg: `CurveBiquadratic`): it is now the base class, more generic, `Curve` type for added flexibility.
* [#4642](https://github.com/NREL/OpenStudio/pull/4642) - Addresses #4575, API change for EvaporativeCoolerFluid:SingleSpeed
    * `EvaporativeCoolerFluid:SingleSpeed` has an API-breaking change related to its `performanceInputMethod` getter. It is now a required field that returns `std::string` instead of `boost::optional<std::string>`.
* [#4644](https://github.com/NREL/OpenStudio/pull/4644) - Addresses #4575, API change for Coil:Cooling:DX:SingleSpeed and Coil:Cooling:DX:TwoSpeed
    * `Coil:Cooling:DX:SingleSpeed` and `Coil:Cooling:DX:TwoSpeed` have API-breaking changes related to many of their getters. They are now required fields that return `double` instead of `boost::optional<double>`. Also removes many deprecated methods.
* [#4632](https://github.com/NREL/OpenStudio/pull/4632) - Remove deprecated methods for AirWallMaterial, Node, SizingSystem, ZoneAirMassFlowConservation
    * Removes the deprecated `AirWallMaterial` class completely, in favor of `ConstructionAirBoundary`
    * Removes functions in `Node` that have been deprecated for 9+ years
    * Removes deprecated methods in the `SizingSystem`, `ZoneAirMassFlowConservation`, `PlantLoop` & `AirLoopHVAC`, `AvailablilityManagerNightCycle`, `ZoneVentilationDesignFlowRate` classes
    * Refer to the CSV file at [developer/ruby/deprecated_methods.csv](https://github.com/NREL/OpenStudio/blob/develop/developer/ruby/deprecated_methods.csv) for more information
* [#4665](https://github.com/NREL/OpenStudio/pull/4665) - Deprecate methods on Coil DX objects
    * `Coil:Cooling:DX:SingleSpeed`, `Coil:Cooling:DX:MultiSpeed:StageData`, and `Coil:Cooling:DX:CurveFit:Speed`: `ratedEvaporatorFanPowerPerVolumeFlowRate` and `setRatedEvaporatorFanPowerPerVolumeFlowRate` in favor of `ratedEvaporatorFanPowerPerVolumeFlowRate2017` and `setRatedEvaporatorFanPowerPerVolumeFlowRate2017`
    * `Coil:Heating:DX:SingleSpeed` and `Coil:Heating:DX:MultiSpeed:StageData`: `ratedSupplyFanPowerPerVolumeFlowRate` and `setRatedSupplyFanPowerPerVolumeFlowRate` in favor of `ratedSupplyFanPowerPerVolumeFlowRate2017` and `setRatedSupplyFanPowerPerVolumeFlowRate2017`


## Minor changes and bug fixes


**Developer changes**:

* [OpenStudio-benchmarks](https://github.com/NREL/OpenStudio-benchmarks) - Added additional performance benchmark tests to improve SDK performance.


## OpenStudio New Contributors


### OpenStudio Standards vXXXX


### OpenStudio Server vXXXX

## Issue Statistics Since Previous Release
