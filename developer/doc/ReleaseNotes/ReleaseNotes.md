# OpenStudio Version 2.8.0

_Release Notes – 4/12/2019_

These release notes describe version 2.8.0 of the OpenStudio software suite developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Systems, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

- Where to Find OpenStudio Documentation
- Installation Notes
- Overview

# Where to Find OpenStudio Documentation

- OpenStudio release documentation, including these release notes, tutorials, and other user documentation, is available at [https://www.openstudio.net/](https://www.openstudio.net/).
- C++ API documentation is available at [https://openstudio-sdk-documentation.s3.amazonaws.com/index.html](https://openstudio-sdk-documentation.s3.amazonaws.com/index.html).
- Measure development documentation is available at [http://nrel.github.io/OpenStudio-user-documentation/reference/measure\_writing\_guide/](http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/).
- A roadmap for planned features is available at [http://nrel.github.io/OpenStudio-user-documentation/getting\_started/roadmap/](http://nrel.github.io/OpenStudio-user-documentation/getting_started/roadmap/). 

# Installation Notes

OpenStudio 2.8.0 is supported on 64-bit Windows 7 – 10, OS X 10.11 – 10.14, and Ubuntu 16.04.

OpenStudio 2.8.0 supports EnergyPlus Release 9.1, which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio 2.8.0.

OpenStudio 2.8.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio 2.8.0.

## Installation Steps

- The OpenStudio SketchUp Plug-in requires [SketchUp 2017 or 2018](https://www.sketchup.com/download/all) (not available for Linux). OpenStudio 2.8.0 is not compatible with SketchUp 2019. The OpenStudio 3.0 prerelease in October 2019 will be the first version of OpenStudio compatible with SketchUp 2019. The OpenStudio SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio to automatically activate the plugin.  If you install SketchUp after OpenStudio, simply rerun the OpenStudio installer.
- Download and install [OpenStudio](https://www.openstudio.net/downloads). Select components for installation.
- Setup a Building Component Library (BCL) account to access online building components and measures. [View instructions on how to setup your account and configure the key in OpenStudio](http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/).

For help with common installation problems please visit, [http://nrel.github.io/OpenStudio-user-documentation/help/troubleshooting/](http://nrel.github.io/OpenStudio-user-documentation/help/troubleshooting/).

# Overview

An important announcement related to future development of the OpenStudio Application is available at: [https://www.openstudio.net/new-future-for-openstudio-application](https://www.openstudio.net/new-future-for-openstudio-application). Please read this announcement carefully to understand changes coming for the OpenStudio Application over future releases.  Please send any questions, comments, or concerns to [OpenStudio@nrel.gov](mailto:OpenStudio@nrel.gov?subject=OpenStudio%20Application%20Transition) and we will try our best to address them.

## OpenStudio SDK:
The 2.8.0 release updates OpenStudio to use EnergyPlus 9.1.  This update includes several new features, performance improvements, and bug fixes.  LBNL contributed new OpenStudio ModelObjects ElectricEquipmentITEAirCooledDefinition and ElectricEquipmentITEAirCooled which can be used to model air-cooled electric information technology equipment (ITE) with variable power consumption as a function of loading and temperature. Users can now set sequential load heating/cooling fractions for equipment on the ZoneHVACEquipmentList object. The DefaultConstructionSet now has an Adiabatic Surface Construction. End-Use Subcategory fields were added for pumps. 

*Other changes*:
 
- Remove 'file://' prefix for file paths listed in OSM files.  External files are now referenced by filename only with search path specified in OSW.  Affects WeatherFile and ScheduleFile.
- Sql methods now make use of the Year field that was added in E+ 8.9.0 to avoid problems with leap years (#3453).
- Force unique names to match new E+ convention following re-factor of E+’s Input Processor. For example, LifeCycleCost objects can no longer bear the same name (#3458).
- Show Measure Directory Name in OpenStudio Application (#3465). This should make it easier to understand which measure is used even after the user has changed the editable name.
- Remove non-implemented UtilityCost_* objects from API documentation (#3462)
- Fix an error in AirTerminalSingleDuctConstantVolumeFourPipeBeam::remove() method that was making the OpenStudio Application crash (#3440)
- Fixes to API for CoilSystemCoolingDXHeatExchangerAssisted and CoilSystemCoolingWaterHeatExchangerAssisted (#3449)
- Fix forward translator issues in CoilHeatingDesuperheater and CoilWaterHeatingDesuperheater (#3452)
- AirLoop::clone was modified to reconnect any supply side coils to the same plant loops as the original coils. This method will clone an AirLoopHVAC with the following rationale:
    - Handle all non-branch components from both the supply and the demand side
    - On the demand side branches, place one terminal of each IddObjectType that is present on the original AirLoopHVAC
    - Clone any SetpointManagers and add them to the correct location
    - If the supply component that is cloned is connected to a PlantLoop, we try to the connect the clone to the same PlantLoop by adding a demand branch
- PlantLoop::clone was added and follows the same logic. This method will clone a Plant Loop with the following rationale:
    - Handle all non-branch components from both the supply and the demand side
    - Handle branch components on the supply side (between supply splitter and mixer)
    - Clone any SetpointManagers and add them to the correct location
    - If the component that is cloned is connected to another PlantLoop, we try to connect the clone to the same other PlantLoop (if comp is on the supply side, we add a demand branch to the other Plantloop)

## OpenStudio Server:

- Upgraded to Ubuntu 16.04 and R version 3.5.2
- Added plots for Sobol method
- Added argument to do out of bounds check for Morris method.  If selected true, analysis won't run if created data points do not respect max/min of variables.
- Correct Morris results json format
- Correctly kill OpenStudio process after timeout.
- Timeouts increased from 2hrs to 4hrs for each data point.
- Adjusted processor allocation for remote deployments.
- Fixed "Cancel" functionality.
- Set analysis status to "post-processing" while analysis finalization script is running.
- Better support for publishing custom AMIs and Docker images.

## OpenStudio Standards:

- Adds a model for heat pump water heaters (HPWH) which uses the WaterHeaterMixed object with an electric fuel and a higher-than-1 efficiency to mimic HPWHs without incurring the significant penalty of the stratified tank.  The nuances of some HPWH behavior are lost, so we don’t recommend using this unless you investigate and accept the accuracy tradeoffs for your application.
- Adds typical refrigeration methods.  This method adds detailed refrigeration systems to grocery stores and adds single-walkin systems to other building types with kitchens.  For grocery stores, the typical characteristics are based on a combination of expert knowledge and the ASHRAE grocery store Advanced Energy Design Guide.  Case and walkin lineups for the grocery store are scaled based on the store size.  Walkins for kitchens are not scaled.
- Adds a new standards worksheet and methods to determine the number of elevators based on building type.  Elevator schedules are now based on adjustments to the occupancy schedule rather than set explicitly. changed code to pull elevator logic from this new sheet.
- Elevator end uses are now tagged with end use category 'Elevators'.
- Adds in both ASHRAE 169-2006 and ASHRAE 169-2013 climate zone definitions to the spreadsheet and the code base. ASHRAE 169-2013 updates city locations and ground temperatures for each climate zone.
- Adds the ability to model air to water heat pumps on hot water loops in place of boilers. The equipment is modeled with a PlantComponentUserDefined object.  A full air to water heat pump object will be available in the next EnergyPlus/OpenStudio release.  
- Change the default condenser loop to follow OATwb with 7F approach instead of a fixed setpoint temperature.
- Default to mandate off-hour controls for air loops per 90.1 section 6.4.3.3, based on zone occupancy schedules on an air loop.
- Use a default infiltration rate matched to the PNNL modeling guideline value of 1.8 cfm/ft2 exterior surface area at 75 Pa instead of zero.
- Exterior equipment now uses the ExteriorFuelEquipment object instead of the ExteriorLights object.
- Exterior lighting calculations now estimate the number of doors based on ground floor area, not total area.
- Refactors exterior area and length calculations for exterior lighting to use standards building type, not standards space type.  Before, the method was looping over standards space types and adding a minimum of one building entry per space type, resulting in very high main entryway lighting.

*DOE prototype model changes:*

- Uses ASHRAE 169-2013 for climate zone definitions and locations instead of ASHRAE 169-2006
- Adds small office and large office detailed prototypes models.
- Deactivate daylighting control for side lighting for the retail prototypes for 90.1-2010 (exception c to section 9.4.1.4 of 90.1-2010) and 90.1-2013 (exception 3 to section Section 9.4.1.1(e)).
- Add Computer Rooms to All 90.1 Schools Templates.
- Fix issue to add thermal zone multipliers where present.

*Bug fixes:*

- DEER schedules now include explicit design day schedules.
- Fix issue where duplicate internal mass objects were being added to the same zone.
- Add more detailed error messages for economizers.
- Set heating maximum air flow fraction to 1.0 for ZoneHVACIdealLoadsAirSystem to allow full heating airflow during sizing.

## Issue Statistics Since Previous Release

- 56 new issues were filed since the 2.7.0 release of OpenStudio (not including opened pull requests).
- 86 issues were closed since the 2.7.0 release of OpenStudio (not including closed pull requests).


