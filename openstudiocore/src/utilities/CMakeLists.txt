# create a library out of the utilities namespace

SET( target_name openstudio_utilities)

INCLUDE ( idd/CMakeLists.txt )
INCLUDE ( idf/CMakeLists.txt )
INCLUDE ( sql/CMakeLists.txt )
INCLUDE ( units/CMakeLists.txt )

CONFIGURE_FILE( "core/ApplicationPathHelpers.cxx.in" "${CMAKE_CURRENT_BINARY_DIR}/core/ApplicationPathHelpers.cxx" )
CONFIGURE_FILE( "core/GeneratorApplicationPathHelpers.cxx.in" "${CMAKE_CURRENT_BINARY_DIR}/core/GeneratorApplicationPathHelpers.cxx" )

SET( core_src
  ${CMAKE_CURRENT_BINARY_DIR}/core/ApplicationPathHelpers.cxx
  core/Application.hpp
  core/Application.cpp
  core/Assert.hpp
  core/Checksum.hpp
  core/Checksum.cpp
  core/CommandLine.hpp
  core/CommandLine.cpp
  core/Compare.hpp
  core/Compare.cpp
  core/Containers.hpp
  core/Containers.cpp
  core/Enum.hpp
  core/EnumHelpers.hpp
  core/Exception.hpp
  core/FileLogSink.hpp
  core/FileLogSink_Impl.hpp
  core/FileLogSink.cpp    
  core/FileReference.hpp
  core/FileReference.cpp
  core/Finder.hpp
  core/Json.hpp
  core/Json.cpp
  core/Logger.hpp
  core/Logger.cpp
  core/LogMessage.hpp
  core/LogMessage.cpp  
  core/LogSink.hpp
  core/LogSink_Impl.hpp
  core/LogSink.cpp  
  core/Macro.hpp
  core/Optional.hpp
  core/Optional.cpp
  core/Path.hpp
  core/Path.cpp
  core/PathHelpers.hpp
  core/PathHelpers.cpp
  core/PathWatcher.hpp
  core/PathWatcher.cpp
  core/Queue.hpp
  core/Serialization.hpp
  core/Singleton.hpp
  core/StaticInitializer.hpp
  core/String.hpp
  core/String.cpp
  core/StringHelpers.hpp
  core/StringHelpers.cpp
  core/StringStreamLogSink.hpp
  core/StringStreamLogSink_Impl.hpp
  core/StringStreamLogSink.cpp  
  core/TemporaryDirectory.hpp
  core/TemporaryDirectory.cpp
  core/System.hpp
  core/System.cpp
  core/UpdateManager.hpp
  core/UpdateManager.cpp
  core/Url.hpp
  core/UUID.hpp
  core/UUID.cpp
  core/URLHelpers.hpp
  core/URLHelpers.cpp
  core/UnzipFile.hpp
  core/UnzipFile.cpp
  core/ZipFile.hpp
  core/ZipFile.cpp
)

SET( data_src
  data/DataEnums.hpp
  data/Attribute.hpp
  data/Attribute_Impl.hpp
  data/Attribute.cpp
  data/CalibrationResult.hpp
  data/CalibrationResult.cpp   
  data/EndUses.hpp
  data/EndUses.cpp 
  data/Matrix.hpp
  data/Matrix.cpp
  data/Tag.hpp
  data/Tag.cpp
  data/TimeSeries.hpp
  data/TimeSeries.cpp
  data/Vector.hpp
  data/Vector.cpp
)

SET( document_src
  document/Document.hpp
  document/Document.cpp
  document/DocumentEnums.hpp
  document/DocumentRegex.hpp
  document/DocumentRegex.cpp
  document/HeadersAndFooters.hpp
  document/HeadersAndFooters.cpp
  document/DocumentElement.hpp
  document/DocumentElement_Impl.hpp
  document/DocumentElement.cpp
  document/SectionHeading.hpp
  document/SectionHeading_Impl.hpp
  document/SectionHeading.cpp
  document/SectionElement.hpp
  document/SectionElement_Impl.hpp
  document/SectionElement.cpp
  document/Section.hpp
  document/Section_Impl.hpp
  document/Section.cpp
  document/Text.hpp
  document/Text_Impl.hpp
  document/Text.cpp
  document/Table.hpp
  document/Table_Impl.hpp
  document/Table.cpp
  document/TableElement.hpp
  document/TableElement_Impl.hpp
  document/TableElement.cpp
)

