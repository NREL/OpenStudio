
CONFIGURE_FILE("makePackage.in" "${CMAKE_CURRENT_BINARY_DIR}/makePackage.R")
CONFIGURE_FILE("DESCRIPTION.in" "${CMAKE_CURRENT_BINARY_DIR}/DESCRIPTION")

SET( target_name R_OpenStudio )

SET( ${target_name}_src 
     OpenStudio-package.R
     OpenStudioAnalysis.R)

# Creates, builds, and tests OpenStudio R package.
# For some reason, package.skeleton FORCE and roxygenize overwrite are not 
# sufficient to get latest .R files copied over, so deleting by hand.
IF(WIN32)
  SET(R_OutputFile "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio_${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}.zip")
  ADD_CUSTOM_COMMAND(OUTPUT "${R_OutputFile}"
                    COMMAND "${CMAKE_COMMAND}" -E remove_directory "OpenStudio"
                    COMMAND "Rscript" "makePackage.R"
                    COMMAND "${CMAKE_COMMAND}" -E copy "DESCRIPTION" "OpenStudio/DESCRIPTION"
                    COMMAND "R" "CMD" "INSTALL" "--build" "OpenStudio"
                    COMMAND "R" "CMD" "check" "OpenStudio"
                    DEPENDS ${${target_name}_src})

ELSE()
  SET(R_OutputFile "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio_${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}.tar.gz")
  ADD_CUSTOM_COMMAND(OUTPUT "${R_OutputFile}"
                    COMMAND "${CMAKE_COMMAND}" -E remove_directory "OpenStudio"
                    COMMAND "R" "CMD" "BATCH" "makePackage.R"
                    COMMAND "${CMAKE_COMMAND}" -E copy "DESCRIPTION" "OpenStudio/DESCRIPTION"
                    COMMAND "R" "CMD" "build" "OpenStudio"
                    COMMAND "R" "CMD" "check" "OpenStudio"
                    DEPENDS ${${target_name}_src})
ENDIF()

ADD_CUSTOM_TARGET(${target_name} ALL
  DEPENDS "${R_OutputFile}")

