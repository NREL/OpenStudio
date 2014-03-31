SET( target_name OpenStudio )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )
#INCLUDE_DIRECTORIES( ${RUBY_INCLUDE_DIR} )
#INCLUDE_DIRECTORIES( ${SWIG_DIR} )

#ADD_CUSTOM_COMMAND(
#  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.h"
#  COMMAND "${SWIG_EXECUTABLE}"
#          "-ruby" 
#          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.h"
#)

# source files
SET( ${target_name}_SRC
  main.cpp
  OpenStudioApp.cpp
  OpenStudioApp.hpp
  StartupView.hpp
  StartupView.cpp
  StartupMenu.hpp
  StartupMenu.cpp
)

# moc files
SET( ${target_name}_moc
  OpenStudioApp.hpp
  StartupView.hpp
  StartupMenu.hpp
)

## Qt UI generation
qt5_wrap_cpp( ${target_name}_moc_src ${${target_name}_moc} )

# configure the AboutBox.hpp file
# this needs to be configured to embed the build number
CONFIGURE_FILE(AboutBox.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AboutBox.hpp")


IF( WIN32 )
  SET(RC_BUILD_STR "0")
  IF(NOT CMAKE_VERSION_BUILD STREQUAL "" AND NOT CMAKE_VERSION_BUILD MATCHES "([a-zA-Z]+)" )
    SET(RC_BUILD_STR "${CMAKE_VERSION_BUILD}")
  ENDIF()
  CONFIGURE_FILE(OpenStudioApp.rc.in "${CMAKE_CURRENT_BINARY_DIR}/OpenStudioApp.rc")
  SET(ICON_SRC "${CMAKE_CURRENT_BINARY_DIR}/OpenStudioApp.rc")
ENDIF()
# For Apple set the icns file containing icons
IF(APPLE)
  # set the name of the bundle icon
  SET(MACOSX_BUNDLE_ICON_FILE os.icns) 
  # set all icons
  SET(ICON_SRC ../../icons/os.icns ../../icons/osm.icns ../../icons/osc.icns ../../icons/ossr.icns ../../icons/folder_icon.rsrc)
  SET_SOURCE_FILES_PROPERTIES(${ICON_SRC}
      PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  SET(MACOSX_BUNDLE_EXECUTABLE_NAME ${target_name})
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "gov.nrel.openstudio.openstudio")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "OpenStudio")
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
ENDIF(APPLE)

INCLUDE_DIRECTORIES( ${RUBY_INCLUDE_DIRS} )

ADD_EXECUTABLE( ${target_name}
  WIN32
  MACOSX_BUNDLE
  ${${target_name}_SRC}
  ${${target_name}_moc_src}
  ${${target_name}_moc}
  ${ICON_SRC}
)

SET( depends
  openstudio_lib
  openstudio_osversion
  openstudio_model
  openstudio_energyplus
  openstudio_modeleditor
  openstudio_sdd
  ${QT_LIBS}
  ${RUBY_LIBRARY}
)

TARGET_LINK_LIBRARIES( ${target_name}  ${depends} )
AddPCH(${target_name})