SET( filetypes_src
  filetypes/EpwFile.hpp
  filetypes/EpwFile.cpp
  filetypes/TimeDependentValuationFile.hpp
  filetypes/TimeDependentValuationFile.cpp
)

SET( geometry_src
  geometry/BoundingBox.hpp
  geometry/BoundingBox.cpp
  geometry/EulerAngles.hpp
  geometry/EulerAngles.cpp  
  geometry/Geometry.hpp
  geometry/Geometry.cpp  
  geometry/Plane.hpp
  geometry/Plane.cpp
  geometry/Point3d.hpp
  geometry/Point3d.cpp
  geometry/Transformation.hpp
  geometry/Transformation.cpp
  geometry/Vector3d.hpp
  geometry/Vector3d.cpp
)

SET( math_src
  math/FloatCompare.hpp
  math/Permutation.hpp
  math/Primes.hpp
)

SET( plot_src
  plot/AnnotatedTimeline.hpp
  plot/AnnotatedTimeline.cpp
  plot/Chart.hpp
  plot/BarChart.hpp
  plot/PieChart.hpp
  plot/FloodPlot.hpp
  plot/FloodPlot.cpp
  plot/LinePlot.hpp
  plot/LinePlot.cpp
  plot/Plot2D.hpp
  plot/Plot2D.cpp
  plot/ProgressBar.hpp
  plot/ProgressBar.cpp
)

SET( time_src
  time/Calendar.hpp
  time/Calendar.cpp
  time/Date.hpp
  time/Date.cpp
  time/DateTime.hpp
  time/DateTime.cpp
  time/Time.hpp
  time/Time.cpp
)

SET( bcl_src
  bcl/BCL.hpp
  bcl/BCL.cpp
  bcl/BCLComponent.hpp
  bcl/BCLComponent.cpp
  bcl/BCLFileReference.hpp
  bcl/BCLFileReference.cpp
  bcl/BCLMeasure.hpp
  bcl/BCLMeasure.cpp
  bcl/BCLXML.hpp
  bcl/BCLXML.cpp
  bcl/LocalBCL.hpp
  bcl/LocalBCL.cpp
  bcl/OnDemandGenerator.hpp
  bcl/OnDemandGenerator.cpp
  bcl/RemoteBCL.hpp
  bcl/RemoteBCL.cpp
)

SET( cloud_src
  cloud/AWSProvider.hpp
  cloud/AWSProvider_Impl.hpp
  cloud/AWSProvider.cpp
  cloud/CloudProvider.hpp
  cloud/CloudProvider_Impl.hpp
  cloud/CloudProvider.cpp
  cloud/OSServer.hpp
  cloud/OSServer_Impl.hpp
  cloud/OSServer.cpp
  cloud/VagrantProvider.hpp
  cloud/VagrantProvider_Impl.hpp
  cloud/VagrantProvider.cpp
)

SET( documentation_src
  mainpage.hpp
)

SET( economics_src
  economics/CostLineItem.hpp
  economics/CostLineItem.cpp
  economics/CostObject.hpp
  economics/CostObject.cpp
  economics/Economics.hpp
  economics/Economics.cpp
)

SET( ${target_name}_depends
  boost_log
  zkqwt
  sqlite
  litesql
  litesql-util
  miniziplib
  qjson
  ${Boost_LIBRARIES}
  ${CMAKE_THREAD_LIBS}
  ${QT_LIBS}
)

IF(WIN32)
  LIST(APPEND ${target_name}_depends qtwinmigrate)
ENDIF(WIN32)

# moc files
SET( bcl_moc
  bcl/BCL.hpp
  bcl/LocalBCL.hpp
  bcl/RemoteBCL.hpp
)

SET( cloud_moc
  cloud/AWSProvider_Impl.hpp
  cloud/CloudProvider_Impl.hpp
  cloud/OSServer_Impl.hpp
  cloud/VagrantProvider_Impl.hpp
)

SET( core_moc
  core/PathWatcher.hpp
  core/UpdateManager.hpp
)

SET( idf_moc
  idf/IdfObject_Impl.hpp
  idf/IdfObjectWatcher.hpp
  idf/WorkspaceObjectWatcher.hpp
  idf/WorkspaceObject_Impl.hpp
  idf/Workspace_Impl.hpp
  idf/WorkspaceWatcher.hpp
)

