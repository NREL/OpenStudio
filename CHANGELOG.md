## OpenStudio v1.7.0
###### 2015-03 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.6.3...v1.7.0 )

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
###### 2015-03 — [Diff]( https://github.com/NREL/OpenStudio/compare/v1.6.2...v1.6.3 )

- ![Added:][added] [#1505]( https://github.com/NREL/OpenStudio/pull/1505 ), Added multi-edit capability to grid views in OpenStudio Application
- ![Improved:][improved] [#1513]( https://github.com/NREL/OpenStudio/issues/1513 ), Partitioning and triangulation of surfaces with doors
- ![Improved:][improved] [#1488]( https://github.com/NREL/OpenStudio/pull/1488 ), Better IDF import log messages
- ![Improved:][improved] [#1486]( https://github.com/NREL/OpenStudio/pull/1486 ), Switching to QWebEngine if building with Qt >= 5.4
- ![Removed:][removed] [#1499]( https://github.com/NREL/OpenStudio/pull/1499 ), Removed `FloodPlot`, `LinePlot`, `AnnotatedTimeline`, `Barchart`, and `PieChart` classes.  These have been deprecated in favor of JavaScript reporting measures.
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
[deprecated]: http://nrel.github.io/OpenStudio-user-documentation/img/deprecated.svg "Deprecated"
[fixed]: http://nrel.github.io/OpenStudio-user-documentation/img/fixed.svg "Fixed"
[improved]: http://nrel.github.io/OpenStudio-user-documentation/img/improved.svg "Improved"
[removed]: http://nrel.github.io/OpenStudio-user-documentation/img/removed.svg "Removed"
