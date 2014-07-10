
configure_file("makePackage.in" "${CMAKE_CURRENT_BINARY_DIR}/makePackage.R")
configure_file("DESCRIPTION.in" "${CMAKE_CURRENT_BINARY_DIR}/DESCRIPTION")

set(target_name R_OpenStudio)

set(${target_name}_src 
  OpenStudio-package.R
  OpenStudioAnalysis.R
)

# Creates, builds, and tests OpenStudio R package.
# For some reason, package.skeleton FORCE and roxygenize overwrite are not 
# sufficient to get latest .R files copied over, so deleting by hand.
if(WIN32)
  set(R_OutputFile "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio_${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}.zip")
  add_custom_command(
    OUTPUT "${R_OutputFile}"
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "OpenStudio"
    COMMAND "Rscript" "makePackage.R"
    COMMAND "${CMAKE_COMMAND}" -E copy "DESCRIPTION" "OpenStudio/DESCRIPTION"
    COMMAND "R" "CMD" "INSTALL" "--build" "OpenStudio"
    COMMAND "R" "CMD" "check" "OpenStudio"
    DEPENDS ${${target_name}_src}
  )

else()
  set(R_OutputFile "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio_${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}.tar.gz")
  add_custom_command(
    OUTPUT "${R_OutputFile}"
    COMMAND "${CMAKE_COMMAND}" -E remove_directory "OpenStudio"
    COMMAND "R" "CMD" "BATCH" "makePackage.R"
    COMMAND "${CMAKE_COMMAND}" -E copy "DESCRIPTION" "OpenStudio/DESCRIPTION"
    COMMAND "R" "CMD" "build" "OpenStudio"
    COMMAND "R" "CMD" "check" "OpenStudio"
    DEPENDS ${${target_name}_src}
  )
endif()

add_custom_target(${target_name} ALL
  DEPENDS "${R_OutputFile}"
)