SET( data_moc
  data/Attribute_Impl.hpp
)

SET( plot_moc
  plot/Plot2D.hpp
  plot/FloodPlot.hpp
  plot/LinePlot.hpp
  plot/ProgressBar.hpp
)

###############################################################################
# idf_moc files include generated source code.  It is important that these
# generated files are generated before moc runs.
# The following commands make sure that happens.
# This could be done by simply touching the moc'ed files in the source tree
# but in the spirit of keeping the source tree clean, we copy them into the 
# build directory instead.
###############################################################################
FOREACH( f IN LISTS idf_moc )
  STRING( REGEX REPLACE hpp$ hxx fxx ${f} )
  LIST( APPEND idfxx_moc ${CMAKE_CURRENT_BINARY_DIR}/${fxx} )
  ADD_CUSTOM_COMMAND( OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${fxx}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${f} ${CMAKE_CURRENT_BINARY_DIR}/${fxx} 
    DEPENDS
    ${CMAKE_CURRENT_BINARY_DIR}/idd/IddEnums.hxx
    ${CMAKE_CURRENT_BINARY_DIR}/idd/IddFieldEnums.hxx
    ${CMAKE_CURRENT_BINARY_DIR}/idd/IddFactory.hxx
    ${CMAKE_CURRENT_BINARY_DIR}/idd/IddFactory.cxx 
    ${CMAKE_CURRENT_SOURCE_DIR}/${f}
  )
ENDFOREACH()
###############################################################################

## Qt MOC generation
qt4_wrap_cpp( bcl_moc_src ${bcl_moc} )
qt4_wrap_cpp( cloud_moc_src ${cloud_moc} )
qt4_wrap_cpp( core_moc_src ${core_moc} )
qt4_wrap_cpp( idf_moc_src ${idfxx_moc} )
qt4_wrap_cpp( data_moc_src ${data_moc} )
qt4_wrap_cpp( plot_moc_src ${plot_moc} )

# qrc files
SET( bcl_qrc
  bcl/bcl.qrc
)

# generate rules for building source files from the resources
qt4_add_resources(bcl_qrc_src ${bcl_qrc})
# set up groups of source files for Visual Studio
SOURCE_GROUP( bcl FILES ${bcl_src} )
SOURCE_GROUP( cloud FILES ${cloud_src} )
SOURCE_GROUP( core FILES ${core_src} )
SOURCE_GROUP( data FILES ${data_src} )
SOURCE_GROUP( document FILES ${document_src} )
SOURCE_GROUP( filetypes FILES ${filetypes_src} )
SOURCE_GROUP( idd FILES ${idd_src} )
SOURCE_GROUP( idf FILES ${idf_src} )
SOURCE_GROUP( sql FILES ${sql_src} )
SOURCE_GROUP( geometry FILES ${geometry_src} )
SOURCE_GROUP( math FILES ${math_src} )
SOURCE_GROUP( plot FILES ${plot_src} )
SOURCE_GROUP( time FILES ${time_src} )
SOURCE_GROUP( units FILES ${units_src} )
SOURCE_GROUP( documentation FILES ${documentation_src} )
SOURCE_GROUP( economics FILES ${economics_src} )


ADD_CUSTOM_COMMAND(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  COMMAND "${SWIG_EXECUTABLE}"
          "-ruby" 
          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
)


SET( ${target_name}_src
  UtilitiesAPI.hpp
  "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx" # referenced here to make sure it is generated when and where we expect
  ${bcl_src}
  ${bcl_moc_src}
  ${bcl_qrc_src}
  ${cloud_src}
  ${cloud_moc_src}
  ${core_src}  
  ${core_moc_src}
  ${data_src} 
  ${data_moc_src} 
  ${document_src}
  ${documentation_src} 
  ${economics_src}
  ${filetypes_src} 
  ${idd_src}
  ${idf_src}
  ${idf_moc_src}
  ${sql_src}
  ${geometry_src}
  ${math_src}
  ${plot_src}
  ${plot_moc_src}
  ${time_src} 
  ${units_src}
)

