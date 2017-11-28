# Configure a header file to point to the resources directory
configure_file("resources.in" "${CMAKE_BINARY_DIR}/src/resources.hxx")

# Configure a lst file for use in transitioning between EnergyPlus versions
configure_file("IdfFilesToTransition.lst.in" "${CMAKE_BINARY_DIR}/resources/IdfFilesToTransition.lst")

########################################################
# utilities resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(utilities_resources_src
  "utilities/BCL/Components/philadelphia pa [724086 TMY2-13739]/component.xml"
  "utilities/BCL/Components/philadelphia pa [724086 TMY2-13739]/files/USA_PA_Philadelphia_TMY2.epw"
  "utilities/BCL/Components/philadelphia pa [724086 TMY2-13739]/images/noimage_100px.png"
  utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/measure.xml
  utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/measure.rb
  utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/tests/EnvelopeAndLoadTestModel_01.osm
  utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/tests/EnvelopeAndLoadTestModel_02_Rotated.osm
  utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/tests/ReverseTranslatedModel.osm
  utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/tests/SetWindowToWallRatioByFacade_Test.rb
  utilities/BCL/Measures/v2/SetWindowToWallRatioByFacade/tests/test.osm
  utilities/BCL/Measures/v2/TestCreateWarningMsgs/measure.rb
  utilities/BCL/Measures/v2/TestCreateWarningMsgs/measure.xml
  utilities/BCL/Measures/v2/TestFlagAsNotApplicable/measure.rb
  utilities/BCL/Measures/v2/TestFlagAsNotApplicable/measure.xml
  utilities/BCL/Measures/v2/TestMissingARequiredArgWithNoDefault/measure.rb
  utilities/BCL/Measures/v2/TestMissingARequiredArgWithNoDefault/measure.xml
  utilities/BCL/Measures/v3/IncreaseRoofRValue/measure.xml
  utilities/BCL/Measures/v3/IncreaseRoofRValue/measure.rb
  utilities/BCL/Measures/v3/IncreaseWallRValue/measure.xml
  utilities/BCL/Measures/v3/IncreaseWallRValue/measure.rb
  utilities/BCL/Measures/v3/SetEplusInfiltration/measure.xml
  utilities/BCL/Measures/v3/SetEplusInfiltration/measure.rb
  utilities/Checksum/Checksum.txt
  utilities/Checksum/Checksum2.txt
  utilities/Filetypes/CHN_Guangdong.Shaoguan.590820_CSWD.epw
  utilities/Filetypes/USA_CO_Golden-NREL.724666_TMY3.epw
  utilities/Filetypes/USA_CO_Golden-NREL.wrap.epw
  utilities/Filetypes/USA_CO_Golden-NREL.amy
  utilities/Filetypes/USA_CO_Golden-NREL.wrap.amy
  utilities/Filetypes/TDV_2008_kBtu_CZ13.csv
  utilities/Filetypes/TUN_Tunis.607150_IWEC.epw
  utilities/Filetypes/min.osw
  utilities/Filetypes/full.osw
  utilities/Geometry/floorplan.json
  utilities/Geometry/surface_match_floorplan.json
  utilities/Geometry/threejs.json
  utilities/Idf/CommentTest.idf
  utilities/Idf/DosLineEndingTest.idf
  utilities/Idf/UnixLineEndingTest.idf
  utilities/Idf/MixedLineEndingTest.idf
  utilities/Idf/FormatPropertyTest_Formatted.idf
  utilities/Idf/FormatPropertyTest_Unformatted.idf
  utilities/SqlFile/1ZoneEvapCooler-V7-0-0.sql
  utilities/SqlFile/1ZoneEvapCooler-V7-1-0.sql
  utilities/SqlFile/1ZoneEvapCooler-V7-2-0.sql
  utilities/SqlFile/1ZoneEvapCooler-V8-0-0.sql
  utilities/SqlFile/1ZoneEvapCooler-V8-1-0.sql
  utilities/SqlFile/1ZoneEvapCooler-V8-2-0.sql
  utilities/SqlFile/1ZoneEvapCooler-V8-3-0.sql
  utilities/SqlFile/1ZoneEvapCooler-V8-4-0.sql
  utilities/Zip/test1.zip
)

# update the resources
UPDATE_RESOURCES("${utilities_resources_src}")

# add target
add_custom_target(openstudio_utilities_resources
  DEPENDS ${utilities_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${utilities_resources_src}
)

CREATE_SRC_GROUPS("${utilities_resources_src}")

########################################################
# model resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(model_resources_src
  model/OpenStudio.idd
)


