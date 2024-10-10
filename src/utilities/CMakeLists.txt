# create a library out of the utilities namespace

set(target_name openstudio_utilities)

include(idd/CMakeLists.txt)
include(idf/CMakeLists.txt)
include(sql/CMakeLists.txt)
include(units/CMakeLists.txt)
include(xml/CMakeLists.txt)

CONFIGURE_FILE_WITH_CHECKSUM("core/ApplicationPathHelpers.cxx.in" "${CMAKE_CURRENT_BINARY_DIR}/core/ApplicationPathHelpers.cxx")

set(core_src
  core/ApplicationPathHelpers.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/core/ApplicationPathHelpers.cxx
  core/Assert.hpp
  core/Checksum.hpp
  core/Checksum.cpp
  core/CommandLine.hpp
  core/CommandLine.cpp
  core/Compare.hpp
  core/Compare.cpp
  core/ContainersMove.hpp
  core/Containers.hpp
  core/Containers.cpp
  core/Deprecated.hpp
  core/DeprecatedHelpers.hpp
  core/DynamicLibrary.hpp
  core/DynamicLibraryPOSIX.hpp
  core/DynamicLibraryWindows.hpp
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
  core/ThreadSafeDeque.hpp
  core/UUID.hpp
  core/UUID.cpp
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
  data/TimeSeries.hpp
  data/TimeSeries.cpp
  data/Variant.hpp
  data/Variant.cpp
  data/Vector.hpp
  data/Vector.cpp
)

set(filetypes_src
  filetypes/CSVFile.hpp
  filetypes/CSVFile_Impl.hpp
  filetypes/CSVFile.cpp
  filetypes/EpwFile.hpp
  filetypes/EpwFile.cpp
  filetypes/RunOptions.hpp
  filetypes/RunOptions_Impl.hpp
  filetypes/RunOptions.cpp
  filetypes/ForwardTranslatorOptions.hpp
  filetypes/ForwardTranslatorOptions_Impl.hpp
  filetypes/ForwardTranslatorOptions.cpp
  filetypes/StandardsJSON.hpp
  filetypes/StandardsJSON_Impl.hpp
  filetypes/StandardsJSON.cpp
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
  geometry/RoofGeometry.cpp
  geometry/RoofGeometry.hpp
  geometry/ThreeJS.hpp
  geometry/ThreeJS.cpp
  geometry/Transformation.hpp
  geometry/Transformation.cpp
  geometry/Vector3d.hpp
  geometry/Vector3d.cpp
  geometry/Polygon3d.hpp
  geometry/Polygon3d.cpp
  geometry/Polyhedron.hpp
  geometry/Polyhedron.cpp
  geometry/StandardShapes.hpp
  geometry/StandardShapes.cpp
  ../polypartition/polypartition.cpp
)

