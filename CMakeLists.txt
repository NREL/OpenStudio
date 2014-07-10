cmake_minimum_required(VERSION 2.8.11)

project(OpenStudio)

set(CMAKE_VERSION_MAJOR 1)
set(CMAKE_VERSION_MINOR 4)
set(CMAKE_VERSION_PATCH 1)

set(CMAKE_VERSION_BUILD "Unknown" CACHE STRING "Build number")
find_package(Git)
if(GIT_FOUND)
  execute_process(COMMAND "${GIT_EXECUTABLE}" "rev-parse" "--short=10" "HEAD"
                  WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
                  TIMEOUT 10
                  RESULT_VARIABLE RESULT
                  OUTPUT_VARIABLE GIT_VERSION
                  ERROR_QUIET
                  OUTPUT_STRIP_TRAILING_WHITESPACE)
  if(${RESULT} EQUAL 0 AND NOT "${GIT_VERSION}" EQUAL "${CMAKE_VERSION_BUILD}")
    set(CMAKE_VERSION_BUILD ${GIT_VERSION} CACHE STRING "Build number" FORCE) # git sha
  endif()

  get_filename_component(GIT_DIR "${GIT_EXECUTABLE}" PATH)
else()
  set(GIT_DIR "")
endif()

find_program(PATCH_EXE patch HINTS "${GIT_DIR}" "${GIT_DIR}/../bin/")
string(COMPARE EQUAL "${PATCH_EXE}" "PATCH_EXE-NOTFOUND" PATCH_EXE_NOTFOUND)
if(PATCH_EXE_NOTFOUND)
  message(SEND_ERROR "Required program patch not found")
endif()

# EnergyPlus Idd version
set(ENERGYPLUS_VERSION "8.1" CACHE INTERNAL "EnergyPlus Version")

option(NIGHTLY_BUILD "Use configurations for Nightly Build" OFF)

set(OPENSTUDIO_VERSION "${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}")

# Search for modules in the openstudiocore dir first to override cmake ones
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/openstudiocore/CMake" "${CMAKE_SOURCE_DIR}")

# Build C++ documentation using Doxygen
# Requires: doxygen
option(BUILD_DOCUMENTATION "Build Documentation" OFF)

if(BUILD_DOCUMENTATION)
  find_package(Doxygen REQUIRED)
endif()

# Build CSharp bindings
# Requires: SWIG
option(BUILD_CSHARP_BINDINGS "Build CSharp bindings" OFF)

# Build Java bindings
# Requires: SWIG
option(BUILD_JAVA_BINDINGS "Build Java bindings" OFF)

# Build Node V8 bindings
# Requires: SWIG
option(BUILD_V8_BINDINGS "Build V8 bindings" OFF)

# Build Python bindings
# Requires: SWIG Python
option(BUILD_PYTHON_BINDINGS "Build Python bindings" OFF)

# Build ctest testing
# Requires: EnergyPlus
option(BUILD_TESTING "Build testing targets" OFF)

# Build package
# Requires: EnergyPlus
option(BUILD_PACKAGE "Build package" OFF)

# Build test runner targets.
# This is a convenience for Visual Studio users
option(ENABLE_TEST_RUNNER_TARGETS "Create test runner targets" OFF)


# Build with OpenSSL support
set(BUILD_WITH_OPENSSL ON CACHE INTERNAL "Build With OpenSSL Support For SSH Connections")
if(UNIX)
  find_package(OpenSSL)
  if(NOT ${OPENSSL_FOUND})
    message(SEND_ERROR "OpenSSL could not be found, and is required for HTTPS communication")
    message(SEND_ERROR "Please install OpenSSL development libraries using your package management system (possibly libssl-dev)")
  else()
    mark_as_advanced(
      LIB_EAY_DEBUG
      LIB_EAY_RELEASE
      SSL_EAY_DEBUG
      SSL_EAY_RELEASE
    )
  endif()
