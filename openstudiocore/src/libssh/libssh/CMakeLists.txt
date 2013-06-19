project(libssh-library C)

set(LIBSSH_PUBLIC_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}/../include
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  CACHE INTERNAL "libssh public include directories"
)

set(LIBSSH_PRIVATE_INCLUDE_DIRS
  ${CMAKE_BINARY_DIR}
  ${ZLIB_INCLUDE_DIRS}
)

set(LIBSSH_SHARED_LIBRARY
  libssh_shared
  CACHE INTERNAL "libssh shared library"
)

if (WITH_STATIC_LIB)
  set(LIBSSH_STATIC_LIBRARY
    libssh_static
    CACHE INTERNAL "libssh static library"
  )
endif (WITH_STATIC_LIB)

set(LIBSSH_LINK_LIBRARIES
  ${LIBSSH_REQUIRED_LIBRARIES}
  ${ZLIB_LIBRARIES}
)

if (WIN32)
  set(LIBSSH_LINK_LIBRARIES
    ${LIBSSH_LINK_LIBRARIES}
    ws2_32
  )
endif (WIN32)

if (HAVE_LIBSOCKET)
  set(LIBSSH_LINK_LIBRARIES
    ${LIBSSH_LINK_LIBRARIES}
    socket
  )
endif (HAVE_LIBSOCKET)

if (OPENSSL_LIBRARIES)
  set(LIBSSH_PRIVATE_INCLUDE_DIRS
    ${LIBSSH_PRIVATE_INCLUDE_DIRS}
    ${OPENSSL_INCLUDE_DIRS}
  )

  set(LIBSSH_LINK_LIBRARIES
    ${LIBSSH_LINK_LIBRARIES}
    ${OPENSSL_LIBRARIES}
  )
endif (OPENSSL_LIBRARIES)

if (GCRYPT_LIBRARY)
  set(LIBSSH_PRIVATE_INCLUDE_DIRS
    ${LIBSSH_PRIVATE_INCLUDE_DIRS}
    ${GCRYPT_INCLUDE_DIRS}
  )

  set(LIBSSH_LINK_LIBRARIES
    ${LIBSSH_LINK_LIBRARIES}
    ${GCRYPT_LIBRARY}
  )
endif (GCRYPT_LIBRARY)

set(libssh_SRCS
  agent.c
  auth.c
  base64.c
  buffer.c
  callbacks.c
  channels.c
  client.c
  config.c
  connect.c
  crc32.c
  crypt.c
  dh.c
  error.c
  gcrypt_missing.c
  gzip.c
  init.c
  kex.c
  keyfiles.c
  keys.c
  log.c
  match.c
  messages.c
  misc.c
  options.c
  packet.c
  poll.c
  session.c
  scp.c
  socket.c
  string.c
  wrapper.c
)

if (WITH_PCAP)
  set(libssh_SRCS
    ${libssh_SRCS}
    pcap.c
  )

endif (WITH_PCAP)

if (WITH_SFTP)
  set(libssh_SRCS
    ${libssh_SRCS}
    sftp.c
  )

  if (WITH_SERVER)
    set(libssh_SRCS
      ${libssh_SRCS}
      sftpserver.c
    )
  endif (WITH_SERVER)
endif (WITH_SFTP)

if (WITH_SSH1)
  set(libssh_SRCS
    ${libssh_SRCS}
    auth1.c
    channels1.c
  )
endif (WITH_SSH1)

if (WITH_SERVER)
  set(libssh_SRCS
    ${libssh_SRCS}
    server.c
  )
endif (WITH_SERVER)

include_directories(
  ${LIBSSH_PUBLIC_INCLUDE_DIRS}
  ${LIBSSH_PRIVATE_INCLUDE_DIRS}
)


if (WITH_SHARED_LIB)
  add_library(${LIBSSH_SHARED_LIBRARY} SHARED ${libssh_SRCS})

  target_link_libraries(${LIBSSH_SHARED_LIBRARY} ${LIBSSH_LINK_LIBRARIES})

  set_target_properties(
    ${LIBSSH_SHARED_LIBRARY}
    PROPERTIES
    VERSION
  	  ${LIBRARY_VERSION}
    SOVERSION
	  ${LIBRARY_SOVERSION}
    OUTPUT_NAME
	  ssh
    DEFINE_SYMBOL
	  LIBSSH_EXPORTS
  )

  if (WITH_VISIBILITY_HIDDEN)
    set_target_properties(${LIBSSH_SHARED_LIBRARY} PROPERTIES COMPILE_FLAGS "-fvisibility=hidden")
  endif (WITH_VISIBILITY_HIDDEN)

  install(
    TARGETS
    ${LIBSSH_SHARED_LIBRARY}
    RUNTIME DESTINATION ${BIN_INSTALL_DIR}
    LIBRARY DESTINATION ${LIB_INSTALL_DIR}
    ARCHIVE DESTINATION ${LIB_INSTALL_DIR}
    COMPONENT libraries
  )
endif (WITH_SHARED_LIB)

if (WITH_STATIC_LIB)
  add_library(${LIBSSH_STATIC_LIBRARY} STATIC ${libssh_SRCS})

  set_target_properties(
    ${LIBSSH_STATIC_LIBRARY}
      PROPERTIES
        VERSION
          ${LIBRARY_VERSION}
        SOVERSION
          ${LIBRARY_SOVERSION}
        COMPILE_FLAGS
          "-DLIBSSH_STATIC"
  )

#  install(
#    TARGETS
#      ${LIBSSH_STATIC_LIBRARY}
#    DESTINATION
#      ${LIB_INSTALL_DIR}
#    COMPONENT
#      libraries
#  )
endif (WITH_STATIC_LIB)

