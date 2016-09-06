set(PAT_SHA "e63190d426bbbd615e3798419a83067039697939")

find_program(NPM_COMMAND npm)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}.zip"
  COMMAND ${CMAKE_COMMAND} "-D" "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}" "-D" "PAT_SHA=${PAT_SHA}" -P "${CMAKE_CURRENT_SOURCE_DIR}/DownloadPat.cmake"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/DownloadPat.cmake"
)

if( APPLE )
  set(PAT_PRODUCT_FILE "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}/tmp/ParametricAnalysisTool.app/Contents/MacOS/ParametricAnalysisTool")
elseif( WIN32 )
  set(PAT_PRODUCT_FILE "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}/tmp/ParametricAnalysisTool.exe")
else()
  set(PAT_PRODUCT_FILE "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}/tmp/ParametricAnalysisTool")
endif()

add_custom_command(OUTPUT "${PAT_PRODUCT_FILE}"
  COMMAND "${CMAKE_COMMAND}" "-D" "NPM_COMMAND=${NPM_COMMAND}" "-D" "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}" "-D" "PAT_SHA=${PAT_SHA}" -P "${CMAKE_CURRENT_SOURCE_DIR}/BuildPat.cmake"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}.zip"
)

add_custom_target(PAT ALL 
  "${CMAKE_COMMAND}" "-E" echo "Building PAT" 
  DEPENDS "${PAT_PRODUCT_FILE}"
)

if( APPLE )
install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}/tmp/ParametricAnalysisTool.app" DESTINATION .
  USE_SOURCE_PERMISSIONS 
  COMPONENT PAT
)
endif()

set(CPACK_COMPONENT_PAT_DISPLAY_NAME "Parametric Analysis Tool")

#TODO figure out what the package structure is like on windows and install that
