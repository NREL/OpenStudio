# - Try to find OpenSSL
# Once done this will define
#
#  OPENSSL_FOUND - system has OpenSSL
#  OPENSSL_INCLUDE_DIRS - the OpenSSL include directory
#  OPENSSL_LIBRARIES - Link these to use OpenSSL
#  OPENSSL_DEFINITIONS - Compiler switches required for using OpenSSL
#
#  Copyright (c) 2009 Andreas Schneider <mail@cynapses.org>
#
#  Redistribution and use is allowed according to the terms of the New
#  BSD license.
#  For details see the accompanying COPYING-CMAKE-SCRIPTS file.
#


if (OPENSSL_LIBRARIES AND OPENSSL_INCLUDE_DIRS)
  # in cache already
  set(OPENSSL_FOUND TRUE)
else (OPENSSL_LIBRARIES AND OPENSSL_INCLUDE_DIRS)
  # use pkg-config to get the directories and then use these values
  # in the FIND_PATH() and FIND_LIBRARY() calls
  if (${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4)
    include(UsePkgConfig)
    pkgconfig(openssl _OPENSSL_INCLUDEDIR _OPENSSL_LIBDIR _OPENSSL_LDFLAGS _OPENSSL_CFLAGS)
  else (${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4)
    find_package(PkgConfig)
    if (PKG_CONFIG_FOUND)
      pkg_check_modules(_OPENSSL openssl)
    endif (PKG_CONFIG_FOUND)
  endif (${CMAKE_MAJOR_VERSION} EQUAL 2 AND ${CMAKE_MINOR_VERSION} EQUAL 4)

  find_path(OPENSSL_INCLUDE_DIR
    NAMES
      openssl/ssl.h
    PATHS
      ${_OPENSSL_DIR}/include
      ${_OPENSSL_INCLUDEDIR}
      /usr/include
      /usr/local/include
      /usr/local/ssl/include
      /opt/local/include
      /sw/include
      /usr/lib/sfw/include
  )
  mark_as_advanced(OPENSSL_INCLUDE_DIR)

  find_library(SSL_LIBRARY
    NAMES
      ssl
      libssl
    PATHS
      ${_OPENSSL_DIR}/lib
      ${_OPENSSL_LIBDIR}
      /usr/lib
      /usr/local/lib
      /usr/local/ssl/lib
      /opt/local/lib
      /sw/lib
      /usr/sfw/lib/64
      /usr/sfw/lib
  )
  mark_as_advanced(SSL_LIBRARY)

  find_library(SSLEAY32_LIBRARY
    NAMES
      ssleay32
    PATHS
      ${_OPENSSL_DIR}/lib
      ${_OPENSSL_LIBDIR}
      /usr/lib
      /usr/local/lib
      /usr/local/ssl/lib
      /opt/local/lib
      /sw/lib
      /usr/sfw/lib/64
      /usr/sfw/lib
  )
  mark_as_advanced(SSLEAY32_LIBRARY)

  find_library(SSLEAY32MD_LIBRARY
    NAMES
      ssleay32MD
    PATHS
      ${_OPENSSL_DIR}/lib
      ${_OPENSSL_LIBDIR}
      /usr/lib
      /usr/local/lib
      /usr/local/ssl/lib
      /opt/local/lib
      /sw/lib
      /usr/sfw/lib/64
      /usr/sfw/lib
  )
  mark_as_advanced(SSLEAY32MD_LIBRARY)

  find_library(CRYPTO_LIBRARY
    NAMES
      crypto
      libcrypto
      eay
      eay32
      libeay
      libeay32
    PATHS
      ${_OPENSSL_DIR}/lib
      ${_OPENSSL_LIBDIR}
      /lib
      /usr/lib
      /usr/local/lib
      /usr/local/ssl/lib
      /opt/local/lib
      /sw/lib
      /usr/sfw/lib/64
      /usr/sfw/lib
  )
  mark_as_advanced(CRYPTO_LIBRARY)

  if (SSL_LIBRARY)
    set(SSL_FOUND TRUE CACHE INTERNAL "Wether the ssl library has been found" FORCE)
  endif (SSL_LIBRARY)

  if (SSLEAY32_LIBRARY)
    set(SSLEAY32_FOUND TRUE CACHE INTERNAL "Wether the ssleay32 library has been found" FORCE)
  endif (SSLEAY32_LIBRARY)

  if (SSLEAY32MD_LIBRARY)
    set(SSLEAY32MD_FOUND TRUE CACHE INTERNAL "Wether the ssleay32MD library has been found" FORCE)
  endif (SSLEAY32MD_LIBRARY)

  if (CRYPTO_LIBRARY)
    set(CRYPTO_FOUND TRUE CACHE INTERNAL "Wether the crypto library has been found" FORCE)
  endif (CRYPTO_LIBRARY)

  set(OPENSSL_INCLUDE_DIRS
    ${OPENSSL_INCLUDE_DIR}
  )

  if (SSL_FOUND)
    set(OPENSSL_LIBRARIES
      ${OPENSSL_LIBRARIES}
      ${SSL_LIBRARY}
    )
  endif (SSL_FOUND)

  if (SSLEAY32_FOUND)
    set(OPENSSL_LIBRARIES
      ${OPENSSL_LIBRARIES}
      ${SSLEAY32_LIBRARY}
    )
  endif (SSLEAY32_FOUND)

  if (SSLEAY32MD_FOUND)
    set(OPENSSL_LIBRARIES
      ${OPENSSL_LIBRARIES}
      ${SSLEAY32MD_LIBRARY}
    )
  endif (SSLEAY32MD_FOUND)

  if (CRYPTO_FOUND)
    set(OPENSSL_LIBRARIES
      ${OPENSSL_LIBRARIES}
      ${CRYPTO_LIBRARY}
    )
  endif (CRYPTO_FOUND)

  if (OPENSSL_INCLUDE_DIRS AND OPENSSL_LIBRARIES)
     set(OPENSSL_FOUND TRUE)
  endif (OPENSSL_INCLUDE_DIRS AND OPENSSL_LIBRARIES)

  if (OPENSSL_FOUND)
    if (NOT OpenSSL_FIND_QUIETLY)
      message(STATUS "Found OpenSSL: ${OPENSSL_LIBRARIES}")
    endif (NOT OpenSSL_FIND_QUIETLY)
  else (OPENSSL_FOUND)
    if (OpenSSL_FIND_REQUIRED)
      message(FATAL_ERROR "Could not find OpenSSL")
    endif (OpenSSL_FIND_REQUIRED)
  endif (OPENSSL_FOUND)

  # show the OPENSSL_INCLUDE_DIRS and OPENSSL_LIBRARIES variables only in the advanced view
  mark_as_advanced(OPENSSL_INCLUDE_DIRS OPENSSL_LIBRARIES)

endif (OPENSSL_LIBRARIES AND OPENSSL_INCLUDE_DIRS)

