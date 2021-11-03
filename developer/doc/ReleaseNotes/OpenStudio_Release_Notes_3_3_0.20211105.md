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

## GitHub auto-generated release notes between v3.3.0 and v3.2.0

* Addresses #3147, add HeatExchanger:Desiccant:BalancedFlow by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4317
* Fix #3514 - Make strings returned from openstudio to ruby UTF8 by @jmarrec in https://github.com/NREL/OpenStudio/pull/4349
* Addresses #4329 and #4330, issues with AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4346
* Fix #4353 - Add a missing reference in E+ Idd for PythonPlugin:Instance (`ProgramNames`) by @jmarrec in https://github.com/NREL/OpenStudio/pull/4356
* Fix #4354 - Issue in AirLoopHVACOutdoorAirSystem::reliefComponents with ZoneHVACComponents: wrong order by @jmarrec in https://github.com/NREL/OpenStudio/pull/4355
* Fix #3599 - Add new fields to control outdoor air for AirTerminalSingleDuctInletMixer by @jmarrec in https://github.com/NREL/OpenStudio/pull/4350
* Fix #2145 - Do not create an extra singlezone SPM humidity min created if humidifier downstream of fan by @jmarrec in https://github.com/NREL/OpenStudio/pull/4348
* Fix #227 - Actually use the EPW start year or start day of week when "UseWeatherFile" is used in YearDescription for assumedYear by @jmarrec in https://github.com/NREL/OpenStudio/pull/4347
* Fix #4335 - Should reset the air nodes when cloning a AirLoopHVACUnitarySystem by @jmarrec in https://github.com/NREL/OpenStudio/pull/4343
* #4046 - Clarify the situation with output file in the ReportingMeasure Template by @jmarrec in https://github.com/NREL/OpenStudio/pull/4345
* Fix #4338 - AirLoopHVAC_Impl::terminalForLastBranch doesn't handle Dual duct correctly by @jmarrec in https://github.com/NREL/OpenStudio/pull/4339
* Addresses #4314, add ScheduleRuleset custom day methods by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4359
* Github Actions to publish python bindings to pypi when it's a release and testing worked by @jmarrec in https://github.com/NREL/OpenStudio/pull/4371
* #4214 Add in automated testing for 32/64 bit nuget package by @tijcolem in https://github.com/NREL/OpenStudio/pull/4369
* Fix #1571 - building.clone breaks space surface matching by @jmarrec in https://github.com/NREL/OpenStudio/pull/4352
* Addresses #1515, Reverse Translator should retain thermal zone name from IDF instead of re-naming it by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4378
* #4177 Non-zero exit code for openstudio-cli by @tijcolem in https://github.com/NREL/OpenStudio/pull/4370
* Addresses #4364, Availability Schedule for DaylightingControl by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4365
* Fix #4327 - FanSystemModel electricPowerFraction should be optional by @jmarrec in https://github.com/NREL/OpenStudio/pull/4342
* Fix #4309 - disable RUBYLIB env and use --include or -I using cli by @tijcolem in https://github.com/NREL/OpenStudio/pull/4376
* Provide more options for installation of openstudio dlls by @macumber in https://github.com/NREL/OpenStudio/pull/4383
* Fixes https://github.com/NREL/OpenStudio/issues/4384 by @macumber in https://github.com/NREL/OpenStudio/pull/4385
* Development BCL Update by @axelstudios in https://github.com/NREL/OpenStudio/pull/4389
* Addresses #4362, add support of HeatPump:PlantLoop:EIR:Heating to model air-to-water heat pump by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4368
* Addresses #4380, completing SDK support of Tubular Daylighting Devices (TDDs) by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4382
* ref #4397 - Fix some issues in our Init calls for embedded ruby by @jmarrec in https://github.com/NREL/OpenStudio/pull/4398
* Fix #4304 - Minimal fix for GroundHeatExchanger:Vertical to avoid confusion between maximum and design flow rates by @jmarrec in https://github.com/NREL/OpenStudio/pull/4305
* Addresses #3906, support new field 'Minimum Air Flow Turndown Schedule Name' for AirTerminals by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4381
* #4399 - Surface fixes by @jmarrec in https://github.com/NREL/OpenStudio/pull/4400
* Fix #4390 - add OSRunner::getOptionalBoolArgumentValue by @jmarrec in https://github.com/NREL/OpenStudio/pull/4392
* Fix #4419 - custom mapping for LCC Use Price Escalation by @jmarrec in https://github.com/NREL/OpenStudio/pull/4420
* Avoid issues with openstudio_ruby revision not matching  by @jmarrec in https://github.com/NREL/OpenStudio/pull/4428
* Fix #4220 - Don't pass built in POD types by const ref& but by value by @jmarrec in https://github.com/NREL/OpenStudio/pull/4421
* Update to new stable tag from Github Actions CI (includes CP720 fix) by @jmarrec in https://github.com/NREL/OpenStudio/pull/4401
* Add AirLoopHVAC to ThreeJS user data by @antoine-galataud in https://github.com/NREL/OpenStudio/pull/4407
* Fix #4423 - enable coverage on one of the linux runners by @jmarrec in https://github.com/NREL/OpenStudio/pull/4427
* Resolves #4395 Add in CLI option to use epJSON by @tijcolem in https://github.com/NREL/OpenStudio/pull/4418
* Fix #4439 - Correctly Reverse Translate gbXML Schedules by @jmarrec in https://github.com/NREL/OpenStudio/pull/4440
* Addresses #3910, realign OS:SizingPeriod:DesignDay with E+ IDD by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4402
* Update to v9.6.0-IOFreeze by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4406
* Fix #4446 - add SizingSystem::autosizedOccupantDiversity. by @jmarrec in https://github.com/NREL/OpenStudio/pull/4450
* Fix #4156 - Support nested subfolders in approved Measure directories by @jmarrec in https://github.com/NREL/OpenStudio/pull/4417
* Fix #4404 #4453 - Fix issues with ForwardTranslator options by @jmarrec in https://github.com/NREL/OpenStudio/pull/4454
* Fixed an issue in Surface splitSurfaceForSubSurfaces which caused the… by @ggartside in https://github.com/NREL/OpenStudio/pull/4431
* Fix #3426 - ZoneHVACUnitHeater components missing air inlet/outlet node names when Fan:OnOff by @jmarrec in https://github.com/NREL/OpenStudio/pull/4459
* Fix #4448 - Issues with angles in DaylightingControl by @jmarrec in https://github.com/NREL/OpenStudio/pull/4452
* Fix #4445 - Fix API issues in Controller:WaterCoil by @jmarrec in https://github.com/NREL/OpenStudio/pull/4455
* Fix #4461 - DesignSpecificationOutdoorAir: when combining, Outdoor Air Flow rate for ACH is off by 3600 when "Maximum" method is used by @jmarrec in https://github.com/NREL/OpenStudio/pull/4462
* Add a full FT test for DaylightingControls (backport from Space PR) by @jmarrec in https://github.com/NREL/OpenStudio/pull/4463
* [Developer] Add Full detailed tests for the FT of SpaceInfiltration objects (whether assigned to space or spacetype) by @jmarrec in https://github.com/NREL/OpenStudio/pull/4466
* [Developer] Refactor Forward Translation of ThermalZone in particular for Space translation PR by @jmarrec in https://github.com/NREL/OpenStudio/pull/4465
* [developer] Add a full FT test for ElectricEquipmentITEAirCooled (backport Space PR) by @jmarrec in https://github.com/NREL/OpenStudio/pull/4468
* Update unit tests for ElectricEquipmentITEAirCooled by @kbenne in https://github.com/NREL/OpenStudio/pull/4467
* [developer] Space PR: adjustements to make DaylightingControls work by @jmarrec in https://github.com/NREL/OpenStudio/pull/4471
* [developer] use a custom build of E+ with a Space DSOA Fx by @jmarrec in https://github.com/NREL/OpenStudio/pull/4470
* Addresses #4409, forward translate Space by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4434
* Fix #4472 - Modify Python in typemaps for `path` to accept string with Python 3 support by @jmarrec in https://github.com/NREL/OpenStudio/pull/4473
* Add ForwardTranslator options to the CLI `run` method by @jmarrec in https://github.com/NREL/OpenStudio/pull/4447
* update gems for workflow and extension gem updates by @tijcolem in https://github.com/NREL/OpenStudio/pull/4476

