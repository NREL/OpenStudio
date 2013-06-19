include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckTypeSize)
include(CheckCXXSourceCompiles)
include(TestBigEndian)

set(PACKAGE ${APPLICATION_NAME})
set(VERSION ${APPLICATION_VERSION})
set(DATADIR ${DATA_INSTALL_DIR})
set(LIBDIR ${LIB_INSTALL_DIR})
set(PLUGINDIR "${PLUGIN_INSTALL_DIR}-${LIBRARY_SOVERSION}")
set(SYSCONFDIR ${SYSCONF_INSTALL_DIR})

set(BINARYDIR ${CMAKE_BINARY_DIR})
set(SOURCEDIR ${CMAKE_SOURCE_DIR})

if(CMAKE_COMPILER_IS_GNUCC)
check_c_compiler_flag("-fvisibility=hidden" WITH_VISIBILITY_HIDDEN)
endif(CMAKE_COMPILER_IS_GNUCC)

# HEADER FILES
check_include_file(argp.h HAVE_ARGP_H)
check_include_file(pty.h HAVE_PTY_H)
check_include_file(terminos.h HAVE_TERMIOS_H)
if (WIN32)
  check_include_files("winsock2.h;ws2tcpip.h;wspiapi.h" HAVE_WSPIAPI_H)
  if (NOT HAVE_WSPIAPI_H)
    message(STATUS "WARNING: Without wspiapi.h (or dependencies), this build will only work on Windows XP and newer versions")
  endif (NOT HAVE_WSPIAPI_H)
  check_include_files("winsock2.h;ws2tcpip.h" HAVE_WS2TCPIP_H)
  if (NOT HAVE_WS2TCPIP_H)
    message(ERROR "WARNING: Does not have ws2tcpip.h or winsock2.h")
  endif (NOT HAVE_WS2TCPIP_H)
  if (HAVE_WSPIAPI_H OR HAVE_WS2TCPIP_H)
    set(HAVE_GETADDRINFO TRUE)
    set(HAVE_GETHOSTBYNAME TRUE)
  endif (HAVE_WSPIAPI_H OR HAVE_WS2TCPIP_H)

  check_function_exists(vsnprintf HAVE_VSNPRINTF)
  if(NOT HAVE_VSNPRINTF)
      check_function_exists(_vsnprintf_s HAVE__VSNPRINTF_S)
      check_function_exists(_vsnprintf HAVE__VSNPRINTF)
  endif(NOT HAVE_VSNPRINTF)
  check_function_exists(snprintf HAVE_SNPRINTF)
  if(NOT HAVE_SNPRINTF)
      check_function_exists(_snprintf HAVE__SNPRINTF)
      check_function_exists(_snprintf_s HAVE__SNPRINTF_S)
  endif(NOT HAVE_SNPRINTF)
  check_function_exists(strncpy HAVE_STRNCPY)

  set(HAVE_SELECT TRUE)
endif (WIN32)

set(CMAKE_REQUIRED_INCLUDES ${OPENSSL_INCLUDE_DIRS})
check_include_file(openssl/aes.h HAVE_OPENSSL_AES_H)

set(CMAKE_REQUIRED_INCLUDES ${OPENSSL_INCLUDE_DIRS})
check_include_file(openssl/blowfish.h HAVE_OPENSSL_BLOWFISH_H)

set(CMAKE_REQUIRED_INCLUDES ${OPENSSL_INCLUDE_DIRS})
check_include_file(openssl/des.h HAVE_OPENSSL_DES_H)

# FUNCTIONS

if (UNIX)
  # libsocket (Solaris)
  check_library_exists(socket getaddrinfo "" HAVE_LIBSOCKET)
  if (HAVE_LIBSOCKET)
    set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} socket)
  endif (HAVE_LIBSOCKET)
  # libnsl (Solaris)
  check_library_exists(nsl gethostbyname "" HAVE_LIBNSL)
  if (HAVE_LIBNSL)
    set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} nsl)
  endif (HAVE_LIBNSL)
  # libresolv
  check_library_exists(resolv hstrerror "" HAVE_LIBRESOLV)
  if (HAVE_LIBRESOLV)
    set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} resolv)
  endif (HAVE_LIBRESOLV)
  check_library_exists(rt nanosleep "" HAVE_LIBRT)
  # librt
  if (HAVE_LIBRT)
    set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} rt)
  endif (HAVE_LIBRT)

  check_function_exists(getaddrinfo HAVE_GETADDRINFO)
  check_function_exists(gethostbyname HAVE_GETHOSTBYNAME)
  check_function_exists(poll HAVE_POLL)
  check_function_exists(select HAVE_SELECT)
  check_function_exists(cfmakeraw HAVE_CFMAKERAW)
  check_function_exists(regcomp HAVE_REGCOMP)
endif (UNIX)

set(LIBSSH_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} CACHE INTERNAL "libssh required system libraries")

# LIBRARIES
if (OPENSSL_FOUND)
  set(HAVE_LIBCRYPTO 1)
endif (OPENSSL_FOUND)

if (GCRYPT_FOUND)
  set(HAVE_LIBGCRYPT 1)
endif (GCRYPT_FOUND)

if (Z_LIBRARY)
  set(HAVE_LIBZ 1)
endif (Z_LIBRARY)

# OPTIONS
if (WITH_DEBUG_CRYPTO)
  set(DEBUG_CRYPTO 1)
endif (WITH_DEBUG_CRYPTO)

if (WITH_DEBUG_CALLTRACE)
  set(DEBUG_CALLTRACE 1)
endif (WITH_DEBUG_CALLTRACE)

# ENDIAN
test_big_endian(WORDS_BIGENDIAN)
