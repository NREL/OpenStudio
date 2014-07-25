include(${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)

set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS HAVE_LIBSQLITE3)
set(HAVE_LIBSQLITE3 1)

check_include_files(dlfcn.h HAVE_DLFCN_H)
check_include_files(inttypes.h HAVE_INTTYPES_H)
check_include_files(memory.h HAVE_MEMORY_H)
check_include_files(stdint.h HAVE_STDINT_H)
check_include_files(stdlib.h HAVE_STDLIB_H)
check_include_files(strings.h HAVE_STRINGS_H)
check_include_files(string.h HAVE_STRING_H)
check_include_files(sys/stat.h HAVE_SYS_STAT_H)
check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)

check_function_exists(malloc HAVE_MALLOC)
check_function_exists(gettimeofday HAVE_GETTIMEOFDAY)
check_function_exists(atof HAVE_ATOF)
check_function_exists(strtoll HAVE_STRTOLL)
check_function_exists(strtof  HAVE_STRTOF)
check_function_exists(localtime_r HAVE_LOCALTIME_R)

if(MSVC)
  # disable warnings
  if(CMAKE_CXX_FLAGS MATCHES "/W3")
    string(REGEX REPLACE "/W3" "/W0" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  elseif(CMAKE_CXX_FLAGS MATCHES "/W4")
    string(REGEX REPLACE "/W4" "/W0" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
  endif()
endif()

# create config.h
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake
  ${CMAKE_BINARY_DIR}/src/litesql/config.h)

# set include directories
include_directories(litesql/include "${CMAKE_BINARY_DIR}/src" "${CMAKE_SOURCE_DIR}/src/expat/lib")

##############################################################################
# litesql-util library
##############################################################################
set(litesql_util_SOURCES
  library/split.cpp
  library/string.cpp
  include/litesql/split.hpp
  include/litesql/string.hpp
)
add_library(litesql-util STATIC ${litesql_util_SOURCES})

##############################################################################
# litesql library
##############################################################################

add_definitions(-DSQLITE_THREADSAFE=1) # 1 is default, serial access
#add_definitions(-DSQLITE_DEBUG)
#add_definitions(-DSQLITE_MEMDEBUG)

set(litesql_SOURCES
  library/database.cpp
  library/expr.cpp
  library/selectquery.cpp
  library/persistent.cpp
  library/backend.cpp
  library/updatequery.cpp
  library/field.cpp
  library/datasource.cpp
  library/compatibility.hpp
  library/compatibility.cpp
  library/datetime.cpp
  include/litesql/database.hpp
  include/litesql/expr.hpp
  include/litesql/selectquery.hpp
  include/litesql/persistent.hpp
  include/litesql/backend.hpp
  include/litesql/updatequery.hpp
  include/litesql/field.hpp
  include/litesql/datasource.hpp
  include/litesql/datetime.hpp
  library/sqlite3_backend.cpp
  library/sqlite3.hpp
  library/sqlite3.c
  library/sqlite3.h
  library/sqlite3ext.h
  #${CMAKE_BINARY_DIR}/litesql/config.h
)
add_library(litesql STATIC ${litesql_SOURCES})

###############################################################################
## litesql gen
###############################################################################
set(litesql-gen_SOURCES
  generator/generator.cpp
  generator/generator.hpp
  generator/litesql-gen-xml.cpp
  generator/litesql-gen-xml.hpp
  generator/litesql-gen-cpp.cpp
  generator/litesql-gen-cpp.hpp
  generator/logger.cpp
  generator/logger.hpp
  generator/xmlobjects.cpp
  generator/xmlobjects.hpp
  generator/md5.cpp
  generator/md5.hpp
  generator/litesql-gen-graphviz.cpp
  generator/litesql-gen-graphviz.hpp
  generator/litesql-gen-ruby-activerecord.cpp
  generator/litesql-gen-ruby-activerecord.hpp
  generator/litesql-gen-main.cpp
  generator/litesql-gen-main.hpp
  generator/xmlparser.hpp
  generator/xmlparser.cpp
  generator/objectmodel.cpp
  generator/objectmodel.hpp
)
#
##include(${CMAKE_ROOT}/Modules/FindEXPAT.cmake)
#
add_executable(litesql-gen ${litesql-gen_SOURCES})
target_link_libraries(litesql-gen zkexpat litesql litesql-util)

if(MSVC)
  set(LITESQL_GEN_EXE "${CMAKE_BINARY_DIR}/Products/$(ConfigurationName)/litesql-gen" PARENT_SCOPE)
else()
  set(LITESQL_GEN_EXE "${CMAKE_BINARY_DIR}/Products/litesql-gen" PARENT_SCOPE)
endif()


add_dependencies(litesql-gen zkexpat)
