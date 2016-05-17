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
