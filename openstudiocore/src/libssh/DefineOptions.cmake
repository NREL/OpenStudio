IF(UNIX)
option(WITH_LIBZ "Build with ZLIB support" ON)
ELSE()
option(WITH_LIBZ "Build with ZLIB support" OFF)
ENDIF(UNIX)

option(WITH_SSH1 "Build with SSH1 support" OFF)
option(WITH_SFTP "Build with SFTP support" ON)
option(WITH_SERVER "Build with SSH server support" OFF)
option(WITH_STATIC_LIB "Build with a static library" ON)
option(WITH_SHARED_LIB "Build with a shared library" OFF)
option(WITH_DEBUG_CRYPTO "Build with cryto debug output" OFF)
option(WITH_DEBUG_CALLTRACE "Build with calltrace debug output" ON)
option(WITH_GCRYPT "Compile against libgcrypt" OFF)
option(WITH_PCAP "Compile with Pcap generation support" OFF)

MARK_AS_ADVANCED(WITH_SSH1 WITH_SFTP WITH_SERVER WITH_STATIC_LIB WITH_DEBUG_CRYPTO WITH_DEBUG_CALLTRACE
 WITH_GCRYPT WITH_PCAP WITH_LIBZ WITH_SHARED_LIB)