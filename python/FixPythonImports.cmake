if(${CMAKE_ARGC} LESS 5)
  foreach(i RANGE 0 ${CMAKE_ARGC})
    message("CMAKE_ARGV${i}=${CMAKE_ARGV${i}}")
  endforeach(i)
  message(FATAL_ERROR "FixPythonImports needs to be called with exactly two arguments (${CMAKE_ARGC} was supplied), the PYTHON_GENERATED_SRC full path, and the MODIFIED_PYTHON_GENERATED_SRC full path")
endif()


set(PYTHON_GENERATED_SRC "${CMAKE_ARGV3}")
set(MODIFIED_PYTHON_GENERATED_SRC "${CMAKE_ARGV4}")
file(READ "${PYTHON_GENERATED_SRC}" filedata)
string(REGEX REPLACE "([ \t]*)import openstudio" "\\1from .import openstudio" filedata "${filedata}")
string(REGEX REPLACE "([ \t]*)from openstudio" "\\1from .openstudio" filedata "${filedata}")
file(WRITE "${MODIFIED_PYTHON_GENERATED_SRC}" "${filedata}")