SET( ${target_name}_test_src
  ${idd_test_src}
  ${idf_test_src}
  ${idf_test_moc_src}
  ${sql_test_src}
  cloud/test/AWSProvider_GTest.cpp
  core/test/CoreFixture.hpp
  core/test/CoreFixture.cpp
  core/test/Checksum_GTest.cpp
  core/test/Compare_GTest.cpp
  core/test/Containers_GTest.cpp
  core/test/Enum_GTest.cpp
  core/test/EnumHelpers_GTest.cpp
  core/test/FileReference_GTest.cpp
  core/test/Finder_GTest.cpp
  core/test/Logger_GTest.cpp
  core/test/Optional_GTest.cpp
  core/test/Path_GTest.cpp
  core/test/PathWatcher_GTest.cpp  
  core/test/SharedFromThis_GTest.cpp
  core/test/System_GTest.cpp
  core/test/String_GTest.cpp

  core/test/UpdateManager_GTest.cpp
  core/test/UUID_GTest.cpp
  core/test/Zip_GTest.cpp
  data/Test/DataFixture.hpp
  data/Test/DataFixture.cpp
  data/Test/Attribute_GTest.cpp
  data/Test/CalibrationResult_GTest.cpp
  data/Test/EndUses_GTest.cpp
  data/Test/Matrix_GTest.cpp
  data/Test/TimeSeries_GTest.cpp
  data/Test/Vector_GTest.cpp
  document/test/DocumentFixture.hpp
  document/test/DocumentFixture.cpp
  document/test/Document_GTest.cpp
  document/test/DocumentRegex_GTest.cpp
  document/test/SectionHeading_GTest.cpp
  document/test/Table_GTest.cpp
  document/test/TableElement_GTest.cpp  
  document/test/Text_GTest.cpp
  economics/test/Economics_GTest.cpp
  filetypes/test/EpwFile_GTest.cpp
  filetypes/test/TimeDependentValuationFile_GTest.cpp
  geometry/Test/BoundingBox_GTest.cpp
  geometry/Test/GeometryFixture.hpp
  geometry/Test/GeometryFixture.cpp
  geometry/Test/Geometry_GTest.cpp
  geometry/Test/Plane_GTest.cpp
  geometry/Test/Transformation_GTest.cpp
  math/test/FloatCompare_GTest.cpp
  math/test/Permutation_GTest.cpp
  math/test/Primes_GTest.cpp
  plot/Test/AnnotatedTimeline_GTest.cpp
  plot/Test/BarChart_GTest.cpp
  plot/Test/FloodPlot_GTest.cpp
  plot/Test/LinePlot_GTest.cpp
  plot/Test/PieChart_GTest.cpp
  plot/Test/ProgressBar_GTest.cpp
  time/Test/Calendar_GTest.cpp
  time/Test/Date_GTest.cpp
  time/Test/DateTime_GTest.cpp
  time/Test/Time_GTest.cpp
  units/test/UnitsFixture.hpp
  units/test/UnitsFixture.cpp
  units/test/Scale_GTest.cpp
  units/test/ScaleFactory_GTest.cpp
  units/test/Unit_GTest.cpp
  units/test/SIUnit_GTest.cpp
  units/test/IPUnit_GTest.cpp
  units/test/BTUUnit_GTest.cpp
  units/test/CFMUnit_GTest.cpp
  units/test/CelsiusUnit_GTest.cpp
  units/test/FahrenheitUnit_GTest.cpp
  units/test/UnitFactory_GTest.cpp
  units/test/QuantityRegex_GTest.cpp
  units/test/Quantity_GTest.cpp
  units/test/OSQuantityVector_GTest.cpp
  units/test/QuantityFactory_GTest.cpp
  units/test/QuantityConverter_GTest.cpp
  units/test/IddUnits_GTest.cpp
  bcl/test/BCLFixture.hpp
  bcl/test/BCLFixture.cpp
  bcl/test/BCL_GTest.cpp
  bcl/test/BCLXML_GTest.cpp
  bcl/test/BCLComponent_GTest.cpp
  bcl/test/BCLFileReference_GTest.cpp
  bcl/test/BCLMeasure_GTest.cpp
  bcl/test/OnDemandGenerator_GTest.cpp
)

