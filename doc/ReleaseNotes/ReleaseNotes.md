# OpenStudio Version 2.3.0

_Release Notes – 9/30/2017_

These release notes describe version 2.3.0 of the OpenStudio software suite developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Systems, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

- Where to Find OpenStudio Documentation
- Installation Notes
- Overview

# Where to Find OpenStudio Documentation

- OpenStudio release documentation, including these release notes, tutorials, and other user documentation, is available at [https://www.openstudio.net/](https://www.openstudio.net/).
- C++ API documentation is available at [https://openstudio-sdk-documentation.s3.amazonaws.com/index.html](https://openstudio-sdk-documentation.s3.amazonaws.com/index.html).
- Measure development documentation is available at [http://nrel.github.io/OpenStudio-user-documentation/reference/measure\_writing\_guide/](http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/).

# Installation Notes

OpenStudio is supported on Windows 7 – Windows 10, OS X 10.10 – 10.11, and 64-bit Ubuntu 14.04.

OpenStudio 2.3.0 supports EnergyPlus Release 8.8.0, which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio 2.3.0.

OpenStudio 2.3.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer have to install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio 2.3.0.

## Installation Steps

- The OpenStudio SketchUp Plug-in requires [SketchUp 2017](http://www.sketchup.com/) (not available for Linux). The OpenStudio SketchUp Plug-in does not support older versions of SketchUp.
  - If the OpenStudio Plug-in does not automatically load in SketchUp, open the Window-&gt;Preferences-&gt;Extensions window in SketchUp and enable the OpenStudio plug-in if it is listed.
- Download and install [OpenStudio](https://www.openstudio.net/downloads). Select components for installation.
- Setup a Building Component Library (BCL) account to access online building components and measures. [View instructions on how to setup your account and configure the key in OpenStudio](http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/).

For help with common installation problems please visit, [http://nrel.github.io/OpenStudio-user-documentation/help/troubleshooting/](http://nrel.github.io/OpenStudio-user-documentation/help/troubleshooting/).

# Overview

The biggest new feature in OpenStudio 2.2.0 is the integration of the OpenStudio Geometry Editor in the OpenStudio Application. The [OpenStudio Geometry Editor](https://github.com/NREL/openstudio-geometry-editor) is a new, open source software module that developers can leverage to produce building energy modeling UIs that include geometry creation. The OpenStudio Geometry Editor is meant to cover simple building geometry use cases only. More complex building geometry is best developed in a full featured CAD tool and exported to gbXML for building energy modeling. The OpenStudio Geometry Editor is implemented in JavaScript with minimal dependencies, allowing it to be integrated into a wide range of applications. The OpenStudio Geometry Editor reads and writes a custom [floorplan.json](https://raw.githubusercontent.com/NREL/openstudio-geometry-editor/develop/schema/geometry_schema.json) JSON file format. New methods have been added to the OpenStudio SDK, which can translate this file format to OSM. Additionally, new methods have been added to the SDK, which allows two OpenStudio Models to be merged. These new methods are demonstrated by integrating the Geometry Editor directly within the OpenStudio Application. The OpenStudio Geometry Editor and integration within the OpenStudio Application are experimental, and are not recommended for production workflows. However, we invite users to try these software components out and provide feedback to help us make them better for use by third party developers.  Instructions for use are at: [http://nrel.github.io/OpenStudio-user-documentation/reference/geometry\_editor/](http://nrel.github.io/OpenStudio-user-documentation/reference/geometry_editor/)

**EnergyPlus**

Openstudio now incorporates EnergyPlus Release 8.8, which provides additional speed improvements, as well as bug fixes.

**OpenStudio**

Through workspace code optimization, large HVAC model creation time has been reduced by roughly 75%. An example test model which originally took 416 seconds to create, took only 101 seconds after this code improvement.

**DView**

For many years, OpenStudio has included ResultsViewer to visualize timeseries data.  In this release, ResultsViewer is now replaced by a visualization tool called DView which was originally developed for viewing residential building and solar energy simulation data. DView brings additional capabilities that ResultsViewer does not have, including IP unit conversion, CSV data import/export, image export, EPW weather file visualization, two synchronized views, stacked timeseries, average profiles, overview statistics, histograms, cumulative density plots, duration curves, and scatter plots.

## Known Issues

DView is not currently available in Ubuntu packages.  We hope to address this in the near future.

PAT&#39;s algorithmic method &#39;RGenoud&#39; is unstable. A software patch will be provided once a fix is in place.  Please use &#39;PSO&#39; until it&#39;s fixed.

## Issue Statistics Since Previous Release

- 81 new issues were filed since the 2.2.0 release of OpenStudio (not including opened pull requests).
- 42 issues were closed since the 2.2.0 release of OpenStudio (not including closed pull requests).
