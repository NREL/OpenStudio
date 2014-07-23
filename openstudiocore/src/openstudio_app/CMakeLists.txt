set(target_name OpenStudio)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
#include_directories(${RUBY_INCLUDE_DIR})
#include_directories(${SWIG_DIR})

#add_custom_command(
#  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.h"
#  COMMAND "${SWIG_EXECUTABLE}"
#          "-ruby"
#          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.h"
#)

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

include_directories(${RUBY_INCLUDE_DIRS})

add_executable(${target_name}
  WIN32
  MACOSX_BUNDLE
  ${${target_name}_SRC}
  ${${target_name}_moc_src}
  ${${target_name}_moc}
  ${ICON_SRC}
)

set(depends
  openstudio_lib
  openstudio_osversion
  openstudio_model
  openstudio_energyplus
  openstudio_modeleditor
  openstudio_sdd
  openstudio_gbxml
  ${QT_LIBS}
  ${RUBY_LIBRARY}
)

target_link_libraries(${target_name} ${depends})
AddPCH(${target_name})

#------------------------------------------------------------------------------
# Installation
#------------------------------------------------------------------------------
if(APPLE)
  set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  set(plugin_dest_dir OpenStudio.app/Contents/MacOS)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/OpenStudio.app")
elseif(WIN32)
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/OpenStudio.exe")
else()
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/OpenStudio")
endif()

install(TARGETS OpenStudio
  BUNDLE DESTINATION .
  RUNTIME DESTINATION bin
)

if(APPLE OR WIN32)

  get_target_property(QT_QSQLITE_PLUGIN Qt5::QSQLiteDriverPlugin LOCATION)

  install(PROGRAMS "${QT_QSQLITE_PLUGIN}"
          DESTINATION "${plugin_dest_dir}/sqldrivers/"
  )

  get_filename_component(QSQLITE_PLUGIN_NAME "${QT_QSQLITE_PLUGIN}" NAME)

  set(QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/sqldrivers/${QSQLITE_PLUGIN_NAME}")

endif()

if(WIN32)

  get_target_property(QT_WINDOWS_PLUGIN Qt5::QWindowsIntegrationPlugin LOCATION)

  install(PROGRAMS "${QT_WINDOWS_PLUGIN}"
          DESTINATION "${plugin_dest_dir}/platforms/"
  )

  get_filename_component(QWINDOWS_PLUGIN_NAME "${QT_WINDOWS_PLUGIN}" NAME)

  list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/platforms/${QWINDOWS_PLUGIN_NAME}")

endif()


# directories to look for dependencies
set(DIRS ${QT_LIBRARY_DIR} ${LIBRARY_SEARCH_DIRECTORY})

if(APPLE)
  #list(APPEND DIRS "${CMAKE_BINARY_DIR}/Products/OSAppConfigInstall")

  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/openstudio_app/Resources/
          DESTINATION OpenStudio.app/Contents/share/openstudio-${OPENSTUDIO_VERSION}/OSApp
  )

  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/pat_app/Measures
          DESTINATION OpenStudio.app/Contents/share/openstudio-${OPENSTUDIO_VERSION}/pat
  )
elseif(WIN32)
  #list(APPEND DIRS "${CMAKE_BINARY_DIR}/Products/OSAppConfigInstall/Release" "${CMAKE_BINARY_DIR}/Products/OSAppConfigInstall/Debug")

  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/openstudio_app/Resources/
          DESTINATION share/openstudio-${OPENSTUDIO_VERSION}/OSApp
  )
else()
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/openstudio_app/Resources/
          DESTINATION share/openstudio-${OPENSTUDIO_VERSION}/OSApp
  )
endif()

