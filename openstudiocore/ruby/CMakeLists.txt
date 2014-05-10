# currently we are using only the ResultsViewer resources for Ruby tests

# which directory will we run ruby tests from
set(RUBY_RUN_DIR "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")

# get all possible configurations
list(LENGTH CMAKE_CONFIGURATION_TYPES POSSIBLE_CONFIGURATIONS_LENGTH)
if(CMAKE_CONFIGURATION_TYPES)
  set(POSSIBLE_CONFIGURATIONS ${CMAKE_CONFIGURATION_TYPES})
else()
  set(POSSIBLE_CONFIGURATIONS ".")
endif()

include(GetPrerequisites)

# find all source
file(GLOB_RECURSE RUBY_SRC "openstudio/*.rb" "cloud/*.template" "cloud/*.sh")

# find all tests
file(GLOB_RECURSE RUBY_TEST_SRC "openstudio/*_Test.rb")

# add a test for each unit test
set(RUBY_TEST_REQUIRES "#include test files")
foreach(f ${RUBY_TEST_SRC})

  file(RELATIVE_PATH REQUIRE_NAME "${CMAKE_SOURCE_DIR}/ruby/" ${f})
  set(RUBY_TEST_REQUIRES "${RUBY_TEST_REQUIRES} \nrequire '${REQUIRE_NAME}'")

  file(READ "${f}" CONTENTS)
  string(REGEX MATCHALL "def +test_([A-Za-z_0-9 ]+)" FOUND_TESTS ${CONTENTS})

  foreach(HIT ${FOUND_TESTS})
    string(REGEX REPLACE "def +test_([A-Za-z_0-9]+)" "\\1" TEST_NAME ${HIT})
    string(REGEX MATCH "/?([A-Za-z_0-9 ]+)\\.rb" FILE_NAME ${f})
    string(REGEX REPLACE "/?([A-Za-z_0-9 ]+)\\.rb" "\\1" FILE_NAME ${FILE_NAME})
    if(BUILD_TESTING)
      if(MSVC)
        add_test(NAME "RubyTest-${FILE_NAME}-${TEST_NAME}"
          COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIGURATION>"
          "$<TARGET_FILE:ruby_cpp>" -I "${RUBY_RUBY_LIB_DIR}" -I "${RUBY_RUBY_LIB_DIR}/i386-mingw32" -I. "openstudio_test.rb" "--name=test_${TEST_NAME}"
        )
      else()
        # $<CONFIGURATION> may resolve to 'Release' on Mac or Linux but directory will not exist
        add_test(NAME "RubyTest-${FILE_NAME}-${TEST_NAME}"
          COMMAND "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_BINARY_DIR}"
          "${RUBY_EXECUTABLE}" -I"${RUBY_RUBY_LIB_DIR}" -I. "openstudio_test.rb" "--name=test_${TEST_NAME}"
        )
      endif()
      set_tests_properties("RubyTest-${FILE_NAME}-${TEST_NAME}" PROPERTIES DEPENDS "${ALL_RUBY_BINDING_TARGETS}" ruby_cpp ResultsViewer_resources)
    endif()
  endforeach()
endforeach()

