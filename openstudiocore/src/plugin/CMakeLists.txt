SET( target_name openstudio_plugin )

ADD_DEFINITIONS(-DOPENSTUDIO_PLUGIN)

INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

# source files
SET( ${target_name}_SRC
  ExternalModelInterface.cpp
  ExternalModelInterface.hpp
  ExternalObjectInterface.cpp
  ExternalObjectInterface.hpp
  ExternalPlugin.cpp
  ExternalPlugin.hpp
  OSObjectInterface.cpp
  OSObjectInterface.hpp
  OSPlugin.cpp
  OSPlugin.hpp
  OSPluginThread.cpp
  OSPluginThread.hpp 
  PluginAPI.hpp
  
  ../openstudio_app/StartupView.hpp
  ../openstudio_app/StartupView.cpp
)

## ui files
#SET( ${target_name}_UI 
#)

# moc files
SET( ${target_name}_moc
  ExternalModelInterface.hpp
  ExternalObjectInterface.hpp
  ExternalPlugin.hpp
  OSObjectInterface.hpp
  OSPlugin.hpp
  OSPluginThread.hpp
  
  ../openstudio_app/StartupView.hpp
)

## resource files
#SET( ${target_name}_qrc
#)

## Qt UI generation
qt4_wrap_cpp( ${target_name}_moc_src ${${target_name}_moc} )

#qt4_wrap_ui( ${target_name}_UIS ${${target_name}_UI} )

# generate rules for building source files from the resources
#qt4_add_resources(${target_name}_qrcs ${${target_name}_qrc} )

ADD_LIBRARY( ${target_name}
  SHARED
  ${${target_name}_SRC}
  #${${target_name}_UI}
  #${${target_name}_UIS}
  ${${target_name}_moc}
  ${${target_name}_moc_src}
  #${${target_name}_qrc}
  #${${target_name}_qrcs}
)

SET( depends
  openstudio_lib
  openstudio_model
  openstudio_energyplus
  openstudio_modeleditor
  ${QT_LIBS}
)

IF( NOT APPLE )
INSTALL(TARGETS ${target_name}
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib )
ENDIF()

TARGET_LINK_LIBRARIES( ${target_name}  ${depends} )
AddPCH(${target_name})

SET( ${target_name}_swig_src
  Plugin.i
)

MAKE_SWIG_TARGET( OpenStudioPlugin plugin "${CMAKE_CURRENT_SOURCE_DIR}/Plugin.i" "${${target_name}_swig_src}" ${target_name} "OpenStudioEnergyPlus;OpenStudioModelEditor" )

