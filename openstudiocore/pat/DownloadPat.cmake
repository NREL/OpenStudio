
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}.zip)
  # Clean out old stuff
  file(GLOB OLD_PATS "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT*")
  foreach(OLD_PAT IN LISTS OLD_PATS)
    file(REMOVE_RECURSE "${OLD_PAT}")
  endforeach()

  # Make github public and use a direct github url please
  # https://github.com/NREL/OpenStudio-PAT/archive/e63190d426bbbd615e3798419a83067039697939.zip
  file(DOWNLOAD "http://openstudio-resources.s3.amazonaws.com/dependencies/OpenStudio-PAT-${PAT_SHA}.zip"
    "${CMAKE_CURRENT_BINARY_DIR}/OpenStudio-PAT-${PAT_SHA}.zip"
    TIMEOUT 120 INACTIVITY_TIMEOUT 120 
    SHOW_PROGRESS
    STATUS DOWNLOAD_STATUS
  )

  list(GET DOWNLOAD_STATUS 0 STATUS) 
  if( STATUS ) 
    message(FATAL_ERROR,"PAT source download failed")
  endif()
endif()
