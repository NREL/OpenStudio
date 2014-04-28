CMAKE_MINIMUM_REQUIRED( VERSION 2.8.7 )

PROJECT(OpenStudio)

SET(CMAKE_VERSION_MAJOR 1)
SET(CMAKE_VERSION_MINOR 3)
SET(CMAKE_VERSION_PATCH 3)

SET(CMAKE_VERSION_BUILD "Unknown" CACHE STRING "Build number")
FIND_PACKAGE(Git)
IF(GIT_FOUND)
  EXECUTE_PROCESS(COMMAND "${GIT_EXECUTABLE}" "rev-parse" "--short=10" "HEAD"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  TIMEOUT 10
                  RESULT_VARIABLE RESULT
                  OUTPUT_VARIABLE GIT_VERSION
                  ERROR_QUIET
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  IF(${RESULT} EQUAL 0 AND NOT "${GIT_VERSION}" EQUAL "${CMAKE_VERSION_BUILD}")
    SET(CMAKE_VERSION_BUILD ${GIT_VERSION} CACHE STRING "Build number" FORCE) # git sha                  
  ENDIF()
  
  GET_FILENAME_COMPONENT(GIT_DIR "${GIT_EXECUTABLE}" PATH)
ELSE()
  SET(GIT_DIR "")
ENDIF()

FIND_PROGRAM(PATCH_EXE patch HINTS "${GIT_DIR}" "${GIT_DIR}/../bin/")
STRING(COMPARE EQUAL "${PATCH_EXE}" "PATCH_EXE-NOTFOUND" PATCH_EXE_NOTFOUND)
IF ($PATCH_EXE_NOTFOUND)
  MESSAGE(SEND_ERROR "Required program patch not found")
ENDIF()

# EnergyPlus Idd version
SET(ENERGYPLUS_VERSION "8.1" CACHE INTERNAL "EnergyPlus Version")

OPTION( NIGHTLY_BUILD "Use configurations for Nightly Build" OFF )

SET(OPENSTUDIO_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}")

# Search for modules in the openstudiocore dir first to override cmake ones 
LIST(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/openstudiocore/CMake" ${CMAKE_SOURCE_DIR})

# Build C++ documentation using Doxygen
# Requires: doxygen
OPTION( BUILD_DOCUMENTATION "Build Documentation" OFF )

IF(BUILD_DOCUMENTATION)
  Find_Package( Doxygen REQUIRED )
ENDIF()

# Build CSharp bindings
# Requires: SWIG
OPTION( BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF )

# Build Java bindings
# Requires: SWIG
OPTION( BUILD_JAVA_BINDINGS "Build Java bindings" OFF )

# Build Node V8 bindings
# Requires: SWIG
OPTION( BUILD_V8_BINDINGS "Build V8 bindings" OFF )

# Build Python bindings
# Requires: SWIG Python
OPTION( BUILD_PYTHON_BINDINGS "Build Python bindings" OFF )

# Build ctest testing
# Requires: EnergyPlus
OPTION( BUILD_TESTING "Build testing targets" OFF )

# Build package
# Requires: EnergyPlus
OPTION( BUILD_PACKAGE "Build package" OFF )

# Build test runner targets.
# This is a convenience for Visual Studio users
OPTION( ENABLE_TEST_RUNNER_TARGETS "Create test runner targets" OFF )

# Build with OpenSSL support
SET( BUILD_WITH_OPENSSL ON CACHE INTERNAL "Build With OpenSSL Support For SSH Connections" )
IF(UNIX)
  FIND_PACKAGE( OpenSSL )
  IF( NOT ${OPENSSL_FOUND} )
    MESSAGE(SEND_ERROR "OpenSSL could not be found, and is required for HTTPS communication")
    MESSAGE(SEND_ERROR "Please install OpenSSL development libraries using your package management system (possibly libssl-dev)")
  ELSE()
    MARK_AS_ADVANCED(LIB_EAY_DEBUG)
    MARK_AS_ADVANCED(LIB_EAY_RELEASE)
    MARK_AS_ADVANCED(SSL_EAY_DEBUG)
    MARK_AS_ADVANCED(SSL_EAY_RELEASE)
  ENDIF()
ELSEIF(WIN32)
  IF(CMAKE_CL_64)
    SET(OPENSSL_ARCH 64)
    SET(OPENSSL_EXPECTED_HASH b63ff40c9d4f94aef43e3c4d8085a5c5)
  ELSE()
    SET(OPENSSL_ARCH 32)
    SET(OPENSSL_EXPECTED_HASH 507b6a190e282ab3018ee4a69371f6d8)
  ENDIF()
  SET(OPENSSL_VERSION "1.0.1g")
  SET(OPENSSL_PATH "OpenSSL-Win${OPENSSL_ARCH}")
  IF(EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip")
    FILE(MD5 "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" OPENSSL_HASH)
  ENDIF()
  IF(NOT EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" OR NOT "${OPENSSL_HASH}" MATCHES "${OPENSSL_EXPECTED_HASH}")
    MESSAGE(STATUS "Downloading OpenSSL ${OPENSSL_VERSION} (${OPENSSL_ARCH}-bit)")
    FILE(DOWNLOAD "http://developer.nrel.gov/downloads/buildings/openstudio/src/OpenSSL-Win${OPENSSL_ARCH}-${OPENSSL_VERSION}.zip" "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${OPENSSL_EXPECTED_HASH})
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
    EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  ENDIF()
  SET(OPENSSL_ROOT_DIR "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
ENDIF()



IF(MSVC)
  # Visual Studio information for the user
  if    (${CMAKE_C_COMPILER_VERSION} VERSION_LESS "15.00.21022.08")
    message(FATAL_ERROR "Visual Studio earlier than VS2008 is not supported")
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "15.00.21022.08")
    message(WARNING "Service Pack 1 for Visual Studio 2008 is strongly recommended")
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "15.00.30729.01")
    # vc90sp1
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "16.00.30319.01")
    message(WARNING "Service Pack 1 for Visual Studio 2010 is strongly recommended")
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "16.00.40219.01")
    # vc100sp1
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_GREATER "16.00.40219.01")
    message(WARNING "Visual Studio newer than VS2010 is not currently supported")
  endif()

  # Build with Multiple Processes
  SET(BUILD_WITH_MULTIPLE_PROCESSES ON CACHE BOOL "/MP compiler flag for full processor utilization")
  MARK_AS_ADVANCED( BUILD_WITH_MULTIPLE_PROCESSES )
ENDIF()

# EnergyPlus
FIND_PACKAGE(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED)
MARK_AS_ADVANCED(
  ENERGYPLUS_EXE
  ENERGYPLUS_IDD
  ENERGYPLUS_WEATHER_DIR
)

# Package Radiance and EnergyPlus for convenience 
OPTION( PACKAGE_RADIANCE_AND_ENERGYPLUS "Package Radiance and EnergyPlus" OFF )
MARK_AS_ADVANCED(FORCE PACKAGE_RADIANCE_AND_ENERGYPLUS)

IF (PACKAGE_RADIANCE_AND_ENERGYPLUS)
  MESSAGE(WARNING "Packaging of Radiance and EnergyPlus is intended for internal use and deployment only")
  SET(ENERGYPLUS_LOCATION "" CACHE PATH "Location of EnergyPlus to Package")
  SET(RADIANCE_LOCATION "" CACHE PATH "Location of Radiance to Package")
ENDIF()

# Build Ruby gem
OPTION( BUILD_RUBY_GEM "Build Ruby Gem" OFF )

IF (BUILD_V8_BINDINGS)
  IF(WIN32)
    MESSAGE(SEND_ERROR "V8 is only tested and supported on Unix like systems")
  ENDIF()

  OPTION( BUILD_NODE_MODULES "Build V8 Bindings as Node Modules" ON )

  IF ( BUILD_NODE_MODULES )
    # Build Node for linking to Node modules
    OPTION( BUILD_NODE "Build Node" ON )
  ELSE()
    OPTION( BUILD_V8 "Build V8" ON )
  ENDIF()

ENDIF()

IF (BUILD_RUBY_GEM)
  # Build Ruby gem and package up all the libs
  OPTION( BUILD_RUBY_GEM_WITH_LIBS "Package all required libs with the Ruby Gem" ON )

  SET(BUILD_PACKAGE ON)
  SET(CPACK_BINARY_DEB OFF)
  SET(CPACK_BINARY_NSIS OFF)
  SET(CPACK_BINARY_RPM OFF)
  SET(CPACK_BINARY_STGZ OFF)
  SET(CPACK_BINARY_TBZ2 OFF)
  SET(CPACK_BINARY_TZ OFF)
  SET(CPACK_BINARY_TGZ ON)

  IF(CPACK_BINARY_DEB OR CPACK_BINARY_NSIS OR CPACK_BINARY_RPM OR CPACK_BINARY_STGZ
      OR CPACK_BINARY_TBZ2 OR CPACK_BINARY_TZ OR NOT CPACK_BINARY_TGZ OR NOT BUILD_PACKAGE)
    MESSAGE(FATAL_ERROR "To build the Ruby Gem please enable ONLY CPACK_BINARY_TGZ of the cpack binary options")
  ENDIF()

  MESSAGE("Enabling Ruby Gem overrides all of your packaging options. To build the Ruby Gem please type 'make package'")
ENDIF()

# Require that DAKOTA_FOUND
#
# Some code (certain analysisdriver tests) are conditionally compiled depending on 
# whether Dakota is found on the system. If this variable is set to true, CMake will give a configure
# error in the case that Dakota is not found.
#
# Dakota is an optional third party algorithm library that we use through its command line
# interface.
OPTION( REQUIRE_DAKOTA "Ensure Dakota (third party analysis tool) is found" OFF )
MARK_AS_ADVANCED( REQUIRE_DAKOTA )

IF(MSVC)
  # need to know if using the Express edition of Visual Studio, devenv is split into multiple programs
  OPTION( MSVC_IS_EXPRESS "using the Express editions of Visual Studio, must install both C++ and C# versions" FALSE )
  
  MARK_AS_ADVANCED(CMAKE_CONFIGURATION_TYPES)
ENDIF()

# Use PCH
OPTION( USE_PCH "Use precompiled headers" OFF )

FIND_PACKAGE (Qt5Widgets) #Qt5Core, Qt5Gui, Qt5Widgets
FIND_PACKAGE (Qt5Sql) #Qt5Core, Qt5Sql
FIND_PACKAGE (Qt5Network) #Qt5Core, Qt5Network
FIND_PACKAGE (Qt5Xml) #Qt5Core, Qt5Xml
FIND_PACKAGE (Qt5WebKit) #Qt5Core, Qt5Gui, Qt5Network, Qt5WebKit
FIND_PACKAGE (Qt5WebKitWidgets) #Qt5Core, Qt5Gui, Qt5Multimedia, Qt5MultimediaWidgets, Qt5Network, Qt5OpenGL, Qt5Positioning, Qt5PrintSupport, Qt5Qml, Qt5Quick, Qt5Sensors, Qt5WebKit, Qt5WebKitWidgets, Qt5Widgets
#FIND_PACKAGE (Qt5WinExtras) #Qt5Core, Qt5Gui, Qt5WinExtras
FIND_PACKAGE (Qt5Concurrent) #Qt5Concurrent, Qt5Core
MARK_AS_ADVANCED(Qt5Concurrent_DIR)
MARK_AS_ADVANCED(Qt5Core_DIR)
MARK_AS_ADVANCED(Qt5Gui_DIR)
MARK_AS_ADVANCED(Qt5Gui_EGL_LIBRARY)
MARK_AS_ADVANCED(Qt5Multimedia_DIR)
MARK_AS_ADVANCED(Qt5MultimediaWidgets_DIR)
MARK_AS_ADVANCED(Qt5Network_DIR)
MARK_AS_ADVANCED(Qt5OpenGL_DIR)
MARK_AS_ADVANCED(Qt5Positioning_DIR)
MARK_AS_ADVANCED(Qt5PrintSupport_DIR)
MARK_AS_ADVANCED(Qt5Qml_DIR)
MARK_AS_ADVANCED(Qt5Quick_DIR)
MARK_AS_ADVANCED(Qt5Sensors_DIR)
MARK_AS_ADVANCED(Qt5Sql_DIR)
MARK_AS_ADVANCED(Qt5WebKit_DIR)
MARK_AS_ADVANCED(Qt5WebKitWidgets_DIR)
MARK_AS_ADVANCED(Qt5Widgets_DIR)
#MARK_AS_ADVANCED(Qt5WinExtras_DIR)
MARK_AS_ADVANCED(Qt5Xml_DIR)

# Build Qt
IF(Qt5Widgets_DIR)
  OPTION( BUILD_QT "Build Qt" OFF)

  IF(${Qt5Widgets_VERSION} VERSION_LESS "5.1.1")
    MESSAGE(FATAL_ERROR "Minimum supported Qt is v5.1.1")
  ENDIF()
ELSE()
  OPTION( BUILD_QT "Build Qt" ON)
ENDIF()

# Apple Sanity Check
IF(APPLE)
  IF (NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    MESSAGE(FATAL_ERROR "OSX Deployment Target Must Be Set")
  ENDIF()

  IF (NOT CMAKE_OSX_SYSROOT)
    MESSAGE(FATAL_ERROR "OSX Sysroot must be set")
  ENDIF()

  IF (CMAKE_OSX_DEPLOYMENT_TARGET MATCHES ".*10\\.9.*" OR CMAKE_OSX_SYSROOT MATCHES ".*10\\.9.*")
    MESSAGE(WARNING "You are building against the 10.9 SDK which means ruby 2.0")
  ENDIF()

ENDIF()

include(ExternalProject)
include(ProcessorCount)

ProcessorCount(CPUCOUNT)
if(CPUCOUNT EQUAL 0)
  SET(CPUCOUNT "1")
endif()

MARK_AS_ADVANCED(Boost_DIR)
MARK_AS_ADVANCED(Boost_LIB_DIAGNOSTIC_DEFINITIONS)
MARK_AS_ADVANCED(ProcessorCount_cmd_getconf)
MARK_AS_ADVANCED(ProcessorCount_cmd_sysctl)

# Boost
IF(APPLE)
  set(Boost_USE_STATIC_LIBS ON)  
ENDIF()
IF (WIN32)
  set(Boost_USE_STATIC_LIBS ON)  

  #uncomment all of this if we want to force dynamic libs on windows
#  set(Boost_USE_STATIC_LIBS OFF)  
#  ADD_DEFINITIONS( -DBOOST_THREAD_USE_DLL -DBOOST_THREAD_DYN_LINK -DBOOST_PROGRAM_OPTIONS_DYN_LINK  -DBOOST_REGEX_DYN_LINK  -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_SYSTEM_DYN_LINK -DBOOST_SERIALIZATION_DYN_LINK  -DBOOST_DATE_TIME_DYN_LINK)
#  LINK_DIRECTORIES( ${Boost_LIBRARY_DIRS} )
#  IF (MSVC)
#    #Ignore dll specific warnings that are out of our hands to control, coming from external projects
#    ADD_DEFINITIONS( "/wd4251 /wd4275" )
#  ENDIF (MSVC)
ENDIF (WIN32)

# DLM: should we allow boost to be linked dynamically on Unix or should we force static for all platforms?

SET( Boost_ADDITIONAL_VERSIONS "1.55")
FIND_PACKAGE( Boost 1.55.0 COMPONENTS filesystem regex program_options system thread date_time serialization log)

IF(Boost_THREAD_FOUND)
  OPTION( BUILD_BOOST "Build Boost" OFF)
ELSE()
  OPTION( BUILD_BOOST "Build Boost" ON)
ENDIF()


FIND_PACKAGE( SWIG 1.3.40 )
MARK_AS_ADVANCED(  
  SWIG_DIR
  SWIG_EXECUTABLE
  SWIG_VERSION
)

IF(SWIG_FOUND AND NOT BUILD_V8_BINDINGS)
  OPTION(BUILD_SWIG "Build SWIG" OFF)
ELSE()
  OPTION(BUILD_SWIG "Build SWIG" ON)
ENDIF()

FIND_PROGRAM( RUBY_EXECUTABLE NAMES ruby PATHS $ENV{CMAKE_RUBY_PATH}  NO_SYSTEM_ENVIRONMENT_PATH )
FIND_PACKAGE( Ruby )


IF(WIN32 AND MSVC AND CMAKE_CL_64)
  MESSAGE(WARNING "64bit Support on Windows is EXPERIMENTAL. Ruby 2.0.0 will be built. The Sketchup plugin will not work. Expect problems with ruby support in general. Unless you've already built Boost and Qt you probably want to let this super build do it. There are lots of warnings, so we are disabling warnings as errors.") 
ENDIF()

IF (UNIX)
  IF(RUBY_FOUND)
    OPTION(BUILD_RUBY "Build Ruby" OFF)
  ELSE()
    OPTION(BUILD_RUBY "Build Ruby" ON)
  ENDIF()
ELSE()
  # Always provide ruby on Windows
  OPTION(BUILD_RUBY "Build Ruby" ON)
  MARK_AS_ADVANCED(BUILD_RUBY)
  
  IF(WIN32 AND MSVC)
    IF(CMAKE_CL_64)
      MESSAGE(WARNING "Cannot use 32bit mingw build of Ruby 2.0 with 64bit build of OpenStudio, forcing 64bit msvc build of Ruby 2.0") 
    ELSE()
      OPTION(BUILD_RUBY_MINGW32_2_0 "Use mingw32 Ruby 2.0" OFF)
      MARK_AS_ADVANCED(BUILD_RUBY_MINGW32_2_0)
    ENDIF()
  ENDIF() 
ENDIF()

IF ( NOT BUILD_RUBY AND NOT BUILD_SWIG )
  SET(RUBY_VER "${RUBY_VERSION_MAJOR}.${RUBY_VERSION_MINOR}.${RUBY_VERSION_PATCH}")

  IF(${RUBY_VER} VERSION_GREATER "1.8.8"
      AND ${SWIG_VERSION} VERSION_LESS "2.0.10")
    MESSAGE(SEND_ERROR "For Ruby >= 1.9.0, SWIG >= 2.0.10 is required. Use of the BUILD_SWIG option is suggested")
  ENDIF()
ENDIF()

IF(BUILD_WITH_MULTIPLE_PROCESSES)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
ENDIF()

IF(CMAKE_CL_64 OR (MSVC AND BUILD_QT))
  IF(CMAKE_CL_64)
    SET(NMAKE_ARCH "/amd64")
  ELSE()
    SET(NMAKE_ARCH "")
  ENDIF()
  IF(MSVC10)
    FIND_PROGRAM(NMAKE_EXE nmake PATHS
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\10.0;InstallDir]/../../VC/bin${NMAKE_ARCH}"
      $ENV{VS100COMNTOOLS}../../VC/bin${NMAKE_ARCH}
    )
  ELSEIF(MSVC12)
    FIND_PROGRAM(NMAKE_EXE nmake PATHS
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\12.0;InstallDir]/../../VC/bin${NMAKE_ARCH}"
      $ENV{VS120COMNTOOLS}../../VC/bin${NMAKE_ARCH}
    )
  ENDIF()

  IF(NMAKE_EXE STREQUAL "NMAKE_EXE-NOTFOUND" OR NMAKE_EXE STREQUAL "nmake")
    MESSAGE(WARNING "NMake not found - CMake will attempt to call nmake directly")
    SET(NMAKE_EXE "nmake" CACHE STRING "nmake executable" FORCE)
  ENDIF()
