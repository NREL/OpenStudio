# currently we are using only the ResultsViewer resources for Ruby tests

# which directory will we run ruby tests from
SET(RUBY_RUN_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")

# get all possible configurations
LIST(LENGTH CMAKE_CONFIGURATION_TYPES POSSIBLE_CONFIGURATIONS_LENGTH)
IF(CMAKE_CONFIGURATION_TYPES)
  SET(POSSIBLE_CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES})
ELSE()
  SET(POSSIBLE_CONFIGURATIONS ".")
ENDIF()

INCLUDE(GetPrerequisites)

# find all source
file(GLOB_RECURSE RUBY_SRC "openstudio/*.rb" "cloud/*.template" "cloud/*.sh")

# find all tests
file(GLOB_RECURSE RUBY_TEST_SRC "openstudio/*_Test.rb")

# add a test for each unit test
SET( RUBY_TEST_REQUIRES "#include test files" )
FOREACH( f ${RUBY_TEST_SRC} )

  FILE( RELATIVE_PATH REQUIRE_NAME "${CMAKE_SOURCE_DIR}/ruby/" ${f} )
  SET( RUBY_TEST_REQUIRES "${RUBY_TEST_REQUIRES} \nrequire '${REQUIRE_NAME}'" )

  FILE( READ "${f}" CONTENTS)
  STRING( REGEX MATCHALL "def +test_([A-Za-z_0-9 ]+)" FOUND_TESTS ${CONTENTS} )

  FOREACH(HIT ${FOUND_TESTS})
    STRING(REGEX REPLACE "def +test_([A-Za-z_0-9]+)" "\\1" TEST_NAME ${HIT})
    STRING(REGEX MATCH "/?([A-Za-z_0-9 ]+)\\.rb" FILE_NAME ${f})
    STRING(REGEX REPLACE "/?([A-Za-z_0-9 ]+)\\.rb" "\\1" FILE_NAME ${FILE_NAME})
    IF(BUILD_TESTING)
      IF(MSVC)
        ADD_TEST(NAME "RubyTest-${FILE_NAME}-${TEST_NAME}"
          COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>"
          "$<TARGET_FILE:ruby_cpp>" -I "${RUBY_RUBY_LIB_DIR}" -I "${RUBY_RUBY_LIB_DIR}/i386-mingw32" -I. "openstudio_test.rb" "--name=test_${TEST_NAME}"
        )
      ELSE()
        # $<CONFIGURATION> may resolve to 'Release' on Mac or Linux but directory will not exist
        ADD_TEST(NAME "RubyTest-${FILE_NAME}-${TEST_NAME}"
          COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
          "${RUBY_EXECUTABLE}" -I"${RUBY_RUBY_LIB_DIR}" -I. "openstudio_test.rb" "--name=test_${TEST_NAME}"
        )
      ENDIF()
      SET_TESTS_PROPERTIES("RubyTest-${FILE_NAME}-${TEST_NAME}" PROPERTIES DEPENDS "${ALL_RUBY_BINDING_TARGETS}" ruby_cpp ResultsViewer_resources)
    ENDIF(BUILD_TESTING)
  ENDFOREACH(HIT)
ENDFOREACH( f ${RUBY_TEST_SRC} )