#------------------------------------------------------------------------------
# Installation
#------------------------------------------------------------------------------
IF(APPLE)
  SET_TARGET_PROPERTIES(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  SET(plugin_dest_dir OpenStudio.app/Contents/MacOS)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/OpenStudio.app")
ELSEIF(WIN32)
  SET(plugin_dest_dir bin)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/OpenStudio.exe")
ELSE()
  SET(plugin_dest_dir bin)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/OpenStudio")
ENDIF()

INSTALL(TARGETS OpenStudio 
    BUNDLE DESTINATION . 
    RUNTIME DESTINATION bin 
    )

if( APPLE OR WIN32 )

  get_target_property(QT_QSQLITE_PLUGIN Qt5::QSQLiteDriverPlugin LOCATION)

  install( PROGRAMS "${QT_QSQLITE_PLUGIN}"
           DESTINATION "${plugin_dest_dir}/sqldrivers/" 
         )

  get_filename_component(QSQLITE_PLUGIN_NAME "${QT_QSQLITE_PLUGIN}" NAME)

  set(QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/sqldrivers/${QSQLITE_PLUGIN_NAME}")

endif()

if( WIN32 )

  get_target_property(QT_WINDOWS_PLUGIN Qt5::QWindowsIntegrationPlugin LOCATION)

  install( PROGRAMS "${QT_WINDOWS_PLUGIN}"
           DESTINATION "${plugin_dest_dir}/platforms/" 
         )

  get_filename_component(QWINDOWS_PLUGIN_NAME "${QT_WINDOWS_PLUGIN}" NAME)

  list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/platforms/${QWINDOWS_PLUGIN_NAME}")

endif()


# directories to look for dependencies
SET(DIRS ${QT_LIBRARY_DIR} ${LIBRARY_SEARCH_DIRECTORY})

if (APPLE)
  #LIST(APPEND DIRS "${CMAKE_BINARY_DIR}/Products/OSAppConfigInstall")
  
  install( DIRECTORY ${CMAKE_SOURCE_DIR}/src/openstudio_app/Resources/
           DESTINATION OpenStudio.app/Contents/share/openstudio/OSApp
         )
elseif (WIN32)
  #LIST(APPEND DIRS "${CMAKE_BINARY_DIR}/Products/OSAppConfigInstall/Release" "${CMAKE_BINARY_DIR}/Products/OSAppConfigInstall/Debug")
  
  install( DIRECTORY ${CMAKE_SOURCE_DIR}/src/openstudio_app/Resources/
           DESTINATION share/openstudio/OSApp
         )
else()
  install( DIRECTORY ${CMAKE_SOURCE_DIR}/src/openstudio_app/Resources/
           DESTINATION share/openstudio/OSApp
         ) 
endif()

# Linux desktop integration
IF (UNIX AND NOT APPLE AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  # Install .desktop file
  CONFIGURE_FILE(OpenStudio.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/OpenStudio.desktop)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/OpenStudio.desktop DESTINATION ../share/applications)

  # Declare mime types
  INSTALL(FILES OpenStudio.xml DESTINATION ../share/mime/packages)

  # Install icons
  INSTALL(FILES ../../icons/os_32.png DESTINATION ../share/pixmaps RENAME OpenStudio.png)

  INSTALL(FILES ../../icons/os_16.png DESTINATION ../share/icons/hicolor/16x16/apps RENAME OpenStudio.png)
  INSTALL(FILES ../../icons/os_32.png DESTINATION ../share/icons/hicolor/32x32/apps RENAME OpenStudio.png)
  INSTALL(FILES ../../icons/os_48.png DESTINATION ../share/icons/hicolor/48x48/apps RENAME OpenStudio.png)
  INSTALL(FILES ../../icons/os_128.png DESTINATION ../share/icons/hicolor/128x128/apps RENAME OpenStudio.png)
  INSTALL(FILES ../../icons/os_256.png DESTINATION ../share/icons/hicolor/256x256/apps RENAME OpenStudio.png)

  INSTALL(FILES ../../icons/osm_16.png DESTINATION ../share/icons/hicolor/16x16/mimetypes RENAME application-openstudio.osm.png)
  INSTALL(FILES ../../icons/osm_32.png DESTINATION ../share/icons/hicolor/32x32/mimetypes RENAME application-openstudio.osm.png)
  INSTALL(FILES ../../icons/osm_48.png DESTINATION ../share/icons/hicolor/48x48/mimetypes RENAME application-openstudio.osm.png)
  INSTALL(FILES ../../icons/osm_128.png DESTINATION ../share/icons/hicolor/128x128/mimetypes RENAME application-openstudio.osm.png)
  INSTALL(FILES ../../icons/osm_256.png DESTINATION ../share/icons/hicolor/256x256/mimetypes RENAME application-openstudio.osm.png)

  INSTALL(FILES ../../icons/osc_16.png DESTINATION ../share/icons/hicolor/16x16/mimetypes RENAME application-openstudio.osc.png)
  INSTALL(FILES ../../icons/osc_32.png DESTINATION ../share/icons/hicolor/32x32/mimetypes RENAME application-openstudio.osc.png)
  INSTALL(FILES ../../icons/osc_48.png DESTINATION ../share/icons/hicolor/48x48/mimetypes RENAME application-openstudio.osc.png)
  INSTALL(FILES ../../icons/osc_128.png DESTINATION ../share/icons/hicolor/128x128/mimetypes RENAME application-openstudio.osc.png)
  INSTALL(FILES ../../icons/osc_256.png DESTINATION ../share/icons/hicolor/256x256/mimetypes RENAME application-openstudio.osc.png)

  INSTALL(FILES ../../icons/ossr_16.png DESTINATION ../share/icons/hicolor/16x16/mimetypes RENAME application-openstudio.ossr.png)
  INSTALL(FILES ../../icons/ossr_32.png DESTINATION ../share/icons/hicolor/32x32/mimetypes RENAME application-openstudio.ossr.png)
  INSTALL(FILES ../../icons/ossr_48.png DESTINATION ../share/icons/hicolor/48x48/mimetypes RENAME application-openstudio.ossr.png)
  INSTALL(FILES ../../icons/ossr_128.png DESTINATION ../share/icons/hicolor/128x128/mimetypes RENAME application-openstudio.ossr.png)
  INSTALL(FILES ../../icons/ossr_256.png DESTINATION ../share/icons/hicolor/256x256/mimetypes RENAME application-openstudio.ossr.png) 
ENDIF()

IF( WIN32 OR APPLE )
  INSTALL(CODE "
      include(BundleUtilities)
      fixup_bundle(\"${APPS}\" \"${QTPLUGINS}\" \"${DIRS}\")
      " )
ENDIF()

IF( WIN32 )
  include(GetPrerequisites)
    INSTALL(CODE "
    include(GetPrerequisites)
    GET_PREREQUISITES(\"${APPS}\" PREREQS 0 0 \"${APPS}\" \"${DIRS}\")
    FOREACH(PREREQ \${PREREQS})
      #message(\"\${PREREQ}\")
      IF( \"\${PREREQ}\" MATCHES \".*ruby.*\" )
        gp_resolve_item(\"${APPS}\" \"\${PREREQ}\" \"${APPS}\" \"${DIRS}\" FULL_PREREQ)
        execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy \"\${FULL_PREREQ}\" \"\${CMAKE_INSTALL_PREFIX}/bin/\")
      ENDIF()
    ENDFOREACH()
    " )
ENDIF()

# Due to duplication of the Apple Qt framework inside of the OpenStudio application
# we need to fix up and include the Ruby libraries inside of the application
# bundle if we want to be able to load them... if we try to load them from the
# Ruby install path, MacOS thinks we are loading two different sets of Qt dlls
# at the same time and it causes static initialization problems and a crash
IF( APPLE )
  INSTALL(CODE "

    file(GLOB_RECURSE PLUGINS
      \"${CMAKE_BINARY_DIR}/Products/ruby/openstudioutilities*.bundle\"
      \"${CMAKE_BINARY_DIR}/Products/ruby/openstudiomodel*.bundle\"
      \"${CMAKE_BINARY_DIR}/Products/ruby/openstudioenergyplus*.bundle\"
      \"${CMAKE_BINARY_DIR}/Products/ruby/openstudioruleset.bundle\")

    file( COPY \${PLUGINS} DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}\" )

    file(GLOB_RECURSE FIXUPPLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\"
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/*.bundle\" )

    include(BundleUtilities)

    fixup_bundle(\"${APPS}\" \"\${FIXUPPLUGINS}\" \"${DIRS}\")
  " )
ENDIF()


SET( ${target_name}_test_src
  test/OpenStudioAppFixture.hpp
  test/OpenStudioAppFixture.cpp
  test/Resources_GTest.cpp
)

SET (${target_name}_test_depends
  ${depends} 
)

CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}" )
CREATE_SRC_GROUPS( "${${target_name}_test_src}" )
IF( BUILD_TESTING )

  #ADD_DEPENDENCIES("${target_name}_tests" 
  #  openstudio_app_resources 
  #)
  
ENDIF()
