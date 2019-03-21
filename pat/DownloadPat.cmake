
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}.zip)
  # Clean out old stuff
  file(GLOB OLD_PATS "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT*")
  foreach(OLD_PAT IN LISTS OLD_PATS)
    file(REMOVE_RECURSE "${OLD_PAT}")
  endforeach()

  message(STATUS "Downloading PAT ${PAT_SHA}")
  # Make github public and use a direct github url please
  #file(DOWNLOAD "https://github.com/NREL/OpenStudio-PAT/archive/${PAT_SHA}.zip"
  file(DOWNLOAD "https://codeload.github.com/NREL/OpenStudio-PAT/zip/${PAT_SHA}"
    "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}.zip"
    TIMEOUT 980 INACTIVITY_TIMEOUT 980 
    SHOW_PROGRESS
    STATUS DOWNLOAD_STATUS
  )

  list(GET DOWNLOAD_STATUS 0 STATUS)
  if( STATUS )
    message(FATAL_ERROR,"PAT source download failed")
  endif()
endif()
