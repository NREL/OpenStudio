# This file is copyrighted under the BSD-license for buildsystem files of KDE
# copyright 2010, Patrick Spendrin <ps_ml@gmx.de>

project(zkexpat)

cmake_minimum_required(VERSION 2.6)
set(PACKAGE_BUGREPORT "zkexpat-bugs@libzkexpat.org")
set(PACKAGE_NAME "zkexpat")
set(PACKAGE_STRING "zkexpat 2.0.1")
set(PACKAGE_TARNAME "zkexpat")
set(PACKAGE_VERSION "2.0.1")

option(zkexpat_BUILD_EXAMPLES "build the examples for zkexpat library" OFF)
MARK_AS_ADVANCED( zkexpat_BUILD_EXAMPLES )

include(ConfigureChecks.cmake)

INCLUDE_DIRECTORIES("${CMAKE_CURRENT_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/lib")

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif(MSVC)

set(zkexpat_SRCS
lib/xmlparse.c
lib/xmlrole.c
lib/xmltok.c 
lib/xmltok_impl.c 
lib/xmltok_ns.c
)

if(WIN32)
    set(zkexpat_SRCS ${zkexpat_SRCS} lib/libexpat.def)
endif(WIN32)

add_library(zkexpat SHARED ${zkexpat_SRCS})

#set(xmlwf_SRCS
#xmlwf/xmlwf.c
#xmlwf/xmlfile.c
#xmlwf/codepage.c
#xmlwf/readfilemap.c
#)

# add_executable(xmlwf ${xmlwf_SRCS})
# target_link_libraries(xmlwf zkexpat)

IF( NOT APPLE )
install(TARGETS zkexpat     RUNTIME DESTINATION bin
                            LIBRARY DESTINATION lib
                            ARCHIVE DESTINATION lib)
ENDIF()

# install(FILES lib/expat.h lib/expat_external.h DESTINATION include)
# install(FILES doc/xmlwf.1 DESTINATION man/man1)


if(zkexpat_BUILD_EXAMPLES)
    add_executable(elements examples/elements.c)
    target_link_libraries(elements zkexpat)

    add_executable(outline examples/outline.c)
    target_link_libraries(outline zkexpat)
endif(zkexpat_BUILD_EXAMPLES)

#CREATE_TEST_TARGETS( "zkexpat" "tests/runtests.c tests/chardata.c tests/minicheck.c" "" )
#CREATE_TEST_TARGETS( "zkexpat_spp" "tests/runtestspp.cpp tests/chardata.c tests/minicheck.c" "" )

