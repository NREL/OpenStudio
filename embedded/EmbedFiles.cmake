# FILES is a list of file paths to embed
# EMBEDDED_LOCATIONS is a indexed matched list of paths for each embedded file
# CXX_OUTPUT_FILES is a variable that will be set to a list of cxx files to compile executable or library
function(embed_files FILES EMBEDDED_LOCATIONS CXX_OUTPUT_FILES)
  list(LENGTH FILES FILES_LENGTH)
  list(LENGTH EMBEDDED_LOCATIONS EMBEDDED_LOCATIONS_LENGTH)

  if( NOT (FILES_LENGTH EQUAL EMBEDDED_LOCATIONS_LENGTH) )
    message(ERROR "embed_files function must be called with FILES and EMBEDDED_LOCATIONS lists of the same length")
    return()
  endif()

  if( ARGV3 )
    set(NAMESPACE ${ARGV3})
  endif()

  math(EXPR END "${FILES_LENGTH} - 1")
  set(NUMFILES ${FILES_LENGTH})

  foreach(i RANGE ${END})
    list(GET FILES ${i} FILE)
    list(GET EMBEDDED_LOCATIONS ${i} EMBEDDED_LOCATION)

    get_filename_component(BASE_PATH ${EMBEDDED_LOCATION} DIRECTORY)
    get_filename_component(BASE_FILENAME ${EMBEDDED_LOCATION} NAME_WE)
    get_filename_component(EXTENSION ${EMBEDDED_LOCATION} EXT)
    
    if (EXTENSION)
      string(REPLACE "." "_" EXTENSION ${EXTENSION})
    endif()

    # EMBED_SOURCE_FILE is the fully qualified path to a cxx file with the embedded data
    set(EMBED_SOURCE_FILE "${CMAKE_CURRENT_BINARY_DIR}/embedded_files/${BASE_PATH}/${BASE_FILENAME}${EXTENSION}.cxx")
    set(EMBED_SOURCE_PATH "${CMAKE_CURRENT_BINARY_DIR}/embedded_files/${BASE_PATH}")
    file(RELATIVE_PATH EMBED_SOURCE_FILE_REL_PATH ${CMAKE_BINARY_DIR} ${EMBED_SOURCE_FILE})
    list(APPEND EMBED_SOURCE_FILES ${EMBED_SOURCE_FILE})

    # Fixup the name. We want to make sure it starts with :/
    string(SUBSTRING ${EMBEDDED_LOCATION} 0 1 EMBEDDED_LOCATION_PREFIX)
    string(SUBSTRING ${EMBEDDED_LOCATION} 0 2 EMBEDDED_LOCATION_PREFIX2)
    if( EMBEDDED_LOCATION_PREFIX2 STREQUAL :/ )
      # don't do anything, already in embedded form
    elseif( EMBEDDED_LOCATION_PREFIX STREQUAL / )
      set( EMBEDDED_LOCATION ":${EMBEDDED_LOCATION}" )
    else()
      set( EMBEDDED_LOCATION ":/${EMBEDDED_LOCATION}" )
    endif()

    add_custom_command(OUTPUT ${EMBED_SOURCE_FILE}
      COMMAND ${CMAKE_COMMAND} -E make_directory ${EMBED_SOURCE_PATH}
      COMMAND CreateEmbeddedSource
        ${FILE}
        ${EMBED_SOURCE_FILE}
        ${i}
        ${EMBEDDED_LOCATION}
        DEPENDS ${FILE}
    )

    set(EMBEDDED_FILE_INCLUDES "${EMBEDDED_FILE_INCLUDES}#include <${EMBED_SOURCE_FILE_REL_PATH}>\n")
  endforeach()

  # file names
  foreach (arg RANGE ${END})
    set(EMBEDDED_FILE_NAMES "${EMBEDDED_FILE_NAMES}    embedded_file_name_${arg}")
    if (NOT arg EQUAL MAXFILECOUNT)
      set(EMBEDDED_FILE_NAMES "${EMBEDDED_FILE_NAMES},")
    endif()
    set(EMBEDDED_FILE_NAMES "${EMBEDDED_FILE_NAMES}\n")
  endforeach()

  # lengths
  foreach (arg RANGE ${END})
    set(EMBEDDED_FILE_LENS "${EMBEDDED_FILE_LENS}    embedded_file_len_${arg}")
    if (NOT arg EQUAL MAXFILECOUNT)
      set(EMBEDDED_FILE_LENS "${EMBEDDED_FILE_LENS},")
    endif()
    set(EMBEDDED_FILE_LENS "${EMBEDDED_FILE_LENS}\n")
  endforeach()

  # datas
  foreach (arg RANGE ${END})
    set(EMBEDDED_FILES "${EMBEDDED_FILES}    embedded_file_${arg}")
    if (NOT arg EQUAL MAXFILECOUNT)
      set(EMBEDDED_FILES "${EMBEDDED_FILES},")
    endif()
    set(EMBEDDED_FILES "${EMBEDDED_FILES}\n")
  endforeach()

  set_source_files_properties(${EMBED_SOURCE_FILES} PROPERTIES HEADER_FILE_ONLY TRUE)

  if( NAMESPACE )
    set(BEGIN_NAMESPACE "namespace ${NAMESPACE} {")
    set(END_NAMESPACE "}")
  endif()

  configure_file("${CMAKE_SOURCE_DIR}/embedded/embedded_files.hxx.in" "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.hxx")
  list(APPEND EMBED_SOURCE_FILES "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.hxx")

  configure_file("${CMAKE_SOURCE_DIR}/embedded/embedded_files.cxx.in" "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.cxx")
  list(APPEND EMBED_SOURCE_FILES "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.cxx")

  set(${CXX_OUTPUT_FILES} ${EMBED_SOURCE_FILES} PARENT_SCOPE)
endfunction()