# update the resources
UPDATE_RESOURCES("${model_resources_src}")

# add target
add_custom_target(openstudio_model_resources
  DEPENDS ${model_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${model_resources_src}
)

CREATE_SRC_GROUPS("${model_resources_src}")

# modeleditor resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(modeleditor_resources_src)

# update the resources
UPDATE_RESOURCES("${modeleditor_resources_src}")

# add target
add_custom_target(openstudio_modeleditor_resources
  DEPENDS ${modeleditor_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${modeleditor_resources_src}
)

CREATE_SRC_GROUPS("${modeleditor_resources_src}")

########################################################
# openstudio resources
set(openstudio_resources_src)

# update the resources
UPDATE_RESOURCES("${openstudio_resources_src}")

# add target
add_custom_target(OpenStudio_resources
  DEPENDS ${openstudio_resources_src}
  SOURCES ${openstudio_resources_src}
)

CREATE_SRC_GROUPS("${openstudio_resources_src}")

########################################################
# openstudio_lib resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(openstudio_lib_resources_src)

# update the resources
UPDATE_RESOURCES("${openstudio_lib_resources_src}")

# add target
add_custom_target(openstudio_lib_resources
  DEPENDS ${openstudio_lib_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${openstudio_lib_resources_src}
)

CREATE_SRC_GROUPS("${openstudio_lib_resources_src}")

########################################################
# measure resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(measure_resources_src)

# update the resources
UPDATE_RESOURCES("${measure_resources_src}")

# add target
add_custom_target(openstudio_measure_resources
  DEPENDS ${measure_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${measure_resources_src}
)

CREATE_SRC_GROUPS("${measure_resources_src}")

########################################################
# energyplus resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(energyplus_resources_src
  energyplus/ProposedEnergy+.idd
  energyplus/NewFileTemplate.idf
  energyplus/5ZoneAirCooled/in.idf
  energyplus/BestestEx/in.idf
  energyplus/Components/idf_roof_1/component.xml
  energyplus/Components/idf_roof_2/component.xml
  energyplus/Components/idf_roof_3/component.xml
  energyplus/Components/idf_roof_4/component.xml
  energyplus/Components/idf_roof_5/component.xml
  "energyplus/Components/idf_roof_1/Large Office TSD_Nonresidential_3A_Roof_Attic and Other.idf"
  "energyplus/Components/idf_roof_2/CEC Title 24-2008_Nonresidential_9_Roof_Insulation Entirely Above Deck.idf"
  "energyplus/Components/idf_roof_3/ASHRAE Std189_Residential_5B_Roof_Attic and Other.idf"
  "energyplus/Components/idf_roof_4/ASHRAE 90.1-2007_Residential_7A_Roof_Metal Building.idf"
  "energyplus/Components/idf_roof_5/ASHRAE 90.1-1999_Residential_B-14_Roof_Insulation Entirely Above Deck.idf"
  energyplus/Components/idf_designday_1/component.xml
  energyplus/Components/idf_designday_2/component.xml
  energyplus/Components/idf_designday_3/component.xml
  energyplus/Components/idf_designday_4/component.xml
  energyplus/Components/idf_designday_5/component.xml
  "energyplus/Components/idf_designday_1/Annual_Humidification_99_6.idf"
  "energyplus/Components/idf_designday_2/Annual_Heating_Wind_99.idf"
  "energyplus/Components/idf_designday_3/Annual_Cooling_DP_MDB_2.idf"
  "energyplus/Components/idf_designday_4/Annual_Cooling_WB_MDB_1.idf"
  "energyplus/Components/idf_designday_5/Annual_Heating_Wind_99.idf"
  energyplus/Daylighting_Office/in.idf
  energyplus/Daylighting_School/in.idf
  energyplus/HospitalBaseline/in.idf # not run
  energyplus/ImfFiles/HPBScheduleSets.imf
  energyplus/ErrorFiles/NoErrorsNoWarnings.err
  energyplus/ErrorFiles/SevereErrors.err
  energyplus/ErrorFiles/Warnings.err
  energyplus/ErrorFiles/WarningsAndCrash.err
  energyplus/ErrorFiles/WarningsAndSevere.err
  energyplus/ErrorFiles/RepeatingWarnings.err
  energyplus/Office_With_Many_HVAC_Types/in.idf
  energyplus/SimpleSurfaces/SimpleSurfaces_Reference.idf
  energyplus/SimpleSurfaces/SimpleSurfaces_Relative.idf
  energyplus/SimpleSurfaces/4ZoneWithShading_Simple_2.idf
  energyplus/TranslatorV2_Tests/Simple_Construction.idf
  energyplus/RefLargeOffice/RefBldgLargeOfficeNew2004_Chicago.idf
  energyplus/ZoneBoundaryCondition/Bug_486_zone_bdr_test.idf
)

# update the resources
UPDATE_RESOURCES("${energyplus_resources_src}")

# add runs
RUN_ENERGYPLUS("in.idf" "energyplus/5ZoneAirCooled" "USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw")
RUN_ENERGYPLUS("in.idf" "energyplus/Daylighting_Office" "USA_CO_Golden-NREL.724666_TMY3.epw")
RUN_ENERGYPLUS("in.idf" "energyplus/Daylighting_School" "USA_CO_Golden-NREL.724666_TMY3.epw")
RUN_ENERGYPLUS("in.idf" "energyplus/Office_With_Many_HVAC_Types" "USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw")

# add target
add_custom_target(openstudio_energyplus_resources
  DEPENDS ${energyplus_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${energyplus_resources_src}
)

CREATE_SRC_GROUPS("${energyplus_resources_src}")

########################################################
# radiance resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(radiance_resources_src
  radiance/Daylighting/annual_day.ill
)

# update the resources
UPDATE_RESOURCES("${radiance_resources_src}")

# add target
add_custom_target(openstudio_radiance_resources
  DEPENDS ${radiance_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${radiance_resources_src}
)

CREATE_SRC_GROUPS("${radiance_resources_src}")

########################################################
# resultsviewer resources

# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(ResultsViewer_resources_src
  resultsviewer/1ZoneEvapCooler/in.idf
  resultsviewer/1ZoneUncontrolled/in.idf
  resultsviewer/DaylightingDeviceShelf/in.idf
  resultsviewer/QkSvcRest/in.idf
  resultsviewer/SmallOffice/SmallOffice.idf
  resultsviewer/SolarCollectorFlatPlateWater/in.idf
)

# update the resources
UPDATE_RESOURCES("${ResultsViewer_resources_src}")

# add runs
RUN_ENERGYPLUS("in.idf" "resultsviewer/QkSvcRest" "USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw")
RUN_ENERGYPLUS("SmallOffice.idf" "resultsviewer/SmallOffice" "USA_IL_Chicago-OHare.Intl.AP.725300_TMY3.epw")

# add target
add_custom_target(ResultsViewer_resources
  DEPENDS ${ResultsViewer_resources_src} ${ENERGYPLUS_OUTPUTS}

  SOURCES ${ResultsViewer_resources_src}
)

CREATE_SRC_GROUPS("${ResultsViewer_resources_src}")

########################################################
# gbxml resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

SET(gbxml_resources_src
  gbxml/simpleBox_vasari.xml
  gbxml/TwoStoryOffice_Trane.xml
  gbxml/ZNETH.xml
)

# update the resources
UPDATE_RESOURCES("${gbxml_resources_src}")

# add target
add_custom_target(openstudio_gbxml_resources
  DEPENDS ${gbxml_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${gbxml_resources_src}
)

CREATE_SRC_GROUPS("${gbxml_resources_src}")

########################################################
# sdd resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(sdd_resources_src)

# update the resources
UPDATE_RESOURCES("${sdd_resources_src}")

# add target
add_custom_target(openstudio_sdd_resources
  DEPENDS ${sdd_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${sdd_resources_src}
)

CREATE_SRC_GROUPS("${sdd_resources_src}")

########################################################
# isomodel resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(isomodel_resources_src
  isomodel/exampleModel.ISO
  isomodel/weather.epw
)

# update the resources
UPDATE_RESOURCES("${isomodel_resources_src}")

# add target
add_custom_target(openstudio_isomodel_resources
  DEPENDS ${isomodel_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${isomodel_resources_src}
)

CREATE_SRC_GROUPS("${isomodel_resources_src}")

########################################################
# bimserver resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(bimserver_resources_src
)

# update the resources
UPDATE_RESOURCES("${bimserver_resources_src}")

# add target
add_custom_target(openstudio_bimserver_resources
  DEPENDS ${bimserver_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${bimserver_resources_src}
)

CREATE_SRC_GROUPS("${bimserver_resources_src}")

########################################################
# CONTAM resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
set(ENERGYPLUS_OUTPUTS "")

set(airflow_resources_src
  contam/CONTAMTemplate.osm
)

# update the resources
UPDATE_RESOURCES("${airflow_resources_src}")

# add target
add_custom_target(openstudio_airflow_resources
  DEPENDS ${airflow_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${airflow_resources_src}
)

CREATE_SRC_GROUPS("${airflow_resources_src}")

install(DIRECTORY "Examples" DESTINATION . COMPONENT "CLI")

