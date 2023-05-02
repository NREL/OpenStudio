# OpenStudio Version 3.6.0

_Release Notes_ -  _05/12/2023_

These release notes describe version 3.6.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Sciences Center, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog

# Overview

As of April 2020, development and distribution of the OpenStudioApplication and the SketchUp plugin have transitioned to the OpenStudio Coalition, who is independently managing and distributing the software through its own [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) repository. The OpenStudio SDK is continuing to be actively developed and distributed by NREL and is released two times per year, through a spring and a fall release.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.6.0**__
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

OpenStudio SDK 3.6.0 is supported on:

* 64-bit Windows 7 – 11
* macOS: 10.15+ x86_64, 12.1+ arm64
* Ubuntu: 20.04 x86_64, 22.04 x86_64, 22.04 arm64
* Centos7

OpenStudio SDK 3.6.0 supports [EnergyPlus Release 23.1.0](https://github.com/NREL/EnergyPlus/releases/tag/v23.1.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio SDK 3.6.0.

OpenStudio SDK 3.6.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio SDK 3.6.0.

As usual, you can refer to the **[OpenStudio SDK Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2021-2022 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply re-run the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/.

# OpenStudio SDK: Changelog

The 3.6.0 is a major release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2022-09-26..2023-05-12+).


## Python Bindings

As of OpenStudio SDK 3.2.0, Python bindings are officially supported and distributed through Python Package Index (PyPI). To install, users will need to have Python3 installed along with pip and simply run the following command in a terminal window.

`pip install openstudio==3.6.0`

Please see [openstudio on PyPi](https://pypi.org/project/openstudio/) for further instructions on how to install. Users can also visit the test channel at [openstudio on TestPyPi](https://test.pypi.org/project/openstudio/) to install development bindings.

You can also refer to the [OpenStudio SDK Python Binding Version Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Python-Binding-Version-Compatibility-Matrix) to see the list of supported platforms and python versions.

## New Features, Major Fixes and API-breaking changes

* [#4719](https://github.com/NREL/OpenStudio/issues/4719),[#4721](https://github.com/NREL/OpenStudio/issues/4721) - Phase 2 support to run Python based measures. This functionality is still experimental and can only be accessed from the CLI using the experimental `labs` subcommand. Phase 2 now allows users to run both a Python and Ruby measure in the same workflow. To explore the various new options this new feature has to offer, you can access the help menu `openstudio labs --help` Also, to help jump start python measure development, included in the installation are example `.osw` workflow files that contain Python, Ruby and Python+Ruby based workflows that can be ran using the labs subcommand. For instance, the following will run a Ruby + Python based workflow: `openstudio labs run -w ./Examples/compact_osw/compact_ruby_and_python.osw`

* [#4778](https://github.com/NREL/OpenStudio/pull/4778) - Wrap `AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl` and `AirConditioner:VariableRefrigerantFlow:FluidTemperatureControl:HR`
    * `ZoneHVAC:TerminalUnit:VariableRefrigerantFlow` has API-breaking changes related to setters and getters for its heating and cooling coils. They now use `HVACComponent` instead of the more restrictive `CoilHeatingDXVariableRefrigerantFlow` and `CoilCoolingDXVariableRefrigerantFlow`.
    * `AirConditionerVariableRefrigerantFlow::clone` was changed to stop cloning the child `Curve` objects (20 of them)

* [#4740](https://github.com/NREL/OpenStudio/pull/4740) - Fix issues around `ScheduleFixedInterval`
    * A minor API breaking change: `intervalLength`/`setIntervalLength` now return/accept an `int` rather than a `double` to conform to the IDD type `\integer`
    * Translated daily schedules start on correct day
    * Leap day is no longer ignored on translation
    * Interval no longer restricted to one day or less

* [#4813](https://github.com/NREL/OpenStudio/pull/4813) - Wrap `SolarCollectorPerformance:PhotovoltaicThermal:BIPVT`
    * `SolarCollectorFlatPlatePhotovoltaicThermal` has API-breaking changes in the `solarCollectorPerformance` getter due to the addition of this new object: it used to return a `SolarCollectorPerformancePhotovoltaicThermalSimple` (the only performance object at the time), now it's a `ModelObject`.
* [#4717](https://github.com/NREL/OpenStudio/pull/4717) Update `GroundHeatExchangerHorizontalTrench`: wrap `Site:GroundTemperature:Undisturbed:XXX` objects
    * Wrap `Site:GroundTemperature:Undisturbed:KusudaAchenbach` and `Site:GroundTemperature:Undisturbed:Xing` objects
    * Update `GroundHeatExchanger:HorizontalTrench` to actually use the Ground Temeprature Model field
* [#4532](https://github.com/NREL/OpenStudio/pull/4532) Performance improvements 
* [#4800](https://github.com/NREL/OpenStudio/pull/4800),[#4844](https://github.com/NREL/OpenStudio/pull/4800) Update to v23.1.0 EnergyPlus 
* [#4808](https://github.com/NREL/OpenStudio/pull/4808) Wrap `OutputControl:Timestamp`
* [#4825](https://github.com/NREL/OpenStudio/pull/4825) add Coil:UserDefined 
* [#4806](https://github.com/NREL/OpenStudio/pull/4806) Wrap Gas-Fired Absorption Heat Pump (GAHP)
    * Wrap `HeatPump:AirToWater:FuelFired:Heating` and `HeatPump:AirToWater:FuelFired:Cooling` objects
    * The objects define an equation-fit fuel-fired absorption heat pump, based on manufacture-provided performance curves and testing/modeling methods
    * The GAHP is a combined space heating and domestic hot water solution, especially for cold climates. It can serve as heat sources for plant loops of space heating coils, hydronic radiators, DHW water heaters, and low-temperature space heating floors or TABS systems. Driven by gas heat or other types of fuel heat, the equipment can consume very little electricity and can have a much higher overall COP than conventional fuel-combustion type boilers or water heaters.
* [#4799](https://github.com/NREL/OpenStudio/pull/4799) Add Fuel-related Methods from OpenStudio-Standard 
* [#4857](https://github.com/NREL/OpenStudio/pull/4857) `Schedule:File` ctor with path
    * Introduce new `ScheduleFile` constructor with path instead of `ExternalFile` argument
* Support Ubuntu 22.04 and remove 18.04


## Minor changes and bug fixes


* [#4828](https://github.com/NREL/OpenStudio/pull/4828) - Fix Space load-based actuator for spaces are named based on thermal zone name
    * As part of this PR, the optional field at the end `Zone Name` is replaced with `Zone or Space` and some API changes are there around it. The only minor breaking one is that `boost::optional<ModelObject> zoneName()` (deprecated) will now return either a Zone or a Space. Before if you called setSpace it would store the space's ThermalZone, now it stores the Space itself. This is unlikely to affect most users.
* [#4769](https://github.com/NREL/OpenStudio/pull/4769) fix(CSharp): fix buildCSharp CI
* [#4774](https://github.com/NREL/OpenStudio/pull/4774) Fix #4773 - Remove double % sign in `Zone Thermal Comfort ASHRAE 55 Adaptive Model 80% Acceptability Status` 
* [#4780](https://github.com/NREL/OpenStudio/pull/4780) Fixup the python bindings during release
* [#4739](https://github.com/NREL/OpenStudio/pull/4739) Fix #4738 - Workflow: Unify RunOptions/ForwardTranslator Options accross entire code base
* [#4785](https://github.com/NREL/OpenStudio/pull/4785) Allow forcing conan package build_type via CONAN_CONFIGURATION_TYPES when using non-multi generators
* [#4787](https://github.com/NREL/OpenStudio/pull/4787) VRF model object lists
* [#4770](https://github.com/NREL/OpenStudio/pull/4770) Height of shading objects is incorrect
* [#4725](https://github.com/NREL/OpenStudio/pull/4725) Fuel query updates
* [#4790](https://github.com/NREL/OpenStudio/pull/4790) Fixes 4783: Triangulation issue in openstudio::subtract
* [#4782](https://github.com/NREL/OpenStudio/pull/4782) Fix for containingHVACComponent
* [#4810](https://github.com/NREL/OpenStudio/pull/4810) Fixes CLI `--help` typo.
* [#4777](https://github.com/NREL/OpenStudio/pull/4777) 4766 different surface counts
* [#4814](https://github.com/NREL/OpenStudio/pull/4814) GenerateClass update: Rule of 5 for public hpp, const-correctness, automatic clang-format + automatic FT/RT tests 
* [#4816](https://github.com/NREL/OpenStudio/pull/4816) Fix #4815 - Adjust `ForwardTranslatorOptions::string()` for backward compat with openstudio-workflow-gem 
* [#4828](https://github.com/NREL/OpenStudio/pull/4828) Fix #4786 - Space load-based actuator for spaces are named based on thermal zone name 
* [#4805](https://github.com/NREL/OpenStudio/pull/4805) adding fix and test for oas removal when doas is removed 
* [#4819](https://github.com/NREL/OpenStudio/pull/4819) Properly forward arguments for execute_ruby_script / execute_python_script and allowing omitting the command
* [#4796](https://github.com/NREL/OpenStudio/pull/4796) autosized methods
* [#4829](https://github.com/NREL/OpenStudio/pull/4829) Fix some failing tests
* [#4833](https://github.com/NREL/OpenStudio/pull/4833) Update gems
* [#4835](https://github.com/NREL/OpenStudio/pull/4835) Fix #4834 - CLI chops inner single quote inside argument since 3.5.1
* [#4836](https://github.com/NREL/OpenStudio/pull/4836) Fix VersionTranslator use after move (in develop only)
* [#4839](https://github.com/NREL/OpenStudio/pull/4839) Conan need to be updated to supported for fmt/9.1.0 and cli11/2.3.2
* [#4845](https://github.com/NREL/OpenStudio/pull/4845) Minor fix to a template sortByObjectName
* [#4848](https://github.com/NREL/OpenStudio/pull/4848) Fix #4831 - Platform dependent results for joinAll
* [#4849](https://github.com/NREL/OpenStudio/pull/4849) Update standards to 0.3.1-rc2 and tbd gem to 3.2.2
* [#4851](https://github.com/NREL/OpenStudio/pull/4851) Recent clang throws on these functions. codecvt_utf8_utf16 is deprecated 
* [#4854](https://github.com/NREL/OpenStudio/pull/4854) Fix #4853 - RemoteBCLTest2
* [#4850](https://github.com/NREL/OpenStudio/pull/4850) Update standards jsons for SpaceType/Materials suggested Standards information 
* [#4855](https://github.com/NREL/OpenStudio/pull/4855) Update copyrights to 2023 
* [#4860](https://github.com/NREL/OpenStudio/pull/4860) Update cppcheck.yml to use --std=c++20 
* [#4859](https://github.com/NREL/OpenStudio/pull/4859) Fix #4856 - escape path with backward slash in Labs 
* [#4862](https://github.com/NREL/OpenStudio/pull/4862) Fix github action build for python and csharp bindings 
* [#4852](https://github.com/NREL/OpenStudio/pull/4852) Fix #4847 - OpenStudio logging behaves differently on Windows/Linux 
* [#4871](https://github.com/NREL/OpenStudio/pull/4871) update gems and set addressable gem to use v2.8.1
* [#4777](https://github.com/NREL/OpenStudio/pull/4777) Fix #4766 different surface counts 


**Full Changelog**: https://github.com/NREL/OpenStudio/compare/v3.5.1...v3.6.0

**New Contributors**:
@wenyikuang


### OpenStudio Standards v0.4.0

* [#1475](https://github.com/NREL/openstudio-standards/pull/1475) # Fix issue #109 check hard-sized values before autosized values
* [#1474](https://github.com/NREL/openstudio-standards/pull/1474) # Fix issue #238 space_type_apply_internal_loads to return boolean instead of nil when no infiltration data available
* [#1473](https://github.com/NREL/openstudio-standards/pull/1473) # Add more descriptive warning messages for missing standards space type, to help with issue #516
* [#1470](https://github.com/NREL/openstudio-standards/pull/1470) # Fix issue #1466 by adding a version reference for initialization summary lookup
* [#1447](https://github.com/NREL/openstudio-standards/pull/1447) # Fix issues #1446 and #1395 breaking out lighting controls into individual spaces
* [#1437](https://github.com/NREL/openstudio-standards/pull/1437) # Fix issue #754 add support for HeatExchangerFluidToFluid objects for heating and cooling fuels
* [#1436](https://github.com/NREL/openstudio-standards/pull/1436) # Fix issue #230 improve PRM baseline boiler naming
* [#1435](https://github.com/NREL/openstudio-standards/pull/1435) # Fix issue #1423 typo in find_and_set_insulation_layer method
* [#1428](https://github.com/NREL/openstudio-standards/pull/1428) # Appendix G PRM bug fixes, including unmet hour load loop, error handling, and schedule type checking with EMS
* [#1422](https://github.com/NREL/openstudio-standards/pull/1422) # Fix issue #1417 add default chiller efficiency
* [#1421](https://github.com/NREL/openstudio-standards/pull/1421) # Fix issue #1326 add model argument to create_cubic_curve and create_curve_exponent
* [#1418](https://github.com/NREL/openstudio-standards/pull/1418) # Fix issue #1228 OS_AirLoopHVAC_UnitarySystem typo in heating and cooling fuels system lookup
* [#1414](https://github.com/NREL/openstudio-standards/pull/1414) # Update DEER ventilation rate assumptions
* [#1410](https://github.com/NREL/openstudio-standards/pull/1410) # Remove obsolete OpenStudio model object autosizing and hardsizing methods


### OpenStudio Server 3.6.0

* [#686](https://github.com/NREL/OpenStudio-server/pull/686) Using bundled ruby gems with analysis runs
* [#687](https://github.com/NREL/OpenStudio-server/pull/687) add ModelMeasure to schema 
* [#690](https://github.com/NREL/OpenStudio-server/pull/690) Short name 
* [#694](https://github.com/NREL/OpenStudio-server/pull/694) Update actions

## Issue Statistics Since Previous Release

[71 Closed Issues](https://github.com/NREL/OpenStudio/issues?q=is%3Aissue+closed%3A%3E2022-11-10+)
[62 New Issues](https://github.com/NREL/OpenStudio/issues?q=is%3Aissue+created%3A%3E2022-11-10)