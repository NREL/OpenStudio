**TODO: Note: Just use the #XXXX notation to link to PRs for convenience. We'll do a search and replace if we end up doing a pandoc conversion to PDF for eg**


# OpenStudio Version 3.0.0

_Release Notes - 4/XX/2020_


These release notes describe version 3.0.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Systems, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog


# Overview

Following an important announcement related to future development of the OpenStudio Application released in September 2019 ([A Shift in BTO’s BEM Strategy: A New Future for the OpenStudio Application](https://www.openstudio.net/new-future-for-openstudio-application)), **this release marks the first installment of the separated OpenStudio SDK from the Application.**
Going forward, the SDK and Application will be distributed as ***separate*** applications and will be released on different schedules. Development of the OpenStudioApplication has been moved to its own [NREL/OpenStudio](https://github.com/NREL/OpenStudioApplication/) GitHub.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.0.0**__
- EnergyPlus
- Command Line Interface (CLI)
- Radiance
- Ruby API
- C++ SDK

**Note** that [PAT](https://github.com/NREL/OpenStudio-PAT) is no longer be included in either the SDK or Application installers. You will need to install PAT separately which is posted on the [OpenStudio-PAT](https://github.com/NREL/OpenStudio-PAT) GitHub page.

## Long-Term Support of 2.x version: 2.9.X-LTS

This new major release is bringing a lot of changes not the least of which is the separation of the OpenStudioApplication as well as some API breaking changes.
Long-Term Support (LTS) of the 2.x version will be provided until XX/XX/XXXX via the `2.9.X-LTS` branch and infrequent releases. Only critical bug-fixes will be implemented in the LTS branch
and no new features will be developed. The supported EnergyPlus version will remain 9.2.0 in the LTS branch. We encourage users to transition to the 3.x version of OpenStudio.

**TODO: We won't upgrade EnergyPlus in the LTS branch right?** 

# Where to Find OpenStudio Documentation

- OpenStudio release documentation, including these release notes, tutorials, and other user documentation, is available at [https://www.openstudio.net/](https://www.openstudio.net/.)
- C++ API documentation is available at [https://openstudio-sdk-documentation.s3.amazonaws.com/index.html](https://openstudio-sdk-documentation.s3.amazonaws.com/index.html)
- Measure development documentation is available at [http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/](http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/ )
- A roadmap for planned features is available at http://nrel.github.io/OpenStudio-user-documentation/getting_started/roadmap/.

# Installation Notes

OpenStudio 3.0.0 is supported on 64-bit Windows 7 – 10, OS X 10.12 – 10.14, and Ubuntu **18.04**.

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
**TODO: Update end date in link**

## New Features, Major Fixes and API-breaking changes

A number of API-breaking changes have been implemented in OpenStudio 3.0:

* Updated EnergyPlus from v9.2.0 to v9.3.0
* Added support for `FanSystemModel` ([#3044](https://github.com/NREL/OpenStudio/issues/3044)) to the Model API
* **PENDING**: Add new DOAS to AHU objects ([#3728](https://github.com/NREL/OpenStudio/issues/3728))
* `ThermalZone::addEquipment` is no longer exposed to the bindings to avoid confusion and problems, as you should always use `ZoneHVACComponent::addToThermalZone` instead (#2920, #3758)
* #3814 - Add ability to create Holiday Schedules in ScheduleRuleset
* **PENDING**: #3882 - Add methods to edit `Output:Table:SummaryReports` in OpenStudio SDK
* #3875 - Add OutputDiagnostics, OutputDebuggingData and OutputJSON to model SDK
* **TODO @joseph-robertson to add notes about #3853 (MasslessOpaqueMaterial)**
* **TODO: GEM STUFF? Perhaps that could be in its own paragraph though. Topics: Native Gems, Ruby 2.5.x, Bundler >= 2.1.x**
* #3796 - Reporting Measure: you can now pass `model` to the `arguments()` method of a reporting measure, for easier setup of output variables.
* #3847 - Chiller:Electric:EIR node API refactor, ChillerAbsorption/ChillerAbsorption tertiary node refactor
    * `ChillerElectricEIR::designHeatRecoveryWaterFlowRate()` now returns an `OptionalDouble` instead of a `double` since it can be now be autosized
    * `ChillerElectricEIR` no longer has getter/setters that set/return std::string for Node Names. It also wraps two new E+ fields: "Condenser Heat Recovery Relative Capacity Fraction", "Heat Recovery Inlet High Temperature Limit Schedule Name" and "Heat Recovery Leaving Temperature Setpoint Node Name"
    * Added missing Tertiary nodes for `ChillerAbsorption`
    * For `ChillerAbsoprtion`, `ChillerAbsoprtionIndirect`, `ChillerElectricEIR`: 
        * `addToTertiaryNode` overriden to only allow supply side connection.
        * `addToNode` overriden to connect to tertiary loop if primary loop is already connected, and Node to connect to is on the supply side of a different plant loop than the current primary loop

## Minor changes:

* #3846 - `SiteWaterMainsTemperature` now defaults to the `CorrelationFromWeatherFile` Calculation Method just like in EnergyPlus.
* #3846 - `BoilerHotWater` "Design Water Outlet Temperature" field to match E+
* #3868 - Fix an error when querying `surfaces()` of `ZoneHVACLowTemperature` objects and surfaces have no construction
* #3844 - Fix crash in FT for `HeatPumpWaterToWaterEquationFitCooling/Heating` when they reference each other as companion coils
* #3871 - Remove deprecated Active Year/Institution fields in `OS:ClimateZones`
* #3878 - Reporting Frequencies (eg: for `OutputVariable`) now differentiates between `RunPeriod` and `Annual` frequencies
* #3805 - Support year in `EpwFile`/`EpwDataPoint`
* #3813 - Change default specific heat of new materials to match E+
* #3849 - Add a missing entry in ScheduleTypeRegistry for `SurfacePropertyOtherSideCoefficients`
* #3855 - Autosize `ZoneHVACUnitHeater` Maximum Water flow rate in constructor
* #3857 - Reverse translation of `WindowMaterial:Glazing` sets Optical Data type field incorrectly
* #3858 - Wrong `OS:Daylighting:Control` rotation angle translated
* #3862 - Minor change to the E+ IDD for `LifeCycleCost:UsePriceEscalation`
* **PENDING**: #3867 - Reset Surface Wind/SunExposure to NoSun/NoWind when setting Outside Boundary Condition to `OtherSideCoefficients`

## Issue Statistics Since Previous Release
