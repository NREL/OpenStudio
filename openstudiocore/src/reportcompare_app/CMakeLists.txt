set(target_name ReportCompare)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

#include_directories(${RUBY_INCLUDE_DIR})
#include_directories(${SWIG_DIR})
#
#add_custom_command(
#  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.h"
#  COMMAND "${SWIG_EXECUTABLE}"
#          "-ruby"
#          -external-runtime "${CMAKE_CURRENT_BINARY_DIR}/SWIGRubyRuntime.h"
#)

# source files
set(${target_name}_SRC
main.cpp
comparedialog.cpp
dropfilebutton.cpp
main.cpp
menuwindow.cpp
)

# moc files
set(${target_name}_moc
comparedialog.h
dropfilebutton.h
menuwindow.h
)

## Qt UI generation
qt5_wrap_cpp_minimally(${target_name}_moc_src ${${target_name}_moc})

# configure the AboutBox.hpp file
# this needs to be configured to embed the build number
configure_file(AboutBox.hpp.in "${CMAKE_CURRENT_BINARY_DIR}/AboutBox.hpp")


if(WIN32)

  set(RC_BUILD_STR "1")
  
  if(NOT CMAKE_VERSION_MAJOR)
	set(CMAKE_VERSION_MAJOR "1")
  endif()
  
  if(NOT CMAKE_VERSION_MINOR)
	set(CMAKE_VERSION_MINOR "0")
  endif()
  
  if(NOT CMAKE_VERSION_PATCH)
	set(CMAKE_VERSION_PATCH "0")
  endif()
  
  if(NOT CMAKE_VERSION_BUILD STREQUAL "" AND NOT CMAKE_VERSION_BUILD MATCHES "([a-zA-Z]+)")
    set(RC_BUILD_STR "${CMAKE_VERSION_BUILD}")
  endif()
  configure_file(ReportCompareApp.rc.in "${CMAKE_CURRENT_BINARY_DIR}/ReportCompareApp.rc")
  set(ICON_SRC "${CMAKE_CURRENT_BINARY_DIR}/ReportCompareApp.rc")
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
  set(MACOSX_BUNDLE_GUI_IDENTIFIER "gov.nrel.ReportCompare.${CMAKE_VERSION_BUILD}.ReportCompare")
  set(MACOSX_BUNDLE_BUNDLE_NAME "ReportCompare")
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
endif()

## Qt UI generation
qt5_wrap_cpp_minimally(${target_name}_moc_src ${${target_name}_moc})

# generate rules for building source files from the resources
qt5_add_resources(${target_name}_qrcs ${${target_name}_qrc})

add_executable(${target_name}
  WIN32
  MACOSX_BUNDLE
  ${${target_name}_SRC}
  ${${target_name}_moc_src}
  ${${target_name}_moc}
  ${ICON_SRC}
)

#INCLUDE( ${QT_USE_FILE} )

message(">>> CMAKE_CURRENT_SOURCE_DIR : " ${CMAKE_CURRENT_SOURCE_DIR})

get_cmake_property(_variableNames VARIABLES)
foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
endforeach()

set(DIRS ${QT_LIBRARY_DIR} ${LIBRARY_SEARCH_DIRECTORY})

message(">>> CMAKE_CURRENT_SOURCE_DIR : " ${CMAKE_CURRENT_SOURCE_DIR})
message(">>> QT_WEB_LIBS : " ${QT_WEB_LIBS})
message(">>> QT_LIBS : " ${QT_LIBS})

set(depends
  ${QT_LIBS}
  ${QT_WEB_LIBS}
)

#C:\Qt\Qt5.4.1\5.4\msvc2013_opengl\lib\Qt5WebKitWidgets.lib
#C:\Qt\Qt5.4.1\5.4\msvc2013_opengl\lib\Qt5WebKit.lib
message(">>> QT_MAIN_LIB : " ${QT_MAIN_LIB})
message(">>> QT_MAIN_LIB_D : " ${QT_MAIN_LIB_D})
target_link_libraries(${target_name} optimized ${QT_MAIN_LIB} debug ${QT_MAIN_LIB_D})
target_link_libraries(${target_name} ${depends} )
#target_link_libraries(<your_app> Qt5::WinMain)
#target_link_libraries(<your_app> ${Qt5Core_QTMAIN_LIBRARIES})
  
AddPCH(${target_name}) 

#------------------------------------------------------------------------------
# Installation
#------------------------------------------------------------------------------
if(APPLE)
  set_target_properties(${target_name} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_BINARY_DIR}/Info.plist)
  set(plugin_dest_dir ReportCompare.app/Contents/MacOS)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/ReportCompare.app")
