## OpenStudio v3.5.0

###### 2022-11-10 — [Diff]( https://github.com/NREL/OpenStudio/compare/v3.4.0...v3.5.0 )

- ![Improved:][improved] [#4689](https://github.com/NREL/OpenStudio/pull/4689),[#4702](https://github.com/NREL/OpenStudio/pull/4702),[#4694](https://github.com/NREL/OpenStudio/pull/4694),[#4674](https://github.com/NREL/OpenStudio/pull/4674),[#4641](https://github.com/NREL/OpenStudio/pull/4641),[#4688](https://github.com/NREL/OpenStudio/pull/4688),[#4653](https://github.com/NREL/OpenStudio/pull/4653) - Phase 1 support to run Python based measures. 
    * Currently, this functionality is experimental and can only be accessed from the CLI using the experimental `labs` subcommand. This also includes a redesigned help menu (`openstudio labs --help`) to explore the various new options this new feature has to offer. 
    * Included in the installation are example .osw workflow files that contain Python based measures that can be ran using the labs subcommand. For instance, the following will run a python based workflow: `openstudio run -w ./Examples/compact_osw/compact_python_only.osw`
    * For this release, only Python or Ruby can be ran in a single workflow file. Running both Python and Ruby support is planned for the next phase 2 installment in v3.6.0
- ![Improved:][improved] [#4600](https://github.com/NREL/OpenStudio/pull/4600) - Phase 1 support to run Python EMS plugin using OpenStudio.
- ![Improved:][improved] [#4623](https://github.com/NREL/OpenStudio/pull/4623) - FloorspaceJS to OpenStudio direct reverse translator vs the 3-way process of converting FloorspaceJS to ThreeJS to OSM. 
- ![Improved:][improved] [#4571](https://github.com/NREL/OpenStudio/pull/4571) - Metadata mapped to GLTF data object at export. Allows users to view metadata in a glTF capable browser. 
- ![Improved:][improved] [#4587](https://github.com/NREL/OpenStudio/pull/4587) - Error handling and gbXML schema validation when running gbXML based workflows. 
- ![Improved:][improved] [#4616](https://github.com/NREL/OpenStudio/pull/4616) - Addresses #4611, allow non-Quadratic curves for the EIR-f-PLR for the Chiller:Electric:EIR object
    * `Chiller:Electric:EIR` has a few API-breaking changes related to its Curves. The types for the constructor, getters and setters used to be explicit (eg: `CurveBiquadratic`): it is now the base class, more generic, `Curve` type for added flexibility.
- ![Improved:][improved] [#4642](https://github.com/NREL/OpenStudio/pull/4642) - Addresses #4575, API change for EvaporativeCoolerFluid:SingleSpeed
    * `EvaporativeCoolerFluid:SingleSpeed` has an API-breaking change related to its `performanceInputMethod` getter. It is now a required field that returns `std::string` instead of `boost::optional<std::string>`.
- ![Improved:][improved] [#4644](https://github.com/NREL/OpenStudio/pull/4644) - Addresses #4575, API change for Coil:Cooling:DX:SingleSpeed and Coil:Cooling:DX:TwoSpeed
    * `Coil:Cooling:DX:SingleSpeed` and `Coil:Cooling:DX:TwoSpeed` have API-breaking changes related to many of their getters. They are now required fields that return `double` instead of `boost::optional<double>`. Also removes many deprecated methods.
- ![Improved:][improved] [#4632](https://github.com/NREL/OpenStudio/pull/4632) - Remove deprecated methods for AirWallMaterial, Node, SizingSystem, ZoneAirMassFlowConservation
    * Removes the deprecated `AirWallMaterial` class completely, in favor of `ConstructionAirBoundary`
    * Removes functions in `Node` that have been deprecated for 9+ years
    * Removes deprecated methods in the `SizingSystem`, `ZoneAirMassFlowConservation`, `PlantLoop` & `AirLoopHVAC`, `AvailablilityManagerNightCycle`, `ZoneVentilationDesignFlowRate` classes
    * Refer to the CSV file at [developer/ruby/deprecated_methods.csv](https://github.com/NREL/OpenStudio/blob/develop/developer/ruby/deprecated_methods.csv) for more information
- ![Improved:][improved] [#4665](https://github.com/NREL/OpenStudio/pull/4665) - Deprecate methods on Coil DX objects
    * `Coil:Cooling:DX:SingleSpeed`, `Coil:Cooling:DX:MultiSpeed:StageData`, and `Coil:Cooling:DX:CurveFit:Speed`: `ratedEvaporatorFanPowerPerVolumeFlowRate` and `setRatedEvaporatorFanPowerPerVolumeFlowRate` in favor of `ratedEvaporatorFanPowerPerVolumeFlowRate2017` and `setRatedEvaporatorFanPowerPerVolumeFlowRate2017`
    * `Coil:Heating:DX:SingleSpeed` and `Coil:Heating:DX:MultiSpeed:StageData`: `ratedSupplyFanPowerPerVolumeFlowRate` and `setRatedSupplyFanPowerPerVolumeFlowRate` in favor of `ratedSupplyFanPowerPerVolumeFlowRate2017` and `setRatedSupplyFanPowerPerVolumeFlowRate2017`
- ![Improved:][improved] [#4666](https://github.com/NREL/OpenStudio/pull/4666) - Changes related to availability schedule methods
    * `Coil:Heating:Gas:MultiStage` has an API-breaking change related to its `availabilitySchedule` getter. It is now a required field that returns `Schedule` instead of `boost::optional<Schedule>`. Method `resetAvailabilitySchedule` is also removed.
- ![Improved:][improved] [#4701](https://github.com/NREL/OpenStudio/pull/4701) - `ZoneHVACPackagedTerminalAirConditioner` and `ZoneHVACPackagedTerminalHeatPump`
    * `ZoneHVACPackagedTerminalAirConditioner` and `ZoneHVACPackagedTerminalHeatPump` have an API-breaking change related to its `supplyAirFanOperatingModeSchedule` getter. It is now a required field that returns `Schedule` instead of `boost::optional<Schedule>`. Method `resetSupplyAirFanOperatingModeSchedule` is also removed.
        * It is set to `alwaysOnDiscreteSchedule` (=Constant) in the Constructor if you provide a `FanConstantVolume` (This is **required** by E+)
        * It is set to `alwaysOffDiscreteSchedule` (=Cycling) in the Constructor if you provide any other fan types (E+ treats a blank schedule as always off)
    * There are unusual `VersionTranslator` Rules for Packaged Systems (PTAC or PTHP) that use a `FanConstantVolume` and that do not have a `Supply Air Fan Operating Mode Schedule`. In 22.1.0 this would effectively, and mistakenly, function as a cycling fan, but this is now disallowed in E+ 22.2.0. In order to retain a similar functionality and energy usage, the `FanConstantVolume` will be replaced by a `FanSystemModel` with an Always Off Schedule (=cycling fan, similar to a `Fan:OnOff`), mapping inputs such as pressure rise and efficiency appropriately.
- ![Improved:][improved] [#4484](https://github.com/NREL/OpenStudio/pull/4484) - Enable C++20
- ![Improved:][improved] [#4671](https://github.com/NREL/OpenStudio/pull/4671) - New Coil:*:WaterToAirHeatPump:EquationFit fields
- ![Improved:][improved] [#4698](https://github.com/NREL/OpenStudio/pull/4698) - Update EnergyPlus to v22.2.0
- ![Improved:][improved] [#4685](https://github.com/NREL/OpenStudio/pull/4685) - Address #4630, wrap Output:Schedules and Output:Constructions objects
- ![Improved:][improved] [#4577](https://github.com/NREL/OpenStudio/pull/4577) - gbXML translation fixes and enhancements
- ![Improved:][improved] [#4610](https://github.com/NREL/OpenStudio/pull/4610) - Addresses #4538, wrap phase change material properties
- ![Improved:][improved] [#4625](https://github.com/NREL/OpenStudio/pull/4625) - Addresses #4615, wrap OutputControl:Table:Style and Output:SQLite
- ![Improved:][improved] [#4580](https://github.com/NREL/OpenStudio/pull/4580) - Add color for Foundation boundary condition
- ![Improved:][improved] [#4568](https://github.com/NREL/OpenStudio/pull/4568) - Installer adjustments for clang 13.1.6+, and some mac arm64 adjustments
- ![Fixed:][fixed] [#4585](https://github.com/NREL/OpenStudio/pull/4585) - Add libintl.8.dylib to mac15 x86 for e+
- ![Improved:][improved] [#4589](https://github.com/NREL/OpenStudio/pull/4589) - Switch to E+ Space FT by default
- ![Improved:][improved] [#4579](https://github.com/NREL/OpenStudio/pull/4579) - Add Volume field to Space
- ![Fixed:][fixed] [#4593](https://github.com/NREL/OpenStudio/pull/4593) - Fix ZoneInfiltration:DesignFlowRate FT when Space FT enabled
- ![Fixed:][fixed] [#4597](https://github.com/NREL/OpenStudio/pull/4597) - Hotfix GLTF extras broke running tests in Debug
- ![Improved:][improved] [#4594](https://github.com/NREL/OpenStudio/pull/4594) - Bump OS ruby to use fPIC and zlib/minizip to 1.2.12 to match
- ![Fixed:][fixed] [#4590](https://github.com/NREL/OpenStudio/pull/4590) - Correct typo: Newell, not Newall
- ![Improved:][improved] [#4316](https://github.com/NREL/OpenStudio/pull/4316) - Addresses #2610, insertComponent can create duplicate unique model objects
- ![Improved:][improved] [#4604](https://github.com/NREL/OpenStudio/pull/4604) - Cached getters for all unique model objects
- ![Improved:][improved] [#4605](https://github.com/NREL/OpenStudio/pull/4605) - Gltf Refactor
- ![Fixed:][fixed] [#4612](https://github.com/NREL/OpenStudio/pull/4612) - Fix build error due to GLTF refactor on Windows
- ![Fixed:][fixed] [#4596](https://github.com/NREL/OpenStudio/pull/4596) - Bump conan dependencies (which will allow MSVC 2022 and C++20) and clang 13.1+ fixes
- ![Fixed:][fixed] [#4614](https://github.com/NREL/OpenStudio/pull/4614) - Support Ubuntu 18.04 and Centos7
- ![Fixed:][fixed] [#4619](https://github.com/NREL/OpenStudio/pull/4619) - Fix #4543 - E+ 22.1.0: Wrap SetpointManager:SystemNodeReset:Temperature and SetpointManager:SystemNodeReset:Humidity
- ![Fixed:][fixed] [#4622](https://github.com/NREL/OpenStudio/pull/4622) - Addresses #4620, harmonize (remove?) max. material thickness limit
- ![Fixed:][fixed] [#4627](https://github.com/NREL/OpenStudio/pull/4627) - Fix #4547 - FT always warns about missing Design Specification Outdoor Air for AirTerminalSingleDuctInletSideMixer
- ![Fixed:][fixed] [#4640](https://github.com/NREL/OpenStudio/pull/4640) - Fix python bindings github workflows with C++20
- ![Fixed:][fixed] [#4626](https://github.com/NREL/OpenStudio/pull/4626) - Fix #4601 - add some useful log messages when requested WWR is rejected
- ![Fixed:][fixed] [#4592](https://github.com/NREL/OpenStudio/pull/4592) - Fix#127 - Fix volume calculation to match EnergyPlus
- ![Fixed:][fixed] [#4621](https://github.com/NREL/OpenStudio/pull/4621) - Fix #4551 - Add minimum/maximum values to numeric OSArguments and use it in validateUserArgument
- ![Fixed:][fixed] [#4643](https://github.com/NREL/OpenStudio/pull/4643) - Update docs for setThermalConductance and setThermalResistance changing thickness
- ![Fixed:][fixed] [#4650](https://github.com/NREL/OpenStudio/pull/4650) - Enable centos7 for c++20 
- ![Fixed:][fixed] [#4629](https://github.com/NREL/OpenStudio/pull/4629) - Addresses #3666, CoilWaterHeaterDesuperheater: Issue Error in ForwardTranslator if Heat Reclaim Efficiency is out of bounds 
- ![Fixed:][fixed] [#3912](https://github.com/NREL/OpenStudio/pull/3912) - Add consistently failing test for warnings/errors and document
- ![Fixed:][fixed] [#4669](https://github.com/NREL/OpenStudio/pull/4669) - Fix #4668 - Wrap E+ 22.2 new People fields
- ![Fixed:][fixed] [#4287](https://github.com/NREL/OpenStudio/pull/4287) - Build and test CSharp bindings for Linux/Mac/Windows
- ![Fixed:][fixed] [#4652](https://github.com/NREL/OpenStudio/pull/4652) - Addresses #4647, wrap the E+ Table:Lookup, Table:IndependentVariableList, and Table:IndependentVariable objects 
- ![Fixed:][fixed] [#4679](https://github.com/NREL/OpenStudio/pull/4679) - Remove cruft
- ![Improved:][improved] [#4672](https://github.com/NREL/OpenStudio/pull/4672) - Fix #4645 - Update FT for space-level infiltration/ventilation objects (E+ 22.2.0-IOFreeze)
- ![Fixed:][fixed] [#4676](https://github.com/NREL/OpenStudio/pull/4676) - Fix #4648 #4654 - SurfaceProperty:GroundSurfaces and SurfaceProperty:IncidentSolarMultiplier
- ![Fixed:][fixed] [#4681](https://github.com/NREL/OpenStudio/pull/4681) - Fix glass U factor sql error
- ![Fixed:][fixed] [#4661](https://github.com/NREL/OpenStudio/pull/4661) - Volume, CeilingHeight, FloorArea for Space object
- ![Fixed:][fixed] [#4660](https://github.com/NREL/OpenStudio/pull/4660) - Fix #4120 - set boost's visibility to global/default instead of hidden
- ![Fixed:][fixed] [#4680](https://github.com/NREL/OpenStudio/pull/4680) - Update ruby gems including the new tbd gem
- ![Fixed:][fixed] [#4686](https://github.com/NREL/OpenStudio/pull/4686) - Addresses #4599, documentation for isGroundSurface() is missing an outside boundary condition
- ![Fixed:][fixed] [#4677](https://github.com/NREL/OpenStudio/pull/4677) - Fix #4675 - E+ 22.2.0 - Sizing:Zone has new fields 
- ![Fixed:][fixed] [#4691](https://github.com/NREL/OpenStudio/pull/4691) - Followup to #4575 
- ![Fixed:][fixed] [#4662](https://github.com/NREL/OpenStudio/pull/4662) - Support CoilCoolingDX on ZoneHVAC, CoilSystem, Desuperheater
- ![Fixed:][fixed] [#4694](https://github.com/NREL/OpenStudio/pull/4694) - Fix #4638 python ruby workflow
- ![Fixed:][fixed] [#4687](https://github.com/NREL/OpenStudio/pull/4687) - Fix #4656 - E+ 22.2.0: Wrap Chiller:Electric:ASHRAE205 
- ![Fixed:][fixed] [#4697](https://github.com/NREL/OpenStudio/pull/4697) - Volume, CeilingHeight, FloorArea for Space object
- ![Fixed:][fixed] [#4658](https://github.com/NREL/OpenStudio/pull/4658) - Update to v22.2.0 
- ![Fixed:][fixed] [#4683](https://github.com/NREL/OpenStudio/pull/4683) - Fix #4663 #4664 - Fix CoilCoolingDXMultiSpeed::clone and extend testing for AirLoopHVAC::clone
- ![Fixed:][fixed] [#4702](https://github.com/NREL/OpenStudio/pull/4702) - Mod to Python Engine
- ![Fixed:][fixed] [#4700](https://github.com/NREL/OpenStudio/pull/4700) - Fix #4673 - Allow setting ZoneMixing objects at Space level
- ![Fixed:][fixed] [#4710](https://github.com/NREL/OpenStudio/pull/4710) - Update gems for standard 0.2.17.rc1
- ![Fixed:][fixed] [#4712](https://github.com/NREL/OpenStudio/pull/4712) - Fix #4659 - Filesystem fixups
- ![Fixed:][fixed] [#4713](https://github.com/NREL/OpenStudio/pull/4713) - Deprecated methods that date back to as early as v2.5.0 have been removed from this release. Moving forward, it is our goal to support deprecated methods for three release cycles and then remove them. If you run into errors (e.g. `undefined method`), please reference this [list](https://github.com/NREL/OpenStudio/blob/develop/developer/ruby/deprecated_methods.csv) for the suggested replacement. 
- ![Improved:][improved] [OpenStudio-benchmarks](https://github.com/NREL/OpenStudio-benchmarks) - Added additional performance benchmark tests to improve SDK performance.

## OpenStudio v3.4.0

###### 2022-05-05 — [Diff]( https://github.com/NREL/OpenStudio/compare/v3.3.0...v3.4.0 )

- ![Improved:][improved] [#4560](https://github.com/NREL/OpenStudio/pull/4560), [#4540](https://github.com/NREL/OpenStudio/pull/4540) - Update to EnergyPlus [v22.1.0](https://github.com/NREL/EnergyPlus/releases/tag/v22.1.0)
- ![Improved:][improved] [#4550](https://github.com/NREL/OpenStudio/pull/4550) - Addresses #4403, add Sql helper methods to retrieve assembly U-factor, SHGC, and Visible Transmittance values for glazing systems
- ![Fixed:][fixed]  [#4456](https://github.com/NREL/OpenStudio/pull/4493) - Fix #4456 - Improve performance of OpenStudio::UnzipFile::extractAllFiles
- ![Fixed:][fixed]  [#4424](https://github.com/NREL/OpenStudio/pull/4424) - Fixes #4361 OpenStudio API-reported surface "netArea" (when dealing with Frame & Divider objects)
- ![Improved:][improved] [#4502](https://github.com/NREL/OpenStudio/pull/4502) - #4496 - Add gems cbor and msgpack and allow using a package from an openstudio-gems PR
- ![Fixed:][fixed]  [#4497](https://github.com/NREL/OpenStudio/pull/4497) - Addresses #4495, ElectricLoadCenterDistribution FT has incomplete charge/discharge logic
- ![Fixed:][fixed]  [#4494](https://github.com/NREL/OpenStudio/pull/4494) - Addresses #4483, OS:Coil:Heating:DX:MultiSpeed:StageData has no Name field
- ![Improved:][improved] [#4499](https://github.com/NREL/OpenStudio/pull/4499) - Addresses #4410, wrap AirflowNetwork:MultiZone:SpecifiedFlowRate
- ![Fixed:][fixed]  [#4510](https://github.com/NREL/OpenStudio/pull/4510) - Addresses #4509, bad nodes created for AirLoopHVAC:UnitarySystem with only cooling coil and supplemental heating coil
- ![Improved:][improved] [#4508](https://github.com/NREL/OpenStudio/pull/4508) - Addresses #4469, method to reverse translate the EnergyPlus ScheduleYear object to the OpenStudio ScheduleRuleset object
- ![Fixed:][fixed]  [#4525](https://github.com/NREL/OpenStudio/pull/4525) - Fixes #4387 gltf changes as per PR [4520](https://github.com/NREL/OpenStudio/pull/4520) comments/suggestions
- ![Fixed:][fixed]  [#4485](https://github.com/NREL/OpenStudio/pull/4485) - Fixes #4438 gbXML export order is not reproducible
- ![Fixed:][fixed]  [#4444](https://github.com/NREL/OpenStudio/pull/4444) - Fixes #4375 gbXML Import reverses floors/ceilings normals and surface types
- ![Fixed:][fixed]  [#4527](https://github.com/NREL/OpenStudio/pull/4527) - Fixes #4372 intersection issue found using create bar
- ![Fixed:][fixed]  [#4535](https://github.com/NREL/OpenStudio/pull/4535) - Fixes #4533 - Wrap SurfaceProperty:LocalEnvironment and SurfaceProperty:SurroundingSurfaces
- ![Improved:][improved] [#4513](https://github.com/NREL/OpenStudio/pull/4513) - Addresses #4457, support gbXML reverse translation where user-input `<Name>` is different from the ID
- ![Improved:][improved] [#4534](https://github.com/NREL/OpenStudio/pull/4534) - Addresses #4311, add water heaters (mixed or stratified) to the supply side of different plant loops using source/use side connections (e.g., to configure a heat recovery chiller)
- ![Fixed:][fixed]  [#4528](https://github.com/NREL/OpenStudio/pull/4528) - Cooling tower fixes (ctor for SingleSpeed, and missing IDD defaults) and model tests
- ![Improved:][improved] [#4520](https://github.com/NREL/OpenStudio/pull/4520) - Adds exports functionality to a use web standard glTF to replace deprecated three.js JSON format
- ![Fixed:][fixed]  [#4548](https://github.com/NREL/OpenStudio/pull/4548) - Addresses #4531, breaking changes for some model getters:
    * CoolingTower:*Speed
        * `evaporationLossMode` (`boost::optional<std::string>` to `std::string`)
        * `blowdownCalculationMode` (`boost::optional<std::string>` to `std::string`)
        * `cellControl` for CoolingTower:VariableSpeed
    * PlantLoop
        * `commonPipeSimulation` (`boost::optional<std::string>` to `std::string`)
        * add new methods: `isCommonPipeSimulationDefaulted`, `commonPipeSimulationValues`, and `validCommonPipeSimulationValues`
- ![Improved:][improved] [#4505](https://github.com/NREL/OpenStudio/pull/4505) Addresses #4477, improve handling of invalid values passed to SDK by adding optional switch for turning off IDD validity checking
- ![Improved:][improved] [#4504](https://github.com/NREL/OpenStudio/pull/4504) Fix #2941 - Add option to show workflow run stdout + style + timings
- ![Improved:][improved] [#4487](https://github.com/NREL/OpenStudio/pull/4487) - Add operator< for BCLComponent and BCLMeasure
- ![Fixed:][fixed]  [#4482](https://github.com/NREL/OpenStudio/pull/4482) - Fix #4481 - add missing `\ip-units W` to the OpenStudio.idd
- ![Fixed:][fixed]  [#4491](https://github.com/NREL/OpenStudio/pull/4491) - Fix #4490 - Expose RoofGeometry helpers to Swig bindings
- ![Improved:][improved] [#4498](https://github.com/NREL/OpenStudio/pull/4498) - Bump to 3.3.1 and add default VT
- ![Improved:][improved] [#4474](https://github.com/NREL/OpenStudio/pull/4474) - Better string representation of Matrix and Transformation in the bindings
- ![Improved:][improved] [#4475](https://github.com/NREL/OpenStudio/pull/4475) - Add a method on the FT to get known ft options
- ![Improved:][improved] [#4512](https://github.com/NREL/OpenStudio/pull/4512) - Update copyrights 2022
- ![Fixed:][fixed]  [#4553](https://github.com/NREL/OpenStudio/pull/4553) - HotFix #4550 - Remove getExteriorFenestrationValue from public API
- ![Fixed:][fixed]  [#4555](https://github.com/NREL/OpenStudio/pull/4555) - Adjustments to WindowPropertyFrameAndDivider and new model gtest
- ![Fixed:][fixed]  [#4562](https://github.com/NREL/OpenStudio/pull/4562) - Update idfs to V22.1.0
- ![Improved:][improved] [#4564](https://github.com/NREL/OpenStudio/pull/4564) - update openstudio gems for standards 0.2.16
- ![Improved:][improved] [#4518](https://github.com/NREL/OpenStudio/pull/4518) - Bump openstudio-gems, conan-openstudio-ruby and add support for Apple M1
- ![Improved:][improved] [#4558](https://github.com/NREL/OpenStudio/pull/4558) - Update macOS M1 (arm64) dependencies
- ![Improved:][improved] [OpenStudio-benchmarks](https://github.com/NREL/OpenStudio-benchmarks) - Added additional performance benchmark tests to improve SDK performance. 

## OpenStudio v3.3.0

###### 2021-11-05 — [Diff]( https://github.com/NREL/OpenStudio/compare/v3.2.1...v3.3.0 )

- ![Improved:][improved] [#4406](https://github.com/NREL/OpenStudio/pull/4406) - Update to EnergyPlus [v9.6.0_PlusSpaceFix1](https://github.com/NREL/EnergyPlus/releases/tag/v9.6.0_PlusSpaceFix1)
    * Experimental Support for the EnergyPlus 9.6.0 [space concept](https://github.com/NREL/EnergyPlus/blob/develop/design/FY2021/NFP-Spaces.md) was added
        * This feature is currently opt-in, either by manually calling `ForwardTranslator::setExcludeSpaceTranslation(false)` before calling `ForwardTranslator::translateModel(Model&)` or passing the CLI argument `--space-translation` to the `run` method (eg: `openstudio run --space-translation -w workflow.osw`)
        * A few limitations due to DaylightingControls/IlluminanceMap exist: in case one of these objects is found in a ThermalZone, spaces will be combined for that ThermalZone (historical behavior)
        * [#4471](https://github.com/NREL/OpenStudio/pull/4471) - Space PR: adjustments to make DaylightingControls work
        * [#4434](https://github.com/NREL/OpenStudio/pull/4434) - Addresses #4409, forward translate Space
        * [#4447](https://github.com/NREL/OpenStudio/pull/4447) - Add ForwardTranslator options to the CLI `run` method
        * [#4465](https://github.com/NREL/OpenStudio/pull/4465) - [Developer] Refactor Forward Translation of ThermalZone in particular for Space translation PR
    * **Note**: EnergyPlus due to the Space feature now enforces unicity of names within the `Zone`, `Space`, `ZoneList` and `SpaceList` objects. **As a result, we are now enforcing this in the `openstudio::model` namespace: the same name can no longer be used for objects of type `ThermalZone`, `Space`, `SpaceType`.**
- ![Fixed:][fixed] [#4350](https://github.com/NREL/OpenStudio/pull/4350) - Fix #3599 - Add new fields to control outdoor air for AirTerminalSingleDuctInletMixer
- ![Improved:][improved] [#4365](https://github.com/NREL/OpenStudio/pull/4365) - Addresses #4364, Availability Schedule for DaylightingControl
- ![Fixed:][fixed] [#4342](https://github.com/NREL/OpenStudio/pull/4342) - Fix #4327 - FanSystemModel electricPowerFraction should be optional
- ![Improved:][improved] [#4368](https://github.com/NREL/OpenStudio/pull/4368) - Addresses #4362, add support of HeatPump:PlantLoop:EIR:Heating and HeatPump:PlantLoop:EIR:Cooling to model air-to-water heat pump
- ![Improved:][improved] [#4382](https://github.com/NREL/OpenStudio/pull/4382) - Addresses #4380, completing SDK support of Tubular and Light Well Daylighting Devices
- ![Improved:][improved] [#4381](https://github.com/NREL/OpenStudio/pull/4381) - Addresses #3906, support new field 'Minimum Air Flow Turndown Schedule Name' for AirTerminals
- ![Improved:][improved] [#4407](https://github.com/NREL/OpenStudio/pull/4407) - Add AirLoopHVAC to ThreeJS user data by @antoine-galataud
- ![Improved:][improved] [#4402](https://github.com/NREL/OpenStudio/pull/4402) - Addresses #3910, realign OS:SizingPeriod:DesignDay with E+ IDD
- ![Fixed:][fixed] [#4417](https://github.com/NREL/OpenStudio/pull/4417) - Fix #4156 - Support nested subfolders in approved Measure directories
- ![Fixed:][fixed] [#4452](https://github.com/NREL/OpenStudio/pull/4452) - Fix #4448 - Issues with angles in DaylightingControl
- ![Fixed:][fixed] [#4317](https://github.com/NREL/OpenStudio/pull/4317) - Addresses #3147, add HeatExchanger:Desiccant:BalancedFlow
- ![Fixed:][fixed] [#4326](https://github.com/NREL/OpenStudio/pull/4326) - Fix Wrap E+ WaterHeater:Sizing object
- ![Fixed:][fixed] [#4319](https://github.com/NREL/OpenStudio/pull/4319) - Fix Wrap E+ FuelFactors object
- ![Fixed:][fixed] [#4418](https://github.com/NREL/OpenStudio/pull/4418) - Resolves #4395 Add in CLI option to use epJSON
- ![Fixed:][fixed] [#4347](https://github.com/NREL/OpenStudio/pull/4347) - Fix #227 - Actually use the EPW start year or start day of week when "UseWeatherFile" is used in YearDescription for assumedYear
    * Please note that before, while the IDD default was `UseWeatherFile`, it would typically use the Assumed Base Year instead (2009) which resulted in using `Thursday` as a Start Day of Week for simulation. Depending on your weather file, you might experience a different Start Day of Week which could lead to slight EUI deviations. More information can be seen on [NREL/OpenStudio-resources#153](https://github.com/NREL/OpenStudio-resources/pull/153)
- ![Fixed:][fixed] [#4389](https://github.com/NREL/OpenStudio/pull/4389) - Update BCL to use BCL2 (https://bcl2.nrel.gov/)
- ![Fixed:][fixed] [#4349](https://github.com/NREL/OpenStudio/pull/4349) - Fix #3514 - Make strings returned from openstudio to ruby UTF8
- ![Fixed:][fixed] [#4346](https://github.com/NREL/OpenStudio/pull/4346) - Addresses #4329 and #4330, issues with AirLoopHVAC:UnitaryHeatCool:VAVChangeoverBypass
- ![Fixed:][fixed] [#4356](https://github.com/NREL/OpenStudio/pull/4356) - Fix #4353 - Add a missing reference in E+ Idd for PythonPlugin:Instance (`ProgramNames`)
- ![Fixed:][fixed] [#4355](https://github.com/NREL/OpenStudio/pull/4355) - Fix #4354 - Issue AirLoopHVACOutdoorAirSystem::reliefComponents with ZoneHVACComponents: wrong order
- ![Fixed:][fixed] [#4348](https://github.com/NREL/OpenStudio/pull/4348) - Fix #2145 - Do not create an extra singlezone SPM humidity min created if humidifier downstream of fan
- ![Fixed:][fixed] [#4343](https://github.com/NREL/OpenStudio/pull/4343) - Fix #4335 - Should reset the air nodes when cloning a AirLoopHVACUnitarySystem
- ![Fixed:][fixed] [#4345](https://github.com/NREL/OpenStudio/pull/4345) - #4046 - Clarify the situation with output file in the ReportingMeasure Template
- ![Fixed:][fixed] [#4339](https://github.com/NREL/OpenStudio/pull/4339) - Fix #4338 - AirLoopHVAC_Impl::terminalForLastBranch doesn't handle Dual duct correctly
- ![Fixed:][fixed] [#4359](https://github.com/NREL/OpenStudio/pull/4359) - Addresses #4314, add ScheduleRuleset custom day methods
- ![Fixed:][fixed] [#4352](https://github.com/NREL/OpenStudio/pull/4352) - Fix #1571 - building.clone breaks space surface matching
- ![Fixed:][fixed] [#4378](https://github.com/NREL/OpenStudio/pull/4378) - Addresses #1515, Reverse Translator should retain thermal zone name from IDF instead of re-naming it
- ![Fixed:][fixed] [#4385](https://github.com/NREL/OpenStudio/pull/4385) - Fixes Crash in floorplanToThreeJS https://github.com/NREL/OpenStudio/issues/4384
- ![Fixed:][fixed] [#4398](https://github.com/NREL/OpenStudio/pull/4398) - ref #4397 - Fix some issues in our Init calls for embedded ruby
- ![Fixed:][fixed] [#4305](https://github.com/NREL/OpenStudio/pull/4305) - Fix #4304 - Minimal fix for GroundHeatExchanger:Vertical to avoid confusion between maximum and design flow rates
- ![Fixed:][fixed] [#4400](https://github.com/NREL/OpenStudio/pull/4400) - #4399 - Surface fixes
- ![Fixed:][fixed] [#4392](https://github.com/NREL/OpenStudio/pull/4392) - Fix #4390 - add OSRunner::getOptionalBoolArgumentValue
- ![Fixed:][fixed] [#4420](https://github.com/NREL/OpenStudio/pull/4420) - Fix #4419 - custom mapping for LCC Use Price Escalation
- ![Fixed:][fixed] [#4428](https://github.com/NREL/OpenStudio/pull/4428) - Avoid issues with openstudio_ruby revision not matching
- ![Fixed:][fixed] [#4440](https://github.com/NREL/OpenStudio/pull/4440) - Fix #4439 - Correctly Reverse Translate gbXML Schedules
- ![Fixed:][fixed] [#4450](https://github.com/NREL/OpenStudio/pull/4450) - Fix #4446 - add SizingSystem::autosizedOccupantDiversity.
- ![Fixed:][fixed] [#4454](https://github.com/NREL/OpenStudio/pull/4454) - Fix #4404 #4453 - Fix issues with ForwardTranslator options
- ![Fixed:][fixed] [#4431](https://github.com/NREL/OpenStudio/pull/4431) - Fixed an issue in Surface splitSurfaceForSubSurfaces
- ![Fixed:][fixed] [#4459](https://github.com/NREL/OpenStudio/pull/4459) - Fix #3426 - ZoneHVACUnitHeater components missing air inlet/outlet node names when Fan:OnOff
- ![Fixed:][fixed] [#4455](https://github.com/NREL/OpenStudio/pull/4455) - Fix #4445 - Fix API issues in Controller:WaterCoil
- ![Fixed:][fixed] [#4462](https://github.com/NREL/OpenStudio/pull/4462) - Fix #4461 - DesignSpecificationOutdoorAir: when combining, Outdoor Air Flow rate for ACH is off by 3600 when "Maximum" method is used
- ![Fixed:][fixed] [#4473](https://github.com/NREL/OpenStudio/pull/4473) - Fix #4472 - Modify Python in typemaps for `path` to accept string with Python 3 support
- ![Fixed:][fixed] [#4307](https://github.com/NREL/OpenStudio/pull/4307) - Standardize Curve IDD references like E+ does (Uni/Bi...variateFunctions) and remove hard check in model code
- ![Fixed:][fixed] [#4337](https://github.com/NREL/OpenStudio/pull/4337) - Fix Encode searchTerm in BCL (including spaces, not only "+" signs)
- ![Fixed:][fixed] [#4344](https://github.com/NREL/OpenStudio/pull/4476) - Update openstudio-gems for new versions: openstudio-extension-gem (v0.5.1), openstudio-workflow-gem (v2.3.1) and openstudio-standards (v0.2.15)
- ![Fixed:][fixed] [#4332](https://github.com/NREL/OpenStudio/pull/4332) - Fix StringHelpers openstudio::replace
- ![Fixed:][fixed] [#4324](https://github.com/NREL/OpenStudio/pull/4324) - Fix Do not Warn if Sql doesn't have a Year field
- ![Fixed:][fixed] [#4376](https://github.com/NREL/OpenStudio/pull/4376) - Fix #4309 - disable RUBYLIB env and use --include or -I using cli
- ![Improved:][improved] [#4401](https://github.com/NREL/OpenStudio/pull/4401) - Update to new stable tag from Github Actions CI (includes CP720 fix)
- ![Improved:][improved] [#4476](https://github.com/NREL/OpenStudio/pull/4476) - Update gems for workflow and extension gem updates
- ![Improved:][improved] [#4463](https://github.com/NREL/OpenStudio/pull/4463) - Add a full FT test for DaylightingControls (backport from Space PR)
- ![Improved:][improved] [#4467](https://github.com/NREL/OpenStudio/pull/4467) - Update unit tests for ElectricEquipmentITEAirCooled
- ![Improved:][improved] [#4383](https://github.com/NREL/OpenStudio/pull/4383) - Provide more options for installation of openstudio dlls
- ![Improved:][improved] [#4302](https://github.com/NREL/OpenStudio/pull/4302) - Fix Deprecation Warning of last argument as keyword parameters
- ![Improved:][improved] [#4421](https://github.com/NREL/OpenStudio/pull/4421) - Fix #4220 - Don't pass built in POD types by const ref& but by value
- ![Improved:][improved] [#4371](https://github.com/NREL/OpenStudio/pull/4371) - Github Actions to publish python bindings to pypi when it's a release and testing worked
- ![Improved:][improved] [#4466](https://github.com/NREL/OpenStudio/pull/4466) - Add Full detailed tests for the FT of SpaceInfiltration objects (whether assigned to space or spacetype)
- ![Improved:][improved] [#4468](https://github.com/NREL/OpenStudio/pull/4468) - Add a full FT test for ElectricEquipmentITEAirCooled (backport Space PR)
- ![Improved:][improved] [#4427](https://github.com/NREL/OpenStudio/pull/4427) - Fix #4423 - enable coverage on one of the linux runners
- ![Improved:][improved] [#4369](https://github.com/NREL/OpenStudio/pull/4369) - #4214 Add in automated testing for 32/64 bit nuget package

## OpenStudio v3.2.1

###### 2021-06-25 — [Diff]( https://github.com/NREL/OpenStudio/compare/v3.2.0...v3.2.1 )

- ![Fixed:][fixed] [#4307](https://github.com/NREL/OpenStudio/pull/4307) - Standardize Curve IDD references like E+ does (Uni/Bi...variateFunctions) and remove hard check in model code 
- ![Fixed:][fixed] [4337](https://github.com/NREL/OpenStudio/pull/4337) - Fix Encode searchTerm in BCL (including spaces, not only "+" signs)
- ![Fixed:][fixed] [#4344](https://github.com/NREL/OpenStudio/pull/4344) - Update openstudio-gems for new versions of measure-tester-gem (v0.3.1), openstudio-extension-gem (v0.4.3), openstudio-workflow-gem (v2.2.1) and openstudio-standards (v0.2.14). 
- ![Fixed:][fixed] [#4332](https://github.com/NREL/OpenStudio/pull/4332) -  Fix StringHelpers openstudio::replace
- ![Fixed:][fixed] [#4326](https://github.com/NREL/OpenStudio/pull/4326) - Fix Wrap E+ WaterHeater:Sizing object
- ![Fixed:][fixed] [#4324](https://github.com/NREL/OpenStudio/pull/4324) - FIx Do not Warn if Sql doesn't have a Year field
- ![Fixed:][fixed] [#4319](https://github.com/NREL/OpenStudio/pull/4319) - Fix Wrap E+ FuelFactors object
- ![Fixed:][fixed] [#4302](https://github.com/NREL/OpenStudio/pull/4302) - Fix Deprecation Warning of last argument as keyword parameters


## OpenStudio v3.2.0

###### 2021-04-04 — [Diff]( https://github.com/NREL/OpenStudio/compare/v3.1.0...v3.2.0 )


- ![Improved:][improved] [#4271](https://github.com/NREL/OpenStudio/pull/4271) - Update to EnergyPlus v9.5.0
    * Most of the actual changes were done in [#4212](https://github.com/NREL/OpenStudio/pull/4212) - Update to V9.5.0-IOFreeze
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

- ![Improved:][improved] [#4199](https://github.com/NREL/OpenStudio/pull/4199) - Add support for translating a Workspace/IdfFile to epJSON
- ![Improved:][improved] [#4253](https://github.com/NREL/OpenStudio/pull/4253) - Allow ZoneHVAC:TerminalUnit:VariableRefrigerantFlow to connect to AirLoopHVAC

- ![Fixed:][fixed] [#4180](https://github.com/NREL/OpenStudio/pull/4180) - Fixes [#4175](https://github.com/NREL/OpenStudio/issues/4175), issues with ft for ElectricLoadCenter:Distribution
    * `ElectricLoadCenterDistribution` objects are now possible with no generators / transformers
    * A default `ElectricLoadCenterDistribution` is no longer instantiated in the constructor of children objects: `GeneratorFuelCell`, `GeneratorMicroTurbine`, `GeneratorPhotovoltaic`, `GeneratorPVWatts`

- ![Fixed:][fixed] [#4153](https://github.com/NREL/OpenStudio/pull/4153) - Fix #3532 #4152 - Make heating/cooling coils optional for ZoneHVACLowTempVariableFlow and add missing fields to coils
    * This is an API breaking change, the coils are now optional so the getters have been modified to account for this.
    ```diff
    - HVACComponent heatingCoil() const;
    + boost::optional<HVACComponent> heatingCoil() const;
    - HVACComponent coolingCoil() const;
    + boost::optional<HVACComponent> coolingCoil() const;
    ```
- ![Improved:][improved] [#4221](https://github.com/NREL/OpenStudio/pull/4221) - Geometry improvements including fixing intersection issues
- ![Fixed:][fixed] [#4136](https://github.com/NREL/OpenStudio/pull/4136) - Fix #4125, Fix #4135 - GbXML ForwardTranslator: clear map&set, and add a modelToGbXMLString method
- ![Fixed:][fixed] [#4137](https://github.com/NREL/OpenStudio/pull/4137) - Addresses #4076, radiant system enhancements
- ![Fixed:][fixed] [#4149](https://github.com/NREL/OpenStudio/pull/4149) - Addresses #3706, add drain water heat exchanger methods to WaterUseConnections
- ![Fixed:][fixed] [#4188](https://github.com/NREL/OpenStudio/pull/4188) - Fix #4187 - speed up `IdfFile::save` (and derived, such as `Model::save`) and general serialization to string by using '\n' instead of `std::endl`
- ![Fixed:][fixed] [#4239](https://github.com/NREL/OpenStudio/pull/4239) -Fix #4226 - add int argument for ScheduleFile.setMinutesperItem method
- ![Fixed:][fixed] [#4061](https://github.com/NREL/OpenStudio/pull/4061) - Fix Cloning a DualDuct AirLoopHVAC breaks the existing loop
- ![Fixed:][fixed] [#4081](https://github.com/NREL/OpenStudio/pull/4081) - #4077 Remove name field from common HVAC data objects that are not visible to user
- ![Fixed:][fixed] [#4123](https://github.com/NREL/OpenStudio/pull/4123) - Addresses #4110, align PTAC and PTHP coils with EnergyPlus
- ![Fixed:][fixed] [#4173](https://github.com/NREL/OpenStudio/pull/4173) - Fixes FT of Design Maximum Continuous Input Power for ElectricLoadCenterStorageConverter
- ![Fixed:][fixed] [#4179](https://github.com/NREL/OpenStudio/pull/4179) - Fixes bug in SurfaceProperty:ExposedFoundationPerimeter IDD
- ![Fixed:][fixed] [#4180](https://github.com/NREL/OpenStudio/pull/4180) - Addresses #4175, issues with ft for ElectricLoadCenter:Distribution
- ![Fixed:][fixed] [#4211](https://github.com/NREL/OpenStudio/pull/4211) - Fix #4205 - WorkspaceObject::setString allows setting invalid names for ModelObjects 
- ![Fixed:][fixed] [#4218](https://github.com/NREL/OpenStudio/pull/4218) - Fix #4217 - Do not make an PlantEqOperationUncontrolled for a Component that is already on a PlantEquipmentOperationComponentSetpoint
- ![Fixed:][fixed] [#4245](https://github.com/NREL/OpenStudio/pull/4245) - Fix #4166 - Merging FloorSpaceJS can delete unique model Objects such as Facility, Building, Site (and children)
- ![Fixed:][fixed] [#4235](https://github.com/NREL/OpenStudio/pull/4235) - Fix #4111 - Shading:Building is improperly mapped when "World" coordinates are used for Rectangular Surfaces
- ![Fixed:][fixed] [#4256](https://github.com/NREL/OpenStudio/pull/4256) - Fix #4254 - Set ComponentType::COOLING for EvaporativeFluidCoolerTwoSpeed
- ![Fixed:][fixed] [#4243](https://github.com/NREL/OpenStudio/pull/4243) - Fix #4240 (and more): add missing \ip-units gal/min via a script

## OpenStudio v3.1.0

###### 2020-10-16 — [Diff]( https://github.com/NREL/OpenStudio/compare/v3.0.1...v3.1.0 )

- ![Improved:][improved] [#3960](https://github.com/NREL/OpenStudio/pull/3960) - Added support for building the C# bindings via `dotnet` CLI, including on Unix platforms
- ![Improved:][improved] [#3966](https://github.com/NREL/OpenStudio/pull/3966) - Closes [#3945](https://github.com/NREL/OpenStudio/issues/3945) - Support the EnergyPlus object SurfaceControl:MovableInsulation which can be used to add insulation surface(s) on top of opaque surfaces, such as a transparent coating that affects the radiative gains to a roof surface
- ![Improved:][improved] [#3959](https://github.com/NREL/OpenStudio/pull/3959) - Also included some improvements in the generated C# bindings by reducing build warnings and properly exposing some types via SWIG
    * `ScheduleTypeKey` (which is normally only use by OpenStudio internals in ScheduleTypeRegistry checks) previously mapped to a `std::pair<std::string, std::string>` which was SWIGed in ruby as an Array of strings of size two, but improperly exposed in C#. It now uses a dedicated helper class with two methods `ScheduleTypeKey::className()` and `ScheduleTypeKey::scheduleDisplayName()`
- ![Improved:][improved] [#3987](https://github.com/NREL/OpenStudio/pull/3987) - Closes [#3907](https://github.com/NREL/OpenStudio/issues/3907) - Support new Coil:Cooling:DX, Coil:Cooling:DX:CurveFit:Performance, Coil:Cooling:DX:CurveFit:OperatingMode, Coil:Cooling:DX:CurveFit:Speed objects
- ![Improved:][improved] [#4009](https://github.com/NREL/OpenStudio/pull/4009) - Closes [#4006](https://github.com/NREL/OpenStudio/issues/4006) - Add SwimmingPool:Indoor to SDK
- ![Fixed:][fixed] [#4050](https://github.com/NREL/OpenStudio/pull/4050) - Fixes [#3921](https://github.com/NREL/OpenStudio/issues/3921) - Some children of RefrigerationSystem can be added several times and produce a fatal when System is removed
    * `RefrigerationSystem` will now be enforcing unicity for all children. This was already the case for a few of the child objects such as Case and WalkIns, it is now the case for all objects (`RefrigerationCondenserCascade`, `RefrigerationSubcoolerMechanical` and `RefrigerationSubcoolerLiquidSuction`, etc.). What this means is that setter methods (or add methods in case of a list) will remove the child for any current RefrigerationSystem it is on first.
- ![Improved:][improved] [#4066](https://github.com/NREL/OpenStudio/pull/4066) - Multiple shading controls referenced by a single subsurface
    * `SubSurface` was historically the one referencing the `ShadingControl` object. Now it's a many-to-many relationship where `ShadingControl` has an extensible 'Sub Surface Name' field, and multiple `ShadingControl` objects can reference the same `SubSurface`. This is trickling down from a change introduced in EnergyPlus version 9.4, and specifically in PR [NREL/EnergyPlus#8196](https://github.com/NREL/EnergyPlus/pull/8196)
    * Methods in `SubSurface` have been deprecated but are kept for backward compatibility. They will be removed in a future version of OpenStudio:
        * `shadingControl()`: prefer `shadingControls()`
        * `setShadingControl(ShadingControl&)`: use `addShadingControl(SubSurface&)`, `addShadingControls(std::vector<SubSurface>&)` or `setShadingControls(std::vector<SubSurface>&)`
        * `resetShadingControl()`: use `removeAllShadingControls()` instead
    * All Shading Control Type values should now be supported. Refer to issue [#4074](https://github.com/NREL/OpenStudio/issues/4074) for more information
    * Fields 'Glare Control Is Active', 'Type of Slat Angle Control for Blinds', 'Slat Angle Schedule Name', 'Setpoint2', and 'Multiple Surface Control Type' are now implemented as well
- ![Fixed:][fixed] [#4026](https://github.com/NREL/OpenStudio/pull/4026) - Fix OpenStudio Windows crashed during save attempt
- ![Fixed:][fixed] [#4028](https://github.com/NREL/OpenStudio/pull/4028) - Fixes [#3835](https://github.com/NREL/OpenStudio/issues/3835) - Decrease overall load time of CLI and lazy load OpenStudio ruby bindings when possible
- ![Improved:][improved] [#4059](https://github.com/NREL/OpenStudio/pull/4059) - Add new OutputControl:Files object to SDK, which allows conditionally turning on/off output from EnergyPlus
- ![Improved:][improved] [#4101](https://github.com/NREL/OpenStudio/pull/4101) - Update EnergyPlus v940 official  
- ![Improved:][improved] [#3961](https://github.com/NREL/OpenStudio/pull/3963) - Update cmake to use included FindPython method and fix zlib's crypt.h
- ![Improved:][improved] [#3970](https://github.com/NREL/OpenStudio/pull/3970) - Update conan dependency fmt/6.2.0 and use option to define FMT_HEADER_ONLY
- ![Improved:][improved] [#3979](#https://github.com/NREL/OpenStudio/pull/3979), [#4004](https://github.com/NREL/OpenStudio/pull/4004), [#4071](https://github.com/NREL/OpenStudio/pull/4071) - Update openstudio-gems to include openstudio-extension v0.3.0, openstudio-workflow v2.1.0, openstudio-standards v0.2.12, openstudio_measure_tester v0.2.3
- ![Improved:][improved] [#4015](https://github.com/NREL/OpenStudio/pull/4015), [#4047](https://github.com/NREL/OpenStudio/pull/4047)- Updates the conan dependencies to try and pin version using hash
- ![Improved:][improved] [#4027](https://github.com/NREL/OpenStudio/pull/4027) - Include 64/32 bit universal nuget package in cmake build process.
- ![Improved:][improved] [#4029](https://github.com/NREL/OpenStudio/pull/4029) - Closes [#3832](https://github.com/NREL/OpenStudio/issues/3832) - Allow for silent install on windows
- ![Improved:][improved] [#4055](https://github.com/NREL/OpenStudio/pull/4055) - Closes [#4054](https://github.com/NREL/OpenStudio/issues/4054) - Add latitude, longitude, elevation accessors to FloorplanJS
- ![Improved:][improved] [#4068](https://github.com/NREL/OpenStudio/pull/4068) - Closes [#4067](https://github.com/NREL/OpenStudio/issues/4067) - Remove the ENV ruby bundle variables so the cli does not use them
- ![Fixed:][fixed] [#3957](https://github.com/NREL/OpenStudio/pull/3969) - Fix for ruby bundle changes. 
- ![Fixed:][fixed] [#3971](https://github.com/NREL/OpenStudio/pull/3971) - Fixes [#3943](https://github.com/NREL/OpenStudio/issues/3943) - ThreeJS should handle ConstructionAirBoundary 
- ![Fixed:][fixed] [#3973](#https://github.com/NREL/OpenStudio/pull/3973) - Fixes [#3972](https://github.com/NREL/OpenStudio/issues/3972) - ScheduleRuleset remove winterDD/summerDD/holiday ScheduleDay in reset. 
- ![Fixed:][fixed] [#3978](https://github.com/NREL/OpenStudio/pull/3978) - Fixes [#3976](https://github.com/NREL/OpenStudio/issues/3976) - Add 'Minimum Outdoor Dry-Bulb Temperature for Compressor Operation' to all Coil:Cooling:DX:XXX objects
- ![Fixed:][fixed] [#3980](https://github.com/NREL/OpenStudio/pull/3980) - Fixes [#2033](https://github.com/NREL/OpenStudio/issues/2033) - Properly connected the secondary/induced air node for ATUs
- ![Fixed:][fixed] [#3981](https://github.com/NREL/OpenStudio/pull/3981) - Fixes [#2977](https://github.com/NREL/OpenStudio/issues/2977) -  Make VAVReheat Method consistent with IDD and other objects
- ![Fixed:][fixed] [#3983](https://github.com/NREL/OpenStudio/pull/3983) - Fixes [#1058](https://github.com/NREL/OpenStudio/issues/1058) - Warn only if the file extension isn't OSM, OSC, IDF, IMF, or DDY
- ![Fixed:][fixed] [#3985](https://github.com/NREL/OpenStudio/pull/3985) - Fixes [#3926](https://github.com/NREL/OpenStudio/issues/3926), Fixes [#3984](https://github.com/NREL/OpenStudio/issues/3984) - Handle multiple AirLoopHVACs and assign the DSOA to all ControllerMechanicalVentilation objects. AirLoopHVAC translates the AirLoopHVACOutdoorAirSystem, which in turns translates the ControllerOutdoorAir
- ![Fixed:][fixed] [#3986](https://github.com/NREL/OpenStudio/pull/3986) - Fixes [#2797](https://github.com/NREL/OpenStudio/issues/2797) - Do not translate a ZoneHVACLowTempRadiant object if it doesn't have any surfaces with Internal Source
- ![Fixed:][fixed] [#3988](https://github.com/NREL/OpenStudio/pull/3988) - Fix bug in GenerateClass.rb
- ![Fixed:][fixed] [#3989](https://github.com/NREL/OpenStudio/pull/3989) - Fixes [#3076](https://github.com/NREL/OpenStudio/issues/3076) - Make openstudio::Vector "Enumerable" in ruby
- ![Fixed:][fixed] [#3993](https://github.com/NREL/OpenStudio/pull/3993) - Added logic to rejoin previously triangulated window geometry before calling computeTriangulation on the base surface. Fixed several bugs in joinAll and translation to ThreeJS format
- ![Fixed:][fixed] [#3995](https://github.com/NREL/OpenStudio/pull/3995) - Implements eql? and hash for UUID in Ruby so UUID can be used as a key in a Ruby Hash
- ![Fixed:][fixed] [#3996](https://github.com/NREL/OpenStudio/pull/3996) - Fixes [#3990](https://github.com/NREL/OpenStudio/issues/3990) - Add an API to CoilCoolingDXMultiSpeed to add/remove stages
- ![Fixed:][fixed] [#3998](https://github.com/NREL/OpenStudio/pull/3998) - Fixes [#3997](https://github.com/NREL/OpenStudio/issues/3997) - GbXMLReverseTranslator applies incorrect scaling to windows when unit isn't meter
- ![Fixed:][fixed] [#3999](https://github.com/NREL/OpenStudio/pull/3999) - Removes a "Propane deprecated" warning message that no longer makes sense
- ![Fixed:][fixed] [#4000](https://github.com/NREL/OpenStudio/pull/4000) - Fixes [#3314](https://github.com/NREL/OpenStudio/issues/3314) - GBXML translate spaces etc even if Facility and/or Building aren't isntantianted 
- ![Fixed:][fixed] [#4002](https://github.com/NREL/OpenStudio/pull/4002) - Fixes [#2100](https://github.com/NREL/OpenStudio/issues/2100) - Remove if defaulted statements in ForwardTranslateThermalZone
- ![Fixed:][fixed] [#4005](https://github.com/NREL/OpenStudio/pull/4005) - Fixes [#4001](https://github.com/NREL/OpenStudio/issues/4001) - gbXML issue on ForwardTranslation - surfaceType attribute written twice for SlabOnGrade
3_bind* to ensure proper escaping of parameters such as Zone Names etc  
- ![Fixed:][fixed] [#4008](https://github.com/NREL/OpenStudio/pull/4008) - Fixes [#1906](https://github.com/NREL/OpenStudio/issues/1906) - Use sqlite
- ![Fixed:][fixed] [#4010](https://github.com/NREL/OpenStudio/pull/4010) - Fixes [#4007](https://github.com/NREL/OpenStudio/issues/4007) - Model objects returned in inconsistent order
- ![Fixed:][fixed] [#4011](https://github.com/NREL/OpenStudio/pull/4011) - Fixes [#1675](https://github.com/NREL/OpenStudio/issues/1675) - ForwardTranslate WaterHeater:Mixed when not on loop but has Peak Use Flow Rate (standalone operation)
- ![Fixed:][fixed] [#4012](https://github.com/NREL/OpenStudio/pull/4012) - Fixes [#2895](https://github.com/NREL/OpenStudio/issues/2895) - Extend the list of acceptable SubSurface types to include Skylight
- ![Fixed:][fixed] [#4013](https://github.com/NREL/OpenStudio/pull/4013) - Fixes [32867](https://github.com/NREL/OpenStudio/issues/2867) - Sum of fraction radiant, fraction lost, etc can be > 1 
- ![Fixed:][fixed] [#4021](https://github.com/NREL/OpenStudio/pull/4021) - Fixes [#4014](https://github.com/NREL/OpenStudio/issues/4014) - Extend download time to BCL 
- ![Fixed:][fixed] [#4023](https://github.com/NREL/OpenStudio/pull/4023) - Fixes [#3936](https://github.com/NREL/OpenStudio/issues/3936) - SubSurface Type reset when assigned to surface
- ![Fixed:][fixed] [#4030](https://github.com/NREL/OpenStudio/pull/4030) - Fixes [#4030](https://github.com/NREL/OpenStudio/issues/4020) - AvailabilityManagerNightCycle: Ensure ModelObjectLists are handled in clone/remove
- ![Fixed:][fixed] [#4032](https://github.com/NREL/OpenStudio/pull/4032) - Fixes [#4016](https://github.com/NREL/OpenStudio/issues/4016) - Move VT from 2.9.0 to 3.0.0 for new fields at end of the ZoneHVAC:TerminalUnit:VariableRefrigerantFlow
- ![Fixed:][fixed] [#4037](https://github.com/NREL/OpenStudio/pull/4037) - Fixes [#3369](https://github.com/NREL/OpenStudio/issues/3369) - Add missing fields to AvailabilityManager:HybridVentilation
- ![Fixed:][fixed] [#4044](https://github.com/NREL/OpenStudio/pull/4044) - Fixes [#4039](https://github.com/NREL/OpenStudio/issues/4039) - Add field ' Design Return Air Flow Fraction of Supply Air Flow' to AirLoopHVAC
- ![Fixed:][fixed] [#4048](https://github.com/NREL/OpenStudio/pull/4048) - Fixes [#4036](https://github.com/NREL/OpenStudio/issues/4036) - Don't create windows or doors for plenums when translating from FloorspaceJSON
- ![Fixed:][fixed] [#4065](https://github.com/NREL/OpenStudio/pull/4065) - Fixes [#4033](https://github.com/NREL/OpenStudio/issues/4033), Fixes [#4034](https://github.com/NREL/OpenStudio/issues/4034) - Fix AVMList clone between different models
- ![Fixed:][fixed] [#4072](https://github.com/NREL/OpenStudio/pull/4072) - Fixes [#4064](https://github.com/NREL/OpenStudio/issues/4064) - RunPeriodControlSpecialDays are not Forward Translated 
- ![Fixed:][fixed] [#4080](https://github.com/NREL/OpenStudio/pull/4080) - Fixes [#4079](https://github.com/NREL/OpenStudio/issues/4079) -  Install E+ pythonwrapper lib via CMake
- ![Fixed:][fixed] [#4088](https://github.com/NREL/OpenStudio/pull/4088) - Hotfix for sql queries
- ![Fixed:][fixed] [#4093](https://github.com/NREL/OpenStudio/pull/4093) - Fixes [#4091](https://github.com/NREL/OpenStudio/issues/4091) - Update FuelType/EndUseFuelType (and Sql) to match E+ 9.4.0
- ![Fixed:][fixed] [#4097](https://github.com/NREL/OpenStudio/pull/4097) - Fixes [#4096](https://github.com/NREL/OpenStudio/issues/4096) - RT error for ZoneControlThermostat
- ![Fixed:][fixed] [#4099](https://github.com/NREL/OpenStudio/pull/4099) - Fixes [#4089](https://github.com/NREL/OpenStudio/issues/4089) - Confirm if idf_output_requests still works in reporting measure template
- ![Fixed:][fixed] [#4104](https://github.com/NREL/OpenStudio/pull/4104) - Fixes [#4102](https://github.com/NREL/OpenStudio/issues/4102) - Fuel Type Renames in EMS Actuator Names 
- ![Fixed:][fixed] [#4155](https://github.com/NREL/OpenStudio/pull/4115) - Fixes [#4109](https://github.com/NREL/OpenStudio/issues/4109) - Fixes ruby bindings issue on windows when working with Sqlite3 db


## OpenStudio v3.0.1

###### 2020-06-26 — [Diff]( https://github.com/NREL/OpenStudio/compare/v3.0.0...v3.0.1 )
- ![Fixed:][fixed] [#3951](https://github.com/NREL/OpenStudio/pull/3951)  - GbXMLReverseTranslator produces incorrect geometry with respect to the windows and exterior walls of the gbxml

## OpenStudio v3.0.0

###### 2020-04-27 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.9.1...v3.0.0 )

- ![Improved:][improved] [#3044](https://github.com/NREL/OpenStudio/issues/3044) Added support for `FanSystemModel` to the Model API
- ![Improved:][improved] `ThermalZone::addEquipment` is no longer exposed to the bindings to avoid confusion and problems, as you should always use `ZoneHVACComponent::addToThermalZone` instead (#2920, #3758)
- ![Improved:][improved] [#3814](https://github.com/NREL/OpenStudio/pull/3814) - Add ability to create Holiday Schedules in ScheduleRuleset
- ![Improved:][improved] [#3882](https://github.com/NREL/OpenStudio/pull/3882) - Add methods to edit `Output:Table:SummaryReports` in OpenStudio SDK
- ![Improved:][improved] [#3875](https://github.com/NREL/OpenStudio/pull/3875) - Add OutputDiagnostics, OutputDebuggingData and OutputJSON to model SDK
- ![Fixed:][fixed] [#3853](https://github.com/NREL/OpenStudio/pull/3853)  - Fixes bug where MasslessOpaqueMaterial incorrectly had methods for setting and getting conductivity, density, and specific heat

- ![Improved:][improved] [#3744](https://github.com/NREL/OpenStudio/issues/3744) - New native extension gems have been added into the CLI:
    * pycall v1.2.1 - Allows for calling Python functions from the Ruby language 
    * sqlite3 v1.3.13 - Ruby bindings for the SQLite3 embedded database
    * Oga v3.2 - Oga is an XML/HTML parser written in Ruby
    * jara_winker v.1.5.4 - implementation of Jaro-Winkler distance algorithm needed for newer versions of rubocop >= v0.80.0  
    
- ![Improved:][improved] [#23](https://github.com/NREL/openstudio-gems/pull/23) - Updated extension gems have been added into the CLI:
    * bundler v2.1.0
    * openstudio-extension v0.2.1
    * openstudio-workflow v2.0.0
    * openstudio-standards v0.2.11
    * openstudio_measure_tester v0.2.2  

- ![Improved:][improved] [#3796](https://github.com/NREL/OpenStudio/pull/3796) - Reporting Measure: you can now pass `model` to the `arguments()` method of a reporting measure, for easier setup of output variables 
- ![Improved:][improved] [#3847](https://github.com/NREL/OpenStudio/pull/3847) - Chiller:Electric:EIR node API refactor, ChillerAbsorption/ChillerAbsorption tertiary node refactor
    * `ChillerElectricEIR::designHeatRecoveryWaterFlowRate()` now returns an `OptionalDouble` instead of a `double` since it can be now be autosized
    * `ChillerElectricEIR` no longer has getter/setters that set/return std::string for Node Names. It also wraps two new E+ fields: "Condenser Heat Recovery Relative Capacity Fraction", "Heat Recovery Inlet High Temperature Limit Schedule Name" and "Heat Recovery Leaving Temperature Setpoint Node Name"
    * Added missing Tertiary nodes for `ChillerAbsorption`
    * For `ChillerAbsoprtion`, `ChillerAbsoprtionIndirect`, `ChillerElectricEIR`: 
        * `addToTertiaryNode` overriden to only allow supply side connection.
        * `addToNode` overriden to connect to tertiary loop if primary loop is already connected, and Node to connect to is on the supply side of a different plant loop than the current primary loop
- ![Improved:][improved] [#3913](https://github.com/NREL/OpenStudio/pull/3913) - E+ 9.3.0: Update ShadowCalculation to match object refactor in EnergyPlus
    * `ShadowCalculation::calculationMethod` was renamed to `ShadowCalculation::shadingCalculationUpdateFrequencyMethod` (same for setter, defaulted, and reset methods)
    * `ShadowCalculation::calculationFrequency` was renamed to `ShadowCalculation::shadingCalculationUpdateFrequency` (same for setter, defaulted, and reset methods)
    * `ShadowCalculation::polygonClippingAlgorithm()` and `ShadowCalculation::skyDiffuseModelingAlgorithm()`: return type changed from `OptionalString` (which was always initialized) to `std::string`
    * All new fields in E+ 9.3.0 were also added
- ![Improved:][improved] [#3846](https://github.com/NREL/OpenStudio/pull/3846) - `SiteWaterMainsTemperature` now defaults to the `CorrelationFromWeatherFile` Calculation Method just like in EnergyPlus.
- ![Improved:][improved] [#3846](https://github.com/NREL/OpenStudio/pull/3846) - `BoilerHotWater` "Design Water Outlet Temperature" field to match E+
- ![Fixed:][fixed] [#3868](https://github.com/NREL/OpenStudio/pull/3868) - Fix an error when querying `surfaces()` of `ZoneHVACLowTemperature` objects and surfaces have no construction
- ![Fixed:][fixed] [#3844](https://github.com/NREL/OpenStudio/pull/3844) - Fix crash in FT for `HeatPumpWaterToWaterEquationFitCooling/Heating` when they reference each other as companion coils
- ![Fixed:][fixed] [#3871](https://github.com/NREL/OpenStudio/pull/3871) - Remove deprecated Active Year/Institution fields in `OS:ClimateZones`
- ![Improved:][improved] [#3878](https://github.com/NREL/OpenStudio/pull/3878) - Reporting Frequencies (eg: for `OutputVariable`) now differentiates between `RunPeriod` and `Annual` frequencies
- ![Improved:][improved] [#3805](https://github.com/NREL/OpenStudio/pull/3805) - Support year in `EpwFile`/`EpwDataPoint`
- ![Fixed:][fixed] [#3813](https://github.com/NREL/OpenStudio/pull/3813) - Change default specific heat of new materials to match E+
- ![Improved:][improved] [#3849](https://github.com/NREL/OpenStudio/pull/3849) - Add a missing entry in ScheduleTypeRegistry for `SurfacePropertyOtherSideCoefficients`
- ![Improved:][improved] [#3855](https://github.com/NREL/OpenStudio/pull/3855) - Autosize `ZoneHVACUnitHeater` Maximum Water flow rate in constructor
- ![Fixed:][fixed] [#3857](https://github.com/NREL/OpenStudio/pull/3857) - Reverse translation of `WindowMaterial:Glazing` sets Optical Data type field incorrectly
- ![Fixed:][fixed] [#3858](https://github.com/NREL/OpenStudio/pull/3858) - Wrong `OS:Daylighting:Control` rotation angle translated
- ![Fixed:][fixed] [#3862](https://github.com/NREL/OpenStudio/pull/3862) - Minor change to the E+ IDD for `LifeCycleCost:UsePriceEscalation`
- ![Improved:][improved] [#3867]() - Reset Surface Wind/SunExposure to NoSun/NoWind when setting Outside Boundary Condition to `OtherSideCoefficients` Fixed in [#3916](https://github.com/NREL/OpenStudio/pull/3916)
- ![Improved:][improved] [#3903](https://github.com/NREL/OpenStudio/issues/3903) - Add new fields to PerformancePrecisionTradeoffs for E+ 9.3.0
- ![Improved:][improved] [#3898](https://github.com/NREL/OpenStudio/issues/3898) - Add Minimum zone ventilation efficiency 
- ![Improved:][improved] [#3894](https://github.com/NREL/OpenStudio/issues/3894) - SqlFile need to support the "Year" field for IlluminanceMaps 
- ![Improved:][improved] [#3892](https://github.com/NREL/OpenStudio/issues/3892) - Add Wrap SimulationControl's Do HVAC Sizing Simulation for Sizing Periods 
- ![Improved:][improved] [#3885](https://github.com/NREL/OpenStudio/issues/3885)- Allow "CorrelationFromWeatherFile" for SiteWaterMainsTemperature's calculation method
- ![Improved:][improved] [#3872](https://github.com/NREL/OpenStudio/issues/3872) - Adding advanced output variables 
- ![Improved:][improved] [#3786](https://github.com/NREL/OpenStudio/issues/3786) [#3787](https://github.com/NREL/OpenStudio/pull/3787)  - Allow SDK users to not have to write out SQLite files
- ![Improved:][improved] [#3785](https://github.com/NREL/OpenStudio/issues/3785) - Expose the ZoneVentilation:WindandStackOpenArea in the Model SDK  
- ![Fixed:][fixed] [#3938](https://github.com/NREL/OpenStudio/issues/3938) - Fix loading native ext gems for new gems that were added. 
- ![Fixed:][fixed] [#3917](https://github.com/NREL/OpenStudio/issues/3917) - Connecting two ThermalZones in series on an AirLoopHVAC demand branch shouldn't be allowed
- ![Fixed:][fixed] [#3888](https://github.com/NREL/OpenStudio/issues/3888) - Fix cloning issue for model objects. 
- ![Fixed:][fixed] [3866](https://github.com/NREL/OpenStudio/issues/3866) - Fix Forward translator Runtime Error if a model has a ZoneHVAC:LowTemperatureRadiant:VariableFlow but no constructions assigned
- ![Fixed:][fixed] [#3854](https://github.com/NREL/OpenStudio/issues/3854) - Fix Maximum Hot Water Flow Rate to autosize
- ![Fixed:][fixed] [#3823](https://github.com/NREL/OpenStudio/issues/3823) - Fix RoofGeometry build errors 
- ![Fixed:][fixed] [#3793](https://github.com/NREL/OpenStudio/issues/3793) - Fix ITE object supply/approach temperature difference schedule 
- ![Fixed:][fixed] [#3792](https://github.com/NREL/OpenStudio/issues/3792 ) - Fix in support of OpenStudio Application



## OpenStudio v2.9.1

###### 2019-12-06 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.9.0...v2.9.1 )
- ![Fixed:][fixed] [#3280]( https://github.com/NREL/OpenStudio/issues/3280) AirLoopHVAC:UnitarySystem missing control type SingleZoneVAV
- ![Fixed:][fixed] [#2991]( https://github.com/NREL/OpenStudio/issues/2991) LoadProfilePlant.new(model) assigns bad default schedule scheduletypelimits


# OpenStudio v2.9.0

###### 2019-10-11 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.8.1...v2.9.0 )


- ![Improved:][improved] [#3553]( https://github.com/NREL/OpenStudio/issues/3553)Added missing methods to model API, FT, RT and VT for HeaderedPumpsConstantSpeed and HeaderedPumpsVariableSpeed. These fields are related to the Design Sizing Method and allow you to enter electric power per flow rate, or per flow rate and head.
- ![Improved:][improved] [#3668]( https://github.com/NREL/OpenStudio/issues/3668) Updated spider gbXML viewer 
- ![Improved:][improved] [#3639]( https://github.com/NREL/OpenStudio/issues/3639) Added new Fields in AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass Added a new ‘Priority Control Mode’ choice `LoadPriority`, added the ‘Minimum Runtime Before Operating Mode Change’ field, and the ‘Plenum or Mixer Inlet Node Name’. For the latter, you can use `AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass::setPlenumorMixer(const Mixer& returnPathComponent)` to connect it to a `AirLoopHVAC:Mixer` or a `AirLoopHVAC:ReturnPlenum` object.
- ![Improved:][improved] [#3584]( https://github.com/NREL/OpenStudio/issues/3584) Added ‘Temperature Difference Between Cutout And Setpoint’ for ZoneControl:Thermostat. This field is added to the `ThermostatSetpointDualSetpoint` class and handled during ForwardTranslation
- ![Improved:][improved] [#3635]( https://github.com/NREL/OpenStudio/issues/3635) Translate AirWalls to Construction:AirBoundary 
- ![Improved:][improved] [#3654]( https://github.com/NREL/OpenStudio/issues/3654) Added ThermalStorage:Ice:Detailed Charging and Discharging Curve Variable Specifications 
- ![Improved:][improved] Updated EnergyPlus from v9.1 to v9.2
- ![Fixed:][fixed] [#3600]( https://github.com/NREL/OpenStudio/issues/3600) Display plantLoop supply equipment in OpenStudioApplication in the same order as the resulting PlantEquipmentList after ForwardTranslaton
- ![Improved:][improved] [#3651](https://github.com/NREL/OpenStudio/issues/3651) Updated idf translator for E+ change Table:MultiVariableLookup  In Energyplus version 9.2.0, the table objects such as Table:MultiVariableLookup object were replaced with a new Table:Lookup object. The corresponding OpenStudio API remains the same using TableMultiVariableLookup, and the appropriate adjustments have been made to OpenStudio's EnergyPlus translator to create valid Table:Lookup objects. For more information please review the New Feature Proposal for EnergyPlus https://github.com/NREL/EnergyPlus/blob/v9.2.0/design/FY2018/NFP-Table-Refactor.md.
- ![Improved:][improved] [#3594]( https://github.com/NREL/OpenStudio/issues/3594) Display non-zero north angle 
- ![Improved:][improved] [#3571]( https://github.com/NREL/OpenStudio/issues/3571) Adds ‘Calculation Method’ field to ShadowCalculation
- ![Improved:][improved] [#3604]( https://github.com/NREL/OpenStudio/issues/3604) Allow AirConditioner:VariableRefrigerant condenser Type=WaterCooled (and EvaporativelyCooled. VRFs can now be connected to the demand side of a PlantLoop, including in the OpenStudioApplication directly, be sure to update the performance curves if you do so.
- ![Fixed:][fixed] [#3347]( https://github.com/NREL/OpenStudio/issues/3347) Fixed Import ScheduleFile from IDF 
- ![Improved:][improved] [#3555]( https://github.com/NREL/OpenStudio/issues/3555)  Added gbXMLId and improving CADObjectId import 
- ![Fixed:][fixed] [#3590]( https://github.com/NREL/OpenStudio/issues/3590) Fixes crash in CLI if json native gem is installed
- ![Fixed:][fixed] [#3591]( https://github.com/NREL/OpenStudio/issues/3591) ForwardTranslate End Use by Subcategory for Fan:VariableVolume and Fan:ConstantVolume 
- ![Fixed:][fixed] [#3585]( https://github.com/NREL/OpenStudio/issues/3585) Fix FloorspaceJS json north axis not being imported into OS model 
- ![Improved:][improved] [#3420]( https://github.com/NREL/OpenStudio/issues/3420) Added exportToBCVTB field to EnergyManagementSystemGlobalVariable 
- ![Fixed:][fixed] [#2792]( https://github.com/NREL/OpenStudio/issues/2792) Fix crash when leaving tabs with combobox2 
- ![Improved:][improved] [#3567]( https://github.com/NREL/OpenStudio/issues/3567) Use Custom Blocks in Foundation Kiva which implements use of custom blocks in the Foundation:Kiva object. Custom blocks can be used to represent solid materials in the two-dimensional context that are not otherwise covered by the other Foundation:Kiva fields 
- ![Improved:][improved] [#3565]( https://github.com/NREL/OpenStudio/issues/3565), [#3647](https://github.com/NREL/OpenStudio/pull/3647) Add more heating source object type and tank object type enumerations for Coil:WaterHeating:Desuperheater object 
- ![Improved:][improved] [#3664](https://github.com/NREL/OpenStudio/issues/3664),[#3671](https://github.com/NREL/OpenStudio/pull/3671) Wrap ZoneProperty:UserViewFactors:bySurfaceName object which allows users to specify the thermal radiation exchange view factors between surfaces in a zone 
- ![Improved:][improved] [#3631]( https://github.com/NREL/OpenStudio/issues/3631) Wrap PerformancePrecisionTradeoffs object which enables users to choose certain options that speed up EnergyPlus simulations 
- ![Improved:][improved] [#3583]( https://github.com/NREL/OpenStudio/issues/3583) Change the Sequential Load Fractions on ZoneHVAC:EquipmentList into schedules 
- ![Improved:][improved] [#50]( https://github.com/NREL/OpenStudio/issues/50), [#3610](https://github.com/NREL/OpenStudio/pull/3610) ScheduleFixedInterval now with option to translate schedule to ScheduleFile using new CSVFile class 

## OpenStudio v2.8.1

###### 2018-06-19 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.8.0...v2.8.1 )

- ![Fixed:][fixed] [#3487](https://github.com/NREL/OpenStudio/issues/3487) Wrong "Temperature Setpoint Node Name" in CoilHeatingElectric 

## OpenStudio v2.8.0

###### 2018-04-11 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.7.0...v2.8.0 )


- ![Fixed:][fixed] [#3453](https://github.com/NREL/OpenStudio/issues/3453) Sql methods now make use of the Year field that was added in E+ 8.9.0 to avoid problems with leap years.
- ![Fixed:][fixed] [#3458](https://github.com/NREL/OpenStudio/issues/3458) Force unique names to match new E+ convention following re-factor of E+’s Input Processor. For example, LifeCycleCost objects can no longer bear the same name.
- ![Improved:][improved] [#3465](https://github.com/NREL/OpenStudio/issues/3465) Show Measure Directory Name in OpenStudio Application. This should make it easier to understand which measure is used even after the user has changed the editable name.
- ![Fixed:][fixed] [#3462]( https://github.com/NREL/OpenStudio/issues/3462) Remove non-implemented UtilityCost_* objects from API documentation
- ![Fixed:][fixed] [#3440]( https://github.com/NREL/OpenStudio/issues/3440) Fix an error in AirTerminalSingleDuctConstantVolumeFourPipeBeam::remove() method that was making the OpenStudio Application crash
- ![Fixed:][fixed] [#3449] (https://github.com/NREL/OpenStudio/issues/3449) Fixes to API for CoilSystemCoolingDXHeatExchangerAssisted and CoilSystemCoolingWaterHeatExchangerAssisted
- ![Fixed:][fixed] [#3452] (https://github.com/NREL/OpenStudio/issues/3452) Fix forward translator issues in CoilHeatingDesuperheater and CoilWaterHeatingDesuperheater
- ![Improved:][improved] AirLoop::clone was modified to reconnect any supply side coils to the same plant loops as the original coils. This method will clone an AirLoopHVAC with the following rationale:
    - Handle all non-branch components from both the supply and the demand side
    - On the demand side branches, place one terminal of each IddObjectType that is present on the original AirLoopHVAC
    - Clone any SetpointManagers and add them to the correct location
    - If the supply component that is cloned is connected to a PlantLoop, we try to the connect the clone to the same PlantLoop by adding a demand branch
- ![Improved:][improved] PlantLoop::clone was added and follows the same logic. This method will clone a Plant Loop with the following rationale:
    - Handle all non-branch components from both the supply and the demand side
    - Handle branch components on the supply side (between supply splitter and mixer)
    - Clone any SetpointManagers and add them to the correct location
    - If the component that is cloned is connected to another PlantLoop, we try to connect the clone to the same other PlantLoop (if comp is on the supply side, we add a demand branch to the other Plantloop)

## OpenStudio v2.7.0
###### 2018-10-12 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.6.0...v2.7.0 )

- ![Fixed:][fixed] [#78]( https://github.com/NREL/OpenStudio/issues/78 ), Zones Tab - Minor issue but why don't we display color for thermal zone like we do elsewhere (Bugzilla #546)
- ![Fixed:][fixed] [#297]( https://github.com/NREL/OpenStudio/issues/297 ), The save as dialog does not pre-populate the file name with the current filename. (Bugzilla #1162)
- ![Fixed:][fixed] [#402]( https://github.com/NREL/OpenStudio/issues/402 ), OS app HVAC and loads tab - water use equipment icon missing or wrong
- ![Fixed:][fixed] [#535]( https://github.com/NREL/OpenStudio/issues/535 ), Renamed loop doesn't get updated in pull down until you switch to "control tab" or change loops
- ![Fixed:][fixed] [#749]( https://github.com/NREL/OpenStudio/issues/749 ), When two spaces in zone have mis-matched "Part of total floor area" flag we should issue a warning on export ot IDF
- ![Fixed:][fixed] [#979]( https://github.com/NREL/OpenStudio/issues/979 ), Exterior lights don't make it through reverse translation
- ![Fixed:][fixed] [#1329]( https://github.com/NREL/OpenStudio/issues/1329 ), OS:WindowMaterial:GlazingGroup:Thermochromic inspector is wrong
- ![Fixed:][fixed] [#1445]( https://github.com/NREL/OpenStudio/issues/1445 ), Add schedule type limits to schedules in HVAC libraries
- ![Fixed:][fixed] [#1554]( https://github.com/NREL/OpenStudio/issues/1554 ), Always enable revert to saved
- ![Fixed:][fixed] [#1566]( https://github.com/NREL/OpenStudio/issues/1566 ), ptac default view in the inspector is not consistent with other zone hvac objects
- ![Fixed:][fixed] [#1717]( https://github.com/NREL/OpenStudio/issues/1717 ), Direction of fan in outdoor air supply
- ![Fixed:][fixed] [#1794]( https://github.com/NREL/OpenStudio/issues/1794 ), Minor update issue on Surface sub-tab of Space tab
- ![Fixed:][fixed] [#1797]( https://github.com/NREL/OpenStudio/issues/1797 ), "Shading" Sub-tab on building tab, pull down list shouldn't show "Space" as option
- ![Fixed:][fixed] [#1801]( https://github.com/NREL/OpenStudio/issues/1801 ), HeatPump:WaterHeater Inlet Air Zone Name Not Exposed
- ![Fixed:][fixed] [#1877]( https://github.com/NREL/OpenStudio/issues/1877 ), Edit tab for many zone objects have an extra non-functional iink sub-tab
- ![Fixed:][fixed] [#2000]( https://github.com/NREL/OpenStudio/issues/2000 ), Constructions from BCL can't be dropped on drop zones in spaces tab
- ![Fixed:][fixed] [#2057]( https://github.com/NREL/OpenStudio/issues/2057 ), Wrong IP units for controllerWaterCoil min/max flow
- ![Fixed:][fixed] [#2059]( https://github.com/NREL/OpenStudio/issues/2059 ), change Fan Efficiency field to Fan Total Efficiency
- ![Fixed:][fixed] [#2109]( https://github.com/NREL/OpenStudio/issues/2109 ), OS:ZoneAirContaminantBalance not translated
- ![Fixed:][fixed] [#2125]( https://github.com/NREL/OpenStudio/issues/2125 ), heat pump water heater (hpwh) object cooling/heating priority always 1
- ![Fixed:][fixed] [#2206]( https://github.com/NREL/OpenStudio/issues/2206 ), Deleting Evaporative cooler on relief branch of outside air system orphans node
- ![Fixed:][fixed] [#2274]( https://github.com/NREL/OpenStudio/issues/2274 ), Water Heat Trank associated with Heat Pump Water Heater can't be dragged from My Model to Plant Loop
- ![Fixed:][fixed] [#2343]( https://github.com/NREL/OpenStudio/issues/2343 ), Odd 'Rated Pump Head' display unit for LowTemperatureRadiant:ConstantFlow
- ![Fixed:][fixed] [#2357]( https://github.com/NREL/OpenStudio/issues/2357 ), OS app crashes under edge case on schedule tab
- ![Fixed:][fixed] [#2422]( https://github.com/NREL/OpenStudio/issues/2422 ), Export gbXML bug for shading surfaces
- ![Fixed:][fixed] [#2432]( https://github.com/NREL/OpenStudio/issues/2432 ), HVAC library empty in OpenStudioApp 2.0.0
- ![Fixed:][fixed] [#2458]( https://github.com/NREL/OpenStudio/issues/2458 ), WSHP, GSHP Plant loop temperatures 
- ![Fixed:][fixed] [#2468]( https://github.com/NREL/OpenStudio/issues/2468 ), Attempting to add WaterHeater:HeatPump to supply side of plant loop creates orphaned node
- ![Fixed:][fixed] [#2555]( https://github.com/NREL/OpenStudio/issues/2555 ), Larger font on Windows in vrf system view 
- ![Fixed:][fixed] [#2600]( https://github.com/NREL/OpenStudio/issues/2600 ), Apply Measure Now incorrectly reporting warning message count with data from info messages.
- ![Fixed:][fixed] [#2625]( https://github.com/NREL/OpenStudio/issues/2625 ), VAV air terminals missing Autosized option for Fixed Minimum Air Flow Rate field
- ![Fixed:][fixed] [#2633]( https://github.com/NREL/OpenStudio/issues/2633 ), Hide "Schedule" for SpaceInfiltrationDesignFlowRate in OS App
- ![Fixed:][fixed] [#2712]( https://github.com/NREL/OpenStudio/issues/2712 ), Add Ideal Air Load Column as Multi-Selectable in the Thermal Zones Tab
- ![Fixed:][fixed] [#2718]( https://github.com/NREL/OpenStudio/issues/2718 ), SystemSizing object N2, \field Central Heating Maximum System Air Flow Ratio
- ![Fixed:][fixed] [#2776]( https://github.com/NREL/OpenStudio/issues/2776 ), Spaces tab Filters do not work for all sub-tabs
- ![Fixed:][fixed] [#2805]( https://github.com/NREL/OpenStudio/issues/2805 ), Missing SPM:HumidityMinimum in 2.3.0 hvac_library
- ![Fixed:][fixed] [#2814]( https://github.com/NREL/OpenStudio/issues/2814 ), OSApp in Debug crashes when switch to "Humidity" subtab of DesignDayGridView
- ![Fixed:][fixed] [#2832]( https://github.com/NREL/OpenStudio/issues/2832 ), Double-clicking osm does not load file if file path contains white space
- ![Fixed:][fixed] [#2934]( https://github.com/NREL/OpenStudio/issues/2934 ), Fix qt warnings in app
- ![Fixed:][fixed] [#3019]( https://github.com/NREL/OpenStudio/issues/3019 ), OS App mac will crash if you go 
- ![Fixed:][fixed] [#3080]( https://github.com/NREL/OpenStudio/issues/3080 ), Stop using Handles in some object names
- ![Fixed:][fixed] [#3093]( https://github.com/NREL/OpenStudio/issues/3093 ), Fix failing Model Gtest CoilHeatingDesuperheater_addToNode
- ![Fixed:][fixed] [#3115]( https://github.com/NREL/OpenStudio/issues/3115 ), VRF Terminal group missing from My Model pane
- ![Fixed:][fixed] [#3116]( https://github.com/NREL/OpenStudio/issues/3116 ), OpenStudio Crashes while selecting plant loop
- ![Fixed:][fixed] [#3119]( https://github.com/NREL/OpenStudio/issues/3119 ), Schedules tab crashes when removing Run Period Profile from schedule
- ![Fixed:][fixed] [#3124]( https://github.com/NREL/OpenStudio/issues/3124 ), FanVariableVolume availability schedule name changes unexpectedly
- ![Fixed:][fixed] [#3138]( https://github.com/NREL/OpenStudio/issues/3138 ), Implement Gem resolver in CLI
- ![Fixed:][fixed] [#3142]( https://github.com/NREL/OpenStudio/issues/3142 ), Copying a zone attached to an air loop and attaching the cloned zone to the same air loop breaks the loop
- ![Fixed:][fixed] [#3155]( https://github.com/NREL/OpenStudio/issues/3155 ), Can't Launch DView on mac
- ![Fixed:][fixed] [#3164]( https://github.com/NREL/OpenStudio/issues/3164 ), WaterHeater:Mixed/Stratified ComponentType:Heating when linked to an HPWH
- ![Fixed:][fixed] [#3165]( https://github.com/NREL/OpenStudio/issues/3165 ), E+ example file 1ZoneDataCenterCRAC_wPumpedDXCoolingCoil.idf fails to import
- ![Fixed:][fixed] [#3169]( https://github.com/NREL/OpenStudio/issues/3169 ), Problems with getFoo paths on linux
- ![Fixed:][fixed] [#3180]( https://github.com/NREL/OpenStudio/issues/3180 ), Opening specific older OSM file in OS app crashes
- ![Fixed:][fixed] [#3181]( https://github.com/NREL/OpenStudio/issues/3181 ), Update SDK Documentation references for OS API to "Latest Iteration" to point to OS v2.6.0
- ![Fixed:][fixed] [#3182]( https://github.com/NREL/OpenStudio/issues/3182 ), EnergyManagementSystem:MeteredOutputVariable emsVariableName translation issue
- ![Fixed:][fixed] [#3183]( https://github.com/NREL/OpenStudio/issues/3183 ), AVM:HybridVentilation not translated
- ![Fixed:][fixed] [#3188]( https://github.com/NREL/OpenStudio/issues/3188 ), EnergyManagementSystem:InternalVariable internalDataIndexKeyName translation issue
- ![Fixed:][fixed] [#3218]( https://github.com/NREL/OpenStudio/issues/3218 ), Fans not allowed on oa system until after oa is attached to an air loop.
- ![Fixed:][fixed] [#3220]( https://github.com/NREL/OpenStudio/issues/3220 ), AirLoopHVAC remove - multiple loops
- ![Fixed:][fixed] [#3222]( https://github.com/NREL/OpenStudio/issues/3222 ), outputVariableNames contain "Average," or "Sum,"
- ![Fixed:][fixed] [#3244]( https://github.com/NREL/OpenStudio/issues/3244 ), Write versionTranslator for new EvporativeCooler:Direct:ResearchSpecial
- ![Fixed:][fixed] [#3249]( https://github.com/NREL/OpenStudio/issues/3249 ), Measure in user's directory that has a dot in path refuses to run in Apply Now
- ![Fixed:][fixed] [#3250]( https://github.com/NREL/OpenStudio/issues/3250 ), C# bindings fail after HVAC split-up.
- ![Fixed:][fixed] [#3260]( https://github.com/NREL/OpenStudio/issues/3260 ), expose End Use Subcategory field for Fan objects
- ![Fixed:][fixed] [#3261]( https://github.com/NREL/OpenStudio/issues/3261 ), OpenStudio.Path() constructor is looking for a Path and not a String
- ![Fixed:][fixed] [#3291]( https://github.com/NREL/OpenStudio/issues/3291 ), Multiple calls to runner.validateArguments register attributes each time, cause failures in show runner output
- ![Fixed:][fixed] [#3299]( https://github.com/NREL/OpenStudio/issues/3299 ), OpenStudio Plugin error visualizing surfaces
- ![Fixed:][fixed] [#2319]( https://github.com/NREL/OpenStudio/issues/2319 ), Adiabatic boundary conditions are not exported to gbXML
- ![Fixed:][fixed] [#3190]( https://github.com/NREL/OpenStudio/issues/3190 ), OS App looks for outdated library paths when installing a new version
- ![Fixed:][fixed] [#3107]( https://github.com/NREL/OpenStudio/issues/3107 ), Display issue on EMS Sensor tab when list too long
- ![Fixed:][fixed] [#3121]( https://github.com/NREL/OpenStudio/issues/3121 ), gbXML Reverse Translator does not import UndergroundWall from gbXML
- ![Fixed:][fixed] [#2992]( https://github.com/NREL/OpenStudio/issues/2992 ), gbXML from Revit imported with mixed up floors/ceilings
- ![Fixed:][fixed] [#2319]( https://github.com/NREL/OpenStudio/issues/2319 ), Adiabatic boundary conditions are not exported to gbXML
- ![Fixed:][fixed] [#3171]( https://github.com/NREL/OpenStudio/pull/3171 ), Add support for multiple AirLoopHVAC objects per ThermalZone
- ![Fixed:][fixed] [#3175]( https://github.com/NREL/OpenStudio/pull/3175 ), Add ElectricLoadCenterTransformer object
- ![Fixed:][fixed] [#3179]( https://github.com/NREL/OpenStudio/pull/3179 ), Add ability to use bundle to specify gem dependencies
- ![Fixed:][fixed] [#3206]( https://github.com/NREL/OpenStudio/pull/3206 ), Updated gbxml, idf, and osm import
- ![Fixed:][fixed] [#3219]( https://github.com/NREL/OpenStudio/pull/3219 ), Allow fans on oa system even without AirLoopHVAC
- ![Fixed:][fixed] [#3243]( https://github.com/NREL/OpenStudio/pull/3243 ), Wrap Schedule:File
- ![Fixed:][fixed] [#3276]( https://github.com/NREL/OpenStudio/pull/3276 ), Handle TableMultiVariableLookup::points
- ![Fixed:][fixed] [#3286]( https://github.com/NREL/OpenStudio/pull/3286 ), Update to EnergyPlus 9.0.0


## OpenStudio v2.6.0
###### 2018-06-30 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.5.0...v2.6.0 )

- ![Fixed:][fixed] [#231]( https://github.com/NREL/OpenStudio/issues/231 ), gbXML export cannot be read into several programs (Bugzilla #989)
- ![Fixed:][fixed] [#2198]( https://github.com/NREL/OpenStudio/issues/2198 ), SWIG takes 22 minutes to complete on OpenStudioModelHVAC
- ![Fixed:][fixed] [#2874]( https://github.com/NREL/OpenStudio/issues/2874 ), Wrong OSApp shared components (resources) install path on Linux
- ![Fixed:][fixed] [#2963]( https://github.com/NREL/OpenStudio/issues/2963 ), CoilHeatingGas.autosizedNominalCapacity() does not work
- ![Fixed:][fixed] [#2974]( https://github.com/NREL/OpenStudio/issues/2974 ), version of gtk dependency on linux is not available on ubuntu 16.04
- ![Fixed:][fixed] [#3082]( https://github.com/NREL/OpenStudio/issues/3082 ), adding AirTerminal to supply side of PlantLoop creates branches
- ![Fixed:][fixed] [#3084]( https://github.com/NREL/OpenStudio/issues/3084 ), Broken version translator for 2.4.3 models in current develop
- ![Fixed:][fixed] [#3096]( https://github.com/NREL/OpenStudio/issues/3096 ), LightsDefinition bug in fraction fields
- ![Fixed:][fixed] [#3102]( https://github.com/NREL/OpenStudio/issues/3102 ), Removing AvailabilityManager with sensor from loop leaves invalid EnergyManagementSystem:Sensor
- ![Fixed:][fixed] [#3103]( https://github.com/NREL/OpenStudio/issues/3103 ), AirLoopHVAC::autosizedDesignSupplyAirFlowRate doesn't always work
- ![Fixed:][fixed] [#3106]( https://github.com/NREL/OpenStudio/issues/3106 ), Linux install UX
- ![Fixed:][fixed] [#3126]( https://github.com/NREL/OpenStudio/issues/3126 ), EMS OutputVariable FT issue
- ![Fixed:][fixed] [#3130]( https://github.com/NREL/OpenStudio/issues/3130 ), Add SetpointManager:SingleZone:Humidity:Minimum to hvac_library
- ![Fixed:][fixed] [#3157]( https://github.com/NREL/OpenStudio/issues/3157 ), OpenStudio::EpwFile.new(weather_file) returns 'my.epw' cannot be processed
- ![Fixed:][fixed] [#3163]( https://github.com/NREL/OpenStudio/issues/3163 ), 2.5.2 CLI returns wrong path
- ![Fixed:][fixed] [#2205]( https://github.com/NREL/OpenStudio/pull/2205 ), Implement missing fields for EvaporativeCoolerDirectResarchSpecial
- ![Fixed:][fixed] [#3011]( https://github.com/NREL/OpenStudio/pull/3011 ), 2198 breakup hvac swig
- ![Fixed:][fixed] [#3014]( https://github.com/NREL/OpenStudio/pull/3014 ), Update Measure Templates and Dependencies
- ![Fixed:][fixed] [#3016]( https://github.com/NREL/OpenStudio/pull/3016 ), PVWatts Generator and Inverter
- ![Fixed:][fixed] [#3031]( https://github.com/NREL/OpenStudio/pull/3031 ), Four pipe beam
- ![Fixed:][fixed] [#3081]( https://github.com/NREL/OpenStudio/pull/3081 ), Fix #3080 - no UUID in IDFs
- ![Fixed:][fixed] [#3085]( https://github.com/NREL/OpenStudio/pull/3085 ), Fix #3084 and NREL/OpenStudio-resources#43
- ![Fixed:][fixed] [#3086]( https://github.com/NREL/OpenStudio/pull/3086 ), Design conditions in OS:EpwFile
- ![Fixed:][fixed] [#3087]( https://github.com/NREL/OpenStudio/pull/3087 ), Four pipe beam merge
- ![Fixed:][fixed] [#3088]( https://github.com/NREL/OpenStudio/pull/3088 ), Fix 3080 merge
- ![Fixed:][fixed] [#3089]( https://github.com/NREL/OpenStudio/pull/3089 ), Fix missing condenser type
- ![Fixed:][fixed] [#3090]( https://github.com/NREL/OpenStudio/pull/3090 ), CI Test PR
- ![Fixed:][fixed] [#3092]( https://github.com/NREL/OpenStudio/pull/3092 ), new CLI arguments
- ![Fixed:][fixed] [#3094]( https://github.com/NREL/OpenStudio/pull/3094 ), Fix #2874 - .deb package install to /usr/local/openstudio-x.y.z instead of /usr/
- ![Fixed:][fixed] [#3095]( https://github.com/NREL/OpenStudio/pull/3095 ), Plant component user defined
- ![Fixed:][fixed] [#3097]( https://github.com/NREL/OpenStudio/pull/3097 ), Fix #3096
- ![Fixed:][fixed] [#3104]( https://github.com/NREL/OpenStudio/pull/3104 ), Turn Off standardOutLogger in Individual tests
- ![Fixed:][fixed] [#3105]( https://github.com/NREL/OpenStudio/pull/3105 ), Just a CI test. Please ignore me.
- ![Fixed:][fixed] [#3108]( https://github.com/NREL/OpenStudio/pull/3108 ), Update gitlab ci yml file
- ![Fixed:][fixed] [#3109]( https://github.com/NREL/OpenStudio/pull/3109 ), Cli embedded files change
- ![Fixed:][fixed] [#3112]( https://github.com/NREL/OpenStudio/pull/3112 ), resolve EMS Sensor issue 3102
- ![Fixed:][fixed] [#3113]( https://github.com/NREL/OpenStudio/pull/3113 ), Fix 2874 install path unix
- ![Fixed:][fixed] [#3117]( https://github.com/NREL/OpenStudio/pull/3117 ), Fix 3116 catch orphaned components
- ![Fixed:][fixed] [#3118]( https://github.com/NREL/OpenStudio/pull/3118 ), Implement missing fields for EvaporativeCoolerDirectResarchSpecial
- ![Fixed:][fixed] [#3125]( https://github.com/NREL/OpenStudio/pull/3125 ), Allow shading controls for skylights.
- ![Fixed:][fixed] [#3129]( https://github.com/NREL/OpenStudio/pull/3129 ), Geometry editor
- ![Fixed:][fixed] [#3131]( https://github.com/NREL/OpenStudio/pull/3131 ), Fix #3130 - Add SPM Single Zone Humidity Minimum to hvac_library
- ![Fixed:][fixed] [#3132]( https://github.com/NREL/OpenStudio/pull/3132 ), add check to EMSOutVar emsVariableName() to look for UUIDs and swap fâ¦
- ![Fixed:][fixed] [#3133]( https://github.com/NREL/OpenStudio/pull/3133 ), Fix #2974 - libwxgtk dependencies on Linux
- ![Fixed:][fixed] [#3134]( https://github.com/NREL/OpenStudio/pull/3134 ), Fix thermostat schedule control value
- ![Fixed:][fixed] [#3135]( https://github.com/NREL/OpenStudio/pull/3135 ), Julien mega merge
- ![Fixed:][fixed] [#3136]( https://github.com/NREL/OpenStudio/pull/3136 ), Four pipe beam only
- ![Fixed:][fixed] [#3137]( https://github.com/NREL/OpenStudio/pull/3137 ), Four pipe beam only
- ![Fixed:][fixed] [#3152]( https://github.com/NREL/OpenStudio/pull/3152 ), Updates openstudio-standards to 0.2.2
- ![Fixed:][fixed] [#3160]( https://github.com/NREL/OpenStudio/pull/3160 ), Fixes #3157

## OpenStudio v2.5.0
###### 2018-03-30 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.4.3...v2.5.0 )

- ![Fixed:][fixed] [#2420]( https://github.com/NREL/OpenStudio/issues/2420 ), OpenStudio 2 on Windows Installation Issues
- ![Fixed:][fixed] [#2561]( https://github.com/NREL/OpenStudio/issues/2561 ), Symlinked CLI cannot find EnergyPlus on Mac
- ![Fixed:][fixed] [#2616]( https://github.com/NREL/OpenStudio/issues/2616 ), OpenStudio CLI fails to run properly if not run using absolute path. 
- ![Fixed:][fixed] [#2709]( https://github.com/NREL/OpenStudio/issues/2709 ), number of surfaces created by geometry editor translator is not deterministic
- ![Fixed:][fixed] [#2715]( https://github.com/NREL/OpenStudio/issues/2715 ), roofceiling adjacent to outdoors instead of surface after geometry editor translation
- ![Fixed:][fixed] [#2810]( https://github.com/NREL/OpenStudio/issues/2810 ), Bad Surface Names from Geometry Editor
- ![Fixed:][fixed] [#2836]( https://github.com/NREL/OpenStudio/issues/2836 ), Geometry Editor Units Display
- ![Fixed:][fixed] [#2911]( https://github.com/NREL/OpenStudio/issues/2911 ), cli run does not find e+ when running a simulation
- ![Fixed:][fixed] [#3038]( https://github.com/NREL/OpenStudio/issues/3038 ), Expose end-use subcategory
- ![Fixed:][fixed] [#3071]( https://github.com/NREL/OpenStudio/issues/3071 ), SpaceType properties lost in merge from Floorspace
- ![Fixed:][fixed] [#3074]( https://github.com/NREL/OpenStudio/issues/3074 ), Broken API for LifeCycleCost (identified via E+ 8.9.0 release)
- ![Fixed:][fixed] [#3075]( https://github.com/NREL/OpenStudio/issues/3075 ), ScheduleTypeLimits: "controlmode" failed to match any enum values E+ 8.9.0

## OpenStudio v2.4.3
###### 2018-03-07 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.4.1...v2.4.3 )

- ![Fixed:][fixed] [#2428]( https://github.com/NREL/OpenStudio/issues/2428 ), OpenStudio 2.0.0 path installation on mac
- ![Fixed:][fixed] [#2735]( https://github.com/NREL/OpenStudio/issues/2735 ), Import Library while in Geometry Editor doesn't work
- ![Fixed:][fixed] [#2775]( https://github.com/NREL/OpenStudio/issues/2775 ), Name synching issue
- ![Fixed:][fixed] [#2807]( https://github.com/NREL/OpenStudio/issues/2807 ), When __SKIP__ is false, other measure arguments should not be validated
- ![Fixed:][fixed] [#2835]( https://github.com/NREL/OpenStudio/issues/2835 ), Misc invalid OSM geometry generated by geometry editor in OS app 2.3.0
- ![Fixed:][fixed] [#2912]( https://github.com/NREL/OpenStudio/issues/2912 ), Space fields not translated
- ![Fixed:][fixed] [#2914]( https://github.com/NREL/OpenStudio/issues/2914 ), Shading objects not translated
- ![Fixed:][fixed] [#2916]( https://github.com/NREL/OpenStudio/issues/2916 ), CentralHeatPumpSystem forward translation problems
- ![Fixed:][fixed] [#2966]( https://github.com/NREL/OpenStudio/issues/2966 ), Remove QPolygonF from utilities geometry
- ![Fixed:][fixed] [#2969]( https://github.com/NREL/OpenStudio/issues/2969 ), Materials not translated from floorspace JSON to ThreeJS
- ![Fixed:][fixed] [#2971]( https://github.com/NREL/OpenStudio/issues/2971 ), Triangulated faces aren't grouped in floorplan -> threejs translation
- ![Fixed:][fixed] [#2972]( https://github.com/NREL/OpenStudio/issues/2972 ), 2.4.0 error: The zone inlet node in the controlled zone is not found.
- ![Fixed:][fixed] [#2984]( https://github.com/NREL/OpenStudio/issues/2984 ), PAT is not building correctly as part of OS Package
- ![Fixed:][fixed] [#2989]( https://github.com/NREL/OpenStudio/issues/2989 ), Ensure consistency between default Timestep object and E+
- ![Fixed:][fixed] [#2997]( https://github.com/NREL/OpenStudio/issues/2997 ), Install measure manager and other rb files 
- ![Fixed:][fixed] [#3000]( https://github.com/NREL/OpenStudio/issues/3000 ), OS 2.4.1 models will not load in current develop (2.4.2)

## OpenStudio v2.4.1
###### 2018-02-02 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.4.0...v2.4.1 )

- ![Fixed:][fixed] [#36]( https://github.com/NREL/OpenStudio/issues/36 ), If you attempt to run simulation with SQL file open it fails even after closing SQL and re-trying (Bugzilla #271)
- ![Fixed:][fixed] [#58]( https://github.com/NREL/OpenStudio/issues/58 ), Another sidecar bug related to EMSWindowShadeControl.idf (duplicate thermostat setpoint objects for each zone) (Bugzilla #452)
- ![Fixed:][fixed] [#86]( https://github.com/NREL/OpenStudio/issues/86 ), Constructions Tab - While Construction sub tab is "Construction Sets" in library still called "Default Construction Sets" (Bugzilla #580)
- ![Fixed:][fixed] [#90]( https://github.com/NREL/OpenStudio/issues/90 ), Schedule Tab - Calendars should show up on right when schedule is selected (Bugzilla #607)
- ![Fixed:][fixed] [#100]( https://github.com/NREL/OpenStudio/issues/100 ), Prevent user from entering bad characters in input (e.g. no ',' in object names) (Bugzilla #647)
- ![Fixed:][fixed] [#112]( https://github.com/NREL/OpenStudio/issues/112 ), When  you add a new script the dirty flag doesn't show up unless you load arguments (Bugzilla #684)
- ![Fixed:][fixed] [#118]( https://github.com/NREL/OpenStudio/issues/118 ), Deleting Construction Set object leaves sub-set objects behind (Bugzilla #704)
- ![Fixed:][fixed] [#152]( https://github.com/NREL/OpenStudio/issues/152 ), We are not adjusting input value precision when it is first drawn on a page, as a result really long numbers (Bugzilla #800)
- ![Fixed:][fixed] [#185]( https://github.com/NREL/OpenStudio/issues/185 ), ExpandObjects job results in EnergyPlus warnings about no idd as specified by ini (Bugzilla #905)
- ![Fixed:][fixed] [#191]( https://github.com/NREL/OpenStudio/issues/191 ), HVAC Tab - Users may get lost in Water Use Connection object (Bugzilla #917)
- ![Fixed:][fixed] [#193]( https://github.com/NREL/OpenStudio/issues/193 ), Hard coded include paths stored for Ruby jobs in run.db (Bugzilla #920)
- ![Fixed:][fixed] [#202]( https://github.com/NREL/OpenStudio/issues/202 ), schedule drop zones in loads tab (shw) and pulldowns in edit pane of HVAC tab don't set or enforce schedule types (Bugzilla #931)
- ![Fixed:][fixed] [#212]( https://github.com/NREL/OpenStudio/issues/212 ), ModelToRad Preprocess makes bad model clone (Bugzilla #950)
- ![Fixed:][fixed] [#213]( https://github.com/NREL/OpenStudio/issues/213 ), Zone Tab - OS:ZoneHVAC:PackagedTerminalHeatPump fields won't go to hard size in GUI (Bugzilla #951)
- ![Fixed:][fixed] [#214]( https://github.com/NREL/OpenStudio/issues/214 ), HVAC Tab - Units on newly added OS:Sizing:System always display IP (Bugzilla #952)
- ![Fixed:][fixed] [#217]( https://github.com/NREL/OpenStudio/issues/217 ), Sometimes when running a simulation a full IDF isn't run, but simulation finishs without error (Bugzilla #955)
- ![Fixed:][fixed] [#225]( https://github.com/NREL/OpenStudio/issues/225 ), Water Heater Setting Problem (from user post, see URL for post) (Bugzilla #971)
- ![Fixed:][fixed] [#235]( https://github.com/NREL/OpenStudio/issues/235 ), Simulation Settings Tab - Minor bugs (Bugzilla #997)
- ![Fixed:][fixed] [#254]( https://github.com/NREL/OpenStudio/issues/254 ), Running model with radiance does not seem to work on 1 space out of 2 space model (Bugzilla #1023)
- ![Fixed:][fixed] [#255]( https://github.com/NREL/OpenStudio/issues/255 ), OpenStudio application does not report errors or warnings from EnergyPlus translator either (Bugzilla #1024)
- ![Fixed:][fixed] [#258]( https://github.com/NREL/OpenStudio/issues/258 ), Boost error if you try to run simulation with eplusout.err file open (Bugzilla #1028)
- ![Fixed:][fixed] [#260]( https://github.com/NREL/OpenStudio/issues/260 ), HVAC Tab - Viewing Loop with lots of hot water reheat objects is very slow (Bugzilla #1034)
- ![Fixed:][fixed] [#261]( https://github.com/NREL/OpenStudio/issues/261 ), HVAC Tab - When you add System type 7 and look in edit tab, chiller shows as air cooled (Bugzilla #1035)
- ![Fixed:][fixed] [#263]( https://github.com/NREL/OpenStudio/issues/263 ), HVAC Tab - more meaningful node and mixed air setpoint manager names (Bugzilla #1037)
- ![Fixed:][fixed] [#264]( https://github.com/NREL/OpenStudio/issues/264 ), Schedule Tab - Cloning schedules is disabled (Bugzilla #1038)
- ![Fixed:][fixed] [#265]( https://github.com/NREL/OpenStudio/issues/265 ), SpaceType tab - Infiltration Design Flow Rate and Infiltration Leakage areas don't show in "My Model" (Bugzilla #1039)
- ![Fixed:][fixed] [#266]( https://github.com/NREL/OpenStudio/issues/266 ), SpaceType tab -Using "Flow/ExteriorWallArea" produces a simulation with no infiltration (Bugzilla #1040)
- ![Fixed:][fixed] [#267]( https://github.com/NREL/OpenStudio/issues/267 ), SpaceType tab - checking to a new space type object clear out the object in the edit tab (Bugzilla #1041)
- ![Fixed:][fixed] [#276]( https://github.com/NREL/OpenStudio/issues/276 ), This file doesn't fail gracefully (Bugzilla #1063)
- ![Fixed:][fixed] [#283]( https://github.com/NREL/OpenStudio/issues/283 ), Using IP units for OutputControl:Table:Style in IDF scripts breaks results tab in OS app (Bugzilla #1072)
- ![Fixed:][fixed] [#285]( https://github.com/NREL/OpenStudio/issues/285 ), Some users have problems getting arguments in scritps tab (Bugzilla #1074)
- ![Fixed:][fixed] [#286]( https://github.com/NREL/OpenStudio/issues/286 ), Intermittent failure due to job cleanup (Bugzilla #1077)
- ![Fixed:][fixed] [#287]( https://github.com/NREL/OpenStudio/issues/287 ), Opening and saving model in OS app deletes any variables added in the SketchUp Plugin (Bugzilla #1093)
- ![Fixed:][fixed] [#334]( https://github.com/NREL/OpenStudio/issues/334 ), ThermalZone leaving connection behind (Bugzilla #1250)
- ![Fixed:][fixed] [#352]( https://github.com/NREL/OpenStudio/issues/352 ), Tab 4 - When you open model in OpenStudio results are blank (Bugzilla #1282)
- ![Fixed:][fixed] [#355]( https://github.com/NREL/OpenStudio/issues/355 ), Problems in model with Daylighting Control Object (Bugzilla #1290)
- ![Fixed:][fixed] [#361]( https://github.com/NREL/OpenStudio/issues/361 ), Tab 2 - Order of design alternatives made using "Create One For Each Selected Measure" can be odd (Bugzilla #1298)
- ![Fixed:][fixed] [#377]( https://github.com/NREL/OpenStudio/issues/377 ), You are not warned if you try to delete a schedule that is being used.
- ![Fixed:][fixed] [#403]( https://github.com/NREL/OpenStudio/issues/403 ), Getting Multiple "Always on Discrete" schedules
- ![Fixed:][fixed] [#405]( https://github.com/NREL/OpenStudio/issues/405 ), OS App inconsistent use of "volume" vs. "speed" for fans
- ![Fixed:][fixed] [#492]( https://github.com/NREL/OpenStudio/issues/492 ), User model simulation on 21 zone model with ideal air loads takes 2 hours to run
- ![Fixed:][fixed] [#587]( https://github.com/NREL/OpenStudio/issues/587 ), A simulation in OS app is showing as failed, but it doesn't seem to be failing
- ![Fixed:][fixed] [#656]( https://github.com/NREL/OpenStudio/issues/656 ), Many objects in OpenStudio still appear to accept bad input values, but seems like it is just displaying the bad value and not storing it.
- ![Fixed:][fixed] [#751]( https://github.com/NREL/OpenStudio/issues/751 ), Results tab in OS app seems to be showing the "MeasureType" vs. the name in the GUI
- ![Fixed:][fixed] [#761]( https://github.com/NREL/OpenStudio/issues/761 ), Update logic for injecting calibration measure to check if calendar year is used
- ![Fixed:][fixed] [#821]( https://github.com/NREL/OpenStudio/issues/821 ), daylight sensor direction not correct on IDF import
- ![Fixed:][fixed] [#825]( https://github.com/NREL/OpenStudio/issues/825 ), WaterUseEquipment is not child of Space
- ![Fixed:][fixed] [#884]( https://github.com/NREL/OpenStudio/issues/884 ), Water use equipment does not show up in facility tree under space
- ![Fixed:][fixed] [#918]( https://github.com/NREL/OpenStudio/issues/918 ), No example model or measures in installer for users to play with
- ![Fixed:][fixed] [#919]( https://github.com/NREL/OpenStudio/issues/919 ), Lots of warnings in project when calling save from within transaction
- ![Fixed:][fixed] [#924]( https://github.com/NREL/OpenStudio/issues/924 ), Purge constructions in app very slow
- ![Fixed:][fixed] [#945]( https://github.com/NREL/OpenStudio/issues/945 ), Cannot make Compressor Rated Superheat or Rated Return Gas Temp Blank
- ![Fixed:][fixed] [#966]( https://github.com/NREL/OpenStudio/issues/966 ), Refrigeration Systems: defining a restocking schedule
- ![Fixed:][fixed] [#991]( https://github.com/NREL/OpenStudio/issues/991 ), There are many VRF curves without limits
- ![Fixed:][fixed] [#1000]( https://github.com/NREL/OpenStudio/issues/1000 ), Tools cannot be loaded or saved from Ruby
- ![Fixed:][fixed] [#1175]( https://github.com/NREL/OpenStudio/issues/1175 ), Can run simulation in OS App when reporting measure's have unset fields
- ![Fixed:][fixed] [#1211]( https://github.com/NREL/OpenStudio/issues/1211 ), QWebkit seems to be slower parsing large objects in Javascript 
- ![Fixed:][fixed] [#1286]( https://github.com/NREL/OpenStudio/issues/1286 ), Hard-sized entry of 0 reverts to Autosized
- ![Fixed:][fixed] [#1341]( https://github.com/NREL/OpenStudio/issues/1341 ), Add ZoneHVAC:FourPipeFanCoil with different capacity control methods to library
- ![Fixed:][fixed] [#1351]( https://github.com/NREL/OpenStudio/issues/1351 ), Generalize E+ version on tests
- ![Fixed:][fixed] [#1404]( https://github.com/NREL/OpenStudio/issues/1404 ), Results screen - reporting error - 1.6.0
- ![Fixed:][fixed] [#1409]( https://github.com/NREL/OpenStudio/issues/1409 ), Too much padding in grid view, especially vertical padding. 
- ![Fixed:][fixed] [#1453]( https://github.com/NREL/OpenStudio/issues/1453 ), Show text or something for non-Schedule:Ruleset objects on Schedules tab
- ![Fixed:][fixed] [#1455]( https://github.com/NREL/OpenStudio/issues/1455 ), Add check for update to all applications
- ![Fixed:][fixed] [#1458]( https://github.com/NREL/OpenStudio/issues/1458 ), Ruby bindings to EpwFile are not very useful
- ![Fixed:][fixed] [#1464]( https://github.com/NREL/OpenStudio/issues/1464 ), Need to deprecate find_energyplus.rb
- ![Fixed:][fixed] [#1469]( https://github.com/NREL/OpenStudio/issues/1469 ), ScheduleEditor issues
- ![Fixed:][fixed] [#1511]( https://github.com/NREL/OpenStudio/issues/1511 ), Two EnergyPlus Results show up in results tab during radiance runs
- ![Fixed:][fixed] [#1538]( https://github.com/NREL/OpenStudio/issues/1538 ), Change default idd file type for Workspace.new
- ![Fixed:][fixed] [#1557]( https://github.com/NREL/OpenStudio/issues/1557 ), Status bar confusing when E+ run fails
- ![Fixed:][fixed] [#1561]( https://github.com/NREL/OpenStudio/issues/1561 ), Temporary run directories not removed
- ![Fixed:][fixed] [#1590]( https://github.com/NREL/OpenStudio/issues/1590 ), Add Model::path() method to support copying files to resources 
- ![Fixed:][fixed] [#1603]( https://github.com/NREL/OpenStudio/issues/1603 ), OpenStudio app won't launch after force quit (in some instances)
- ![Fixed:][fixed] [#1645]( https://github.com/NREL/OpenStudio/issues/1645 ), Sometimes get crash when pasting text into string argument value in measures tab
- ![Fixed:][fixed] [#1696]( https://github.com/NREL/OpenStudio/issues/1696 ), Add inheritance to grid view
- ![Fixed:][fixed] [#1725]( https://github.com/NREL/OpenStudio/issues/1725 ), Open in ResultsViewer doesn't work on mac
- ![Fixed:][fixed] [#1754]( https://github.com/NREL/OpenStudio/issues/1754 ), ProjectDatabase::preOpenCheck breaks existing connections
- ![Fixed:][fixed] [#2019]( https://github.com/NREL/OpenStudio/issues/2019 ), Enable generate log files with installed OpenStudio
- ![Fixed:][fixed] [#2096]( https://github.com/NREL/OpenStudio/issues/2096 ), Incomplete published EDAPT Schema (Refrigeration)
- ![Fixed:][fixed] [#2168]( https://github.com/NREL/OpenStudio/issues/2168 ), Add AirTerminalDualDuctVAVOutdoorAir to OpenStudio
- ![Fixed:][fixed] [#2179]( https://github.com/NREL/OpenStudio/issues/2179 ), Plenum Space Type - Odd behavior when rename space type
- ![Fixed:][fixed] [#2222]( https://github.com/NREL/OpenStudio/issues/2222 ), Illuminance Map Scale Does Not Work
- ![Fixed:][fixed] [#2275]( https://github.com/NREL/OpenStudio/issues/2275 ), Develop PlantEquipmentOperationScheme logic for ThermalStorage and develop logic to make unknown objects use uncontrolled scheme.
- ![Fixed:][fixed] [#2294]( https://github.com/NREL/OpenStudio/issues/2294 ), Subsurfaces Tab in Spaces has Bogus Search Field
- ![Fixed:][fixed] [#2391]( https://github.com/NREL/OpenStudio/issues/2391 ), Change peak flow rate in water use equipment definitions from m3/s to m3/h or dm3/s
- ![Fixed:][fixed] [#2479]( https://github.com/NREL/OpenStudio/issues/2479 ), can't remove material layers from ConstructionInternalSource
- ![Fixed:][fixed] [#2498]( https://github.com/NREL/OpenStudio/issues/2498 ), Clean up OpenStudio-PAT repo README
- ![Fixed:][fixed] [#2501]( https://github.com/NREL/OpenStudio/issues/2501 ), Show units & descriptions for arguments in PAT
- ![Fixed:][fixed] [#2523]( https://github.com/NREL/OpenStudio/issues/2523 ), Database backup on server returns an error
- ![Fixed:][fixed] [#2532]( https://github.com/NREL/OpenStudio/issues/2532 ), can't delete datapoints from Run or Reports tabs
- ![Fixed:][fixed] [#2552]( https://github.com/NREL/OpenStudio/issues/2552 ), Visualize should be defaulted to true for any selected outputs
- ![Fixed:][fixed] [#2553]( https://github.com/NREL/OpenStudio/issues/2553 ), Enable Scaling in PAT
- ![Fixed:][fixed] [#2563]( https://github.com/NREL/OpenStudio/issues/2563 ), Pre-run error checking in PAT for algorithmic mode 
- ![Fixed:][fixed] [#2570]( https://github.com/NREL/OpenStudio/issues/2570 ), PAT duplicate measure names don't increment
- ![Fixed:][fixed] [#2571]( https://github.com/NREL/OpenStudio/issues/2571 ), PAT ability to rename measures in algorithmic mode
- ![Fixed:][fixed] [#2573]( https://github.com/NREL/OpenStudio/issues/2573 ), ScheduleDay accepts nan for values in API.
- ![Fixed:][fixed] [#2581]( https://github.com/NREL/OpenStudio/issues/2581 ), About ParametricAnalysisTool doesn't show proper version number
- ![Fixed:][fixed] [#2582]( https://github.com/NREL/OpenStudio/issues/2582 ), PAT 2.1.0 bug when switching discrete -> integer_sequence
- ![Fixed:][fixed] [#2587]( https://github.com/NREL/OpenStudio/issues/2587 ), PAT 2.1 runs on wrong target
- ![Fixed:][fixed] [#2588]( https://github.com/NREL/OpenStudio/issues/2588 ), Duplicate discrete variables cause server failures
- ![Fixed:][fixed] [#2590]( https://github.com/NREL/OpenStudio/issues/2590 ), PAT Results Tab Shows Datapoint Errors in Algorithm Mode
- ![Fixed:][fixed] [#2592]( https://github.com/NREL/OpenStudio/issues/2592 ), Can't run PAT 2.X projects from network drive
- ![Fixed:][fixed] [#2594]( https://github.com/NREL/OpenStudio/issues/2594 ), PAT 2.1.0 Model generated datapoints have no means of attaching measure taxonomy tags.
- ![Fixed:][fixed] [#2595]( https://github.com/NREL/OpenStudio/issues/2595 ), PAT extraneous algorithm setting for Diagonal
- ![Fixed:][fixed] [#2599]( https://github.com/NREL/OpenStudio/issues/2599 ), PAT does not filter incompatible (newer) AMIs
- ![Fixed:][fixed] [#2606]( https://github.com/NREL/OpenStudio/issues/2606 ), Large Uploads to Server (i.e. lots of weather files) Seem to Timeout
- ![Fixed:][fixed] [#2608]( https://github.com/NREL/OpenStudio/issues/2608 ), Implement server & worker init & final scripts
- ![Fixed:][fixed] [#2620]( https://github.com/NREL/OpenStudio/issues/2620 ), Standardize return types for set(...) Methods
- ![Fixed:][fixed] [#2622]( https://github.com/NREL/OpenStudio/issues/2622 ), App crash in macOS while editing water fixtures/connections
- ![Fixed:][fixed] [#2627]( https://github.com/NREL/OpenStudio/issues/2627 ), Measures copied into workflow should have tests/output directory removed
- ![Fixed:][fixed] [#2638]( https://github.com/NREL/OpenStudio/issues/2638 ), Issue with Measure Manager in PAT
- ![Fixed:][fixed] [#2639]( https://github.com/NREL/OpenStudio/issues/2639 ), PAT 2.1.1 copy measure creates invalid measure
- ![Fixed:][fixed] [#2643]( https://github.com/NREL/OpenStudio/issues/2643 ), Updating measure in PAT doesn't handle new and/or deleted arguments properly.
- ![Fixed:][fixed] [#2644]( https://github.com/NREL/OpenStudio/issues/2644 ), Cancel run from PAT prevents later analyses from starting
- ![Fixed:][fixed] [#2657]( https://github.com/NREL/OpenStudio/issues/2657 ), Cannot delete Measure Option in PAT
- ![Fixed:][fixed] [#2661]( https://github.com/NREL/OpenStudio/issues/2661 ), Cannot quit PAT
- ![Fixed:][fixed] [#2662]( https://github.com/NREL/OpenStudio/issues/2662 ), Problems with results measure
- ![Fixed:][fixed] [#2663]( https://github.com/NREL/OpenStudio/issues/2663 ), PAT should not start measure manager on default port
- ![Fixed:][fixed] [#2668]( https://github.com/NREL/OpenStudio/issues/2668 ), PAT Shows Updates Available for Measures but Clicking the Update Button Does Nothing
- ![Fixed:][fixed] [#2671]( https://github.com/NREL/OpenStudio/issues/2671 ), PAT Crashes due to non ascii characters
- ![Fixed:][fixed] [#2672]( https://github.com/NREL/OpenStudio/issues/2672 ), Show Stdout output from E+ in PAT in real time
- ![Fixed:][fixed] [#2673]( https://github.com/NREL/OpenStudio/issues/2673 ), PAT run fails due to mongo validation, starts E+ over and over again
- ![Fixed:][fixed] [#2675]( https://github.com/NREL/OpenStudio/issues/2675 ), PAT starts more runs than cores
- ![Fixed:][fixed] [#2678]( https://github.com/NREL/OpenStudio/issues/2678 ), Allow custom analysis name different than PAT project name
- ![Fixed:][fixed] [#2679]( https://github.com/NREL/OpenStudio/issues/2679 ), Scatter Plot on Server Contains Extraneous Data
- ![Fixed:][fixed] [#2680]( https://github.com/NREL/OpenStudio/issues/2680 ), Results Icon in PAT's Run Tab Not Populating Correctly
- ![Fixed:][fixed] [#2682]( https://github.com/NREL/OpenStudio/issues/2682 ), PAT order of design alternatives isn't preserved
- ![Fixed:][fixed] [#2684]( https://github.com/NREL/OpenStudio/issues/2684 ), PAT can show incorrect # of datapoints
- ![Fixed:][fixed] [#2685]( https://github.com/NREL/OpenStudio/issues/2685 ), PAT improvements for Algorithmic projects
- ![Fixed:][fixed] [#2689]( https://github.com/NREL/OpenStudio/issues/2689 ), Issues with PAT2.1.2
- ![Fixed:][fixed] [#2696]( https://github.com/NREL/OpenStudio/issues/2696 ), Measures that download components from BCL not running properly in local PAT runs
- ![Fixed:][fixed] [#2701]( https://github.com/NREL/OpenStudio/issues/2701 ), PAT RUNS FOR EVER 
- ![Fixed:][fixed] [#2703]( https://github.com/NREL/OpenStudio/issues/2703 ), Fix heartbeat communication in dockerized deployments
- ![Fixed:][fixed] [#2711]( https://github.com/NREL/OpenStudio/issues/2711 ), Save as in App breaks model
- ![Fixed:][fixed] [#2725]( https://github.com/NREL/OpenStudio/issues/2725 ), Surfaces are not matched on merge
- ![Fixed:][fixed] [#2742]( https://github.com/NREL/OpenStudio/issues/2742 ), PAT Doesn't Correctly Skip Measures When Selected Points are Re-Run
- ![Fixed:][fixed] [#2755]( https://github.com/NREL/OpenStudio/issues/2755 ), PAT analysis name cannot contain . or continues to loop forever
- ![Fixed:][fixed] [#2758]( https://github.com/NREL/OpenStudio/issues/2758 ), OS Server Dies Mid-Analysis
- ![Fixed:][fixed] [#2759]( https://github.com/NREL/OpenStudio/issues/2759 ), PAT Measure Manager Not Finding New Content in MyMeasures
- ![Fixed:][fixed] [#2761]( https://github.com/NREL/OpenStudio/issues/2761 ), Add DView to OpenStudio
- ![Fixed:][fixed] [#2766]( https://github.com/NREL/OpenStudio/issues/2766 ), PAT double/integer bug
- ![Fixed:][fixed] [#2770]( https://github.com/NREL/OpenStudio/issues/2770 ), PAT Updating measures doesn't preserve argument order
- ![Fixed:][fixed] [#2773]( https://github.com/NREL/OpenStudio/issues/2773 ), "Download All" in PAT only downloads 150 datapoints
- ![Fixed:][fixed] [#2777]( https://github.com/NREL/OpenStudio/issues/2777 ), Qt WebEngine problems on Ubuntu preclude ResultsViewer (OS 2.2.1) and DView (OS 2.2.2) from running on Ubuntu
- ![Fixed:][fixed] [#2779]( https://github.com/NREL/OpenStudio/issues/2779 ), DView does not close cleanly on Mac
- ![Fixed:][fixed] [#2787]( https://github.com/NREL/OpenStudio/issues/2787 ), removing airloop removes ptac from inlet port list
- ![Fixed:][fixed] [#2794]( https://github.com/NREL/OpenStudio/issues/2794 ), Modifying Construction Rendering Color in SketchUp OS Plugin or OS APP
- ![Fixed:][fixed] [#2799]( https://github.com/NREL/OpenStudio/issues/2799 ), workspace.getObjectsByType() for ZoneHVAC:IdealLoadsAirSystem objects
- ![Fixed:][fixed] [#2803]( https://github.com/NREL/OpenStudio/issues/2803 ), DView crashes on Ubuntu
- ![Fixed:][fixed] [#2806]( https://github.com/NREL/OpenStudio/issues/2806 ),  "Could not set argument" should show visible error in GUI
- ![Fixed:][fixed] [#2808]( https://github.com/NREL/OpenStudio/issues/2808 ), OpenStudio crashes when switching from Space Type to Space Tab when load visible in inspector
- ![Fixed:][fixed] [#2811]( https://github.com/NREL/OpenStudio/issues/2811 ), output hash should be included in the OSA for a manual simulation
- ![Fixed:][fixed] [#2824]( https://github.com/NREL/OpenStudio/issues/2824 ), openstudio-server and openstudio-rserve latest tags out of state
- ![Fixed:][fixed] [#2829]( https://github.com/NREL/OpenStudio/issues/2829 ), Odd Summary Table behavior for EUI when design alternatives have negative value.
- ![Fixed:][fixed] [#2830]( https://github.com/NREL/OpenStudio/issues/2830 ), Openstudio failed to run
- ![Fixed:][fixed] [#2834]( https://github.com/NREL/OpenStudio/issues/2834 ), Cannot compute plane for points Could not create surface for vertices
- ![Fixed:][fixed] [#2840]( https://github.com/NREL/OpenStudio/issues/2840 ), PAT manual analysis fails if no variables are setup
- ![Fixed:][fixed] [#2842]( https://github.com/NREL/OpenStudio/issues/2842 ), Empty string or string with just space as string argument value is not treated as valid in PAT but is in OS app 
- ![Fixed:][fixed] [#2843]( https://github.com/NREL/OpenStudio/issues/2843 ), PAT app appears to run forever when server hits maximum RAM
- ![Fixed:][fixed] [#2845]( https://github.com/NREL/OpenStudio/issues/2845 ), An open DView instance will prevent new eplusout.sql from being written
- ![Fixed:][fixed] [#2846]( https://github.com/NREL/OpenStudio/issues/2846 ), External Wall Measure in PAT does not run
- ![Fixed:][fixed] [#2857]( https://github.com/NREL/OpenStudio/issues/2857 ), PAT 2.3.0 on Windows - Local Server Not Starting - Server Error
- ![Fixed:][fixed] [#2883]( https://github.com/NREL/OpenStudio/issues/2883 ), Fix failing EMS tests
- ![Fixed:][fixed] [#2885]( https://github.com/NREL/OpenStudio/issues/2885 ), Fix Failing FT tests for ScheduleInterval
- ![Fixed:][fixed] [#2892]( https://github.com/NREL/OpenStudio/issues/2892 ), Re-deploying os-server to clusters leads to worker service container instability on remote nodes
- ![Fixed:][fixed] [#2897]( https://github.com/NREL/OpenStudio/issues/2897 ), Memory leak in results_file model
- ![Fixed:][fixed] [#2898]( https://github.com/NREL/OpenStudio/issues/2898 ), Memory leak in eval leads to slow leak in long running analyses
- ![Fixed:][fixed] [#2899]( https://github.com/NREL/OpenStudio/issues/2899 ), Memory 'leak' in send_data method in ActionPack
- ![Fixed:][fixed] [#2900]( https://github.com/NREL/OpenStudio/issues/2900 ), Crash when clicking on a pump in OS App
- ![Fixed:][fixed] [#2904]( https://github.com/NREL/OpenStudio/issues/2904 ), Match ScheduleTypeLimits 'Unit Type' options between E+ and OS
- ![Fixed:][fixed] [#2905]( https://github.com/NREL/OpenStudio/issues/2905 ), simulation errors and warnings notification unavailable
- ![Fixed:][fixed] [#2909]( https://github.com/NREL/OpenStudio/issues/2909 ), OS 2.4.0 EMS Actuator FT issue?
- ![Fixed:][fixed] [#2910]( https://github.com/NREL/OpenStudio/issues/2910 ), Back Side Solar Reflectance Field Not Editable in App
- ![Fixed:][fixed] [#2919]( https://github.com/NREL/OpenStudio/issues/2919 ), App doesn't launch correctly when double-clicking an OSM on Mac
- ![Fixed:][fixed] [#2921]( https://github.com/NREL/OpenStudio/issues/2921 ), OS App crashes on exit
- ![Fixed:][fixed] [#2937]( https://github.com/NREL/OpenStudio/issues/2937 ), Daylighting output variables not registered
- ![Fixed:][fixed] [#2944]( https://github.com/NREL/OpenStudio/issues/2944 ), Add dialog to edit default component libraries 
- ![Fixed:][fixed] [#2978]( https://github.com/NREL/OpenStudio/issues/2978 ), Open Studio crashes when importing DOE IDF files
- ![Fixed:][fixed] [#2986]( https://github.com/NREL/OpenStudio/issues/2986 ), Merging empty model with existing floorplan causes bad JSON

## OpenStudio v2.4.0
###### 2017-12-12 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.3.1...v2.4.0 )

- ![Fixed:][fixed] [#230]( https://github.com/NREL/OpenStudio/issues/230 ), No way to make or use Hot Water Equipment objects in App (Bugzilla #988)
- ![Fixed:][fixed] [#882]( https://github.com/NREL/OpenStudio/issues/882 ), E+ translator warning: Unknown object OS:Coil:Heating:Water:Baseboard
- ![Fixed:][fixed] [#1422]( https://github.com/NREL/OpenStudio/issues/1422 ), Document Thermal Zones and Zone Equipment
- ![Fixed:][fixed] [#1423]( https://github.com/NREL/OpenStudio/issues/1423 ), Document Airloops
- ![Fixed:][fixed] [#1424]( https://github.com/NREL/OpenStudio/issues/1424 ), Document Plantloops
- ![Fixed:][fixed] [#1425]( https://github.com/NREL/OpenStudio/issues/1425 ), Document VRF
- ![Fixed:][fixed] [#1426]( https://github.com/NREL/OpenStudio/issues/1426 ), Document Refrigeration
- ![Fixed:][fixed] [#1427]( https://github.com/NREL/OpenStudio/issues/1427 ), Document Utility Rates
- ![Fixed:][fixed] [#1428]( https://github.com/NREL/OpenStudio/issues/1428 ), Document EnergyPlus Troubleshooting
- ![Fixed:][fixed] [#1429]( https://github.com/NREL/OpenStudio/issues/1429 ), Document Timeseries Results
- ![Fixed:][fixed] [#1430]( https://github.com/NREL/OpenStudio/issues/1430 ), Document General Modeling QAQC
- ![Fixed:][fixed] [#2305]( https://github.com/NREL/OpenStudio/issues/2305 ), ATU CV Four Pipe problems with InducedAirInletNode
- ![Fixed:][fixed] [#2750]( https://github.com/NREL/OpenStudio/issues/2750 ), EMS:Sensor constructor with OutputVariable should get the KeyValue
- ![Fixed:][fixed] [#2879]( https://github.com/NREL/OpenStudio/issues/2879 ), The default chilled beam type is missing
- ![Fixed:][fixed] [#2888]( https://github.com/NREL/OpenStudio/issues/2888 ), Cannot edit any of the Fractions for the loads in current develop in OS App
- ![Fixed:][fixed] [#2891]( https://github.com/NREL/OpenStudio/issues/2891 ), 2.3.1 AirLoopHVAC doesn't have an AvailabilityManagerAssignmentList assigned
- ![Improved:][improved] [#907]( https://github.com/NREL/OpenStudio/issues/907 ), Water end-use column missing from PAT results tab

## OpenStudio v2.3.1
###### 2017-12-04 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.3.0...v2.3.1 )

- ![Fixed:][fixed] [#25]( https://github.com/NREL/OpenStudio/issues/25 ), SQL Alias doesn't show correctly until you close and re-open (Bugzilla #7)
- ![Fixed:][fixed] [#30]( https://github.com/NREL/OpenStudio/issues/30 ), Re-arranged column order doesn't stick next launch (Bugzilla #99)
- ![Fixed:][fixed] [#128]( https://github.com/NREL/OpenStudio/issues/128 ), Changing Units can show wrong data in currently selected object (Bugzilla #731)
- ![Fixed:][fixed] [#137]( https://github.com/NREL/OpenStudio/issues/137 ), Inspector Dialog does not disconnect from model when sketchup window closed (Bugzilla #763)
- ![Fixed:][fixed] [#138]( https://github.com/NREL/OpenStudio/issues/138 ), Opening an osm with weather file object but file missing on disk, app should clear weather file object (Bugzilla #764)
- ![Fixed:][fixed] [#146]( https://github.com/NREL/OpenStudio/issues/146 ), onPercentageUpdated can be called with negative percentage when importing ddy file (Bugzilla #787)
- ![Fixed:][fixed] [#147]( https://github.com/NREL/OpenStudio/issues/147 ), Translation of objects to EnergyPlus should happen in sorted order (Bugzilla #788)
- ![Fixed:][fixed] [#157]( https://github.com/NREL/OpenStudio/issues/157 ), Cannot plot two variables with different units (Bugzilla #830)
- ![Fixed:][fixed] [#158]( https://github.com/NREL/OpenStudio/issues/158 ), Many output variable names changed in E+ 7.1 (Bugzilla #832)
- ![Fixed:][fixed] [#159]( https://github.com/NREL/OpenStudio/issues/159 ), Space Type on Demand offline now - we should have more generic script message. (Bugzilla #836)
- ![Fixed:][fixed] [#161]( https://github.com/NREL/OpenStudio/issues/161 ), Simulation runs slower on EnergyPlus 7.1 and 7.0 (Bugzilla #838)
- ![Fixed:][fixed] [#166]( https://github.com/NREL/OpenStudio/issues/166 ), Facility Tab - Seems PTHP and PTAC shouldn't show under Library (Bugzilla #850)
- ![Fixed:][fixed] [#178]( https://github.com/NREL/OpenStudio/issues/178 ), HVAC Tab - Fields with Autosize/Hardsize are right justified (Bugzilla #892)
- ![Fixed:][fixed] [#182]( https://github.com/NREL/OpenStudio/issues/182 ), with surface selected inspector pulldown for "Outside Boundary Object" shows both surfaces and sub-surfaces (Bugzilla #899)
- ![Fixed:][fixed] [#187]( https://github.com/NREL/OpenStudio/issues/187 ), When we warn users that we flipped surface we give UUID vs. name (Bugzilla #909)
- ![Fixed:][fixed] [#188]( https://github.com/NREL/OpenStudio/issues/188 ), Add template for loop with hot water heater (Bugzilla #912)
- ![Fixed:][fixed] [#205]( https://github.com/NREL/OpenStudio/issues/205 ), BCL tagged material that came in as part of a construction do not show at the bottom in the left pane when looking at materials (Bugzilla #937)
- ![Fixed:][fixed] [#206]( https://github.com/NREL/OpenStudio/issues/206 ), Child records can be removed from the ProjectDatabase with poor results (Bugzilla #939)
- ![Fixed:][fixed] [#207]( https://github.com/NREL/OpenStudio/issues/207 ), Any chance OpenStudio could use "!-" instead of "!"? (Bugzilla #941)
- ![Fixed:][fixed] [#215]( https://github.com/NREL/OpenStudio/issues/215 ), HVAC Tab - When you select splitter or mixer the list of zones are not sorted (Bugzilla #953)
- ![Fixed:][fixed] [#216]( https://github.com/NREL/OpenStudio/issues/216 ), HVAC Tab - Thermal Zones ni Library collapse as soon as you add one to drop zone (Bugzilla #954)
- ![Fixed:][fixed] [#220]( https://github.com/NREL/OpenStudio/issues/220 ), HVAC Tab - Hard to tell when outdoor air object is selected (Bugzilla #960)
- ![Fixed:][fixed] [#222]( https://github.com/NREL/OpenStudio/issues/222 ), Crash when close properties dialog in results viewer (Bugzilla #964)
- ![Fixed:][fixed] [#224]( https://github.com/NREL/OpenStudio/issues/224 ), Get invalid warning messages when saving preferences (Bugzilla #966)
- ![Fixed:][fixed] [#232]( https://github.com/NREL/OpenStudio/issues/232 ), Thermal Zone Tab - If  thermal zone is on loop and you try and add it again the terminal is removed (Bugzilla #990)
- ![Fixed:][fixed] [#234]( https://github.com/NREL/OpenStudio/issues/234 ), Scan for tools not finding ReadVars (Bugzilla #993)
- ![Fixed:][fixed] [#236]( https://github.com/NREL/OpenStudio/issues/236 ), If an invalid osm is loaded as a template the user isn't given a dialog letting them know this (Bugzilla #1000)
- ![Fixed:][fixed] [#241]( https://github.com/NREL/OpenStudio/issues/241 ), OS toolbar greyed out with .osm loaded (Bugzilla #1007)
- ![Fixed:][fixed] [#246]( https://github.com/NREL/OpenStudio/issues/246 ), Plugin failure on mac with specific workflow and file (Bugzilla #1014)
- ![Fixed:][fixed] [#269]( https://github.com/NREL/OpenStudio/issues/269 ), RefBldgMidriseApartmentNew2004_Chicago.idf building crashes on import into OpenStudio (Bugzilla #1048)
- ![Fixed:][fixed] [#279]( https://github.com/NREL/OpenStudio/issues/279 ), Facility Tab - The on/off switch for thermostats does not function (Bugzilla #1067)
- ![Fixed:][fixed] [#299]( https://github.com/NREL/OpenStudio/issues/299 ), Model gets marked as dirty if I open Show Tools, don't do anything, then click ok. (Bugzilla #1165)
- ![Fixed:][fixed] [#308]( https://github.com/NREL/OpenStudio/issues/308 ), Run Tab - When I run installation test on 0.10.5 installer got odd warning in GUI about EP 7.2 missing (Bugzilla #1195)
- ![Fixed:][fixed] [#313]( https://github.com/NREL/OpenStudio/issues/313 ), Scan for tools will incorrectly find newer versions of E+ (Bugzilla #1214)
- ![Fixed:][fixed] [#315]( https://github.com/NREL/OpenStudio/issues/315 ), Illuminance maps do not show up for radout.sql files in tree view (Bugzilla #1216)
- ![Fixed:][fixed] [#333]( https://github.com/NREL/OpenStudio/issues/333 ), Example of bad link with SKP and OSM file (Bugzilla #1249)
- ![Fixed:][fixed] [#336]( https://github.com/NREL/OpenStudio/issues/336 ), Clean up and consolidate results processing (Bugzilla #1254)
- ![Fixed:][fixed] [#345]( https://github.com/NREL/OpenStudio/issues/345 ), Enforce unique naming when using "OpenStudio::Model::LifeCycleCost.createLifeCycleCost" (Bugzilla #1272)
- ![Fixed:][fixed] [#356]( https://github.com/NREL/OpenStudio/issues/356 ), PAT target does not depend on ruby dlls (Bugzilla #1291)
- ![Fixed:][fixed] [#358]( https://github.com/NREL/OpenStudio/issues/358 ), Instance method translation hides class instance in Transformation class (Bugzilla #1293)
- ![Fixed:][fixed] [#364]( https://github.com/NREL/OpenStudio/issues/364 ), Construction with internal source does not update (Bugzilla #1301)
- ![Fixed:][fixed] [#381]( https://github.com/NREL/OpenStudio/issues/381 ), On Mac if you lose path to Load Library in OS App, you can't browse back to it.
- ![Fixed:][fixed] [#407]( https://github.com/NREL/OpenStudio/issues/407 ), OS App does not tell you what happened when it fails to set weather file
- ![Fixed:][fixed] [#430]( https://github.com/NREL/OpenStudio/issues/430 ), ResultsViewer tree view grouping not logical with E+8.0 output variable renaming
- ![Fixed:][fixed] [#466]( https://github.com/NREL/OpenStudio/issues/466 ), People work efficiency default too high
- ![Fixed:][fixed] [#474]( https://github.com/NREL/OpenStudio/issues/474 ), OpenStudio App no longer prompts to scan for tools on run
- ![Fixed:][fixed] [#479]( https://github.com/NREL/OpenStudio/issues/479 ), On mac clicking the "Open Results Viewer .. " button launches the app but does not bring it to the front
- ![Fixed:][fixed] [#499]( https://github.com/NREL/OpenStudio/issues/499 ), ExtractUserScriptArguments.rb should be removed
- ![Fixed:][fixed] [#532]( https://github.com/NREL/OpenStudio/issues/532 ), All drop zone under Water Use Equipment Definitions doesn't enforce schedule types
- ![Fixed:][fixed] [#533]( https://github.com/NREL/OpenStudio/issues/533 ), Water Use Equipment Drop zones don't let you overwrite schedules
- ![Fixed:][fixed] [#536]( https://github.com/NREL/OpenStudio/issues/536 ), Setpoint Manager Warmest doesn't have an icon in the library
- ![Fixed:][fixed] [#629]( https://github.com/NREL/OpenStudio/issues/629 ), Bad default version in GenerateIddFactory.cpp
- ![Fixed:][fixed] [#710]( https://github.com/NREL/OpenStudio/issues/710 ), App no longer saves after run completes?
- ![Fixed:][fixed] [#753]( https://github.com/NREL/OpenStudio/issues/753 ), After failed simulation old results still show in results tab, but not functional
- ![Fixed:][fixed] [#791]( https://github.com/NREL/OpenStudio/issues/791 ), Measure fail to run on fresh install
- ![Fixed:][fixed] [#810]( https://github.com/NREL/OpenStudio/issues/810 ), SketchUp 14 - some dialog initially come up too small and have to be scaled
- ![Fixed:][fixed] [#833]( https://github.com/NREL/OpenStudio/issues/833 ), When a surface boundary condition is changed to "Outdoors" the default should be to make it sun and wind exposed
- ![Fixed:][fixed] [#926]( https://github.com/NREL/OpenStudio/issues/926 ), I need to look at EPD for Print room in Templates
- ![Fixed:][fixed] [#1063]( https://github.com/NREL/OpenStudio/issues/1063 ), Issue with construction sets that have res and non res versions - duplicate names
- ![Fixed:][fixed] [#1114]( https://github.com/NREL/OpenStudio/issues/1114 ), New model from template prompts to save in templates dir
- ![Fixed:][fixed] [#1342]( https://github.com/NREL/OpenStudio/issues/1342 ), Add model.getLights alias for model.getLightss
- ![Fixed:][fixed] [#1412]( https://github.com/NREL/OpenStudio/issues/1412 ), Changing a Space Type definition's load in IG does not update in gridview
- ![Fixed:][fixed] [#1471]( https://github.com/NREL/OpenStudio/issues/1471 ), SQL files containing illuminance maps are very large
- ![Fixed:][fixed] [#1525]( https://github.com/NREL/OpenStudio/issues/1525 ), Illuminance maps have confusing time scale
- ![Fixed:][fixed] [#1535]( https://github.com/NREL/OpenStudio/issues/1535 ), Resultsviewer center and span button defaults are wrong for flood plots
- ![Fixed:][fixed] [#1546]( https://github.com/NREL/OpenStudio/issues/1546 ), Apply to Selected - issue on larger models (Selecting affect zones/ space types)
- ![Fixed:][fixed] [#1550]( https://github.com/NREL/OpenStudio/issues/1550 ), Thermal Zone tab Library disappears
- ![Fixed:][fixed] [#1562]( https://github.com/NREL/OpenStudio/issues/1562 ), Loading empty DDY file appears to work but does nothing
- ![Fixed:][fixed] [#1569]( https://github.com/NREL/OpenStudio/issues/1569 ), ResultsViewer cannot open file on network drive
- ![Fixed:][fixed] [#1572]( https://github.com/NREL/OpenStudio/issues/1572 ), Updates to measure guide
- ![Fixed:][fixed] [#1574]( https://github.com/NREL/OpenStudio/issues/1574 ), Weather files with non-epw extensions (e.g. amy)
- ![Fixed:][fixed] [#1592]( https://github.com/NREL/OpenStudio/issues/1592 ), If file requested by OS:WeatherFile can't be found no simulation should be run.
- ![Fixed:][fixed] [#1595]( https://github.com/NREL/OpenStudio/issues/1595 ), RubyTest-DaylightSim_Test-DaylightSim_genannual failure
- ![Fixed:][fixed] [#1651]( https://github.com/NREL/OpenStudio/issues/1651 ), Convert method units documentation
- ![Fixed:][fixed] [#1663]( https://github.com/NREL/OpenStudio/issues/1663 ), Closing sqlfile in ResultsViewer does not remove the timeseries associated with it
- ![Fixed:][fixed] [#1714]( https://github.com/NREL/OpenStudio/issues/1714 ), measure testing issue on mac
- ![Fixed:][fixed] [#1715]( https://github.com/NREL/OpenStudio/issues/1715 ), Thermal Zone Grid View won't accept decimals for zone multiplier
- ![Fixed:][fixed] [#1750]( https://github.com/NREL/OpenStudio/issues/1750 ), Setting color scales in ResultsViewer does not work
- ![Fixed:][fixed] [#1850]( https://github.com/NREL/OpenStudio/issues/1850 ), ResultsViewer images have poor image quality
- ![Fixed:][fixed] [#1882]( https://github.com/NREL/OpenStudio/issues/1882 ), OpenStudio ForwardTranslator doesn't check if bypass already exists on demand side of plantLoop
- ![Fixed:][fixed] [#1884]( https://github.com/NREL/OpenStudio/issues/1884 ), PTACs have different characteristics between library and OpenStudio::Model::addSystemType1
- ![Fixed:][fixed] [#1939]( https://github.com/NREL/OpenStudio/issues/1939 ), Calibration queries do not work without variable request workaround
- ![Fixed:][fixed] [#1952]( https://github.com/NREL/OpenStudio/issues/1952 ), Assertion failure in StandardReports
- ![Fixed:][fixed] [#2008]( https://github.com/NREL/OpenStudio/issues/2008 ), Name not reverse translated for ScheduleConstant from IDF to OSM
- ![Fixed:][fixed] [#2013]( https://github.com/NREL/OpenStudio/issues/2013 ), 1.10.0 closes when changing Results Summary Report
- ![Fixed:][fixed] [#2022]( https://github.com/NREL/OpenStudio/issues/2022 ), Air terminal magic not applied to all terminals
- ![Fixed:][fixed] [#2038]( https://github.com/NREL/OpenStudio/issues/2038 ), Decimal separator disable in construction material values fields
- ![Fixed:][fixed] [#2047]( https://github.com/NREL/OpenStudio/issues/2047 ), ResultsViewer updated alias does not propagate to the Table or Tree Views
- ![Fixed:][fixed] [#2091]( https://github.com/NREL/OpenStudio/issues/2091 ), Add  WaterHeater:HeatPump:WrappedCondenser
- ![Fixed:][fixed] [#2095]( https://github.com/NREL/OpenStudio/issues/2095 ), new FanZoneExhaust fields not turned on after version update
- ![Fixed:][fixed] [#2117]( https://github.com/NREL/OpenStudio/issues/2117 ), 1.10.4 - Thermal Zones inspector breaks after deleting zone hvac object
- ![Fixed:][fixed] [#2176]( https://github.com/NREL/OpenStudio/issues/2176 ), IP Units Humidity Indicating Type
- ![Fixed:][fixed] [#2253]( https://github.com/NREL/OpenStudio/issues/2253 ), 1.11.4 - CoilCoolingDXTwoStageWithHumidityControlMode missing required fields
- ![Fixed:][fixed] [#2266]( https://github.com/NREL/OpenStudio/issues/2266 ), ThermalStorage categorys dont show in "My Model" on HVAC Tab
- ![Fixed:][fixed] [#2373]( https://github.com/NREL/OpenStudio/issues/2373 ), Thermal Storage Number of Nodes forward translation
- ![Fixed:][fixed] [#2410]( https://github.com/NREL/OpenStudio/issues/2410 ), Add AvailabilityManager:HighTemperatureTurnOff and others for solar collector
- ![Fixed:][fixed] [#2460]( https://github.com/NREL/OpenStudio/issues/2460 ), objects display Node handles instead of names
- ![Fixed:][fixed] [#2463]( https://github.com/NREL/OpenStudio/issues/2463 ), Purge unused not working in Stories subtab
- ![Fixed:][fixed] [#2568]( https://github.com/NREL/OpenStudio/issues/2568 ), Install Instructions page needs to be updated for 2.x
- ![Fixed:][fixed] [#2621]( https://github.com/NREL/OpenStudio/issues/2621 ), Any reason for 2x API to support Meter as well as OutputMeter, or should measures using this just change
- ![Fixed:][fixed] [#2647]( https://github.com/NREL/OpenStudio/issues/2647 ), Intersect and Surface Match Regression Between v2.1 and v1.14
- ![Fixed:][fixed] [#2652]( https://github.com/NREL/OpenStudio/issues/2652 ), Update Command Line Interface documentation
- ![Fixed:][fixed] [#2656]( https://github.com/NREL/OpenStudio/issues/2656 ), Change template for new measures to replace deprecated OpenStudio::Ruleset with OpenStudio::Measure
- ![Fixed:][fixed] [#2693]( https://github.com/NREL/OpenStudio/issues/2693 ), Importing DDY file creates duplicate design days
- ![Fixed:][fixed] [#2697]( https://github.com/NREL/OpenStudio/issues/2697 ), SaveAs while server starting mixes up files between the original location and the new location
- ![Fixed:][fixed] [#2702]( https://github.com/NREL/OpenStudio/issues/2702 ), SaveAs after server finishes starting writes wrong path in new location and can delete measures from old location
- ![Fixed:][fixed] [#2710]( https://github.com/NREL/OpenStudio/issues/2710 ), Can't edit fraction radiant, visible, or return air fraction for luminaires and lights
- ![Fixed:][fixed] [#2714]( https://github.com/NREL/OpenStudio/issues/2714 ), Expose fuel type for other equipment in UI
- ![Fixed:][fixed] [#2720]( https://github.com/NREL/OpenStudio/issues/2720 ), Crash on clone plenum space type
- ![Fixed:][fixed] [#2749]( https://github.com/NREL/OpenStudio/issues/2749 ), Better default for HeatExchangerFluidToFluid ComponentType
- ![Fixed:][fixed] [#2757]( https://github.com/NREL/OpenStudio/issues/2757 ), Space load instances have non-editable fields in inspector
- ![Fixed:][fixed] [#2764]( https://github.com/NREL/OpenStudio/issues/2764 ), OptionalBCLMeasure is not exported to SWIG
- ![Fixed:][fixed] [#2778]( https://github.com/NREL/OpenStudio/issues/2778 ), Luminaire won't allow input higher than 1.0 for Lighting Power
- ![Fixed:][fixed] [#2781]( https://github.com/NREL/OpenStudio/issues/2781 ), Clicking on WaterUseConnection in project crashes OS app
- ![Fixed:][fixed] [#2785]( https://github.com/NREL/OpenStudio/issues/2785 ), thermal_zone.airLoopHVACTerminal returns PTAC object
- ![Fixed:][fixed] [#2812]( https://github.com/NREL/OpenStudio/issues/2812 ), Incorrect MultiSpeed forward translation in 2.3 pre-release
- ![Fixed:][fixed] [#2821]( https://github.com/NREL/OpenStudio/issues/2821 ), AWS optimization runs never finish on the server
- ![Fixed:][fixed] [#2833]( https://github.com/NREL/OpenStudio/issues/2833 ), OpenStudio Crashes on Preview in Geometry Tab
- ![Fixed:][fixed] [#2839]( https://github.com/NREL/OpenStudio/issues/2839 ), Purge Unused Objects in Space Tab deletes all spaces
- ![Fixed:][fixed] [#2849]( https://github.com/NREL/OpenStudio/issues/2849 ), Daylighting Control Illuminance Setpoint Error
- ![Fixed:][fixed] [#2850]( https://github.com/NREL/OpenStudio/issues/2850 ), App and Plugin allow invalid zone load fractions on daylighting controls
- ![Fixed:][fixed] [#2855]( https://github.com/NREL/OpenStudio/issues/2855 ), add AirLoopHVACUnitarySystem::setControlType
- ![Fixed:][fixed] [#2858]( https://github.com/NREL/OpenStudio/issues/2858 ), add missing pump fields from E+ 8.5 and beyond
- ![Fixed:][fixed] [#2862]( https://github.com/NREL/OpenStudio/issues/2862 ), Allow ZoneHVACTerminalUnitVariableRefrigerantFlow to be configured without a heating coil
- ![Fixed:][fixed] [#2864]( https://github.com/NREL/OpenStudio/issues/2864 ), conditionally translate VRF terminal OA mixer
- ![Fixed:][fixed] [#2865]( https://github.com/NREL/OpenStudio/issues/2865 ), Add new fields for cooling towers
- ![Fixed:][fixed] [#2868]( https://github.com/NREL/OpenStudio/issues/2868 ), Extend AvailabilityManager:NightCycle to include new fields
- ![Fixed:][fixed] [#2872]( https://github.com/NREL/OpenStudio/issues/2872 ), make AirLoopHVAC:UnitarySystem object Zone HVAC Equipment
- ![Improved:][improved] [#311]( https://github.com/NREL/OpenStudio/issues/311 ), Test models from measures make it into every datapoint folder (Bugzilla #1207)
- ![Improved:][improved] [#1375]( https://github.com/NREL/OpenStudio/issues/1375 ), CSV export from OpenStudio
- ![Improved:][improved] [#1432]( https://github.com/NREL/OpenStudio/issues/1432 ), Create Measures for 90.1 Appendix G System Types
- ![Improved:][improved] [#1479]( https://github.com/NREL/OpenStudio/issues/1479 ), Construct Vector from Ruby array
- ![Improved:][improved] [#1581]( https://github.com/NREL/OpenStudio/issues/1581 ), Consistent Placement of Add/Delete/Copy Buttons
- ![Improved:][improved] [#1582]( https://github.com/NREL/OpenStudio/issues/1582 ), Increase Header Emphasis 
- ![Improved:][improved] [#1626]( https://github.com/NREL/OpenStudio/issues/1626 ), Ability to copy log messages from measures
- ![Improved:][improved] [#1661]( https://github.com/NREL/OpenStudio/issues/1661 ), Add IP / SI unit preference to measures 
- ![Improved:][improved] [#1729]( https://github.com/NREL/OpenStudio/issues/1729 ), Add method for creating non-ribbon windows
- ![Improved:][improved] [#1973]( https://github.com/NREL/OpenStudio/issues/1973 ), Add missing dual duct terminal types.
- ![Improved:][improved] [#2112]( https://github.com/NREL/OpenStudio/issues/2112 ), add Unassigned choice to Spaces tab Space Type filter
- ![Improved:][improved] [#2175]( https://github.com/NREL/OpenStudio/issues/2175 ), Expose Sky Clearness field in OS App for Design Days
- ![Improved:][improved] [#2215]( https://github.com/NREL/OpenStudio/issues/2215 ), ZoneVentilation:DesignFlowRate doesn't react as usual when setting flow
- ![Improved:][improved] [#2296]( https://github.com/NREL/OpenStudio/issues/2296 ), AirLoopHVAC:UnitarySystem Feature Request
- ![Improved:][improved] [#2307]( https://github.com/NREL/OpenStudio/issues/2307 ), Coil:Heating:WaterToAirHeatPump:EquationFit expects optional doubles
- ![Improved:][improved] [#2379]( https://github.com/NREL/OpenStudio/issues/2379 ), No Thermostats for cooling only/ heating only zones

## OpenStudio v2.3.0
###### 2017-09-29 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.2.2...v2.3.0 )

- ![Fixed:][fixed] [#2079]( https://github.com/NREL/OpenStudio/issues/2079 ), Fan:ZoneExhaust Balance Exhaust Fraction default to the opposite of EnergyPlus
- ![Fixed:][fixed] [#2182]( https://github.com/NREL/OpenStudio/issues/2182 ), Wrong boundary in OS:WaterHeater:HeatPump  'Minimum Inlet Air Temperature for Compressor Operation'
- ![Fixed:][fixed] [#2598]( https://github.com/NREL/OpenStudio/issues/2598 ), Fwd translation of EMS:InternalVariable does not sub UIDs w/ object names
- ![Fixed:][fixed] [#2604]( https://github.com/NREL/OpenStudio/issues/2604 ), App locks up on Results Tab in Windows 10
- ![Fixed:][fixed] [#2786]( https://github.com/NREL/OpenStudio/issues/2786 ), Outside Boundary Condition Object blank for interior surfaces
- ![Fixed:][fixed] [#2789]( https://github.com/NREL/OpenStudio/issues/2789 ), FourPipe Fan Coil fails in 2.2.2 simulation with EnergyPlus 8.8
- ![Improved:][improved] [#2054]( https://github.com/NREL/OpenStudio/issues/2054 ), CoilCoolingDXSingleSpeed requires an OptionalDouble for setRatedCOP

## OpenStudio v2.2.2
###### 2017-09-13 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.2.1...v2.2.2 )

- ![Fixed:][fixed] [#2562]( https://github.com/NREL/OpenStudio/issues/2562 ), Apply Measure Now Clears Measures Tab
- ![Fixed:][fixed] [#2602]( https://github.com/NREL/OpenStudio/issues/2602 ), Can't save PAT 2.1.1 project on exit
- ![Fixed:][fixed] [#2730]( https://github.com/NREL/OpenStudio/issues/2730 ), Opening existing floorplan in editor freezes app
- ![Fixed:][fixed] [#2763]( https://github.com/NREL/OpenStudio/issues/2763 ), PAT Doesn't Show Analysis Complete for Modest Cloud Runs

## OpenStudio v2.2.1
###### 2017-09-06 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.2.0...v2.2.1 )

- ![Fixed:][fixed] [#162]( https://github.com/NREL/OpenStudio/issues/162 ), Typo in People Definition tab (Bugzilla #840)
- ![Fixed:][fixed] [#1632]( https://github.com/NREL/OpenStudio/issues/1632 ), Incorrect results in OpenStudio App.
- ![Fixed:][fixed] [#2317]( https://github.com/NREL/OpenStudio/issues/2317 ), incorrct areas in EnergyPlus Results
- ![Fixed:][fixed] [#2397]( https://github.com/NREL/OpenStudio/issues/2397 ),  OpenStudio SketchUp 2016 crashed when opens
- ![Fixed:][fixed] [#2457]( https://github.com/NREL/OpenStudio/issues/2457 ), OS App Units missing on Materials and Internal Loads (except for people) in 2.0.1
- ![Improved:][improved] [#2500]( https://github.com/NREL/OpenStudio/issues/2500 ), Make PAT "Additional Analysis Files" path relative
- ![Fixed:][fixed] [#2619]( https://github.com/NREL/OpenStudio/issues/2619 ), Successive add/delete of Measures in App's Measures Tab Creates Cruft
- ![Fixed:][fixed] [#2629]( https://github.com/NREL/OpenStudio/issues/2629 ), Duplicated measure results are overwritten
- ![Improved:][improved] [#2632]( https://github.com/NREL/OpenStudio/issues/2632 ), Server takes to long to start
- ![Fixed:][fixed] [#2669]( https://github.com/NREL/OpenStudio/issues/2669 ), Prevent user from naming PAT project 'PAT'
- ![Fixed:][fixed] [#2676]( https://github.com/NREL/OpenStudio/issues/2676 ), PAT 2.1.2 deletes measure from library
- ![Fixed:][fixed] [#2677]( https://github.com/NREL/OpenStudio/issues/2677 ), PAT Results Summary Table Report has wrong sign for capital cost increase
- ![Fixed:][fixed] [#2681]( https://github.com/NREL/OpenStudio/issues/2681 ), Surface disappears after saving in SketchUp - after using Intersect and Divide Inter-Zone Surface for the entire model
- ![Fixed:][fixed] [#2688]( https://github.com/NREL/OpenStudio/issues/2688 ), Supply air fan operating mode
- ![Fixed:][fixed] [#2690]( https://github.com/NREL/OpenStudio/issues/2690 ), Duplicate Measure, breaks if you then update the measure
- ![Fixed:][fixed] [#2691]( https://github.com/NREL/OpenStudio/issues/2691 ), About PAT window hasn't been updated in agest
- ![Fixed:][fixed] [#2694]( https://github.com/NREL/OpenStudio/issues/2694 ), Segfault in Materials Tab
- ![Fixed:][fixed] [#2695]( https://github.com/NREL/OpenStudio/issues/2695 ), User Reports PAT Local Server Not Starting Reliably on Mac
- ![Improved:][improved] [#2716]( https://github.com/NREL/OpenStudio/issues/2716 ), AWS deployed server needs to run 350k data-points
- ![Fixed:][fixed] [#2719]( https://github.com/NREL/OpenStudio/issues/2719 ), PAT hangs when running simulation on the cloud
- ![Fixed:][fixed] [#2721]( https://github.com/NREL/OpenStudio/issues/2721 ), PAT thinks cluster is running (but it's not)
- ![Fixed:][fixed] [#2723]( https://github.com/NREL/OpenStudio/issues/2723 ), Third PAT algorithm argument in Morris should be Grid Jump
- ![Fixed:][fixed] [#2724]( https://github.com/NREL/OpenStudio/issues/2724 ), Morris Method Box Plots are Empty in 2.2.0
- ![Fixed:][fixed] [#2728]( https://github.com/NREL/OpenStudio/issues/2728 ), PAT Analysis Error when updating measure w/ duplicates
- ![Fixed:][fixed] [#2729]( https://github.com/NREL/OpenStudio/issues/2729 ), Place Map dialog no longer pops up on new editor
- ![Fixed:][fixed] [#2732]( https://github.com/NREL/OpenStudio/issues/2732 ), No way to dismiss preview window
- ![Fixed:][fixed] [#2733]( https://github.com/NREL/OpenStudio/issues/2733 ), Geometry boundary conditions in editor preview are wrong
- ![Improved:][improved] [#2734]( https://github.com/NREL/OpenStudio/issues/2734 ), No option to continue editing geometry after merge operation
- ![Fixed:][fixed] [#2736]( https://github.com/NREL/OpenStudio/issues/2736 ), Returning to geometry editor pane after merge produces inconsistent results
- ![Improved:][improved] [#2737]( https://github.com/NREL/OpenStudio/issues/2737 ), No feedback from App post-merge
- ![Fixed:][fixed] [#2738]( https://github.com/NREL/OpenStudio/issues/2738 ), Boundary condition between floors doesn't appear correct
- ![Fixed:][fixed] [#2740]( https://github.com/NREL/OpenStudio/issues/2740 ), Floor height isn't translated
- ![Fixed:][fixed] [#2745]( https://github.com/NREL/OpenStudio/issues/2745 ), Going to Preview Tab throws away unmerged changes
- ![Fixed:][fixed] [#2746]( https://github.com/NREL/OpenStudio/issues/2746 ), Prevent PAT from running projects with zero variables
- ![Fixed:][fixed] [#2748]( https://github.com/NREL/OpenStudio/issues/2748 ), Levels in Morris Method Must be >1
- ![Fixed:][fixed] [#2754]( https://github.com/NREL/OpenStudio/issues/2754 ), App locks up on image import
- ![Fixed:][fixed] [#2760]( https://github.com/NREL/OpenStudio/issues/2760 ), PAT Not removing Measures from Projects

## OpenStudio v2.2.0
###### 2017-06-30 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.1.2...v2.2.0 )

- ![Fixed:][fixed] [#2496]( https://github.com/NREL/OpenStudio/issues/2496 ), Chinese characters in Design Alternatives grid
- ![Fixed:][fixed] [#2503]( https://github.com/NREL/OpenStudio/issues/2503 ), Skipping measures in datapoint throws measure messages out of sync
- ![Fixed:][fixed] [#2512]( https://github.com/NREL/OpenStudio/issues/2512 ), Dropdown menu for "Select Cluster or Make New One" is not functioning correctly. PAT v2.0.4 (windows 7)
- ![Fixed:][fixed] [#2541]( https://github.com/NREL/OpenStudio/issues/2541 ), Variable distribution plots appear on server as broken links
- ![Fixed:][fixed] [#2630]( https://github.com/NREL/OpenStudio/issues/2630 ), OS Cloud Management Console pagination bugs
- ![Fixed:][fixed] [#2634]( https://github.com/NREL/OpenStudio/issues/2634 ), Crash on Utility Bill page
- ![Fixed:][fixed] [#2637]( https://github.com/NREL/OpenStudio/issues/2637 ), Rerunning old PAT project in 2.1.2 creates new data points instead of overwriting old ones
- ![Fixed:][fixed] [#2640]( https://github.com/NREL/OpenStudio/issues/2640 ), PAT's Results Tab Goes Completely Blank w/ Large Algorithm Projects
- ![Fixed:][fixed] [#2650]( https://github.com/NREL/OpenStudio/issues/2650 ), Solution exception:index out of range: 0 _ energyPlus
- ![Fixed:][fixed] [#2665]( https://github.com/NREL/OpenStudio/issues/2665 ), App not shutting down correctly
- ![Fixed:][fixed] [#2667]( https://github.com/NREL/OpenStudio/issues/2667 ), Closing App Window on Mac Destroys Menu Options
- ![Fixed:][fixed] [#2674]( https://github.com/NREL/OpenStudio/issues/2674 ), PlantLoop GlycolConcentration is not initialized
- ![Improved:][improved] [#2601]( https://github.com/NREL/OpenStudio/issues/2601 ), Ability to skip NA simulations

## OpenStudio v2.1.2
###### 2017-06-20 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.1.1...v2.1.2 )

- ![Fixed:][fixed] [#59]( https://github.com/NREL/OpenStudio/issues/59 ), Installer does not install OpenStudio.idd (Bugzilla #459)
- ![Fixed:][fixed] [#76]( https://github.com/NREL/OpenStudio/issues/76 ), RunManager fails to search for tools (Bugzilla #519)
- ![Fixed:][fixed] [#181]( https://github.com/NREL/OpenStudio/issues/181 ), EnergyPlus ForwardTranslator errors do not appear in the RunManager GUI elements (Bugzilla #897)
- ![Fixed:][fixed] [#183]( https://github.com/NREL/OpenStudio/issues/183 ), Web Documentation points to files that are not in installer (Bugzilla #900)
- ![Fixed:][fixed] [#288]( https://github.com/NREL/OpenStudio/issues/288 ), PAT and OpenStudio store tools separately (Bugzilla #1100)
- ![Fixed:][fixed] [#290]( https://github.com/NREL/OpenStudio/issues/290 ), Misc. GUI things I noticed (Bugzilla #1127)
- ![Fixed:][fixed] [#292]( https://github.com/NREL/OpenStudio/issues/292 ), Weird spacing when you expand a data point job with content (Bugzilla #1137)
- ![Fixed:][fixed] [#300]( https://github.com/NREL/OpenStudio/issues/300 ), Application name is Pat.exe instead of ParametricAnalysisTool.exe (Bugzilla #1167)
- ![Fixed:][fixed] [#303]( https://github.com/NREL/OpenStudio/issues/303 ), Weather files are incorrect for models opened in openstudio from results tab (Bugzilla #1179)
- ![Fixed:][fixed] [#306]( https://github.com/NREL/OpenStudio/issues/306 ), Tab 3 - ruby errors for measure not as described as in command line run (Bugzilla #1188)
- ![Fixed:][fixed] [#309]( https://github.com/NREL/OpenStudio/issues/309 ), Tab 3 - would be nice to test measures before clicking off runs in to improve workflow (Bugzilla #1199)
- ![Fixed:][fixed] [#310]( https://github.com/NREL/OpenStudio/issues/310 ), Tab 3 - After running a job and adding new alternatives the warnings disappear from original run (Bugzilla #1200)
- ![Fixed:][fixed] [#329]( https://github.com/NREL/OpenStudio/issues/329 ), crash if forget "return args" in measure.rb then click "update my measures" (Bugzilla #1239)
- ![Fixed:][fixed] [#331]( https://github.com/NREL/OpenStudio/issues/331 ), I opened a project Andrew made in 0.11.3 in current trunk, crashes when I try to select a measure instance in tab one (Bugzilla #1242)
- ![Fixed:][fixed] [#332]( https://github.com/NREL/OpenStudio/issues/332 ), Measure Tab - You can paste multiline text into name field (Bugzilla #1248)
- ![Fixed:][fixed] [#339]( https://github.com/NREL/OpenStudio/issues/339 ), PAT memory leak crash - this one came from Alex (Bugzilla #1263)
- ![Fixed:][fixed] [#342]( https://github.com/NREL/OpenStudio/issues/342 ), The dialog saying that measures were brought in from baseline seems to come up even when there are no measures (Bugzilla #1267)
- ![Fixed:][fixed] [#343]( https://github.com/NREL/OpenStudio/issues/343 ), Measure Tab - if you change seed model, you don't always see red triangle for measure where you should (Bugzilla #1268)
- ![Fixed:][fixed] [#346]( https://github.com/NREL/OpenStudio/issues/346 ), updates to XML don't show up in PAT for projects that already have that measure (Bugzilla #1273)
- ![Fixed:][fixed] [#351]( https://github.com/NREL/OpenStudio/issues/351 ), Tab 4 - the line at the right side of the design alternative box isn't visible. (Bugzilla #1279)
- ![Fixed:][fixed] [#353]( https://github.com/NREL/OpenStudio/issues/353 ), Tab 1 - The measures in MyMeasures are not sorted by display name (Bugzilla #1286)
- ![Fixed:][fixed] [#362]( https://github.com/NREL/OpenStudio/issues/362 ), Clear results doesn't always clear all results (Bugzilla #1299)
- ![Fixed:][fixed] [#385]( https://github.com/NREL/OpenStudio/issues/385 ), Measure writing guide example for "makeChoiceArguemntOfWorkspaceOjbects" doesn't work.
- ![Fixed:][fixed] [#480]( https://github.com/NREL/OpenStudio/issues/480 ), PAT no longer shows warning icon if baseline osm is not set
- ![Fixed:][fixed] [#507]( https://github.com/NREL/OpenStudio/issues/507 ), Problem when importing measure with same name as measure already in project
- ![Fixed:][fixed] [#541]( https://github.com/NREL/OpenStudio/issues/541 ), Possible file size reductions in PAT
- ![Fixed:][fixed] [#542]( https://github.com/NREL/OpenStudio/issues/542 ), On large job you still see "0% Failed after a job fails in the workflow
- ![Fixed:][fixed] [#546]( https://github.com/NREL/OpenStudio/issues/546 ), Issues in Onlnie BCL window when viewing sections with more than 10 measures
- ![Fixed:][fixed] [#569]( https://github.com/NREL/OpenStudio/issues/569 ), In some situations design alternates made using "Create One For Each Selected Measure" are out of order
- ![Fixed:][fixed] [#600]( https://github.com/NREL/OpenStudio/issues/600 ), EnergyPlus failing on one run, then not when I re-run
- ![Fixed:][fixed] [#613]( https://github.com/NREL/OpenStudio/issues/613 ), There is no way for a user to manually shut down the cloud whenever they want.
- ![Fixed:][fixed] [#614]( https://github.com/NREL/OpenStudio/issues/614 ), Should be able to download details from run tab even if DataPoint is complete
- ![Fixed:][fixed] [#621]( https://github.com/NREL/OpenStudio/issues/621 ), On mac, opening PAT projects produces a strange RunManager error
- ![Fixed:][fixed] [#626]( https://github.com/NREL/OpenStudio/issues/626 ), Alert user if file fails to save. Ran across this in Plugin but may need to check App and PAT as well
- ![Fixed:][fixed] [#650]( https://github.com/NREL/OpenStudio/issues/650 ), Conflict between OpenStudio SketchUp plugin and Vray
- ![Fixed:][fixed] [#658]( https://github.com/NREL/OpenStudio/issues/658 ), PAT - unexpected jump to Tab 1
- ![Fixed:][fixed] [#692]( https://github.com/NREL/OpenStudio/issues/692 ), Invalid "tag" in built-in Replace Opentudio Model measure.rb file
- ![Fixed:][fixed] [#702]( https://github.com/NREL/OpenStudio/issues/702 ), Cloud run sometimes can't find the weather file
- ![Fixed:][fixed] [#703]( https://github.com/NREL/OpenStudio/issues/703 ), Clear Results Bug and graphic confusion
- ![Fixed:][fixed] [#815]( https://github.com/NREL/OpenStudio/issues/815 ), Measure warning icon does not go away on update my measures
- ![Fixed:][fixed] [#868]( https://github.com/NREL/OpenStudio/issues/868 ), Assert when running PAT project on the cloud
- ![Fixed:][fixed] [#916]( https://github.com/NREL/OpenStudio/issues/916 ), Radiance warnings presented in PAT everytime cloud status changes
- ![Fixed:][fixed] [#922]( https://github.com/NREL/OpenStudio/issues/922 ), Cannot download failed datapoints from server in PAT
- ![Fixed:][fixed] [#958]( https://github.com/NREL/OpenStudio/issues/958 ), Scan for tools finding wrong Ruby version
- ![Fixed:][fixed] [#959]( https://github.com/NREL/OpenStudio/issues/959 ), Spreadsheet export does not work with data points run on cloud
- ![Fixed:][fixed] [#989]( https://github.com/NREL/OpenStudio/issues/989 ), PAT crash involving directories opened in Explorer
- ![Fixed:][fixed] [#1001]( https://github.com/NREL/OpenStudio/issues/1001 ), PAT open last project path should not be altered when setting a baseline
- ![Fixed:][fixed] [#1042]( https://github.com/NREL/OpenStudio/issues/1042 ), Error message says BCL key not working when internet is not working
- ![Fixed:][fixed] [#1051]( https://github.com/NREL/OpenStudio/issues/1051 ), Scan for tools should be modal dialog
- ![Fixed:][fixed] [#1066]( https://github.com/NREL/OpenStudio/issues/1066 ), Terminology used by RunManager doesn't match OS terminology.
- ![Fixed:][fixed] [#1115]( https://github.com/NREL/OpenStudio/issues/1115 ), Items selected after clearing results seems random in PAT
- ![Fixed:][fixed] [#1116]( https://github.com/NREL/OpenStudio/issues/1116 ), PAT slow switching to tab 3
- ![Fixed:][fixed] [#1161]( https://github.com/NREL/OpenStudio/issues/1161 ), Info Messages not showing on cloud run
- ![Fixed:][fixed] [#1203]( https://github.com/NREL/OpenStudio/issues/1203 ), App Opening Delayed When Offline
- ![Fixed:][fixed] [#1204]( https://github.com/NREL/OpenStudio/issues/1204 ), Measure is added to project even if drop fails
- ![Fixed:][fixed] [#1261]( https://github.com/NREL/OpenStudio/issues/1261 ), PAT cloud can get stuck
- ![Fixed:][fixed] [#1315]( https://github.com/NREL/OpenStudio/issues/1315 ), QSslSocket: cannot resolve TLSv1_1_client_method
- ![Fixed:][fixed] [#1439]( https://github.com/NREL/OpenStudio/issues/1439 ), AWS.rb script should put logs in project dir
- ![Fixed:][fixed] [#1474]( https://github.com/NREL/OpenStudio/issues/1474 ), Bad warning message when no weather file attached
- ![Fixed:][fixed] [#1477]( https://github.com/NREL/OpenStudio/issues/1477 ), URL links in measure description are not clickable 
- ![Fixed:][fixed] [#1497]( https://github.com/NREL/OpenStudio/issues/1497 ), epw2wea check fails on Linux
- ![Fixed:][fixed] [#1536]( https://github.com/NREL/OpenStudio/issues/1536 ), setMinimumHotWaterOrStreamFlowRate (should be steam instead of stream)
- ![Fixed:][fixed] [#1690]( https://github.com/NREL/OpenStudio/issues/1690 ), Issues with commas in choice arguments in PAT
- ![Fixed:][fixed] [#1708]( https://github.com/NREL/OpenStudio/issues/1708 ), Error exporting PAT project to analysis spreadsheet
- ![Fixed:][fixed] [#1710]( https://github.com/NREL/OpenStudio/issues/1710 ), Export to spreadsheet when argument is choice list from model static value shows UUID vs. string
- ![Fixed:][fixed] [#1723]( https://github.com/NREL/OpenStudio/issues/1723 ), PAT Cloud not able to download detailed results
- ![Fixed:][fixed] [#1726]( https://github.com/NREL/OpenStudio/issues/1726 ), Runmanager loads models against wrong version of idd 
- ![Fixed:][fixed] [#1735]( https://github.com/NREL/OpenStudio/issues/1735 ), Standard report results take too long to render in 1.8.0
- ![Fixed:][fixed] [#1842]( https://github.com/NREL/OpenStudio/issues/1842 ), PAT - Amazon EC2 - Unable to update uuid with mismatches child job type
- ![Fixed:][fixed] [#1885]( https://github.com/NREL/OpenStudio/issues/1885 ), Running paegent seems to interfere with PAT cloud
- ![Fixed:][fixed] [#1898]( https://github.com/NREL/OpenStudio/issues/1898 ), When you copy a measure, the resulting measure has inadvertent space in display name.
- ![Fixed:][fixed] [#2017]( https://github.com/NREL/OpenStudio/issues/2017 ), PAT's "Sync Project Measures with Library" only updates 10 measures
- ![Fixed:][fixed] [#2021]( https://github.com/NREL/OpenStudio/issues/2021 ), Cannot use RubyJobBuilder for Reporting measures
- ![Fixed:][fixed] [#2161]( https://github.com/NREL/OpenStudio/issues/2161 ), Open Studio 1.10.0 makes SketchUp 2016 crash
- ![Fixed:][fixed] [#2167]( https://github.com/NREL/OpenStudio/issues/2167 ), Rename Fan Constant Volume object attribute "FanEfficiency" to "FanTotalEfficiency"
- ![Fixed:][fixed] [#2229]( https://github.com/NREL/OpenStudio/issues/2229 ), AirLoopHVACUnitarySystem fan placement doesn't default
- ![Fixed:][fixed] [#2273]( https://github.com/NREL/OpenStudio/issues/2273 ), Upgrade from 1.11.4 to 1.11.5 leads to empty OutdoorAir:NodeList in IDF file
- ![Fixed:][fixed] [#2324]( https://github.com/NREL/OpenStudio/issues/2324 ), PAT cloud setting
- ![Fixed:][fixed] [#2334]( https://github.com/NREL/OpenStudio/issues/2334 ), CLI documentation
- ![Fixed:][fixed] [#2337]( https://github.com/NREL/OpenStudio/issues/2337 ), typo - "euqipment" instead of equipment
- ![Fixed:][fixed] [#2339]( https://github.com/NREL/OpenStudio/issues/2339 ), Incorrect Display Name for 'Central Heating Maximum System Air Flow Ratio'
- ![Fixed:][fixed] [#2345]( https://github.com/NREL/OpenStudio/issues/2345 ), isomodel measure inconsistency
- ![Fixed:][fixed] [#2364]( https://github.com/NREL/OpenStudio/issues/2364 ), Crash in translate ZoneHVACEnergyRecoveryVentilator 
- ![Fixed:][fixed] [#2404]( https://github.com/NREL/OpenStudio/issues/2404 ), Is the E+ Ground Heat Exchanger:Slinky model present in Open Studio?
- ![Fixed:][fixed] [#2409]( https://github.com/NREL/OpenStudio/issues/2409 ), OpenStudio does not open & SketchUp plugin load errors
- ![Fixed:][fixed] [#2413]( https://github.com/NREL/OpenStudio/issues/2413 ), Can't assign CoilHeatingElectric to ZoneHVACFourPipeFanCoil
- ![Fixed:][fixed] [#2429]( https://github.com/NREL/OpenStudio/issues/2429 ), In PAT finder window (on mac), cannot create folder
- ![Fixed:][fixed] [#2431]( https://github.com/NREL/OpenStudio/issues/2431 ), PAT unable to reopen 2.0.0 project on Mac
- ![Fixed:][fixed] [#2435]( https://github.com/NREL/OpenStudio/issues/2435 ), Optional argument shows in red in OS 2.0
- ![Fixed:][fixed] [#2497]( https://github.com/NREL/OpenStudio/issues/2497 ), All options not getting first column value when measure arg isn't "variable"
- ![Fixed:][fixed] [#2501]( https://github.com/NREL/OpenStudio/issues/2501 ), Show units & descriptions for arguments in PAT
- ![Fixed:][fixed] [#2507]( https://github.com/NREL/OpenStudio/issues/2507 ), For measure args without default values I have to set as variable to run properly
- ![Fixed:][fixed] [#2518]( https://github.com/NREL/OpenStudio/issues/2518 ), AWS Workers Stuck in Queued State w/ 4 Core Server
- ![Fixed:][fixed] [#2521]( https://github.com/NREL/OpenStudio/issues/2521 ), Results Tab Not Working on Cloud Run
- ![Fixed:][fixed] [#2529]( https://github.com/NREL/OpenStudio/issues/2529 ), PAT won't download datapoint zip on Mac
- ![Fixed:][fixed] [#2540]( https://github.com/NREL/OpenStudio/issues/2540 ), PAT projects created on Windows don't run successfully on Mac
- ![Fixed:][fixed] [#2543]( https://github.com/NREL/OpenStudio/issues/2543 ), PAT Add Measure - Missing "Category" Filter
- ![Fixed:][fixed] [#2545]( https://github.com/NREL/OpenStudio/issues/2545 ), Optional (string) argument not working as expected in PAT 2.1.0
- ![Fixed:][fixed] [#2547]( https://github.com/NREL/OpenStudio/issues/2547 ), OpenStudio 2.0 Ruby inteface mismatch? or missing? from 1.x versions
- ![Fixed:][fixed] [#2548]( https://github.com/NREL/OpenStudio/issues/2548 ), Results disappear from Run Tab in Cloud Mode
- ![Fixed:][fixed] [#2551]( https://github.com/NREL/OpenStudio/issues/2551 ), Need some human-readable diagnostic messages for missing arguments
- ![Fixed:][fixed] [#2557]( https://github.com/NREL/OpenStudio/issues/2557 ), PAT 2.1.0 datapoint error due to optional arg
- ![Fixed:][fixed] [#2558]( https://github.com/NREL/OpenStudio/issues/2558 ), Can't start Amazon server on Windows w/ PAT 2.1.0
- ![Fixed:][fixed] [#2559]( https://github.com/NREL/OpenStudio/issues/2559 ), PAT 2.1.0 missing units
- ![Fixed:][fixed] [#2565]( https://github.com/NREL/OpenStudio/issues/2565 ), PAT 2.1.0 datapoint errors using duplicate measure
- ![Fixed:][fixed] [#2566]( https://github.com/NREL/OpenStudio/issues/2566 ), Window 2.1 installer sometimes silently quits
- ![Fixed:][fixed] [#2569]( https://github.com/NREL/OpenStudio/issues/2569 ), PAT 2.1.0 Analyses Fail w/ No Useful User Feedback Under Some Default Arg Conditions
- ![Fixed:][fixed] [#2574]( https://github.com/NREL/OpenStudio/issues/2574 ), Odd behavior on measure options grid for bool template
- ![Fixed:][fixed] [#2579]( https://github.com/NREL/OpenStudio/issues/2579 ), ShadowCalculation field updates another in OS App
- ![Fixed:][fixed] [#2583]( https://github.com/NREL/OpenStudio/issues/2583 ), Diag.rb error using integer_sequence
- ![Fixed:][fixed] [#2585]( https://github.com/NREL/OpenStudio/issues/2585 ), PAT 2.1.0 analysis error w/ no variables for diag
- ![Fixed:][fixed] [#2586]( https://github.com/NREL/OpenStudio/issues/2586 ), PAT Project Measures Get Out of State
- ![Fixed:][fixed] [#2591]( https://github.com/NREL/OpenStudio/issues/2591 ),  GroundHeatExchangerVertical::gFunctions not swigged
- ![Fixed:][fixed] [#2596]( https://github.com/NREL/OpenStudio/issues/2596 ), Measure manager doesn't remove missing files from the XML
- ![Fixed:][fixed] [#2605]( https://github.com/NREL/OpenStudio/issues/2605 ), Deal with external volume storage driver
- ![Fixed:][fixed] [#2607]( https://github.com/NREL/OpenStudio/issues/2607 ), Use private swarm-based registry
- ![Fixed:][fixed] [#2612]( https://github.com/NREL/OpenStudio/issues/2612 ), OpenStudio 2.1.1 Results Tab Unavailable
- ![Fixed:][fixed] [#2617]( https://github.com/NREL/OpenStudio/issues/2617 ), Custom Output Variables don't make it into analysis JSON properly
- ![Fixed:][fixed] [#2618]( https://github.com/NREL/OpenStudio/issues/2618 ), Static value for measure arguments/variables not being set properly in analysis JSON
- ![Fixed:][fixed] [#2623]( https://github.com/NREL/OpenStudio/issues/2623 ), Forward Translate AvailabilityManager:DifferentialThermostat
- ![Fixed:][fixed] [#2628]( https://github.com/NREL/OpenStudio/issues/2628 ), Feature Request: Add actual number of floors to model. 
- ![Fixed:][fixed] [#2635]( https://github.com/NREL/OpenStudio/issues/2635 ), Area mismatch- not a software issue. an internal wall construction was force assigned to a exterior wall
- ![Fixed:][fixed] [#2645]( https://github.com/NREL/OpenStudio/issues/2645 ), PTAC, PTHP and WAHP should omit OA Mixer when not used
- ![Fixed:][fixed] [#2646]( https://github.com/NREL/OpenStudio/issues/2646 ), PTAC, PTHP, and water to air HP should support blow through and draw through
- ![Fixed:][fixed] [#2651]( https://github.com/NREL/OpenStudio/issues/2651 ), ZoneHVACFourPipeFanCoil fan operating mode schedule
- ![Improved:][improved] [#294]( https://github.com/NREL/OpenStudio/issues/294 ), View Reports Tab - Open Copy in OpenStudio doesn't bring weather file over (Bugzilla #1153)
- ![Improved:][improved] [#305]( https://github.com/NREL/OpenStudio/issues/305 ), Tab 1 - gui isn't sorting choice objects in the pull down list (Bugzilla #1186)
- ![Improved:][improved] [#1444]( https://github.com/NREL/OpenStudio/issues/1444 ), Easily set weather file in PAT
- ![Improved:][improved] [#1483]( https://github.com/NREL/OpenStudio/issues/1483 ), Allow measures to require gems
- ![Improved:][improved] [#1553]( https://github.com/NREL/OpenStudio/issues/1553 ), Remove scan for tools
- ![Improved:][improved] [#1583]( https://github.com/NREL/OpenStudio/issues/1583 ), Wrap Tab Boundary Around Whole Viewing Pane
- ![Improved:][improved] [#1724]( https://github.com/NREL/OpenStudio/issues/1724 ), RegisterInfo method very slow
- ![Improved:][improved] [#1793]( https://github.com/NREL/OpenStudio/issues/1793 ), Show Simple ventilation in reporting measure
- ![Improved:][improved] [#2044]( https://github.com/NREL/OpenStudio/issues/2044 ), Reorder design alternatives in PAT for reporting
- ![Improved:][improved] [#2122]( https://github.com/NREL/OpenStudio/issues/2122 ), OS:ZoneHVAC:WaterToAirHeatPump accepts only boost optional for OA airflow
- ![Improved:][improved] [#2575]( https://github.com/NREL/OpenStudio/issues/2575 ), Enable autosizing of WaterToWaterHeatPumps

## OpenStudio v2.1.1
###### 2017-04-19 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.1.0...v2.1.1 )

- ![Fixed:][fixed] [#2414]( https://github.com/NREL/OpenStudio/issues/2414 ), HPWH wrapped condenser - don't set source nodes on tank
- ![Fixed:][fixed] [#2493]( https://github.com/NREL/OpenStudio/issues/2493 ), Internal NREL user specific Analysis Error related to Faraday
- ![Fixed:][fixed] [#2531]( https://github.com/NREL/OpenStudio/issues/2531 ), 2.0.5 03/20 pat results tab not working, not finding results measure
- ![Fixed:][fixed] [#2544]( https://github.com/NREL/OpenStudio/issues/2544 ), PAT arg default value is missing
- ![Fixed:][fixed] [#2546]( https://github.com/NREL/OpenStudio/issues/2546 ), Output Variable Table in Output Tab is Empty in PAT 2.1.0
- ![Fixed:][fixed] [#2549]( https://github.com/NREL/OpenStudio/issues/2549 ), Duplicate Measure and Option can create invalid workflow
- ![Fixed:][fixed] [#2556]( https://github.com/NREL/OpenStudio/issues/2556 ), Links to older SDK documentation
- ![Improved:][improved] [#2564]( https://github.com/NREL/OpenStudio/issues/2564 ), PAT 2.1.0 integer_sequence distribution for pivot variable

## OpenStudio v2.1.0
###### 2017-03-31 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.0.5...v2.1.0 )

- ![Fixed:][fixed] [#2432]( https://github.com/NREL/OpenStudio/issues/2432 ), HVAC library empty in OpenStudioApp 2.0.0 on mac
- ![Fixed:][fixed] [#2436]( https://github.com/NREL/OpenStudio/issues/2436 ), OS 2.0 on mac shows up a second, bouncing, icon when on Results tab
- ![Fixed:][fixed] [#2438]( https://github.com/NREL/OpenStudio/issues/2438 ), HTML report formatting messed up in OS App 2.0.0
- ![Fixed:][fixed] [#2441]( https://github.com/NREL/OpenStudio/issues/2441 ), OS2.0 can't add BCL key
- ![Fixed:][fixed] [#2515]( https://github.com/NREL/OpenStudio/issues/2515 ), update standards os_lib_schedules
- ![Fixed:][fixed] [#2519]( https://github.com/NREL/OpenStudio/issues/2519 ), Server Plots Not Working
- ![Fixed:][fixed] [#2525]( https://github.com/NREL/OpenStudio/issues/2525 ), ResultsViewer button in App starts RV but doesn't load data
- ![Fixed:][fixed] [#2530]( https://github.com/NREL/OpenStudio/issues/2530 ), Crash on go to results after saving new model
- ![Fixed:][fixed] [#2536]( https://github.com/NREL/OpenStudio/issues/2536 ), Can't view OpenStudio Results or some other Reporting measures in OS App
- ![Fixed:][fixed] [#2537]( https://github.com/NREL/OpenStudio/issues/2537 ), I may have missed 8.6 to 8.7 changes for tariff reporting

## OpenStudio v2.0.5
###### 2017-03-17 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.0.4...v2.0.5 )

- ![Fixed:][fixed] [#1684]( https://github.com/NREL/OpenStudio/issues/1684 ), Crash when editing string argument value after running a successful simulation.
- ![Fixed:][fixed] [#1915]( https://github.com/NREL/OpenStudio/issues/1915 ), OpenStudio App loses weather file
- ![Fixed:][fixed] [#1916]( https://github.com/NREL/OpenStudio/issues/1916 ), Open Studio does not provide an error message when running VRF systems without cooling or heating schedules.
- ![Fixed:][fixed] [#2001]( https://github.com/NREL/OpenStudio/issues/2001 ), Copying a measure produces invalid measure if "ended" is in description string (probably also issue in name and modeler description)
- ![Fixed:][fixed] [#2005]( https://github.com/NREL/OpenStudio/issues/2005 ), Run Simulation Tab Says "Hello World" When Using Apply Measure Now
- ![Fixed:][fixed] [#2007]( https://github.com/NREL/OpenStudio/issues/2007 ), alphabetize HVAC Systems list
- ![Fixed:][fixed] [#2034]( https://github.com/NREL/OpenStudio/issues/2034 ), App closes when Air Loop availability schedule is set to blank and the 'Control' button clicked.
- ![Fixed:][fixed] [#2037]( https://github.com/NREL/OpenStudio/issues/2037 ), HeatExchanger:FluidToFluid problems
- ![Fixed:][fixed] [#2043]( https://github.com/NREL/OpenStudio/issues/2043 ), CoilHeatingWaterToAirHeatPump:EquationFit forward translator sets hard sized rated water flow rate as air flow rate
- ![Fixed:][fixed] [#2046]( https://github.com/NREL/OpenStudio/issues/2046 ), HVAC templates should have a water cooled chiller and an air cooled chiller.
- ![Fixed:][fixed] [#2055]( https://github.com/NREL/OpenStudio/issues/2055 ), PAT datapoints with comma's in name fail to run
- ![Fixed:][fixed] [#2061]( https://github.com/NREL/OpenStudio/issues/2061 ), PAT 1.10.0 stops working if user changes User Defined Measure category
- ![Fixed:][fixed] [#2070]( https://github.com/NREL/OpenStudio/issues/2070 ), SetpointManager:Coldest translates to two identical energyplus objects
- ![Fixed:][fixed] [#2080]( https://github.com/NREL/OpenStudio/issues/2080 ), Deleting AirTerminal with reheat does not delete HW coil
- ![Fixed:][fixed] [#2081]( https://github.com/NREL/OpenStudio/issues/2081 ), Dropping Schedule
- ![Fixed:][fixed] [#2086]( https://github.com/NREL/OpenStudio/issues/2086 ), Mismatch between OpenStudio and EnergyPlus idd choice for PlantLoop Load Distribution Scheme
- ![Fixed:][fixed] [#2094]( https://github.com/NREL/OpenStudio/issues/2094 ), PAT Cloud not working in 1.10.0
- ![Fixed:][fixed] [#2097]( https://github.com/NREL/OpenStudio/issues/2097 ), Update OS to E+ 8.5
- ![Fixed:][fixed] [#2098]( https://github.com/NREL/OpenStudio/issues/2098 ), air loop diagram
- ![Fixed:][fixed] [#2149]( https://github.com/NREL/OpenStudio/issues/2149 ), Fix Date/Time Of Peak query in EnergyPlus 8.5
- ![Fixed:][fixed] [#2153]( https://github.com/NREL/OpenStudio/issues/2153 ), Fails to import IDF when CondenserLoop is part of IDF
- ![Fixed:][fixed] [#2155]( https://github.com/NREL/OpenStudio/issues/2155 ), 1.10.5 closes after apply measure now

## OpenStudio v2.0.4
###### 2017-03-06 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.0.3...v2.0.4 )

- ![Fixed:][fixed] [#643]( https://github.com/NREL/OpenStudio/issues/643 ), Radiance Forward Translator Bugs
- ![Fixed:][fixed] [#775]( https://github.com/NREL/OpenStudio/issues/775 ), Crash on delete air loop (on second loop I delete)
- ![Fixed:][fixed] [#1349]( https://github.com/NREL/OpenStudio/issues/1349 ), Measure Groups Rename on Reorder
- ![Fixed:][fixed] [#1473]( https://github.com/NREL/OpenStudio/issues/1473 ), Add Import / Load Library Feature for MEP Schedules
- ![Fixed:][fixed] [#1589]( https://github.com/NREL/OpenStudio/issues/1589 ), Launch OS button doesn't work for OSM files on network drives
- ![Fixed:][fixed] [#1730]( https://github.com/NREL/OpenStudio/issues/1730 ), ModelObjects not translated to E+
- ![Fixed:][fixed] [#1870]( https://github.com/NREL/OpenStudio/issues/1870 ), PAT 1.8.0 - An unhandled exception has occured - AnalysisDriver.cpp - Invalid Results
- ![Fixed:][fixed] [#1901]( https://github.com/NREL/OpenStudio/issues/1901 ), Fix Radiance translator's reporting behavior
- ![Fixed:][fixed] [#1969]( https://github.com/NREL/OpenStudio/issues/1969 ), Using "x2" in app on thermal zone removes thermostat from other zones that used it.
- ![Fixed:][fixed] [#2024]( https://github.com/NREL/OpenStudio/issues/2024 ), .osm files disappear from computer, possibly caused by uninstalling and reinstalling OpenStudio and Sketchup applications
- ![Fixed:][fixed] [#2030]( https://github.com/NREL/OpenStudio/issues/2030 ), PAT 1.10.0 run time depends on number of External File User Defined Measures
- ![Fixed:][fixed] [#2045]( https://github.com/NREL/OpenStudio/issues/2045 ), draw dual ducts
- ![Fixed:][fixed] [#2051]( https://github.com/NREL/OpenStudio/issues/2051 ), Mac packages fail at runtime due to geo lib
- ![Fixed:][fixed] [#2060]( https://github.com/NREL/OpenStudio/issues/2060 ), PAT 1.10.0 Run tab breaks if run cancelled
- ![Fixed:][fixed] [#2078]( https://github.com/NREL/OpenStudio/issues/2078 ), Fix ShadingControl solar setpoint and units handling
- ![Fixed:][fixed] [#2088]( https://github.com/NREL/OpenStudio/issues/2088 ), PAT XML export is not exporting user-defined names
- ![Fixed:][fixed] [#2089]( https://github.com/NREL/OpenStudio/issues/2089 ), Radiance measure blows up real good on large models
- ![Fixed:][fixed] [#2090]( https://github.com/NREL/OpenStudio/issues/2090 ), OS 1.10.0 Win 64 App does preserve inherited relatonships for constructions assigned to surfaces and sub surfaces
- ![Fixed:][fixed] [#2092]( https://github.com/NREL/OpenStudio/issues/2092 ), PAT deletes all datapoints after closing and reopening
- ![Fixed:][fixed] [#2101]( https://github.com/NREL/OpenStudio/issues/2101 ), Unit problem for “Add Daylight Sensor at the Center of Space” measure
- ![Fixed:][fixed] [#2113]( https://github.com/NREL/OpenStudio/issues/2113 ), Air wall construction missing
- ![Improved:][improved] [#1123]( https://github.com/NREL/OpenStudio/issues/1123 ), Better Radiance Preflight Testing

## OpenStudio v2.0.3
###### 2017-02-21 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.0.2...v2.0.3 )

- ![Fixed:][fixed] [#123]( https://github.com/NREL/OpenStudio/issues/123 ), Address changes made to shared thermostats with make unique (Bugzilla #722)
- ![Fixed:][fixed] [#1897]( https://github.com/NREL/OpenStudio/issues/1897 ), Biquadratic curve min/max value edits not retained
- ![Fixed:][fixed] [#1929]( https://github.com/NREL/OpenStudio/issues/1929 ), Radiance Measure flails when initial E+ run fails.
- ![Fixed:][fixed] [#1930]( https://github.com/NREL/OpenStudio/issues/1930 ), Support switchable glazing in Radiance Measure
- ![Fixed:][fixed] [#2012]( https://github.com/NREL/OpenStudio/issues/2012 ), Fix Glare Sensor Support
- ![Fixed:][fixed] [#2042]( https://github.com/NREL/OpenStudio/issues/2042 ), Assign thermostat to zones problem
- ![Fixed:][fixed] [#2049]( https://github.com/NREL/OpenStudio/issues/2049 ), fix ownership issues with zone controllers
- ![Fixed:][fixed] [#2056]( https://github.com/NREL/OpenStudio/issues/2056 ), Radiance Path Order
- ![Fixed:][fixed] [#2064]( https://github.com/NREL/OpenStudio/issues/2064 ), CBECC generated files fail to open in current OS (1.10.2)
- ![Fixed:][fixed] [#2065]( https://github.com/NREL/OpenStudio/issues/2065 ), Tariff Selection Time And Date Dependent measure has wrong season for winter demand charge
- ![Fixed:][fixed] [#2069]( https://github.com/NREL/OpenStudio/issues/2069 ), Fix delta temperature in design day table
- ![Fixed:][fixed] [#2071]( https://github.com/NREL/OpenStudio/issues/2071 ), Missing Object Types in Sketchup Plugin Inspector
- ![Fixed:][fixed] [#2072]( https://github.com/NREL/OpenStudio/issues/2072 ), Cannot create thermostats in Sketchup 2016 with OS 1.10
- ![Fixed:][fixed] [#2074]( https://github.com/NREL/OpenStudio/issues/2074 ), Deleting space orphans lifecycle cost objects that had been associated with the space
- ![Fixed:][fixed] [#2082]( https://github.com/NREL/OpenStudio/issues/2082 ), Autosize water heater ends in IDF without WaterHeater:Sizing object
- ![Fixed:][fixed] [#2083]( https://github.com/NREL/OpenStudio/issues/2083 ), Fix Radiance version sniff
- ![Improved:][improved] [#713]( https://github.com/NREL/OpenStudio/issues/713 ), Support multiple glare sensors per space
- ![Improved:][improved] [#1968]( https://github.com/NREL/OpenStudio/issues/1968 ), Add new oa method to ControllerMechanicalVentilation

## OpenStudio v2.0.2
###### 2017-01-25 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.0.1...v2.0.2 )


## OpenStudio v2.0.1
###### 2017-01-25 — [Diff]( https://github.com/NREL/OpenStudio/compare/v2.0.0...v2.0.1 )


## OpenStudio v2.0.0
###### 2016-12-21 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.14.0...v2.0.0 )


## OpenStudio v1.14.0
###### 2016-12-09 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.13.4...v1.14.0 )

- ![Fixed:][fixed] [#1519]( https://github.com/NREL/OpenStudio/issues/1519 ), People Def can have values for multiple unit types, other loads can't
- ![Fixed:][fixed] [#2178]( https://github.com/NREL/OpenStudio/issues/2178 ), Plenum Space Type - space.spaceType.is_initialized not functioning as expected
- ![Fixed:][fixed] [#2179]( https://github.com/NREL/OpenStudio/issues/2179 ), Plenum Space Type - Odd behavior when rename space type
- ![Fixed:][fixed] [#2299]( https://github.com/NREL/OpenStudio/issues/2299 ), Issue with Zone Sensible Cooling and Heating when heating or cooling only system
- ![Fixed:][fixed] [#2370]( https://github.com/NREL/OpenStudio/issues/2370 ), Error in intersectSurfaces
- ![Fixed:][fixed] [#2372]( https://github.com/NREL/OpenStudio/issues/2372 ), Conversion issues from 1.12.0 to 1.13.0
- ![Fixed:][fixed] [#2376]( https://github.com/NREL/OpenStudio/issues/2376 ), ScheduleRuleset.remove orphans winter and summer profiles

## OpenStudio v1.13.4
###### 2016-12-01 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.13.3...v1.13.4 )

- ![Fixed:][fixed] [#105]( https://github.com/NREL/OpenStudio/issues/105 ), Deleting Schedule Ruleset object leaves Schedule Day objects behind (Bugzilla #662)
- ![Fixed:][fixed] [#1459]( https://github.com/NREL/OpenStudio/issues/1459 ), LowTemperatureRadiant system input 'Radiant Surface Type' translates unexpected surfaces to SurfaceGroup
- ![Fixed:][fixed] [#1597]( https://github.com/NREL/OpenStudio/issues/1597 ), API allows you to enter invalid object names
- ![Fixed:][fixed] [#2195]( https://github.com/NREL/OpenStudio/issues/2195 ), Timeseries Not Found is being thrown on template reporting measure
- ![Fixed:][fixed] [#2216]( https://github.com/NREL/OpenStudio/issues/2216 ), Spectral Data Set name doesn't forward translate to IDF
- ![Fixed:][fixed] [#2217]( https://github.com/NREL/OpenStudio/issues/2217 ), BCLMeasure should force update if measure.xml missing class_name or other required fields
- ![Fixed:][fixed] [#2241]( https://github.com/NREL/OpenStudio/issues/2241 ), Bad data in energyPlusOutputRequests of reporting measure can break simulation
- ![Fixed:][fixed] [#2251]( https://github.com/NREL/OpenStudio/issues/2251 ), Cannot load openstudio-standards in SketchUp user scripts
- ![Fixed:][fixed] [#2304]( https://github.com/NREL/OpenStudio/issues/2304 ), Missing Objects in EnergyPlus Forward Translator
- ![Fixed:][fixed] [#2312]( https://github.com/NREL/OpenStudio/issues/2312 ), Cannot create custom window construction
- ![Fixed:][fixed] [#2328]( https://github.com/NREL/OpenStudio/issues/2328 ), Need backwards compatible shim for require 'openstudio/ruleset/ShowRunnerOutput'
- ![Fixed:][fixed] [#2331]( https://github.com/NREL/OpenStudio/issues/2331 ), Clean up and embed ruby files in openstudiocore/ruby/openstudio
- ![Fixed:][fixed] [#2332]( https://github.com/NREL/OpenStudio/issues/2332 ), Export zone loads to gbXML
- ![Fixed:][fixed] [#2354]( https://github.com/NREL/OpenStudio/issues/2354 ), ZoneCapacitanceMultiplierResearchSpecial not translated to E+
- ![Fixed:][fixed] [#2362]( https://github.com/NREL/OpenStudio/issues/2362 ), files missing
- ![Fixed:][fixed] [#2368]( https://github.com/NREL/OpenStudio/issues/2368 ), OpenStudio Measure test templates don't work in 2.x
- ![Fixed:][fixed] [#2369]( https://github.com/NREL/OpenStudio/issues/2369 ), OpenStudio 2.x not backwards compatible with 1.x measure tests
- ![Fixed:][fixed] [#2383]( https://github.com/NREL/OpenStudio/issues/2383 ), Refrigeration missing from monthly peak demand
- ![Fixed:][fixed] [#2393]( https://github.com/NREL/OpenStudio/issues/2393 ), Cash flow for fuels not populating because of E+ query change
- ![Fixed:][fixed] [#2394]( https://github.com/NREL/OpenStudio/issues/2394 ), Allow more zone hvac to be drag and dropped from the airloophvac view
- ![Fixed:][fixed] [#2396]( https://github.com/NREL/OpenStudio/issues/2396 ), GeneratorPhotovoltaic::simple constructor makes incomplete object

## OpenStudio v1.13.3
###### 2016-11-10 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.13.2...v1.13.3 )

- ![Fixed:][fixed] [#2388]( https://github.com/NREL/OpenStudio/pull/2388 ), 131770395 reporting measure 2x support

## OpenStudio v1.13.2
###### 2016-10-29 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.13.1...v1.13.2 )

- ![Added:][added] [#2382]( https://github.com/NREL/OpenStudio/pull/2382 ), Added support for EMS functionality.
- ![Added:][added] [#2230]( https://github.com/NREL/OpenStudio/pull/2230 ), Added OS:WaterHeater:HeatPump:WrappedCondenser and OS:Coil:WaterHeating:AirToWaterHeatPump:Wrapped objects.
- ![Added:][added] [#2375]( https://github.com/NREL/OpenStudio/pull/2375 ), Added Fuel Type to OS:OtherEquipment:Definition and OS:Coil:Heating:Gas.
- ![Fixed:][fixed] [#2230]( https://github.com/NREL/OpenStudio/pull/2230 ), 2091 wrapped hpwh
- ![Fixed:][fixed] [#2366]( https://github.com/NREL/OpenStudio/pull/2366 ), Set PreferredToolArchitecture = x64 when possible.
- ![Fixed:][fixed] [#2367]( https://github.com/NREL/OpenStudio/pull/2367 ), Updating standard report in pat_app dir
- ![Fixed:][fixed] [#2375]( https://github.com/NREL/OpenStudio/pull/2375 ), Adding Fuel Type to OtherEquipment and Coil:Heating:Gas
- ![Fixed:][fixed] [#2377]( https://github.com/NREL/OpenStudio/issues/2377 ), remove limit on number of fans on air loops
- ![Fixed:][fixed] [#2380]( https://github.com/NREL/OpenStudio/pull/2380 ), removed CAV and VAV fan limit from air loops
- ![Fixed:][fixed] [#2381]( https://github.com/NREL/OpenStudio/pull/2381 ), Pull request for EMS
- ![Fixed:][fixed] [#2382]( https://github.com/NREL/OpenStudio/pull/2382 ), EMS
- ![Fixed:][fixed] [#2386]( https://github.com/NREL/OpenStudio/pull/2386 ), Updates openstudio-standards to 0.1.8 to fix issue hit during PSD tra…

## OpenStudio v1.13.1
###### 2016-10-15 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.13.0...v1.13.1 )

- ![Added:][added] [#2363]( https://github.com/NREL/OpenStudio/pull/2363 ), Add a BuildingUnit object that is optionally referenced by a space such that it can group together spaces into a unit such as an apartment in a multifamily building
- ![Fixed:][fixed] [#2247]( https://github.com/NREL/OpenStudio/issues/2247 ), Update class generator
- ![Fixed:][fixed] [#2360]( https://github.com/NREL/OpenStudio/issues/2360 ), Incorrect Zone Volume in Outdoor Air - OpenStudio report
- ![Fixed:][fixed] [#2371]( https://github.com/NREL/OpenStudio/issues/2371 ), OpenStudio 2.0: Importing C# library in IronPython threw 'OpenStudio.OpenStudioModelCorePINVOKE'
- ![Fixed:][fixed] [#2374]( https://github.com/NREL/OpenStudio/pull/2374 ), Make OpenStudio 2.X more backwards compatible with 1.X measure tests
- ![Fixed:][fixed] [#2378]( https://github.com/NREL/OpenStudio/pull/2378 ), Updates openstudio-standards to 0.1.7

## OpenStudio v1.13.0
###### 2016-10-01 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.12.6...v1.13.0 )

- ![Fixed:][fixed] [#2308]( https://github.com/NREL/OpenStudio/issues/2308 ), OS App crashes when switching to Measures tab with specific model
- ![Fixed:][fixed] [#2355]( https://github.com/NREL/OpenStudio/pull/2355 ), Updates openstudio-standards gem to 0.1.5, which supports baseline au…
- ![Fixed:][fixed] [#2356]( https://github.com/NREL/OpenStudio/pull/2356 ), Os 2 0 fix static build
- ![Fixed:][fixed] [#2359]( https://github.com/NREL/OpenStudio/pull/2359 ), License update
- ![Fixed:][fixed] [#2361]( https://github.com/NREL/OpenStudio/pull/2361 ), Updates openstudio-standards to 0.1.6

## OpenStudio v1.12.6
###### 2016-09-21 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.12.5...v1.12.6 )

- ![Fixed:][fixed] [#2353]( https://github.com/NREL/OpenStudio/pull/2353 ), Os 2 0 develop linux

## OpenStudio v1.12.5
###### 2016-09-09 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.12.4...v1.12.5 )

- ![Fixed:][fixed] [#2293]( https://github.com/NREL/OpenStudio/issues/2293 ), User model crashes when delete Zone

## OpenStudio v1.12.4
###### 2016-08-18 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.12.3...v1.12.4 )

## OpenStudio v1.12.3
###### 2016-08-10 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.12.2...v1.12.3 )

- ![Fixed:][fixed] [#561]( https://github.com/NREL/OpenStudio/issues/561 ), Adjacent stacked spaces in same zones may result in incorrect area and likely loads as well.
- ![Fixed:][fixed] [#1361]( https://github.com/NREL/OpenStudio/issues/1361 ), gbXML does not import WindowType elements
- ![Fixed:][fixed] [#2298]( https://github.com/NREL/OpenStudio/issues/2298 ), gbxml import makes bad OS:Schedule:Week
- ![Fixed:][fixed] [#2303]( https://github.com/NREL/OpenStudio/issues/2303 ), gbXML import/export should properly use names and ids
- ![Fixed:][fixed] [#2310]( https://github.com/NREL/OpenStudio/issues/2310 ), Fenestration related "impedance mismatch" between OS and JEPlus
- ![Fixed:][fixed] [#2313]( https://github.com/NREL/OpenStudio/issues/2313 ), Coil:Cooling:DX:TwoStageWithHumidityControl not working
- ![Fixed:][fixed] [#2314]( https://github.com/NREL/OpenStudio/issues/2314 ), OS 1.12.1 to 1.12.2 Conversion Error
- ![Fixed:][fixed] [#2316]( https://github.com/NREL/OpenStudio/issues/2316 ), Year Schedules appears twice in Schedules tab Library pane
- ![Fixed:][fixed] [#2320]( https://github.com/NREL/OpenStudio/issues/2320 ), PAT select measures 

## OpenStudio v1.12.2
###### 2016-07-25 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.12.1...v1.12.2 )

Closed Issues: 12 (#93, #94, #273, #275, #398, #399, #1362, #1925, #1991, #2172, #2278, #2306)
- ![Fixed:][fixed] [#93]( https://github.com/NREL/OpenStudio/issues/93 ), gbXML translator should convert floor to roof and vice versa when reverses geometry (Bugzilla #634)
- ![Fixed:][fixed] [#94]( https://github.com/NREL/OpenStudio/issues/94 ), gbXML translator should provide a value for material roughness if not in the file (Bugzilla #635)
- ![Fixed:][fixed] [#273]( https://github.com/NREL/OpenStudio/issues/273 ), Error working on gbXML file after I imported it (Bugzilla #1053)
- ![Fixed:][fixed] [#275]( https://github.com/NREL/OpenStudio/issues/275 ), An xml with a zone containing multiple spaces is split into multiple zones on import. (Bugzilla #1056)
- ![Fixed:][fixed] [#398]( https://github.com/NREL/OpenStudio/issues/398 ), Non-mirror constructions in export of std 140 model to gbXML
- ![Fixed:][fixed] [#399]( https://github.com/NREL/OpenStudio/issues/399 ), Unable to import gbXML export of hospital reference building into Bentley
- ![Fixed:][fixed] [#1362]( https://github.com/NREL/OpenStudio/issues/1362 ), OpenStudio GUI does not show ScheduleYear objects where it should
- ![Fixed:][fixed] [#1925]( https://github.com/NREL/OpenStudio/issues/1925 ), GBXML Export Uses wrong Coordinate Geometry
- ![Fixed:][fixed] [#1991]( https://github.com/NREL/OpenStudio/issues/1991 ), Shading surfaces not exported to gbXML
- ![Fixed:][fixed] [#2278]( https://github.com/NREL/OpenStudio/issues/2278 ), UnitarySystemPerformance:Multispeed is not cloning correctly
- ![Fixed:][fixed] [#2306]( https://github.com/NREL/OpenStudio/issues/2306 ), Zone overview bug in open studio results
- ![Improved:][improved] [#2172]( https://github.com/NREL/OpenStudio/issues/2172 ), Translate all coils supported by the IDD in AirLoopHVACUnitarySystem

## OpenStudio v1.12.1
###### 2016-07-07 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.12.0...v1.12.1 )

- ![Fixed:][fixed] [#2248]( https://github.com/NREL/OpenStudio/pull/2248 ), Add Generator:MicroTurbine to OpenStudio
- ![Fixed:][fixed] [#2263]( https://github.com/NREL/OpenStudio/pull/2263 ), Add Meter:Custom and Meter:CustomDecrement to the SDK
- ![Fixed:][fixed] [#2276]( https://github.com/NREL/OpenStudio/pull/2276 ), Add ElectricLoadCenter:Storage:Simple and expose rest of ELC:Distribution fields
- ![Fixed:][fixed] [#2282]( https://github.com/NREL/OpenStudio/pull/2282 ), Jmarrec merge
- ![Fixed:][fixed] [#2287]( https://github.com/NREL/OpenStudio/pull/2287 ), Spectral dataset

## OpenStudio v1.12.0
###### 2016-07-01 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.11.6...v1.12.0 )

## OpenStudio v1.11.6
###### 2016-06-24 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.11.5...v1.11.6 )

- ![Fixed:][fixed] [#2279]( https://github.com/NREL/OpenStudio/issues/2279 ), 1.11.5 - Surface class shadingSurfaceGroup method doesn't work

## OpenStudio v1.11.5
###### 2016-06-13 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.11.4...v1.11.5 )

- ![Improved:][improved] [#2172]( https://github.com/NREL/OpenStudio/issues/2172 ), Translate all coils supported by the IDD in AirLoopHVACUnitarySystem
- ![Improved:][improved] [#2213]( https://github.com/NREL/OpenStudio/issues/2213 ), Enable heat recovery for ChillerElectricEIR
- ![Fixed:][fixed] [#247]( https://github.com/NREL/OpenStudio/issues/247 ), Opening OSM file in SketchUp Plugin changes Daylight Savings values (Bugzilla #1015)
- ![Fixed:][fixed] [#981]( https://github.com/NREL/OpenStudio/issues/981 ), Daylight savings time in user file is overwritten when opening model in app
- ![Fixed:][fixed] [#2099]( https://github.com/NREL/OpenStudio/issues/2099 ), TimeStep of 6 per hour in new model isn't respected when simulation is run
- ![Fixed:][fixed] [#2177]( https://github.com/NREL/OpenStudio/issues/2177 ), WaterHeaterMixed - Ambient Temperature Indicator : Outdoors does not work
- ![Fixed:][fixed] [#2184]( https://github.com/NREL/OpenStudio/issues/2184 ), unit ventilator object is incomplete in 1.11.0
- ![Fixed:][fixed] [#2185]( https://github.com/NREL/OpenStudio/issues/2185 ), Daylight savings time allways get the American option
- ![Fixed:][fixed] [#2204]( https://github.com/NREL/OpenStudio/issues/2204 ), Update forward translator so it doesn't translate orphan surfaces
- ![Fixed:][fixed] [#2221]( https://github.com/NREL/OpenStudio/issues/2221 ), Temp dir is not deleted
- ![Fixed:][fixed] [#2237]( https://github.com/NREL/OpenStudio/issues/2237 ), Site:GroundTemperature:Deep not forward translating
- ![Fixed:][fixed] [#2240]( https://github.com/NREL/OpenStudio/issues/2240 ), Unit conversion error - in^2 to cm^2
- ![Fixed:][fixed] [#2249]( https://github.com/NREL/OpenStudio/issues/2249 ), Handle "Baseline" checks on PAT XML export
- ![Fixed:][fixed] [#2265]( https://github.com/NREL/OpenStudio/issues/2265 ), OpenStudio::Workspace constructor creates an OS:Version object that makes reverse translation fail
- ![Fixed:][fixed] [#2268]( https://github.com/NREL/OpenStudio/issues/2268 ), WaterHeaterMixed is writing a corrupt oa node on translate to idf
- ![Fixed:][fixed] [#2269]( https://github.com/NREL/OpenStudio/issues/2269 ), 1.11.4 - Coil:Cooling:DX:TwoStageWithHumidityControlMode causes sim to fail

## OpenStudio v1.11.4
###### 2016-05-27 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.11.3...v1.11.4 )

- ![Fixed:][fixed] [#2126]( https://github.com/NREL/OpenStudio/issues/2126 ), 1.10.4 EnergyPlus crash with Output:Table:SummaryReports measure
- ![Fixed:][fixed] [#2129]( https://github.com/NREL/OpenStudio/issues/2129 ), Additional Radiance Measure Server Fixes
- ![Fixed:][fixed] [#2188]( https://github.com/NREL/OpenStudio/issues/2188 ), change 90.1 construction sets to match the PRM
- ![Fixed:][fixed] [#2223]( https://github.com/NREL/OpenStudio/issues/2223 ), Controller Mechanical Ventilation Availability Schedule
- ![Fixed:][fixed] [#2226]( https://github.com/NREL/OpenStudio/issues/2226 ), AirConditionerVariableRefrigerantFlow::clone does not include curves
- ![Fixed:][fixed] [#2231]( https://github.com/NREL/OpenStudio/issues/2231 ), PTHP does not allow water coils
- ![Fixed:][fixed] [#2233]( https://github.com/NREL/OpenStudio/issues/2233 ), Enhance ZoneHVACBaseboardConvectiveWater
- ![Fixed:][fixed] [#2235]( https://github.com/NREL/OpenStudio/issues/2235 ), OtherEquipment should allow negative schedule values to -1.
- ![Fixed:][fixed] [#2238]( https://github.com/NREL/OpenStudio/issues/2238 ), Remove custom populateValidityReport implementations from ModelObject classes
- ![Fixed:][fixed] [#2242]( https://github.com/NREL/OpenStudio/issues/2242 ), CBECC Translate OA equipment including HX
- ![Fixed:][fixed] [#2243]( https://github.com/NREL/OpenStudio/issues/2243 ), OpenStudio crashes when going to "Measures" tab on two user models
- ![Fixed:][fixed] [#2244]( https://github.com/NREL/OpenStudio/issues/2244 ), OA Mixer should be at the end of the oa system equipment list

## OpenStudio v1.11.3
###### 2016-05-17 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.11.2...v1.11.3 )

- ![Fixed:][fixed] [#2225]( https://github.com/NREL/OpenStudio/issues/2225 ), Daylighting Controls Measure: OS:Daylighting:Control objects do not forward translate in certain cases

## OpenStudio v1.11.2
###### 2016-05-02 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.11.1...v1.11.2 )

- ![Fixed:][fixed] [#1627]( https://github.com/NREL/OpenStudio/issues/1627 ), HVAC Tab - Low Temp Radient components come in without any names
- ![Fixed:][fixed] [#2203]( https://github.com/NREL/OpenStudio/issues/2203 ), Ruby inside of Mac installer won't load openstudio bindings
- ![Fixed:][fixed] [#2207]( https://github.com/NREL/OpenStudio/issues/2207 ), Fix exterior lights on standard report
- ![Fixed:][fixed] [#2208]( https://github.com/NREL/OpenStudio/issues/2208 ), Fix Variable Speed Pump reporting in standard report

## OpenStudio v1.11.1
###### 2016-04-21 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.11.0...v1.11.1 )

- ![Fixed:][fixed] [#1359]( https://github.com/NREL/OpenStudio/issues/1359 ), EnergyPlus does not update standard output during run
- ![Fixed:][fixed] [#2181]( https://github.com/NREL/OpenStudio/issues/2181 ), Add "high" accuracy settings option to Radiance Measure
- ![Fixed:][fixed] [#2186]( https://github.com/NREL/OpenStudio/issues/2186 ), SketchUp does not see plugin signature as valid
- ![Fixed:][fixed] [#2191]( https://github.com/NREL/OpenStudio/issues/2191 ), MultiSpeed Coil Inlet/Outlet Nodes Not Being Connected Properly

## OpenStudio v1.11.0
###### 2016-03-28 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.10.6...v1.11.0 )

- ![Fixed:][fixed] [#2169]( https://github.com/NREL/OpenStudio/pull/2169 ), Adding comments only
- ![Fixed:][fixed] [#2171]( https://github.com/NREL/OpenStudio/pull/2171 ), Unit test fixes

## OpenStudio v1.10.6
###### 2016-03-22 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.10.5...v1.10.6 )

- ![Fixed:][fixed] [#1916]( https://github.com/NREL/OpenStudio/issues/1916 ), Open Studio does not provide an error message when running VRF systems without cooling or heating schedules.
- ![Fixed:][fixed] [#2005]( https://github.com/NREL/OpenStudio/issues/2005 ), Run Simulation Tab Says "Hello World" When Using Apply Measure Now
- ![Fixed:][fixed] [#2007]( https://github.com/NREL/OpenStudio/issues/2007 ), alphabetize HVAC Systems list
- ![Fixed:][fixed] [#2034]( https://github.com/NREL/OpenStudio/issues/2034 ), App closes when Air Loop availability schedule is set to blank and the 'Control' button clicked.
- ![Fixed:][fixed] [#2037]( https://github.com/NREL/OpenStudio/issues/2037 ), HeatExchanger:FluidToFluid problems
- ![Fixed:][fixed] [#2043]( https://github.com/NREL/OpenStudio/issues/2043 ), CoilHeatingWaterToAirHeatPump:EquationFit forward translator sets hard sized rated water flow rate as air flow rate
- ![Fixed:][fixed] [#2046]( https://github.com/NREL/OpenStudio/issues/2046 ), HVAC templates should have a water cooled chiller and an air cooled chiller.
- ![Fixed:][fixed] [#2055]( https://github.com/NREL/OpenStudio/issues/2055 ), PAT datapoints with comma's in name fail to run
- ![Fixed:][fixed] [#2080]( https://github.com/NREL/OpenStudio/issues/2080 ), Deleting AirTerminal with reheat does not delete HW coil
- ![Fixed:][fixed] [#2081]( https://github.com/NREL/OpenStudio/issues/2081 ), Dropping Schedule
- ![Fixed:][fixed] [#2086]( https://github.com/NREL/OpenStudio/issues/2086 ), Mismatch between OpenStudio and EnergyPlus idd choice for PlantLoop Load Distribution Scheme
- ![Fixed:][fixed] [#2097]( https://github.com/NREL/OpenStudio/issues/2097 ), Update OS to E+ 8.5
- ![Fixed:][fixed] [#2098]( https://github.com/NREL/OpenStudio/issues/2098 ), air loop diagram
- ![Fixed:][fixed] [#2149]( https://github.com/NREL/OpenStudio/issues/2149 ), Fix Date/Time Of Peak query in EnergyPlus 8.5
- ![Fixed:][fixed] [#2153]( https://github.com/NREL/OpenStudio/issues/2153 ), Fails to import IDF when CondenserLoop is part of IDF
- ![Fixed:][fixed] [#2155]( https://github.com/NREL/OpenStudio/issues/2155 ), 1.10.5 closes after apply measure now

## OpenStudio v1.10.5
###### 2016-03-08 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.10.4...v1.10.5 )

- ![Improved:][improved] [#1123]( https://github.com/NREL/OpenStudio/issues/1123 ), Better Radiance Preflight Testing
- ![Fixed:][fixed] [#643]( https://github.com/NREL/OpenStudio/issues/643 ), Radiance Forward Translator Bugs
- ![Fixed:][fixed] [#775]( https://github.com/NREL/OpenStudio/issues/775 ), Crash on delete air loop (on second loop I delete)
- ![Fixed:][fixed] [#1349]( https://github.com/NREL/OpenStudio/issues/1349 ), Measure Groups Rename on Reorder
- ![Fixed:][fixed] [#1473]( https://github.com/NREL/OpenStudio/issues/1473 ), Add Import / Load Library Feature for MEP Schedules
- ![Fixed:][fixed] [#1589]( https://github.com/NREL/OpenStudio/issues/1589 ), Launch OS button doesn't work for OSM files on network drives
- ![Fixed:][fixed] [#1684]( https://github.com/NREL/OpenStudio/issues/1684 ), Crash when editing string argument value after running a successful simulation.
- ![Fixed:][fixed] [#1730]( https://github.com/NREL/OpenStudio/issues/1730 ), ModelObjects not translated to E+
- ![Fixed:][fixed] [#1870]( https://github.com/NREL/OpenStudio/issues/1870 ), PAT 1.8.0 - An unhandled exception has occurred - AnalysisDriver.cpp - Invalid Results
- ![Fixed:][fixed] [#1901]( https://github.com/NREL/OpenStudio/issues/1901 ), Fix Radiance translator's reporting behavior
- ![Fixed:][fixed] [#1915]( https://github.com/NREL/OpenStudio/issues/1915 ), OpenStudio App loses weather file
- ![Fixed:][fixed] [#1969]( https://github.com/NREL/OpenStudio/issues/1969 ), Using "x2" in app on thermal zone removes thermostat from other zones that used it.
- ![Fixed:][fixed] [#1999]( https://github.com/NREL/OpenStudio/issues/1999 ), Model does not display in SketchUp
- ![Fixed:][fixed] [#2001]( https://github.com/NREL/OpenStudio/issues/2001 ), Copying a measure produces invalid measure if "ended" is in description string (probably also issue in name and modeler description)
- ![Fixed:][fixed] [#2024]( https://github.com/NREL/OpenStudio/issues/2024 ), .osm files disappear from computer, possibly caused by uninstalling and reinstalling OpenStudio and Sketchup applications
- ![Fixed:][fixed] [#2030]( https://github.com/NREL/OpenStudio/issues/2030 ), PAT 1.10.0 run time depends on number of External File User Defined Measures
- ![Fixed:][fixed] [#2045]( https://github.com/NREL/OpenStudio/issues/2045 ), draw dual ducts
- ![Fixed:][fixed] [#2051]( https://github.com/NREL/OpenStudio/issues/2051 ), Mac packages fail at runtime due to geo lib
- ![Fixed:][fixed] [#2060]( https://github.com/NREL/OpenStudio/issues/2060 ), PAT 1.10.0 Run tab breaks if run cancelled
- ![Fixed:][fixed] [#2061]( https://github.com/NREL/OpenStudio/issues/2061 ), PAT 1.10.0 stops working if user changes User Defined Measure category
- ![Fixed:][fixed] [#2070]( https://github.com/NREL/OpenStudio/issues/2070 ), SetpointManager:Coldest translates to two identical energyplus objects
- ![Fixed:][fixed] [#2078]( https://github.com/NREL/OpenStudio/issues/2078 ), Fix ShadingControl solar setpoint and units handling
- ![Fixed:][fixed] [#2088]( https://github.com/NREL/OpenStudio/issues/2088 ), PAT XML export is not exporting user-defined names
- ![Fixed:][fixed] [#2089]( https://github.com/NREL/OpenStudio/issues/2089 ), Radiance measure blows up real good on large models
- ![Fixed:][fixed] [#2090]( https://github.com/NREL/OpenStudio/issues/2090 ), OS 1.10.0 Win 64 App does preserve inherited relationships for constructions assigned to surfaces and sub surfaces
- ![Fixed:][fixed] [#2092]( https://github.com/NREL/OpenStudio/issues/2092 ), PAT deletes all datapoints after closing and reopening
- ![Fixed:][fixed] [#2094]( https://github.com/NREL/OpenStudio/issues/2094 ), PAT Cloud not working in 1.10.0
- ![Fixed:][fixed] [#2101]( https://github.com/NREL/OpenStudio/issues/2101 ), Unit problem for “Add Daylight Sensor at the Center of Space” measure
- ![Fixed:][fixed] [#2113]( https://github.com/NREL/OpenStudio/issues/2113 ), Air wall construction missing

## OpenStudio v1.10.4
###### 2016-02-21 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.10.3...v1.10.4 )

- ![Fixed:][fixed] [#1929]( https://github.com/NREL/OpenStudio/issues/1929 ), Radiance Measure flails when initial E+ run fails.
- ![Fixed:][fixed] [#1930]( https://github.com/NREL/OpenStudio/issues/1930 ), Support switchable glazing in Radiance Measure
- ![Fixed:][fixed] [#2064]( https://github.com/NREL/OpenStudio/issues/2064 ), CBECC generated files fail to open in current OS (1.10.2)
- ![Fixed:][fixed] [#2065]( https://github.com/NREL/OpenStudio/issues/2065 ), Tariff Selection Time And Date Dependent measure has wrong season for winter demand charge
- ![Fixed:][fixed] [#2069]( https://github.com/NREL/OpenStudio/issues/2069 ), Fix delta temperature in design day table
- ![Fixed:][fixed] [#2071]( https://github.com/NREL/OpenStudio/issues/2071 ), Missing Object Types in Sketchup Plugin Inspector
- ![Fixed:][fixed] [#2072]( https://github.com/NREL/OpenStudio/issues/2072 ), Cannot create thermostats in Sketchup 2016 with OS 1.10
- ![Fixed:][fixed] [#2074]( https://github.com/NREL/OpenStudio/issues/2074 ), Deleting space orphans lifecycle cost objects that had been associated with the space
- ![Fixed:][fixed] [#2082]( https://github.com/NREL/OpenStudio/issues/2082 ), Autosize water heater ends in IDF without WaterHeater:Sizing object
- ![Fixed:][fixed] [#2083]( https://github.com/NREL/OpenStudio/issues/2083 ), Fix Radiance version sniff

## OpenStudio v1.10.3
###### 2016-01-21 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.10.2...v1.10.3 )

- ![Improved:][improved] [#713]( https://github.com/NREL/OpenStudio/issues/713 ), Support multiple glare sensors per space
- ![Improved:][improved] [#985]( https://github.com/NREL/OpenStudio/issues/985 ), Allow shared thermostats
- ![Improved:][improved] [#1968]( https://github.com/NREL/OpenStudio/issues/1968 ), Add new oa method to ControllerMechanicalVentilation
- ![Fixed:][fixed] [#123]( https://github.com/NREL/OpenStudio/issues/123 ), Address changes made to shared thermostats with make unique (Bugzilla #722)
- ![Fixed:][fixed] [#1897]( https://github.com/NREL/OpenStudio/issues/1897 ), Biquadratic curve min/max value edits not retained
- ![Fixed:][fixed] [#2012]( https://github.com/NREL/OpenStudio/issues/2012 ), Fix Glare Sensor Support
- ![Fixed:][fixed] [#2026]( https://github.com/NREL/OpenStudio/issues/2026 ), PAT is breaking 1.9.0 files when opening with 1.10
- ![Fixed:][fixed] [#2040]( https://github.com/NREL/OpenStudio/issues/2040 ), New Geo lib breaks OpenStudio-Resources build
- ![Fixed:][fixed] [#2042]( https://github.com/NREL/OpenStudio/issues/2042 ), Assign thermostat to zones problem
- ![Fixed:][fixed] [#2049]( https://github.com/NREL/OpenStudio/issues/2049 ), fix ownership issues with zone controllers
- ![Fixed:][fixed] [#2056]( https://github.com/NREL/OpenStudio/issues/2056 ), Radiance Path Order

## OpenStudio v1.10.2
###### 2016-01-21 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.10.1...v1.10.2 )

- ![Fixed:][fixed] [#1713]( https://github.com/NREL/OpenStudio/issues/1713 ), open studio not working on windows 8.1
- ![Fixed:][fixed] [#1947]( https://github.com/NREL/OpenStudio/issues/1947 ), Expose all fields of the Sizing:Zone object
- ![Fixed:][fixed] [#2028]( https://github.com/NREL/OpenStudio/issues/2028 ), Forward Translation error when multiple spaces of same space type in a zone when load type is design load

## OpenStudio v1.10.1
###### 2016-01-08 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.10.0...v1.10.1 )

- ![Fixed:][fixed] [#1959]( https://github.com/NREL/OpenStudio/issues/1959 ), Building of OpenStudio on Mac doesn't find EnergyPlus or Radiance
- ![Fixed:][fixed] [#2002]( https://github.com/NREL/OpenStudio/issues/2002 ), Removing arguments from a measure leaves orphan arguments in XML
- ![Fixed:][fixed] [#2003]( https://github.com/NREL/OpenStudio/issues/2003 ), GUI Configuration Failure - OS 1.9.0 and 1.9.5 for Load Profile Plant Object 
- ![Fixed:][fixed] [#2004]( https://github.com/NREL/OpenStudio/issues/2004 ), 64 bit C# bindings have a 32 bit dll
- ![Fixed:][fixed] [#2014]( https://github.com/NREL/OpenStudio/issues/2014 ), OpenStudio application 1.10

## OpenStudio v1.10.0
###### 2015-12-11 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.9.5...v1.10.0 )

- ![Fixed:][fixed] [#1526]( https://github.com/NREL/OpenStudio/issues/1526 ), Floor/Ceiling Constructions creating issues with construction.getNetArea method
- ![Fixed:][fixed] [#1608]( https://github.com/NREL/OpenStudio/issues/1608 ), Thermal Zone tab crash when editing zone equipment
- ![Fixed:][fixed] [#1831]( https://github.com/NREL/OpenStudio/issues/1831 ), Select All in GridView should only select all rows/sub-rows that are currently visible (i.e. unfiltered)
- ![Fixed:][fixed] [#1881]( https://github.com/NREL/OpenStudio/issues/1881 ), Default constructor for PlantEquipmentOperation:OutdoorDryBulb has temp limits reversed
- ![Fixed:][fixed] [#1951]( https://github.com/NREL/OpenStudio/issues/1951 ), PAT 1.9.4 crashes when creating a new project or opening an existing one
- ![Fixed:][fixed] [#1963]( https://github.com/NREL/OpenStudio/issues/1963 ), Render by surface type leaves some surfaces transparent
- ![Fixed:][fixed] [#1971]( https://github.com/NREL/OpenStudio/issues/1971 ), OS:LoadProfile:Plant Object not functioning properly 
- ![Fixed:][fixed] [#1977]( https://github.com/NREL/OpenStudio/issues/1977 ), Setpoint managers are not allowed on plant demand side
- ![Fixed:][fixed] [#1983]( https://github.com/NREL/OpenStudio/issues/1983 ), AddOutputVariable measure should support the "annual" reporting frequency
- ![Fixed:][fixed] [#1984]( https://github.com/NREL/OpenStudio/issues/1984 ), View_data Measure Doesn't Rotate Shading Surfaces when North Axis is Rotated
- ![Fixed:][fixed] [#1985]( https://github.com/NREL/OpenStudio/issues/1985 ), Control For Outdoor Air Field Not Initialized In AirTerminal Causes Crash
- ![Fixed:][fixed] [#1986]( https://github.com/NREL/OpenStudio/issues/1986 ), Link to measure writing guide in new measure is wrong
- ![Fixed:][fixed] [#1995]( https://github.com/NREL/OpenStudio/issues/1995 ), Induction terminals (still) not disconnecting cleanly

## OpenStudio v1.9.5
###### 2015-11-25 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.9.4...v1.9.5 )

- ![Improved:][improved] [#1394]( https://github.com/NREL/OpenStudio/issues/1394 ), OS -> SDD Translate ZoneHVAC 
- ![Fixed:][fixed] [#1905]( https://github.com/NREL/OpenStudio/issues/1905 ), Crash when clicking on Solar Flat Plat Collector in Plant Loop
- ![Fixed:][fixed] [#1919]( https://github.com/NREL/OpenStudio/issues/1919 ), ZoneVentilation:DesignFlowRate won't work on zones without any zone equipment.
- ![Fixed:][fixed] [#1924]( https://github.com/NREL/OpenStudio/issues/1924 ), SketchUp Plugin doors default to "glass door" vs. door
- ![Fixed:][fixed] [#1936]( https://github.com/NREL/OpenStudio/issues/1936 ), Controller:WaterCoil node references not cleaned up on airloop edit
- ![Fixed:][fixed] [#1948]( https://github.com/NREL/OpenStudio/issues/1948 ), OS 1.9 loses space loads when converting v1.8 model
- ![Fixed:][fixed] [#1949]( https://github.com/NREL/OpenStudio/issues/1949 ), PAT and ResultsViewer 1.9.3 won't launch
- ![Fixed:][fixed] [#1954]( https://github.com/NREL/OpenStudio/issues/1954 ), Crash before run simulation
- ![Fixed:][fixed] [#1956]( https://github.com/NREL/OpenStudio/issues/1956 ), OS App 1.9.4 crashes in Apply Now when changing selected measure

## OpenStudio v1.9.4
###### 2015-11-12 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.9.3...v1.9.4 )

- ![Improved:][improved] [#1911]( https://github.com/NREL/OpenStudio/issues/1911 ), Add support for E+ 8.4
- ![Improved:][improved] [#1921]( https://github.com/NREL/OpenStudio/issues/1921 ), Add water to water heat pump
- ![Fixed:][fixed] [#1460]( https://github.com/NREL/OpenStudio/issues/1460 ), Inherited Default Construction and Schedule Sets Not Displayed in Facility Tab
- ![Fixed:][fixed] [#1598]( https://github.com/NREL/OpenStudio/issues/1598 ), dynamic generated argument choice values in measure.xml
- ![Fixed:][fixed] [#1812]( https://github.com/NREL/OpenStudio/issues/1812 ), Build Win Version Ruby - Product extension is set to .so
- ![Fixed:][fixed] [#1847]( https://github.com/NREL/OpenStudio/issues/1847 ), Space Tab - Shading sub tab is missing Shading Surface Name column
- ![Fixed:][fixed] [#1861]( https://github.com/NREL/OpenStudio/issues/1861 ), Default BoilerHotWater::BoilerFlowMode
- ![Fixed:][fixed] [#1892]( https://github.com/NREL/OpenStudio/issues/1892 ), Application says "Drop Zone" instead of "Drop Space Type" in the space type tab
- ![Fixed:][fixed] [#1893]( https://github.com/NREL/OpenStudio/issues/1893 ), 1.9.0 Space tab: do not see inherited properties
- ![Fixed:][fixed] [#1896]( https://github.com/NREL/OpenStudio/issues/1896 ), constructionId for SubSurface translated to Surface
- ![Fixed:][fixed] [#1899]( https://github.com/NREL/OpenStudio/issues/1899 ), Crash when viewing plant loop on user model
- ![Fixed:][fixed] [#1928]( https://github.com/NREL/OpenStudio/issues/1928 ), Unable to assign thermalZone to Inverter
- ![Fixed:][fixed] [#1932]( https://github.com/NREL/OpenStudio/issues/1932 ), PAT XML Export is missing cash flow results in 1.9

## OpenStudio v1.9.3
###### 2015-11-02 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.9.2...v1.9.3 )

- ![Improved:][improved] [#1907]( https://github.com/NREL/OpenStudio/issues/1907 ), Enable Design OA spec on terminal
- ![Fixed:][fixed] [#1734]( https://github.com/NREL/OpenStudio/issues/1734 ), Weather file location 1.8.1
- ![Fixed:][fixed] [#1900]( https://github.com/NREL/OpenStudio/issues/1900 ), User has to specify at least one measure on alternate model in PAT
- ![Fixed:][fixed] [#1902]( https://github.com/NREL/OpenStudio/issues/1902 ), LayeredConstruction::visibleTransmittance doesn't work on multiple layer constructions
- ![Fixed:][fixed] [#1917]( https://github.com/NREL/OpenStudio/issues/1917 ), Bug Report - Thermostat
- ![Fixed:][fixed] [#1923]( https://github.com/NREL/OpenStudio/issues/1923 ), ElectricalLoadCenter:Generators is missing

## OpenStudio v1.9.2
###### 2015-10-19 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.9.1...v1.9.2 )

- ![Improved:][improved] [#1303]( https://github.com/NREL/OpenStudio/issues/1303 ), Can't make choice arguments from model for E+ measures
- ![Fixed:][fixed] [#298]( https://github.com/NREL/OpenStudio/issues/298 ), weather (epw) and ddy file path issues (Bugzilla #1163)
- ![Fixed:][fixed] [#1549]( https://github.com/NREL/OpenStudio/issues/1549 ), Coordinate fine/coarse Sky Subdivision Params in idd
- ![Fixed:][fixed] [#1555]( https://github.com/NREL/OpenStudio/issues/1555 ), Cannot set or view site name in app
- ![removed:][removed] [#1907]( https://github.com/NREL/OpenStudio/issues/1907 ), Removed method AirTerminalSingleDuctVAVNoReheat::designSpecificationOutdoorAir in favor of a more user friendly API.

## OpenStudio v1.9.1
###### 2015-10-01 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.9.0...v1.9.1 )

- ![Fixed:][fixed] [#1886]( https://github.com/NREL/OpenStudio/issues/1886 ), Incorrect units for  Refrigeration Walk-In Construction U-value 
- ![Fixed:][fixed] [#1894]( https://github.com/NREL/OpenStudio/issues/1894 ), 1.9.0 Turning off "Run for sizing period" yet it still seems to run them

## OpenStudio v1.9.0
###### 2015-09-25 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.8.5...v1.9.0 )

- ![Fixed:][fixed] [#404]( https://github.com/NREL/OpenStudio/issues/404 ), OS App Water Use Equipment Definitions, Peak Flow Rate Units always in SI units
- ![Fixed:][fixed] [#470]( https://github.com/NREL/OpenStudio/issues/470 ), Odd temps on outdoor air temp reset in OS app
- ![Fixed:][fixed] [#593]( https://github.com/NREL/OpenStudio/issues/593 ), Node.addSetPointManager() does not accept all types of  Setpoint Managers
- ![Fixed:][fixed] [#1005]( https://github.com/NREL/OpenStudio/issues/1005 ), Coil Cooling/Heating Water to Air HP wrappers are empty in UI
- ![Fixed:][fixed] [#1135]( https://github.com/NREL/OpenStudio/issues/1135 ), Radiance Parameters are reset to Coarse every time the osm is loaded
- ![Fixed:][fixed] [#1347]( https://github.com/NREL/OpenStudio/issues/1347 ), Update web-assets for 1.6.0 package
- ![Fixed:][fixed] [#1365]( https://github.com/NREL/OpenStudio/issues/1365 ), Design Flow Air Rate in CoilCoolingWater object uses fluid vs. air units
- ![Fixed:][fixed] [#1468]( https://github.com/NREL/OpenStudio/issues/1468 ), Change peak flow rate in water use equipment definitions from cfm to gal/min
- ![Fixed:][fixed] [#1543]( https://github.com/NREL/OpenStudio/issues/1543 ), Seeing "Autocalculate" in Maximum Air Flow Rate for terminal object
- ![Fixed:][fixed] [#1547]( https://github.com/NREL/OpenStudio/issues/1547 ), SpaceType Tab - Load filter should function on infiltration objects
- ![Fixed:][fixed] [#1604]( https://github.com/NREL/OpenStudio/issues/1604 ), PumpConstantSpeed - PumpFlowRateSchedule not in schedule registry
- ![Fixed:][fixed] [#1610]( https://github.com/NREL/OpenStudio/issues/1610 ), boiler efficiency curve causes fatal error
- ![Fixed:][fixed] [#1636]( https://github.com/NREL/OpenStudio/issues/1636 ), Output Control Reporting Tolerance units default to SI despite app unit preference
- ![Fixed:][fixed] [#1657]( https://github.com/NREL/OpenStudio/issues/1657 ), SketchUp Plug-in installation error message does not apply to all cases
- ![Fixed:][fixed] [#1703]( https://github.com/NREL/OpenStudio/issues/1703 ), Radiance simulation does not use the EnergyPlus installed with OpenStudio
- ![Fixed:][fixed] [#1749]( https://github.com/NREL/OpenStudio/issues/1749 ), Missing SetpointManager:Scheduled icon
- ![Fixed:][fixed] [#1757]( https://github.com/NREL/OpenStudio/issues/1757 ), Schedule Sets sub tab "My Model" doesn't populate at first
- ![Fixed:][fixed] [#1771]( https://github.com/NREL/OpenStudio/issues/1771 ), AirTerminalSingleDuctSeriesPIUReheat leaves ZoneHVAC:EquipmentList in bad state
- ![Fixed:][fixed] [#1784]( https://github.com/NREL/OpenStudio/issues/1784 ), Crash When Deleting HPWH
- ![Fixed:][fixed] [#1804]( https://github.com/NREL/OpenStudio/issues/1804 ), Pipe:Adiabatic icon doesn't show, OS 1.8.4 Develop Branch on Windows
- ![Fixed:][fixed] [#1819]( https://github.com/NREL/OpenStudio/issues/1819 ), Material in templates is missing roughness and causes failed simulation
- ![Fixed:][fixed] [#1834]( https://github.com/NREL/OpenStudio/issues/1834 ), Changing minimum values in SpaceInfiltration:DesignFlowRate object
- ![Fixed:][fixed] [#1836]( https://github.com/NREL/OpenStudio/issues/1836 ), Fresh develop build Crash "Bad cast" after clicking on "Spaces" tab
- ![Fixed:][fixed] [#1839]( https://github.com/NREL/OpenStudio/issues/1839 ), component setpoint operation scheme logic is flawed
- ![Fixed:][fixed] [#1840]( https://github.com/NREL/OpenStudio/issues/1840 ), AVM Night Cycle Issue
- ![Fixed:][fixed] [#1843]( https://github.com/NREL/OpenStudio/issues/1843 ), Space Tab - Subsurfaces sub-tab in lets you make new spaces with green "+"
- ![Fixed:][fixed] [#1844]( https://github.com/NREL/OpenStudio/issues/1844 ), Space Tab - Shading Controls sub-tab 
- ![Fixed:][fixed] [#1845]( https://github.com/NREL/OpenStudio/issues/1845 ), Default low temp radiant variable flow setpoint schedules reversed
- ![Fixed:][fixed] [#1853]( https://github.com/NREL/OpenStudio/issues/1853 ), Number of WaterHeater:Stratified Nodes Fixed at One
- ![Fixed:][fixed] [#1859]( https://github.com/NREL/OpenStudio/issues/1859 ), IP unit errors
- ![Fixed:][fixed] [#1863]( https://github.com/NREL/OpenStudio/issues/1863 ), LoadProfilePlant default load schedule is incorrect
- ![Fixed:][fixed] [#1866]( https://github.com/NREL/OpenStudio/issues/1866 ), PlantLoop::remove doesn't work!
- ![Fixed:][fixed] [#1868]( https://github.com/NREL/OpenStudio/issues/1868 ), PAT 1.8.5 hanging when running multiple simulations
- ![Fixed:][fixed] [#1873]( https://github.com/NREL/OpenStudio/issues/1873 ), Building Stories not exposed in "My Model" pane for Spaces > Property sub-tab
- ![Fixed:][fixed] [#1874]( https://github.com/NREL/OpenStudio/issues/1874 ), WaterHeater:Mixed is missing form My Model, prevents some user use cases
- ![Fixed:][fixed] [#1875]( https://github.com/NREL/OpenStudio/issues/1875 ), When you make a new schedule "Temperature" shows up twice in the Schedule Type list.
- ![Fixed:][fixed] [#1879]( https://github.com/NREL/OpenStudio/issues/1879 ), OS App creates simulation settings objects different from E+ defaults
- ![Fixed:][fixed] [#1887]( https://github.com/NREL/OpenStudio/issues/1887 ), Deleting a plant component can orphan a node
- ![Fixed:][fixed] [#1891]( https://github.com/NREL/OpenStudio/issues/1891 ), Application is freezing when trying to remove plantloop from model
0

## OpenStudio v1.8.5
###### 2015-09-04 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.8.4...v1.8.5 )

- ![Added:][added] [#1670]( https://github.com/NREL/OpenStudio/issues/1670 ), Add support for heat pump water heater
- ![Added:][added] [#1679]( https://github.com/NREL/OpenStudio/issues/1679 ), Add Support of dual duct systems and related components.
- ![Added:][added] [#1712]( https://github.com/NREL/OpenStudio/issues/1712 ), Add missing availability managers
- ![Added:][added] [#1774]( https://github.com/NREL/OpenStudio/issues/1774 ), Add miscellaneous remaining objects
- ![Improved:][improved] [#1441]( https://github.com/NREL/OpenStudio/issues/1441 ), Ship Radiance with OpenStudio
- ![Fixed:][fixed] [#937]( https://github.com/NREL/OpenStudio/issues/937 ), Create From External File + facility.exteriorLights crashes ruby
- ![Fixed:][fixed] [#1496]( https://github.com/NREL/OpenStudio/issues/1496 ), Installer puts 64 bit E+ in 32 bit OS installer.
- ![Fixed:][fixed] [#1577]( https://github.com/NREL/OpenStudio/issues/1577 ), Scan for tools finding wrong EnergyPlus version
- ![Fixed:][fixed] [#1601]( https://github.com/NREL/OpenStudio/issues/1601 ), PAT ruby script path not set
- ![Fixed:][fixed] [#1617]( https://github.com/NREL/OpenStudio/issues/1617 ), Repeatable crash when loading specific seed model
- ![Fixed:][fixed] [#1656]( https://github.com/NREL/OpenStudio/issues/1656 ), VRF OA Unit IG policy
- ![Fixed:][fixed] [#1695]( https://github.com/NREL/OpenStudio/issues/1695 ), The min. cfm/sf value in the zone:sizing object is being converted from IP to SI wrong in OS 1.7.0
- ![Fixed:][fixed] [#1701]( https://github.com/NREL/OpenStudio/issues/1701 ), Deleting plant equipment with setpoint schedule on outlet node orphans setpoint schedule
- ![Fixed:][fixed] [#1706]( https://github.com/NREL/OpenStudio/issues/1706 ), Zones with FanZoneExhaust require thermostat schedules
- ![Fixed:][fixed] [#1730]( https://github.com/NREL/OpenStudio/issues/1730 ), ModelObjects not translated to E+
- ![Fixed:][fixed] [#1740]( https://github.com/NREL/OpenStudio/issues/1740 ), Detached drop downs 
- ![Fixed:][fixed] [#1746]( https://github.com/NREL/OpenStudio/issues/1746 ), OpenStudio 1.8.0 does not work with installed E+
- ![Fixed:][fixed] [#1747]( https://github.com/NREL/OpenStudio/issues/1747 ), SetpointManager:Scheduled crash
- ![Fixed:][fixed] [#1755]( https://github.com/NREL/OpenStudio/issues/1755 ), OpenStudio App crash on user file
- ![Fixed:][fixed] [#1762]( https://github.com/NREL/OpenStudio/issues/1762 ), Documentation for removeSupplyBranchWithComponent is incorrect
- ![Fixed:][fixed] [#1763]( https://github.com/NREL/OpenStudio/issues/1763 ), An unhandled exception has occurred. Access violation - no RTTI data
- ![Fixed:][fixed] [#1778]( https://github.com/NREL/OpenStudio/issues/1778 ), Incorrect SI to IP unit conversion in sizing:zone gridview
- ![Fixed:][fixed] [#1779]( https://github.com/NREL/OpenStudio/issues/1779 ), Can't Delete the First OS:WaterHeater:Stratified object Added to Plant Loop Using Measure
- ![Fixed:][fixed] [#1780]( https://github.com/NREL/OpenStudio/issues/1780 ), WaterHeating:AirToWaterHeatPump Rated Condenser Inlet Water Temperature Forcing Random Value
- ![Fixed:][fixed] [#1786]( https://github.com/NREL/OpenStudio/issues/1786 ), Issue with Python bindings for v1.8.0
- ![Fixed:][fixed] [#1787]( https://github.com/NREL/OpenStudio/issues/1787 ), Orphaned connections when deleting Air-to-Air HX from outdoor air system with other outdoor air equipment
- ![Fixed:][fixed] [#1795]( https://github.com/NREL/OpenStudio/issues/1795 ), Hide "Apply to Selected" for Outside Boundary Condition Object 
- ![Fixed:][fixed] [#1808]( https://github.com/NREL/OpenStudio/issues/1808 ), Effective Air Leakage Area Units Incorrect
- ![Fixed:][fixed] [#1810]( https://github.com/NREL/OpenStudio/issues/1810 ), OS version test failing
- ![Fixed:][fixed] [#1815]( https://github.com/NREL/OpenStudio/issues/1815 ), Install Radiance with OpenStudio
- ![Fixed:][fixed] [#1816]( https://github.com/NREL/OpenStudio/issues/1816 ), EDAPT XML not encoded in UTF-8
- ![Fixed:][fixed] [#1817]( https://github.com/NREL/OpenStudio/issues/1817 ), Need to warn user if IP units measure is used
- ![Fixed:][fixed] [#1834]( https://github.com/NREL/OpenStudio/issues/1834 ), Changing minimum values in SpaceInfiltration:DesignFlowRate object

## OpenStudio v1.8.4
###### 2015-08-24 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.8.3...v1.8.4 )

- ![Added:][added] [#1744]( https://github.com/NREL/OpenStudio/pull/1744 ), Added new setpoint managers
- ![Added:][added] [#1759]( https://github.com/NREL/OpenStudio/pull/1759 ), Added new coils
- ![Added:][added] [#1773]( https://github.com/NREL/OpenStudio/pull/1773 ), Added new ZoneHVAC objects
- ![Added:][added] [#1707]( https://github.com/NREL/OpenStudio/pull/1707 ), Added Dual Duct
- ![Added:][added] [#1719]( https://github.com/NREL/OpenStudio/pull/1719 ), Added Duct, FluidCoolerSingleSpeed, FluidCoolerTwoSpeed, PipeIndoor, PipeOutdoor
- ![Added:][added] [#1727]( https://github.com/NREL/OpenStudio/pull/1727 ), Added new availability managers
- ![Added:][added] [#1739]( https://github.com/NREL/OpenStudio/pull/1739 ), Added ZoneMixing
- ![Added:][added] [#1742]( https://github.com/NREL/OpenStudio/pull/1742 ), Added solar collectors
- ![Added:][added] [#1769]( https://github.com/NREL/OpenStudio/pull/1769 ), Added Plant operation schemes
- ![Added:][added] [#1788]( https://github.com/NREL/OpenStudio/pull/1788 ), Added misc objects
- ![Added:][added] [#1782]( https://github.com/NREL/OpenStudio/pull/1782 ), Added Zone ventilation design flow rate
- ![Improved:][improved] [#1764]( https://github.com/NREL/OpenStudio/pull/1764 ), Radiance functionality refactored as a measure
- ![Improved:][improved] [#1642]( https://github.com/NREL/OpenStudio/issues/1642 ), Add Space Infiltration Effective Leakage Area object to library files
- ![Fixed:][fixed] [#1758]( https://github.com/NREL/OpenStudio/issues/1758 ), OpenStudio::EpwFile.new() crashes on Linux
- ![Fixed:][fixed] [#1767]( https://github.com/NREL/OpenStudio/issues/1767 ), Hard Crash Opening 1.5.3 CBECC-Com OSM in 1.8.3
- ![Fixed:][fixed] [#1770]( https://github.com/NREL/OpenStudio/issues/1770 ), Facility tab grid view crash
- ![Fixed:][fixed] [#1776]( https://github.com/NREL/OpenStudio/issues/1776 ), WaterHeaterStratified does not work with HPWH
- ![Fixed:][fixed] [#1781]( https://github.com/NREL/OpenStudio/issues/1781 ), OpenStudio measure without measure.rb file crashes PAT
- ![Fixed:][fixed] [#1783]( https://github.com/NREL/OpenStudio/issues/1783 ), Files won't open with OS App 1.8.3
- ![Fixed:][fixed] [#1766]( https://github.com/NREL/OpenStudio/pull/1766 ), Update DiagnosticScript to remove cruft from badly initialized zones

## OpenStudio v1.8.3
###### 2015-08-10 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.8.2...v1.8.3 )

- ![Added:][added] [#1388]( https://github.com/NREL/OpenStudio/issues/1388 ), Allow users to specify costs and ECMs on external models imported to PAT
- ![Added:][added] [#1711]( https://github.com/NREL/OpenStudio/pull/1711 ), Facility tab grid view
- ![Improved:][improved] [#854]( https://github.com/NREL/OpenStudio/issues/854 ), Radiance FwdTranslator - Improve material handling on building shading surfaces
- ![Improved:][improved] [#891]( https://github.com/NREL/OpenStudio/issues/891 ), Show District Heating and Cooling End-uses in Results Summary Graphs
- ![Fixed:][fixed] [#1709]( https://github.com/NREL/OpenStudio/issues/1709 ), Bug in plant operation schemes object for component setpoint
- ![Fixed:][fixed] [#1745]( https://github.com/NREL/OpenStudio/pull/1745 ), Fix component setpoint scheme

## OpenStudio v1.8.2
###### 2015-07-24 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.8.1...v1.8.2 )

- ![Added:][added] [#1674]( https://github.com/NREL/OpenStudio/pull/1674 ), Add support of heat pump water heater
- ![Added:][added] [#1682]( https://github.com/NREL/OpenStudio/issues/1682 ), Add support of WaterHeater:Stratified
- ![Improved:][improved] [#1653]( https://github.com/NREL/OpenStudio/pull/1653 ), IFC import improvements
- ![Fixed:][fixed] [#1489]( https://github.com/NREL/OpenStudio/pull/1489 ), EPWFile ruby access
- ![Fixed:][fixed] [#1733]( https://github.com/NREL/OpenStudio/pull/1733 ), TimeSeries Fix

## OpenStudio v1.8.1
###### 2015-07-14 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.8.0...v1.8.1 )

- ![Added:][added] [#1652]( https://github.com/NREL/OpenStudio/pull/1652 ) Design days grid view
- ![Improved:][improved] [#1716]( https://github.com/NREL/OpenStudio/pull/1716 ) New default standard reporting measure
- ![Improved:][improved] [#1458]( https://github.com/NREL/OpenStudio/issues/1458 ), 
EPWFile includes new methods to get TimeSeries data from weather files, SWIG bindings to the EPWFile class have been improved
- ![Improved:][improved] [#1440]( https://github.com/NREL/OpenStudio/issues/1440 ), Integrate openstudio-aws-gem into openstudio
- ![Improved:][improved] [#1698]( https://github.com/NREL/OpenStudio/pull/1698 ), AWS and Gem Updates
- ![Fixed:][fixed] [#1699]( https://github.com/NREL/OpenStudio/issues/1699 ), Meters are missing time indices in sql file

## OpenStudio v1.8.0
###### 2015-06-26 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.7.5...v1.8.0 )

- ![Added:][added] Allow Gem installation at build time
- ![Fixed:][fixed] [#599]( https://github.com/NREL/OpenStudio/issues/599 ), Some HVAC components that exist in OpenStudio are missing from the HVAC library
- ![Fixed:][fixed] [#1440]( https://github.com/NREL/OpenStudio/issues/1440 ), Update PAT to use aws-gem for cloud initialization
- ![Fixed:][fixed] [#1476]( https://github.com/NREL/OpenStudio/issues/1476 ), IG Hard Sized / Autosized Behavior 
- ![Fixed:][fixed] [#1556]( https://github.com/NREL/OpenStudio/issues/1556 ), Create One With Selected Measures creates design alternative with empty name
- ![Fixed:][fixed] [#1573]( https://github.com/NREL/OpenStudio/issues/1573 ), Some RefrigerationCase defrost options are not supported 
- ![Fixed:][fixed] [#1605]( https://github.com/NREL/OpenStudio/issues/1605 ), Error in AirLoopHVACUnitarySystem Library 
- ![Fixed:][fixed] [#1606]( https://github.com/NREL/OpenStudio/issues/1606 ), HW Coil in AirTerminalSingleDuctParallelPIUReheat cannot be connected
- ![Fixed:][fixed] [#1644]( https://github.com/NREL/OpenStudio/issues/1644 ), Wrong unit shown for Heat Exchanger U-Factor Times Area Value in OS:HeatExchanger:FluidToFluid
- ![Fixed:][fixed] [#1667]( https://github.com/NREL/OpenStudio/issues/1667 ), SketchUp Inspector does not properly create ThermalZone
- ![Fixed:][fixed] [#1673]( https://github.com/NREL/OpenStudio/issues/1673 ), UnitHeater doesn't allow Fan:OnOff 
- ![Fixed:][fixed] [#1676]( https://github.com/NREL/OpenStudio/issues/1676 ), Two HeatExchangerAirToAirSensibleAndLatent are output to EquipmentList
- ![Fixed:][fixed] [#1686]( https://github.com/NREL/OpenStudio/issues/1686 ), Fix error in ThermalZone::clone 
- ![Fixed:][fixed] [#1687]( https://github.com/NREL/OpenStudio/issues/1687 ), ThermalZone::remove leaves an orphaned connection
- ![Fixed:][fixed] [#1692]( https://github.com/NREL/OpenStudio/issues/1692 ), TimeSeries timestamps made incorrectly using E+ 8.3 sql file
- ![Fixed:][fixed] [#1699]( https://github.com/NREL/OpenStudio/issues/1699 ), Meters are missing time indices in sql file

## OpenStudio v1.7.5
###### 2015-06-01 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.7.4...v1.7.5 )

- ![Added:][added] [#1666]( https://github.com/NREL/OpenStudio/pull/1666 ), Added HeaderedPumpsConstantSpeed to model
- ![Added:][added] [#1666]( https://github.com/NREL/OpenStudio/pull/1666 ), Added HeaderedPumpsVariableSpeed to model
- ![Added:][added] [#1666]( https://github.com/NREL/OpenStudio/pull/1666 ), Added TemperingValve to model
- ![Added:][added] [#1666]( https://github.com/NREL/OpenStudio/pull/1666 ), Added CoilPerformanceDXCooling to model
- ![Added:][added] [#1666]( https://github.com/NREL/OpenStudio/pull/1666 ), Added CoilCoolingDXTwoStageWithHumidityControlMode to model
- ![Added:][added] [#1666]( https://github.com/NREL/OpenStudio/pull/1666 ), Added CoilSystemCoolingWaterHeatExchangerAssisted to model
- ![Added:][added] [#1666]( https://github.com/NREL/OpenStudio/pull/1666 ), Added GroundHeatExchangerHorizontalTrench to model
- ![Added:][added] [#1666]( https://github.com/NREL/OpenStudio/pull/1666 ), Added ThermalStorageIceDetailed to model
- ![Added:][added] [#1666]( https://github.com/NREL/OpenStudio/pull/1666 ), Added ChillerAbsorptionIndirect to model
- ![Added:][added] [#1666]( https://github.com/NREL/OpenStudio/pull/1666 ), Added ChillerAbsorption to model
- ![Changed:][changed] [#1658]( https://github.com/NREL/OpenStudio/issues/1658 ), StraightComponent::addToNode will now remove from existing loop first
- ![Changed:][changed] [#1662]( https://github.com/NREL/OpenStudio/issues/1662 ), WaterHeaterMixed will now be more fully initialized 
- ![Improved:][improved] [#1637]( https://github.com/NREL/OpenStudio/issues/1637 ), DistrictHeating / DistrictCooling ::nominalCapacity is now autosizable
- ![Improved:][improved] [#1638]( https://github.com/NREL/OpenStudio/issues/1638 ), HumidifierSteamElectric::ratedCapacity is now autosizable
- ![Improved:][improved] [#1666] ( https://github.com/NREL/OpenStudio/pull/1666 ), Enhance SizingSystem with scalable sizing 
- ![Improved:][improved] [#1639] ( https://github.com/NREL/OpenStudio/pull/1639 ), Add SizingPlant coincident sizing option 
- ![Fixed:][fixed] [#1655] ( https://github.com/NREL/OpenStudio/issues/1655 ), SQLFile queries much slower for E+ 8.3 results
- ![Fixed:][fixed] [#1660] ( https://github.com/NREL/OpenStudio/issues/1660 ), PAT and OS App helps links are dead
- ![Fixed:][fixed] [#1665] ( https://github.com/NREL/OpenStudio/issues/1665 ), OpenStudio OS X Packages are broken
- ![Removed:][removed] [#1648]( https://github.com/NREL/OpenStudio/issues/1648 ), Removed AirLoopHVAC::addAirLoopComp, please use HVACComponent::addToNode instead

## OpenStudio v1.7.4
###### 2015-05-18 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.7.3...v1.7.4 )

- ![Fixed:][fixed] [#1411]( https://github.com/NREL/OpenStudio/issues/1411 ), IG should not add a NULL choice to the lights definition combo box.

## OpenStudio v1.7.3
###### 2015-04-29 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.7.2...v1.7.3 )

- ![Added:][added] [#1631] ( https://github.com/NREL/OpenStudio/pull/1631 ) Added new method OpenStudio::getEnergyPlusExecutable which can be used to determine the location of the EnergyPlus installation included in OpenStudio.
- ![Fixed:][fixed] [#1580]( https://github.com/NREL/OpenStudio/issues/1580 ), Fixed errors in which OpenStudio could not be used on a system without a graphics adapter.
- ![Fixed:][fixed] [#1580]( https://github.com/NREL/OpenStudio/issues/1629 ), Evap cooler 1.7.1 -> 1.7.2 version translation error 

## OpenStudio v1.7.2
###### 2015-04-21 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.7.1...v1.7.2 )

- ![Added:][added] [#1624]( https://github.com/NREL/OpenStudio/pull/1624 ), Support for EnergyPlus 8.3
- ![Improved:][improved] [#1621]( https://github.com/NREL/OpenStudio/issues/1621 ), EvaporativeCoolerDirectResearchSpecial::RecirculatingWaterPumpPowerConsumption is now autosizable
- ![Improved:][improved] [#1623]( https://github.com/NREL/OpenStudio/issues/1623 ), EvaporativeCoolerIndirectResearchSpecial::RecirculatingWaterPumpPowerConsumption is now autosizable
- ![Fixed:][fixed] [#1594]( https://github.com/NREL/OpenStudio/issues/1594 ), RadianceParameters coarse and fine value adjustments
- ![Fixed:][fixed] [#1620]( https://github.com/NREL/OpenStudio/issues/1620 ), Building shading surfaces exported incorrectly in RadFwdTrans 

## OpenStudio v1.7.1
###### 2015-03-31 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.7.0...v1.7.1 )

- ![Added:][added] [#1565]( https://github.com/NREL/OpenStudio/pull/1565 ), Import IFC into OpenStudio via BIMserver, [tutorial here](http://nrel.github.io/OpenStudio-user-documentation/tutorials/tutorial_ifcimport/)

## OpenStudio v1.7.0
###### 2015-03-27 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.6.3...v1.7.0 )

- ![Added:][added] [#1524]( https://github.com/NREL/OpenStudio/pull/1524 ), Download VRF components from BCL and use in OpenStudio Application
- ![Changed:][changed] [#1521]( https://github.com/NREL/OpenStudio/issues/1521 ), ZoneHVACComponent coils should stay attached to plant when cloning into same model
- ![Changed:][changed] [#1510]( https://github.com/NREL/OpenStudio/pull/1510 ), ExteriorLights::schedule now returns boost::optional<Schedule> instead of Schedule
- ![Fixed:][fixed] [#485]( https://github.com/NREL/OpenStudio/issues/485 ), Radiance ForwardTranslator crashes if interior partitions missing construction
- ![Fixed:][fixed] [#667]( https://github.com/NREL/OpenStudio/issues/667 ), User should not be able to change space shading to building shading in inspector
- ![Fixed:][fixed] [#883]( https://github.com/NREL/OpenStudio/issues/883 ),  E+ translator warning: Unknown object OS:ClimateZones
- ![Fixed:][fixed] [#1297]( https://github.com/NREL/OpenStudio/issues/1297 ), Exterior lights crash on tranlastion to idf
- ![Fixed:][fixed] [#1299]( https://github.com/NREL/OpenStudio/issues/1299 ), Matched surface error if one space is not translated
- ![Fixed:][fixed] [#1340]( https://github.com/NREL/OpenStudio/issues/1340 ), Opening a previously run PAT project in a new OS version deletes all of the results
- ![Fixed:][fixed] [#1346]( https://github.com/NREL/OpenStudio/issues/1346 ), Results lost on model re-open - "Error opening measure and run data"
- ![Fixed:][fixed] [#1348]( https://github.com/NREL/OpenStudio/issues/1348 ), Arguments for E+ measures gets empty workspace in app
- ![Fixed:][fixed] [#1363]( https://github.com/NREL/OpenStudio/issues/1363 ), 1.6.0 PAT crash when switching to results tab
- ![Fixed:][fixed] [#1364]( https://github.com/NREL/OpenStudio/issues/1364 ), 1.6.0 PAT crash when run then cancel a simulation
- ![Fixed:][fixed] [#1376]( https://github.com/NREL/OpenStudio/issues/1376 ), PAT export to spreadsheet broken
- ![Fixed:][fixed] [#1461]( https://github.com/NREL/OpenStudio/issues/1461 ), Sql.annualTotalUtilityCost and .annualTotalCost methods incorrect when district heating or cooling
- ![Fixed:][fixed] [#1481]( https://github.com/NREL/OpenStudio/issues/1481 ), Infinite directories for OSM in measure directory
- ![Fixed:][fixed] [#1506]( https://github.com/NREL/OpenStudio/issues/1506 ), GridView column headings are sometimes clipped
- ![Fixed:][fixed] [#1507]( https://github.com/NREL/OpenStudio/issues/1507 ), Unused resource objects not purged from input models to RadianceForwardTranslator
- ![Fixed:][fixed] [#1509]( https://github.com/NREL/OpenStudio/issues/1509 ), ZoneHVACComponent::clone
- ![Fixed:][fixed] [#1518]( https://github.com/NREL/OpenStudio/issues/1518 ), ResultsViewer bugs from QWT update
- ![Fixed:][fixed] [#1523]( https://github.com/NREL/OpenStudio/issues/1523 ), TableMultiVariableLookup not translating correctly
- ![Fixed:][fixed] [#1539]( https://github.com/NREL/OpenStudio/issues/1539 ), Window reveals are incorrect

## OpenStudio v1.6.3
###### 2015-03-10 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.6.2...v1.6.3 )

- ![Added:][added] [#1505]( https://github.com/NREL/OpenStudio/pull/1505 ), Added multi-edit capability to grid views in OpenStudio Application
- ![Improved:][improved] [#1513]( https://github.com/NREL/OpenStudio/issues/1513 ), Partitioning and triangulation of surfaces with doors
- ![Improved:][improved] [#1488]( https://github.com/NREL/OpenStudio/pull/1488 ), Better IDF import log messages
- ![Improved:][improved] [#1486]( https://github.com/NREL/OpenStudio/pull/1486 ), Switching to QWebEngine if building with Qt >= 5.4
- ![Fixed:][fixed] [#318]( https://github.com/NREL/OpenStudio/issues/318 ), Daylighting results only applied to 1 zone out of 9
- ![Fixed:][fixed] [#447]( https://github.com/NREL/OpenStudio/issues/447 ), User model crashes when trying to add thermal zone to air loop named "Gas Fired Furnace"
- ![Fixed:][fixed] [#641]( https://github.com/NREL/OpenStudio/issues/641 ), Orphan Controller:MechanicalVentilation object
- ![Fixed:][fixed] [#870]( https://github.com/NREL/OpenStudio/issues/870 ), Cloning object from library multiple times will keep making extra copies in the model
- ![Fixed:][fixed] [#978]( https://github.com/NREL/OpenStudio/issues/978 ), Cloning Building doesn't bring over thermostatSetpointDualSetpoint
- ![Fixed:][fixed] [#980]( https://github.com/NREL/OpenStudio/issues/980 ), Clone Building doesn't keep spaces connected to thermal zones
- ![Fixed:][fixed] [#1296]( https://github.com/NREL/OpenStudio/issues/1296 ), Schedule causes crash on translation to IDF
- ![Fixed:][fixed] [#1350]( https://github.com/NREL/OpenStudio/issues/1350 ), Repeatable way to have walls fly away when adding fenestration in Plugin
- ![Fixed:][fixed] [#1357]( https://github.com/NREL/OpenStudio/issues/1357 ), SPM Mixed Air Magic Fails on AirLoops with Unitary Equipment
- ![Fixed:][fixed] [#1372]( https://github.com/NREL/OpenStudio/issues/1372 ), PAT Crash when any measure fails because of a ruby error
- ![Fixed:][fixed] [#1378]( https://github.com/NREL/OpenStudio/issues/1378 ), Crash when running user file
- ![Fixed:][fixed] [#1482]( https://github.com/NREL/OpenStudio/issues/1482 ), Define New Schedule dialog isn't modal, can get lost behind app.
- ![Fixed:][fixed] [#1495]( https://github.com/NREL/OpenStudio/issues/1495 ), Shading surfaces not imported from gbXML
- ![Removed:][removed] [#1499]( https://github.com/NREL/OpenStudio/pull/1499 ), Removed `FloodPlot`, `LinePlot`, `AnnotatedTimeline`, `Barchart`, and `PieChart` classes.  These have been deprecated in favor of JavaScript reporting measures.

## [OpenStudio v1.6.2]( https://github.com/NREL/OpenStudio/releases/tag/v1.6.2 )
###### 2015-02-19 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.6.1...v1.6.2 )

- ![Added:][added] [#1418]( https://github.com/NREL/OpenStudio/pull/1418 ), New fan coils and cycling PTAC for the library
- ![Added:][added] [#1457]( https://github.com/NREL/OpenStudio/pull/1457 ), Window frame and divider object 
- ![Added:][added] [#1457]( https://github.com/NREL/OpenStudio/pull/1457 ), Added support for wall thickness in Radiance, translating WindowFrameAndDivider objects to impostor geometry
- ![Added:][added] [#1503]( https://github.com/NREL/OpenStudio/pull/1503 ), Radiance 3-phase simulation for dynamic window shading controls
- ![Improved:][improved] [#1377]( https://github.com/NREL/OpenStudio/pull/1377 ), Testing around implementation of clone 
- ![Fixed:][fixed] [#486]( https://github.com/NREL/OpenStudio/issues/486 ), ModelToRadPreProcess changes GlassDoor to regular Door
- ![Fixed:][fixed] [#1380]( https://github.com/NREL/OpenStudio/issues/1380 ), Crash when manually disconnecting CoilHeatingWater from loop
- ![Fixed:][fixed] [#1413]( https://github.com/NREL/OpenStudio/issues/1413 ), Unparented widget flash on selected in grid view
- ![Fixed:][fixed] [#1456]( https://github.com/NREL/OpenStudio/pull/1456 ), Crash on remove water coil


## [OpenStudio v1.6.1]( https://github.com/NREL/OpenStudio/releases/tag/v1.6.1 )
###### 2015-02-09 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.6.0...v1.6.1 )

- ![Added:][added] [#1356]( https://github.com/NREL/OpenStudio/pull/1356 ), Support for TransOption in SDD ReverseTranslator
- ![Added:][added] [#1367]( https://github.com/NREL/OpenStudio/pull/1367 ), AirLoopHVAC::supplyFan, AirLoopHVAC::returnFan, AirLoopHVAC::reliefFan. These DO NOT account for fans inside compound objects like unitary systems.
- ![Improved:][improved] [#1374]( https://github.com/NREL/OpenStudio/pull/1374 ), Implementation of thermal zone clone
- ![Improved:][improved] [#1463]( https://github.com/NREL/OpenStudio/pull/1463 ), Schedule editor GUI updates in OpenStudio Application including ability to copy schedules
- ![Improved:][improved] [#1367]( https://github.com/NREL/OpenStudio/pull/1367 ), Setpoint manager logic so that fans inside unitary systems are accounted for. This is done during OS -> IDF translation.
- ![Fixed:][fixed] [#7]( https://github.com/NREL/OpenStudio/issues/7 ), A number of Simulation Settings Tab fields should be OSComboBoxes rather than OSLineEdits
- ![Fixed:][fixed] [#645]( https://github.com/NREL/OpenStudio/issues/645 ), PAT tab 4 - mouse over to see measures doesn't work in calibration mode
- ![Fixed:][fixed] [#1291]( https://github.com/NREL/OpenStudio/issues/1291 ), OutputControl:ReportingTolerances not SWIGd
















[added]: http://nrel.github.io/OpenStudio-user-documentation/img/added.svg "Added"
[changed]: http://nrel.github.io/OpenStudio-user-documentation/img/changed.svg "Changed"
[improved]: http://nrel.github.io/OpenStudio-user-documentation/img/improved.svg "Improved"
[fixed]: http://nrel.github.io/OpenStudio-user-documentation/img/fixed.svg "Fixed"
[deprecated]: http://nrel.github.io/OpenStudio-user-documentation/img/deprecated.svg "Deprecated"
[removed]: http://nrel.github.io/OpenStudio-user-documentation/img/removed.svg "Removed"
