set(target_name Pat)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

# source files
set(${target_name}_SRC
  CloudDialog.cpp
  CloudDialog.hpp
  CloudMonitor.cpp
  CloudMonitor.hpp
  DesignAlternativesTabController.cpp
  DesignAlternativesTabController.hpp
  DesignAlternativesView.cpp
  DesignAlternativesView.hpp
  ExportSpreadsheet.cpp
  ExportSpreadsheet.hpp
  ExportXML.cpp
  ExportXML.hpp
  ExportXMLDialog.cpp
  ExportXMLDialog.hpp
  HorizontalTabWidget.cpp
  HorizontalTabWidget.hpp
  main.cpp
  MainRightColumnController.cpp
  MainRightColumnController.hpp
  MeasuresTabController.cpp
  MeasuresTabController.hpp
  MeasuresView.cpp
  MeasuresView.hpp
  MonitorUseDialog.cpp
  MonitorUseDialog.hpp
  PatAPI.hpp
  PatApp.cpp
  PatApp.hpp
  PatConstants.hpp
  PatMainMenu.cpp
  PatMainMenu.hpp
  PatMainTabView.cpp
  PatMainTabView.hpp
  PatMainWindow.cpp
  PatMainWindow.hpp
  PatVerticalTabWidget.cpp
  PatVerticalTabWidget.hpp
  ResultsTabController.cpp
  ResultsTabController.hpp
  ResultsView.cpp
  ResultsView.hpp
  RunTabController.cpp
  RunTabController.hpp
  RunView.cpp
  RunView.hpp
  StartupView.cpp
  StartupView.hpp
  "${CMAKE_CURRENT_BINARY_DIR}/VagrantConfiguration.hxx"

  # shared objects
  ../shared_gui_components/BCLMeasureDialog.cpp
  ../shared_gui_components/BCLMeasureDialog.hpp
  ../shared_gui_components/BuildingComponentDialog.cpp
  ../shared_gui_components/BuildingComponentDialog.hpp
  ../shared_gui_components/BuildingComponentDialogCentralWidget.cpp
  ../shared_gui_components/BuildingComponentDialogCentralWidget.hpp
  ../shared_gui_components/BusyWidget.cpp
  ../shared_gui_components/BusyWidget.hpp
  ../shared_gui_components/Buttons.cpp
  ../shared_gui_components/Buttons.hpp
  ../shared_gui_components/CollapsibleComponent.cpp
  ../shared_gui_components/CollapsibleComponent.hpp
  ../shared_gui_components/CollapsibleComponentHeader.cpp
  ../shared_gui_components/CollapsibleComponentHeader.hpp
  ../shared_gui_components/CollapsibleComponentList.cpp
  ../shared_gui_components/CollapsibleComponentList.hpp
  ../shared_gui_components/Component.cpp
  ../shared_gui_components/Component.hpp
  ../shared_gui_components/ComponentList.cpp
  ../shared_gui_components/ComponentList.hpp
  ../shared_gui_components/EditController.cpp
  ../shared_gui_components/EditController.hpp
  ../shared_gui_components/EditView.cpp
  ../shared_gui_components/EditView.hpp
  ../shared_gui_components/HeaderViews.cpp
  ../shared_gui_components/HeaderViews.hpp
  ../shared_gui_components/LocalLibraryController.cpp
  ../shared_gui_components/LocalLibraryController.hpp
  ../shared_gui_components/LocalLibraryView.cpp
  ../shared_gui_components/LocalLibraryView.hpp
  ../shared_gui_components/LostCloudConnectionDialog.cpp
  ../shared_gui_components/LostCloudConnectionDialog.hpp
  ../shared_gui_components/MeasureBadge.cpp
  ../shared_gui_components/MeasureBadge.hpp
  ../shared_gui_components/MeasureDragData.cpp
  ../shared_gui_components/MeasureDragData.hpp
  ../shared_gui_components/MeasureManager.cpp
  ../shared_gui_components/MeasureManager.hpp
  ../shared_gui_components/OSCheckBox.cpp
  ../shared_gui_components/OSCheckBox.hpp
  ../shared_gui_components/OSCollapsibleView.cpp
  ../shared_gui_components/OSCollapsibleView.hpp
  ../shared_gui_components/OSComboBox.cpp
  ../shared_gui_components/OSComboBox.hpp
  ../shared_gui_components/OSDialog.cpp
  ../shared_gui_components/OSDialog.hpp
  ../shared_gui_components/OSDoubleEdit.cpp
  ../shared_gui_components/OSDoubleEdit.hpp
  ../shared_gui_components/OSDragableView.cpp
  ../shared_gui_components/OSDragableView.hpp
  ../shared_gui_components/OSIntegerEdit.cpp
  ../shared_gui_components/OSIntegerEdit.hpp
  ../shared_gui_components/OSLineEdit.cpp
  ../shared_gui_components/OSLineEdit.hpp
  ../shared_gui_components/OSListController.cpp
  ../shared_gui_components/OSListController.hpp
  ../shared_gui_components/OSListView.cpp
  ../shared_gui_components/OSListView.hpp
  ../shared_gui_components/OSQObjectController.cpp
  ../shared_gui_components/OSQObjectController.hpp
  ../shared_gui_components/OSQuantityEdit.cpp
  ../shared_gui_components/OSQuantityEdit.hpp
  ../shared_gui_components/OSSwitch.cpp
  ../shared_gui_components/OSSwitch.hpp
  ../shared_gui_components/OSViewSwitcher.cpp
  ../shared_gui_components/OSViewSwitcher.hpp
  ../shared_gui_components/PageNavigator.cpp
  ../shared_gui_components/PageNavigator.hpp
  ../shared_gui_components/ProcessEventsProgressBar.cpp
  ../shared_gui_components/ProcessEventsProgressBar.hpp
  ../shared_gui_components/SyncMeasuresDialog.cpp
  ../shared_gui_components/SyncMeasuresDialog.hpp
  ../shared_gui_components/SyncMeasuresDialogCentralWidget.cpp
  ../shared_gui_components/SyncMeasuresDialogCentralWidget.hpp
  ../shared_gui_components/TIDItemModel.cpp
  ../shared_gui_components/TIDItemModel.hpp
  ../shared_gui_components/VariableList.cpp
  ../shared_gui_components/VariableList.hpp
  ../shared_gui_components/VariableView.cpp
  ../shared_gui_components/VariableView.hpp
  ../shared_gui_components/WorkflowTools.cpp
  ../shared_gui_components/WorkflowTools.hpp

  ../shared_gui_components/NetworkProxyDialog.cpp
  ../shared_gui_components/NetworkProxyDialog.hpp
)

