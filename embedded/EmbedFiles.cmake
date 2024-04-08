#[=======================================================================[.rst:
EmbedFiles
----------

This module defines functions to embed some files as binary into the executable.

.. command:: embed_files

  This is the command responsible for calling CreateEmbeddedSource::

    embed_files(<FILES> <EMBEDDED_LOCATIONS> <CXX_OUTPUT_FILES>)

  The required arguments:

  ``FILES``
    The list of file paths on disk to embed

  ``EMBEDDED_LOCATIONS``
    a indexed matched list of paths for each embedded file

  ``CXX_OUTPUT_FILES``
    a variable that will be set to a list of cxx files to compile executable or library

.. command:: glob_for_file_exts_and_prepare_for_embedded

  This is a helper that will glob recursively for specific file extensions in a base directory and return
  two lists corresponding to ``FILES`` and ``EMBEDDED_LOCATION`` (a relative path with ``BASE_DIR`` as base)

    glob_for_file_exts_and_prepare_for_embedded(
      <BASE_DIR> <foundFiles> <foundEmbeddedPaths>
      [VERBOSE]
      EXTENSIONS <ext> [<ext>...]
    )

  The required arguments:

  ``BASE_DIR``
    The base directory on disk to search in

  ``foundFiles``
  Result variable for the disk files found that match the extensions requested

  ``foundEmbeddedPaths``
  Paths relative to the ``BASE_DIR``

  ``EXTENSIONS``
  a list of extensions to search for. At least one must be provided or it'll throw

  The options are:

  ``VERBOSE``
  outputs a table with the number of files found for each extension requested
#]=======================================================================]

#------------------------------------------------------------------------------
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
    file(RELATIVE_PATH EMBED_SOURCE_FILE_REL_PATH ${PROJECT_BINARY_DIR} ${EMBED_SOURCE_FILE})
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

  configure_file("${PROJECT_SOURCE_DIR}/embedded/embedded_files.hxx.in" "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.hxx")
  list(APPEND EMBED_SOURCE_FILES "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.hxx")

  configure_file("${PROJECT_SOURCE_DIR}/embedded/embedded_files.cxx.in" "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.cxx")
  list(APPEND EMBED_SOURCE_FILES "${CMAKE_CURRENT_BINARY_DIR}/embedded_files.cxx")

  set(${CXX_OUTPUT_FILES} ${EMBED_SOURCE_FILES} PARENT_SCOPE)
endfunction()
#------------------------------------------------------------------------------


#------------------------------------------------------------------------------
function(glob_for_file_exts_and_prepare_for_embedded BASE_DIR foundFiles foundEmbeddedPaths)
  set(prefix "")
  set(valueLessKeywords VERBOSE)
  set(singleValueKeywords "")
  set(multiValueKeywords EXTENSIONS)

  cmake_parse_arguments(
    PARSE_ARGV 3 # Start at one with NAME is the first param
      "${prefix}"
      "${valueLessKeywords}"
      "${singleValueKeywords}"
      "${multiValueKeywords}"
  )

  if (_UNPARSED_ARGUMENTS)
    message(FATAL_ERROR "Extra unknown arguments were passed: ${_UNPARSED_ARGUMENTS}")
  endif()
  if (_KEYWORDS_MISSING_VALUES)
    message(FATAL_ERROR "Keywords missing values: ${_KEYWORDS_MISSING_VALUES}")
  endif()
  if (_VERBOSE)
    message("Globbing in ${BASE_DIR}:")
    message("| extension | nFound |")
    message("| --------- | ------ |")
  endif()

  if (NOT _EXTENSIONS)
    message(FATAL_ERROR "EXTENSIONS is required")
  endif()
  foreach(_ext ${_EXTENSIONS})
    file(GLOB_RECURSE _found_files FOLLOW_SYMLINKS "${BASE_DIR}/**/*.${_ext}")
    if (_VERBOSE)
      list(LENGTH _found_files _nFound)
      # message("- Found ${_nFound} files for extension ${_ext}")
      string(LENGTH ${_ext} _extLength)
      math(EXPR _spacing "9 - ${_extLength}")
      string(REPEAT " " ${_spacing} _spaces)

      string(LENGTH ${_nFound} _foundLength)
      math(EXPR _spacing "6 - ${_foundLength}")
      string(REPEAT " " ${_spacing} _spaces2)

      message("| ${_ext}${_spaces} | ${_nFound}${_spaces2} |")
    endif()
    foreach(_file ${_found_files} )
      file(RELATIVE_PATH LOCATION ${BASE_DIR} ${_file})
      list(APPEND _files ${_file})
      list(APPEND _embeddedPaths ${LOCATION})
    endforeach()
  endforeach()

  if (_VERBOSE)
    list(LENGTH _files _nFound)
    message("| ========= | ====== |")
    message("|   Total   |  ${_nFound}  |")
  endif()

  # Return
  set(${foundFiles} ${_files} PARENT_SCOPE)
  set(${foundEmbeddedPaths} ${_embeddedPaths} PARENT_SCOPE)
endfunction()
#------------------------------------------------------------------------------
