SET( target_name qtwinmigrate )

SET (CMAKE_AUTOMOC ON)

SET( ${target_name}_src 
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

SET( ${target_name}_depends
  user32
  ${QT_LIBS}
)

INCLUDE_DIRECTORIES( ${Qt5Gui_PRIVATE_INCLUDE_DIRS} )

ADD_LIBRARY( ${target_name} SHARED ${${target_name}_src} )
TARGET_LINK_LIBRARIES(${target_name} ${${target_name}_depends})

IF( NOT APPLE )
INSTALL(TARGETS ${target_name}
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib )
ENDIF()
