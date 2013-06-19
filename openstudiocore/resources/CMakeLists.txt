# Configure a header file to point to the resources directory
CONFIGURE_FILE( "resources.in" "${CMAKE_BINARY_DIR}/src/resources.hxx" )

# Configure a lst file for use in transitioning between EnergyPlus versions
CONFIGURE_FILE( "IdfFilesToTransition.lst.in" "${CMAKE_BINARY_DIR}/resources/IdfFilesToTransition.lst" )

########################################################
# utilities resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(utilities_resources_src
  "utilities/BCL/Components/philadelphia pa [724086 TMY2-13739]/component.xml"
  "utilities/BCL/Components/philadelphia pa [724086 TMY2-13739]/files/USA_PA_Philadelphia_TMY2.epw"
  "utilities/BCL/Components/philadelphia pa [724086 TMY2-13739]/images/noimage_100px.png"
  utilities/BCL/Measures/SetWindowToWallRatioByFacade/measure.xml
  utilities/BCL/Measures/SetWindowToWallRatioByFacade/measure.rb
  utilities/BCL/Measures/SetWindowToWallRatioByFacade/tests/ExampleModel_Test.rb
  utilities/BCL/Measures/SetWindowToWallRatioByFacade/tests/SimpleModel_Test.rb
  utilities/BCL/Measures/SetWindowToWallRatioByFacade/tests/SimpleModel.osm
  utilities/BCL/Measures/TestCreateWarningMsgs/measure.rb
  utilities/BCL/Measures/TestCreateWarningMsgs/measure.xml
  utilities/BCL/Measures/TestFlagAsNotApplicable/measure.rb
  utilities/BCL/Measures/TestFlagAsNotApplicable/measure.xml
  utilities/BCL/Measures/TestMissingARequiredArgWithNoDefault/measure.rb
  utilities/BCL/Measures/TestMissingARequiredArgWithNoDefault/measure.xml
  utilities/Checksum/Checksum.txt
  utilities/Table/HeightWeight.csv
  utilities/Table/EUI.csv
  utilities/Table/OddlyShaped.csv
  utilities/Idf/CommentTest.idf
  utilities/Idf/DosLineEndingTest.idf
  utilities/Idf/UnixLineEndingTest.idf
  utilities/Idf/MixedLineEndingTest.idf
  utilities/Idf/FormatPropertyTest_Formatted.idf
  utilities/Idf/FormatPropertyTest_Unformatted.idf
  utilities/Zip/test1.zip
)

# update the resources
UPDATE_RESOURCES("${utilities_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_utilities_resources
  DEPENDS ${utilities_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${utilities_resources_src}
)

CREATE_SRC_GROUPS( "${utilities_resources_src}" )

########################################################
# runmanager resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(runmanager_resources_src
  runmanager/5ZoneWarmest.idf
  runmanager/USA_CO_Golden-NREL.724666_TMY3.epw
  runmanager/USA_CA_San.Francisco.Intl.AP.724940_TMY3.epw
  runmanager/USA_FL_Tampa.Intl.AP.722110_TMY3.epw
  runmanager/in.file
  runmanager/test.rb
  runmanager/ScheduleFile.csv
  runmanager/databasev1.db
  runmanager/create_os_result.rb
  runmanager/create_os_result_success.rb
  runmanager/DummyMeasure/measure.xml
  runmanager/DummyMeasure/measure.rb
  runmanager/SimpleModel.osm
)

# update the resources
UPDATE_RESOURCES("${runmanager_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_runmanager_resources
  DEPENDS ${runmanager_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${runmanager_resources_src}
)

CREATE_SRC_GROUPS( "${runmanager_resources_src}" )


########################################################
# model resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(model_resources_src
  model/OpenStudio.idd
)


