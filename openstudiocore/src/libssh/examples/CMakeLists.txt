project(libssh-examples C)

set(examples_SRCS
  authentication.c
  knownhosts.c
  connect_ssh.c
)

include_directories(
  ${LIBSSH_PUBLIC_INCLUDE_DIRS}
  ${LIBSSH_PRIVATE_INCLUDE_DIRS}  
)

add_executable(libssh_scp libssh_scp.c ${examples_SRCS})
add_executable(scp_download scp_download.c ${examples_SRCS})
add_executable(samplessh sample.c ${examples_SRCS})
add_executable(exec exec.c ${examples_SRCS})

target_link_libraries(libssh_scp ${LIBSSH_SHARED_LIBRARY})
target_link_libraries(scp_download ${LIBSSH_SHARED_LIBRARY})
target_link_libraries(samplessh ${LIBSSH_SHARED_LIBRARY})
target_link_libraries(exec ${LIBSSH_SHARED_LIBRARY})

include_directories(
  ${LIBSSH_PUBLIC_INCLUDE_DIRS}
  ${CMAKE_BINARY_DIR}
)

if (WITH_SFTP)
  add_executable(samplesftp samplesftp.c ${examples_SRCS})
  target_link_libraries(samplesftp ${LIBSSH_SHARED_LIBRARY})
endif (WITH_SFTP)

if (WITH_SERVER)
  add_executable(samplesshd samplesshd.c)
  target_link_libraries(samplesshd ${LIBSSH_SHARED_LIBRARY})
endif (WITH_SERVER)

