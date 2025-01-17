# OpenStudio Version 3.9.1

_Release Notes_ -  _TDB_

These release notes describe version 3.9.1 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Sciences Center, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog

# Overview

As of April 2020, development and distribution of the OpenStudioApplication and the SketchUp plugin have transitioned to the OpenStudio Coalition, who is independently managing and distributing the software through its own [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) repository. The OpenStudio SDK is continuing to be actively developed and distributed by NREL and is released two times per year, through a spring and a fall release.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.9.1**__
- EnergyPlus
- Command Line Interface (CLI)
- Radiance
- Ruby API
- Python API
- C++ SDK

**Note** that [PAT](https://github.com/NREL/OpenStudio-PAT) is not included in either the SDK or the OpenStudio Coalition's Application installers. You will need to install PAT separately which is distributed on the [OpenStudio-PAT](https://github.com/NREL/OpenStudio-PAT) GitHub page.

# Where to Find OpenStudio SDK Documentation

- OpenStudio SDK release documentation, including these release notes, tutorials, and other user documentation, is available at [https://www.openstudio.net/](https://www.openstudio.net/.)
- C++ API documentation is available at [OpenStudio SDK Documentation](https://openstudio-sdk-documentation.s3.amazonaws.com/index.html)
- Measure development documentation is available at [OpenStudio Measure Writer's Reference Guide](http://nrel.github.io/OpenStudio-user-documentation/reference/measure_writing_guide/ )
- A roadmap for planned features is available at [Roadmap](http://nrel.github.io/OpenStudio-user-documentation/getting_started/roadmap/)

# Installation Notes

OpenStudio SDK 3.9.1 is supported on:

* 64-bit Windows 7 – 11
* macOS: 11.6+ x86_64, 12.1+ arm64
* Ubuntu: 20.04 x86_64, 22.04 x86_64, 22.04 arm64
* Centos7

OpenStudio SDK 3.9.1 supports [EnergyPlus Release @EP_VERSION@](https://github.com/NREL/EnergyPlus/releases/tag/v@EP_VERSION@), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio SDK 3.9.1.

OpenStudio SDK 3.9.1 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio SDK 3.9.1.

As usual, you can refer to the **[OpenStudio SDK Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or any third party tool that embeds the OpenStudio SDK into their software such as [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- A BCL AUth Key is no longer needed to access content on [Building Component Library (BCL)](https://bcl.nrel.gov) BCL is typically accessed through third party OpenStudio applications to provide modelers access to building components and OpenStudio measures; however you an also browse it and download content using the BCL link above.

For help with common installation problems please visit [Getting Started](http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/).

# OpenStudio SDK: Changelog

The 3.9.1 is a **<minor/major>** release. This update includes several new features, performance improvements, and bug fixes.

## C++ Workflow code

As of OpenStudio SDK 3.7.0 a re-written workflow written in C++ is used by default in place of the Ruby based Workflow Gem that had been used in the past. This enhancement is in support of Python measures being used in an OpenStudio workflow, including mixed language workflows that include both Ruby Measures and Python Measures. If you need to use the older Workflow Gem implementation, you can do that by using the `classic` subcommand after `openstudio`. `classic` will be deprecated in a future version of OpenStudio.

## Python Bindings

As of OpenStudio SDK 3.2.0, Python bindings are officially supported and distributed through Python Package Index (PyPI). To install, users will need to have Python3 installed along with pip and simply run the following command in a terminal window.

`pip install openstudio==3.9.1`

Please see [openstudio on PyPi](https://pypi.org/project/openstudio/) for further instructions on how to install. Users can also visit the test channel at [openstudio on TestPyPi](https://test.pypi.org/project/openstudio/) to install development bindings.

You can also refer to the [OpenStudio SDK Python Binding Version Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Python-Binding-Version-Compatibility-Matrix) to see the list of supported platforms and python versions.

## New Features, Major Fixes and API-breaking changes

* [#5326](https://github.com/NREL/OpenStudio/pull/5326) - Wrap ZoneHVAC:EvaporativeCoolerUnit
    * The object was wrapped in the SDK.
    * Note: in EnergyPlus 24.2.0, the `Zone Relief Air Node Name` is an optional field. The OpenStudio SDK always fills with the connected zone's Exhaust Air Node, meaning the airflow is always being balanced by EnergyPlus: the object will extract air from the zone to balance the air supplied to the zone by the cooler outlet node.

## Minor changes and bug fixes

Refer to the changelog on the release page at [v3.9.1](https://github.com/NREL/OpenStudio/releases/v3.9.1)

**Full Changelog**: https://github.com/NREL/OpenStudio/compare/v3.9.0...v3.9.1

**New Contributors**:

### OpenStudio Standards v@STANDARDS_VERSION@

Update the openstudio-standards gem to version [@STANDARDS_VERSION@](https://github.com/NREL/openstudio-standards/releases/tag/v@STANDARDS_VERSION@)
In addition to some refactoring, this release also included conversion of 90.1 data to formal database.


### OpenStudio Server vXXX




---
# This YAML header controls the pandoc (via TeX) to PDF settings
# To convert the markdown to pdf, do `pandoc release_notes.md -o release_notes.pdf`
title:  'OpenStudio Release Notes - 3.9.1'
author:
- National Renewable Energy Laboratory
colorlinks: true
linkcolor: blue
urlcolor: Mahogany
toccolor: gray
geometry:
- margin=1in
---
