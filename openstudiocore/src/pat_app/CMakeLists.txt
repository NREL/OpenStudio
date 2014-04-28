SET( target_name Pat )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

# source files
SET( ${target_name}_SRC
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
SET( ${target_name}_moc
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
  ../shared_gui_components/TIDItemModel.hpp
  ../shared_gui_components/VariableList.hpp
  ../shared_gui_components/VariableView.hpp
  ../shared_gui_components/NetworkProxyDialog.hpp
)

## resource files
SET( ${target_name}_qrc
  pat.qrc
)

## Qt UI generation
qt5_wrap_cpp( ${target_name}_moc_src ${${target_name}_moc} )

# generate rules for building source files from the resources
qt5_add_resources(${target_name}_qrcs ${${target_name}_qrc} )

# configure the AboutBox.hpp file
# this needs to be configured to embed the build number
CONFIGURE_FILE(AboutBox.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AboutBox.hpp")

# configure vagrant 
SET(VAGRANT_SERVER_PATH "" CACHE PATH "Pat to vagrant openstudio server")
SET(VAGRANT_WORKER_PATH "" CACHE PATH "Pat to vagrant openstudio worker")
CONFIGURE_FILE(VagrantConfiguration.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/VagrantConfiguration.hxx")

IF( WIN32 )
  SET(RC_BUILD_STR "0")
  IF( NOT CMAKE_VERSION_BUILD STREQUAL "" AND NOT CMAKE_VERSION_BUILD MATCHES "([a-zA-Z]+)" )
    SET(RC_BUILD_STR "${CMAKE_VERSION_BUILD}")
  ENDIF()
  CONFIGURE_FILE(PatApp.rc.in "${CMAKE_CURRENT_BINARY_DIR}/PatApp.rc")
  SET(ICON_SRC "${CMAKE_CURRENT_BINARY_DIR}/PatApp.rc")
ENDIF()
# For Apple set the icns file containing icons
IF(APPLE)
  # set the name of the bundle icon
  SET(MACOSX_BUNDLE_ICON_FILE pa.icns)
  # set all icons
  SET(ICON_SRC ../../icons/pa.icns ../../icons/osp.icns)
  SET_SOURCE_FILES_PROPERTIES(${ICON_SRC}
      PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
  SET(MACOSX_BUNDLE_EXECUTABLE_NAME ${target_name})
  SET(MACOSX_BUNDLE_GUI_IDENTIFIER "gov.nrel.openstudio.parametricanalysistool")
  SET(MACOSX_BUNDLE_BUNDLE_NAME "ParametricAnalysisTool")
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
ENDIF(APPLE)

INCLUDE_DIRECTORIES( ${RUBY_INCLUDE_DIRS} )

ADD_EXECUTABLE( ${target_name}
  WIN32
  MACOSX_BUNDLE
  ${${target_name}_SRC}
  ${${target_name}_moc_src}
  ${${target_name}_moc}
  ${${target_name}_qrcs}
  ${ICON_SRC}
)

SET( depends
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

ADD_DEPENDENCIES( "${target_name}"
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
                  ruby_OpenStudioRuleset )

TARGET_LINK_LIBRARIES( ${target_name}  ${depends} )
AddPCH(${target_name})

#------------------------------------------------------------------------------
# Installation
#------------------------------------------------------------------------------
IF(APPLE)
  SET_TARGET_PROPERTIES(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  SET(plugin_dest_dir Pat.app/Contents/MacOS)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/Pat.app")
ELSEIF(WIN32)
  SET(plugin_dest_dir bin)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/Pat.exe")
ELSE()
  SET(plugin_dest_dir bin)
  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/Pat")
ENDIF()

INSTALL(TARGETS Pat 
    BUNDLE DESTINATION . 
    RUNTIME DESTINATION bin 
    )

if( APPLE OR WIN32 )

  get_target_property(QT_QSQLITE_PLUGIN Qt5::QSQLiteDriverPlugin LOCATION)

  install( PROGRAMS "${QT_QSQLITE_PLUGIN}"
           DESTINATION "${plugin_dest_dir}/sqldrivers/" 
         )

  get_filename_component(QSQLITE_PLUGIN_NAME "${QT_QSQLITE_PLUGIN}" NAME)

  SET(QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/sqldrivers/${QSQLITE_PLUGIN_NAME}")

endif()

if( WIN32 )

  get_target_property(QT_WINDOWS_PLUGIN Qt5::QWindowsIntegrationPlugin LOCATION)

  install( PROGRAMS "${QT_WINDOWS_PLUGIN}"
           DESTINATION "${plugin_dest_dir}/platforms/" 
         )

  get_filename_component(QWINDOWS_PLUGIN_NAME "${QT_WINDOWS_PLUGIN}" NAME)

  list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/platforms/${QWINDOWS_PLUGIN_NAME}")

endif()

# Linux desktop integration
IF (UNIX AND NOT APPLE AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  # Install .desktop file
  CONFIGURE_FILE(Pat.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/Pat.desktop)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/Pat.desktop DESTINATION ../share/applications)

  # Declare mime types
  INSTALL(FILES Pat.xml DESTINATION ../share/mime/packages)

  # Install icons
  INSTALL(FILES ../../icons/pa_32.png DESTINATION ../share/pixmaps RENAME Pat.png)

  INSTALL(FILES ../../icons/pa_16.png DESTINATION ../share/icons/hicolor/16x16/apps RENAME Pat.png)
  INSTALL(FILES ../../icons/pa_32.png DESTINATION ../share/icons/hicolor/32x32/apps RENAME Pat.png)
  INSTALL(FILES ../../icons/pa_48.png DESTINATION ../share/icons/hicolor/48x48/apps RENAME Pat.png)
  INSTALL(FILES ../../icons/pa_128.png DESTINATION ../share/icons/hicolor/128x128/apps RENAME Pat.png)
  INSTALL(FILES ../../icons/pa_256.png DESTINATION ../share/icons/hicolor/256x256/apps RENAME Pat.png)

  INSTALL(FILES ../../icons/osp_16.png DESTINATION ../share/icons/hicolor/16x16/mimetypes RENAME application-openstudio.osp.png)
  INSTALL(FILES ../../icons/osp_32.png DESTINATION ../share/icons/hicolor/32x32/mimetypes RENAME application-openstudio.osp.png)
  INSTALL(FILES ../../icons/osp_48.png DESTINATION ../share/icons/hicolor/48x48/mimetypes RENAME application-openstudio.osp.png)
  INSTALL(FILES ../../icons/osp_128.png DESTINATION ../share/icons/hicolor/128x128/mimetypes RENAME application-openstudio.osp.png)
  INSTALL(FILES ../../icons/osp_256.png DESTINATION ../share/icons/hicolor/256x256/mimetypes RENAME application-openstudio.osp.png) 
ENDIF()

# directories to look for dependencies
SET(DIRS ${QT_LIBRARY_DIR} ${LIBRARY_SEARCH_DIRECTORY})

if (APPLE)  
  install( DIRECTORY ${CMAKE_SOURCE_DIR}/src/pat_app/Measures
           DESTINATION Pat.app/Contents/share/openstudio/pat
         )
else()
  install( DIRECTORY ${CMAKE_SOURCE_DIR}/src/pat_app/Measures
           DESTINATION share/openstudio/pat
         ) 
endif()

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