ENDIF()

IF(BUILD_NODE)
  # OSX can work with a static ruby library
  ExternalProject_Add(Node
    URL http://nodejs.org/dist/v0.10.15/node-v0.10.15.tar.gz
    CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && sh -c "./configure --prefix=${CMAKE_BINARY_DIR}/Node-prefix/src/Node-install"
    BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && $(MAKE)
    INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && $(MAKE) install && $(MAKE) install-includes && ../Node-install/bin/npm install jasmine-node node-gyp -g && ../Node-install/bin/node ../Node-install/bin/node-gyp install
    )
  SET(NODE_BIN_DIR ${CMAKE_BINARY_DIR}/Node-prefix/src/Node-install/bin/)
  SET(NODE_INCLUDE_DIR "$ENV{HOME}/.node-gyp/0.10.15")
ENDIF()

IF(BUILD_V8)
  ExternalProject_Add(V8
    SVN_REPOSITORY http://v8.googlecode.com/svn/branches/3.19
    CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/V8-prefix/src/V8 && $(MAKE) dependencies
    BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/V8-prefix/src/V8 && $(MAKE) x64
    INSTALL_COMMAND ""
    )
  SET(V8_INCLUDE_DIR ${CMAKE_BINARY_DIR}/V8-prefix/src/V8/include)
