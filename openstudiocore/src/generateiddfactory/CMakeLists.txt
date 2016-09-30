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

set(depends
  ${Boost_LIBRARIES}
)

target_link_libraries(${target_name} ${depends})