# moc files
set(${target_name}_moc
  CloudDialog.hpp
  CloudMonitor.hpp
  DesignAlternativesTabController.hpp
  DesignAlternativesView.hpp
  ExportXMLDialog.hpp
  HorizontalTabWidget.hpp
  MainRightColumnController.hpp
  MeasuresTabController.hpp
  MeasuresView.hpp
  MonitorUseDialog.hpp
  PatApp.hpp
  PatMainMenu.hpp
  PatMainTabView.hpp
  PatMainWindow.hpp
  PatVerticalTabWidget.hpp
  ResultsTabController.hpp
  ResultsView.hpp
  RunTabController.hpp
  RunView.hpp
  StartupView.hpp

  ../shared_gui_components/BCLMeasureDialog.hpp
  ../shared_gui_components/BuildingComponentDialog.hpp
  ../shared_gui_components/BuildingComponentDialogCentralWidget.hpp
  ../shared_gui_components/BusyWidget.hpp
  ../shared_gui_components/Buttons.hpp
  ../shared_gui_components/CollapsibleComponent.hpp
  ../shared_gui_components/CollapsibleComponentHeader.hpp
  ../shared_gui_components/CollapsibleComponentList.hpp
  ../shared_gui_components/Component.hpp
  ../shared_gui_components/ComponentList.hpp
  ../shared_gui_components/EditController.hpp
  ../shared_gui_components/EditView.hpp
  ../shared_gui_components/HeaderViews.hpp
  ../shared_gui_components/LocalLibraryController.hpp
  ../shared_gui_components/LocalLibraryView.hpp
  ../shared_gui_components/LostCloudConnectionDialog.hpp
  ../shared_gui_components/MeasureBadge.hpp
  ../shared_gui_components/MeasureDragData.hpp
  ../shared_gui_components/MeasureManager.hpp
  ../shared_gui_components/OSCheckBox.hpp
  ../shared_gui_components/OSCollapsibleView.hpp
  ../shared_gui_components/OSComboBox.hpp
  ../shared_gui_components/OSDialog.hpp
  ../shared_gui_components/OSDoubleEdit.hpp
  ../shared_gui_components/OSDragableView.hpp
  ../shared_gui_components/OSIntegerEdit.hpp
  ../shared_gui_components/OSLineEdit.hpp
  ../shared_gui_components/OSListController.hpp
  ../shared_gui_components/OSListView.hpp
  ../shared_gui_components/OSQObjectController.hpp
  ../shared_gui_components/OSQuantityEdit.hpp
  ../shared_gui_components/OSSwitch.hpp
  ../shared_gui_components/OSViewSwitcher.hpp
  ../shared_gui_components/PageNavigator.hpp
  ../shared_gui_components/SyncMeasuresDialog.hpp
  ../shared_gui_components/SyncMeasuresDialogCentralWidget.hpp
  ../shared_gui_components/TIDItemModel.hpp
  ../shared_gui_components/VariableList.hpp
  ../shared_gui_components/VariableView.hpp
  ../shared_gui_components/NetworkProxyDialog.hpp
)

