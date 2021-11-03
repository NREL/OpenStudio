# OpenStudio Version 3.3.0

_Release Notes - 11/05/2021_

These release notes describe version 3.3.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Sciences Center, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog

# Overview
As of April 2020, development and distribution of the OpenStudioApplication and the SketchUp plugin have transitioned to the OpenStudio Coalition, who is independently managing and distributing the software through its own [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) repository. The OpenStudio SDK is continuing to be actively developed and distributed by NREL and is released two times per year, through a spring and a fall release.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.3.0**__
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

OpenStudio SDK 3.3.0 is supported on 64-bit Windows 7 – 10, OS X 10.15, and Ubuntu 18.04, 20.04

OpenStudio SDK 3.3.0 supports [EnergyPlus Release 9.6](https://github.com/NREL/EnergyPlus/releases/tag/v9.6.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio SDK 3.3.0.

OpenStudio SDK 3.3.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio SDK 3.3.0.

As usual, you can refer to the **[OpenStudio SDK Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2019 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply re-run the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/.

# OpenStudio SDK: Changelog

The 3.3.0 is a major release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2021-05-04..2021-11-05+).


## Python Bindings 

As of OpenStudio SDK 3.2.0, Python bindings are offically supported and distrbuted through  Python Package Index (PyPI). To install, users will need to have Python3 installed along with pip and simply run the following command in a terminal window. 

`pip install openstudio==3.3.0` 

 Please see [openstudio on PyPi](https://pypi.org/project/openstudio/) for further instructions on how to install. Users can also visit the test channel at https://test.pypi.org/project/openstudio/ to install development bindings. 

## New Features, Major Fixes and API-breaking changes

* Support for EnergyPlus 9.6 [space concept](https://github.com/NREL/EnergyPlus/blob/develop/design/FY2021/NFP-Spaces.md):
  * [#4471](https://github.com/NREL/OpenStudio/pull/4471) - Space PR: adjustments to make DaylightingControls work by @jmarrec
  * [#4434](https://github.com/NREL/OpenStudio/pull/4434) - Addresses #4409, forward translate Space by @joseph-robertson
  * [#4447](https://github.com/NREL/OpenStudio/pull/4447) - Add ForwardTranslator options to the CLI `run` method by @jmarrec
* [#4406](https://github.com/NREL/OpenStudio/pull/4406) - Update to EnergyPlus v9.6.0 by @joseph-robertson
* [#4350](https://github.com/NREL/OpenStudio/pull/4350) - Fix #3599 - Add new fields to control outdoor air for AirTerminalSingleDuctInletMixer by @jmarrec
* [#4365](https://github.com/NREL/OpenStudio/pull/4365) - Addresses #4364, Availability Schedule for DaylightingControl by @joseph-robertson
* [#4342](https://github.com/NREL/OpenStudio/pull/4342) - Fix #4327 - FanSystemModel electricPowerFraction should be optional by @jmarrec
* [#4368](https://github.com/NREL/OpenStudio/pull/4368) - Addresses #4362, add support of HeatPump:PlantLoop:EIR:Heating and HeatPump:PlantLoop:EIR:Cooling to model air-to-water heat pump by @joseph-robertson
* [#4382](https://github.com/NREL/OpenStudio/pull/4382) - Addresses #4380, completing SDK support of Tubular and Light Well Daylighting Devices by @joseph-robertson
* [#4381](https://github.com/NREL/OpenStudio/pull/4381) - Addresses #3906, support new field 'Minimum Air Flow Turndown Schedule Name' for AirTerminals by @joseph-robertson
* [#4407](https://github.com/NREL/OpenStudio/pull/4407) - Add AirLoopHVAC to ThreeJS user data by @antoine-galataud
* [#4402](https://github.com/NREL/OpenStudio/pull/4402) - Addresses #3910, realign OS:SizingPeriod:DesignDay with E+ IDD by @joseph-robertson
* [#4417](https://github.com/NREL/OpenStudio/pull/4417) - Fix #4156 - Support nested subfolders in approved Measure directories by @jmarrec
* [#4452](https://github.com/NREL/OpenStudio/pull/4452) - Fix #4448 - Issues with angles in DaylightingControl by @jmarrec
* [#4465](https://github.com/NREL/OpenStudio/pull/4465) - [Developer] Refactor Forward Translation of ThermalZone in particular for Space translation PR by @jmarrec
* [#4317](https://github.com/NREL/OpenStudio/pull/4317) - Addresses #3147, add HeatExchanger:Desiccant:BalancedFlow by @joseph-robertson


## Minor changes and bug fixes

* [#4349](https://github.com/NREL/OpenStudio/pull/4349) - Fix #3514 - Make strings returned from openstudio to ruby UTF8 by @jmarrec
* [#4346](https://github.com/NREL/OpenStudio/pull/4346) - Addresses #4329 and #4330, issues with AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass by @joseph-robertson
* [#4356](https://github.com/NREL/OpenStudio/pull/4356) - Fix #4353 - Add a missing reference in E+ Idd for PythonPlugin:Instance (`ProgramNames`) by @jmarrec
* [#4355](https://github.com/NREL/OpenStudio/pull/4355) - Fix #4354 - Issue AirLoopHVACOutdoorAirSystem::reliefComponents with ZoneHVACComponents: wrong order by @jmarrec
* [#4348](https://github.com/NREL/OpenStudio/pull/4348) - Fix #2145 - Do not create an extra singlezone SPM humidity min created if humidifier downstream of fan by @jmarrec
* [#4347](https://github.com/NREL/OpenStudio/pull/4347) - Fix #227 - Actually use the EPW start year or start day of week when "UseWeatherFile" is used in YearDescription for assumedYear by @jmarrec
* [#4343](https://github.com/NREL/OpenStudio/pull/4343) - Fix #4335 - Should reset the air nodes when cloning a AirLoopHVACUnitarySystem by @jmarrec
* [#4345](https://github.com/NREL/OpenStudio/pull/4345) - #4046 - Clarify the situation with output file in the ReportingMeasure Template by @jmarrec
* [#4339](https://github.com/NREL/OpenStudio/pull/4339) - Fix #4338 - AirLoopHVAC_Impl::terminalForLastBranch doesn't handle Dual duct correctly by @jmarrec 
* [#4359](https://github.com/NREL/OpenStudio/pull/4359) - Addresses #4314, add ScheduleRuleset custom day methods by @joseph-robertson
* [#4371](https://github.com/NREL/OpenStudio/pull/4371) - Github Actions to publish python bindings to pypi when it's a release and testing worked by @jmarrec
* [#4369](https://github.com/NREL/OpenStudio/pull/4369) - #4214 Add in automated testing for 32/64 bit nuget package by @tijcolem
* [#4352](https://github.com/NREL/OpenStudio/pull/4352) - Fix #1571 - building.clone breaks space surface matching by @jmarrec
* [#4378](https://github.com/NREL/OpenStudio/pull/4378) - Addresses #1515, Reverse Translator should retain thermal zone name from IDF instead of re-naming it by @joseph-robertson 
* [#4370](https://github.com/NREL/OpenStudio/pull/4370) - #4177 Non-zero exit code for openstudio-cli by @tijcolem
* [#4376](https://github.com/NREL/OpenStudio/pull/4376) - Fix #4309 - disable RUBYLIB env and use --include or -I using cli by @tijcolem
* [#4383](https://github.com/NREL/OpenStudio/pull/4383) - Provide more options for installation of openstudio dlls by @macumber
* [#4385](https://github.com/NREL/OpenStudio/pull/4385) - Fixes Crash in floorplanToThreeJS https://github.com/NREL/OpenStudio/issues/4384 by @macumber
* [#4389](https://github.com/NREL/OpenStudio/pull/4389) - BCL Update by @axelstudios
* [#4398](https://github.com/NREL/OpenStudio/pull/4398) - ref #4397 - Fix some issues in our Init calls for embedded ruby by @jmarrec
* [#4305](https://github.com/NREL/OpenStudio/pull/4305) - Fix #4304 - Minimal fix for GroundHeatExchanger:Vertical to avoid confusion between maximum and design flow rates by @jmarrec
* [#4400](https://github.com/NREL/OpenStudio/pull/4400) - #4399 - Surface fixes by @jmarrec
* [#4392](https://github.com/NREL/OpenStudio/pull/4392) - Fix #4390 - add OSRunner::getOptionalBoolArgumentValue by @jmarrec
* [#4420](https://github.com/NREL/OpenStudio/pull/4420) - Fix #4419 - custom mapping for LCC Use Price Escalation by @jmarrec
* [#4428](https://github.com/NREL/OpenStudio/pull/4428) - Avoid issues with openstudio_ruby revision not matching  by @jmarrec
* [#4421](https://github.com/NREL/OpenStudio/pull/4421) - Fix #4220 - Don't pass built in POD types by const ref& but by value by @jmarrec
* [#4401](https://github.com/NREL/OpenStudio/pull/4401) - Update to new stable tag from Github Actions CI (includes CP720 fix) by @jmarrec
* [#4427](https://github.com/NREL/OpenStudio/pull/4427) - Fix #4423 - enable coverage on one of the linux runners by @jmarrec
* [#4418](https://github.com/NREL/OpenStudio/pull/4418) - Resolves #4395 Add in CLI option to use epJSON by @tijcolem
* [#4440](https://github.com/NREL/OpenStudio/pull/4440) - Fix #4439 - Correctly Reverse Translate gbXML Schedules by @jmarrec
* [#4450](https://github.com/NREL/OpenStudio/pull/4450) - Fix #4446 - add SizingSystem::autosizedOccupantDiversity. by @jmarrec
* [#4454](https://github.com/NREL/OpenStudio/pull/4454) - Fix #4404 #4453 - Fix issues with ForwardTranslator options by @jmarrec
* [#4431](https://github.com/NREL/OpenStudio/pull/4431) - Fixed an issue in Surface splitSurfaceForSubSurfaces @ggartside
* [#4459](https://github.com/NREL/OpenStudio/pull/4459) - Fix #3426 - ZoneHVACUnitHeater components missing air inlet/outlet node names when Fan:OnOff by @jmarrec
* [#4455](https://github.com/NREL/OpenStudio/pull/4455) - Fix #4445 - Fix API issues in Controller:WaterCoil by @jmarrec
* [#4462](https://github.com/NREL/OpenStudio/pull/4462) - Fix #4461 - DesignSpecificationOutdoorAir: when combining, Outdoor Air Flow rate for ACH is off by 3600 when "Maximum" method is used by @jmarrec
* [#4463](https://github.com/NREL/OpenStudio/pull/4463) - Add a full FT test for DaylightingControls (backport from Space PR) by @jmarrec
* [#4466](https://github.com/NREL/OpenStudio/pull/4466) - [Developer] Add Full detailed tests for the FT of SpaceInfiltration objects (whether assigned to space or spacetype) by @jmarrec
* [#4468](https://github.com/NREL/OpenStudio/pull/4468) - [developer] Add a full FT test for ElectricEquipmentITEAirCooled (backport Space PR) by @jmarrec
* [#4467](https://github.com/NREL/OpenStudio/pull/4467) - Update unit tests for ElectricEquipmentITEAirCooled by @kbenne
* [#4470](https://github.com/NREL/OpenStudio/pull/4470) - [developer] use a custom build of E+ with a Space DSOA Fx by @jmarrec
* [#4473](https://github.com/NREL/OpenStudio/pull/4473) Fix #4472 - Modify Python in typemaps for `path` to accept string with Python 3 support by @jmarrec
* [#4476](https://github.com/NREL/OpenStudio/pull/4476) - Update gems for workflow and extension gem updates by @tijcolem
* [#4307](https://github.com/NREL/OpenStudio/pull/4307) - Standardize Curve IDD references like E+ does (Uni/Bi...variateFunctions) and remove hard check in model code by @jmarrec
* [#4337](https://github.com/NREL/OpenStudio/pull/4337) - Fix Encode searchTerm in BCL (including spaces, not only "+" signs) by @jmarrec
* [#4344](https://github.com/NREL/OpenStudio/pull/4476) - Update openstudio-gems for new versions: openstudio-extension-gem (v0.5.1), openstudio-workflow-gem (v2.3.1) and openstudio-standards (v0.2.15) by @tijcolem
* [#4332](https://github.com/NREL/OpenStudio/pull/4332) - Fix StringHelpers openstudio::replace by @jmarrec
* [#4326](https://github.com/NREL/OpenStudio/pull/4326) - Fix Wrap E+ WaterHeater:Sizing object by @jmarrec
* [#4324](https://github.com/NREL/OpenStudio/pull/4324) - FIx Do not Warn if Sql doesn't have a Year field by @jmarrec
* [#4319](https://github.com/NREL/OpenStudio/pull/4319) - Fix Wrap E+ FuelFactors object by @jmarrec
* [#4302](https://github.com/NREL/OpenStudio/pull/4302) - Fix Deprecation Warning of last argument as keyword parameters by @tijcolem


### OpenStudio Standards v0.2.15


* [#1153](https://github.com/NREL/openstudio-standards/pull/1153) - Fix error handling in thermal zone occupancy schedule method
* [#1160](https://github.com/NREL/openstudio-standards/pull/1160) - Fix HPWH capacity fix
* [#1123](https://github.com/NREL/openstudio-standards/pull/1123) - Updated documentation to cover all methods #1123
* [#1131](https://github.com/NREL/openstudio-standards/pull/1131) - Updated gem dependencies to resolve security issues #1131
* [#1120](https://github.com/NREL/openstudio-standards/pull/1120) - Added college building prototype #1120
* [#1117](https://github.com/NREL/openstudio-standards/pull/1117) - Fixed bug in schedule_compact_annual_min_max_value method #1117
* [#1113](https://github.com/NREL/openstudio-standards/pull/1113) - Disable sizing run periods after sizing run #1113
	
* Added new requirements for ASHRAE Standard 90.1-2019:
		
	* Hotel guest room ventilation and thermostat control
	* Update to freezer and reach-in refrigerator efficiency
	* Multi-zone system temperature supply applied to warm and humid climate zones
	* Update to computer room air-conditioning efficiency
	* Update to walk-in cooler and freezer efficiency
	* Update to damper leakage rate requirements
	* Added clean water pumps requirements
	* Update to boiler and furnace efficiency
	* Added logic to implement occupant standby mode control requirements
	* Updated daylighting control dimming requirements
	* Added non-transient dwelling units energy recovery based on enthalpy recovery ratio
	* Added ASHRAE Standard 62.1-2019 Simplified Procedure

### OpenStudio Server v3.3.0

* [#633](https://github.com/NREL/OpenStudio-server/pull/633) - Merge urbanopt/reopt by @brianlball
* [#634](https://github.com/NREL/OpenStudio-server/pull/634) - Fix in.osm not downloadable by @kflemin
* [#635](https://github.com/NREL/OpenStudio-server/pull/635) - Bump puma version for security by @nllong
* [#636](https://github.com/NREL/OpenStudio-server/pull/636) - Add docker container scanning using Trivy by @nllong

## Issue Statistics Since Previous Release

[116 Closed Issues](https://github.com/NREL/OpenStudio/issues?q=is%3Aissue+closed%3A%3E2021-05-04+)  
[99 New Issues](https://github.com/NREL/OpenStudio/issues?q=is%3Aissue+created%3A%3E2021-05-04)