# OpenStudio Version 2.7.0

_Release Notes – 10/12/2018_

These release notes describe version 2.7.0 of the OpenStudio software suite developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Systems, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

- Where to Find OpenStudio Documentation
- Installation Notes
- Overview

# Where to Find OpenStudio Documentation

- OpenStudio release documentation, including these release notes, tutorials, and other user documentation, is available at [https://www.openstudio.net/](https://www.openstudio.net/).
- C++ API documentation is available at [https://openstudio-sdk-documentation.s3.amazonaws.com/index.html](https://openstudio-sdk-documentation.s3.amazonaws.com/index.html).
- Measure development documentation is available at [http://nrel.github.io/OpenStudio-user-documentation/reference/measure\_writing\_guide/](http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/).

# Installation Notes

OpenStudio is supported on Windows 7 – Windows 10, OS X 10.10 – 10.11, and 64-bit Ubuntu 14.04.

OpenStudio 2.7.0 supports EnergyPlus Release 9.0, which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio 2.7.0.

OpenStudio 2.7.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio 2.7.0.


## Installation Steps

- The OpenStudio SketchUp Plug-in requires [SketchUp 2017](http://www.sketchup.com/) (not available for Linux). The OpenStudio SketchUp Plug-in does not support older versions of SketchUp.
  - If the OpenStudio Plug-in does not automatically load in SketchUp, open the Window-&gt;Preferences-&gt;Extensions window in SketchUp and enable the OpenStudio plug-in if it is listed.
- Download and install [OpenStudio](https://www.openstudio.net/downloads). Select components for installation.
- Setup a Building Component Library (BCL) account to access online building components and measures. [View instructions on how to setup your account and configure the key in OpenStudio](http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/).

For help with common installation problems please visit, [http://nrel.github.io/OpenStudio-user-documentation/help/troubleshooting/](http://nrel.github.io/OpenStudio-user-documentation/help/troubleshooting/).

# Overview

An important announcement related to future development of the OpenStudio Application is available at: [https://www.openstudio.net/new-future-for-openstudio-application](https://www.openstudio.net/new-future-for-openstudio-application). Please read this announcement carefully to understand changes coming for the OpenStudio Application over future releases.  Please send any questions, comments, or concerns to [OpenStudio@nrel.gov](mailto:OpenStudio@nrel.gov?subject=OpenStudio%20Application%20Transition) and we will try our best to address them.

## OpenStudio SDK:
The 2.7.0 release updates OpenStudio to use EnergyPlus 9.0.  This update includes several new features, performance improvements, and bug fixes.  This release also includes two new command line switches for the OpenStudio Command Line Interface (CLI): `--bundle` and `--bundle_path`.  These switches allow the OpenStudio CLI to use a pre-made bundle of Ruby gems, allowing more control over gems used by measures.  Geometry import and merge functionality was enhanced from gbXML, IDF, and OSM sources.  Users can import and merge geometry from these file types with their existing OpenStudio Models.  The OpenStudio API was extended to support multiple AirloopHVAC objects per ThermalZone.  New model objects were added for ExternalFile and ScheduleFile allowing CSV files to be used for schedule input.  There is a [known issue (#3297)](https://github.com/NREL/OpenStudio/issues/3297) with ScheduleFile objects created using Apply Measure Now.  The ElectricLoadCenterTransformer object was also added to allow modeling of electrical transformers.  Many bugs were addressed in this release.  Notably, several issues with forward translation of EMS programs were resolved. Serious issues related to SetpointManagers and other HVAC equipment introduced into the C# bindings as of OpenStudio 2.6.0 were resolved.

*2.7.0 API Changes*: 
- The class AirTerminalSingleDuctUncontrolled was renamed to AirTerminalSingleDuctConstantVolumeNoReheat for consistency with EnergyPlus, a back port was added to the Ruby bindings to preserve compatibility with the old name AirTerminalSingleDuctUncontrolled.
- The methods constantMinimumAirFlowFraction and fixedMinimumAirFlowRate of AirTerminalSingleDuctVAVReheat now return an optional double rather than a double.

## OpenStudio Server:
OpenStudio Server has been updated to address several important issues:

The OpenStudio Server was updated to run datapoints via the OpenStudio Command Line Interface rather than Workflow Gem.  This update includes use of the new `--bundle` and `--bundle_path` command line switches allowing OpenStudio gems (bundle) to be customized independent of OpenStudio Server bundle.  Analysis Initialize and Finalize Scripts can be run for Resque-based environments (ie not on local PAT). Datapoint Initialize and Finalize Scripts have been restructured and can be run on Resque-based environments. 

## OpenStudio Standards:

OpenStudio Standards underwent a major HVAC system code refactor with updates to the DOE and DEER Prototypes.

- Adds optimum start
- Adds door and attic infiltration to DOE prototypes
- Adds skylight frames
- Fixes construction sets for attics and soffits
- Disables ERV SAT outlet control
- Updates motorized OA damper control to skip loop if a schedule other than AlwaysOn is set for min OA
- Changes SEER and EER to COP conversion formulas
- Changes UnitHeaters to be normal (as opposed to small) fans
- Adds vestibule control for unit heaters
- Condenses geometry and HVAC JSON files for DOE prototypes to avoid duplication
- Updates water heater skin loss coefficients
- Adds water heater to zone (instead of assuming 70F air)
- Other minor changes to hard-coded DOE prototype assumptions
- Fixes to DOE and DEER prototype geometries
- Moves HVAC inference and size categorization to the standards spreadsheet
- Adds fans as a database in standards spreadsheet
- HVAC system methods now use keyword arguments and assume defaults, with component constructors (coils, fans, boilers) moved to their own modules
- Adds several NZE systems, including: 
    - VRF
    - DOAS
    - DOAS with DCV controls
- Name HVAC objects, including naming nodes to be more descriptive
- Aligns design system, coil, loop, and setpoint manager temperatures where appropriate
- General bug fixes to HVAC systems
- Set acoustical tile ceiling construction for models with plenum zones (DOE MediumOffice, DOE LargeOffice, and most DEER models)
- Adds infiltration to 'DOE Ref 1980-2004' and 'DOE Ref Pre-1980' plenums


## Issue Statistics Since Previous Release

- 39 new issues were filed since the 2.6.0 release of OpenStudio (not including opened pull requests).
- 73 issues were closed since the 2.6.0 release of OpenStudio (not including closed pull requests).

