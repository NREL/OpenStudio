# OpenStudio Version 3.2.0

_Release Notes - 05/04/2021_

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

OpenStudio SDK 3.2.0 is supported on 64-bit Windows 7 – 10, OS X 10.14 – 10.15, and Ubuntu 18.04, 20.04

OpenStudio SDK 3.2.0 supports [EnergyPlus Release 9.5](https://github.com/NREL/EnergyPlus/releases/tag/v9.5.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio SDK 3.2.0.

OpenStudio SDK 3.2.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio SDK 3.2.0.

OpenStudio SDK 3.2.0 now uses **Ruby 2.7.2** as the supported version (2.5.5 previously in the 3.x series, and 2.2.4 for 2.x series).
<strong style='color: red;'>TODO:</strong> Probably include a dedicated section?


As usual, you can refer to the **[OpenStudio SDK Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2019 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply rerun the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user-documentation/help/troubleshooting/.

# OpenStudio SDK: Changelog

The 3.2.0 is a major release. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2020-10-16..2021-05-04+).

## New Features, Major Fixes and API-breaking changes

A number of new features and API-breaking changes have been implemented in OpenStudio SDK 3.2.0:

* [#4207](https://github.com/NREL/OpenStudio/pull/4207) - Update to ruby 2.7.2
* [#4271](https://github.com/NREL/OpenStudio/pull/4271) - Update to EnergyPlus v9.5.0
    * Most of the actual changes were done in [#4212](https://github.com/NREL/OpenStudio/pull/4212) - Update to V9.5.0-IOFreeze
* [#4122](https://github.com/NREL/OpenStudio/pull/4122) - Improvements to Python bindings, and produce a PyPi package, see [openstudio on PyPi](https://test.pypi.org/project/openstudio/)
    * <strong style='color: red;'>TODO:</strong> Python: write a better write up. maybe its own section?

* **New objects**:
    * [#4155](https://github.com/NREL/OpenStudio/pull/4155) - Wrap `Humidifier:Stream:Gas` in OS SDK
    * [#4157](https://github.com/NREL/OpenStudio/pull/4157) - Wrap `ZoneHVAC:CoolingPanel:RadiantConvective:Water` in OS SDK
    * [#4159](https://github.com/NREL/OpenStudio/pull/4159) - Wrap `Refrigeration:ComporessorRack` in OS SDK
    * [#4186](https://github.com/NREL/OpenStudio/pull/4186) - Wrap `Chiller:Electric:ReformulatedEIR` in OS SDK
    * [#4192](https://github.com/NREL/OpenStudio/pull/4192) - Wrap `Generator:WindTurbine` in OS SDK
    * [#4193](https://github.com/NREL/OpenStudio/pull/4193) - Wrap `Fan:ComponentModel` in OS SDK
    * [#4194](https://github.com/NREL/OpenStudio/pull/4194) - Wrap `PhotovoltaicPerformance:Sandia` in OS SDK
        * The recommended instantiation is via the factory methods `GeneratorPhotovoltaic::sandia(const Model&)` or `GeneratorPhotovoltaic::fromSandiaDatabase(const Model&, const std::string& sandiaModulePerformanceName)` where `sandiaModulePerformanceName` is one of keys in the now-embedded Sandia database. Please use the `PhotovoltaicPerformanceSandia::sandiaModulePerformanceNames()` static method to lookup the valid names.
    * [#4225](https://github.com/NREL/OpenStudio/pull/4225) - Wrap `ElectricLoadCenter:Storage:LiIonNMCBattery` in OS SDK
    * [#4236](https://github.com/NREL/OpenStudio/pull/4236) - Wrap `CoilSystem:IntegratedHeatPump:AirSource` and `Coil:WaterHeating:AirToWaterHeatPump:VariableSpeed` in OS SDK
    * [#4258](https://github.com/NREL/OpenStudio/pull/4258) - Wrap `ZoneInfiltration:FlowCoefficient` in OS SDK.
        * This object is wrapped as `SpaceInfiltrationFlowCoefficient` and is a `SpaceLoad`

* [#4199](https://github.com/NREL/OpenStudio/pull/4199) - Add support for translating a Workspace/IdfFile to epJSON
* [#4253](https://github.com/NREL/OpenStudio/pull/4253) - Allow ZoneHVAC:TerminalUnit:VariableRefrigerantFlow to connect to AirLoopHVAC

* [#4180](https://github.com/NREL/OpenStudio/pull/4180) - Fixes [#4175](https://github.com/NREL/OpenStudio/issues/4175), issues with ft for ElectricLoadCenter:Distribution
    * `ElectricLoadCenterDistribution` objects are now possible with no generators / transformers
    * A default `ElectricLoadCenterDistribution` is no longer instantiated in the constructor of children objects: `GeneratorFuelCell`, `GeneratorMicroTurbine`, `GeneratorPhotovoltaic`, `GeneratorPVWatts

* [#4153](https://github.com/NREL/OpenStudio/pull/4153) - Fix #3532 #4152 - Make heating/cooling coils optional for ZoneHVACLowTempVariableFlow and add missing fields to coils
    * This is an API breaking change, the coils are now optional so the getters have been modified to account for this.
    ```diff
    - HVACComponent heatingCoil() const;
    + boost::optional<HVACComponent> heatingCoil() const;
    - HVACComponent coolingCoil() const;
    + boost::optional<HVACComponent> coolingCoil() const;
    ```


* [#4221](https://github.com/NREL/OpenStudio/pull/4221) - Geometry improvements including fixing intersection issues


## Minor changes:

* [#4136](https://github.com/NREL/OpenStudio/pull/4136) - Fix #4125, Fix #4135 - GbXML ForwardTranslator: clear map&set, and add a modelToGbXMLString method
* [#4137](https://github.com/NREL/OpenStudio/pull/4137) - Addresses #4076, radiant system enhancements
* [#4149](https://github.com/NREL/OpenStudio/pull/4149) - Addresses #3706, add drain water heat exchanger methods to WaterUseConnections
* [#4188](https://github.com/NREL/OpenStudio/pull/4188) - Fix #4187 - speed up `IdfFile::save` (and derived, such as `Model::save`) and general serialization to string by using '\n' instead of `std::endl`
* [#4239](https://github.com/NREL/OpenStudio/pull/4239) -Fix #4226 - add int argument for ScheduleFile.setMinutesperItem method

* [#4061](https://github.com/NREL/OpenStudio/pull/4061) - Fix Cloning a DualDuct AirLoopHVAC breaks the existing loop
* [#4081](https://github.com/NREL/OpenStudio/pull/4081) - #4077 Remove name field from common HVAC data objects that are not visible to user


## Minor Bug Fixes

* [#4123](https://github.com/NREL/OpenStudio/pull/4123) - Addresses #4110, align PTAC and PTHP coils with EnergyPlus
* [#4173](https://github.com/NREL/OpenStudio/pull/4173) - Fixes FT of Design Maximum Continuous Input Power for ElectricLoadCenterStorageConverter
* [#4179](https://github.com/NREL/OpenStudio/pull/4179) - Fixes bug in SurfaceProperty:ExposedFoundationPerimeter IDD
* [#4180](https://github.com/NREL/OpenStudio/pull/4180) - Addresses #4175, issues with ft for ElectricLoadCenter:Distribution
* [#4211](https://github.com/NREL/OpenStudio/pull/4211) - Fix #4205 - WorkspaceObject::setString allows setting invalid names for ModelObjects 
* [#4218](https://github.com/NREL/OpenStudio/pull/4218) - Fix #4217 - Do not make an PlantEqOperationUncontrolled for a Component that is already on a PlantEquipmentOperationComponentSetpoint
* [#4245](https://github.com/NREL/OpenStudio/pull/4245) - Fix #4166 - Merging FloorSpaceJS can delete unique model Objects such as Facility, Building, Site (and children)
* [#4235](https://github.com/NREL/OpenStudio/pull/4235) - Fix #4111 - Shading:Building is improperly mapped when "World" coordinates are used for Rectangular Surfaces
* [#4256](https://github.com/NREL/OpenStudio/pull/4256) - Fix #4254 - Set ComponentType::COOLING for EvaporativeFluidCoolerTwoSpeed
* [#4243](https://github.com/NREL/OpenStudio/pull/4243) - Fix #4240 (and more): add missing \ip-units gal/min via a script

### OpenStudio Standards vX.Y.Z

* Fixed a bug related to the implementation of DCV requirements which now impacts many more buildings
* Zone HVAC equipment now defaults to the zone occupancy schedule if it provides ventilation, otherwise it cycles
* Added the ability to export standards .json data to a separate repository https://github.com/NREL/BuildingStandardsData
* Refactored PSZ systems to use the unitary system object for all PSZ types
* Clarified the distinction between and added support for fluid coolers and closed cooling towers
* Added standards 90.1-2016 and 90.1-2016 methods and data
* Included several updates to ComStock versions of standards for commercial stock modeling


### OpenStudio Server v3.2.0


<strong style='color: red;'>TODO</strong>


## Issue Statistics Since Previous Release

184 Closed Issues  
86  New issues