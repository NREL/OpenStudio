set(target_name OpenStudioApp)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# source files
set(${target_name}_SRC
  main.cpp
  OpenStudioApp.cpp
  OpenStudioApp.hpp
  StartupView.hpp
  StartupView.cpp
  StartupMenu.hpp
  StartupMenu.cpp

  ../shared_gui_components/BusyWidget.cpp
  ../shared_gui_components/BusyWidget.hpp
  ../shared_gui_components/OSDialog.cpp
  ../shared_gui_components/OSDialog.hpp
  ../shared_gui_components/WaitDialog.cpp
  ../shared_gui_components/WaitDialog.hpp
)

# moc files
set(${target_name}_moc
  OpenStudioApp.hpp
  StartupView.hpp
  StartupMenu.hpp

  ../shared_gui_components/BusyWidget.hpp
  ../shared_gui_components/OSDialog.hpp
  ../shared_gui_components/WaitDialog.hpp
)

## Qt UI generation
qt5_wrap_cpp_minimally(${target_name}_moc_src ${${target_name}_moc})

# configure the AboutBox.hpp file
# this needs to be configured to embed the build number
configure_file(AboutBox.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AboutBox.hpp")


if(WIN32)
  set(RC_BUILD_STR "0")
  if(NOT CMAKE_VERSION_BUILD STREQUAL "" AND NOT CMAKE_VERSION_BUILD MATCHES "([a-zA-Z]+)")
    set(RC_BUILD_STR "${CMAKE_VERSION_BUILD}")
  endif()
  configure_file(OpenStudioApp.rc.in "${CMAKE_CURRENT_BINARY_DIR}/OpenStudioApp.rc")
  set(ICON_SRC "${CMAKE_CURRENT_BINARY_DIR}/OpenStudioApp.rc")
endif()
# For Apple set the icns file containing icons
if(APPLE)
  # set the name of the bundle icon
  set(MACOSX_BUNDLE_ICON_FILE os.icns)
  # set all icons
  set(ICON_SRC ../../icons/os.icns ../../icons/osm.icns ../../icons/osc.icns ../../icons/ossr.icns ../../icons/folder_icon.rsrc)
  set_source_files_properties(${ICON_SRC}
    PROPERTIES MACOSX_PACKAGE_LOCATION Resources
  )
  set(MACOSX_BUNDLE_EXECUTABLE_NAME ${target_name})
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "gov.nrel.openstudio.${CMAKE_VERSION_BUILD}.openstudio")
  set(MACOSX_BUNDLE_BUNDLE_NAME "OpenStudio")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
endif()

#include_directories(${RUBY_INCLUDE_DIRS})

add_executable(${target_name}
  WIN32
  MACOSX_BUNDLE
  ${${target_name}_SRC}
  ${${target_name}_moc_src}
  ${${target_name}_moc}
  ${ICON_SRC}
)

# This is because Apple has app bundles for .apps.
# We copy the openstudio cli into the app bundle in an effort
# to be self contained. It also makes it easy to find the cli since
# it will be sitting next to the app executable.
if( APPLE )
add_custom_command(TARGET ${target_name}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:openstudio> $<TARGET_FILE_DIR:${target_name}>/$<TARGET_FILE_NAME:openstudio>
)
endif()

if( NOT Qt5Static )
  add_custom_command(TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${target_name}>/platforms/
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${target_name}>/sqldrivers/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QSQLiteDriverPlugin> $<TARGET_FILE_DIR:${target_name}>/sqldrivers/
  )
  
  if( APPLE )
    add_custom_command(TARGET ${target_name}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QCocoaIntegrationPlugin> $<TARGET_FILE_DIR:${target_name}>/platforms/ )
  elseif( WIN32 )
    add_custom_command(TARGET ${target_name}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:${target_name}>/platforms/ )
    
    foreach(LIB ${QT_LIBS})
      add_custom_command(TARGET ${target_name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${LIB}> $<TARGET_FILE_DIR:${target_name}>/ )
    endforeach()
    
    foreach(LIB ${QT_WEB_LIBS})
      add_custom_command(TARGET ${target_name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${LIB}> $<TARGET_FILE_DIR:${target_name}>/ )
    endforeach()
  endif()
endif()

set(depends
  openstudio_lib
  openstudio_osversion
  openstudio_model
  openstudio_energyplus
  openstudio_modeleditor
  openstudio_sdd
  openstudio_gbxml
  openstudio_bimserver
  ${QT_WEB_LIBS}
  ${QT_PLUGIN_LIBS}
  ${COREFOUNDATION_LIBRARY}
)

target_link_libraries(${target_name} ${depends})
if( Qt5Static )
  target_link_libraries(${target_name} ${QT_SHARED_LIBS})
else()
  target_link_libraries(${target_name} ${QT_LIBS})
endif()

# because the app calls the cli
add_dependencies(${target_name} openstudio)

AddPCH(${target_name})

#------------------------------------------------------------------------------
# Installation
#------------------------------------------------------------------------------
if(APPLE)
  set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  set(plugin_dest_dir OpenStudio.app/Contents/MacOS)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/OpenStudioApp.app")
elseif(WIN32)
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/OpenStudioApp.exe")
else()
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/OpenStudioApp")
endif()

if( APPLE )
  install(TARGETS ${target_name}
    DESTINATION .
    COMPONENT "OpenStudioApp"
  )
else()
  install(TARGETS ${target_name}
    DESTINATION bin
    COMPONENT "OpenStudioApp"
  )
endif()

# directories to look for dependencies
# This is probably out of date
set(DIRS "${QT_INSTALL_DIR}/bin" "${QT_INSTALL_DIR}/lib" "${RUBY_INSTALL_DIR}/bin")

if(APPLE)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/openstudio_app/Resources/
          DESTINATION OpenStudioApp.app/Contents/Resources
          COMPONENT "Resources"
  )
elseif(WIN32)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/openstudio_app/Resources/
    DESTINATION Resources
    COMPONENT "Resources"
  )
else()
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/openstudio_app/Resources/
	  DESTINATION Resources
    COMPONENT "Resources"
  )
endif()

if(WIN32 OR APPLE)
  install(CODE "
    include(BundleUtilities)
    include(\"${CMAKE_SOURCE_DIR}/CMake/FixupBundleOverrides.cmake\")
    fixup_bundle(\"${APPS}\" \"${QTPLUGINS}\" \"${DIRS}\")
    " COMPONENT "OpenStudioApp" )
endif()

set(${target_name}_test_src
  test/OpenStudioAppFixture.hpp
  test/OpenStudioAppFixture.cpp
  test/Resources_GTest.cpp
)

set(${target_name}_test_depends
  ${depends}
)

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
if(BUILD_TESTING)

  #add_dependencies("${target_name}_tests"
  #  openstudio_app_resources
  #)

endif()
