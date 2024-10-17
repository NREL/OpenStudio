#
# RunCommands.cmake
#
# Run a set of commands in series.
# This is useful for creating tests which consist of more than one command.
#
# Commands are passed as -DCMD<n> as n is between 1 and 9
#
# Example Usage:
# add_test(test_name COMMAND ${CMAKE_COMMAND}
#   "-DCMD1=echo hello"
#   "-DCMD2=echo goodbye"
#   -P ${CMAKE_SOURCE_DIR}/CMake/RunCommands)
#
# An error will be throw if any command returns a non-zero exit code or if a command cannot be found.
# When an error is thrown no further commands will be executed.
#
# STDOUT and STDERR are untouched and reach output as normal.
# Before running each command is echoed to STDOUT.

foreach(CMD_IDX RANGE 1 10)
  if(DEFINED CMD${CMD_IDX})
    separate_arguments(ARGS NATIVE_COMMAND PROGRAM SEPARATE_ARGS "${CMD${CMD_IDX}}")
    if(ARGS)
      list(POP_FRONT ARGS CMD)
      execute_process(COMMAND ${CMD} ${ARGS} COMMAND_ECHO STDOUT COMMAND_ERROR_IS_FATAL ANY)
    else()
      message(FATAL_ERROR "Cannot find command to run: '${CMD${CMD_IDX}}'")
    endif()
  else()
    break()
  endif()
endforeach()