* [#4307](https://github.com/NREL/OpenStudio/pull/4307) - Standardize Curve IDD references like E+ does (Uni/Bi...variateFunctions) and remove hard check in model code 
* [4337](https://github.com/NREL/OpenStudio/pull/4337) - Fix Encode searchTerm in BCL (including spaces, not only "+" signs)
* [#4344](https://github.com/NREL/OpenStudio/pull/4344) - Update openstudio-gems for new versions of measure-tester-gem (v0.3.1), openstudio-extension-gem (v0.4.3), openstudio-workflow-gem (v2.2.1) and openstudio-standards (v0.2.14). 
* [#4332](https://github.com/NREL/OpenStudio/pull/4332) -  Fix StringHelpers openstudio::replace
* [#4326](https://github.com/NREL/OpenStudio/pull/4326) - Fix Wrap E+ WaterHeater:Sizing object
* [#4324](https://github.com/NREL/OpenStudio/pull/4324) - FIx Do not Warn if Sql doesn't have a Year field
* [#4319](https://github.com/NREL/OpenStudio/pull/4319) - Fix Wrap E+ FuelFactors object
* [#4302](https://github.com/NREL/OpenStudio/pull/4302) - Fix Deprecation Warning of last argument as keyword parameters

## New Contributors
* @antoine-galataud made their first contribution in https://github.com/NREL/OpenStudio/pull/4407

## New Features, Major Fixes and API-breaking changes

TODO

* **New objects**:

TODO

## Minor changes:

TODO

## Minor Bug Fixes
TODO

### OpenStudio Standards v0.2.15

* Added comstock 2016 and 2019 templates
* Fixed a coil reference for radiant systems 
* Fixed a method name compatibility issue for VAV terminals for OS v3.0.0 and earlier
* Adjusted radiant fractions for lighting to better reflected lighting fractions in newer lighting technology
* ...
* Add full list TODO


### OpenStudio Server v3.3.0

TODO

## Issue Statistics Since Previous Release

[116 Closed Issues](https://github.com/NREL/OpenStudio/issues?q=is%3Aissue+closed%3A%3E2021-05-04+)  
[99 New Issues](https://github.com/NREL/OpenStudio/issues?q=is%3Aissue+created%3A%3E2021-05-04)