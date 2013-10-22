SET( target_name RunManager )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

# source files
SET( ${target_name}_SRC
  main.cpp
  MainWindow.cpp
  MainWindow.hpp
  #  AdvancedFileDialog.cpp
  #AdvancedFileDialog.hpp
  FileSystemSearch.cpp
  FileSystemSearch.hpp
)

# ui files
SET( ${target_name}_UI
  MainWindow.ui
  #AdvancedFileDialog.ui
)

# moc files
SET( ${target_name}_moc
  MainWindow.hpp
  #AdvancedFileDialog.hpp
  FileSystemSearch.hpp
)

# resource files
SET( ${target_name}_qrc
  RunManagerUI.qrc
)

## Qt UI generation
qt4_wrap_cpp( ${target_name}_moc_src ${${target_name}_moc} )

qt4_wrap_ui( ${target_name}_UIS ${${target_name}_UI} )

# generate rules for building source files from the resources
qt4_add_resources(${target_name}_qrcs ${${target_name}_qrc} )

# configure the AboutBox.hpp file
# this needs to be configured to embed the build number
CONFIGURE_FILE(AboutBox.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AboutBox.hpp")

#--------------------------------------------------------------------------------
# For Windows set the rc file pointing to the icon file
IF( WIN32 )
  SET(RC_BUILD_STR "0")
  IF( NOT CMAKE_VERSION_BUILD STREQUAL "" AND NOT CMAKE_VERSION_BUILD MATCHES "([a-zA-Z]+)" )
    SET(RC_BUILD_STR "${CMAKE_VERSION_BUILD}")
  ENDIF()
  CONFIGURE_FILE(RunManagerUI.rc.in "${CMAKE_CURRENT_BINARY_DIR}/RunManagerUI.rc")
  SET(ICON_SRC "${CMAKE_CURRENT_BINARY_DIR}/RunManagerUI.rc")
ENDIF()
# For Apple set the icns file containing icons
IF(APPLE)
  # set the name of the bundle icon
  SET(MACOSX_BUNDLE_ICON_FILE rm.icns)
  # set all icons
  SET(ICON_SRC ../../../icons/rm.icns)
  SET_SOURCE_FILES_PROPERTIES(${ICON_SRC}
      PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  SET(MACOSX_BUNDLE_EXECUTABLE_NAME ${target_name})
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "gov.nrel.openstudio.runmanager")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "RunManager")
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
ENDIF(APPLE)

ADD_EXECUTABLE( ${target_name}
  WIN32
  MACOSX_BUNDLE
  ${${target_name}_SRC}
  ${${target_name}_UI}
  ${${target_name}_UIS}
  ${${target_name}_moc_src}
  ${${target_name}_moc}
  ${${target_name}_UIS}
  ${${target_name}_qrcs}
  ${ICON_SRC}
)

AddPCH(${target_name})


SET( depends
  openstudio_runmanager
)

TARGET_LINK_LIBRARIES( ${target_name}  ${depends} )

#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
SET(plugin_dest_dir bin)
SET(qtconf_dest_dir bin)
SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/RunManager")
IF(APPLE)
  SET_TARGET_PROPERTIES(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  SET(plugin_dest_dir RunManager.app/Contents/MacOS)
  SET(qtconf_dest_dir RunManager.app/Contents/Resources)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/RunManager.app")
ENDIF(APPLE)
IF(WIN32)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/RunManager.exe")
ENDIF(WIN32)

#--------------------------------------------------------------------------------
# Install the QtTest application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
INSTALL(TARGETS RunManager
    BUNDLE DESTINATION .
    RUNTIME DESTINATION bin
    )


#--------------------------------------------------------------------------------
# Install needed Qt plugins by copying directories from the qt installation
# One can cull what gets copied by using 'REGEX "..." EXCLUDE'
#INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${plugin_dest_dir}/plugins )

#--------------------------------------------------------------------------------
# install a qt.conf file
# this inserts some cmake code into the install script to write the file
IF( APPLE )
  INSTALL(CODE "
      file(COPY \"${QT_LIBRARY_DIR}/QtGui.framework/Resources/qt_menu.nib\" 
      DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/\") 
      " )
ENDIF()

IF( APPLE OR WIN32 )
  INSTALL(CODE "
      file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
      " )
ENDIF()

# Linux desktop integration
IF (UNIX AND NOT APPLE AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  # Install .desktop file
  CONFIGURE_FILE(RunManager.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/RunManager.desktop)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/RunManager.desktop DESTINATION ../share/applications)

  # Install icons
  INSTALL(FILES ../../../icons/rm_32.png DESTINATION ../share/pixmaps RENAME RunManager.png)

  INSTALL(FILES ../../../icons/rm_16.png DESTINATION ../share/icons/hicolor/16x16/apps RENAME RunManager.png)
  INSTALL(FILES ../../../icons/rm_32.png DESTINATION ../share/icons/hicolor/32x32/apps RENAME RunManager.png)
  INSTALL(FILES ../../../icons/rm_48.png DESTINATION ../share/icons/hicolor/48x48/apps RENAME RunManager.png)
  INSTALL(FILES ../../../icons/rm_128.png DESTINATION ../share/icons/hicolor/128x128/apps RENAME RunManager.png)
  INSTALL(FILES ../../../icons/rm_256.png DESTINATION ../share/icons/hicolor/256x256/apps RENAME RunManager.png) 
ENDIF()

#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# directories to look for dependencies
SET(DIRS ${QT_LIBRARY_DIR} ${LIBRARY_SEARCH_DIRECTORY})

# Now the work of copying dependencies into the bundle/package
# The quotes are escaped and variables to use at install time have their $ escaped
# An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
# Note that the image plugins depend on QtSvg and QtXml, and it got those copied
# over.
IF( WIN32 OR APPLE )
  INSTALL(CODE "
      file(GLOB_RECURSE QTPLUGINS
        \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
      include(BundleUtilities)
      fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
      " )
ENDIF()
