# OpenStudio Version 2.6.0

_Release Notes – 6/30/2018_

These release notes describe version 2.6.0 of the OpenStudio software suite developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Systems, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

- Where to Find OpenStudio Documentation
- Installation Notes
- Overview

# Where to Find OpenStudio Documentation

- OpenStudio release documentation, including these release notes, tutorials, and other user documentation, is available at [https://www.openstudio.net/](https://www.openstudio.net/).
- C++ API documentation is available at [https://openstudio-sdk-documentation.s3.amazonaws.com/index.html](https://openstudio-sdk-documentation.s3.amazonaws.com/index.html).
- Measure development documentation is available at [http://nrel.github.io/OpenStudio-user-documentation/reference/measure\_writing\_guide/](http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/).

# Installation Notes

OpenStudio is supported on Windows 7 – Windows 10, OS X 10.10 – 10.11, and 64-bit Ubuntu 14.04.

OpenStudio 2.6.0 supports EnergyPlus Release 8.9.0, which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio 2.6.0.

OpenStudio 2.6.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio 2.6.0.


## Installation Steps

- The OpenStudio SketchUp Plug-in requires [SketchUp 2017](http://www.sketchup.com/) (not available for Linux). The OpenStudio SketchUp Plug-in does not support older versions of SketchUp.
  - If the OpenStudio Plug-in does not automatically load in SketchUp, open the Window-&gt;Preferences-&gt;Extensions window in SketchUp and enable the OpenStudio plug-in if it is listed.
- Download and install [OpenStudio](https://www.openstudio.net/downloads). Select components for installation.
- Setup a Building Component Library (BCL) account to access online building components and measures. [View instructions on how to setup your account and configure the key in OpenStudio](http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/).

For help with common installation problems please visit, [http://nrel.github.io/OpenStudio-user-documentation/help/troubleshooting/](http://nrel.github.io/OpenStudio-user-documentation/help/troubleshooting/).

# Overview

## OpenStudio SDK:
The 2.6.0 release of OpenStudio includes new features to help write better measures!  The new measure templates have been updated to create a license and README.mb.erb for each new measure.  When a measure is updated, the README.mb.erb is configured with information from the measure to create a README.md file which is the perfect place to document your measure. You can reference images or other content your measure’s docs/ folder from the README.md file to make your documentation really stand out.  Additionally, a new option has been added to the OpenStudio CLI which runs the tests for all measures in a directory and saves a dashboard with results: `openstudio measure -r /dir/to/measures/`.  This dashboard quickly shows all passing/failing tests as well as provide input on Ruby style using Rubocop.  Code coverage will be added to the measure testing dashboard in a future release.

The following new model objects have been added to the OpenStudio SDK:

- GeneratorPVWatts 
- ElectricLoadCenterInverterPVWatts 
- PlantComponentUserDefined
- AirTerminalSingleDuctConstantVolumeFourPipeBeam
- CoilCoolingFourPipeBeam
- CoilHeatingFourPipeBeam

EMS Sensor and Actuator have been expanded to include SpaceLoads arguments.  This will allow for correct EMS actuators on SpaceLoads that end up in ZoneLists in E+.  The next release will expand on this capability.

## OpenStudio Server:
OpenStudio Server has been updated to address several important issues:

- Fix cursor deleted with accessing /analyses/<id>/analysis_data
- Set version of mongo and redis for docker deploys
- Backend for Docker deployment now used Resque with Redis instead of delayed jobs.
- Large Uploads to Server (i.e. lots of weather files) seem to Timeout

## OpenStudio Standards:
OpenStudio Standards has the following updates:

- Added additional DEER prototypes, including multifamily
- Added service hot water to DEER prototypes
- General bug fixes for DEER prototypes

## Issue Statistics Since Previous Release

- 48 new issues were filed since the 2.5.0 release of OpenStudio (not including opened pull requests).
- 15 issues were closed since the 2.5.0 release of OpenStudio (not including closed pull requests).
