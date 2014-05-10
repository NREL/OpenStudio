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

set(${target_name}_depends
  user32
  ${QT_LIBS}
)

include_directories(${Qt5Gui_PRIVATE_INCLUDE_DIRS})

add_library(${target_name} SHARED ${${target_name}_src})
target_link_libraries(${target_name} ${${target_name}_depends})

if(NOT APPLE)
  install(TARGETS ${target_name}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
  )
endif()
