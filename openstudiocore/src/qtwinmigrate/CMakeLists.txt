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
)

add_library(${target_name} STATIC ${${target_name}_src})
target_link_libraries(${target_name} ${${target_name}_depends})
target_include_directories(${target_name} PUBLIC ${Qt5Gui_PRIVATE_INCLUDE_DIRS})
target_include_directories(${target_name} PUBLIC ${Qt5Gui_INCLUDE_DIRS})
target_include_directories(${target_name} PUBLIC ${Qt5Widgets_INCLUDE_DIRS})

add_library(${target_name}_static STATIC ${${target_name}_src})
target_link_libraries(${target_name}_static ${${target_name}_depends})
target_include_directories(${target_name}_static PUBLIC ${QT_STATIC_INCLUDES})
target_compile_definitions(${target_name}_static PUBLIC ${QT_DEFS})