# configure ruby files for installer
file(COPY "${CMAKE_SOURCE_DIR}/ruby/openstudio.rb" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/install")
file(COPY "${CMAKE_SOURCE_DIR}/ruby/Win32API.so" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/install")
configure_file("${CMAKE_SOURCE_DIR}/ruby/openstudio_test.in" "${CMAKE_CURRENT_BINARY_DIR}/install/openstudio_test.rb")
configure_file("${CMAKE_SOURCE_DIR}/ruby/openstudio/sketchup_plugin/Startup.in" "${CMAKE_CURRENT_BINARY_DIR}/install/SketchUp/Plugins/OpenStudio/Startup.rb")
file(COPY "${CMAKE_SOURCE_DIR}/ruby/openstudio/sketchup_plugin/OpenStudio.rb" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/install/SketchUp/Plugins")

configure_file("${CMAKE_SOURCE_DIR}/ruby/cloud/aws.rb.in" "${CMAKE_CURRENT_BINARY_DIR}/cloud/aws.rb")
if(WIN32 OR APPLE)
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cloud/aws.rb" DESTINATION "Ruby/cloud")
else() #Linux
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cloud/aws.rb" DESTINATION "${RUBY_MODULE_DIR}/cloud")
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cloud/aws.rb" DESTINATION "share/openstudio-${OPENSTUDIO_VERSION}/Ruby/cloud")
endif()
file(COPY "${CMAKE_SOURCE_DIR}/ruby/cloud/mongoid.yml.template" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/cloud")
file(COPY "${CMAKE_SOURCE_DIR}/ruby/cloud/server_script.sh" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/cloud")
file(COPY "${CMAKE_SOURCE_DIR}/ruby/cloud/worker_script.sh.template" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/cloud")

# install qt plugins and fixup for apple

if(WIN32 OR APPLE)
  get_target_property(QT_QSQLITE_PLUGIN Qt5::QSQLiteDriverPlugin LOCATION)

  install(PROGRAMS "${QT_QSQLITE_PLUGIN}"
          DESTINATION "Ruby/openstudio/sqldrivers/"
  )

  get_filename_component(QSQLITE_PLUGIN_NAME "${QT_QSQLITE_PLUGIN}" NAME)

  if(WIN32)
    get_target_property(QT_WINDOWS_PLUGIN Qt5::QWindowsIntegrationPlugin LOCATION)

    install(PROGRAMS "${QT_WINDOWS_PLUGIN}"
            DESTINATION "Ruby/openstudio/platforms/" 
    )
  endif()

  if(APPLE)
    set(QTPLUGIN "\${CMAKE_INSTALL_PREFIX}/Ruby/openstudio/sqldrivers/${QSQLITE_PLUGIN_NAME}")

    set(Prereq_Dirs)

    install(CODE "
      include(GetPrerequisites)
      get_prerequisites(\"${QTPLUGIN}\" PREREQUISITES 1 0 \"\" \"${Prereq_Dirs}\")

      foreach(PREREQ IN LISTS PREREQUISITES)
        gp_resolve_item(\"\" \${PREREQ} \"\" \"${LIBRARY_SEARCH_DIRECTORY}\" resolved_item_var)

        get_filename_component(PREREQNAME \${resolved_item_var} NAME)

        execute_process(COMMAND \"install_name_tool\" -change \"\${PREREQ}\" \"@loader_path/../\${PREREQNAME}\" \"${QTPLUGIN}\")
      endforeach()
    ")
  endif()
endif()

# optionally build ruby gem
if(BUILD_RUBY_GEM)
  set(GEMDIR "${CMAKE_BINARY_DIR}/Products/RubyGem")

  install(CODE "
    execute_process(COMMAND \"${CMAKE_COMMAND}\" -E remove_directory \"${GEMDIR}/lib\")
    execute_process(COMMAND \"${CMAKE_COMMAND}\" -E make_directory \"${GEMDIR}/lib\")
    execute_process(COMMAND \"${CMAKE_COMMAND}\" -E remove_directory \"${GEMDIR}/share\")
    execute_process(COMMAND \"${CMAKE_COMMAND}\" -E make_directory \"${GEMDIR}/share/openstudio-${OPENSTUDIO_VERSION}\")
    execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy \"${CMAKE_CURRENT_SOURCE_DIR}/openstudio.rb\" \"${GEMDIR}/lib\")
  ")

  # Find libs that need to be copied for generating ruby gem
  if(BUILD_RUBY_GEM_WITH_LIBS)
    install(CODE "
      set(qtinstalled false)
      set(SEARCH_PATHS \"/usr/local/lib;\${CMAKE_INSTALL_PREFIX}/lib;\${CMAKE_INSTALL_PREFIX}/bin;${Boost_LIBRARY_DIRS};${QT_LIBRARY_DIR}\")
      file(GLOB ruby_libs \"\${CMAKE_INSTALL_PREFIX}/${RUBY_MODULE_ARCH_DIR}/*\")
      include(GetPrerequisites)
      foreach(lib IN LISTS ruby_libs)
        get_prerequisites(\"\${lib}\" prereqs 1 1 \"\" \"\${SEARCH_PATHS}\" )
        execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy_if_different \"\${lib}\" \"${GEMDIR}/lib\")
        foreach(prereq IN LISTS prereqs)
          string(TOLOWER \"\${prereq}\" lowerprereq)
          string(REGEX MATCH \".*qtcore.*\" regexmatch \"\${lowerprereq}\")
          if(regexmatch)
            set(qtinstalled true)
          endif()
          gp_resolve_item(\"\" \${prereq} \"\" \"\${SEARCH_PATHS}\" resolved_item_var)
          execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy_if_different \"\${resolved_item_var}\" \"${GEMDIR}/lib\")
          get_filename_component(PREREQNAME \${resolved_item_var} NAME)
        endforeach()
      endforeach()

      execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy_directory \"\${CMAKE_INSTALL_PREFIX}/share/openstudio-${OPENSTUDIO_VERSION}/osversion\" \"${GEMDIR}/share/openstudio/osversion\")

      if(qtinstalled)
        execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy_directory \"${QT_PLUGINS_DIR}\" \"${GEMDIR}/lib/plugins\")
      endif()

    ")
  endif()

  configure_file("${CMAKE_SOURCE_DIR}/ruby/openstudio.gemspec.in" "${CMAKE_BINARY_DIR}/Products/RubyGem/openstudio.gemspec")

  install(CODE "
    if(WIN32)
      execute_process(COMMAND cmd.exe /C \"${GEM} build ${CMAKE_BINARY_DIR}/Products/RubyGem/openstudio.gemspec\"
                      WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}/Products/RubyGem/\")
    else()
      execute_process(COMMAND \"${GEM}\" build \"${CMAKE_BINARY_DIR}/Products/RubyGem/openstudio.gemspec\"
                      WORKING_DIRECTORY \"${CMAKE_BINARY_DIR}/Products/RubyGem/\")
    endif()

    execute_process(COMMAND \"${CMAKE_COMMAND}\" -E copy_if_different \"${CMAKE_BINARY_DIR}/Products/RubyGem/openstudio-${CMAKE_VERSION_MAJOR}.${CMAKE_VERSION_MINOR}.${CMAKE_VERSION_PATCH}.gem\" \"${CMAKE_BINARY_DIR}/../../../\")
  ")
endif()


# configure ruby files for testing from build directory
foreach(POSSIBLE_CONFIGURATION ${POSSIBLE_CONFIGURATIONS})
  file(COPY "${CMAKE_SOURCE_DIR}/ruby/openstudio_dev.rb" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}")
  file(RENAME "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/openstudio_dev.rb" "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/openstudio.rb")

  configure_file("${CMAKE_SOURCE_DIR}/ruby/config.in" "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/config.rb")
  configure_file("${CMAKE_SOURCE_DIR}/ruby/openstudio_test.in" "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/openstudio_test.rb")

  configure_file("${CMAKE_SOURCE_DIR}/ruby/openstudio/sketchup_plugin/Startup_dev.in" "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/SketchUp/Plugins/OpenStudio/Startup.rb")
  file(COPY "${CMAKE_SOURCE_DIR}/ruby/openstudio/sketchup_plugin/OpenStudio.rb" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${POSSIBLE_CONFIGURATION}/SketchUp/Plugins")
  
  if(WIN32 AND ${BUILD_WITH_OPENSSL} AND ${OPENSSL_FOUND})
    file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/Products/ruby/${POSSIBLE_CONFIGURATION}")
    file(COPY "${LIBEAY32_PATH}" "${SSLEAY32_PATH}" DESTINATION "${CMAKE_BINARY_DIR}/Products/ruby/${POSSIBLE_CONFIGURATION}")
  endif()

endforeach()

# run rdoc for openstudio
if(BUILD_DOCUMENTATION)
  add_custom_target(ruby_OpenStudioSketchUpPlugin_rdoc
    ${CMAKE_COMMAND} -E chdir "${RUBY_RUN_DIR}" "${RUBY_EXECUTABLE}" "${CMAKE_SOURCE_DIR}/../developer/ruby/OpenStudioSketchUpPluginRDoc.rb" "${RUBY_RUN_DIR}/"
  )

  list(APPEND ALL_RDOC_TARGETS ruby_OpenStudioSketchUpPlugin_rdoc)
  set(ALL_RDOC_TARGETS "${ALL_RDOC_TARGETS}" PARENT_SCOPE)

endif()

# some unit tests previously defined depend on the ruby bindings, set those here so they are all defined
if(BUILD_TESTING)
  add_dependencies(openstudio_runmanager_tests ${ALL_RUBY_BINDING_TARGETS})
  add_dependencies(openstudio_analysisdriver_tests ${ALL_RUBY_BINDING_TARGETS})
endif()

# run all the unit tests
if(ENABLE_TEST_RUNNER_TARGETS)
  if(MSVC)
    add_custom_target(ruby_run_tests
      ${CMAKE_COMMAND} -E chdir "${RUBY_RUN_DIR}" "${RUBY_EXECUTABLE}" -I "${RUBY_RUBY_LIB_DIR}" -I "${RUBY_RUBY_LIB_DIR}/i386-mingw32" -I. "openstudio_test.rb"
      DEPENDS ${ALL_RUBY_BINDING_TARGETS} ruby_cpp ResultsViewer_resources openstudio_energyplus_resources
    )  
  else()
    add_custom_target(ruby_run_tests
      ${CMAKE_COMMAND} -E chdir "${RUBY_RUN_DIR}" "${RUBY_EXECUTABLE}" -I "${RUBY_RUBY_LIB_DIR}" -I. "openstudio_test.rb"
      DEPENDS ${ALL_RUBY_BINDING_TARGETS} ruby_cpp ResultsViewer_resources openstudio_energyplus_resources
    )  
  endif()
endif()

# install the files
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/install/openstudio.rb" DESTINATION ${RUBY_MODULE_DIR})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/install/Win32API.so" DESTINATION ${RUBY_MODULE_DIR})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/install/openstudio_test.rb" DESTINATION ${RUBY_MODULE_DIR})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/install/SketchUp/Plugins/OpenStudio/Startup.rb" DESTINATION "${RUBY_MODULE_DIR}/Plugins/OpenStudio/")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/install/SketchUp/Plugins/OpenStudio.rb" DESTINATION "${RUBY_MODULE_DIR}/Plugins/")

# installation
if(WIN32 OR APPLE)
  foreach(f ${RUBY_SRC})
    file(RELATIVE_PATH SRC_FILE "${CMAKE_SOURCE_DIR}/ruby/" ${f})
    get_filename_component(SRC_PATH ${SRC_FILE} PATH)
    #message("install ${f} -> Ruby/${SRC_PATH}")
    install(FILES ${f} DESTINATION "Ruby/${SRC_PATH}")
  endforeach()
else() #Linux
  foreach(f ${RUBY_SRC})
    file(RELATIVE_PATH SRC_FILE "${CMAKE_SOURCE_DIR}/ruby/" ${f})
    get_filename_component(SRC_PATH ${SRC_FILE} PATH)
    #message("install ${f} -> Ruby/${SRC_PATH}")
    install(FILES ${f} DESTINATION "${RUBY_MODULE_DIR}/${SRC_PATH}")
    install(FILES ${f} DESTINATION "share/openstudio-${OPENSTUDIO_VERSION}/Ruby/${SRC_PATH}")
  endforeach()
endif()

# install the openstudio resources (html, gif, etc.)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/openstudio/sketchup_plugin"
        DESTINATION "${RUBY_MODULE_DIR}/openstudio"
        PATTERN "*.rb" EXCLUDE
)
