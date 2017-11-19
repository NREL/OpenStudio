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

add_custom_command(TARGET ${target_name}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${target_name}>/platforms/
  COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${target_name}>/sqldrivers/
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QSQLiteDriverPlugin> $<TARGET_FILE_DIR:${target_name}>/sqldrivers/
  #COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${target_name}>/sceneparsers/
  #COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::GLTFSceneParserPlugin> $<TARGET_FILE_DIR:${target_name}>/sceneparsers/
  #COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::AssimpParserPlugin> $<TARGET_FILE_DIR:${target_name}>/sceneparsers/
)

if( APPLE )
  add_custom_command(TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QCocoaIntegrationPlugin> $<TARGET_FILE_DIR:${target_name}>/platforms/ )
elseif( UNIX )
  
  #add_custom_command(TARGET ${target_name}
  #  POST_BUILD
  #  COMMAND chrpath -r "$ORIGIN/." $<TARGET_FILE:${target_name}>
  #  VERBATIM
  #)

  add_custom_command(TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:${target_name}>/xcbglintegrations/
  )

  add_custom_command(TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QXcbIntegrationPlugin> $<TARGET_FILE_DIR:${target_name}>/platforms/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QXcbGlxIntegrationPlugin> $<TARGET_FILE_DIR:${target_name}>/xcbglintegrations/
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${Qt5XcbQpaLib} $<TARGET_FILE_DIR:${target_name}>/ 
    COMMAND chrpath -r "$ORIGIN/.." $<TARGET_FILE_DIR:${target_name}>/platforms/$<TARGET_FILE_NAME:Qt5::QXcbIntegrationPlugin>
    COMMAND chrpath -r "$ORIGIN/.." $<TARGET_FILE_DIR:${target_name}>/xcbglintegrations/$<TARGET_FILE_NAME:Qt5::QXcbGlxIntegrationPlugin>
    COMMAND chrpath -r "$ORIGIN/.." $<TARGET_FILE_DIR:${target_name}>/sqldrivers/$<TARGET_FILE_NAME:Qt5::QSQLiteDriverPlugin>
    #COMMAND chrpath -r "$ORIGIN/.." $<TARGET_FILE_DIR:${target_name}>/sceneparsers/$<TARGET_FILE_NAME:Qt5::GLTFSceneParserPlugin>
    #COMMAND chrpath -r "$ORIGIN/.." $<TARGET_FILE_DIR:${target_name}>/sceneparsers/$<TARGET_FILE_NAME:Qt5::AssimpParserPlugin>
    VERBATIM
    )
  
  foreach(LIB ${QT_LIBS})
    add_custom_command(TARGET ${target_name}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_SONAME_FILE:${LIB}> $<TARGET_FILE_DIR:${target_name}>/ )
  endforeach()
  
  foreach(LIB ${QT_WEB_LIBS})
    add_custom_command(TARGET ${target_name}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_SONAME_FILE:${LIB}> $<TARGET_FILE_DIR:${target_name}>/ )
  endforeach()

  add_custom_command(TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::WebEngine> $<TARGET_FILE_DIR:${target_name}>/ )

  add_custom_command(TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QtWebEngineProcess} $<TARGET_FILE_DIR:${target_name}>/ 
    COMMAND chrpath -r "$ORIGIN/." $<TARGET_FILE_DIR:${target_name}>/QtWebEngineProcess
    VERBATIM)
    
elseif( WIN32 )
  add_custom_command(TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> $<TARGET_FILE_DIR:${target_name}>/platforms/ )
  
  foreach(LIB ${QT_LIBS})
    # release dlls are needed in debug dir for QtWebEngineProcess, second copy_if_different will be a no-op
    get_property(LIB_RELEASE_PATH TARGET ${LIB} PROPERTY IMPORTED_LOCATION_RELEASE)
    add_custom_command(TARGET ${target_name}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${LIB}> $<TARGET_FILE_DIR:${target_name}>/ 
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIB_RELEASE_PATH} $<TARGET_FILE_DIR:${target_name}>/ 
    )
  endforeach()
  
  foreach(LIB ${QT_WEB_LIBS})
    # release dlls are needed in debug dir for QtWebEngineProcess, second copy_if_different will be a no-op
    get_property(LIB_RELEASE_PATH TARGET ${LIB} PROPERTY IMPORTED_LOCATION_RELEASE)
    add_custom_command(TARGET ${target_name}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${LIB}> $<TARGET_FILE_DIR:${target_name}>/ 
      COMMAND ${CMAKE_COMMAND} -E copy_if_different ${LIB_RELEASE_PATH} $<TARGET_FILE_DIR:${target_name}>/ 
    )
  endforeach()
  
  target_link_libraries(${target_name} optimized ${QT_MAIN_LIB} debug ${QT_MAIN_LIB_D})

  add_custom_command(TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${QtWebEngineProcess} $<TARGET_FILE_DIR:${target_name}>/ )
  
  add_custom_command(TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${icudtl} $<TARGET_FILE_DIR:${target_name}>/ 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${qweb_resources} $<TARGET_FILE_DIR:${target_name}>/ 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${qweb_resources_100} $<TARGET_FILE_DIR:${target_name}>/ 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${qweb_resources_200} $<TARGET_FILE_DIR:${target_name}>/ )