elseif(WIN32)
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/ReportCompare.exe")
else()
  set(plugin_dest_dir bin)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/ReportCompare")
endif()

install(TARGETS ReportCompare
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

  if(WIN32)
    get_target_property(QPA_PLUGIN Qt5::QWindowsIntegrationPlugin LOCATION)
    if(${Qt5Widgets_VERSION} VERSION_LESS "5.3.2")
      # Qt does not populate the windows print plugin property on 5.3.1 and earlier, so we have to do this manually
      get_filename_component(QPA_PATH ${QPA_PLUGIN} DIRECTORY)
      set(PRINT_PLUGIN "${QPA_PATH}/../printsupport/windowsprintersupport.dll")
    else()
      get_target_property(PRINT_PLUGIN Qt5::QWindowsPrinterSupportPlugin LOCATION)
    endif()
  elseif(APPLE)
    get_target_property(QPA_PLUGIN Qt5::QCocoaIntegrationPlugin LOCATION)
    # Qt does not create a mac print module in CMake, so we have to do this manually
    get_filename_component(QPA_PATH ${QPA_PLUGIN} DIRECTORY)
    set(PRINT_PLUGIN "${QPA_PATH}/../printsupport/libcocoaprintersupport.dylib")
  endif()

  install(PROGRAMS "${QPA_PLUGIN}"
          DESTINATION "${plugin_dest_dir}/platforms/"
  )
  install(PROGRAMS "${PRINT_PLUGIN}"
          DESTINATION "${plugin_dest_dir}/printsupport/"
  )

  get_filename_component(QPA_PLUGIN_NAME "${QPA_PLUGIN}" NAME)
  get_filename_component(PRINT_PLUGIN_NAME "${PRINT_PLUGIN}" NAME)

    list(APPEND QTPLUGINS
    "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/platforms/${QPA_PLUGIN_NAME}"
    "\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/printsupport/${PRINT_PLUGIN_NAME}"
  )

endif()


# directories to look for dependencies
set(DIRS ${QT_LIBRARY_DIR} ${LIBRARY_SEARCH_DIRECTORY})

if(APPLE)
  #list(APPEND DIRS "${CMAKE_BINARY_DIR}/Products/OSAppConfigInstall")

  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/reportcompare_app/Resources/
          DESTINATION ReportCompare.app/Contents/share/ReportCompare-${OPENSTUDIO_VERSION}/OSApp
  )

  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/pat_app/Measures
          DESTINATION ReportCompare.app/Contents/share/ReportCompare-${OPENSTUDIO_VERSION}/pat
  )
elseif(WIN32)
  #list(APPEND DIRS "${CMAKE_BINARY_DIR}/Products/OSAppConfigInstall/Release" "${CMAKE_BINARY_DIR}/Products/OSAppConfigInstall/Debug")

  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/reportcompare_app/Resources/
          DESTINATION share/ReportCompare-${OPENSTUDIO_VERSION}/OSApp
  )
else()
  install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/reportcompare_app/Resources/
          DESTINATION share/ReportCompare-${OPENSTUDIO_VERSION}/OSApp
  )
endif()

# Linux desktop integration
if(UNIX AND NOT APPLE AND "${CMAKE_INSTALL_PREFIX}" STREQUAL "/usr/local")
  # Install .desktop file
  configure_file(ReportCompare.desktop.in ${CMAKE_CURRENT_BINARY_DIR}/ReportCompare.desktop)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/ReportCompare.desktop DESTINATION ../share/applications)

  # Declare mime types
  install(FILES ReportCompare.xml DESTINATION ../share/mime/packages)

  # Install icons
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


    file(COPY \${PLUGINS} DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}\")

    file(GLOB_RECURSE FIXUPPLUGINS
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\"
      \"\${CMAKE_INSTALL_PREFIX}/${plugin_dest_dir}/*.bundle\"
    )

    include(BundleUtilities)

    fixup_bundle(\"${APPS}\" \"\${FIXUPPLUGINS}\" \"${DIRS}\")
  ")
endif()

set(${target_name}_test_depends
  ${depends}
)

CREATE_TEST_TARGETS(${target_name} "${${target_name}_test_src}" "${${target_name}_test_depends}")
CREATE_SRC_GROUPS("${${target_name}_test_src}")
if(BUILD_TESTING)

  #add_dependencies("${target_name}_tests"
  #  ReportCompare_app_resources
  #)

endif()
