SET( target_name openstudio_modeleditor )

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

#--------------------------------------------------------------------------------
# First the library
#--------------------------------------------------------------------------------

# lib files
SET(${target_name}_src
  BridgeClasses.hpp
  BridgeClasses.cpp
  ClassViewWidget.hpp
  ClassViewWidget.cpp
  EditorFrame.hpp
  EditorFrame.cpp
  InspectorDialog.hpp
  InspectorDialog.cpp
  InspectorGadget.hpp
  InspectorGadget.cpp
  ListWidget.hpp
  ListWidget.cpp
  ModalDialogs.hpp
  ModalDialogs.cpp
  ModelExplorer.hpp
  ModelExplorer.cpp
  modeltest.h
  modeltest.cpp
  ObjectExplorer.hpp
  ObjectExplorer.cpp
  tablemodel.h
  tablemodel.cpp
  TableView.hpp
  TableView.cpp
  TableWidget.hpp
  TableWidget.cpp
  TestButton.hpp
  TestButton.cpp
  treeitem.h
  treeitem.cpp
  treemodel.h
  treemodel.cpp
  TreeView.hpp
  TreeView.cpp
  TreeViewWidget.hpp
  TreeViewWidget.cpp
  ViewWidget.hpp
  ViewWidget.cpp
  IGPrecisionDialog.hpp
  IGPrecisionDialog.cpp
  IGLineEdit.hpp
  IGLineEdit.cpp
  IGSpinBoxes.hpp
  IGSpinBoxes.cpp
)

# lib moc files
SET( ${target_name}_moc
  ClassViewWidget.hpp
  EditorFrame.hpp
  InspectorDialog.hpp
  InspectorGadget.hpp
  ListWidget.hpp
  modeltest.h
  ModalDialogs.hpp
  ModelExplorer.hpp
  ObjectExplorer.hpp
  tablemodel.h
  TableView.hpp
  TableWidget.hpp
  TestButton.hpp
  treemodel.h
  TreeView.hpp
  TreeViewWidget.hpp
  ViewWidget.hpp
  IGPrecisionDialog.hpp
  IGLineEdit.hpp
  IGSpinBoxes.hpp
  BridgeClasses.hpp
)

# lib ui files
SET( ${target_name}_ui
  precisiondialog.ui
)

# lib resource files
SET( ${target_name}_qrc
  modeleditorlib.qrc
)

