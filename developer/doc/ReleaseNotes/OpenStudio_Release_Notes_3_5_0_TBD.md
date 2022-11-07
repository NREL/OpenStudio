# OpenStudio Version 3.5.0 (TDB)

_Release Notes_ -  _TDB_

These release notes describe version 3.5.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Sciences Center, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog

# Overview

As of April 2020, development and distribution of the OpenStudioApplication and the SketchUp plugin have transitioned to the OpenStudio Coalition, who is independently managing and distributing the software through its own [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) repository. The OpenStudio SDK is continuing to be actively developed and distributed by NREL and is released two times per year, through a spring and a fall release.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.5.0**__
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

OpenStudio SDK 3.5.0 is supported on 64-bit Windows 7 – 10, OS X 10.15, and Ubuntu 18.04, 20.04

OpenStudio SDK 3.5.0 supports [EnergyPlus Release 22.1.0](https://github.com/NREL/EnergyPlus/releases/tag/v22.1.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio SDK 3.5.0.

OpenStudio SDK 3.5.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio SDK 3.5.0.

As usual, you can refer to the **[OpenStudio SDK Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2021-2022 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply re-run the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/.

# OpenStudio SDK: Changelog

The 3.5.0 is a minor release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2021-04-27..2022-09-26+).


## Python Bindings

As of OpenStudio SDK 3.2.0, Python bindings are officially supported and distributed through Python Package Index (PyPI). To install, users will need to have Python3 installed along with pip and simply run the following command in a terminal window.

`pip install openstudio==3.5.0`

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
* [#4666](https://github.com/NREL/OpenStudio/pull/4666) - Changes related to availability schedule methods
    * `Coil:Heating:Gas:MultiStage` has an API-breaking change related to its `availabilitySchedule` getter. It is now a required field that returns `Schedule` instead of `boost::optional<Schedule>`. Method `resetAvailabilitySchedule` is also removed.
* [#4701](https://github.com/NREL/OpenStudio/pull/4701) - `ZoneHVACPackagedTerminalAirConditioner` and `ZoneHVACPackagedTerminalHeatPump`
    * `ZoneHVACPackagedTerminalAirConditioner` and `ZoneHVACPackagedTerminalHeatPump` have an API-breaking change related to its `supplyAirFanOperatingModeSchedule` getter. It is now a required field that returns `Schedule` instead of `boost::optional<Schedule>`. Method `resetSupplyAirFanOperatingModeSchedule` is also removed.
        * It is set to `alwaysOnDiscreteSchedule` (=Constant) in the Constructor if you provide a `FanConstantVolume` (This is **required** by E+)
        * It is set to `alwaysOffDiscreteSchedule` (=Cycling) in the Constructor if you provide any other fan types (E+ treats a blank schedule as always off)
    * There are unusual `VersionTranslator` Rules for Packaged Systems (PTAC or PTHP) that use a `FanConstantVolume` and that do not have a `Supply Air Fan Operating Mode Schedule`. In 22.1.0 this would effectively, and mistakenly, function as a cycling fan, but this is now disallowed in E+ 22.2.0. In order to retain a similar functionality and energy usage, the `FanConstantVolume` will be replaced by a `FanSystemModel` with an Always Off Schedule (=cycling fan, similar to a `Fan:OnOff`), mapping inputs such as pressure rise and efficiency appropriately.


## Minor changes and bug fixes

* [4580](https://github.com/NREL/OpenStudio/pull/4580) Add color for Foundation boundary condition
* [4568](https://github.com/NREL/OpenStudio/pull/4568)Installer adjustments for clang 13.1.6+, and some mac arm64 adjustments by 
* [4585](https://github.com/NREL/OpenStudio/pull/4585) add libintl.8.dylib to mac15 x86 for e+
* [4589](https://github.com/NREL/OpenStudio/pull/4589) Switch to E+ Space FT by default
* [4571](https://github.com/NREL/OpenStudio/pull/4571) Issue 4387 gl tf extras
* [4579](https://github.com/NREL/OpenStudio/pull/4579) Add Volume field to Space
* [4593]( https://github.com/NREL/OpenStudio/pull/4593) Fix ZoneInfiltration:DesignFlowRate FT when Space FT enabled
* [4597](https://github.com/NREL/OpenStudio/pull/4597) Hotfix GLTF extras broke running tests in Debug
* [4594](https://github.com/NREL/OpenStudio/pull/4594) Bump OS ruby to use fPIC and zlib/minizip to 1.2.12 to match
* [4590](https://github.com/NREL/OpenStudio/pull/4590) Correct typo: Newell, not Newall
* [4316](https://github.com/NREL/OpenStudio/pull/4316) Addresses #2610, insertComponent can create duplicate unique model objects
* [4604](https://github.com/NREL/OpenStudio/pull/4604) Cached getters for all unique model objects
* [4605](https://github.com/NREL/OpenStudio/pull/4605) Gltf Refactor
* [4612](https://github.com/NREL/OpenStudio/pull/4612) Fix build error due to GLTF refactor on Windows
* [4596](https://github.com/NREL/OpenStudio/pull/4596) Bump conan dependencies (which will allow MSVC 2022 and C++20) and clang 13.1+ fixes
* [4614](https://github.com/NREL/OpenStudio/pull/4614) Support Ubuntu 18.04 and Centos7
* [4484](https://github.com/NREL/OpenStudio/pull/4484) Enable C++20
* [4587](https://github.com/NREL/OpenStudio/pull/4587) XML schema validation 
* [4619](https://github.com/NREL/OpenStudio/pull/4619) Fix #4543 - E+ 22.1.0: Wrap SetpointManager:SystemNodeReset:Temperature and SetpointManager:SystemNodeReset:Humidity by @jmarrec in 
* [4600](https://github.com/NREL/OpenStudio/pull/4600) Python plugin by @joseph-robertson in 
* [4610](https://github.com/NREL/OpenStudio/pull/4610) Addresses #4538, wrap phase change material properties by @joseph-robertson in 
* [4622](https://github.com/NREL/OpenStudio/pull/4622) Addresses #4620, harmonize (remove?) max. material thickness limit by @joseph-robertson in 
* [4616](https://github.com/NREL/OpenStudio/pull/4616) Addresses #4611, allow non-Quadratic curves for the EIR-f-PLR for the Chiller:Electric:EIR object by @joseph-robertson in 
* [4625](https://github.com/NREL/OpenStudio/pull/4625) Addresses #4615, wrap OutputControl:Table:Style and Output:SQLite by @joseph-robertson in 
* [4627](https://github.com/NREL/OpenStudio/pull/4627) Fix #4547 - FT always warns about missing Design Specification Outdoor Air for AirTerminalSingleDuctInletSideMixer by @jmarrec in 
* [4640](https://github.com/NREL/OpenStudio/pull/4640) Fix python bindings github workflows with C++20 by @jmarrec in 
* [4626](https://github.com/NREL/OpenStudio/pull/4626) Fix #4601 - add some useful log messages when requested WWR is rejected by @jmarrec in 
* [4592](https://github.com/NREL/OpenStudio/pull/4592) Fix#127 - Fix volume calculation to match EnergyPlus by @jmarrec in 
* [4623](https://github.com/NREL/OpenStudio/pull/4623) FloorspaceJS to OpenStudio reverse translator. FIrst implementation p… by @ggartside in 
* [4577](https://github.com/NREL/OpenStudio/pull/4577) gbXML translation fixes and enhancements by @joseph-robertson in 
* [4621](https://github.com/NREL/OpenStudio/pull/4621) #4551 - Add minimum/maximum values to numeric OSArguments and use it in validateUserArgument by @jmarrec in 
* [4643](https://github.com/NREL/OpenStudio/pull/4643) Update docs for setThermalConductance and setThermalResistance changing thickness by @joseph-robertson in 
* [4650](https://github.com/NREL/OpenStudio/pull/4650) Enable centos7 for c++20 by @tijcolem in
* [4642](https://github.com/NREL/OpenStudio/pull/4642) Addresses #4575, API change for EvaporativeCoolerFluid:SingleSpeed by @joseph-robertson in 
* [4629](https://github.com/NREL/OpenStudio/pull/4629) Addresses #3666, CoilWaterHeaterDesuperheater: Issue Error in ForwardTranslator if Heat Reclaim Efficiency is out of bounds by @joseph-robertson in 
* [3912](https://github.com/NREL/OpenStudio/pull/3912) Add consistently failing test for warnings/errorsa and document by @lefticus in 
* [4644](https://github.com/NREL/OpenStudio/pull/4644) Addresses #4575, API change for Coil:Cooling:DX:SingleSpeed and Coil:Cooling:DX:TwoSpeed by @joseph-robertson in 
* [4632](https://github.com/NREL/OpenStudio/pull/4632) Remove deprecated methods for AirWallMaterial, Node, SizingSystem, ZoneAirMassFlowConservation by @joseph-robertson in 
* [4669]( https://github.com/NREL/OpenStudio/pull/4669) #4668 - Wrap E+ 22.2 new People fields by @jmarrec in
* [4287](https://github.com/NREL/OpenStudio/pull/4287) Build and test CSharp bindings for Linux/Mac/Windows by @MingboPeng in 
* [4652](https://github.com/NREL/OpenStudio/pull/4652) Addresses #4647, wrap the E+ Table:Lookup, Table:IndependentVariableList, and Table:IndependentVariable objects by @joseph-robertson in 
* [4679](https://github.com/NREL/OpenStudio/pull/4679) Remove cruft by @jmarrec in 
* [4672](https://github.com/NREL/OpenStudio/pull/4672) Fix #4645 - Update FT for space-level infiltration/ventilation objects (E+ 22.2.0-IOFreeze) by @jmarrec in 
* [4676](https://github.com/NREL/OpenStudio/pull/4676) Fix #4648 #4654 - SurfaceProperty:GroundSurfaces and SurfaceProperty:IncidentSolarMultiplier by @jmarrec in
* [4681](https://github.com/NREL/OpenStudio/pull/4681) Fix glass U factor sql error by @joseph-robertson in 
* [4661](https://github.com/NREL/OpenStudio/pull/4661) Volume, CeilingHeight, FloorArea for Space object by @joseph-robertson in 
* [4660](https://github.com/NREL/OpenStudio/pull/4660) Fix #4120 - set boost's visibility to global/default instead of hidden by @jmarrec in 
* [4680](https://github.com/NREL/OpenStudio/pull/4680) update ruby gems including the new tbd gem by @tijcolem in 
* [4686]() Addresses #4599, documentation for isGroundSurface() is missing an outside boundary condition by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4686
* [4665](https://github.com/NREL/OpenStudio/pull/4665) New 2017 and 2023 flow rate fields by @joseph-robertson in 
* [4671](https://github.com/NREL/OpenStudio/pull/4671) New Coil:*:WaterToAirHeatPump:EquationFit fields by @joseph-robertson in 
* [4677](https://github.com/NREL/OpenStudio/pull/4677) Fix #4675 - E+ 22.2.0 - Sizing:Zone has new fields by @jmarrec in 
* [4689](https://github.com/NREL/OpenStudio/pull/4689) 4653 python engine bcl by @jmarrec in 
* [4691](https://github.com/NREL/OpenStudio/pull/4691) Followup to #4575 by @joseph-robertson in 
* [4662](https://github.com/NREL/OpenStudio/pull/4662) Support CoilCoolingDX on ZoneHVAC, CoilSystem, Desuperheater by @joseph-robertson in 
* [4666](https://github.com/NREL/OpenStudio/pull/4666) Addresses #4646, wrap E+ 22.2 new supplemental heating coil type for AirLoopHVAC:UnitarySystem object by @joseph-robertson in 
* [4694](https://github.com/NREL/OpenStudio/pull/4694) 4638 python ruby workflow by @jmarrec in 
* [4687](https://github.com/NREL/OpenStudio/pull/4687) Fix #4656 - E+ 22.2.0: Wrap Chiller:Electric:ASHRAE205 by @jmarrec in 
* [4698](https://github.com/NREL/OpenStudio/pull/4698) Update EnergyPlus to v22.2.0 by @joseph-robertson in
* [4697](https://github.com/NREL/OpenStudio/pull/4697) Volume, CeilingHeight, FloorArea for Space object, try 2 by @joseph-robertson in 
* [4658](https://github.com/NREL/OpenStudio/pull/4658) Update to v22.2.0 by @joseph-robertson in 
* [4683](https://github.com/NREL/OpenStudio/pull/4683) #4663 #4664 - Fix CoilCoolingDXMultiSpeed::clone and extend testing for AirLoopHVAC::clone by @jmarrec in 
* [4685](https://github.com/NREL/OpenStudio/pull/4685) Address #4630, wrap Output:Schedules and Output:Constructions objects by @joseph-robertson in
* [4702](https://github.com/NREL/OpenStudio/pull/4702) Mod to Python Engine by @jmarrec in 
* [4700](https://github.com/NREL/OpenStudio/pull/4700) Fix #4673 - Allow setting ZoneMixing objects at Space level by @jmarrec in 
* [4701](https://github.com/NREL/OpenStudio/pull/4701) Fix #4699 - E+ 22.2: ZoneHVAC:PackagedTerminal AirConditionner/HeatPump requires a supply air fan operating mode schedule by @jmarrec in 
* [4710](https://github.com/NREL/OpenStudio/pull/4710) update gems for standard 0.2.17.rc1 by @tijcolem in 
* [4674](https://github.com/NREL/OpenStudio/pull/4674) 4641 ruby shared lib by @kbenne in 
* [4688](https://github.com/NREL/OpenStudio/pull/4688) 4653 python engine by @kbenne in
* [4712](https://github.com/NREL/OpenStudio/pull/4712) Fix #4659 - Filesystem fixups by @jmarrec in 
* [4713](https://github.com/NREL/OpenStudio/pull/4713) Deprecated methods csv by @jmarrec in 


**Full Changelog**: https://github.com/NREL/OpenStudio/compare/v3.4.0-vcomp...v3.5.0-rc1

**Developer changes**:

* [OpenStudio-benchmarks](https://github.com/NREL/OpenStudio-benchmarks) - Added additional performance benchmark tests to improve SDK performance.

## New Contributors
* [4287](https://github.com/NREL/OpenStudio/pull/4287) @MingboPeng made their first contribution in https://github.com/NREL/OpenStudio/pull/4287


### OpenStudio Standards v0.3.0

Bug fixes include:

* [1310](https://github.com/NREL/OpenStudio/pull/1310) Fix issue #1309 Lookup template for model_find_target_eui from model
* [1316](https://github.com/NREL/OpenStudio/pull/1316) add missing small hotel lighting schedules in ComStock data
* [1320](https://github.com/NREL/OpenStudio/pull/1320) adjust lighting values and schedules in ComStock data
* [1342](https://github.com/NREL/OpenStudio/pull/1342) add missing schedules for elevators in ComStock data
* [1346](https://github.com/NREL/OpenStudio/pull/1346) Fix issue #1345 Air-cooled chiller efficiency lookup returning multiple results because there is no default for condenser type
* [1379](https://github.com/NREL/OpenStudio/pull/1379) NRCan changes
* [1383](https://github.com/NREL/OpenStudio/pull/1383) Catch exceptions for some OS prototypes
* [1384](https://github.com/NREL/OpenStudio/pull/1384) Include global scorecard for openstudio-standards prototypes
* [1393](https://github.com/NREL/OpenStudio/pull/1393) Fix issues generated from E+ and OS API changes in OS 3.5.0-alpha
* [1394](https://github.com/NREL/OpenStudio/pull/1394) Fix issue #1372 to use VRP enumeratiosn for newer E+ versions and make Evz template-dependent
* [1396](https://github.com/NREL/OpenStudio/pull/1396) Fix issue #1338 corect heating setpoint schedule method for low temperature radiant electric objects
* [1397](https://github.com/NREL/OpenStudio/pull/1397) Fix issue #1343 check that wall area is non-zero for wwr calculation and error gracefully
* [1399](https://github.com/NREL/OpenStudio/pull/1399) Fix issue #1254 create temperature schedule limits object for heat pup water heater when no thermal zone specified
* [1400](https://github.com/NREL/OpenStudio/pull/1400) Fix issue #1339 give a path length warning when loading weather files
* [1401](https://github.com/NREL/OpenStudio/pull/1401) Fix issue #1151 missing laboratory exterior lighting and entryway data
* [1402](https://github.com/NREL/OpenStudio/pull/1402) Fix issue #1352 correct UA units in info messages
* [1403](https://github.com/NREL/OpenStudio/pull/1403) Fix issue #1382 small data centers failing on cold climates (CZ6-8)


PNNL extended and expanded the Create Baseline Building measure to include the 2019 version of ASHRAE 90.1 Appendix G and generate a more complete baseline model. This was a significant effort thanks to contributions from Doug Maddox, Jeremy Lerond, Weili Xu, Jian Zhang, Yunyang Ye, Xuechen (Jerry) Lei, and Juan Gonzalez Matamoros.

 

Appendix G PRM release notes:

Pull Request | Description

* [809](https://github.com/NREL/OpenStudio/pull/809) Update WWR adjustment for Appendix G stable baseline
* [823](https://github.com/NREL/OpenStudio/pull/823) Constructions for Appendix G stable baseline
* [837](https://github.com/NREL/OpenStudio/pull/837) Lighting power for Appendix G stable baseline
* [863](https://github.com/NREL/OpenStudio/pull/863) Lighting occupancy sensor for Appendix G stable baseline
* [921](https://github.com/NREL/OpenStudio/pull/921) Infiltration calculations for Appendix G stable baseline
* [923](https://github.com/NREL/OpenStudio/pull/923) Baseline system mapping and assignment for Appendix G stable baseline
* [949](https://github.com/NREL/OpenStudio/pull/949) WWR determination based on actual space conditioning type
* [967](https://github.com/NREL/OpenStudio/pull/967) Add system 11 and related SAT controls
* [1003](https://github.com/NREL/OpenStudio/pull/1003) HVAC sizing for Appendix G stable baseline
* [1086](https://github.com/NREL/OpenStudio/pull/1086) Set number of chillers and number of boilers for Appendix G stable baseline
* [1230](https://github.com/NREL/OpenStudio/pull/1230) CHW and HW supply temperature reset for Appendix G stable baseline
* [1238](https://github.com/NREL/OpenStudio/pull/1238) Add preheat coil for Appendix G stable baseline
* [1266](https://github.com/NREL/OpenStudio/pull/1266) Handle multiple building area types for Appendix G stable baseline
* [1297](https://github.com/NREL/OpenStudio/pull/1297) Update minimum flow setpoints for VAV terminals for Appendix G stable baseline
* [1300](https://github.com/NREL/OpenStudio/pull/1300) Update parallel PIU fan control for Appendix G stable baseline
* [1301](https://github.com/NREL/OpenStudio/pull/) Check vav part load curve for Appendix G stable baseline
* [1308](https://github.com/NREL/OpenStudio/pull/1308) Economizer exception for Appendix G stable baseline
* [1312](https://github.com/NREL/OpenStudio/pull/1312) Set lighting space types from user data for Appendix G stable baseline
* [1314](https://github.com/NREL/OpenStudio/pull/1314) Remove piping losses from Appendix G stable baseline
* [1315](https://github.com/NREL/OpenStudio/pull/1315) Fan power adjustments for Appendix G stable baseline
* [1319](https://github.com/NREL/OpenStudio/pull/1319) Set plug load from user data for Appendix G stable baseline
* [1322](https://github.com/NREL/OpenStudio/pull/1322) Building rotation requirements for Appendix G stable baseline
* [1325](https://github.com/NREL/OpenStudio/pull/1325) F/C-factor requirements and unenclosed/unconditioned spaces for Appendix G stable baseline
* [1327](https://github.com/NREL/OpenStudio/pull/1327) Update minimum HVAC efficiency requirements for Appendix G stable baseline
* [1331](https://github.com/NREL/OpenStudio/pull/1331) Assign space conditioning category for supply air plenums
* [1334](https://github.com/NREL/OpenStudio/pull/1334) Check user and baseline model unmet load hours
* [1335](https://github.com/NREL/OpenStudio/pull/1335) Demand controlled ventilation for Appendix G stable baseline
* [1349](https://github.com/NREL/OpenStudio/pull/1349) HVAC controls for Appendix G stable baseline
* [1350](https://github.com/NREL/OpenStudio/pull/1350) Exterior lighting for Appendix G stable baseline
* [1351](https://github.com/NREL/OpenStudio/pull/1351) Window to wall ratio increase methods
* [1354](https://github.com/NREL/OpenStudio/pull/1354) Lighting exceptions for Appendix G stable baseline
* [1371](https://github.com/NREL/OpenStudio/pull/1371) Enthalpy recovery ratio requirements for Appendix G stable baseline
* [1374](https://github.com/NREL/OpenStudio/pull/1374) Configure primary/secondary pumping for Appendix G stable baseline
* [1375](https://github.com/NREL/OpenStudio/pull/1375) Add user data option for number of systems per zone for Appendix G stable baseline
* [1380](https://github.com/NREL/OpenStudio/pull/1380) Baseline elevator power and process loads for Appendix G stable baseline

### OpenStudio Server 3.5.0

* [662](https://github.com/NREL/OpenStudio-server/pull/662) update mongoid queries 
* [664](https://github.com/NREL/OpenStudio-server/pull/664) fix Radar plot 
* [658](https://github.com/NREL/OpenStudio-server/pull/658) fix #539 and add significant digit toggle 
* [665](https://github.com/NREL/OpenStudio-server/pull/665) update plots with display name choices in 
* [670](https://github.com/NREL/OpenStudio-server/pull/670) Ubuntu 20.04
* [668](https://github.com/NREL/OpenStudio-server/pull/668) Sobol and morris fix
* [671](https://github.com/NREL/OpenStudio-server/pull/671) Uo update
* [672](https://github.com/NREL/OpenStudio-server/pull/672) add URBANopt template OSA for single_run 


## Issue Statistics Since Previous Release
