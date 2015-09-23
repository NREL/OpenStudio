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
