# Add PCH support

macro(GCCConfigurePCHFile ARCH)
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/PCH.hpp.gcc.in" "${CMAKE_CURRENT_BINARY_DIR}/${ARCH}/PCH.hpp")
  configure_file("${CMAKE_CURRENT_SOURCE_DIR}/PCH.hpp.gcc.in" "${CMAKE_CURRENT_BINARY_DIR}/${ARCH}/PCH.cpp")
endmacro()

macro(GCCAddPCHTarget ARCH)
  if("${ARCH}" STREQUAL "")
    set(ARCHNAME "")
    set(PCH_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/PCH.cpp")
    set(PCH_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/PCH.hpp.gch")
  else()
    set(ARCHNAME "_${ARCH}")
    set(PCH_SOURCES "${CMAKE_CURRENT_BINARY_DIR}/${ARCH}/PCH.cpp")
    set(PCH_OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${ARCH}/PCH.hpp.gch")
  endif()

  add_library(pchlib${ARCHNAME} STATIC
    ${PCH_SOURCES}
  )

  if(NOT "${ARCH}" STREQUAL "")
    set_target_properties(pchlib${ARCHNAME}
      PROPERTIES OSX_ARCHITECTURES "${arch}"
    )
  endif()

  set_source_files_properties(${PCH_SOURCES}
    PROPERTIES COMPILE_FLAGS "-x c++-header"
  )

  add_custom_command(OUTPUT ${PCH_OUTPUT}
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/pchlib${ARCHNAME}.dir/${arch}/PCH.cpp.o ${PCH_OUTPUT}
    DEPENDS pchlib${ARCHNAME}
  )

  add_custom_target(BUILD_PCH${ARCHNAME}
    DEPENDS ${PCH_OUTPUT}
  )
endmacro()

if(USE_PCH)
  if(CMAKE_COMPILER_IS_GNUCXX)
    if(CMAKE_OSX_ARCHITECTURES)
      foreach(arch ${CMAKE_OSX_ARCHITECTURES})
        GCCConfigurePCHFile(${arch})
      endforeach()
    else()
      GCCConfigurePCHFile("")
    endif()
  elseif(MSVC)
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/PCH.hpp.msvc.in" "${CMAKE_CURRENT_BINARY_DIR}/PCH.hpp")
  endif()

  if(CMAKE_COMPILER_IS_GNUCXX)
    if(CMAKE_OSX_ARCHITECTURES)
      foreach(arch ${CMAKE_OSX_ARCHITECTURES})
        GCCAddPCHTarget(${arch})
      endforeach()
    else()
      GCCAddPCHTarget("")
    endif()
  endif()
endif()

macro(AddPCH TARGET_NAME)
  if(USE_PCH)
    get_target_property(target_files ${TARGET_NAME} SOURCES)

    if(CMAKE_COMPILER_IS_GNUCXX)
      if(CMAKE_OSX_ARCHITECTURES)
        set(PROPSTRING "")
        foreach(arch ${CMAKE_OSX_ARCHITECTURES})
          set(PROPSTRING "${PROPSTRING} -Xarch_${arch} -include${CMAKE_BINARY_DIR}/src/utilities/pch/${arch}/PCH.hpp")
          add_dependencies(${TARGET_NAME} BUILD_PCH_${arch})
        endforeach()
        set_target_properties(${TARGET_NAME}
          PROPERTIES COMPILE_FLAGS "${PROPSTRING}"
        )
      else()
        set_target_properties(${TARGET_NAME}
          PROPERTIES COMPILE_FLAGS "-include ${CMAKE_BINARY_DIR}/src/utilities/pch/PCH.hpp"
        )
        add_dependencies(${TARGET_NAME} BUILD_PCH)
      endif()
    elseif(MSVC)

      set(num_files "0")

      set(cpp_first FALSE)
      set(hpp_first FALSE)

      foreach(f ${target_files})
        get_source_file_property(p "${f}" LOCATION)

        get_filename_component(extension "${p}" EXT)

        if("${extension}" STREQUAL ".cpp" OR "${extension}" STREQUAL ".cxx")
          math(EXPR num_files "${num_files} + 1")
        endif()
      endforeach()


      if(num_files LESS 10)
        message(STATUS "Not enabling PCH for ${TARGET_NAME} for MSVC because the target contains fewer than 10 source files: ${target_files}")
      else()
        set(PCH_OUTPUT "${CMAKE_BINARY_DIR}/${TARGET_NAME}-$(ConfigurationName).hpp.pch")

        set_target_properties(${TARGET_NAME}
          PROPERTIES COMPILE_FLAGS "/FIutilities/pch/PCH.hpp /Yuutilities/pch/PCH.hpp /Fp${PCH_OUTPUT}"
        )

        foreach(f ${target_files})
          get_source_file_property(p "${f}" LOCATION)
          get_filename_component(extension "${p}" EXT)
          if("${extension}" STREQUAL ".cpp" OR "${extension}" STREQUAL ".cxx")
            message(STATUS "Setting /Yc flag on: ${p}")
            set_source_files_properties("${f}"
              PROPERTIES COMPILE_FLAGS "/Ycutilities/pch/PCH.hpp"
            )

            break()
          endif()
        endforeach()
      endif()

    endif()
  endif()
endmacro()


# end of pch support