# configure ruby files for installer
FILE( COPY "${CMAKE_SOURCE_DIR}/ruby/openstudio.rb" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/install" )
FILE( COPY "${CMAKE_SOURCE_DIR}/ruby/Win32API.so" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/install" )
CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/ruby/openstudio_test.in" "${CMAKE_CURRENT_BINARY_DIR}/install/openstudio_test.rb" )
CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/ruby/openstudio/sketchup_plugin/Startup.in" "${CMAKE_CURRENT_BINARY_DIR}/install/SketchUp/Plugins/OpenStudio/Startup.rb" )
FILE( COPY "${CMAKE_SOURCE_DIR}/ruby/openstudio/sketchup_plugin/OpenStudio.rb" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/install/SketchUp/Plugins" )

CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/ruby/cloud/aws.rb.in" "${CMAKE_CURRENT_BINARY_DIR}/cloud/aws.rb" )
IF (WIN32 OR APPLE)
  INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/cloud/aws.rb" DESTINATION "Ruby/cloud" )
ELSE() #Linux
  INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/cloud/aws.rb" DESTINATION "${RUBY_MODULE_DIR}/cloud" )
  INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/cloud/aws.rb" DESTINATION "share/openstudio/Ruby/cloud" )
ENDIF()
FILE( COPY "${CMAKE_SOURCE_DIR}/ruby/cloud/mongoid.yml.template" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/cloud" )
FILE( COPY "${CMAKE_SOURCE_DIR}/ruby/cloud/server_script.sh" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/cloud" )
FILE( COPY "${CMAKE_SOURCE_DIR}/ruby/cloud/worker_script.sh.template" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/cloud" )

# install qt plugins and fixup for apple

IF( WIN32 OR APPLE )
  get_target_property(QT_QSQLITE_PLUGIN Qt5::QSQLiteDriverPlugin LOCATION)

  install( PROGRAMS "${QT_QSQLITE_PLUGIN}"
           DESTINATION "Ruby/openstudio/sqldrivers/"
         )

  get_filename_component(QSQLITE_PLUGIN_NAME "${QT_QSQLITE_PLUGIN}" NAME)

  IF( WIN32 )
    get_target_property(QT_WINDOWS_PLUGIN Qt5::QWindowsIntegrationPlugin LOCATION)

    install( PROGRAMS "${QT_WINDOWS_PLUGIN}"
             DESTINATION "Ruby/openstudio/platforms/" 
           )
  ENDIF()

  IF(APPLE)
    SET(QTPLUGIN "\${CMAKE_INSTALL_PREFIX}/Ruby/openstudio/sqldrivers/${QSQLITE_PLUGIN_NAME}")

    SET( Prereq_Dirs
    )

    INSTALL(CODE "
      INCLUDE(GetPrerequisites)
      GET_PREREQUISITES( \"${QTPLUGIN}\" PREREQUISITES 1 0 \"\" \"${Prereq_Dirs}\" )

      FOREACH( PREREQ IN LISTS PREREQUISITES )
        GP_RESOLVE_ITEM( \"\" \${PREREQ} \"\" \"${LIBRARY_SEARCH_DIRECTORY}\" resolved_item_var )

        GET_FILENAME_COMPONENT( PREREQNAME \${resolved_item_var} NAME)

        EXECUTE_PROCESS(COMMAND \"install_name_tool\" -change \"\${PREREQ}\" \"@loader_path/../\${PREREQNAME}\" \"${QTPLUGIN}\")
      ENDFOREACH( PREREQ IN LISTS PREREQUISITES )
    " )
  ENDIF()
ENDIF()

# optionally build ruby gem
IF (BUILD_RUBY_GEM)
  SET(GEMDIR "${CMAKE_BINARY_DIR}/Products/RubyGem")

  INSTALL(CODE "
    EXECUTE_PROCESS(COMMAND \"${CMAKE_COMMAND}\" -E remove_directory \"${GEMDIR}/lib\")
    EXECUTE_PROCESS(COMMAND \"${CMAKE_COMMAND}\" -E make_directory \"${GEMDIR}/lib\")
    EXECUTE_PROCESS(COMMAND \"${CMAKE_COMMAND}\" -E remove_directory \"${GEMDIR}/share\")
    EXECUTE_PROCESS(COMMAND \"${CMAKE_COMMAND}\" -E make_directory \"${GEMDIR}/share/openstudio\")
    EXECUTE_PROCESS(COMMAND \"${CMAKE_COMMAND}\" -E copy \"${CMAKE_CURRENT_SOURCE_DIR}/openstudio.rb\" \"${GEMDIR}/lib\")
  ")

  # Find libs that need to be copied for generating ruby gem
  IF(BUILD_RUBY_GEM_WITH_LIBS)
    INSTALL(CODE "
      SET(qtinstalled false)
      SET(SEARCH_PATHS \"/usr/local/lib;\${CMAKE_INSTALL_PREFIX}/lib;\${CMAKE_INSTALL_PREFIX}/bin;${Boost_LIBRARY_DIRS};${QT_LIBRARY_DIR}\")
      FILE(GLOB ruby_libs \"\${CMAKE_INSTALL_PREFIX}/${RUBY_MODULE_ARCH_DIR}/*\")
      INCLUDE(GetPrerequisites)
      FOREACH(lib IN LISTS ruby_libs)
        GET_PREREQUISITES(\"\${lib}\" prereqs 1 1 \"\" \"\${SEARCH_PATHS}\" )
        EXECUTE_PROCESS(COMMAND \"${CMAKE_COMMAND}\" -E copy_if_different \"\${lib}\" \"${GEMDIR}/lib\")
        FOREACH(prereq IN LISTS prereqs)
          STRING(TOLOWER \"\${prereq}\" lowerprereq)
          STRING(REGEX MATCH \".*qtcore.*\" regexmatch \"\${lowerprereq}\")
          IF(regexmatch)
            SET(qtinstalled true)
          ENDIF()
          GP_RESOLVE_ITEM( \"\" \${prereq} \"\" \"\${SEARCH_PATHS}\" resolved_item_var )
          EXECUTE_PROCESS(COMMAND \"${CMAKE_COMMAND}\" -E copy_if_different \"\${resolved_item_var}\" \"${GEMDIR}/lib\")
          GET_FILENAME_COMPONENT( PREREQNAME \${resolved_item_var} NAME)
        ENDFOREACH(prereq IN LISTS prereqs)
      ENDFOREACH(lib IN LISTS ruby_libs)

      execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy_directory \"\${CMAKE_INSTALL_PREFIX}/share/openstudio/osversion\" \"${GEMDIR}/share/openstudio/osversion\")

      IF( qtinstalled )
        execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy_directory \"${QT_PLUGINS_DIR}\" \"${GEMDIR}/lib/plugins\")
      ENDIF()

     ")
  ENDIF()

  CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/ruby/openstudio.gemspec.in" "${CMAKE_BINARY_DIR}/Products/RubyGem/openstudio.gemspec" )

  INSTALL(CODE "
    IF(WIN32)
      EXECUTE_PROCESS(COMMAND cmd.exe /C \"${GEM} build ${CMAKE_BINARY_DIR}/Products/RubyGem/openstudio.gemspec\"
                      WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}/Products/RubyGem/\")
    ELSE()
      EXECUTE_PROCESS(COMMAND \"${GEM}\" build \"${CMAKE_BINARY_DIR}/Products/RubyGem/openstudio.gemspec\"
                      WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}/Products/RubyGem/\")
    ENDIF()

    EXECUTE_PROCESS(COMMAND \"${CMAKE_COMMAND}\" -E copy_if_different \"${CMAKE_BINARY_DIR}/Products/RubyGem/openstudio-${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}.gem\" \"${CMAKE_BINARY_DIR}/../../../\")
  ")
ENDIF()


# configure ruby files for testing from build directory
FOREACH(POSSIBLE_CONFIGURATION ${POSSIBLE_CONFIGURATIONS})
  FILE( COPY "${CMAKE_SOURCE_DIR}/ruby/openstudio_dev.rb" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}" )
  FILE( RENAME "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/openstudio_dev.rb" "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/openstudio.rb" )

  CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/ruby/config.in" "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/config.rb" )
  CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/ruby/openstudio_test.in" "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/openstudio_test.rb" )

  CONFIGURE_FILE( "${CMAKE_SOURCE_DIR}/ruby/openstudio/sketchup_plugin/Startup_dev.in" "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/SketchUp/Plugins/OpenStudio/Startup.rb" )
  FILE( COPY "${CMAKE_SOURCE_DIR}/ruby/openstudio/sketchup_plugin/OpenStudio.rb" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/SketchUp/Plugins" )
  
  IF( WIN32 AND ${BUILD_WITH_OPENSSL} AND ${OPENSSL_FOUND} )
    FILE(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Products/ruby/${POSSIBLE_CONFIGURATION}")
    FILE(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/ruby/${POSSIBLE_CONFIGURATION}")
  ENDIF()

ENDFOREACH(POSSIBLE_CONFIGURATION IN ${POSSIBLE_CONFIGURATIONS})

# run rdoc for openstudio
IF( BUILD_DOCUMENTATION )
  ADD_CUSTOM_TARGET( ruby_OpenStudioSketchUpPlugin_rdoc
    ${CMAKE_COMMAND} -E chdir "${RUBY_RUN_DIR}" "${RUBY_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/../developer/ruby/OpenStudioSketchUpPluginRDoc.rb" "${RUBY_RUN_DIR}/"
  )

  list( APPEND ALL_RDOC_TARGETS ruby_OpenStudioSketchUpPlugin_rdoc )
  set( ALL_RDOC_TARGETS "${ALL_RDOC_TARGETS}" PARENT_SCOPE )

ENDIF()

# some unit tests previously defined depend on the ruby bindings, set those here so they are all defined
IF( BUILD_TESTING )
  ADD_DEPENDENCIES(openstudio_runmanager_tests ${ALL_RUBY_BINDING_TARGETS})
  ADD_DEPENDENCIES(openstudio_analysisdriver_tests ${ALL_RUBY_BINDING_TARGETS})
ENDIF()

# run all the unit tests
IF( ENABLE_TEST_RUNNER_TARGETS )
  IF(MSVC)
    ADD_CUSTOM_TARGET( ruby_run_tests
      ${CMAKE_COMMAND} -E chdir "${RUBY_RUN_DIR}" "${RUBY_EXECUTABLE}" -I "${RUBY_RUBY_LIB_DIR}" -I "${RUBY_RUBY_LIB_DIR}/i386-mingw32" -I. "openstudio_test.rb"
      DEPENDS ${ALL_RUBY_BINDING_TARGETS} ruby_cpp ResultsViewer_resources openstudio_energyplus_resources
    )  
  ELSE()
    ADD_CUSTOM_TARGET( ruby_run_tests
      ${CMAKE_COMMAND} -E chdir "${RUBY_RUN_DIR}" "${RUBY_EXECUTABLE}" -I "${RUBY_RUBY_LIB_DIR}" -I. "openstudio_test.rb"
      DEPENDS ${ALL_RUBY_BINDING_TARGETS} ruby_cpp ResultsViewer_resources openstudio_energyplus_resources
    )  
  ENDIF()
ENDIF()

# install the files
INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/install/openstudio.rb" DESTINATION ${RUBY_MODULE_DIR} )
INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/install/Win32API.so" DESTINATION ${RUBY_MODULE_DIR} )
INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/install/openstudio_test.rb" DESTINATION ${RUBY_MODULE_DIR} )
INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/install/SketchUp/Plugins/OpenStudio/Startup.rb" DESTINATION "${RUBY_MODULE_DIR}/Plugins/OpenStudio/" )
INSTALL( FILES "${CMAKE_CURRENT_BINARY_DIR}/install/SketchUp/Plugins/OpenStudio.rb" DESTINATION "${RUBY_MODULE_DIR}/Plugins/" )

# installation
IF (WIN32 OR APPLE)
  FOREACH(f ${RUBY_SRC})
    FILE( RELATIVE_PATH SRC_FILE "${CMAKE_SOURCE_DIR}/ruby/" ${f} )
    GET_FILENAME_COMPONENT(SRC_PATH ${SRC_FILE} PATH)
    #MESSAGE("install ${f} -> Ruby/${SRC_PATH}")
    INSTALL( FILES ${f} DESTINATION "Ruby/${SRC_PATH}" )
  ENDFOREACH(f)
ELSE() #Linux
  FOREACH(f ${RUBY_SRC})
    FILE( RELATIVE_PATH SRC_FILE "${CMAKE_SOURCE_DIR}/ruby/" ${f} )
    GET_FILENAME_COMPONENT(SRC_PATH ${SRC_FILE} PATH)
    #MESSAGE("install ${f} -> Ruby/${SRC_PATH}")
    INSTALL( FILES ${f} DESTINATION "${RUBY_MODULE_DIR}/${SRC_PATH}" )
    INSTALL( FILES ${f} DESTINATION "share/openstudio/Ruby/${SRC_PATH}" )
  ENDFOREACH(f)
ENDIF()

# install the openstudio resources (html, gif, etc.)
INSTALL( DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/sketchup_plugin"
        DESTINATION "${RUBY_MODULE_DIR}/openstudio"
        PATTERN "*.rb" EXCLUDE )
