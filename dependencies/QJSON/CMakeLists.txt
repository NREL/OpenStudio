# add_custom_command (OUTPUT ${qjson_SOURCE_DIR}/lib/json_parser.cc
#                    PRE_BUILD
#                    COMMAND bison -t -o json_parser.cc -d json_parser.yy
#                    DEPENDS json_parser.yy
#                    WORKING_DIRECTORY ${qjson_SOURCE_DIR}/lib/
#                    )

set(qjson_MOC_HDRS
  parserrunnable.h
  serializerrunnable.h
)

IF(UNIX)
  # all warnings
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -fno-strict-aliasing ")
  SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -fno-strict-aliasing --std=c99")
  # effective c++
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weffc++")
  # treat warnings as errors
  #SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
ENDIF(UNIX)

qt4_wrap_cpp(qjson_MOC_SRCS ${qjson_MOC_HDRS})

set (qjson_SRCS parser.cpp qobjecthelper.cpp json_scanner.cpp json_parser.cc parserrunnable.cpp serializer.cpp serializerrunnable.cpp)
set (qjson_HEADERS parser.h parserrunnable.h qobjecthelper.h serializer.h serializerrunnable.h qjson_export.h)

#add_library (qjson SHARED ${qjson_SRCS} ${qjson_MOC_SRCS} ${qjson_HEADERS})
#target_link_libraries( qjson ${QT_LIBRARIES})

# Let's make it static instead for our purposes

add_library (qjson STATIC ${qjson_SRCS} ${qjson_MOC_SRCS} ${qjson_HEADERS})
# target_link_libraries( qjson ${QT_LIBRARIES})

if(NOT android)
	set_target_properties(qjson PROPERTIES
	                      VERSION ${QJSON_LIB_MAJOR_VERSION}.${QJSON_LIB_MINOR_VERSION}.${QJSON_LIB_PATCH_VERSION}
                              SOVERSION ${QJSON_LIB_MAJOR_VERSION}
	                      )
endif()

# force no exports to make it a static lib (this only matters on windows)
set_target_properties(qjson PROPERTIES
                      DEFINE_SYMBOL QJSON_EXPORT
                      PUBLIC_HEADER "${qjson_HEADERS}"
                      FRAMEWORK ${OSX_FRAMEWORK}
                      )

INSTALL(TARGETS qjson EXPORT qjson-export
   LIBRARY DESTINATION ${LIB_INSTALL_DIR}
   RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin
   ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
   FRAMEWORK DESTINATION ${FRAMEWORK_INSTALL_DIR}
   PUBLIC_HEADER DESTINATION ${INCLUDE_INSTALL_DIR}/qjson
)
