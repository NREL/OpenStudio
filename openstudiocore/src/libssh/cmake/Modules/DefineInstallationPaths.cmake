if (UNIX)
  IF (NOT APPLICATION_NAME)
    MESSAGE(STATUS "${PROJECT_NAME} is used as APPLICATION_NAME")
    SET(APPLICATION_NAME ${PROJECT_NAME})
  ENDIF (NOT APPLICATION_NAME)

  # Suffix for Linux
  SET(LIB_SUFFIX
    CACHE STRING "Define suffix of directory name (32/64)"
  )

  SET(EXEC_INSTALL_PREFIX
    "${CMAKE_INSTALL_PREFIX}"
    CACHE INTERNAL  "Base directory for executables and libraries"
  )
  SET(SHARE_INSTALL_PREFIX
    "${CMAKE_INSTALL_PREFIX}/share"
    CACHE INTERNAL "Base directory for files which go to share/"
  )
  SET(DATA_INSTALL_PREFIX
    "${SHARE_INSTALL_PREFIX}/${APPLICATION_NAME}"
    CACHE INTERNAL "The parent directory where applications can install their data")

  # The following are directories where stuff will be installed to
  SET(BIN_INSTALL_DIR
    "${EXEC_INSTALL_PREFIX}/bin"
    CACHE INTERNAL "The ${APPLICATION_NAME} binary install dir (default prefix/bin)"
  )
  SET(SBIN_INSTALL_DIR
    "${EXEC_INSTALL_PREFIX}/sbin"
    CACHE INTERNAL "The ${APPLICATION_NAME} sbin install dir (default prefix/sbin)"
  )
  SET(LIB_INSTALL_DIR
    "${EXEC_INSTALL_PREFIX}/lib${LIB_SUFFIX}"
    CACHE INTERNAL "The subdirectory relative to the install prefix where libraries will be installed (default is prefix/lib)"
  )
  SET(LIBEXEC_INSTALL_DIR
    "${EXEC_INSTALL_PREFIX}/libexec"
    CACHE INTERNAL "The subdirectory relative to the install prefix where libraries will be installed (default is prefix/libexec)"
  )
  SET(PLUGIN_INSTALL_DIR
    "${LIB_INSTALL_DIR}/${APPLICATION_NAME}"
    CACHE INTERNAL "The subdirectory relative to the install prefix where plugins will be installed (default is prefix/lib/${APPLICATION_NAME})"
  )
  SET(INCLUDE_INSTALL_DIR
    "${CMAKE_INSTALL_PREFIX}/include"
    CACHE INTERNAL "The subdirectory to the header prefix (default prefix/include)"
  )

  SET(DATA_INSTALL_DIR
    "${DATA_INSTALL_PREFIX}"
    CACHE INTERNAL "The parent directory where applications can install their data (default prefix/share/${APPLICATION_NAME})"
  )
  SET(HTML_INSTALL_DIR
    "${DATA_INSTALL_PREFIX}/doc/HTML"
    CACHE INTERNAL "The HTML install dir for documentation (default data/doc/html)"
  )
  SET(ICON_INSTALL_DIR
    "${DATA_INSTALL_PREFIX}/icons"
    CACHE INTERNAL "The icon install dir (default data/icons/)"
  )
  SET(SOUND_INSTALL_DIR
    "${DATA_INSTALL_PREFIX}/sounds"
    CACHE INTERNAL "The install dir for sound files (default data/sounds)"
  )

  SET(LOCALE_INSTALL_DIR
    "${SHARE_INSTALL_PREFIX}/locale"
    CACHE INTERNAL "The install dir for translations (default prefix/share/locale)"
  )

  SET(XDG_APPS_DIR
    "${SHARE_INSTALL_PREFIX}/applications/"
    CACHE INTERNAL "The XDG apps dir"
  )
  SET(XDG_DIRECTORY_DIR
    "${SHARE_INSTALL_PREFIX}/desktop-directories"
    CACHE INTERNAL "The XDG directory"
  )

  SET(SYSCONF_INSTALL_DIR
    "${EXEC_INSTALL_PREFIX}/etc"
    CACHE INTERNAL "The ${APPLICATION_NAME} sysconfig install dir (default prefix/etc)"
  )
  SET(MAN_INSTALL_DIR
    "${SHARE_INSTALL_PREFIX}/man"
    CACHE INTERNAL "The ${APPLICATION_NAME} man install dir (default prefix/man)"
  )
  SET(INFO_INSTALL_DIR
    "${SHARE_INSTALL_PREFIX}/info"
    CACHE INTERNAL "The ${APPLICATION_NAME} info install dir (default prefix/info)"
  )
endif (UNIX)

if (WIN32)
  # Same same
  set(BIN_INSTALL_DIR "bin" CACHE INTERNAL "-")
  set(SBIN_INSTALL_DIR "." CACHE INTERNAL "-")
  set(LIB_INSTALL_DIR "lib" CACHE INTERNAL "-")
  set(INCLUDE_INSTALL_DIR "include" CACHE INTERNAL "-")
  set(PLUGIN_INSTALL_DIR "plugins" CACHE INTERNAL "-")
  set(HTML_INSTALL_DIR "doc/HTML" CACHE INTERNAL "-")
  set(ICON_INSTALL_DIR "." CACHE INTERNAL "-")
  set(SOUND_INSTALL_DIR "." CACHE INTERNAL "-")
  set(LOCALE_INSTALL_DIR "lang" CACHE INTERNAL "-")
endif (WIN32)