# update the resources
UPDATE_RESOURCES("${model_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_model_resources
  DEPENDS ${model_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${model_resources_src}
)

CREATE_SRC_GROUPS( "${model_resources_src}" )

########################################################
# osversion resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(osversion_resources_src
  osversion/0_7_0/OpenStudio.idd
  osversion/0_7_0/example.osm
  osversion/0_7_0/example.osc
  osversion/0_7_1/OpenStudio.idd
  osversion/0_7_1/example.osm
  osversion/0_7_1/example.osc
  osversion/0_7_2/OpenStudio.idd
  osversion/0_7_2/example.osm
  osversion/0_7_2/example.osc
  osversion/0_7_3/OpenStudio.idd
  osversion/0_7_3/example.osm
  osversion/0_7_3/example.osc
  osversion/0_7_4/OpenStudio.idd
  osversion/0_7_4/example.osm
  osversion/0_7_4/example.osc
  osversion/0_7_5/OpenStudio.idd
  osversion/0_7_5/example.osm
  osversion/0_7_5/example.osc
  osversion/0_7_6/OpenStudio.idd
  osversion/0_7_6/example.osm
  osversion/0_7_6/example.osc
  osversion/0_8_0/OpenStudio.idd
  osversion/0_8_0/example.osm
  osversion/0_8_0/example.osc
  osversion/0_8_1/OpenStudio.idd
  osversion/0_8_1/example.osm
  osversion/0_8_1/example.osc
  osversion/0_8_2/OpenStudio.idd
  osversion/0_8_2/example.osm
  osversion/0_8_2/example.osc
  osversion/0_8_3/OpenStudio.idd
  osversion/0_8_3/example.osm
  osversion/0_8_3/example.osc
  osversion/0_8_4/OpenStudio.idd
  osversion/0_8_4/example.osm
  osversion/0_8_4/example.osc
  osversion/0_8_5/OpenStudio.idd
  osversion/0_8_5/example.osm
  osversion/0_8_5/example.osc
  osversion/0_9_0/OpenStudio.idd
  osversion/0_9_0/example.osm
  osversion/0_9_0/example.osc
  osversion/0_9_1/OpenStudio.idd
  osversion/0_9_1/example.osm
  osversion/0_9_1/example.osc
  osversion/0_9_2/OpenStudio.idd
  osversion/0_9_2/example.osm
  osversion/0_9_2/example.osc
  osversion/0_9_3/OpenStudio.idd
  osversion/0_9_3/example.osm
  osversion/0_9_3/example.osc
  osversion/0_9_4/OpenStudio.idd
  osversion/0_9_4/example.osm
  osversion/0_9_4/example.osc
  osversion/0_9_5/OpenStudio.idd
  osversion/0_9_5/example.osm
  osversion/0_9_5/example.osc
  osversion/0_9_6/OpenStudio.idd
  osversion/0_9_6/example.osm
  osversion/0_9_6/example.osc
  osversion/0_10_0/OpenStudio.idd
  osversion/0_10_0/example.osm
  osversion/0_10_0/example.osc
  osversion/0_10_1/OpenStudio.idd
  osversion/0_10_1/example.osm
  osversion/0_10_1/example.osc
  osversion/0_10_2/OpenStudio.idd
  osversion/0_10_2/example.osm
  osversion/0_10_2/example.osc
  osversion/0_10_3/OpenStudio.idd
  osversion/0_10_3/example.osm
  osversion/0_10_3/example.osc
  osversion/0_10_4/OpenStudio.idd
  osversion/0_10_4/example.osm
  osversion/0_10_4/example.osc
  osversion/0_10_5/OpenStudio.idd
  osversion/0_10_5/example.osm
  osversion/0_10_5/example.osc
  osversion/0_11_0/OpenStudio.idd
  osversion/0_11_0/example.osm
  osversion/0_11_0/example.osc
  osversion/0_11_1/OpenStudio.idd
  osversion/0_11_1/example.osm
  osversion/0_11_1/example.osc
  osversion/0_11_2/OpenStudio.idd
  osversion/0_11_2/example.osm
  osversion/0_11_2/example.osc
  osversion/0_11_3/OpenStudio.idd
  osversion/0_11_3/example.osm
  osversion/0_11_3/example.osc
  osversion/0_11_4/OpenStudio.idd
  osversion/0_11_4/example.osm
  osversion/0_11_4/example.osc
  osversion/0_11_5/OpenStudio.idd
  osversion/0_11_5/example.osm
  osversion/0_11_5/example.osc
  osversion/0_11_6/OpenStudio.idd
  osversion/0_11_6/example.osm
  osversion/0_11_6/example.osc
  osversion/unknown.osc
)


# update the resources
UPDATE_RESOURCES("${osversion_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_osversion_resources
  DEPENDS ${osversion_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${osversion_resources_src}
)

CREATE_SRC_GROUPS( "${osversion_resources_src}" )


if (APPLE)
  install( DIRECTORY ${CMAKE_SOURCE_DIR}/resources/osversion
           DESTINATION sharedresources
           PATTERN ".svn" EXCLUDE
         )
else()
  install( DIRECTORY ${CMAKE_SOURCE_DIR}/resources/osversion
           DESTINATION share/openstudio
           PATTERN ".svn" EXCLUDE
         )
endif()


########################################################
# modeleditor resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(modeleditor_resources_src
)

# update the resources
UPDATE_RESOURCES("${modeleditor_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_modeleditor_resources
  DEPENDS ${modeleditor_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${modeleditor_resources_src}
)

CREATE_SRC_GROUPS( "${modeleditor_resources_src}" )

########################################################
# openstudio_lib resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(openstudio_lib_resources_src
)

