set(PAT_SHA "fb2c5c18b5c3eba21d13a20f0ed25b67f2fee30d")

find_program(NPM_COMMAND npm)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}.zip"
  COMMAND ${CMAKE_COMMAND} "-D" CMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" "-D" PAT_SHA=${PAT_SHA} -P "${CMAKE_CURRENT_SOURCE_DIR}/DownloadPat.cmake"
  DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/DownloadPat.cmake"
)

if( APPLE )
  set(PAT_PRODUCT_FILE "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT/tmp/ParametricAnalysisTool.app/Contents/MacOS/ParametricAnalysisTool")
elseif( WIN32 )
  set(PAT_PRODUCT_FILE "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT/tmp/pat/ParametricAnalysisTool.exe")
else()
  set(PAT_PRODUCT_FILE "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT/tmp/ParametricAnalysisTool")
endif()

configure_file(BuildPat.bat.in BuildPat.bat)

add_custom_command(OUTPUT "${PAT_PRODUCT_FILE}"
  COMMAND cmd /C "${CMAKE_CURRENT_BINARY_DIR}/BuildPat.bat"
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}.zip"
  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
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
else()
  install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT/tmp/pat" DESTINATION .
    USE_SOURCE_PERMISSIONS 
    COMPONENT PAT
  )
endif()

#TODO figure out what the package structure is like on windows and install that