endif()

set(depends
  openstudio_lib
  openstudio_osversion
  openstudio_model
  openstudio_energyplus
  openstudio_modeleditor
  openstudio_sdd
  openstudio_gbxml
  openstudio_bec
  openstudio_bimserver
  ${QT_WEB_LIBS}
  #${QT_PLUGIN_LIBS}
  #${COREFOUNDATION_LIBRARY}
)

target_link_libraries(${target_name} ${depends})

# because the app calls the cli
add_dependencies(${target_name} openstudio)

#AddPCH(${target_name})

#------------------------------------------------------------------------------
# Installation
#------------------------------------------------------------------------------
if(APPLE)
  set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  set(plugin_dest_dir OpenStudioApp.app/Contents/MacOS)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/OpenStudioApp.app")
elseif(WIN32)
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/OpenStudioApp.exe")
else()
  set_target_properties(${target_name} PROPERTIES INSTALL_RPATH "$ORIGIN/.")
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

# since on apple the whole app bundle is installed (including these webkit components) we can exclude apple
if( NOT APPLE )
  install(FILES ${icudtl} DESTINATION bin COMPONENT OpenStudioApp )
  install(FILES ${qweb_resources} DESTINATION bin COMPONENT OpenStudioApp )
  install(FILES ${qweb_resources_100} DESTINATION bin COMPONENT OpenStudioApp )
  install(FILES ${qweb_resources_200} DESTINATION bin COMPONENT OpenStudioApp )
  if(WIN32)
    install(PROGRAMS ${QtWebEngineProcess} DESTINATION bin COMPONENT OpenStudioApp )
    install(PROGRAMS $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> DESTINATION bin/platforms/ COMPONENT OpenStudioApp)
    install(PROGRAMS $<TARGET_FILE:Qt5::QSQLiteDriverPlugin> DESTINATION bin/sqldrivers/ COMPONENT OpenStudioApp)
    #install(PROGRAMS $<TARGET_FILE:Qt5::GLTFSceneParserPlugin> DESTINATION bin/sceneparsers/ COMPONENT OpenStudioApp)
    #install(PROGRAMS $<TARGET_FILE:Qt5::AssimpParserPlugin> DESTINATION bin/sceneparsers/ COMPONENT OpenStudioApp)
  elseif(UNIX)
    install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/QtWebEngineProcess DESTINATION bin/ COMPONENT OpenStudioApp)
    install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/platforms/$<TARGET_FILE_NAME:Qt5::QXcbIntegrationPlugin> DESTINATION bin/platforms/ COMPONENT OpenStudioApp)
    install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/xcbglintegrations/$<TARGET_FILE_NAME:Qt5::QXcbGlxIntegrationPlugin> DESTINATION bin/xcbglintegrations/ COMPONENT OpenStudioApp)
    install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/sqldrivers/$<TARGET_FILE_NAME:Qt5::QSQLiteDriverPlugin> DESTINATION bin/sqldrivers/ COMPONENT OpenStudioApp)
    #install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/sceneparsers/$<TARGET_FILE_NAME:Qt5::GLTFSceneParserPlugin> DESTINATION bin/sceneparsers/ COMPONENT OpenStudioApp)
    #install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/sceneparsers/$<TARGET_FILE_NAME:Qt5::AssimpParserPlugin> DESTINATION bin/sceneparsers/ COMPONENT OpenStudioApp)
    install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/libQt5XcbQpa.so.5 DESTINATION bin/ COMPONENT OpenStudioApp)
    
    foreach(LIB ${QT_LIBS})
      install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/$<TARGET_SONAME_FILE_NAME:${LIB}> DESTINATION bin/ COMPONENT OpenStudioApp)
    endforeach()
    
    foreach(LIB ${QT_WEB_LIBS})
      install(PROGRAMS $<TARGET_FILE_DIR:${target_name}>/$<TARGET_SONAME_FILE_NAME:${LIB}> DESTINATION bin/ COMPONENT OpenStudioApp)
    endforeach()
  endif()
endif()

if( APPLE )
  list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/platforms/libqcocoa.dylib")
  list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/sqldrivers/libqsqlite.dylib")
  #list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/sceneparsers/libgltfsceneparser.dylib")
  #list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/sceneparsers/libassimpsceneparser.dylib")
  install(DIRECTORY ${QtWebEngineProcess} DESTINATION "OpenStudioApp.app/Contents/Frameworks/QtWebEngineCore.framework/Versions/5/Helpers/" COMPONENT OpenStudioApp USE_SOURCE_PERMISSIONS)
  list(APPEND QTPLUGINS "\${CMAKE_INSTALL_PREFIX}/OpenStudioApp.app/Contents/Frameworks/QtWebEngineCore.framework/Versions/5/Helpers/QtWebEngineProcess.app/Contents/MacOS/QtWebEngineProcess")
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
  test/Units_GTest.cpp
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
