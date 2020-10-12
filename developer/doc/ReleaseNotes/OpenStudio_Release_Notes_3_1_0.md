# OpenStudio Version 3.1.0

_Release Notes - 10/26/2020_

These release notes describe version 3.1.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Systems, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog

# Overview
As of April, 2020, development and distribution of the OpenStudioApplication has been moved to its own [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) repository. OpenStudio SDK is still actively developed and distributed by NREL and is released two times per year, in April and October. 

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

OpenStudio 3.1.0 is supported on 64-bit Windows 7 – 10, OS X 10.14 – 10.15, and Ubuntu 18.04.

OpenStudio 3.1.0 supports [EnergyPlus Release 9.4](https://github.com/NREL/EnergyPlus/releases/tag/v9.4.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio 3.1.0.

OpenStudio 3.1.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio 3.1.0.

OpenStudio 3.x now uses **Ruby 2.5.5** as the supported version (2.2.4 previously).

As usual, you can refer to the **[OpenStudio Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2019 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply rerun the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user- documentation/help/troubleshooting/.

# OpenStudio SDK: Changelog

The 3.1.0 is a major release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2020-04-28..2020-10-15+).


## New Features, Major Fixes and API-breaking changes

A number of API-breaking changes have been implemented in OpenStudio 3.1.0:

* [#3960](https://github.com/NREL/OpenStudio/pull/3960) - Added support for building the C# bindings via `dotnet` CLI, including on Unix platforms.
* [#3959](https://github.com/NREL/OpenStudio/pull/3959) - Also included some improvements in the generated C# bindings by reducing build warnings and properly exposing some types via SWIG
    * `ScheduleTypeKey` (which is normally only use by OpenStudio internals in ScheduleTypeRegistry checks) previously mapped to a `std::pair<std::string, std::string>` which was SWIGed in ruby as an Array of strings of size two, but improperly exposed in C#. It now uses a dedicated helper class with two methods `ScheduleTypeKey::className()` and `ScheduleTypeKey::scheduleDisplayName()`
* [#4050](https://github.com/NREL/OpenStudio/pull/4050) - Fix #3921 - some children of RefrigerationSystem can be added several times and produce a fatal when System is removed.
    * `RefrigerationSystem` will now be enforcing unicity for all children. This was already the case for a few of the child objects such as Case and WalkIns, it is now the case for all objects (`RefrigerationCondenserCascade`, `RefrigerationSubcoolerMechanical` and `RefrigerationSubcoolerLiquidSuction`, etc.). What this means is that setter methods (or add methods in case of a list) will remove the child for any current RefrigerationSystem it is on first.
* [#4066](https://github.com/NREL/OpenStudio/pull/4066) - Multiple shading controls referenced by a single subsurface
    * `SubSurface` was historically the one referencing the `ShadingControl` object. Now it's a many-to-many relationship where `ShadingControl` has an extensible 'Sub Surface Name' field, and multiple `ShadingControl` objects can reference the same `SubSurface`. This is trickling down from a change introduced in EnergyPlus version 9.4, and specifically in PR [NREL/EnergyPlus#8196](https://github.com/NREL/EnergyPlus/pull/8196)
    * Methods in `SubSurface` have been deprecated but are kept for backward compatibility. They will be removed in a future version of OpenStudio:
        * `shadingControl()`: prefer `shadingControls()`
        * `setShadingControl(ShadingControl&)`: use `addShadingControl(SubSurface&)`, `addShadingControls(std::vector<SubSurface>&)` or `setShadingControls(std::vector<SubSurface>&)`
        * `resetShadingControl()`: use `removeAllShadingControls()` instead
    * All Shading Control Type values should now be supported. Refer to issue [#4074](https://github.com/NREL/OpenStudio/issues/4074) for more information
    * Fields 'Glare Control Is Active', 'Type of Slat Angle Control for Blinds', 'Slat Angle Schedule Name', 'Setpoint2', and 'Multiple Surface Control Type' are now implemented as well
* [#4026](https://github.com/NREL/OpenStudio/pull/4026) - Fix OpenStudio Windows crashed during save attempt
* [#4028](https://github.com/NREL/OpenStudio/pull/4028) - Fixes [#3835](https://github.com/NREL/OpenStudio/issues/3835) Decrease overall load time of CLI and lazy load OpenStudio ruby bindings when possible.
* [#4101](https://github.com/NREL/OpenStudio/pull/4101) - Update EnergyPlus v940 official  


## Minor changes:

* [#3966](https://github.com/NREL/OpenStudio/pull/3966) - Closes [#3945](https://github.com/NREL/OpenStudio/issues/3945) Support the EnergyPlus object SurfaceControl:MovableInsulation. 
* [#3961](https://github.com/NREL/OpenStudio/pull/3963) - Update cmake to use included FindPython method and fix zlib's crypt.h
* [#3970](https://github.com/NREL/OpenStudio/pull/3970) - Update conan dependency  fmt/6.2.0 and use option to define FMT_HEADER_ONLY
* [#3979](#https://github.com/NREL/OpenStudio/pull/3979), [#4004](https://github.com/NREL/OpenStudio/pull/4004), [#4071](https://github.com/NREL/OpenStudio/pull/4071) - Update openstudio-gems to include openstudio-extension v0.3.0, openstudio-workflow v2.1.0, openstudio-standards v0.2.12, openstudio_measure_tester v0.2.3
* [#3987](https://github.com/NREL/OpenStudio/pull/3987) - Closes [#3907](https://github.com/NREL/OpenStudio/issues/3907) - Support new object Coil:Cooling:DX, Coil:Cooling:DX:CurveFit:Performance, Coil:Cooling:DX:CurveFit:OperatingMode, Coil:Cooling:DX:CurveFit:Speed
* [#4009](https://github.com/NREL/OpenStudio/pull/4009) - Closes [#4006](https://github.com/NREL/OpenStudio/issues/4006) - Add SwimmingPool:Indoor to SDK 
* [#4015](https://github.com/NREL/OpenStudio/pull/4015),[#4047](https://github.com/NREL/OpenStudio/pull/4047)- Updates the conan dependencies to try and pin version using hash.  
* [#4027](https://github.com/NREL/OpenStudio/pull/4027) - Include 64/32 bit universal nuget package in cmake build process.
* [#4029](https://github.com/NREL/OpenStudio/pull/4029) - Closes [#3832](https://github.com/NREL/OpenStudio/issues/3832) - Allow for silent install on windows
* [#4055](https://github.com/NREL/OpenStudio/pull/4055) - Closes [#4054](https://github.com/NREL/OpenStudio/issues/4054) - Add latitude, longitude, elevation accessors to FloorplanJS
* [#4059](https://github.com/NREL/OpenStudio/pull/4059) - Add new OutputControl:Files object to SDK
* [#4068](https://github.com/NREL/OpenStudio/pull/4068) - Closes [#4067](https://github.com/NREL/OpenStudio/issues/4067) - Remove the ENV ruby bundle variables so the cli does not use them. 
* [#4098](https://github.com/NREL/OpenStudio/pull/4098) - Remove setter/getter for OutputEND (since it is required by the openstudio-workflow-gem).



## Minor Bug Fixes


* [#3957](https://github.com/NREL/OpenStudio/pull/3969) - Fix for ruby bundle changes. 
* [#3971](https://github.com/NREL/OpenStudio/pull/3971) - Fixes [#3943](https://github.com/NREL/OpenStudio/issues/3943) ThreeJS should handle ConstructionAirBoundary 
* [#3973](#https://github.com/NREL/OpenStudio/pull/3973) - Fixes [#3972](https://github.com/NREL/OpenStudio/issues/3972) - ScheduleRuleset remove winterDD/summerDD/holiday ScheduleDay in reset. 
* [#3978](https://github.com/NREL/OpenStudio/pull/3978) - Fixes [#3976](https://github.com/NREL/OpenStudio/issues/3976) - Add 'Minimum Outdoor Dry-Bulb Temperature for Compressor Operation' to all Coil:Cooling:DX:XXX objects
* [#3980](https://github.com/NREL/OpenStudio/pull/3980) - Fixes [#2033](https://github.com/NREL/OpenStudio/issues/2033) - Properly connected the secondary/induced air node for ATUs
* [#3981](https://github.com/NREL/OpenStudio/pull/3981) - Fixes [#2977](https://github.com/NREL/OpenStudio/issues/2977) -  Make VAVReheat Method consistent with IDD and other objects
* [#3983](https://github.com/NREL/OpenStudio/pull/3983) - Fixes [#1058](https://github.com/NREL/OpenStudio/issues/1058) - Warn only if the file extension isn't OSM, OSC, IDF, IMF, or DDY
* [#3985](https://github.com/NREL/OpenStudio/pull/3985) Fixes [#3926](https://github.com/NREL/OpenStudio/issues/3926), Fixes [#3984](https://github.com/NREL/OpenStudio/issues/3984) - handle multiple AirLoopHVACs and assign the DSOA to all ControllerMechanicalVentilation objects. AirLoopHVAC translates the AirLoopHVACOutdoorAirSystem, which in turns translates the ControllerOutdoorAir.
* [#3986](https://github.com/NREL/OpenStudio/pull/3986) - Fixes [#2797](https://github.com/NREL/OpenStudio/issues/2797) - Do not translate a ZoneHVACLowTempRadiant object if it doesn't have any surfaces with Internal Source
* [#3988](https://github.com/NREL/OpenStudio/pull/3988) - Fix bug in GenerateClass.rb
* [#3989](https://github.com/NREL/OpenStudio/pull/3989) - Fixes [#3076](https://github.com/NREL/OpenStudio/issues/3076) - Make openstudio::Vector "Enumerable" in ruby
* [#3993](https://github.com/NREL/OpenStudio/pull/3993) - Added logic to rejoin previously triangulated window geometry before calling computeTriangulation on the base surface. Fixed several bugs in joinAll and translation to ThreeJS format.
* [#3995](https://github.com/NREL/OpenStudio/pull/3995) - Implements eql? and hash for UUID in Ruby so UUID can be used as a key in a Ruby Hash
* [#3996](https://github.com/NREL/OpenStudio/pull/3996) - Fixes [#3990](https://github.com/NREL/OpenStudio/issues/3990) - Add an API to CoilCoolingDXMultiSpeed to add/remove stages
* [#3998](https://github.com/NREL/OpenStudio/pull/3998) - Fixes [#3997](https://github.com/NREL/OpenStudio/issues/3997) - GbXMLReverseTranslator applies incorrect scaling to windows when unit isn't meter
* [#3999](https://github.com/NREL/OpenStudio/pull/3999) - Removes a warning message that no longer makes sense:
* [#4000](https://github.com/NREL/OpenStudio/pull/4000) - Fixes [#3314](https://github.com/NREL/OpenStudio/issues/3314) - GBXML translate spaces etc even if Facility and/or Building aren't isntantianted 
* [#4002](https://github.com/NREL/OpenStudio/pull/4002) - Fixes [#2100](https://github.com/NREL/OpenStudio/issues/2100) - Remove if defaulted statements in ForwardTranslateThermalZone
* [#4005](https://github.com/NREL/OpenStudio/pull/4005) - Fixes [#4001](https://github.com/NREL/OpenStudio/issues/4001) - gbXML issue on ForwardTranslation - surfaceType attribute written twice for SlabOnGrade
3_bind* to ensure proper escaping of parameters such as Zone Names etc  
* [#4008](https://github.com/NREL/OpenStudio/pull/4008) - Fixes [#1906](https://github.com/NREL/OpenStudio/issues/1906) - Use sqlite
* [#4010](https://github.com/NREL/OpenStudio/pull/4010) - Fixes [#4007](https://github.com/NREL/OpenStudio/issues/4007) - Model objects returned in inconsistent order
* [#4011](https://github.com/NREL/OpenStudio/pull/4011) - Fixes [#1675](https://github.com/NREL/OpenStudio/issues/1675) - ForwardTranslate WaterHeater:Mixed when not on loop but has Peak Use Flow Rate (standalone operation)
* [#4012](https://github.com/NREL/OpenStudio/pull/4012) - Fixes [#2895](https://github.com/NREL/OpenStudio/issues/2895) - Extend the list of acceptable SubSurface types to include Skylight.  
* [#4013](https://github.com/NREL/OpenStudio/pull/4013) - Fixes [32867](https://github.com/NREL/OpenStudio/issues/2867) - Sum of fraction radiant, fraction lost, etc can be > 1 
* [#4021](https://github.com/NREL/OpenStudio/pull/4021) - Fixes [#4014](https://github.com/NREL/OpenStudio/issues/4014) - Extend download time to BCL 
* [#4023](https://github.com/NREL/OpenStudio/pull/4023) - Fixes [#3936](https://github.com/NREL/OpenStudio/issues/3936) - SubSurface Type reset when assigned to surface 
* [#4024](https://github.com/NREL/OpenStudio/pull/4024) Fixes [#3578](https://github.com/NREL/OpenStudio/issues/3578) - Addresses warning message when running measure test 'Cannot find current Workflow Step' 
* [#4030](https://github.com/NREL/OpenStudio/pull/4030) - Fixes [#4030](https://github.com/NREL/OpenStudio/issues/4020) - AvailabilityManagerNightCycle: Ensure ModelObjectLists are handled in clone/remove
* [#4032](https://github.com/NREL/OpenStudio/pull/4032) - Fixes [#4016](https://github.com/NREL/OpenStudio/issues/4016) - Move VT from 2.9.0 to 3.0.0 for new fields at end of the ZoneHVAC:TerminalUnit:VariableRefrigerantFlow
* [#4037](https://github.com/NREL/OpenStudio/pull/4037) - Fixes [#3369](https://github.com/NREL/OpenStudio/issues/3369) - Add missing fields to AvailabilityManager:HybridVentilation
* [#4044](https://github.com/NREL/OpenStudio/pull/4044) - Fixes [#4039](https://github.com/NREL/OpenStudio/issues/4039) - Add field ' Design Return Air Flow Fraction of Supply Air Flow' to AirLoopHVAC
* [#4048](https://github.com/NREL/OpenStudio/pull/4048) - Fixes [#4036](https://github.com/NREL/OpenStudio/issues/4036) - Don't create windows or doors for plenums when translating from FloorspaceJSON
* [#4065](https://github.com/NREL/OpenStudio/pull/4065) - Fixes [#4033](https://github.com/NREL/OpenStudio/issues/4033), Fixes [#4034](https://github.com/NREL/OpenStudio/issues/4034) - Fix AVMList clone between different models
* [#4072](https://github.com/NREL/OpenStudio/pull/4072) - Fixes [#4064](https://github.com/NREL/OpenStudio/issues/4064) - RunPeriodControlSpecialDays are not Forward Translated 
* [#4080](https://github.com/NREL/OpenStudio/pull/4080) - Fixes [#4079](https://github.com/NREL/OpenStudio/issues/4079) -  Install E+ pythonwrapper lib via CMake
* [#4088](https://github.com/NREL/OpenStudio/pull/4088) - hotfix for sql queries
* [#4093](https://github.com/NREL/OpenStudio/pull/4093) - Fixes [#4091](https://github.com/NREL/OpenStudio/issues/4091) - Update FuelType/EndUseFuelType (and Sql) to match E+ 9.4.0
* [#4097](https://github.com/NREL/OpenStudio/pull/4097) - Fixes [#4096](https://github.com/NREL/OpenStudio/issues/4096) - RT error for ZoneControlThermostat
* [#4099](https://github.com/NREL/OpenStudio/pull/4099) - Fixes [#4089](https://github.com/NREL/OpenStudio/issues/4089) - Confirm if idf_output_requests still works in reporting measure template
* [#4104](https://github.com/NREL/OpenStudio/pull/4104) - Fixes [#4102](https://github.com/NREL/OpenStudio/issues/4102) - Fuel Type Renames in EMS Actuator Names 
* [#4155](https://github.com/NREL/OpenStudio/pull/4115) - Fixes [#4109] - Fixes ruby bindings issue on windows when working with Sqlite3 db. 

### OpenStudio Standards v0.2.12

* added missing nonresidential construction properties to the ZE standards
* updated data center model
* changed the location for climate zone 0 and 1 weather files
* use common geometry templates
* ground refactor to use the FC-factor methodology for 90.1 prototype models
* bug fixes to align with the OpenStudio 3.0.1 API using Ruby 2.5
* bug fix to fuel type naming FuelOil#1 -> FuelOil No1 per a recent EnergyPlus change
* added more detailed logging for economizer logic
* switch tests to use Jenkins instead of CircleCI
* updates to central plant models for LargeOffices and Hospitals
* bug fix to schedule type limits
* updates to elevators
* additional support for other types of fluid coolers and cooling towers in the HVAC constructor logic
* added missing refrigerator case lookups
* added transformers to several prototypes
* moved heat recovery lookups to data .json files
* moved economizer lookups to data .json files
* bug fix to DCV logic
* added TallBuilding and SuperTallBuilding prototypes
* added draft College prototype.  While this release includes data for colleges, this only an early draft and should not be used for anything other that early testing or work to improve the college prototype.
* removed old, central data .json files

### OpenStudio Server v3.1.0

* [#555](https://github.com/NREL/OpenStudio-server/pull/555) - Update Ruby gem envs in the openstudio_meta cli  
* [#561](https://github.com/NREL/OpenStudio-server/pull/561) - Fixes [#560](https://github.com/NREL/OpenStudio-server/issues/560) - Fixes unzip method
* [#563](https://github.com/NREL/OpenStudio-server/pull/563) - Add linux support for extracted gems. 
* [#564](https://github.com/NREL/OpenStudio-server/pull/564) - Remove hardcoded openstudio version and sha
* [#566](https://github.com/NREL/OpenStudio-server/pull/566) - Make tmpname deprecate
* [#571](https://github.com/NREL/OpenStudio-server/pull/571) - Make downloading files back to server optional 
* [#602](https://github.com/NREL/OpenStudio-server/pull/602) - Add algorithmic test suites
* [#597](https://github.com/NREL/OpenStudio-server/pull/597) - Save the travis gem package artifacts
* [#599](https://github.com/NREL/OpenStudio-server/pull/599) - Save the windows gem package artifacts
* [#580](https://github.com/NREL/OpenStudio-server/issues/580) - add report capability to PSO 
* [#579](https://github.com/NREL/OpenStudio-server/pull/579) - Update nokogiri
* [#585](https://github.com/NREL/OpenStudio-server/pull/585) - Update docker-compose version
* [#589](https://github.com/NREL/OpenStudio-server/pull/589) - Use most recent rubocop rules



## Issue Statistics Since Previous Release

* 70 new issues were filed since the 3.0.0 release of OpenStudio (not including opened pull requests)

* 143 issues were closed since the 3.0.0 release of OpenStudio (not including closed pull requests).