ENDIF()

IF(BUILD_RUBY)
  IF (APPLE)
    # OSX can work with a static ruby library
    ExternalProject_Add(Ruby 
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-1.8.7-p370.tar.gz
      URL_MD5 98b00bbd1cdde3116155edb6e555b781
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && sh -c "./configure --prefix=${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install --without-tcl --without-tk  CFLAGS=\"-arch i386 -arch x86_64 -fPIC\" LDFLAGS=\"-arch i386 -arch x86_64\""
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && $(MAKE)
      INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && $(MAKE) install
      )
    SET(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/ruby)
  ELSEIF (UNIX)
    # but linux cannot, it has a double free when unloading a ruby module
    ExternalProject_Add(Ruby 
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-1.8.7-p370.tar.gz
      URL_MD5 98b00bbd1cdde3116155edb6e555b781
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && sh -c "./configure --enable-shared --prefix=${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install CFLAGS=\"-fPIC\" "
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && $(MAKE)
      INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && $(MAKE) install
      )
    SET(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/ruby)
  ELSE()

    IF(CMAKE_CL_64)
      find_program(NMAKE_EXE nmake)

      # Ruby 1.8 cannot be built for 64bit windows because of faulty assumptions in the code,
      # and Ruby 2.0 isn't shipped in any way we can link to it, so for now we build our own
      ExternalProject_Add(Ruby
        URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0-p353.zip
        URL_MD5 0f8aeb1f1b1cd606ab9790badabd0fb4
        PATCH_COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/mkexports.rb ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/win32/mkexports.rb
        CONFIGURE_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-build && ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/win32/configure.bat --prefix=${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install --target=x64-mswin64 --disable-install-doc --disable-win95"
        BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-build && ${NMAKE_EXE}
        INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-build && ${NMAKE_EXE} install
        )
      SET(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/ruby.exe )
      SET(RUBY_DLL x64-msvcr100-ruby200.dll)
      SET(RUBY_DLLPATH "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/${RUBY_DLL}")
      SET(RUBY_LIBRARY "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/x64-msvcr100-ruby200.lib")
    ELSE()
      IF (BUILD_RUBY_MINGW32_2_0)
        # And windows requires mingw to build ruby, so we just unpackage it locally
        ExternalProject_Add(Ruby
          URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0p353-aws-win32.zip
          URL_MD5 1f6e4348099b26e92909db62dcd3b173
          PATCH_COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/ruby/mingw32-ruby2.0/msvcrt-ruby200.lib  ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/lib COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/ruby/mingw32-ruby2.0/config.h ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/include/ruby-2.0.0/i386-mingw32/ruby COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/ruby/mingw32-ruby2.0/missing.h ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/include/ruby-2.0.0/ruby COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/ruby/mingw32-ruby2.0/ruby.h ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/include/ruby-2.0.0/ruby
          CONFIGURE_COMMAND ""
          BUILD_COMMAND ""
          INSTALL_COMMAND ""
          )
        SET(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/ruby.exe )
        SET(RUBY_DLL msvcrt-ruby200.dll)
        SET(RUBY_DLLPATH "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/${RUBY_DLL}")
        SET(RUBY_LIBRARY "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/lib/msvcrt-ruby200.lib")
      ELSE()
        # And windows requires mingw to build ruby, so we just unpackage it locally
        ExternalProject_Add(Ruby
          URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-1.8.6-msvc-ssl.zip
          URL_MD5 d3fdb49726e63f123941f5f41f594149
          CONFIGURE_COMMAND ""
          BUILD_COMMAND ""
          INSTALL_COMMAND ""
          )
        SET(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/ruby.exe )
        SET(RUBY_DLL msvcrt-ruby18.dll)
        SET(RUBY_DLLPATH "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/${RUBY_DLL}")
        SET(RUBY_LIBRARY "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/lib/msvcrt-ruby18.lib") 
      ENDIF()
    ENDIF()
    
    
  ENDIF()
ENDIF() 


IF(BUILD_SWIG)
  IF (UNIX)
    IF (BUILD_V8_BINDINGS)
      MESSAGE("Building SWIG for V8, the latest source will be cloned from git://github.com/lefticus/swig-v8.git. Autoconf tools are required")

      FIND_PROGRAM(autoconf "autoconf")

      IF (autoconf STREQUAL "autoconf_NOTFOUND")
        MESSAGE(FATAL_ERROR "autoconf is required to compile SWIG with V8 support")
      ELSE()
        GET_FILENAME_COMPONENT(autoconf_path "${autoconf}" PATH)
        MESSAGE("autoconf directory: ${autoconf_path}")
      ENDIF()



      # We patch it up with a version of pcre we provide to avoid having to have the requirement locally
      ExternalProject_Add( SWIG
        GIT_REPOSITORY git://github.com/lefticus/swig-v8.git
        GIT_TAG devel
        # URL http://github.com/lefticus/swig-v8/archive/devel.zip
        PATCH_COMMAND cp ${CMAKE_SOURCE_DIR}/dependencies/pcre-8.31.tar.bz2 ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && ../SWIG/Tools/pcre-build.sh
        CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG && /bin/sh -c "PATH=\$PATH:${autoconf_path} ./autogen.sh" && cd - && ../SWIG/configure --prefix=${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install --disable-ccache
        BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && $(MAKE)
        INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && $(MAKE) install
        )
      SET(SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install/bin/swig )
    ELSE()
      # We patch it up with a version of pcre we provide to avoid having to have the requirement locally
      ExternalProject_Add( SWIG
        URL http://developer.nrel.gov/downloads/buildings/openstudio/src/swig-2.0.10.tar.gz
        #URL http://downloads.sourceforge.net/project/swig/swig/swig-3.0.0/swig-3.0.0.tar.gz
        URL_MD5 6d5e7ad05b4a404e5e85db9befb70c9a

        PATCH_COMMAND cp ${CMAKE_SOURCE_DIR}/dependencies/pcre-8.31.tar.bz2 ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && ../SWIG/Tools/pcre-build.sh
        CONFIGURE_COMMAND ../SWIG/configure --prefix=${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install
        BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && $(MAKE)
        INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && $(MAKE) install
        )
      SET(SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install/bin/swig )
    ENDIF()
  ELSE()
    # SWIG requires MinGW to compile on windows, so we just copy in the prebuilt binary
    ExternalProject_Add( SWIG
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/swigwin-2.0.10.zip
      URL_MD5 cbb7006ecc912f056a2ec7f322fe59fb
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      )
    SET(SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG/swig.exe )
  ENDIF()
ENDIF() 

IF(BUILD_BOOST)
  IF(APPLE)
    IF (CMAKE_OSX_DEPLOYMENT_TARGET MATCHES ".*10\\.9.*")
      set(apple-frameworks "-stdlib=libc++")
    ELSE()
      set(apple-frameworks "-stdlib=libstdc++")
    ENDIF()
    ExternalProject_Add( Boost
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/boost_1_55_0.tar.gz
      URL_MD5 93780777cfbf999a600f62883bd54b17
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./bootstrap.sh
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/Boost/xcode_51a.diff < ${CMAKE_SOURCE_DIR}/dependencies/Boost/xcode_51b.diff
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./b2 toolset=clang cxxflags=${apple-frameworks} linkflags=${apple-frameworks} variant=debug,release address-model=32_64 architecture=x86 define=BOOST_CHRONO_HEADER_ONLY --layout=tagged --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-serialization --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT} install
      INSTALL_COMMAND ""
      )
    SET(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
  ELSEIF(UNIX)
    ExternalProject_Add( Boost
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/boost_1_55_0.tar.gz
      URL_MD5 93780777cfbf999a600f62883bd54b17
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./bootstrap.sh
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./b2 cxxflags=-fPIC link=static variant=debug,release define=BOOST_CHRONO_HEADER_ONLY --layout=tagged --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-serialization --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT} install
      INSTALL_COMMAND ""
      )
    SET(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
  ELSE()
    # The call to cmd and VS_UNICODE_OUTPUT is to fix an oddity where boost configuration complains about missing ICU when it shouldn't 
    # be trying to find it
    
    SET(B2_TOOLSET "")
    IF(MSVC90)
      SET(B2_TOOLSET "toolset=msvc-9.0")
    ELSEIF(MSVC10)
      SET(B2_TOOLSET "toolset=msvc-10.0")
    ELSEIF(MSVC12)
      SET(B2_TOOLSET "toolset=msvc-12.0")
    ENDIF(MSVC90)

    IF(CMAKE_CL_64)
      ExternalProject_Add( Boost
        URL http://developer.nrel.gov/downloads/buildings/openstudio/src/boost_1_55_0.tar.gz
        URL_MD5 93780777cfbf999a600f62883bd54b17
        CONFIGURE_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && bootstrap.bat"
        PATCH_COMMAND "${PATCH_EXE}" "${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost/boost/archive/iterators/transform_width.hpp" "${CMAKE_SOURCE_DIR}/dependencies/Boost/transform_width.diff"
        BUILD_COMMAND cmd /C "set VS_UNICODE_OUTPUT=& cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && b2 ${B2_TOOLSET} link=static address-model=64 define=BOOST_CHRONO_HEADER_ONLY --build-type=minimal --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-serialization --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT}" install
        INSTALL_COMMAND ""
        )
    ELSE()
      ExternalProject_Add( Boost
        URL http://developer.nrel.gov/downloads/buildings/openstudio/src/boost_1_55_0.tar.gz
        URL_MD5 93780777cfbf999a600f62883bd54b17
        CONFIGURE_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && bootstrap.bat"
        PATCH_COMMAND "${PATCH_EXE}" "${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost/boost/archive/iterators/transform_width.hpp" "${CMAKE_SOURCE_DIR}/dependencies/Boost/transform_width.diff"
        BUILD_COMMAND cmd /C "set VS_UNICODE_OUTPUT=& cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && b2 ${B2_TOOLSET} link=static address-model=32 define=BOOST_CHRONO_HEADER_ONLY --build-type=minimal --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-serialization --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT}" install
        INSTALL_COMMAND ""
        )
    ENDIF()

    SET(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
  ENDIF()
ENDIF()

ExternalProject_Add( GoogleTest
  URL http://developer.nrel.gov/downloads/buildings/openstudio/src/gtest-1.7.0.tar.gz
  URL_MD5 bad74f626d18f724cad4c9fe2e6ef27d
  PATCH_COMMAND "${CMAKE_COMMAND}" -E copy_directory ${CMAKE_SOURCE_DIR}/dependencies/GoogleTest ${CMAKE_BINARY_DIR}/GoogleTest-prefix/src/GoogleTest
  INSTALL_COMMAND ""
  CMAKE_CACHE_ARGS
  -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
  -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
  -DCMAKE_OSX_SYSROOT:STRING=${CMAKE_OSX_SYSROOT}
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
  -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
  -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
  -DCMAKE_MODULE_LINKER_FLAGS:STRING=${CMAKE_MODULE_LINKER_FLAGS}
  -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
   )

SET(GTEST_INCLUDE_DIR ${CMAKE_BINARY_DIR}/GoogleTest-prefix/src/GoogleTest/include)
SET(GTEST_LIB_DIR ${CMAKE_BINARY_DIR}/GoogleTest-prefix/src/GoogleTest-build)

IF(BUILD_QT)
  IF(APPLE)
    # We're going to explicitly use the SDK specified in the configuration by the user
    #SET(APPLE_SDK_COMMAND "-sdk \"${CMAKE_OSX_SYSROOT}\"" )
    SET(APPLE_SDK_COMMAND "")

    IF (CMAKE_OSX_SYSROOT MATCHES ".*10.7.sdk")
      SET(APPLE_PATCH_COMMAND "${CMAKE_COMMAND}" -E rename "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/src/3rdparty/webkit/WebKitLibraries/libWebKitSystemInterfaceMountainLion.a" "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/src/3rdparty/webkit/WebKitLibraries/libWebKitSystemInterfaceMountainLion.a.bak" COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/src/3rdparty/webkit/WebKitLibraries/libWebKitSystemInterfaceLion.a" "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/src/3rdparty/webkit/WebKitLibraries/libWebKitSystemInterfaceMountainLion.a")
    ELSE()
      SET(APPLE_PATCH_COMMAND "")
    ENDIF()

    # At the moment we are building all of debug,release,x86 and x86_64
    # it would appear that calling "make" in some way that CMake can detect that we are calling
    # make (and attempt to pass parameters to the child make jobs) is breaking our ability to install
    # the below code circumvents that by calling make through a call to "sh"
    MESSAGE(WARNING "Qt5 super build is currently untested")
    ExternalProject_Add( Qt
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/qt-everywhere-opensource-src-5.2.1.tar.gz
      URL_MD5 a78408c887c04c34ce615da690e0b4c8
      CONFIGURE_COMMAND /bin/sh -c "cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/ && ./configure -v ${APPLE_SDK_COMMAND} -debug-and-release -opensource -openssl -qt-sql-sqlite -plugin-sql-sqlite -nomake examples -nomake tests -confirm-license -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install"
      PATCH_COMMAND ${APPLE_PATCH_COMMAND}
      BUILD_COMMAND  /bin/sh -c "cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt && make"
      INSTALL_COMMAND /bin/sh -c "cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt && make -j1 install"
      )
  ELSEIF(UNIX)
    MESSAGE(WARNING "Qt5 super build is currently untested")
    ExternalProject_Add( Qt
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/qt-everywhere-opensource-src-5.2.1.tar.gz
      URL_MD5 a78408c887c04c34ce615da690e0b4c8
      CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/configure -debug-and-release -opensource -confirm-license -shared -qt-sql-sqlite -no-qt3support -openssl -nomake examples -nomake demos -nomake docs -plugindir ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/share/openstudio/qtplugins -qtlibinfix OpenStudio -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install
      BUILD_COMMAND $(MAKE) -f ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-build/Makefile
      INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-build && $(MAKE) install
      )
  ELSE()
    MESSAGE(FATAL_ERROR "Qt5 cannot currently be built for Windows with the super build")
    #find_program(NMAKE_EXE nmake)

    #ExternalProject_Add( Qt
    #  URL http://developer.nrel.gov/downloads/buildings/openstudio/src/qt-everywhere-opensource-src-5.2.1.tar.gz
    #  URL_MD5 a78408c887c04c34ce615da690e0b4c8
    #  CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt && configure -debug-and-release -opensource -confirm-license -shared -plugin-sql-sqlite -no-qt3support -nomake examples -nomake docs -nomake demos -mp -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install -L ${OPENSSL_ROOT_DIR}/lib -I ${OPENSSL_ROOT_DIR}/include -openssl
    #  BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt && ${NMAKE_EXE}
    #  INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt && ${NMAKE_EXE} install
    #  )
  ENDIF()

  #Update this for Windows after BUILD_QT is updated for WINDOWS
  IF(UNIX)
    LIST(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake)
  ENDIF()
ENDIF()


set(OpenStudioCore_DIR ${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build)

# Set up the dependencies for OpenStudioCore
SET(OpenStudioCore_DEPENDS GoogleTest)

IF(BUILD_QT)
  LIST(APPEND OpenStudioCore_DEPENDS Qt)
ENDIF()

IF(BUILD_SWIG)
  LIST(APPEND OpenStudioCore_DEPENDS SWIG)
ENDIF()

IF(BUILD_BOOST)
  LIST(APPEND OpenStudioCore_DEPENDS Boost)
ENDIF()

IF(BUILD_RUBY)
  LIST(APPEND OpenStudioCore_DEPENDS Ruby)
ENDIF()

IF(BUILD_NODE)
  LIST(APPEND OpenStudioCore_DEPENDS Node)
ENDIF()

IF(BUILD_V8)
  LIST(APPEND OpenStudioCore_DEPENDS V8)
ENDIF()


ExternalProject_Add( OpenStudioCore
  DEPENDS ${OpenStudioCore_DEPENDS}
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/openstudiocore
  CMAKE_CACHE_ARGS
    -DGTEST_LIB_DIR:PATH=${GTEST_LIB_DIR}
    -DGTEST_INCLUDE_DIR:PATH=${GTEST_INCLUDE_DIR}
    -DCMAKE_OSX_ARCHITECTURES:STRING=${CMAKE_OSX_ARCHITECTURES}
    -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
    -DCMAKE_OSX_SYSROOT:STRING=${CMAKE_OSX_SYSROOT}
    -DENERGYPLUS_VERSION:STRING=${ENERGYPLUS_VERSION}
    -DBUILD_DOCUMENTATION:BOOL=${BUILD_DOCUMENTATION}
    -DBUILD_CSHARP_BINDINGS:BOOL=${BUILD_CSHARP_BINDINGS}
    -DBUILD_JAVA_BINDINGS:BOOL=${BUILD_JAVA_BINDINGS}
    -DBUILD_V8_BINDINGS:BOOL=${BUILD_V8_BINDINGS}
    -DBUILD_NODE_MODULES:BOOL=${BUILD_NODE_MODULES}
    -DBUILD_PYTHON_BINDINGS:BOOL=${BUILD_PYTHON_BINDINGS}
    -DBUILD_TESTING:BOOL=${BUILD_TESTING}
    -DBUILD_PACKAGE:BOOL=${BUILD_PACKAGE}
    -DENABLE_TEST_RUNNER_TARGETS:BOOL=${ENABLE_TEST_RUNNER_TARGETS}
    -DBUILD_WITH_MULTIPLE_PROCESSES:BOOL=${BUILD_WITH_MULTIPLE_PROCESSES}
    -DBUILD_WITH_OPENSSL:BOOL=${BUILD_WITH_OPENSSL}
    -DBUILD_RUBY_GEM:BOOL=${BUILD_RUBY_GEM}
    -DBUILD_RUBY_GEM_WITH_LIBS:BOOL=${BUILD_RUBY_GEM_WITH_LIBS}
    -DREQUIRE_DAKOTA:BOOL=${REQUIRE_DAKOTA}
    -DMSVC_IS_EXPRESS:BOOL=${MSVC_IS_EXPRESS}
    -DUSE_PCH:BOOL=${USE_PCH}
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
    -DCMAKE_CXX_FLAGS:STRING=${CMAKE_CXX_FLAGS}
    -DCMAKE_C_FLAGS:STRING=${CMAKE_C_FLAGS}
    -DCMAKE_EXE_LINKER_FLAGS:STRING=${CMAKE_EXE_LINKER_FLAGS}
    -DCMAKE_MODULE_LINKER_FLAGS:STRING=${CMAKE_MODULE_LINKER_FLAGS}
    -DCMAKE_SHARED_LINKER_FLAGS:STRING=${CMAKE_SHARED_LINKER_FLAGS}
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_INSTALL_PREFIX}
    -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
    -DCMAKE_VERSION_MAJOR:STRING=${CMAKE_VERSION_MAJOR}
    -DCMAKE_VERSION_MINOR:STRING=${CMAKE_VERSION_MINOR}
    -DCMAKE_VERSION_PATCH:STRING=${CMAKE_VERSION_PATCH}
    -DCMAKE_VERSION_BUILD:STRING=${CMAKE_VERSION_BUILD}
    -DBOOST_ROOT:STRING=${BOOST_ROOT}
    -DBOOST_LIBRARYDIR:STRING=${BOOST_ROOT}/lib
    -DQt5Widgets_DIR:STRING=${Qt5Widgets_DIR}
    -DQt5Sql_DIR:STRING=${Qt5Sql_DIR}
    -DQt5Network_DIR:STRING=${Qt5Network_DIR}
    -DQt5Xml_DIR:STRING=${Qt5Xml_DIR}
    -DQt5WebKit_DIR:STRING=${Qt5WebKit_DIR}
    -DQt5WebKitWidgets_DIR:STRING=${Qt5WebKitWidgets_DIR}
    #-DQt5WinExtras_DIR:STRING=${Qt5WinExtras_DIR}
    -DQt5Concurrent_DIR:STRING=${Qt5Concurrent_DIR}
    -DSWIG_EXECUTABLE:STRING=${SWIG_EXECUTABLE}
    -DRUBY_EXECUTABLE:STRING=${RUBY_EXECUTABLE}
    -DNODE_BIN_DIR:STRING=${NODE_BIN_DIR}
    -DNODE_INCLUDE_DIR:STRING=${NODE_INCLUDE_DIR}
    -DV8_INCLUDE_DIR:STRING=${V8_INCLUDE_DIR}
    -DRUBY_LIBRARY:STRING=${RUBY_LIBRARY}
    -DDOXYGEN_EXECUTABLE:STRING=${DOXYGEN_EXECUTABLE}
    -DDOXYGEN_DOT_EXECUTABLE:STRING=${DOXYGEN_DOT_EXECUTABLE}
    -DPACKAGE_RADIANCE_AND_ENERGYPLUS:BOOL=${PACKAGE_RADIANCE_AND_ENERGYPLUS}
    -DENERGYPLUS_LOCATION:PATH=${ENERGYPLUS_LOCATION}
    -DRADIANCE_LOCATION:PATH=${RADIANCE_LOCATION}
    -DOPENSSL_ROOT_DIR:PATH=${OPENSSL_ROOT_DIR}
    INSTALL_COMMAND ""
)

if (BUILD_QT AND MSVC)
  file(GLOB dlls ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/bin/*.dll)
  foreach(dll ${dlls})
    get_filename_component(filename "${dll}" NAME_WE)
    ExternalProject_Add_Step(OpenStudioCore "Install${filename}ReleaseDLL"
       COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Release"
       DEPENDERS configure
     )
    ExternalProject_Add_Step(OpenStudioCore "Install${filename}DebugDLL"
       COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Debug"
       DEPENDERS configure
     )
    ExternalProject_Add_Step(OpenStudioCore "Install${filename}RelWithDebInfoDLL"
       COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/RelWithDebInfo"
       DEPENDERS configure
     )
    ExternalProject_Add_Step(OpenStudioCore "Install${filename}MinSizeRelDLL"
       COMMAND ${CMAKE_COMMAND} -E copy_if_different "${dll}" "${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/MinSizeRel"
       DEPENDERS configure
     )
  endforeach()
endif()

if (BUILD_RUBY AND MSVC)
  ExternalProject_Add_Step(OpenStudioCore MakeRubyReleaseFolder
     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Release
     DEPENDERS configure
   )     
  ExternalProject_Add_Step(OpenStudioCore MakeRubyDebugFolder
   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Debug
     DEPENDERS configure
   )     
  ExternalProject_Add_Step(OpenStudioCore MakeRubyRelWithDebInfoFolder
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/RelWithDebInfo
     DEPENDERS configure
   )     
  ExternalProject_Add_Step(OpenStudioCore MakeRubyMinSizeRelFolder
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/MinSizeRel
     DEPENDERS configure
   )     

   
  ExternalProject_Add_Step(OpenStudioCore MakeRubyReleaseDLL
    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Release/${RUBY_DLL}"
     DEPENDERS configure
   )     
  ExternalProject_Add_Step(OpenStudioCore MakeRubyDebugDLL
    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/Debug/${RUBY_DLL}"
     DEPENDERS configure
   )     
  ExternalProject_Add_Step(OpenStudioCore MakeRubyRelWithDebInfoDLL
    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/RelWithDebInfo/${RUBY_DLL}"
     DEPENDERS configure
   )     
  ExternalProject_Add_Step(OpenStudioCore MakeRubyMinSizeRelDLL
    COMMAND ${CMAKE_COMMAND} -E copy "${RUBY_DLLPATH}" "${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products/MinSizeRel/${RUBY_DLL}"
     DEPENDERS configure
   )     
      
endif()


set(OPENSTUDIOCORE_BUILD_DIR ${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build)
set(OPENSTUDIOCORE_LIB_DIR ${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build/Products)
set(OPENSTUDIOCORE_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/openstudiocore/src)
set(OPENSTUDIOCORE_ROOT_DIR ${CMAKE_SOURCE_DIR}/openstudiocore/)

###############################################################################
# Use CPack
IF( BUILD_PACKAGE )
  INCLUDE(OpenStudioCPack.cmake )
ENDIF( BUILD_PACKAGE )
###############################################################################


