set(target_name ResultsViewer)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# source files
set(${target_name}_SRC
  main.cpp
  mainpage.hpp
  MainWindow.cpp
  TreeView.cpp
  TableView.hpp
  TableView.cpp
  ResultsViewerData.hpp
  ResultsViewerData.cpp
  BrowserView.hpp
  BrowserView.cpp
  PlotView.hpp
  PlotView.cpp
  ChangeAliasDialog.hpp
  ChangeAliasDialog.cpp
  TabDropDock.hpp
  TabDropDock.cpp
  TabBarDrag.hpp
  TabBarDrag.cpp
  PlotViewProperties.hpp
  PlotViewProperties.cpp
)

# ui files
set(${target_name}_UI
  MainWindow.ui
  PlotViewProperties.ui
)

# moc files
set(${target_name}_moc
  MainWindow.hpp
  TreeView.hpp
  TableView.hpp
  PlotView.hpp
  BrowserView.hpp
  ChangeAliasDialog.hpp
  TabDropDock.hpp
  TabBarDrag.hpp
  PlotViewProperties.hpp
)

# resource files
set(${target_name}_qrc
  ResultsViewer.qrc
)

## Qt UI generation
qt5_wrap_cpp_minimally(${target_name}_moc_src ${${target_name}_moc})

qt5_wrap_ui(${target_name}_UIS ${${target_name}_UI})

# generate rules for building source files from the resources
qt5_add_resources(${target_name}_qrcs ${${target_name}_qrc} )

# configure the AboutBox.hpp file
# this needs to be configured to embed the build number
configure_file(AboutBox.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AboutBox.hpp")

#--------------------------------------------------------------------------------
# For Windows set the rc file pointing to the icon file
if(WIN32)
  set(RC_BUILD_STR "0")
  if(NOT CMAKE_VERSION_BUILD STREQUAL "" AND NOT CMAKE_VERSION_BUILD MATCHES "([a-zA-Z]+)")
    set(RC_BUILD_STR "${CMAKE_VERSION_BUILD}")
  endif()
  configure_file(ResultsViewer.rc.in "${CMAKE_CURRENT_BINARY_DIR}/ResultsViewer.rc")
  set(ICON_SRC "${CMAKE_CURRENT_BINARY_DIR}/ResultsViewer.rc")
endif()
# For Apple set the icns file containing icons
if(APPLE)
  # set the name of the bundle icon
  set(MACOSX_BUNDLE_ICON_FILE rv.icns)
  # set all icons
  set(ICON_SRC ../../icons/rv.icns)
  set_source_files_properties(${ICON_SRC}
    PROPERTIES MACOSX_PACKAGE_LOCATION Resources
  )
  set(MACOSX_BUNDLE_EXECUTABLE_NAME ${target_name})
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "gov.nrel.openstudio.${CMAKE_VERSION_BUILD}.resultsviewer")
  set(MACOSX_BUNDLE_BUNDLE_NAME "ResultsViewer")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
endif()

add_executable(${target_name}
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

set(depends
  openstudio_utilities
)

target_link_libraries(${target_name} ${depends})
AddPCH(${target_name})

#--------------------------------------------------------------------------------
# Now the installation stuff below
#--------------------------------------------------------------------------------
set(plugin_dest_dir bin)
set(rvdoc_dest_dir doc)
set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/ResultsViewer")
if(APPLE)
  set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  set(plugin_dest_dir ResultsViewer.app/Contents/MacOS)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/ResultsViewer.app")
  set(rvdoc_dest_dir ResultsViewer.app/Contents/doc)
endif()
if(WIN32)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/ResultsViewer.exe")
endif()

if(UNIX AND NOT APPLE)
  set(rvdoc_dest_dir share/doc/openstudio/resultsviewer)
endif()

#--------------------------------------------------------------------------------
# Install the QtTest application, on Apple, the bundle is at the root of the
# install tree, and on other platforms it'll go into the bin directory.
install(TARGETS ResultsViewer
  BUNDLE DESTINATION .
  RUNTIME DESTINATION bin
)


# Linux desktop integration
if(UNIX AND NOT APPLE AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  # Install .desktop file
  configure_file(ResultsViewer.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/ResultsViewer.desktop)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ResultsViewer.desktop DESTINATION ../share/applications)

  # Install icons
  install(FILES ../../icons/rv_32.png DESTINATION ../share/pixmaps RENAME ResultsViewer.png)

  install(FILES ../../icons/rv_16.png DESTINATION ../share/icons/hicolor/16x16/apps RENAME ResultsViewer.png)
  install(FILES ../../icons/rv_32.png DESTINATION ../share/icons/hicolor/32x32/apps RENAME ResultsViewer.png)
  install(FILES ../../icons/rv_48.png DESTINATION ../share/icons/hicolor/48x48/apps RENAME ResultsViewer.png)
  install(FILES ../../icons/rv_128.png DESTINATION ../share/icons/hicolor/128x128/apps RENAME ResultsViewer.png)
  install(FILES ../../icons/rv_256.png DESTINATION ../share/icons/hicolor/256x256/apps RENAME ResultsViewer.png)
endif()

#--------------------------------------------------------------------------------
# Use BundleUtilities to get all other dependencies for the application to work.
# It takes a bundle or executable along with possible plugins and inspects it
# for dependencies.  If they are not system dependencies, they are copied.

# directories to look for dependencies
set(DIRS ${QT_LIBRARY_DIR} ${LIBRARY_SEARCH_DIRECTORY})

if(WIN32)

  get_target_property(QT_WINDOWS_PLUGIN Qt5::QWindowsIntegrationPlugin LOCATION)

  install(PROGRAMS "${QT_WINDOWS_PLUGIN}"
          DESTINATION "${plugin_dest_dir}/platforms/"
  )

  get_filename_component(QWINDOWS_PLUGIN_NAME "${QT_WINDOWS_PLUGIN}" NAME)

  list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/platforms/${QWINDOWS_PLUGIN_NAME}")

endif()

if(WIN32 OR APPLE)
  install(CODE "
    include(BundleUtilities)
    fixup_bundle(\"${APPS}\" \"${QTPLUGINS}\" \"${DIRS}\")
  ")
endif()
