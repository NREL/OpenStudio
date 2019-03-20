set(target_name qtwinmigrate)

include_directories(${QT_INCLUDES})

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

add_library(${target_name} ${${target_name}_src})
target_link_libraries(${target_name} ${${target_name}_depends})

target_include_directories(${target_name} PUBLIC ${QT_INCLUDES})
target_compile_definitions(${target_name} PUBLIC ${QT_DEFS})