# Linux desktop integration
if(UNIX AND NOT APPLE AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  # Install .desktop file
  configure_file(OpenStudio.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/OpenStudio.desktop)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/OpenStudio.desktop DESTINATION ../share/applications)

  # Declare mime types
  install(FILES OpenStudio.xml DESTINATION ../share/mime/packages)

  # Install icons
  install(FILES ../../icons/os_32.png DESTINATION ../share/pixmaps RENAME OpenStudio.png)

  install(FILES ../../icons/os_16.png DESTINATION ../share/icons/hicolor/16x16/apps RENAME OpenStudio.png)
  install(FILES ../../icons/os_32.png DESTINATION ../share/icons/hicolor/32x32/apps RENAME OpenStudio.png)
  install(FILES ../../icons/os_48.png DESTINATION ../share/icons/hicolor/48x48/apps RENAME OpenStudio.png)
  install(FILES ../../icons/os_128.png DESTINATION ../share/icons/hicolor/128x128/apps RENAME OpenStudio.png)
  install(FILES ../../icons/os_256.png DESTINATION ../share/icons/hicolor/256x256/apps RENAME OpenStudio.png)

  install(FILES ../../icons/osm_16.png DESTINATION ../share/icons/hicolor/16x16/mimetypes RENAME application-openstudio.osm.png)
  install(FILES ../../icons/osm_32.png DESTINATION ../share/icons/hicolor/32x32/mimetypes RENAME application-openstudio.osm.png)
  install(FILES ../../icons/osm_48.png DESTINATION ../share/icons/hicolor/48x48/mimetypes RENAME application-openstudio.osm.png)
  install(FILES ../../icons/osm_128.png DESTINATION ../share/icons/hicolor/128x128/mimetypes RENAME application-openstudio.osm.png)
  install(FILES ../../icons/osm_256.png DESTINATION ../share/icons/hicolor/256x256/mimetypes RENAME application-openstudio.osm.png)

  install(FILES ../../icons/osc_16.png DESTINATION ../share/icons/hicolor/16x16/mimetypes RENAME application-openstudio.osc.png)
  install(FILES ../../icons/osc_32.png DESTINATION ../share/icons/hicolor/32x32/mimetypes RENAME application-openstudio.osc.png)
  install(FILES ../../icons/osc_48.png DESTINATION ../share/icons/hicolor/48x48/mimetypes RENAME application-openstudio.osc.png)
  install(FILES ../../icons/osc_128.png DESTINATION ../share/icons/hicolor/128x128/mimetypes RENAME application-openstudio.osc.png)
  install(FILES ../../icons/osc_256.png DESTINATION ../share/icons/hicolor/256x256/mimetypes RENAME application-openstudio.osc.png)

  install(FILES ../../icons/ossr_16.png DESTINATION ../share/icons/hicolor/16x16/mimetypes RENAME application-openstudio.ossr.png)
  install(FILES ../../icons/ossr_32.png DESTINATION ../share/icons/hicolor/32x32/mimetypes RENAME application-openstudio.ossr.png)
  install(FILES ../../icons/ossr_48.png DESTINATION ../share/icons/hicolor/48x48/mimetypes RENAME application-openstudio.ossr.png)
  install(FILES ../../icons/ossr_128.png DESTINATION ../share/icons/hicolor/128x128/mimetypes RENAME application-openstudio.ossr.png)
  install(FILES ../../icons/ossr_256.png DESTINATION ../share/icons/hicolor/256x256/mimetypes RENAME application-openstudio.ossr.png)
endif()

if(WIN32 OR APPLE)
  install(CODE "
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"${QTPLUGINS}\" \"${DIRS}\")
  ")
endif()

if(WIN32)
  include(GetPrerequisites)
  install(CODE "
    include(GetPrerequisites)
    get_prerequisites(\"${APPS}\" PREREQS 0 0 \"${APPS}\" \"${DIRS}\")
    foreach(PREREQ \${PREREQS})
      #message(\"\${PREREQ}\")
      if(\"\${PREREQ}\" MATCHES \".*ruby.*\")
        gp_resolve_item(\"${APPS}\" \"\${PREREQ}\" \"${APPS}\" \"${DIRS}\" FULL_PREREQ)
        execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy \"\${FULL_PREREQ}\" \"\${CMAKE_INSTALL_PREFIX}/bin/\")
      endif()
    endforeach()
  ")
endif()

# Due to duplication of the Apple Qt framework inside of the OpenStudio application
# we need to fix up and include the Ruby libraries inside of the application
# bundle if we want to be able to load them... if we try to load them from the
# Ruby install path, MacOS thinks we are loading two different sets of Qt dlls
# at the same time and it causes static initialization problems and a crash
if(APPLE)
  install(CODE "

    file(GLOB_RECURSE PLUGINS
      \"${CMAKE_BINARY_DIR}/Products/ruby/openstudioutilities*.bundle\"
      \"${CMAKE_BINARY_DIR}/Products/ruby/openstudiomodel*.bundle\"
      \"${CMAKE_BINARY_DIR}/Products/ruby/openstudioenergyplus*.bundle\"
      \"${CMAKE_BINARY_DIR}/Products/ruby/openstudioruleset.bundle\")

    file(COPY \${PLUGINS} DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}\")

    file(GLOB_RECURSE FIXUPPLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\"
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/*.bundle\"
    )

    include(BundleUtilities)

    fixup_bundle(\"${APPS}\" \"\${FIXUPPLUGINS}\" \"${DIRS}\")
  ")
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
