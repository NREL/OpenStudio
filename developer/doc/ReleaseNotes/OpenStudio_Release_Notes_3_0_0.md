# OpenStudio Version 3.0.0

_Release Notes - 4/24/2020_

These release notes describe version 3.0.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Systems, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog


# Overview

Following an important announcement related to future development of the OpenStudio Application released in September 2019 ([A Shift in BTO’s BEM Strategy: A New Future for the OpenStudio Application](https://www.openstudio.net/new-future-for-openstudio-application)), **this release marks the first installment of the separated OpenStudio SDK from the Application.**
Going forward, the SDK and Application will be distributed as ***separate*** applications and will be released on different schedules. Development of the OpenStudioApplication has been moved to its own [NREL/OpenStudioApplication](https://github.com/NREL/OpenStudioApplication/) GitHub.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.0.0**__
- EnergyPlus
- Command Line Interface (CLI)
- Radiance
- Ruby API
- C++ SDK

**Note** that [PAT](https://github.com/NREL/OpenStudio-PAT) is no included in either the SDK or Application installers. You will need to install PAT separately which is distributed on the [OpenStudio-PAT](https://github.com/NREL/OpenStudio-PAT) GitHub page.

## Long-Term Support of 2.x version: 2.9.X-LTS

This new major release is bringing a lot of changes not the least of which is the separation of the OpenStudioApplication as well as some API breaking changes.
Long-Term Support (LTS) of the 2.x version will be provided until XX/XX/XXXX via the `2.9.X-LTS` branch and infrequent releases. Only critical bug-fixes will be implemented in the LTS branch
and no new features will be developed. The supported EnergyPlus version will remain 9.2.0 in the LTS branch. We encourage users to transition to the 3.x version of OpenStudio.

# Where to Find OpenStudio Documentation

- OpenStudio release documentation, including these release notes, tutorials, and other user documentation, is available at [https://www.openstudio.net/](https://www.openstudio.net/.)
- C++ API documentation is available at [https://openstudio-sdk-documentation.s3.amazonaws.com/index.html](https://openstudio-sdk-documentation.s3.amazonaws.com/index.html)
- Measure development documentation is available at [http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/](http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/ )
- A roadmap for planned features is available at http://nrel.github.io/OpenStudio-user-documentation/getting_started/roadmap/.

# Installation Notes

OpenStudio 3.0.0 is supported on 64-bit Windows 7 – 10, OS X 10.12 – 10.15, and Ubuntu 18.04.

OpenStudio 3.0.0 supports [EnergyPlus Release 9.3](https://github.com/NREL/EnergyPlus/releases/tag/v9.3.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio 3.0.0.

OpenStudio 3.0.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio 3.0.0.

OpenStudio 3.x now uses **Ruby 2.5.5** as the supported version (2.2.4 previously).

As usual, you can refer to the **[OpenStudio Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-Version-Compatibility-Matrix)** for more information.

## Installation Steps

- Download and install OpenStudio SDK and/or [OpenStudioApplication]( depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2019 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply rerun the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user- documentation/help/troubleshooting/.

# OpenStudio SDK: Changelog

The 3.0.0 is a major release
The 3.0.0 release updates OpenStudio to use EnergyPlus 9.3. This update includes several new features, performance improvements, and bug fixes.
You can find the list of Pull Requests that got into this release [here](https://github.com/NREL/OpenStudio/pulls?utf8=%E2%9C%93&q=is%3Apr+is%3Aclosed+created%3A2019-12-07..2020-02-25+).


## New Features, Major Fixes and API-breaking changes

A number of API-breaking changes have been implemented in OpenStudio 3.0:

* Updated EnergyPlus from v9.2.0 to v9.3.0
* ([#3044](https://github.com/NREL/OpenStudio/issues/3044)) Added support for `FanSystemModel` to the Model API
* `ThermalZone::addEquipment` is no longer exposed to the bindings to avoid confusion and problems, as you should always use `ZoneHVACComponent::addToThermalZone` instead (#2920, #3758)
* [#3814](https://github.com/NREL/OpenStudio/pull/3814) - Add ability to create Holiday Schedules in ScheduleRuleset
* [#3882](https://github.com/NREL/OpenStudio/pull/3882) - Add methods to edit `Output:Table:SummaryReports` in OpenStudio SDK
* [#3875](https://github.com/NREL/OpenStudio/pull/3875) - Add OutputDiagnostics, OutputDebuggingData and OutputJSON to model SDK
* [#3853](https://github.com/NREL/OpenStudio/pull/3853)  - Fixes bug where MasslessOpaqueMaterial incorrectly had methods for setting and getting conductivity, density, and specific heat

* [#3744](https://github.com/NREL/OpenStudio/issues/3744) - New native extension gems have been added into the CLI:
    * pycall v1.2.1 - Allows for calling Python functions from the Ruby language 
    * sqlite3 v1.3.13 - Ruby bindings for the SQLite3 embedded database
    * Oga v3.2 - Oga is an XML/HTML parser written in Ruby
    * jara_winker v.1.5.4 - implementation of Jaro-Winkler distance algorithm needed for newer versions of rubocop >= v0.80.0  
    
* [#23](https://github.com/NREL/openstudio-gems/pull/23) - Updated extension gems have been added into the CLI:
    * bundler v2.1.0
    * openstudio-extension v0.2.1
    * openstudio-workflow v2.0.0
    * openstudio-standards v0.2.11
    * openstudio_measure_tester v0.2.2  

* [#3796](https://github.com/NREL/OpenStudio/pull/3796) - Reporting Measure: you can now pass `model` to the `arguments()` method of a reporting measure, for easier setup of output variables.
* [#3847](https://github.com/NREL/OpenStudio/pull/3847) - Chiller:Electric:EIR node API refactor, ChillerAbsorption/ChillerAbsorption tertiary node refactor
    * `ChillerElectricEIR::designHeatRecoveryWaterFlowRate()` now returns an `OptionalDouble` instead of a `double` since it can be now be autosized
    * `ChillerElectricEIR` no longer has getter/setters that set/return std::string for Node Names. It also wraps two new E+ fields: "Condenser Heat Recovery Relative Capacity Fraction", "Heat Recovery Inlet High Temperature Limit Schedule Name" and "Heat Recovery Leaving Temperature Setpoint Node Name"
    * Added missing Tertiary nodes for `ChillerAbsorption`
    * For `ChillerAbsoprtion`, `ChillerAbsoprtionIndirect`, `ChillerElectricEIR`: 
        * `addToTertiaryNode` overriden to only allow supply side connection.
        * `addToNode` overriden to connect to tertiary loop if primary loop is already connected, and Node to connect to is on the supply side of a different plant loop than the current primary loop
* [#3913](https://github.com/NREL/OpenStudio/pull/3913) - E+ 9.3.0: Update ShadowCalculation to match object refactor in EnergyPlus
    * `ShadowCalculation::calculationMethod` was renamed to `ShadowCalculation::shadingCalculationUpdateFrequencyMethod` (same for setter, defaulted, and reset methods)
    * `ShadowCalculation::calculationFrequency` was renamed to `ShadowCalculation::shadingCalculationUpdateFrequency` (same for setter, defaulted, and reset methods)
    * `ShadowCalculation::polygonClippingAlgorithm()` and `ShadowCalculation::skyDiffuseModelingAlgorithm()`: return type changed from `OptionalString` (which was always initialized) to `std::string`
    * All new fields in E+ 9.3.0 were also added

## Minor changes:

* [#3846](https://github.com/NREL/OpenStudio/pull/3846) - `SiteWaterMainsTemperature` now defaults to the `CorrelationFromWeatherFile` Calculation Method just like in EnergyPlus.
* [#3846](https://github.com/NREL/OpenStudio/pull/3846) - `BoilerHotWater` "Design Water Outlet Temperature" field to match E+
* [#3868](https://github.com/NREL/OpenStudio/pull/3868) - Fix an error when querying `surfaces()` of `ZoneHVACLowTemperature` objects and surfaces have no construction
* [#3844](https://github.com/NREL/OpenStudio/pull/3844) - Fix crash in FT for `HeatPumpWaterToWaterEquationFitCooling/Heating` when they reference each other as companion coils
* [#3871](https://github.com/NREL/OpenStudio/pull/3871) - Remove deprecated Active Year/Institution fields in `OS:ClimateZones`
* [#3878](https://github.com/NREL/OpenStudio/pull/3878) - Reporting Frequencies (eg: for `OutputVariable`) now differentiates between `RunPeriod` and `Annual` frequencies
* [#3805](https://github.com/NREL/OpenStudio/pull/3805) - Support year in `EpwFile`/`EpwDataPoint`
* [#3813](https://github.com/NREL/OpenStudio/pull/3813) - Change default specific heat of new materials to match E+
* [#3849](https://github.com/NREL/OpenStudio/pull/3849) - Add a missing entry in ScheduleTypeRegistry for `SurfacePropertyOtherSideCoefficients`
* [#3855](https://github.com/NREL/OpenStudio/pull/3855) - Autosize `ZoneHVACUnitHeater` Maximum Water flow rate in constructor
* [#3857](https://github.com/NREL/OpenStudio/pull/3857) - Reverse translation of `WindowMaterial:Glazing` sets Optical Data type field incorrectly
* [#3858](https://github.com/NREL/OpenStudio/pull/3858) - Wrong `OS:Daylighting:Control` rotation angle translated
* [#3862](https://github.com/NREL/OpenStudio/pull/3862) - Minor change to the E+ IDD for `LifeCycleCost:UsePriceEscalation`
* [#3867]() - Reset Surface Wind/SunExposure to NoSun/NoWind when setting Outside Boundary Condition to `OtherSideCoefficients` Fixed in [#3916](https://github.com/NREL/OpenStudio/pull/3916)
* [#3903](https://github.com/NREL/OpenStudio/issues/3903) - Add new fields to PerformancePrecisionTradeoffs for E+ 9.3.0
* [#3898](https://github.com/NREL/OpenStudio/issues/3898) - Add Minimum zone ventilation efficiency 
* [#3894](https://github.com/NREL/OpenStudio/issues/3894) - SqlFile need to support the "Year" field for IlluminanceMaps 
* [#3892](https://github.com/NREL/OpenStudio/issues/3892) - Add Wrap SimulationControl's Do HVAC Sizing Simulation for Sizing Periods 
* [#3885](https://github.com/NREL/OpenStudio/issues/3885)- Allow "CorrelationFromWeatherFile" for SiteWaterMainsTemperature's calculation method
* [#3872](https://github.com/NREL/OpenStudio/issues/3872) - Adding advanced output variables 
* [#3786](https://github.com/NREL/OpenStudio/issues/3786) [#3787](https://github.com/NREL/OpenStudio/pull/3787)  - Allow SDK users to not have to write out SQLite files
* [#3785](https://github.com/NREL/OpenStudio/issues/3785) - Expose the ZoneVentilation:WindandStackOpenArea in the Model SDK  

## Minor Bug Fixes 


* [#3938](https://github.com/NREL/OpenStudio/issues/3938) - Fix loading native ext gems for new gems that were added. 
* [#3917](https://github.com/NREL/OpenStudio/issues/3917) - Connecting two ThermalZones in series on an AirLoopHVAC demand branch shouldn't be allowed
* [#3888](https://github.com/NREL/OpenStudio/issues/3888) - Fix cloning issue for model objects. 
* [3866](https://github.com/NREL/OpenStudio/issues/3866) - Fix Forward translator Runtime Error if a model has a ZoneHVAC:LowTemperatureRadiant:VariableFlow but no constructions assigned
* [#3854](https://github.com/NREL/OpenStudio/issues/3854) - Fix Maximum Hot Water Flow Rate to autosize
* [#3823](https://github.com/NREL/OpenStudio/issues/3823) - Fix RoofGeometry build errors 
* [#3793](https://github.com/NREL/OpenStudio/issues/3793) - Fix ITE object supply/approach temperature difference schedule 
* [#3792](https://github.com/NREL/OpenStudio/issues/3792 ) - Fix in support of OpenStudio Application

### OpenStudio Standards

* correctly enable DOAS DCV
* updated and add values for the NREL ZNE Ready 2017 standard and added a ZE AEDG Multifamily standard
* updated refrigeration code to add walk-in refrigators and freezers to all relevant zones
* updated economizer logic
* add properties for climate zones 0A and 0B
* refactor methods to work with OS 3.0
* add doors to DOE prototype models
* updated multizone ventilation calculation logic in the prototype models
* various bug fixes to prototype models and hvac system methods


## Issue Statistics Since Previous Release

* 67 new issues were filed since the 2.9.1 release of OpenStudio (not including opened pull requests)

* 124 issues were closed since the 2.9.1 release of OpenStudio (not including closed pull requests).