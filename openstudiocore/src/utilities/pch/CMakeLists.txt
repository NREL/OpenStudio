# Add PCH support

MACRO(GCCConfigurePCHFile ARCH)
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/PCH.hpp.gcc.in" "${CMAKE_CURRENT_BINARY_DIR}/${ARCH}/PCH.hpp" )
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/PCH.hpp.gcc.in" "${CMAKE_CURRENT_BINARY_DIR}/${ARCH}/PCH.cpp" )
ENDMACRO()

MACRO(GCCAddPCHTarget ARCH)
  IF ("${ARCH}" STREQUAL "")
    set(ARCHNAME "")
    SET (PCH_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/PCH.cpp" )
    SET (PCH_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/PCH.hpp.gch")
  ELSE()
    set(ARCHNAME "_${ARCH}")
    SET (PCH_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/${ARCH}/PCH.cpp" )
    SET (PCH_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${ARCH}/PCH.hpp.gch")
  ENDIF()

  ADD_LIBRARY( pchlib${ARCHNAME} STATIC
    ${PCH_SOURCES}
    )

  IF (NOT "${ARCH}" STREQUAL "")
    SET_TARGET_PROPERTIES(pchlib${ARCHNAME}
      PROPERTIES OSX_ARCHITECTURES "${arch}"
      )
  ENDIF()

  SET_SOURCE_FILES_PROPERTIES(${PCH_SOURCES}
    PROPERTIES COMPILE_FLAGS "-x c++-header")

  ADD_CUSTOM_COMMAND(OUTPUT ${PCH_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/pchlib${ARCHNAME}.dir/${arch}/PCH.cpp.o ${PCH_OUTPUT}
    DEPENDS pchlib${ARCHNAME}
    )

  ADD_CUSTOM_TARGET(BUILD_PCH${ARCHNAME}
    DEPENDS
    ${PCH_OUTPUT}
    )
ENDMACRO()

IF (USE_PCH)
  IF (CMAKE_COMPILER_IS_GNUCXX)
    IF (CMAKE_OSX_ARCHITECTURES)
      FOREACH(arch ${CMAKE_OSX_ARCHITECTURES})
        GCCConfigurePCHFile(${arch})
      ENDFOREACH()
    ELSE()
      GCCConfigurePCHFile("")
    ENDIF()
  ELSEIF(MSVC)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/PCH.hpp.msvc.in" "${CMAKE_CURRENT_BINARY_DIR}/PCH.hpp" )
  ENDIF()

  IF(CMAKE_COMPILER_IS_GNUCXX)
    IF (CMAKE_OSX_ARCHITECTURES)
      FOREACH(arch ${CMAKE_OSX_ARCHITECTURES})
        GCCAddPCHTarget(${arch})
      ENDFOREACH()
    ELSE()
      GCCAddPCHTarget("")
    ENDIF()
  ENDIF()
ENDIF()

MACRO(AddPCH TARGET_NAME)
  IF (USE_PCH)
    GET_TARGET_PROPERTY(target_files ${TARGET_NAME} SOURCES)

    IF (CMAKE_COMPILER_IS_GNUCXX)
      IF (CMAKE_OSX_ARCHITECTURES)
        SET(PROPSTRING "")
        FOREACH(arch ${CMAKE_OSX_ARCHITECTURES})
          SET(PROPSTRING  "${PROPSTRING} -Xarch_${arch} -include${CMAKE_BINARY_DIR}/src/utilities/pch/${arch}/PCH.hpp")
          ADD_DEPENDENCIES( ${TARGET_NAME} BUILD_PCH_${arch} )
        ENDFOREACH()
        SET_TARGET_PROPERTIES(${TARGET_NAME}
          PROPERTIES COMPILE_FLAGS "${PROPSTRING}")
      ELSE()
        SET_TARGET_PROPERTIES(${TARGET_NAME}
          PROPERTIES COMPILE_FLAGS "-include ${CMAKE_BINARY_DIR}/src/utilities/pch/PCH.hpp")
        ADD_DEPENDENCIES( ${TARGET_NAME} BUILD_PCH )
      ENDIF()
    ELSEIF(MSVC)

      SET(num_files "0")

      SET(cpp_first FALSE)
      SET(hpp_first FALSE)

      FOREACH(f ${target_files})
        GET_SOURCE_FILE_PROPERTY(p "${f}" LOCATION)

        GET_FILENAME_COMPONENT(extension "${p}" EXT)

        IF ("${extension}" STREQUAL ".cpp" OR "${extension}" STREQUAL ".cxx")
          MATH(EXPR num_files "${num_files} + 1" )
        ENDIF()
      ENDFOREACH()


      IF (num_files LESS 10)
        MESSAGE(STATUS "Not enabling PCH for ${TARGET_NAME} for MSVC because the target contains fewer than 10 source files: ${target_files}")
      ELSE()
        SET(PCH_OUTPUT "${CMAKE_BINARY_DIR}/${TARGET_NAME}-$(ConfigurationName).hpp.pch")

        SET_TARGET_PROPERTIES(${TARGET_NAME}
          PROPERTIES COMPILE_FLAGS "/FIutilities/pch/PCH.hpp /Yuutilities/pch/PCH.hpp /Fp${PCH_OUTPUT}")

        FOREACH(f ${target_files})
          GET_SOURCE_FILE_PROPERTY(p "${f}" LOCATION)
          GET_FILENAME_COMPONENT(extension "${p}" EXT)
          IF ("${extension}" STREQUAL ".cpp" OR "${extension}" STREQUAL ".cxx")
            MESSAGE(STATUS "Setting /Yc flag on: ${p}")
            SET_SOURCE_FILES_PROPERTIES("${f}"
              PROPERTIES COMPILE_FLAGS "/Ycutilities/pch/PCH.hpp")

            BREAK()
          ENDIF()
        ENDFOREACH()
      ENDIF()

    ENDIF()
  ENDIF()
ENDMACRO()


# end of pch support

