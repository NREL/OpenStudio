# create a library out of the utilities namespace

set(target_name openstudio_utilities)

include_directories(${CMAKE_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR}/idd)

include(idd/CMakeLists.txt)
include(idf/CMakeLists.txt)
include(sql/CMakeLists.txt)
include(units/CMakeLists.txt)

CONFIGURE_FILE_WITH_CHECKSUM("core/ApplicationPathHelpers.cxx.in" "${CMAKE_CURRENT_BINARY_DIR}/core/ApplicationPathHelpers.cxx")

set(core_src
  core/ApplicationPathHelpers.hpp
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
  core/Deprecated.hpp
  core/Enum.hpp
  core/EnumHelpers.hpp
  core/Exception.hpp
  core/FileLogSink.hpp
  core/FileLogSink_Impl.hpp
  core/FileLogSink.cpp
  core/FileReference.hpp
  core/FileReference.cpp
  core/Filesystem.hpp
  core/Filesystem.cpp
  core/FilesystemHelpers.hpp
  core/FilesystemHelpers.cpp
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
  core/RubyInterpreter.hpp
  core/RubyException.hpp
  core/Singleton.hpp
  core/StaticInitializer.hpp
  core/String.hpp
  core/String.cpp
  core/StringHelpers.hpp
  core/StringHelpers.cpp
  core/StringStreamLogSink.hpp
  core/StringStreamLogSink_Impl.hpp
  core/StringStreamLogSink.cpp
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

set(data_src
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
  data/Variant.hpp
  data/Variant.cpp
  data/Vector.hpp
  data/Vector.cpp
)

set(filetypes_src
  filetypes/EpwFile.hpp
  filetypes/EpwFile.cpp
  filetypes/RunOptions.hpp
  filetypes/RunOptions_Impl.hpp
  filetypes/RunOptions.cpp
  filetypes/WorkflowJSON.hpp
  filetypes/WorkflowJSON_Impl.hpp
  filetypes/WorkflowJSON.cpp
  filetypes/WorkflowStep.hpp
  filetypes/WorkflowStep_Impl.hpp
  filetypes/WorkflowStep.cpp
  filetypes/WorkflowStepResult.hpp
  filetypes/WorkflowStepResult_Impl.hpp
  filetypes/WorkflowStepResult.cpp
)

set(geometry_src
  geometry/BoundingBox.hpp
  geometry/BoundingBox.cpp
  geometry/EulerAngles.hpp
  geometry/EulerAngles.cpp
  geometry/FloorplanJS.hpp
  geometry/FloorplanJS.cpp
  geometry/Geometry.hpp
  geometry/Geometry.cpp
  geometry/Intersection.hpp
  geometry/Intersection.cpp
  geometry/Plane.hpp
  geometry/Plane.cpp
  geometry/Point3d.hpp
  geometry/Point3d.cpp
  geometry/PointLatLon.hpp
  geometry/PointLatLon.cpp
  geometry/ThreeJS.hpp
  geometry/ThreeJS.cpp
  geometry/Transformation.hpp
  geometry/Transformation.cpp
  geometry/Vector3d.hpp
  geometry/Vector3d.cpp
  ../polypartition/polypartition.cpp
)

set(math_src
  math/FloatCompare.hpp
  math/Permutation.hpp
  math/Primes.hpp
)

set(plot_src
  plot/ProgressBar.hpp
  plot/ProgressBar.cpp
)

set(time_src
  time/Calendar.hpp
  time/Calendar.cpp
  time/Date.hpp
  time/Date.cpp
  time/DateTime.hpp
  time/DateTime.cpp
  time/Time.hpp
  time/Time.cpp
)

set(bcl_src
  bcl/BCL.hpp
  bcl/BCL.cpp
  bcl/BCLComponent.hpp
  bcl/BCLComponent.cpp
  bcl/BCLFileReference.hpp
  bcl/BCLFileReference.cpp
  bcl/BCLMeasure.hpp
  bcl/BCLMeasure.cpp
  bcl/BCLMeasureArgument.hpp
  bcl/BCLMeasureArgument.cpp
  bcl/BCLMeasureOutput.hpp
  bcl/BCLMeasureOutput.cpp
  bcl/BCLXML.hpp
  bcl/BCLXML.cpp
  bcl/LocalBCL.hpp
  bcl/LocalBCL.cpp
  bcl/RemoteBCL.hpp
  bcl/RemoteBCL.cpp
)

#set(cloud_src
#  cloud/AWSProvider.hpp
#  cloud/AWSProvider_Impl.hpp
#  cloud/AWSProvider.cpp
#  cloud/CloudProvider.hpp
#  cloud/CloudProvider_Impl.hpp
#  cloud/CloudProvider.cpp
#  cloud/OSServer.hpp
#  cloud/OSServer_Impl.hpp
#  cloud/OSServer.cpp
#  cloud/VagrantProvider.hpp
#  cloud/VagrantProvider_Impl.hpp
#  cloud/VagrantProvider.cpp
#)

set(documentation_src
  mainpage.hpp
)

set(${target_name}_depends
  sqlite
  jsoncpp
  #litesql
  #litesql-util
  miniziplib
  ${Boost_LIBRARIES}
  #JWD: On Ubuntu 14.04, the thread lib is included in Boost_LIBRARIES
  # ${CMAKE_THREAD_LIBS}
)

if(WIN32)
  list(APPEND ${target_name}_depends qtwinmigrate)
  list(APPEND ${target_name}_depends mpr)
endif()

# moc files
set(bcl_moc
  bcl/BCL.hpp
  bcl/LocalBCL.hpp
  bcl/RemoteBCL.hpp
)

#set(cloud_moc
#  cloud/AWSProvider_Impl.hpp
#  cloud/CloudProvider_Impl.hpp
#  cloud/OSServer_Impl.hpp
#  cloud/VagrantProvider_Impl.hpp
#)

set(core_moc
  core/PathWatcher.hpp
  core/UpdateManager.hpp
)

set(idf_moc
#  idf/IdfObject_Impl.hpp
#  idf/IdfObjectWatcher.hpp
#  idf/WorkspaceObjectWatcher.hpp
#  idf/WorkspaceObject_Impl.hpp
#  idf/Workspace_Impl.hpp
#  idf/WorkspaceWatcher.hpp
)

set(data_moc
#  data/Attribute_Impl.hpp
)

set(plot_moc
#  plot/ProgressBar.hpp
)

###############################################################################
# idf_moc files include generated source code.  It is important that these
# generated files are generated before moc runs.
# The following commands make sure that happens.
# This could be done by simply touching the moc'ed files in the source tree
# but in the spirit of keeping the source tree clean, we copy them into the
# build directory instead.
#
# DLM: This is no longer needed as GenerateIddFactoryRun will run before anything in utilities is built
#
###############################################################################
#foreach(f IN LISTS idf_moc)
#  string(REGEX REPLACE hpp$ hxx fxx ${f})
#  list(APPEND idfxx_moc ${CMAKE_CURRENT_BINARY_DIR}/${fxx})
#  add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${fxx}
#    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/${f} ${CMAKE_CURRENT_BINARY_DIR}/${fxx}
#    DEPENDS
#    ${CMAKE_CURRENT_BINARY_DIR}/idd/IddEnums.hxx
#    ${CMAKE_CURRENT_BINARY_DIR}/idd/IddFieldEnums.hxx
#    ${CMAKE_CURRENT_BINARY_DIR}/idd/IddFactory.hxx
#    ${CMAKE_CURRENT_BINARY_DIR}/idd/IddFactory.cxx
#    ${CMAKE_CURRENT_SOURCE_DIR}/${f}
#  )
#endforeach()
###############################################################################

## Qt MOC generation
qt5_wrap_cpp_minimally(bcl_moc_src ${bcl_moc})
#qt5_wrap_cpp_minimally(cloud_moc_src ${cloud_moc})
qt5_wrap_cpp_minimally(core_moc_src ${core_moc}) # RM
qt5_wrap_cpp_minimally(idf_moc_src ${idfxx_moc})
qt5_wrap_cpp_minimally(data_moc_src ${data_moc}) # RM
qt5_wrap_cpp_minimally(plot_moc_src ${plot_moc}) # [sudo] RM [-rf]


# embedded files, bump again to update common files
file(GLOB_RECURSE BCL_TEMPLATES  FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/bcl/templates/**/*.*")
foreach( _FILE ${BCL_TEMPLATES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/bcl/" ${_FILE})
  list(APPEND E_FILES ${_FILE})
  list(APPEND E_PATHS ${LOCATION})
endforeach()

file(GLOB_RECURSE IDD_FILES  FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/idd/versions/*/*.idd")
foreach( _FILE ${IDD_FILES} )
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}" ${_FILE})
  list(APPEND E_FILES ${_FILE})
  list(APPEND E_PATHS ${LOCATION})
endforeach()

include("${CMAKE_SOURCE_DIR}/embedded/EmbedFiles.cmake")
embed_files("${E_FILES}" "${E_PATHS}" EMBEDDED_OUTPUT openstudio)

# set up groups of source files for Visual Studio
source_group(bcl FILES ${bcl_src})
#source_group(cloud FILES ${cloud_src})
source_group(core FILES ${core_src})
source_group(data FILES ${data_src})
source_group(filetypes FILES ${filetypes_src})
source_group(idd FILES ${idd_src})
source_group(idf FILES ${idf_src})
source_group(sql FILES ${sql_src})
source_group(geometry FILES ${geometry_src})
source_group(math FILES ${math_src})
source_group(plot FILES ${plot_src})
source_group(time FILES ${time_src})
source_group(units FILES ${units_src})
source_group(documentation FILES ${documentation_src})
source_group(embedded FILES ${EMBEDDED_OUTPUT})

if(UNIX)
  set(SWIG_DEPENDS "SWIG")
else()
  set(SWIG_DEPENDS "")
endif()


add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  COMMAND "${SWIG_EXECUTABLE}"
          "-ruby"
          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  DEPENDS "${SWIG_DEPENDS}"
)

set(${target_name}_src
  UtilitiesAPI.hpp
  "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx" # referenced here to make sure it is generated when and where we expect
  ${bcl_src}
  ${bcl_moc_src}
#  ${cloud_src}
#  ${cloud_moc_src}
  ${core_src}
  ${core_moc_src}
  ${data_src}
  ${data_moc_src}
  ${documentation_src}
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
  ${EMBEDDED_OUTPUT}
)

set(${target_name}_test_src
  ${idd_test_src}
  ${idf_test_src}
  ${idf_test_moc_src}
  ${sql_test_src}

#  cloud/test/AWSProvider_GTest.cpp
#  cloud/test/VagrantProvider_GTest.cpp

  core/test/CoreFixture.hpp
  core/test/CoreFixture.cpp
  core/test/ApplicationPathHelpers_GTest.cpp
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
  data/Test/Variant_GTest.cpp
  data/Test/Vector_GTest.cpp

  filetypes/test/EpwFile_GTest.cpp
  filetypes/test/WorkflowJSON_GTest.cpp

  geometry/Test/BoundingBox_GTest.cpp
  geometry/Test/GeometryFixture.hpp
  geometry/Test/GeometryFixture.cpp
  geometry/Test/Geometry_GTest.cpp
  geometry/Test/Intersection_GTest.cpp
  geometry/Test/Plane_GTest.cpp
  geometry/Test/ThreeJS_GTest.cpp
  geometry/Test/FloorplanJS_GTest.cpp
  geometry/Test/Transformation_GTest.cpp

  math/test/FloatCompare_GTest.cpp
  math/test/Permutation_GTest.cpp
  math/test/Primes_GTest.cpp

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
)

set(${target_name}_swig_src
  #  Utilities.i
  ${CMAKE_BINARY_DIR}/src/OpenStudio.hxx
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
#  cloud/Cloud.i
  filetypes/Filetypes.i
  geometry/Geometry.i
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
  data/Variant.i
  data/Vector.i
  plot/Plot.i
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

#include subdir for pch
add_subdirectory(pch)

add_library(${target_name}
  ${${target_name}_src}
)
add_dependencies("${target_name}" GenerateIddFactoryRun)

if(UNIX)
  add_dependencies("${target_name}" SWIG)
endif()

AddPCH(${target_name})

# No need to install static lib
#if(NOT APPLE)
#  install(TARGETS ${target_name}
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#  )
#endif()

target_link_libraries(${target_name} PUBLIC ${${target_name}_depends} GeographicLib_STATIC ${QT_LIBS})

CREATE_SRC_GROUPS("${${target_name}_test_src}")
CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_depends}")

set(${target_name}_static_depends
  sqlite
  jsoncpp
  miniziplib
  ${Boost_LIBRARIES}
)

if(WIN32)
  list(APPEND ${target_name}_static_depends qtwinmigrate_static)
  list(APPEND ${target_name}_static_depends mpr)
endif()

add_library(${target_name}_static
  ${${target_name}_src}
)
add_dependencies("${target_name}_static" GenerateIddFactoryRun)

target_link_libraries(${target_name}_static ${${target_name}_static_depends} GeographicLib_STATIC)

target_include_directories(${target_name}_static PUBLIC ${QT_STATIC_INCLUDES})

list(LENGTH QT_STATIC_LIBS QT_STATIC_LIBS_LENGTH)
list(LENGTH QT_STATIC_LIBS_D QT_STATIC_LIBS_D_LENGTH)
if (NOT ${QT_STATIC_LIBS_LENGTH} EQUAL ${QT_STATIC_LIBS_D_LENGTH})
  message(FATAL_ERROR "Mismatched sizes QT_STATIC_LIBS ${QT_STATIC_LIBS_LENGTH} != QT_STATIC_LIBS_D #{QT_STATIC_LIBS_D_LENGTH}")
endif()

math(EXPR N "${QT_STATIC_LIBS_LENGTH} - 1")
foreach(i RANGE ${N})
  list(GET QT_STATIC_LIBS ${i} lib_)
  list(GET QT_STATIC_LIBS_D ${i} libd)
  target_link_libraries(${target_name}_static optimized ${lib_} debug ${libd})
endforeach()

target_compile_definitions(${target_name}_static PUBLIC ${QT_DEFS})

if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(${target_name} PUBLIC openstudio_utilities_EXPORTS)
  target_compile_definitions(${target_name}_static PUBLIC openstudio_utilities_EXPORTS)
endif()

if(BUILD_TESTING)
  add_dependencies("${target_name}_tests" openstudio_energyplus_resources)
endif()

CREATE_SRC_GROUPS("${${target_name}_swig_src}")

set(swig_target_name ${target_name}_static)

MAKE_SWIG_TARGET(OpenStudioUtilitiesCore utilitiescore "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesCore.i" "${${target_name}_swig_src}" ${swig_target_name} "")

MAKE_SWIG_TARGET(OpenStudioUtilitiesIdd utilitiesidd "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesIdd.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesCore)

MAKE_SWIG_TARGET(OpenStudioUtilitiesIdf utilitiesidf "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesIdf.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesIdd)

MAKE_SWIG_TARGET(OpenStudioUtilitiesBCL utilitiesbcl "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesBCL.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesIdf)
#MAKE_SWIG_TARGET(OpenStudioUtilitiesCloud utilitiescloud "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesCloud.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesCore)
MAKE_SWIG_TARGET(OpenStudioUtilitiesData utilitiesdata "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesData.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesBCL)
MAKE_SWIG_TARGET(OpenStudioUtilitiesGeometry utilitiesgeometry "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesGeometry.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesCore)
MAKE_SWIG_TARGET(OpenStudioUtilitiesPlot utilitiesplot "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesPlot.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesGeometry)
MAKE_SWIG_TARGET(OpenStudioUtilitiesSql utilitiessql "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesSql.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesPlot)
MAKE_SWIG_TARGET(OpenStudioUtilitiesTime utilitiestime "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesTime.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesSql)
MAKE_SWIG_TARGET(OpenStudioUtilitiesUnits utilitiesunits "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesUnits.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesTime)
MAKE_SWIG_TARGET(OpenStudioUtilitiesFileTypes utilitiesfiletypes "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesFileTypes.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesUnits)

MAKE_SWIG_TARGET(OpenStudioUtilities utilities "${CMAKE_CURRENT_SOURCE_DIR}/Utilities.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesFileTypes)
