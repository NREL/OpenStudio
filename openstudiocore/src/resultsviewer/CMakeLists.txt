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
  LinePlot.hpp
  LinePlot.cpp  
  FloodPlot.hpp
  FloodPlot.cpp   
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
  qwt
  openstudio_utilities
)

target_link_libraries(${target_name} ${depends})

if(MSVC)
  target_link_libraries(${target_name} optimized ${QT_MAIN_LIB} debug ${QT_MAIN_LIB_D})
endif()

AddPCH(${target_name})

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
endif()

#------------------------------------------------------------------------------
# Installation
#------------------------------------------------------------------------------
if(APPLE)
  set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  set(plugin_dest_dir ResultsViewer.app/Contents/MacOS)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/ResultsViewer.app")
elseif(WIN32)
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/ResultsViewer.exe")
else()
  set_target_properties(${target_name} PROPERTIES INSTALL_RPATH "$ORIGIN/.")
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/ResultsViewer")
endif()

if( APPLE )
  install(TARGETS ${target_name}
    DESTINATION .
    COMPONENT "ResultsViewer"
  )
else()
  install(TARGETS ${target_name}
    DESTINATION bin
    COMPONENT "ResultsViewer"
  )
endif()

# directories to look for dependencies
# This is probably out of date
set(DIRS "${CMAKE_BINARY_DIR}/Products" "${CMAKE_BINARY_DIR}/Products/Release" "${QT_INSTALL_DIR}/bin" "${QT_INSTALL_DIR}/lib" "${RUBY_INSTALL_DIR}/bin")

# since on apple the whole app bundle is installed (including these webkit components) we can exclude apple
if( NOT APPLE )
  #install(FILES ${icudtl} DESTINATION bin COMPONENT ResultsViewer )
  if(WIN32)
    install(PROGRAMS $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> DESTINATION bin/platforms/ COMPONENT ResultsViewer)
    install(PROGRAMS $<TARGET_FILE:Qt5::QSQLiteDriverPlugin> DESTINATION bin/sqldrivers/ COMPONENT ResultsViewer)
  elseif(UNIX)
    install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/platforms/$<TARGET_FILE_NAME:Qt5::QXcbIntegrationPlugin> DESTINATION bin/platforms/ COMPONENT ResultsViewer)
    install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/xcbglintegrations/$<TARGET_FILE_NAME:Qt5::QXcbGlxIntegrationPlugin> DESTINATION bin/xcbglintegrations/ COMPONENT ResultsViewer)
    install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/sqldrivers/$<TARGET_FILE_NAME:Qt5::QSQLiteDriverPlugin> DESTINATION bin/sqldrivers/ COMPONENT ResultsViewer)
    install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/libQt5XcbQpa.so.5 DESTINATION bin/ COMPONENT ResultsViewer)
    
    foreach(LIB ${QT_LIBS})
      install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/$<TARGET_SONAME_FILE_NAME:${LIB}> DESTINATION bin/ COMPONENT ResultsViewer)
    endforeach()
    
    #foreach(LIB ${QT_WEB_LIBS})
    #  install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/$<TARGET_SONAME_FILE_NAME:${LIB}> DESTINATION bin/ COMPONENT OpenStudioApp)
    #endforeach()
  endif()
endif()

if( APPLE )
  list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/platforms/libqcocoa.dylib")
  list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/sqldrivers/libqsqlite.dylib")
  #install(PROGRAMS ${QtWebEngineProcess} DESTINATION ResultsViewer.app/Contents/MacOS/ COMPONENT ResultsViewer )
  #list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/QtWebEngineProcess")
endif()

if(WIN32 OR APPLE)
  install(CODE "
    include(BundleUtilities)
    include(\"${CMAKE_SOURCE_DIR}/CMake/FixupBundleOverrides.cmake\")
    fixup_bundle(\"${APPS}\" \"${QTPLUGINS}\" \"${DIRS}\")
    " COMPONENT "ResultsViewer" )
endif()