elseif(WIN32)
  if(CMAKE_CL_64)
    set(OPENSSL_ARCH 64)
    set(OPENSSL_EXPECTED_HASH b63ff40c9d4f94aef43e3c4d8085a5c5)
  else()
    set(OPENSSL_ARCH 32)
    set(OPENSSL_EXPECTED_HASH 507b6a190e282ab3018ee4a69371f6d8)
  endif()
  set(OPENSSL_VERSION "1.0.1g")
  set(OPENSSL_PATH "OpenSSL-Win${OPENSSL_ARCH}")
  if(EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip")
    file(MD5 "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" OPENSSL_HASH)
  endif()
  if(NOT EXISTS "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" OR NOT "${OPENSSL_HASH}" MATCHES "${OPENSSL_EXPECTED_HASH}")
    message(STATUS "Downloading OpenSSL ${OPENSSL_VERSION} (${OPENSSL_ARCH}-bit)")
    file(DOWNLOAD "http://developer.nrel.gov/downloads/buildings/openstudio/src/OpenSSL-Win${OPENSSL_ARCH}-${OPENSSL_VERSION}.zip" "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" TIMEOUT 120 INACTIVITY_TIMEOUT 120 SHOW_PROGRESS EXPECTED_MD5 ${OPENSSL_EXPECTED_HASH})
    execute_process(COMMAND ${CMAKE_COMMAND} -E remove_directory "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
    execute_process(COMMAND ${CMAKE_COMMAND} -E tar xfz "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}.zip" WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
  endif()
  set(OPENSSL_ROOT_DIR "${CMAKE_BINARY_DIR}/${OPENSSL_PATH}")
endif()



if(MSVC)
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
  elseif(${CMAKE_C_COMPILER_VERSION} VERSION_EQUAL "18.0.21005.1")
    # vc120: no-op
  endif()

  # Build with Multiple Processes
  set(BUILD_WITH_MULTIPLE_PROCESSES ON CACHE BOOL "/MP compiler flag for full processor utilization")
  mark_as_advanced(BUILD_WITH_MULTIPLE_PROCESSES)
endif()

# EnergyPlus
find_package(EnergyPlus "${ENERGYPLUS_VERSION}" REQUIRED)
mark_as_advanced(
  ENERGYPLUS_EXE
  ENERGYPLUS_IDD
  ENERGYPLUS_WEATHER_DIR
)

# Package Radiance and EnergyPlus for convenience
option(PACKAGE_RADIANCE_AND_ENERGYPLUS "Package Radiance and EnergyPlus" OFF)
mark_as_advanced(FORCE PACKAGE_RADIANCE_AND_ENERGYPLUS)

if(PACKAGE_RADIANCE_AND_ENERGYPLUS)
  message(WARNING "Packaging of Radiance and EnergyPlus is intended for internal use and deployment only")
  set(ENERGYPLUS_LOCATION "" CACHE PATH "Location of EnergyPlus to Package")
  set(RADIANCE_LOCATION "" CACHE PATH "Location of Radiance to Package")
endif()

# Build Ruby gem
option(BUILD_RUBY_GEM "Build Ruby Gem" OFF)

if(BUILD_V8_BINDINGS)
  if(WIN32)
    message(SEND_ERROR "V8 is only tested and supported on Unix like systems")
  endif()

  option(BUILD_NODE_MODULES "Build V8 Bindings as Node Modules" ON)

  if(BUILD_NODE_MODULES)
    # Build Node for linking to Node modules
    option(BUILD_NODE "Build Node" ON)
  else()
    option(BUILD_V8 "Build V8" ON)
  endif()

endif()

if(BUILD_RUBY_GEM)
  # Build Ruby gem and package up all the libs
  option(BUILD_RUBY_GEM_WITH_LIBS "Package all required libs with the Ruby Gem" ON)

  set(BUILD_PACKAGE ON)
  set(CPACK_BINARY_DEB OFF)
  set(CPACK_BINARY_NSIS OFF)
  set(CPACK_BINARY_RPM OFF)
  set(CPACK_BINARY_STGZ OFF)
  set(CPACK_BINARY_TBZ2 OFF)
  set(CPACK_BINARY_TZ OFF)
  set(CPACK_BINARY_TGZ ON)

  if(CPACK_BINARY_DEB OR CPACK_BINARY_NSIS OR CPACK_BINARY_RPM OR CPACK_BINARY_STGZ
      OR CPACK_BINARY_TBZ2 OR CPACK_BINARY_TZ OR NOT CPACK_BINARY_TGZ OR NOT BUILD_PACKAGE)
    message(FATAL_ERROR "To build the Ruby Gem please enable ONLY CPACK_BINARY_TGZ of the cpack binary options")
  endif()

  message("Enabling Ruby Gem overrides all of your packaging options. To build the Ruby Gem please type 'make package'")
endif()

if(MSVC)
  # need to know if using the Express edition of Visual Studio, devenv is split into multiple programs
  option(MSVC_IS_EXPRESS "using the Express editions of Visual Studio, must install both C++ and C# versions" FALSE)

  mark_as_advanced(CMAKE_CONFIGURATION_TYPES)
endif()

# Use PCH
option(USE_PCH "Use precompiled headers" OFF)

if(MSVC AND MSVC_IS_EXPRESS)
  if(CMAKE_CL_64)
    list(APPEND CMAKE_LIBRARY_PATH "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x64/")
  else()
    list(APPEND CMAKE_LIBRARY_PATH "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x86/")
    list(APPEND CMAKE_LIBRARY_PATH "C:/Program Files/Windows Kits/8.0/Lib/win8/um/x86/")
  endif()
endif()

find_package(Qt5Widgets) #Qt5Core, Qt5Gui, Qt5Widgets
find_package(Qt5Sql) #Qt5Core, Qt5Sql
find_package(Qt5Network) #Qt5Core, Qt5Network
find_package(Qt5Xml) #Qt5Core, Qt5Xml
find_package(Qt5WebKit) #Qt5Core, Qt5Gui, Qt5Network, Qt5WebKit
find_package(Qt5WebKitWidgets) #Qt5Core, Qt5Gui, Qt5Multimedia, Qt5MultimediaWidgets, Qt5Network, Qt5OpenGL, Qt5Positioning, Qt5PrintSupport, Qt5Qml, Qt5Quick, Qt5Sensors, Qt5WebKit, Qt5WebKitWidgets, Qt5Widgets
#find_package(Qt5WinExtras) #Qt5Core, Qt5Gui, Qt5WinExtras
find_package(Qt5Concurrent) #Qt5Concurrent, Qt5Core
mark_as_advanced(
  Qt5Concurrent_DIR
  Qt5Core_DIR
  Qt5Gui_DIR
  Qt5Gui_EGL_LIBRARY
  Qt5Location_DIR
  Qt5Multimedia_DIR
  Qt5MultimediaWidgets_DIR
  Qt5Network_DIR
  Qt5OpenGL_DIR
  Qt5Positioning_DIR
  Qt5PrintSupport_DIR
  Qt5Qml_DIR
  Qt5Quick_DIR
  Qt5Sensors_DIR
  Qt5Sql_DIR
  Qt5WebKit_DIR
  Qt5WebKitWidgets_DIR
  Qt5Widgets_DIR
  #Qt5WinExtras_DIR
  Qt5Xml_DIR
)

# Build Qt
if(Qt5Widgets_DIR)
  option(BUILD_QT "Build Qt" OFF)

  if(${Qt5Widgets_VERSION} VERSION_LESS "5.1.1")
    message(FATAL_ERROR "Minimum supported Qt is v5.1.1")
  endif()
else()
  option(BUILD_QT "Build Qt" ON)
endif()

# Apple Sanity Check
if(APPLE)
  if(NOT CMAKE_OSX_DEPLOYMENT_TARGET)
    message(FATAL_ERROR "OSX Deployment Target Must Be Set")
  endif()

  if(NOT CMAKE_OSX_SYSROOT)
    message(FATAL_ERROR "OSX Sysroot must be set")
  endif()

  if(CMAKE_OSX_DEPLOYMENT_TARGET MATCHES ".*10\\.9.*" OR CMAKE_OSX_SYSROOT MATCHES ".*10\\.9.*")
    message(WARNING "You are building against the 10.9 SDK which means ruby 2.0")
  endif()

endif()

include(ExternalProject)
include(ProcessorCount)

ProcessorCount(CPUCOUNT)
if(CPUCOUNT EQUAL 0)
  set(CPUCOUNT "1")
endif()

mark_as_advanced(
  Boost_DIR
  BOOST_THREAD_LIBRARY
  ProcessorCount_cmd_getconf
  ProcessorCount_cmd_sysctl
)

# Boost
if(APPLE)
  set(Boost_USE_STATIC_LIBS ON)
elseif(WIN32)
  set(Boost_USE_STATIC_LIBS ON)

  #uncomment all of this if we want to force dynamic libs on windows
  #  set(Boost_USE_STATIC_LIBS OFF)
  #  add_definitions(-DBOOST_THREAD_USE_DLL -DBOOST_THREAD_DYN_LINK -DBOOST_PROGRAM_OPTIONS_DYN_LINK -DBOOST_REGEX_DYN_LINK -DBOOST_FILESYSTEM_DYN_LINK -DBOOST_SYSTEM_DYN_LINK  -DBOOST_DATE_TIME_DYN_LINK)
  #  link_directories(${Boost_LIBRARY_DIRS})
  #  if(MSVC)
  #    #Ignore dll specific warnings that are out of our hands to control, coming from external projects
  #    add_definitions(/wd4251 /wd4275)
  #  endif()
endif()

# DLM: should we allow boost to be linked dynamically on Unix or should we force static for all platforms?

find_package(Boost 1.55.0 COMPONENTS filesystem regex program_options system thread date_time log QUIET)

# Check default Boost install location for Windows if Boost has not been found, and display missing components
if(MSVC AND NOT Boost_FOUND AND EXISTS "C:/local/boost_1_55_0/")
  if(CMAKE_CL_64)
    set(BOOST_ARCH 64)
  else()
    set(BOOST_ARCH 32)
  endif()
  if(MSVC10)
    set(BOOST_MSVC 10.0)
  elseif(MSVC12)
    set(BOOST_MSVC 12.0)
  endif()
  
  if(EXISTS "C:/local/boost_1_55_0/lib${BOOST_ARCH}-msvc-${BOOST_MSVC}")
    set(BOOST_ROOT "C:/local/boost_1_55_0")
    set(BOOST_LIBRARYDIR "C:/local/boost_1_55_0/lib${BOOST_ARCH}-msvc-${BOOST_MSVC}")
    find_package(Boost 1.55.0 COMPONENTS filesystem regex program_options system thread date_time log QUIET)
    if(NOT Boost_FOUND)
      message(STATUS "Could NOT find Boost\n${Boost_ERROR_REASON}")
    endif()
  endif()
elseif(NOT Boost_FOUND)
  message(STATUS "Could NOT find Boost\n${Boost_ERROR_REASON}")
endif()

if(Boost_FOUND)
  option(BUILD_BOOST "Build Boost" OFF)
else()
  option(BUILD_BOOST "Build Boost" ON)
endif()


find_package(SWIG 2.0)
mark_as_advanced(
  SWIG_DIR
  SWIG_EXECUTABLE
  SWIG_VERSION
)

# Currently MacOS and/or V8 require special builds
# as of 2014-06-04. This should be re-checked after swig >= 3.0.2 is released
if(SWIG_FOUND AND NOT BUILD_V8_BINDINGS AND NOT APPLE)
  option(BUILD_SWIG "Build SWIG" OFF)
else()
  option(BUILD_SWIG "Build SWIG" ON)
endif()

find_program(RUBY_EXECUTABLE NAMES ruby PATHS $ENV{CMAKE_RUBY_PATH} NO_SYSTEM_ENVIRONMENT_PATH)
find_package(Ruby)


if(WIN32 AND MSVC AND CMAKE_CL_64)
  message(WARNING "64bit Support on Windows is EXPERIMENTAL. Ruby 2.0.0 will be built. The SketchUp plugin will not work. Expect problems with ruby support in general. Unless you've already built Boost and Qt you probably want to let this super build do it. There are lots of warnings, so we are disabling warnings as errors.")
endif()

if(UNIX)
  if(RUBY_FOUND)
    option(BUILD_RUBY "Build Ruby" OFF)
  else()
    option(BUILD_RUBY "Build Ruby" ON)
  endif()
else()
  # Always provide ruby on Windows
  option(BUILD_RUBY "Build Ruby" ON)
  mark_as_advanced(BUILD_RUBY)

  if(WIN32 AND MSVC)
    if(CMAKE_CL_64)
      if(MSVC12)
        message(WARNING "Cannot use 32bit mingw build of Ruby 2.0 with 64bit build of OpenStudio, forcing 64bit msvc build of Ruby 2.0")
      else()
        message(SEND_ERROR "64bit build of Ruby 2.0 only supported with MSVC12")
      endif()
    else()
      option(BUILD_RUBY_MINGW32_2_0 "Use mingw32 Ruby 2.0" OFF)
      mark_as_advanced(BUILD_RUBY_MINGW32_2_0)
    endif()
  endif()
endif()

if(NOT BUILD_RUBY AND NOT BUILD_SWIG)
  set(RUBY_VER "${RUBY_VERSION_MAJOR}.${RUBY_VERSION_MINOR}.${RUBY_VERSION_PATCH}")

  if(${RUBY_VER} VERSION_GREATER "1.8.8" AND ${SWIG_VERSION} VERSION_LESS "2.0.10")
    message(SEND_ERROR "For Ruby >= 1.9.0, SWIG >= 2.0.10 is required. Use of the BUILD_SWIG option is suggested")
  endif()
endif()

if(BUILD_WITH_MULTIPLE_PROCESSES)
  add_definitions(/MP)
endif()

if(CMAKE_CL_64 OR (MSVC AND BUILD_QT))
  if(CMAKE_CL_64)
    if(MSVC10)
      set(NMAKE_ARCH "/amd64")
    elseif(MSVC12)
      set(NMAKE_ARCH "")
    endif()
  else()
    set(NMAKE_ARCH "")
  endif()
  if(MSVC10)
    find_program(NMAKE_EXE nmake PATHS
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\10.0;InstallDir]/../../VC/bin${NMAKE_ARCH}"
      $ENV{VS100COMNTOOLS}../../VC/bin${NMAKE_ARCH}
    )
  elseif(MSVC12)
    find_program(NMAKE_EXE nmake PATHS
      "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Wow6432Node\\Microsoft\\VisualStudio\\12.0;InstallDir]/../../VC/bin${NMAKE_ARCH}"
      $ENV{VS120COMNTOOLS}../../VC/bin${NMAKE_ARCH}
    )
  endif()

  if(NMAKE_EXE STREQUAL "NMAKE_EXE-NOTFOUND" OR NMAKE_EXE STREQUAL "nmake")
    message(WARNING "NMake not found - CMake will attempt to call nmake directly")
    set(NMAKE_EXE "nmake" CACHE STRING "nmake executable" FORCE)
  endif()
endif()

if(BUILD_NODE)
  # OSX can work with a static ruby library
  ExternalProject_Add(Node
    URL http://nodejs.org/dist/v0.10.28/node-v0.10.28.tar.gz
    CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && sh -c "./configure --prefix=${CMAKE_BINARY_DIR}/Node-prefix/src/Node-install"
    BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && $(MAKE)
    INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Node-prefix/src/Node && $(MAKE) install && $(MAKE) install-includes && ../Node-install/bin/npm install jasmine-node node-gyp -g && ../Node-install/bin/node ../Node-install/bin/node-gyp install
  )
  set(NODE_BIN_DIR ${CMAKE_BINARY_DIR}/Node-prefix/src/Node-install/bin/)
  set(NODE_INCLUDE_DIR "$ENV{HOME}/.node-gyp/0.10.28")
endif()

if(BUILD_V8)
  ExternalProject_Add(V8
    SVN_REPOSITORY http://v8.googlecode.com/svn/branches/3.19
    CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/V8-prefix/src/V8 && $(MAKE) dependencies
    BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/V8-prefix/src/V8 && $(MAKE) x64
    INSTALL_COMMAND ""
  )
  set(V8_INCLUDE_DIR ${CMAKE_BINARY_DIR}/V8-prefix/src/V8/include)
endif()

if(BUILD_RUBY)
  if(APPLE)
    # OSX can work with a static ruby library
    ExternalProject_Add(Ruby
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-1.8.7-p370.tar.gz
      URL_MD5 98b00bbd1cdde3116155edb6e555b781
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && sh -c "./configure --prefix=${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install --without-tcl --without-tk  CFLAGS=\"-arch i386 -arch x86_64 -fPIC\" LDFLAGS=\"-arch i386 -arch x86_64\""
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && $(MAKE)
      INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && $(MAKE) install
    )
    set(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/ruby)
  elseif(UNIX)
    # but linux cannot, it has a double free when unloading a ruby module
    ExternalProject_Add(Ruby
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-1.8.7-p370.tar.gz
      URL_MD5 98b00bbd1cdde3116155edb6e555b781
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && sh -c "./configure --enable-shared --prefix=${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install CFLAGS=\"-fPIC\" "
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && $(MAKE)
      INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby && $(MAKE) install
    )
    set(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/ruby)
  else()

    if(CMAKE_CL_64)
      #find_program(NMAKE_EXE nmake)
      
      # Ruby 1.8 cannot be built for 64bit windows because of faulty assumptions in the code,
      # and Ruby 2.0 isn't shipped in any way we can link to it, so for now we build our own
      ExternalProject_Add(Ruby
        URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0-p481.tar.gz
        URL_MD5 3913e0ad6cc572b7358e4c6a8c4b2551
        PATCH_COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/mkexports.rb ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/win32/mkexports.rb COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/ruby/ruby-2.0.0-p481/Makefile.sub ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/win32/Makefile.sub
        CONFIGURE_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-build && ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/win32/configure.bat --prefix=${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install --target=x64-mswin64 --disable-install-doc --disable-win95"
        BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-build && ${NMAKE_EXE}
        INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-build && ${NMAKE_EXE} install
      )
      set(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/ruby.exe )
      set(RUBY_DLL x64-msvcr120-ruby200.dll)
      set(RUBY_DLLPATH "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/bin/${RUBY_DLL}")
      set(RUBY_LIBRARY "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby-install/lib/x64-msvcr120-ruby200.lib")
    else()
      if(BUILD_RUBY_MINGW32_2_0)
        # And windows requires mingw to build ruby, so we just unpackage it locally
        ExternalProject_Add(Ruby
          URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-2.0.0p353-aws-win32.zip
          URL_MD5 1f6e4348099b26e92909db62dcd3b173
          PATCH_COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/ruby/mingw32-ruby2.0/msvcrt-ruby200.lib  ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/lib COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/ruby/mingw32-ruby2.0/config.h ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/include/ruby-2.0.0/i386-mingw32/ruby COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/ruby/mingw32-ruby2.0/missing.h ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/include/ruby-2.0.0/ruby COMMAND "${CMAKE_COMMAND}" -E copy ${CMAKE_SOURCE_DIR}/dependencies/ruby/mingw32-ruby2.0/ruby.h ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/include/ruby-2.0.0/ruby
          CONFIGURE_COMMAND ""
          BUILD_COMMAND ""
          INSTALL_COMMAND ""
        )
        set(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/ruby.exe)
        set(RUBY_DLL msvcrt-ruby200.dll)
        set(RUBY_DLLPATH "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/${RUBY_DLL}")
        set(RUBY_LIBRARY "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/lib/msvcrt-ruby200.lib")
      else()
        # And windows requires mingw to build ruby, so we just unpackage it locally
        ExternalProject_Add(Ruby
          URL http://developer.nrel.gov/downloads/buildings/openstudio/src/ruby-1.8.6-msvc-ssl.zip
          URL_MD5 d3fdb49726e63f123941f5f41f594149
          CONFIGURE_COMMAND ""
          BUILD_COMMAND ""
          INSTALL_COMMAND ""
        )
        set(RUBY_EXECUTABLE ${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/ruby.exe)
        set(RUBY_DLL msvcrt-ruby18.dll)
        set(RUBY_DLLPATH "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/bin/${RUBY_DLL}")
        set(RUBY_LIBRARY "${CMAKE_BINARY_DIR}/Ruby-prefix/src/Ruby/lib/msvcrt-ruby18.lib")
      endif()
    endif()


  endif()
endif()


if(BUILD_SWIG)
  if(UNIX)
    if(BUILD_V8_BINDINGS)
      message("Building SWIG for V8, the latest source will be cloned from git://github.com/lefticus/swig-v8.git. Autoconf tools are required")

      find_program(autoconf "autoconf")

      if(autoconf STREQUAL "autoconf_NOTFOUND")
        message(FATAL_ERROR "autoconf is required to compile SWIG with V8 support")
      else()
        get_filename_component(autoconf_path "${autoconf}" PATH)
        message("autoconf directory: ${autoconf_path}")
      endif()



      # We patch it up with a version of pcre we provide to avoid having to have the requirement locally
      ExternalProject_Add(SWIG
        GIT_REPOSITORY git://github.com/lefticus/swig.git
        GIT_TAG merged_fixes
        # URL http://github.com/lefticus/swig-v8/archive/devel.zip
        PATCH_COMMAND cp ${CMAKE_SOURCE_DIR}/dependencies/pcre-8.31.tar.bz2 ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && ../SWIG/Tools/pcre-build.sh
        CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG && /bin/sh -c "PATH=\$PATH:${autoconf_path} ./autogen.sh" && cd - && ../SWIG/configure --prefix=${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install --disable-ccache
        BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && $(MAKE)
        INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && $(MAKE) install
      )
      set(SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install/bin/swig)
    else()
      # We patch it up with a version of pcre we provide to avoid having to have the requirement locally
      ExternalProject_Add(SWIG
        URL http://developer.nrel.gov/downloads/buildings/openstudio/src/swig-3.0.0.tar.gz
        URL_MD5 9c8278fad527dda4bf38edffe5acc394

        PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/swig_ruby_patch.patch && cp ${CMAKE_SOURCE_DIR}/dependencies/pcre-8.31.tar.bz2 ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && ../SWIG/Tools/pcre-build.sh
        CONFIGURE_COMMAND ../SWIG/configure --prefix=${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install
        BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && $(MAKE)
        INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-build && $(MAKE) install
      )
      set(SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG-install/bin/swig)
    endif()
  else()
    # SWIG requires MinGW to compile on windows, so we just copy in the prebuilt binary
    ExternalProject_Add(SWIG
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/swigwin-3.0.0.zip
      URL_MD5 c1d85fee2421d4edac552923fc4e5bde
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
    )
    set(SWIG_EXECUTABLE ${CMAKE_BINARY_DIR}/SWIG-prefix/src/SWIG/swig.exe)
  endif()
endif()

if(BUILD_BOOST)
  if(APPLE)
    if(CMAKE_OSX_DEPLOYMENT_TARGET MATCHES ".*10\\.9.*")
      set(apple-frameworks "-stdlib=libc++ -std=c++11")
    else()
      set(apple-frameworks "-stdlib=libstdc++")
    endif()
    ExternalProject_Add(Boost
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/boost_1_55_0.tar.gz
      URL_MD5 93780777cfbf999a600f62883bd54b17
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./bootstrap.sh
      PATCH_COMMAND ${PATCH_EXE} -p1 < ${CMAKE_SOURCE_DIR}/dependencies/Boost/xcode_51a.diff < ${CMAKE_SOURCE_DIR}/dependencies/Boost/xcode_51b.diff
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./b2 toolset=clang cxxflags=${apple-frameworks} linkflags=${apple-frameworks} variant=debug,release address-model=32_64 architecture=x86 define=BOOST_CHRONO_HEADER_ONLY --layout=tagged --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT} install
      INSTALL_COMMAND ""
    )
    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
    set(BOOST_LIBRARYDIR ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install/lib)
  elseif(UNIX)
    ExternalProject_Add(Boost
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/boost_1_55_0.tar.gz
      URL_MD5 93780777cfbf999a600f62883bd54b17
      CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./bootstrap.sh
      BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && ./b2 cxxflags=-fPIC link=static variant=debug,release define=BOOST_CHRONO_HEADER_ONLY --layout=tagged --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT} install
      INSTALL_COMMAND ""
    )
    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
    set(BOOST_LIBRARYDIR ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install/lib)
  else()
    # The call to cmd and VS_UNICODE_OUTPUT is to fix an oddity where boost configuration complains about missing ICU when it shouldn't
    # be trying to find it

    set(B2_TOOLSET "")
    if(MSVC10)
      set(B2_TOOLSET "toolset=msvc-10.0")
    elseif(MSVC12)
      set(B2_TOOLSET "toolset=msvc-12.0")
    endif()

    if(CMAKE_CL_64)
      ExternalProject_Add(Boost
        URL http://developer.nrel.gov/downloads/buildings/openstudio/src/boost_1_55_0.tar.gz
        URL_MD5 93780777cfbf999a600f62883bd54b17
        CONFIGURE_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && bootstrap.bat"
        PATCH_COMMAND "${PATCH_EXE}" "${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost/boost/archive/iterators/transform_width.hpp" "${CMAKE_SOURCE_DIR}/dependencies/Boost/transform_width.diff"
        BUILD_COMMAND cmd /C "set VS_UNICODE_OUTPUT=& cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && b2 ${B2_TOOLSET} link=static address-model=64 define=BOOST_CHRONO_HEADER_ONLY --build-type=minimal --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT}" install
        INSTALL_COMMAND ""
      )
    else()
      ExternalProject_Add(Boost
        URL http://developer.nrel.gov/downloads/buildings/openstudio/src/boost_1_55_0.tar.gz
        URL_MD5 93780777cfbf999a600f62883bd54b17
        CONFIGURE_COMMAND cmd /C "cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && bootstrap.bat"
        PATCH_COMMAND "${PATCH_EXE}" "${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost/boost/archive/iterators/transform_width.hpp" "${CMAKE_SOURCE_DIR}/dependencies/Boost/transform_width.diff"
        BUILD_COMMAND cmd /C "set VS_UNICODE_OUTPUT=& cd ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost && b2 ${B2_TOOLSET} link=static address-model=32 define=BOOST_CHRONO_HEADER_ONLY --build-type=minimal --with-filesystem --with-regex --with-program_options --with-system --with-thread --with-date_time --with-log --prefix=${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install -j${CPUCOUNT}" install
        INSTALL_COMMAND ""
      )
    endif()

    set(BOOST_ROOT ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install)
    set(BOOST_LIBRARYDIR ${CMAKE_BINARY_DIR}/Boost-prefix/src/Boost-install/lib)
  endif()
endif()

ExternalProject_Add(GoogleTest
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

set(GTEST_INCLUDE_DIR ${CMAKE_BINARY_DIR}/GoogleTest-prefix/src/GoogleTest/include)
set(GTEST_LIB_DIR ${CMAKE_BINARY_DIR}/GoogleTest-prefix/src/GoogleTest-build)

if(BUILD_QT)
  if(APPLE)
    # We're going to explicitly use the SDK specified in the configuration by the user
    #set(APPLE_SDK_COMMAND "-sdk \"${CMAKE_OSX_SYSROOT}\"" )
    set(APPLE_SDK_COMMAND "")

    if(CMAKE_OSX_SYSROOT MATCHES ".*10.7.sdk")
      set(APPLE_PATCH_COMMAND "${CMAKE_COMMAND}" -E rename "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/src/3rdparty/webkit/WebKitLibraries/libWebKitSystemInterfaceMountainLion.a" "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/src/3rdparty/webkit/WebKitLibraries/libWebKitSystemInterfaceMountainLion.a.bak" COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/src/3rdparty/webkit/WebKitLibraries/libWebKitSystemInterfaceLion.a" "${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/src/3rdparty/webkit/WebKitLibraries/libWebKitSystemInterfaceMountainLion.a")
    else()
      set(APPLE_PATCH_COMMAND "")
    endif()

    # At the moment we are building all of debug,release,x86 and x86_64
    # it would appear that calling "make" in some way that CMake can detect that we are calling
    # make (and attempt to pass parameters to the child make jobs) is breaking our ability to install
    # the below code circumvents that by calling make through a call to "sh"
    message(WARNING "Qt5 super build is currently untested")
    ExternalProject_Add(Qt
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/qt-everywhere-opensource-src-5.2.1.tar.gz
      URL_MD5 a78408c887c04c34ce615da690e0b4c8
      CONFIGURE_COMMAND /bin/sh -c "cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/ && ./configure -v ${APPLE_SDK_COMMAND} -debug-and-release -opensource -openssl -qt-sql-sqlite -plugin-sql-sqlite -nomake examples -nomake tests -confirm-license -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install"
      PATCH_COMMAND ${APPLE_PATCH_COMMAND}
      BUILD_COMMAND  /bin/sh -c "cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt && make"
      INSTALL_COMMAND /bin/sh -c "cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt && make -j1 install"
    )
  elseif(UNIX)
    message(WARNING "Qt5 super build is currently untested")
    ExternalProject_Add(Qt
      URL http://developer.nrel.gov/downloads/buildings/openstudio/src/qt-everywhere-opensource-src-5.2.1.tar.gz
      URL_MD5 a78408c887c04c34ce615da690e0b4c8
      CONFIGURE_COMMAND ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt/configure -debug-and-release -opensource -confirm-license -shared -qt-sql-sqlite -qt-xcb -openssl -nomake examples -nomake tests -plugindir ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/share/openstudio/qtplugins -qtlibinfix OpenStudio -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install
      BUILD_COMMAND $(MAKE) -f ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-build/Makefile
      INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-build && $(MAKE) install
    )
  else()
    message(FATAL_ERROR "Qt5 cannot currently be built for Windows with the super build")
    #find_program(NMAKE_EXE nmake)

    #ExternalProject_Add(Qt
    #  URL http://developer.nrel.gov/downloads/buildings/openstudio/src/qt-everywhere-opensource-src-5.2.1.tar.gz
    #  URL_MD5 a78408c887c04c34ce615da690e0b4c8
    #  CONFIGURE_COMMAND cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt && configure -debug-and-release -opensource -confirm-license -shared -plugin-sql-sqlite -no-qt3support -nomake examples -nomake docs -nomake demos -mp -prefix ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install -L ${OPENSSL_ROOT_DIR}/lib -I ${OPENSSL_ROOT_DIR}/include -openssl
    #  BUILD_COMMAND cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt && ${NMAKE_EXE}
    #  INSTALL_COMMAND cd ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt && ${NMAKE_EXE} install
    #)
  endif()

  #Update this for Windows after BUILD_QT is updated for WINDOWS
  if(UNIX)
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR}/Qt-prefix/src/Qt-install/lib/cmake)
  endif()
endif()


set(OpenStudioCore_DIR ${CMAKE_BINARY_DIR}/OpenStudioCore-prefix/src/OpenStudioCore-build)

# Set up the dependencies for OpenStudioCore
set(OpenStudioCore_DEPENDS GoogleTest)

if(BUILD_QT)
  list(APPEND OpenStudioCore_DEPENDS Qt)
endif()

if(BUILD_SWIG)
  list(APPEND OpenStudioCore_DEPENDS SWIG)
endif()

if(BUILD_BOOST)
  list(APPEND OpenStudioCore_DEPENDS Boost)
endif()

if(BUILD_RUBY)
  list(APPEND OpenStudioCore_DEPENDS Ruby)
endif()

if(BUILD_NODE)
  list(APPEND OpenStudioCore_DEPENDS Node)
endif()

if(BUILD_V8)
  list(APPEND OpenStudioCore_DEPENDS V8)
endif()


ExternalProject_Add(OpenStudioCore
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
    -DBOOST_LIBRARYDIR:STRING=${BOOST_LIBRARYDIR}
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

if(BUILD_QT AND MSVC)
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

if(BUILD_RUBY AND MSVC)
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
if(BUILD_PACKAGE)
  include(OpenStudioCPack.cmake)
endif()
###############################################################################
