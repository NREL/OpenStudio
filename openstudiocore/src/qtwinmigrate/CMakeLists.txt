set(target_name qtwinmigrate)

set(CMAKE_AUTOMOC ON)

set(${target_name}_src
  #QMfcApp
  qmfcapp.cpp
  qmfcapp.h
  #QWinHost
  qwinhost.cpp
  qwinhost.h
  #QWinWidget
  qwinwidget.cpp
  qwinwidget.h
)

message("OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")
message("EGL_LIBRARIES: ${Qt5Gui_EGL_LIBRARIES}")
message("OPENGL_LIBRARIES: ${Qt5Gui_OPENGL_LIBRARIES}")
set(${target_name}_depends
  user32
  ${QT_LIBS}
)
#set_property(TARGET Qt5::Gui_EGL APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Qt5Gui_EGL_INCLUDE_DIRS})
#set_property(TARGET Qt5::Gui_GLESv2 APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Qt5Gui_OPENGL_INCLUDE_DIRS})


include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

add_library(${target_name} SHARED ${${target_name}_src})
target_link_libraries(${target_name} ${${target_name}_depends})

if(NOT APPLE)
  install(TARGETS ${target_name}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
  )
endif()