# configure the AboutBox.hpp file
# this needs to be configured to embed the build number
CONFIGURE_FILE(AboutBox.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AboutBox.hpp")

## Qt MOC generation
qt5_wrap_cpp( ${target_name}_mocs ${${target_name}_moc} )

## Qt UI Generation
qt5_wrap_ui( ${target_name}_uis ${${target_name}_ui} )

# generate rules for building source files from the resources
qt5_add_resources(${target_name}_qrcs ${${target_name}_qrc} )

# make the shared library
ADD_LIBRARY( ${target_name} SHARED
  ${${target_name}_src}
  ${${target_name}_moc}
  ${${target_name}_mocs}
  ${${target_name}_qrc}
  ${${target_name}_qrcs}
  ${${target_name}_ui}
  ${${target_name}_uis}
)

# lib dependencies
SET( ${target_name}_depends
  openstudio_energyplus
  openstudio_model
)

ADD_DEPENDENCIES( ${target_name}  ${${target_name}_depends} )
TARGET_LINK_LIBRARIES( ${target_name}  ${${target_name}_depends} )

IF( NOT APPLE )
INSTALL(TARGETS ${target_name}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
)
ENDIF()

SET( ${target_name}_test_src
  test/ModelEditorFixture.hpp
  test/ModelEditorFixture.cpp
  test/InspectorDialog_GTest.cpp
  test/ModalDialogs_GTest.cpp
)

SET (${target_name}_test_depends
  ${${target_name}_depends}
)

CREATE_TEST_TARGETS( ${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}" )
CREATE_SRC_GROUPS( "${${target_name}_test_src}" )
IF( BUILD_TESTING )

  ADD_DEPENDENCIES("${target_name}_tests"
    openstudio_modeleditor_resources 
  )

ENDIF()


# lib swig files
SET( ${target_name}_swig_src
  ModelEditor.i
)

MAKE_SWIG_TARGET( OpenStudioModelEditor modeleditor "${CMAKE_CURRENT_SOURCE_DIR}/ModelEditor.i" "${${target_name}_swig_src}" ${target_name} OpenStudioEnergyPlus )
ADD_DEPENDENCIES( ruby_OpenStudioModelEditor ruby_OpenStudioModel )

##--------------------------------------------------------------------------------
## Now the ModelEditor.exe stuff below
##--------------------------------------------------------------------------------
#
#SET( target_name ModelEditor )
#
#MAKE_LITE_SQL_TARGET(ModelEditor.xml modelobjectdatabase)
#MAKE_LITE_SQL_TARGET(ProjectModel.xml projectmodeldatabase)
#
## source files
#SET(${target_name}_src
#  main.cpp
#  mainpage.hpp
#  ModelEditor.hpp
#  ModelEditor.cpp
#  "${CMAKE_CURRENT_BINARY_DIR}/modelobjectdatabase.hxx"
#  "${CMAKE_CURRENT_BINARY_DIR}/modelobjectdatabase.cxx"
#  "${CMAKE_CURRENT_BINARY_DIR}/projectmodeldatabase.hxx"
#  "${CMAKE_CURRENT_BINARY_DIR}/projectmodeldatabase.cxx"
#)
#
## moc files
#SET( ${target_name}_moc
#  ModelEditor.hpp
#)
#
## lib ui files
#SET( ${target_name}_ui
#)
#
## resource files
#SET( ${target_name}_qrc
#  application.qrc
#)
#
## Qt MOC generation
#qt5_wrap_cpp( ${target_name}_mocs ${${target_name}_moc} )
#
## Qt UI generation
#qt5_wrap_ui( ${target_name}_uis ${${target_name}_ui} )
#
## generate rules for building source files from the resources
#qt5_add_resources(${target_name}_qrcs ${${target_name}_qrc} )
#
## configure the AboutBox.hpp file
## this needs to be configured to embed the build number
#CONFIGURE_FILE(AboutBox.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AboutBox.hpp")
#
##--------------------------------------------------------------------------------
## Icons
##--------------------------------------------------------------------------------
## For Windows set the rc file pointing to the icon file
#IF( WIN32 )
#  SET(ICON_SRC "${CMAKE_CURRENT_SOURCE_DIR}/ModelEditor.rc")
#ENDIF()
## For Apple set the icns file containing icons
#IF(APPLE)
#  # set how it shows up in the Info.plist file
#  SET(MACOSX_BUNDLE_ICON_FILE me.icns)
#  # set where in the bundle to put the icns file
#  SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_SOURCE_DIR}/../../icons/me.icns
#      PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
#  # include the icns file in the target
#  SET(ICON_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../icons/me.icns)
#ENDIF(APPLE)
#
#ADD_EXECUTABLE( ${target_name}
#  WIN32
#  MACOSX_BUNDLE
#  ${${target_name}_src}
#  ${${target_name}_ui}
#  ${${target_name}_uis}
#  ${${target_name}_moc}
#  ${${target_name}_mocs}
#  ${${target_name}_qrc}
#  ${${target_name}_qrcs}
#  ${ICON_SRC}
#)
#
#SET( ${target_name}_depends
#  openstudio_modeleditor
#)
#
#TARGET_LINK_LIBRARIES( ${target_name} ${${target_name}_depends} )
#AddPCH(${target_name})
#
##--------------------------------------------------------------------------------
## Now the installation stuff below
##--------------------------------------------------------------------------------
#SET(plugin_dest_dir bin)
#SET(qtconf_dest_dir bin)
#SET(rvdoc_dest_dir doc)
#SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/ModelEditor")
#IF(APPLE)
#  SET(plugin_dest_dir ModelEditor.app/Contents/MacOS)
#  SET(qtconf_dest_dir ModelEditor.app/Contents/Resources)
#  SET(APPS "\${CMAKE_INSTALL_PREFIX}/ModelEditor.app")
#  SET(rvdoc_dest_dir ModelEditor.app/Contents/doc)
#ENDIF(APPLE)
#IF(WIN32)
#  SET(APPS "\${CMAKE_INSTALL_PREFIX}/bin/ModelEditor.exe")
#ENDIF(WIN32)
#
## NOTE: is there a Linux version that needs to be installed
#
##--------------------------------------------------------------------------------
## Install the QtTest application, on Apple, the bundle is at the root of the
## install tree, and on other platforms it'll go into the bin directory.
#INSTALL(TARGETS ModelEditor
#    BUNDLE DESTINATION .
#    RUNTIME DESTINATION bin
#    )
#
##--------------------------------------------------------------------------------
## Install the documentation
##INSTALL(FILES "doc/ReadMe.txt"
##      DESTINATION ${rvdoc_dest_dir}/
##  )
#
##--------------------------------------------------------------------------------
## Install needed Qt plugins by copying directories from the qt installation
## One can cull what gets copied by using 'REGEX "..." EXCLUDE'
##INSTALL(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION ${plugin_dest_dir}/plugins )
#
##--------------------------------------------------------------------------------
## install a qt.conf file
## this inserts some cmake code into the install script to write the file
#IF( APPLE )
#  INSTALL(CODE "
#      file(COPY \"${QT_LIBRARY_DIR}/QtGui.framework/Resources/qt_menu.nib\"
#      DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/\")
#      " )
#ENDIF()
#
#IF( APPLE OR WIN32 )
#  INSTALL(CODE "
#      file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${qtconf_dest_dir}/qt.conf\" \"\")
#      " )
#ENDIF()
#
##--------------------------------------------------------------------------------
## Use BundleUtilities to get all other dependencies for the application to work.
## It takes a bundle or executable along with possible plugins and inspects it
## for dependencies.  If they are not system dependencies, they are copied.
#
## directories to look for dependencies
#SET(DIRS ${QT_LIBRARY_DIR} ${LIBRARY_SEARCH_DIRECTORY})
#
## Now the work of copying dependencies into the bundle/package
## The quotes are escaped and variables to use at install time have their $ escaped
## An alternative is the do a configure_file() on a script and use install(SCRIPT  ...).
## Note that the image plugins depend on QtSvg and QtXml, and it got those copied
## over.
#IF( WIN32 OR APPLE )
#  INSTALL(CODE "
#      file(GLOB_RECURSE QTPLUGINS
#        \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
#      include(BundleUtilities)
#      fixup_bundle(\"${APPS}\" \"\${QTPLUGINS}\" \"${DIRS}\")
#      " )
#ENDIF()