# update the resources
UPDATE_RESOURCES("${openstudio_lib_resources_src}")

# add target
ADD_CUSTOM_TARGET(openstudio_lib_resources
  DEPENDS ${openstudio_lib_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${openstudio_lib_resources_src}
)

CREATE_SRC_GROUPS( "${openstudio_lib_resources_src}" )

########################################################
# project resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(project_resources_src
  project/buildings/SmallOffice.idf
  project/locations/LocationDependSets.imf
  project/version/project_0_8_0.osp
  project/version/project_0_8_0.db
  project/version/project_0_8_3.osp
  project/version/project_0_8_3.db
  project/version/project_0_9_0.osp
  project/version/project_0_9_0.db
  project/version/project_0_10_0.osp
  project/version/project_0_10_0.db
  project/version/project_0_10_2.osp
  project/version/project_0_10_2.db
  project/version/project_0_10_3.osp
  project/version/project_0_10_3.db
  project/version/project_0_10_4.osp
  project/version/project_0_10_4.db
  project/version/project_0_11_5.osp
  project/version/project_0_11_5.db
)

# update the resources
UPDATE_RESOURCES("${project_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_project_resources
  DEPENDS ${project_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${project_resources_src}
)

CREATE_SRC_GROUPS( "${project_resources_src}" )

########################################################
# analysisdriver resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(analysisdriver_resources_src
  analysisdriver/PostProcessAppendToReportXML.rb
)

# update the resources
UPDATE_RESOURCES("${analysisdriver_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_analysisdriver_resources
  DEPENDS ${analysisdriver_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${analysisdriver_resources_src}
)

CREATE_SRC_GROUPS( "${analysisdriver_resources_src}" )

########################################################
# standardsinterface resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(standardsinterface_resources_src
  standardsinterface/CEC_Buildings/SmallOffice.idf
  standardsinterface/CEC_WeatherFiles/CZ01RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ02RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ03RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ04RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ05RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ06RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ07RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ08RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ09RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ10RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ11RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ12RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ13RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ14RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ15RV2.epw
  standardsinterface/CEC_WeatherFiles/CZ16RV2.epw
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ01.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ02.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ03.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ04.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ05.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ06.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ07.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ08.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ09.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ10.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ11.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ12.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ13.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ14.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ15.csv
  standardsinterface/CEC_TDVs/TDV_2008_kBtu_CZ16.csv
  standardsinterface/DataDictionary/DataDictionary.xml
  standardsinterface/DataDictionary/Project_Objects.csv
  standardsinterface/DataDictionary/Project_Attributes.csv
  standardsinterface/DataDictionary/Project_Measurements.csv
  standardsinterface/DataDictionary/Envelope_Objects.csv
  standardsinterface/DataDictionary/Envelope_Attributes.csv
  standardsinterface/DataDictionary/HVAC_Objects.csv
  standardsinterface/DataDictionary/HVAC_Attributes.csv
  standardsinterface/DataDictionary/Lighting_Objects.csv
  standardsinterface/DataDictionary/Lighting_Attributes.csv
)

# update the resources
UPDATE_RESOURCES("${standardsinterface_resources_src}")

RUN_ENERGYPLUS_CUSTOMEPW("standardsinterface/CEC_Buildings/SmallOffice.idf" "standardsinterface/CEC_WeatherFiles/CZ01RV2.epw" "standardsinterface/CEC_Buildings/SmallOffice")

# add target
ADD_CUSTOM_TARGET( openstudio_standardsinterface_resources
  DEPENDS ${standardsinterface_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${standardsinterface_resources_src}
)

CREATE_SRC_GROUPS( "${standardsinterface_resources_src}" )

########################################################
# ruleset resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(ruleset_resources_src
  ruleset/Title24_2008/PrescriptiveEnvelopeComponent_Table143A_ExteriorWallUFactor.csv
  ruleset/Title24_2008/PrescriptiveEnvelopeComponent_Table143A_RoofAndExteriorCeilingUFactor_ExteriorCeiling.csv
  ruleset/Title24_2008/PrescriptiveEnvelopeComponent_Table143A_RoofAndExteriorCeilingUFactor_Roof.csv
  ruleset/Title24_2008/PrescriptiveEnvelopeComponent_Table143A_WindowUFactor.csv
)

# update the resources
UPDATE_RESOURCES("${ruleset_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_ruleset_resources
  DEPENDS ${ruleset_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${ruleset_resources_src}
)

CREATE_SRC_GROUPS( "${ruleset_resources_src}" )

########################################################
# rulesengine resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(rulesengine_unconfigured_resources_src
  rulesengine/ImfFiles/StandardsSchedules.imf
  rulesengine/ImfFiles/Title24Schedules.imf
)

