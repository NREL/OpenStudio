set(target_name GenerateIddFactory)

# source files
set(${target_name}_SRC
  main.cpp
  GenerateIddFactoryOutFiles.hpp
  GenerateIddFactoryOutFiles.cpp
  GenerateIddFactory.hpp
  GenerateIddFactory.cpp
  GeneratorApplicationPathHelpers.hpp
  GeneratorApplicationPathHelpers.cpp
  IddFileFactoryData.hpp
  IddFileFactoryData.cpp
  ../utilities/UtilitiesAPI.hpp
  ../utilities/core/Checksum.hpp
  ../utilities/core/Checksum.cpp
  ../utilities/idd/IddRegex.hpp
  ../utilities/idd/IddRegex.cpp
)

add_executable(${target_name}
  ${${target_name}_SRC}
)

set_target_properties(${target_name} PROPERTIES COMPILE_DEFINITIONS OPENSTUDIO_DIRECT_INCLUDE)

target_link_libraries(${target_name}
  boost::boost
  fmt::fmt
  #Boost::filesystem
  #Boost::regex
  #Boost::program_options
  #boost::boost_crc - not sure where that went
)

# generate the IddFactory files
add_custom_command(
  OUTPUT
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFactoryFileIndex.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddEnums.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFieldEnums.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFactory.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFactory.cxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFactory_EnergyPlus.cxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFactory_OpenStudio.cxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFieldEnums.ixx"
  COMMAND GenerateIddFactory "--outdir=${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd" "EnergyPlus,${PROJECT_SOURCE_DIR}/resources/energyplus/ProposedEnergy+.idd" "${PROJECT_SOURCE_DIR}/resources/model/OpenStudio.idd"
  DEPENDS
  GenerateIddFactory
  "${PROJECT_SOURCE_DIR}/resources/energyplus/ProposedEnergy+.idd"
  "${PROJECT_SOURCE_DIR}/resources/model/OpenStudio.idd"
)

add_custom_target("GenerateIddFactoryRun"
  DEPENDS
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFactoryFileIndex.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddEnums.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFieldEnums.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFactory.hxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFactory.cxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFactory_EnergyPlus.cxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFactory_OpenStudio.cxx"
  "${CMAKE_CURRENT_BINARY_DIR}/../utilities/idd/IddFieldEnums.ixx"
)