set(floorspace_src
  floorspace/FSModel.hpp
  floorspace/FSModel.cpp
  floorspace/FSVisitor.hpp
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
  bcl/BCLEnums.hpp
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

set(documentation_src
  mainpage.hpp
)

# embedded files, bump again to update common files
file(GLOB_RECURSE BCL_TEMPLATES  FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/bcl/templates/**/*.*")
foreach(_FILE ${BCL_TEMPLATES})
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/bcl/" ${_FILE})
  list(APPEND E_FILES ${_FILE})
  list(APPEND E_PATHS ${LOCATION})
endforeach()

file(GLOB_RECURSE IDD_FILES  FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/idd/versions/*/*.idd")
foreach(_FILE ${IDD_FILES})
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}" ${_FILE})
  list(APPEND E_FILES ${_FILE})
  list(APPEND E_PATHS ${LOCATION})
endforeach()

file(GLOB_RECURSE XLST_TRANSFORM_FILES  FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/xml/resources/*.xsl")
foreach(_FILE ${XLST_TRANSFORM_FILES})
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}" ${_FILE})
  list(APPEND E_FILES ${_FILE})
  list(APPEND E_PATHS ${LOCATION})
endforeach()

file(GLOB_RECURSE XSD_SCHEMA_FILES  FOLLOW_SYMLINKS "${CMAKE_CURRENT_SOURCE_DIR}/xml/resources/*.xsd")
foreach(_FILE ${XSD_SCHEMA_FILES})
  file(RELATIVE_PATH LOCATION "${CMAKE_CURRENT_SOURCE_DIR}" ${_FILE})
  list(APPEND E_FILES ${_FILE})
  list(APPEND E_PATHS ${LOCATION})
endforeach()

include("${PROJECT_SOURCE_DIR}/embedded/EmbedFiles.cmake")
embed_files("${E_FILES}" "${E_PATHS}" EMBEDDED_OUTPUT openstudio)

# set up groups of source files for Visual Studio
source_group(bcl FILES ${bcl_src})
source_group(core FILES ${core_src})
source_group(data FILES ${data_src})
source_group(filetypes FILES ${filetypes_src})
source_group(idd FILES ${idd_src})
source_group(idf FILES ${idf_src})
source_group(sql FILES ${sql_src})
source_group(xml FILES ${xml_src})
source_group(geometry FILES ${geometry_src})
source_group(floorspace FILES ${floorspace_src})
source_group(math FILES ${math_src})
source_group(plot FILES ${plot_src})
source_group(time FILES ${time_src})
source_group(units FILES ${units_src})
source_group(documentation FILES ${documentation_src})
source_group(embedded FILES ${EMBEDDED_OUTPUT})

# Must call CMake itself in order to set the SWIG_DIR env var for add_custom_command
add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
  COMMAND ${CMAKE_COMMAND} -E env SWIG_DIR="${SWIG_DIR}"
          "${SWIG_EXECUTABLE}"
          "-v"
          "-ruby"
          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx"
)

set(${target_name}_src
  UtilitiesAPI.hpp
  "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.hxx" # referenced here to make sure it is generated when and where we expect
  ${bcl_src}
  ${core_src}
  ${data_src}
  ${documentation_src}
  ${filetypes_src}
  ${idd_src}
  ${idf_src}
  ${sql_src}
  ${xml_src}
  ${geometry_src}
  ${floorspace_src}
  ${math_src}
  ${plot_src}
  ${time_src}
  ${units_src}
  ${EMBEDDED_OUTPUT}
)

set(${target_name}_test_src
  ${idd_test_src}
  ${idf_test_src}
  ${sql_test_src}
  ${xml_test_src}

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
  core/test/SharedFromThis_GTest.cpp
  core/test/System_GTest.cpp
  core/test/String_GTest.cpp
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

  filetypes/test/CSVFile_GTest.cpp
  filetypes/test/EpwFile_GTest.cpp
  filetypes/test/WorkflowJSON_GTest.cpp
  filetypes/test/StandardsJSON_GTest.cpp

  geometry/Test/BoundingBox_GTest.cpp
  geometry/Test/GeometryFixture.hpp
  geometry/Test/GeometryFixture.cpp
  geometry/Test/Geometry_GTest.cpp
  geometry/Test/Intersection_GTest.cpp
  geometry/Test/Plane_GTest.cpp
  geometry/Test/RoofGeometry_GTest.cpp
  geometry/Test/ThreeJS_GTest.cpp
  geometry/Test/FloorplanJS_GTest.cpp
  geometry/Test/Transformation_GTest.cpp
  geometry/Test/Polyhedron_GTest.cpp

  math/test/FloatCompare_GTest.cpp
  math/test/Permutation_GTest.cpp
  math/test/Primes_GTest.cpp

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
  ${PROJECT_BINARY_DIR}/src/OpenStudio.hxx
  ${idd_swig_src}
  ${idf_swig_src}
  ${sql_swig_src}
  bcl/LocalBCL.i
  core/Checksum.i
  core/CommonImport.i
  core/CommonInclude.i
  core/Core.i
  core/Enum.i
  core/Exception.i
  core/Logger.i
  core/Path.i
  core/Singleton.i
  core/System.i
  core/UUID.i
  core/UnzipFile.i
  core/ZipFile.i
  core/jsoncpp.i
  core/ruby/LanguageSpecific.i
  core/python/LanguageSpecific.i
  core/csharp/LanguageSpecific.i
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
  plot/ProgressBar.i
  units/Scale.i
  units/ScaleFactory.i
  units/Unit.i
  units/UnitFactory.i
  units/QuantityRegex.i
  units/Quantity.i
  units/QuantityFactory.i
  units/QuantityConverter.i
  xml/XMLValidator.i
)


set(${target_name}_depends
  SQLite::SQLite3
  JsonCpp::JsonCpp
  minizip::minizip
  ZLIB::ZLIB
  fmt::fmt
  pugixml::pugixml
  boost::boost
  cpprestsdk::cpprestsdk
  openssl::openssl
  GeographicLib::GeographicLib
  LibXml2::LibXml2
  libxslt::libxslt
)

if(WIN32)
  list(APPEND ${target_name}_depends mpr)
elseif(APPLE)
  list(APPEND ${target_name}_depends ${SECURITY_FRAMEWORK})
  # For cpprest
  list(APPEND ${target_name}_depends ${COREFOUNDATION_LIBRARY})
endif()

add_library(${target_name}
  OBJECT
  ${${target_name}_src}
)
add_dependencies(${target_name} GenerateIddFactoryRun)


set(${target_name}_depends ${${target_name}_depends} PARENT_SCOPE)

target_link_libraries(${target_name} PUBLIC ${${target_name}_depends})
target_compile_definitions(${target_name} PRIVATE openstudio_utilities_EXPORTS SHARED_OS_LIBS)
target_include_directories(${target_name} PUBLIC ${PROJECT_BINARY_DIR})
target_include_directories(${target_name} PUBLIC ${PROJECT_BINARY_DIR}/src)

include(CheckIncludeFile)
check_include_file("unistd.h" HAVE_UNISTD_H)
if(HAVE_UNISTD_H)
  target_compile_definitions(${target_name} PUBLIC "-DHAVE_UNISTD_H")
endif()

# To be used by the main cli program, this target
# is the most minimal thing that provides Path, and Filesystem
# capabilities, which the cli requires.
add_library(openstudio_utilities_minimal
  OBJECT
  core/ApplicationPathHelpers.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/core/ApplicationPathHelpers.cxx
  core/DynamicLibrary.hpp
  core/DynamicLibraryPOSIX.hpp
  core/DynamicLibraryWindows.hpp
  core/Filesystem.hpp
  core/Filesystem.cpp
  core/FilesystemHelpers.hpp
  core/FilesystemHelpers.cpp
  core/Logger.hpp
  core/Logger.cpp
  core/LogMessage.hpp
  core/LogMessage.cpp
  core/LogSink.hpp
  core/LogSink_Impl.hpp
  core/LogSink.cpp
  core/Path.hpp
  core/Path.cpp
  core/PathHelpers.hpp
  core/PathHelpers.cpp
  core/String.hpp
  core/String.cpp
  core/StringHelpers.hpp
  core/StringHelpers.cpp
  core/StringStreamLogSink.hpp
  core/StringStreamLogSink_Impl.hpp
  core/StringStreamLogSink.cpp
  bcl/BCLEnums.hpp
)
target_link_libraries(openstudio_utilities_minimal PUBLIC ${${target_name}_depends})

if(HAVE_UNISTD_H)
  target_compile_definitions(openstudio_utilities_minimal PUBLIC "-DHAVE_UNISTD_H")
endif()

if(BUILD_TESTING)
  CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" openstudiolib openstudio_model_resources openstudio_energyplus_resources fmt::fmt)
endif()

if(BUILD_BENCHMARK)

  set(core_benchmark_src
    core/benchmark/Checksum_Benchmark.cpp
    core/benchmark/Zip_Benchmark.cpp
  )
  set(${target_name}_benchmark_src
    ${core_benchmark_src}
    ${idf_benchmark_src}
    ${idd_benchmark_src}
  )

  foreach( bench_file ${${target_name}_benchmark_src} )
    get_filename_component(bench_name ${bench_file} NAME_WE)
    message("bench_name=${bench_name}")
    add_executable( ${bench_name} ${bench_file} )
    if (${bench_name} STREQUAL Checksum_Benchmark)
      target_link_libraries(${bench_name}
        PUBLIC
          benchmark::benchmark_main
          fmt::fmt
          boost::boost
      )
    else()
      target_link_libraries(${bench_name}
        PUBLIC
          benchmark::benchmark_main
          openstudiolib
          ${${target_name}_depends}
      )
    endif()
    set_target_properties(${bench_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/benchmark")
    add_dependencies(run_benchmarks ${bench_name})
  endforeach()

endif()


CREATE_SRC_GROUPS("${${target_name}_swig_src}")

set(swig_target_name ${target_name})

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
MAKE_SWIG_TARGET(OpenStudioUtilitiesXML utilitiesxml "${CMAKE_CURRENT_SOURCE_DIR}/UtilitiesXML.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesBCL)

MAKE_SWIG_TARGET(OpenStudioUtilities utilities "${CMAKE_CURRENT_SOURCE_DIR}/Utilities.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioUtilitiesFileTypes)