SET(rulesengine_resources_src
  ${rulesengine_unconfigured_resources_src}
)

# update the resources
UPDATE_RESOURCES("${rulesengine_unconfigured_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_rulesengine_resources
  DEPENDS ${rulesengine_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${rulesengine_resources_src}
)

CREATE_SRC_GROUPS( "${rulesengine_resources_src}" )

########################################################
# energyplus resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(energyplus_resources_src
  energyplus/Energy+.idd
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
  energyplus/HospitalBaseline/in.idf                     # not run
  energyplus/ImfFiles/HPBScheduleSets.imf
  energyplus/ErrorFiles/NoErrorsNoWarnings.err
  energyplus/ErrorFiles/SevereErrors.err
  energyplus/ErrorFiles/Warnings.err
  energyplus/ErrorFiles/WarningsAndCrash.err
  energyplus/ErrorFiles/WarningsAndSevere.err
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

# add target
ADD_CUSTOM_TARGET( openstudio_energyplus_resources
  DEPENDS ${energyplus_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${energyplus_resources_src}
)

CREATE_SRC_GROUPS( "${energyplus_resources_src}" )

########################################################
# radiance resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(radiance_resources_src
  radiance/Daylighting/annual_day.ill
)

# update the resources
UPDATE_RESOURCES("${radiance_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_radiance_resources
  DEPENDS ${radiance_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${radiance_resources_src}
)

CREATE_SRC_GROUPS( "${radiance_resources_src}" )

########################################################
# eefg resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(eefg_resources_src
  eefg/dummy.idd
  eefg/dummy.idf
)

# update the resources
UPDATE_RESOURCES("${eefg_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_eefg_resources
  DEPENDS ${eefg_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${eefg_resources_src}
)

CREATE_SRC_GROUPS( "${eefg_resources_src}" )

########################################################
# resultsviewer resources

# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(ResultsViewer_resources_src
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
ADD_CUSTOM_TARGET( ResultsViewer_resources
  DEPENDS ${ResultsViewer_resources_src} ${ENERGYPLUS_OUTPUTS}

  SOURCES ${ResultsViewer_resources_src}
)

CREATE_SRC_GROUPS( "${ResultsViewer_resources_src}" )

########################################################
# gbxml resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(gbxml_resources_src
  gbxml/simpleBox_vasari.xml
  gbxml/TwoStoryOffice_Trane.xml
  gbxml/ZNETH.xml
)

# update the resources
UPDATE_RESOURCES("${gbxml_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_gbxml_resources
  DEPENDS ${gbxml_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${gbxml_resources_src}
)

CREATE_SRC_GROUPS( "${gbxml_resources_src}" )

########################################################
# simxml resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
IF(BUILD_SIMXML)

  SET(ENERGYPLUS_OUTPUTS "")

  SET(simxml_resources_src
    simxml/ClassLevel_Test-3.xml
  )

  # update the resources
  UPDATE_RESOURCES("${simxml_resources_src}")

  # add target
  ADD_CUSTOM_TARGET( openstudio_simxml_resources
    DEPENDS ${simxml_resources_src} ${ENERGYPLUS_OUTPUTS}
    SOURCES ${simxml_resources_src}
  )

  CREATE_SRC_GROUPS( "${simxml_resources_src}" )

ENDIF(BUILD_SIMXML)

########################################################
# sdd resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
IF(BUILD_SDD)

  SET(ENERGYPLUS_OUTPUTS "")

  SET(sdd_resources_src
  )

  # update the resources
  UPDATE_RESOURCES("${sdd_resources_src}")

  # add target
  ADD_CUSTOM_TARGET( openstudio_sdd_resources
    DEPENDS ${sdd_resources_src} ${ENERGYPLUS_OUTPUTS}
    SOURCES ${sdd_resources_src}
  )

  CREATE_SRC_GROUPS( "${sdd_resources_src}" )

ENDIF(BUILD_SDD)

########################################################
# analysis resources
# clear runs, calls to RUN_ENERGYPLUS will populate this
SET(ENERGYPLUS_OUTPUTS "")

SET(analysis_resources_src
)

# update the resources
UPDATE_RESOURCES("${analysis_resources_src}")

# add target
ADD_CUSTOM_TARGET( openstudio_analysis_resources
  DEPENDS ${analysis_resources_src} ${ENERGYPLUS_OUTPUTS}
  SOURCES ${analysis_resources_src}
)

CREATE_SRC_GROUPS( "${analysis_resources_src}" )
