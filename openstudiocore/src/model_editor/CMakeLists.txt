set(target_name openstudio_modeleditor)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${QT_INCLUDES})

#--------------------------------------------------------------------------------
# First the library
#--------------------------------------------------------------------------------

# lib files
set(${target_name}_src
  Application.hpp
  Application.cpp
  AccessPolicyStore.hpp
  AccessPolicyStore.cpp
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
  OSProgressBar.hpp
  OSProgressBar.cpp
  PathWatcher.hpp
  PathWatcher.cpp
  QMetaTypes.hpp
  QMetaTypes.cpp
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
  UserSettings.hpp
  UserSettings.cpp
  Utilities.hpp
  Utilities.cpp
  IGPrecisionDialog.hpp
  IGPrecisionDialog.cpp
  IGLineEdit.hpp
  IGLineEdit.cpp
  IGSpinBoxes.hpp
  IGSpinBoxes.cpp
)

# lib moc files
set(${target_name}_moc
  ClassViewWidget.hpp
  EditorFrame.hpp
  InspectorDialog.hpp
  InspectorGadget.hpp
  ListWidget.hpp
  modeltest.h
  ModalDialogs.hpp
  ModelExplorer.hpp
  ObjectExplorer.hpp
  PathWatcher.hpp
  # QMetaTypes.hpp # Not needed
  tablemodel.h
  TableView.hpp
  TableWidget.hpp
  TestButton.hpp
  treemodel.h
  TreeView.hpp
  TreeViewWidget.hpp
  Utilities.hpp
  ViewWidget.hpp
  IGPrecisionDialog.hpp
  IGLineEdit.hpp
  IGSpinBoxes.hpp
  BridgeClasses.hpp
)

# lib ui files
set(${target_name}_ui
  precisiondialog.ui
)

# lib resource files
set(${target_name}_qrc
  modeleditorlib.qrc
)

# configure the AboutBox.hpp file
# this needs to be configured to embed the build number
CONFIGURE_FILE_WITH_CHECKSUM(AboutBox.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AboutBox.hpp")

## Qt MOC generation
qt5_wrap_cpp_minimally(${target_name}_mocs ${${target_name}_moc})

## Qt UI Generation
qt5_wrap_ui(${target_name}_uis ${${target_name}_ui})

# generate rules for building source files from the resources
qt5_add_resources(${target_name}_qrcs ${${target_name}_qrc})

# make the shared library
add_library(${target_name}
  ${${target_name}_src}
  ${${target_name}_moc}
  ${${target_name}_mocs}
  ${${target_name}_qrc}
  ${${target_name}_qrcs}
  ${${target_name}_ui}
  ${${target_name}_uis}
)

# lib dependencies
set(${target_name}_depends
  openstudio_energyplus
  openstudio_model
  ${QT_LIBS}
)
if(WIN32)
  list(APPEND ${target_name}_depends qtwinmigrate)
endif()

add_dependencies(${target_name} ${${target_name}_depends})
target_link_libraries(${target_name} ${${target_name}_depends})

# make the static library
add_library(${target_name}
  ${${target_name}_src}
  ${${target_name}_moc}
  ${${target_name}_mocs}
  ${${target_name}_qrc}
  ${${target_name}_qrcs}
  ${${target_name}_ui}
  ${${target_name}_uis}
)

# lib dependencies
set(${target_name}_depends
  openstudio_energyplus
  openstudio_model
  ${QT_LIBS}
)
if(WIN32)
  list(APPEND ${target_name}_depends qtwinmigrate)
endif()

target_link_libraries(${target_name} ${${target_name}_depends})

target_compile_definitions(${target_name} PUBLIC model_editor_EXPORTS)
target_compile_definitions(${target_name} PUBLIC model_editor_EXPORTS)

#if(NOT APPLE)
#  install(TARGETS ${target_name}
#    RUNTIME DESTINATION bin
#    LIBRARY DESTINATION lib
#  )
#endif()

set(${target_name}_test_src
  test/ModelEditorFixture.hpp
  test/ModelEditorFixture.cpp
  test/InspectorDialog_GTest.cpp
  test/ModalDialogs_GTest.cpp
  test/PathWatcher_GTest.cpp
  test/QMetaTypes_GTest.cpp
  test/Utilities_GTest.cpp
)

set(${target_name}_test_depends
  ${${target_name}_depends}
)

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")

# lib swig files
set(${target_name}_swig_src
  ModelEditor.i
  Qt.i
)

set(swig_target_name ${target_name})

MAKE_SWIG_TARGET(OpenStudioModelEditor modeleditor "${CMAKE_CURRENT_SOURCE_DIR}/ModelEditor.i" "${${target_name}_swig_src}" ${swig_target_name} OpenStudioEnergyPlus)
add_dependencies(ruby_OpenStudioModelEditor ruby_OpenStudioModel)
