# OpenStudio Version 3.7.0

_Release Notes_ - 11/17/2023

These release notes describe version 3.7.0 of the OpenStudio SDK developed by the National Renewable Energy Laboratory (NREL), Buildings and Thermal Sciences Center, Commercial Buildings Research Group, Tools Development Section, and associated collaborators. The notes are organized into the following sections:

-  Overview
-  Where to Find OpenStudio Documentation
-  Installation Notes
-  OpenStudio SDK: Changelog

# Overview

As of April 2020, development and distribution of the OpenStudioApplication and the SketchUp plugin have transitioned to the OpenStudio Coalition, who is independently managing and distributing the software through its own [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) repository. The OpenStudio SDK is continuing to be actively developed and distributed by NREL and is released two times per year, through a spring and a fall release.

Below is the list of components that is included in this SDK installer:

__**OpenStudio SDK 3.7.0**__
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

OpenStudio SDK 3.7.0 is supported on:

* 64-bit Windows 7 – 11
* macOS: 10.15+ x86_64, 12.1+ arm64
* Ubuntu: 20.04 x86_64, 22.04 x86_64, 22.04 arm64
* Centos7

OpenStudio SDK 3.7.0 supports [EnergyPlus Release 23.2.0](https://github.com/NREL/EnergyPlus/releases/tag/v23.2.0), which is bundled with the OpenStudio installer. It is no longer necessary to download and install EnergyPlus separately. Other builds of EnergyPlus are not supported by OpenStudio SDK 3.7.0.

OpenStudio SDK 3.7.0 supports Radiance 5.0.a.12, which is bundled with the OpenStudio installer; users no longer must install Radiance separately, and OpenStudio will use the included Radiance version regardless of any other versions that may be installed on the system. Other builds of Radiance are not supported by OpenStudio SDK 3.7.0.

As usual, you can refer to the **[OpenStudio SDK Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Version-Compatibility-Matrix)** for more information.


## Installation Steps

- Download and install [OpenStudio SDK](https://github.com/NREL/openstudio) and/or [openstudiocoalition/OpenStudioApplication](https://github.com/openstudiocoalition/OpenStudioApplication) depending on your needs. Select components for installation. Note that OpenStudio Application is a standalone app and does not require you to install OpenStudio SDK.
- Setup a Building Component Library (BCL) account to access online building components and measures. View instructions on how to setup your account and configure the key in OpenStudio.
- The OpenStudio Application SketchUp Plug-in requires SketchUp 2021-2022 (not available for Linux). The OpenStudio Application SketchUp Plug-in does not support older versions of SketchUp. SketchUp must be installed before OpenStudio Application to automatically activate the plugin. If you install SketchUp after OpenStudio Application, simply re-run the OpenStudio Application installer.

For help with common installation problems please visit, http://nrel.github.io/OpenStudio-user-documentation/getting_started/getting_started/.

# OpenStudio SDK:

The 3.7.0 is a **major** release. This update includes several new features, performance improvements, and bug fixes.

## C++ Workflow code

As of OpenStudio SDK 3.7.0 a re-written workflow written in C++ is used by default in place of the Ruby based Workflow Gem that had been used in the past. This enhancement is in support of Python measures being used in an OpenStudio workflow, including mixed language workflows that include both Ruby Measures and Python Measures. If you need to use the older Workflow Gem implementation, you can do that by using the `classic` subcommand after `openstudio`. `classic` will be deprecated in a future version of OpenStudio. 

## Python Bindings

As of OpenStudio SDK 3.2.0, Python bindings are officially supported and distributed through Python Package Index (PyPI). To install, users will need to have Python3 installed along with pip and simply run the following command in a terminal window.

`pip install openstudio==3.7.0`

Please see [openstudio on PyPi](https://pypi.org/project/openstudio/) for further instructions on how to install. Users can also visit the test channel at [openstudio on TestPyPi](https://test.pypi.org/project/openstudio/) to install development bindings.

You can also refer to the [OpenStudio SDK Python Binding Version Compatibility Matrix](https://github.com/NREL/OpenStudio/wiki/OpenStudio-SDK-Python-Binding-Version-Compatibility-Matrix) to see the list of supported platforms and python versions.

## New Features, Major Fixes and API-breaking changes

* Make the "labs" cli implementation the default by @kbenne in https://github.com/NREL/OpenStudio/pull/5006
* Fix #3831 -  New (labs) CLI command for `measure new` and `measure copy` by @jmarrec in https://github.com/NREL/OpenStudio/pull/4875
* Labs cli remove verbose in favor of `--loglevel `+ minor swig adjustments by @jmarrec in https://github.com/NREL/OpenStudio/pull/4922
* #4846 - Allow importing Python package with C-extension e.g. numpy in the labs CLI by @jmarrec in https://github.com/NREL/OpenStudio/pull/4868
* [internal] Schedule:File relative paths mod by @jmarrec in https://github.com/NREL/OpenStudio/pull/5010
* Schedule:File relative paths by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4879
* Improve weather file handling in OS Workflow by @kbenne in https://github.com/NREL/OpenStudio/pull/5011
* AirLoopHVACUnitarySystem set Method During XXX Operation - option, 2 by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4927
* AirLoopHVACUnitarySystem set Method During XXX Operation - option by @jmarrec in https://github.com/NREL/OpenStudio/pull/4905
* AirLoopHVACUnitarySystem set Method During XXX Operation by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4873
* Support undisturbed ground temperature models on GroundHeatExchangerVertical by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4932
* #4894 - `setAmbientTemperatureSchedule` and `setAmbientAirVelocitySchedule` setter for `PipeIndoor` do not work by @jmarrec in https://github.com/NREL/OpenStudio/pull/4901
* Fix #4869 - Handle AirToAirComponents (ERVs) when cloning AirLoopHVACOutdoorAirSystem by @jmarrec in https://github.com/NREL/OpenStudio/pull/4872
* Fix #4896 - SPM:MixedAir creation for CoilSystemCoolingWaterHeatExchangerAssisted does not work with FanSystemModel by @jmarrec in https://github.com/NREL/OpenStudio/pull/4899
* Fix #4926 - WindowPropertyFrameAndDivider: "Outside Reveal Depth" not forward translated  by @jmarrec in https://github.com/NREL/OpenStudio/pull/4959
* LoadProfilePlant: new Fluid Type and Degree of SubCooling fields by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4951
* ControllerOutdoorAir: new Economizer Operation Staging field by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4948
* Improve OpenStudio's support for Python unitttest by @kbenne in https://github.com/NREL/OpenStudio/pull/4973
* BoilerHotWater: new Off Cycle Parasitic Fuel Load field by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4949
* HeatPump:PlantLoop:EIR:*: new fields by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4962
* Update gbXML schema to v7.03 by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4995
* Update to EnergyPlus v23.2.0 official by @jmarrec in https://github.com/NREL/OpenStudio/pull/4989

## Minor changes and bug fixes

* Fix #5018 - C++ CLI: Calling a non existing method in a measure ends up with a crash and stack trace by @jmarrec in https://github.com/NREL/OpenStudio/pull/5023
* Fixing run workflow with outdated measures by @jmarrec in https://github.com/NREL/OpenStudio/pull/5015
* C++ CLI: Create results.json and handle analysis.json (create objectives.json) by @jmarrec in https://github.com/NREL/OpenStudio/pull/5028
* Don't delete the osw when it is in the run dir by @kbenne in https://github.com/NREL/OpenStudio/pull/5041
* Fix #5030 - C++ CLI: run.log is huge and missing the runner.registerWarning messages by @jmarrec in https://github.com/NREL/OpenStudio/pull/5035
* Fix #5037 - Adjust log formatters for workflow (stdout + run.log) by @jmarrec in https://github.com/NREL/OpenStudio/pull/5042
* Bump to rc3. by @wenyikuang in https://github.com/NREL/OpenStudio/pull/5043
* Add missing `add_result_measure_info` so that out.osw step result have the same info as before by @jmarrec in https://github.com/NREL/OpenStudio/pull/5044
* Allow edges to have same surface on both sides by @macumber in https://github.com/NREL/OpenStudio/pull/5003
* 4990 python run tests by @kbenne in https://github.com/NREL/OpenStudio/pull/5008
* Bump to rc2 by @wenyikuang in https://github.com/NREL/OpenStudio/pull/5012
* CSharp fixes by @jmarrec in https://github.com/NREL/OpenStudio/pull/4893
* Master master + Bump to 3.7.0-alpha and add default VT (+ Ubuntu 22.04 arm64 python bindings) by @jmarrec in https://github.com/NREL/OpenStudio/pull/4898
* AirLoopHVACUnitarySystem FT fixes by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4864
* Add surface method for `SurfacePropertyExposedFoundationPerimeter` by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4892
* Fix #4909 - Update license to new format by @jmarrec in https://github.com/NREL/OpenStudio/pull/4910
* Fix #4737 - Replace sprintf usage with fmt::format by @jmarrec in https://github.com/NREL/OpenStudio/pull/4902
* Fix #4824 - Use a unique_path in create_temporary_directory to avoid collision by @jmarrec in https://github.com/NREL/OpenStudio/pull/4903
* Fix compiler warning treated as error by @macumber in https://github.com/NREL/OpenStudio/pull/4866
* Fix #4837 - Geometry improvements - Detect incorrectly oriented surfaces in spaces, non convex spaces by @jmarrec in https://github.com/NREL/OpenStudio/pull/4843
* Fix #4911 - Avoid extra warning message in ShadingControl by @jmarrec in https://github.com/NREL/OpenStudio/pull/4914
* Deprecation handling - better error messages + compile-time error when time to deprecate by @jmarrec in https://github.com/NREL/OpenStudio/pull/4912
* Fix #4897 - openstudiolib.dll/libopenstudiolib.so not found without C++ API Component selected in IFW by @jmarrec in https://github.com/NREL/OpenStudio/pull/4900
* Fix #4918 - Add a `-l` / `--loglevel` argument to the labs CLI by @jmarrec in https://github.com/NREL/OpenStudio/pull/4919
* Add a swig out typemap for Json::Value + openstudio::path typemap in/out for Python + add toJSON() for Workfow files and BCLMeasure by @jmarrec in https://github.com/NREL/OpenStudio/pull/4917
* Fix #4931 - Install C# OpenStudio.dll properly. by @jmarrec in https://github.com/NREL/OpenStudio/pull/4933
* Shadowing introduced by mistake in #3973 by @jmarrec in https://github.com/NREL/OpenStudio/pull/4939
* Fix #4692 - Modify Model::load to use the VersionTranslator by @jmarrec in https://github.com/NREL/OpenStudio/pull/4923
* update allowed limits radiant changeover by @mdahlhausen in https://github.com/NREL/OpenStudio/pull/4925
* More detailed data documentation by @mdahlhausen in https://github.com/NREL/OpenStudio/pull/4953
* #4957 - Some copy constructors aren't declared in SWIG  by @jmarrec in https://github.com/NREL/OpenStudio/pull/4960
* Fix issue in create_temporary_directory (affecting XMLValidator with schematron) by @jmarrec in https://github.com/NREL/OpenStudio/pull/4961
* v23.2.0-IOFreeze: Coil and Unitary systems changes by @jmarrec in https://github.com/NREL/OpenStudio/pull/4958
* V23.2.0-IOFreeze - Fuel Type enum changes (DistrictHeatingWater / DistrictHeatingSteam) by @jmarrec in https://github.com/NREL/OpenStudio/pull/4968
* Fix #4969 - add `json_schemer` to embedded ruby gems by @jmarrec in https://github.com/NREL/OpenStudio/pull/4974
* Coil:Heating:*: on and off cycle name changes by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4970
* V23.2.0-IOFreeze - wrap `DistrictHeating:Steam` and rename `DistrictHeating` to `DistrictHeatingWater` by @jmarrec in https://github.com/NREL/OpenStudio/pull/4972
* Addresses #4941, wrap DistrictHeating:Steam by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4954
* Addresses #4946, allow TableLookup curves for Coil:*:WaterToAirHeatPump:EquationFit objects by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4950
* Fix #4978 - adjust definitions for boost 1.79 to work with modern clang/apple clang by @jmarrec in https://github.com/NREL/OpenStudio/pull/4984
* #4963 - AirLoopHVACUnitarySystem Default Supply Air Flow Rate Method When No Cooling or Heating is Required to None by @jmarrec in https://github.com/NREL/OpenStudio/pull/4971
* BoilerHotWater: on cycle name change by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4985
* Update to v23.2.0-IOFreeze by @joseph-robertson in https://github.com/NREL/OpenStudio/pull/4940
* Fix #4885 - Rewrite MeasureManager in C++ by @jmarrec in https://github.com/NREL/OpenStudio/pull/4920
* Fix #4975 - Build python bindings on windows for python 3.11 by @jmarrec in https://github.com/NREL/OpenStudio/pull/4986
* Fix #4977 - Address regression tests failures by @jmarrec in https://github.com/NREL/OpenStudio/pull/4988
* Add deprecated ruby method IdfObject::to_<deprecated> by @jmarrec in https://github.com/NREL/OpenStudio/pull/4991
* Fix #4997 - Add Workspace casting methods in python bindings by @jmarrec in https://github.com/NREL/OpenStudio/pull/4998
* Fix #4999 - Change latent degration constructor defaults for CoilCoolingWaterToAirHeatPump:XXXX by @jmarrec in https://github.com/NREL/OpenStudio/pull/5000
* Fix #4979 - Add operator __bool__ for optional in python bindings by @jmarrec in https://github.com/NREL/OpenStudio/pull/4987
* [Partial] Update openstudio gems with new TBD and openstudio extensio… by @wenyikuang in https://github.com/NREL/OpenStudio/pull/4996

## Known Issues / Limitations
* In some cases you may need to install an extra dependency needed by EnergyPlus on Linux and Mac. See this EnergyPlus issue for details https://github.com/NREL/EnergyPlus/issues/10284 
* While we udpated the gbXML schema to v7.03, the forward and reverse translation between gbXML and OSM does not support any new gbXML fields that may have been added.

**Full Changelog**: https://github.com/NREL/OpenStudio/compare/v3.6.0...v3.7.0

**New Contributors**:
* @mdahlhausen made their first contribution in https://github.com/NREL/OpenStudio/pull/4925
* @wenyikuang made their first contribution in https://github.com/NREL/OpenStudio/pull/4996