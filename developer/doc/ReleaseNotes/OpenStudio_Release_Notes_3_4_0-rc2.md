# OpenStudio Version 3.4.0

_Release Notes - TBD

These release notes describe version 3.4.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Sciences Center, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog

# Overview
As of April 2020, development and distribution of the OpenStudioApplication and the SketchUp plugin have transitioned to the OpenStudio Coalition, who is independently managing and distributing the software through its own [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) repository. The OpenStudio SDK is continuing to be actively developed and distributed by NREL and is released two times per year, through a spring and a fall release.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.4.0**__
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

OpenStudio SDK 3.4.0 is supported on 64-bit Windows 7 – 10, OS X 10.15, and Ubuntu 18.04, 20.04

OpenStudio SDK 3.4.0 supports [EnergyPlus Release 22.1.0](https://github.com/NREL/EnergyPlus/releases/tag/v22.1.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio SDK 3.4.0.

OpenStudio SDK 3.4.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio SDK 3.4.0.

As usual, you can refer to the **[OpenStudio SDK Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2019 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply re-run the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/.

# OpenStudio SDK: Changelog

The 3.4.0 is a major release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2021-11-05..2022-04-26+).


## Python Bindings

As of OpenStudio SDK 3.2.0, Python bindings are officially supported and distributed through Python Package Index (PyPI). To install, users will need to have Python3 installed along with pip and simply run the following command in a terminal window. 

`pip install openstudio==3.4.0`

Please see [openstudio on PyPi](https://pypi.org/project/openstudio/) for further instructions on how to install. Users can also visit the test channel at https://test.pypi.org/project/openstudio/ to install development bindings.

## New Features, Major Fixes and API-breaking changes
* [#4560](https://github.com/NREL/OpenStudio/pull/4560),[#4540](https://github.com/NREL/OpenStudio/pull/4540)  Update EnergyPlus to v22.1.0
* [#4550](https://github.com/NREL/OpenStudio/pull/4550) Addresses #4403, add Sql helper methods to retrieve U-factors, SHGC, or VT for glazing systems
* [#4456](https://github.com/NREL/OpenStudio/pull/4493) Fix #4456 - Improve performance of OpenStudio::UnzipFile::extractAllFiles
* [#4424](https://github.com/NREL/OpenStudio/pull/4424) Fixes #4361 OpenStudio API-reported surface "netArea" (when dealing with Frame & Divider objects) 
* [#4502](https://github.com/NREL/OpenStudio/pull/4502) #4496 - Add gems cbor and msgpack and allow using a package from an openstudio-gems PR 
* [#4497](https://github.com/NREL/OpenStudio/pull/4497) Addresses #4495, ElectricLoadCenterDistribution FT has incomplete charge/discharge logic
* [#4494](https://github.com/NREL/OpenStudio/pull/4494) Addresses #4483, OS:Coil:Heating:DX:MultiSpeed:StageData has no Name field 
* [#4499](https://github.com/NREL/OpenStudio/pull/4499) Addresses #4410, wrap AirflowNetwork:MultiZone:SpecifiedFlowRate 
* [#4510](https://github.com/NREL/OpenStudio/pull/4510) Addresses #4509, bad nodes created for AirLoopHVAC:UnitarySystem with only cooling coil and supplemental heating coil 
* [#4508](https://github.com/NREL/OpenStudio/pull/4508) Addresses #4469, method to Translate ScheduleYear/ScheduleWeek to ScheduleRuleset
* [#4525](https://github.com/NREL/OpenStudio/pull/4525) Fixes #4387 gltf changes as per PR [4520](https://github.com/NREL/OpenStudio/pull/4520) comments/suggestions  
* [#4485](https://github.com/NREL/OpenStudio/pull/4485) Fixes #4438 gbXML export order is not reproducible 
* [#4444](https://github.com/NREL/OpenStudio/pull/4444) Fixes #4375 GBXML Import reverses floors/ceilings normals and surface types 
* [#4527](https://github.com/NREL/OpenStudio/pull/4527) Fixes #4372 intersection issue found using create bar 
* [#4535](https://github.com/NREL/OpenStudio/pull/4535) Fixes #4533 - Wrap SurfaceProperty:LocalEnvironment and SurfaceProperty:SurroundingSurfaces
* [#4513](https://github.com/NREL/OpenStudio/pull/4513) Addresses #4457, support gbXML translation where user-input <Name> is different from the id
* [#4534](https://github.com/NREL/OpenStudio/pull/4534) Addresses #4311, add water heaters to supply side of different loops
* [#4528](https://github.com/NREL/OpenStudio/pull/4528) Cooling tower fixes and tests
* [#4520](https://github.com/NREL/OpenStudio/pull/4520) Adds exports functionality to a use web standard glTF to replace deprecated three.js JSON format
* [#4548](https://github.com/NREL/OpenStudio/pull/4548) Addresses #4531, breaking changes
- CoolingTower:*Speed
  - `evaporationLossMode` (`boost::optional<std::string>` to `std::string`)
  - `blowdownCalculationMode` (`boost::optional<std::string>` to `std::string`)
  - `cellControl` for CoolingTower:VariableSpeed
- PlantLoop
  - `commonPipeSimulation`  (`boost::optional<std::string> to `std::string`)
  - `isCommonPipeSimulationDefaulted`
* [#4505](https://github.com/NREL/OpenStudio/pull/4505) Addresses #4477, improve handling of invalid values passed to SDK by adding optional switch for turning off idd validity checking.
* [#4504](https://github.com/NREL/OpenStudio/pull/4504) Fix #2941 - Add option to show workflow run stdout + style + timings

## Minor changes and bug fixes
* [#4487](https://github.com/NREL/OpenStudio/pull/4487) Add operator< for BCLComponent and BCLMeasure
* [#4482](https://github.com/NREL/OpenStudio/pull/4482) Fix #4481 - add missing `\ip-units W` to the OpenStudio.idd
* [#4491](https://github.com/NREL/OpenStudio/pull/4491) Fix #4490 - Expose RoofGeometry helpers to Swig bindings
* [#4498](https://github.com/NREL/OpenStudio/pull/4498) Bump to 3.3.1 and add default VT
* [#4474](https://github.com/NREL/OpenStudio/pull/4474) Better string representation of Matrix and Transformation in the bindings
* [#4475](https://github.com/NREL/OpenStudio/pull/4475) Add a method on the FT to get known ft options 
* [#4512](https://github.com/NREL/OpenStudio/pull/4512) Update copyrights 2022
* [#4537](https://github.com/NREL/OpenStudio/pull/4537) Review PR #4534 (for issue #4311) - Proposed changes
* [#4553](https://github.com/NREL/OpenStudio/pull/4553) HotFix #4550 - Remove getExteriorFenestrationValue from public API
* [#4555](https://github.com/NREL/OpenStudio/pull/4555) Adjustments to WindowPropertyFrameAndDivider and new model gtest
* [#4562](https://github.com/NREL/OpenStudio/pull/4562) Update idfs to V22.1.0 by @jmarrec in 
* [#4564](https://github.com/NREL/OpenStudio/pull/4564) update openstudio gems for standards 0.2.16 
* [#4518](https://github.com/NREL/OpenStudio/pull/4518) Bump openstudio-gems, conan-openstudio-ruby and add support for Apple M1
* [#4558](https://github.com/NREL/OpenStudio/pull/4558) Update macOS M1 (arm64) dependencies

**Developer changes**:
* [OpenStudio-benchmarks](https://github.com/NREL/OpenStudio-benchmarks) - Added additional performance benchmark tests to improve SDK performance. 


## OpenStudio New Contributors
* @Brijendra21 made their first contribution in https://github.com/NREL/OpenStudio/pull/4525


### OpenStudio Standards v0.2.16

* [#1229](https://github.com/NREL/openstudio-standards/pull/1229) Fix autosized minimum OA flow rate for OA controllers and ERV SAT control 
* [#1231](https://github.com/NREL/openstudio-standards/pull/1231) Fixed bug adding duplicate water heater object when adding a heat pump water heater 
* [#1248](https://github.com/NREL/openstudio-standards/pull/1248) Update deprecated methods from OS 3.2 
* [#1249](https://github.com/NREL/openstudio-standards/pull/1249) Fix ERV lookup for 90.1-2019 
* [#1250](https://github.com/NREL/openstudio-standards/pull/1250) Fix high-rise apartment water heater ambient temperature 
* [#1242](https://github.com/NREL/openstudio-standards/pull/1242) Enable autosizing for HPWHs 
* [#1244](https://github.com/NREL/openstudio-standards/pull/1244) Fix check for zone unitary system fuel and fan power lookup 
* [#1253](https://github.com/NREL/openstudio-standards/pull/1253) Fix skylight requirements for 90.1-2016, 90.1-2019 
* [#1255](https://github.com/NREL/openstudio-standards/pull/1255) Implement new construction search method to accommodate wwr specific lookups
* [#1259](https://github.com/NREL/openstudio-standards/pull/1259) Update ground construction properties to use FC factor constructions for non-prototype uses 
* [#1257](https://github.com/NREL/openstudio-standards/pull/1257) Adjust climate zone lookup in construction search to account certain sub-climate zone variations 
* [#1263](https://github.com/NREL/openstudio-standards/pull/1263) Add U-values for pre 90.1-2004 attic roofs tagged as residential
* [#1268](https://github.com/NREL/openstudio-standards/pull/1268) Update deprecated zone ventilation method 
* [#1270](https://github.com/NREL/openstudio-standards/pull/1270) Fix non-rectangular sky-light in primary school prototype 
* [#1271](https://github.com/NREL/openstudio-standards/pull/1271) Remove fluid cooler artifact from geometry templates 
* [#1272](https://github.com/NREL/openstudio-standards/pull/1272) Add guard clause in the efficiency lookup in case coil has no capacity 
* [#1276](https://github.com/NREL/openstudio-standards/pull/1276) Updated college building occupancy schedules 
* [#1281](https://github.com/NREL/openstudio-standards/pull/1281) Correct design outdoor air requirements for some building types, which fixes issue #933
* [#1284](https://github.com/NREL/openstudio-standards/pull/1284) Fix #1284 (low U-value constructions) by removing insulation for low U-value constructions
* [#1278](https://github.com/NREL/openstudio-standards/pull/1278) Fix issue #1278 by adding lab fumehood exhaust fan
* [#1288](https://github.com/NREL/openstudio-standards/pull/1288) Expose occupancy hours for radiant slab controls
* [#1291](https://github.com/NREL/openstudio-standards/pull/1291) Fix issue #1226 by also removing AirLoopHVACDedicatedOutdoorAirSystem objects when removing all HVAC systems
* [#1292](https://github.com/NREL/openstudio-standards/pull/1292) Fix issue #1100 by adding support for propane heating systems
* [#1293](https://github.com/NREL/openstudio-standards/pull/1293) Fix issue #1221 by coordinating inputs to hot water boilers
* [#1294](https://github.com/NREL/openstudio-standards/pull/1294) Fix issue #170 by explicitly setting the standards construction type for exterior doors
* [#1295](https://github.com/NREL/openstudio-standards/pull/1295) Fix issue #1295 by using longstanding ruby syntax
* [#1304](https://github.com/NREL/openstudio-standards/pull/1304) Fix issue #1119 (duplicate constant day schedules) by using a tolerance instead of value check for constant day schedules
* [#1305](https://github.com/NREL/openstudio-standards/pull/1305) Enable and fix testing for college and courthouse prototypes #1302
* [1306](https://github.com/NREL/openstudio-standards/pull/1307) Fix issue #1306 by setting the correct arguments when adding psz-ac systems to avoid an empty hot water loop


### OpenStudio Server v3.4.0

* Update to 3.4.0 OpenStudio
* [#651](https://github.com/NREL/OpenStudio-server/pull/651) Update rails and puma for security patches
* [#642](https://github.com/NREL/OpenStudio-server/pull/642) pass in allow_disk_usage to mongo  in 
* [#646](https://github.com/NREL/OpenStudio-server/pull/646) Remove references to travis in documentation and fix badges
* [#647](https://github.com/NREL/OpenStudio-server/pull/647) Update rails for security patches

## Issue Statistics Since Previous Release

[35 Closed Issues](https://github.com/NREL/OpenStudio/issues?q=is%3Aissue+closed%3A%3E2021-11-05+)
[39 New Issues](https://github.com/NREL/OpenStudio/issues?q=is%3Aissue+created%3A%3E2021-11-05)