## resource files
set(${target_name}_qrc
  pat.qrc
)

## Qt UI generation
qt5_wrap_cpp_minimally(${target_name}_moc_src ${${target_name}_moc})

# generate rules for building source files from the resources
qt5_add_resources(${target_name}_qrcs ${${target_name}_qrc})

# configure the AboutBox.hpp file
# this needs to be configured to embed the build number
configure_file(AboutBox.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AboutBox.hpp")

# configure vagrant
set(VAGRANT_SERVER_PATH "" CACHE PATH "Pat to vagrant openstudio server")
set(VAGRANT_WORKER_PATH "" CACHE PATH "Pat to vagrant openstudio worker")
configure_file(VagrantConfiguration.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/VagrantConfiguration.hxx")

if(WIN32)
  set(RC_BUILD_STR "0")
  if(NOT CMAKE_VERSION_BUILD STREQUAL "" AND NOT CMAKE_VERSION_BUILD MATCHES "([a-zA-Z]+)")
    set(RC_BUILD_STR "${CMAKE_VERSION_BUILD}")
  endif()
  configure_file(PatApp.rc.in "${CMAKE_CURRENT_BINARY_DIR}/PatApp.rc")
  set(ICON_SRC "${CMAKE_CURRENT_BINARY_DIR}/PatApp.rc")
endif()
# For Apple set the icns file containing icons
if(APPLE)
  # set the name of the bundle icon
  set(MACOSX_BUNDLE_ICON_FILE pa.icns)
  # set all icons
  set(ICON_SRC ../../icons/pa.icns ../../icons/osp.icns)
  set_source_files_properties(${ICON_SRC}
    PROPERTIES MACOSX_PACKAGE_LOCATION Resources
  )
  set(MACOSX_BUNDLE_EXECUTABLE_NAME ${target_name})
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "gov.nrel.openstudio.${CMAKE_VERSION_BUILD}.parametricanalysistool")
  set(MACOSX_BUNDLE_BUNDLE_NAME "ParametricAnalysisTool")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
endif()

include_directories(${RUBY_INCLUDE_DIRS})

add_executable(${target_name}
  WIN32
  MACOSX_BUNDLE
  ${${target_name}_SRC}
  ${${target_name}_moc_src}
  ${${target_name}_moc}
  ${${target_name}_qrcs}
  ${ICON_SRC}
)

set(depends
  openstudio_model
  openstudio_energyplus
  openstudio_modeleditor
  openstudio_runmanager
  openstudio_ruleset
  openstudio_analysis
  openstudio_project
  openstudio_analysisdriver
  openstudio_osversion
  ${QT_LIBS}
  ${RUBY_LIBRARY}
)

add_dependencies("${target_name}"
  ruby_OpenStudioUtilitiesCore
  ruby_OpenStudioUtilitiesBCL
  ruby_OpenStudioUtilitiesIdd
  ruby_OpenStudioUtilitiesIdf
  ruby_OpenStudioUtilities
  ruby_OpenStudioModel
  ruby_OpenStudioModelCore
  ruby_OpenStudioModelSimulation
  ruby_OpenStudioModelResources
  ruby_OpenStudioModelGeometry
  ruby_OpenStudioModelHVAC
  ruby_OpenStudioEnergyPlus
  ruby_OpenStudioRuleset
)

target_link_libraries(${target_name} ${depends})
AddPCH(${target_name})

#------------------------------------------------------------------------------
# Installation
#------------------------------------------------------------------------------
if(APPLE)
  set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  set(plugin_dest_dir Pat.app/Contents/MacOS)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/Pat.app")
elseif(WIN32)
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/Pat.exe")
else()
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/Pat")
endif()

install(TARGETS Pat
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

# Linux desktop integration
if(UNIX AND NOT APPLE AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  # Install .desktop file
  configure_file(Pat.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/Pat.desktop)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/Pat.desktop DESTINATION ../share/applications)

  # Declare mime types
  install(FILES Pat.xml DESTINATION ../share/mime/packages)

  # Install icons
  install(FILES ../../icons/pa_32.png DESTINATION ../share/pixmaps RENAME Pat.png)

  install(FILES ../../icons/pa_16.png DESTINATION ../share/icons/hicolor/16x16/apps RENAME Pat.png)
  install(FILES ../../icons/pa_32.png DESTINATION ../share/icons/hicolor/32x32/apps RENAME Pat.png)
  install(FILES ../../icons/pa_48.png DESTINATION ../share/icons/hicolor/48x48/apps RENAME Pat.png)
  install(FILES ../../icons/pa_128.png DESTINATION ../share/icons/hicolor/128x128/apps RENAME Pat.png)
  install(FILES ../../icons/pa_256.png DESTINATION ../share/icons/hicolor/256x256/apps RENAME Pat.png)

  install(FILES ../../icons/osp_16.png DESTINATION ../share/icons/hicolor/16x16/mimetypes RENAME application-openstudio.osp.png)
  install(FILES ../../icons/osp_32.png DESTINATION ../share/icons/hicolor/32x32/mimetypes RENAME application-openstudio.osp.png)
  install(FILES ../../icons/osp_48.png DESTINATION ../share/icons/hicolor/48x48/mimetypes RENAME application-openstudio.osp.png)
  install(FILES ../../icons/osp_128.png DESTINATION ../share/icons/hicolor/128x128/mimetypes RENAME application-openstudio.osp.png)
  install(FILES ../../icons/osp_256.png DESTINATION ../share/icons/hicolor/256x256/mimetypes RENAME application-openstudio.osp.png)
endif()

# directories to look for dependencies
set(DIRS ${QT_LIBRARY_DIR} ${LIBRARY_SEARCH_DIRECTORY})

if(APPLE)
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/pat_app/Measures
          DESTINATION Pat.app/Contents/share/openstudio-${OPENSTUDIO_VERSION}/pat
  )
else()
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/pat_app/Measures
          DESTINATION share/openstudio-${OPENSTUDIO_VERSION}/pat
  )
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
      \"${CMAKE_BINARY_DIR}/Products/ruby/openstudioruleset.bundle\"
    )

    file(COPY \${PLUGINS} DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}\")

    file(GLOB_RECURSE FIXUPPLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\"
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/*.bundle\"
    )

    include(BundleUtilities)

    fixup_bundle(\"${APPS}\" \"\${FIXUPPLUGINS}\" \"${DIRS}\")
  ")
endif()
