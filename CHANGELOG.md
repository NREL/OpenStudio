## OpenStudio v1.7.0
###### 2015-03 — [Diff](https://github.com/NREL/OpenStudio/compare/v1.6.3...v1.7.0)

- ![Changed:][changed] ExteriorLights::schedule now returns boost::optional<Schedule> instead of Schedule
- ![Fixed:][fixed] [#667](https://github.com/NREL/OpenStudio/issues/667), User should not be able to change space shading to building shading in inspector

## OpenStudio v1.6.3
###### 2015-03 — [Diff](https://github.com/NREL/OpenStudio/compare/v1.6.2...v1.6.3)

- ![Added:][added] Added multi-edit capability to grid views in OpenStudio Application
- ![Improved:][improved] Surface partitioning and triangulation 
- ![Improved:][improved] IDF import log messages
- ![Improved:][improved] Switching to QWebEngine if building with Qt >= 5.4
- ![Removed:][removed] `FloodPlot`, `LinePlot`, `AnnotatedTimeline`, `Barchart`, and `PieChart` classes.  These have been deprecated in favor of JavaScript reporting measures.
- ![Fixed:][fixed] [#318](https://github.com/NREL/OpenStudio/issues/318), Daylighting results only applied to 1 zone out of 9
- ![Fixed:][fixed] [#368](https://github.com/NREL/OpenStudio/issues/368), Ruby load paths not correctly initialized in PAT or OS app
- ![Fixed:][fixed] [#386](https://github.com/NREL/OpenStudio/issues/386), PAT crashes if I have SQL file open in ResultsViewer and then load a new baseline model
- ![Fixed:][fixed] [#447](https://github.com/NREL/OpenStudio/issues/447), User model crashes when trying to add thermal zone to air loop named "Gas Fired Furnace"
- ![Fixed:][fixed] [#486](https://github.com/NREL/OpenStudio/issues/486), ModelToRadPreProcess changes GlassDoor to regular Door
- ![Fixed:][fixed] [#498](https://github.com/NREL/OpenStudio/issues/498), Saving osm over existing osm should delete old companion directory
- ![Fixed:][fixed] [#521](https://github.com/NREL/OpenStudio/issues/521), Measure instance data still in edit pane after instance has been deleted
- ![Fixed:][fixed] [#641](https://github.com/NREL/OpenStudio/issues/641), Orphan Controller:MechanicalVentilation object
- ![Fixed:][fixed] [#688](https://github.com/NREL/OpenStudio/issues/688), Implementation of Shading Control needs some work before we show  people how to use it
- ![Fixed:][fixed] [#746](https://github.com/NREL/OpenStudio/issues/746), Crash when adding zone to air loop
- ![Fixed:][fixed] [#870](https://github.com/NREL/OpenStudio/issues/870), Cloning object from library multiple times will keep making extra copies in the model
- ![Fixed:][fixed] [#978](https://github.com/NREL/OpenStudio/issues/978), cloning building doesn't bring over thermostatSetpointDualSetpoint
- ![Fixed:][fixed] [#980](https://github.com/NREL/OpenStudio/issues/980), Clone building doesn't keep spaces connected to thermal zones
- ![Fixed:][fixed] [#1296](https://github.com/NREL/OpenStudio/issues/1296), Schedule causes crash on translation to IDF
- ![Fixed:][fixed] [#1350](https://github.com/NREL/OpenStudio/issues/1350), Repeatable way to have walls fly away when adding fenestration in Plugin
- ![Fixed:][fixed] [#1357](https://github.com/NREL/OpenStudio/issues/1357), SPM Mixed Air Magic Fails on AirLoops with Unitary Equipment
- ![Fixed:][fixed] [#1372](https://github.com/NREL/OpenStudio/issues/1372), PAT Crash when any measure fails because of a ruby error
- ![Fixed:][fixed] [#1378](https://github.com/NREL/OpenStudio/issues/1378), Crash when running user file
- ![Fixed:][fixed] [#1482](https://github.com/NREL/OpenStudio/issues/1482), Define New Schedule dialog isn't modal, can get lost behind app.
- ![Fixed:][fixed] [#1495](https://github.com/NREL/OpenStudio/issues/1495), Shading surfaces not imported from gbXML


## [OpenStudio v1.6.2](https://github.com/NREL/OpenStudio/releases/tag/v1.6.2)
###### 2015-02-19 — [Diff](https://github.com/NREL/OpenStudio/compare/v1.6.1...v1.6.2)

- ![Added:][added] New fan coils and cycling PTAC for the library
- ![Added:][added] Window frame and divider object 
- ![Added:][added] Radiance 3-phase simulation for dynamic window shading controls
- ![Improved:][improved] Testing around implementation of clone [#discussion here]( https://github.com/NREL/OpenStudio/pull/1377 )
- ![Fixed:][fixed] [#1380]( https://github.com/NREL/OpenStudio/issues/1380 ), crash when manually disconnecting CoilHeatingWater from loop
- ![Fixed:][fixed] [#1413]( https://github.com/NREL/OpenStudio/issues/1413 ), Unparented widget flash on selected in grid view
- ![Fixed:][fixed] [#1456]( https://github.com/NREL/OpenStudio/pull/1456 ), Crash on remove water coil
- ![Fixed:][fixed] [#1462]( https://github.com/NREL/OpenStudio/issues/1462 ), SizingPlant Units and Conversion Appear Incorrect


## [OpenStudio v1.6.1](https://github.com/NREL/OpenStudio/releases/tag/v1.6.1)
###### 2015-02-09 — [Diff](https://github.com/NREL/OpenStudio/compare/v1.6.0...v1.6.1)

- ![Added:][added] Support for TransOption in SDD ReverseTranslator
- ![Added:][added] AirLoopHVAC::supplyFan, AirLoopHVAC::returnFan, AirLoopHVAC::reliefFan. These DO NOT account for fans inside compound objects like unitary systems.
- ![Improved:][improved] Implementation of thermal zone clone
- ![Improved:][improved] Schedule editor GUI in OpenStudio Application
- ![Improved:][improved] Setpoint manager logic so that fans inside unitary systems are accounted for. This is done during OS -> IDF translation.
- ![Fixed:][fixed] [#7]( https://github.com/NREL/OpenStudio/issues/7 ), A number of Simulation Settings Tab fields should be OSComboBoxes rather than OSLineEdits
- ![Fixed:][fixed] [#645]( https://github.com/NREL/OpenStudio/issues/645 ), PAT tab 4 - mouse over to see measures doesn't work in calibration mode
- ![Fixed:][fixed] [#1291]( https://github.com/NREL/OpenStudio/issues/1291 ), OutputControl:ReportingTolerances not SWIGd






[added]: http://nrel.github.io/OpenStudio-user-documentation/img/added.svg "Added"
[changed]: http://nrel.github.io/OpenStudio-user-documentation/img/changed.svg "Changed"
[deprecated]: http://nrel.github.io/OpenStudio-user-documentation/img/deprecated.svg "Deprecated"
[fixed]: http://nrel.github.io/OpenStudio-user-documentation/img/fixed.svg "Fixed"
[improved]: http://nrel.github.io/OpenStudio-user-documentation/img/improved.svg "Improved"
[removed]: http://nrel.github.io/OpenStudio-user-documentation/img/removed.svg "Removed"