SET( ${target_name}_swig_src
  #  Utilities.i
  ${idd_swig_src}
  ${idf_swig_src}
  ${sql_swig_src}
  bcl/LocalBCL.i
  core/Application.i
  core/Checksum.i
  core/CommonImport.i
  core/CommonInclude.i
  core/Core.i
  core/Enum.i
  core/Exception.i
  core/Logger.i
  core/Path.i
  core/PathWatcher.i
  core/Qt.i
  core/Serialization.i
  core/Singleton.i
  core/System.i
  core/UpdateManager.i
  core/Url.i
  core/UUID.i
  core/UnzipFile.i
  core/ZipFile.i
  core/ruby/LanguageSpecific.i
  core/python/LanguageSpecific.i
  core/csharp/LanguageSpecific.i
  cloud/Cloud.i
  economics/Economics.i
  filetypes/Filetypes.i
  geometry/Geometry.i
  document/Document.i
  time/Time.i
  time/TimeImpl.i
  time/Date.i
  time/Calendar.i
  time/DateTime.i
  data/Data.i
  data/Attribute.i
  data/CalibrationResult.i
  data/EndUses.i
  data/Matrix.i
  data/TimeSeries.i
  data/Vector.i
  plot/Plot.i
  plot/AnnotatedTimeline.i
  plot/FloodPlot.i
  plot/LinePlot.i
  plot/Plot2D.i
  plot/ProgressBar.i
  units/Scale.i
  units/ScaleFactory.i
  units/Unit.i
  units/UnitFactory.i
  units/QuantityRegex.i
  units/Quantity.i
  units/QuantityFactory.i
  units/QuantityConverter.i
)

# make sure zconf.h can be found
include_directories(${CMAKE_BINARY_DIR}/src/zlib)

#include subdir for pch
add_subdirectory(pch)

ADD_LIBRARY( ${target_name} SHARED
  ${${target_name}_src}
)

AddPCH(${target_name})

IF( NOT APPLE )
INSTALL(TARGETS ${target_name}
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib )
ENDIF()

TARGET_LINK_LIBRARIES(${target_name} ${${target_name}_depends})
CREATE_SRC_GROUPS( "${${target_name}_test_src}" )
CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_depends}"  )

IF( BUILD_TESTING )
  ADD_DEPENDENCIES("${target_name}_tests" openstudio_energyplus_resources openstudio_standardsinterface_resources)
ENDIF()

CREATE_SRC_GROUPS( "${${target_name}_swig_src}" )

MAKE_SWIG_TARGET( OpenStudioUtilitiesCore utilitiescore "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesCore.i" "${${target_name}_swig_src}" ${target_name} "" )

MAKE_SWIG_TARGET( OpenStudioUtilitiesIdd utilitiesidd "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesIdd.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesCore )

MAKE_SWIG_TARGET( OpenStudioUtilitiesIdf utilitiesidf "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesIdf.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesIdd )

MAKE_SWIG_TARGET( OpenStudioUtilitiesBCL utilitiesbcl "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesBCL.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesIdf )
MAKE_SWIG_TARGET( OpenStudioUtilitiesCloud utilitiescloud "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesCloud.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesCore )
MAKE_SWIG_TARGET( OpenStudioUtilitiesData utilitiesdata "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesData.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesBCL )
MAKE_SWIG_TARGET( OpenStudioUtilitiesDocument utilitiesdocument "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesDocument.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesData )
MAKE_SWIG_TARGET( OpenStudioUtilitiesEconomics utilitieseconomics "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesEconomics.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesDocument )
MAKE_SWIG_TARGET( OpenStudioUtilitiesGeometry utilitiesgeometry "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesGeometry.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesEconomics )
MAKE_SWIG_TARGET( OpenStudioUtilitiesPlot utilitiesplot "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesPlot.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesGeometry )
MAKE_SWIG_TARGET( OpenStudioUtilitiesSql utilitiessql "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesSql.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesPlot )
MAKE_SWIG_TARGET( OpenStudioUtilitiesTime utilitiestime "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesTime.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesSql )
MAKE_SWIG_TARGET( OpenStudioUtilitiesUnits utilitiesunits "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesUnits.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesTime )
MAKE_SWIG_TARGET( OpenStudioUtilitiesFileTypes utilitiesfiletypes "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesFileTypes.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesUnits )

MAKE_SWIG_TARGET( OpenStudioUtilities utilities "${CMAKE_CURRENT_SOURCE_DIR}/Utilities.i" "${${target_name}_swig_src}" ${target_name} OpenStudioUtilitiesFileTypes )



