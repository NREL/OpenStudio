SET( target_name qtwinmigrate )

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

SET( ${target_name}_moc
  qwinhost.h
  qwinwidget.h
)

qt4_wrap_cpp( ${target_name}_moc_src ${${target_name}_moc} )

SET( ${target_name}_depends
  user32
  ${QT_LIBS}
)

ADD_LIBRARY( ${target_name} SHARED ${${target_name}_src} ${${target_name}_moc_src} )
TARGET_LINK_LIBRARIES(${target_name} ${${target_name}_depends})

IF( NOT APPLE )
INSTALL(TARGETS ${target_name}
  RUNTIME DESTINATION bin 
  LIBRARY DESTINATION lib )
ENDIF()
