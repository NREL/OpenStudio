SET( target_name GenerateIddFactory )

# source files
SET( ${target_name}_SRC
  main.cpp
  GenerateIddFactoryOutFiles.hpp
  GenerateIddFactoryOutFiles.cpp
  GenerateIddFactory.hpp
  GenerateIddFactory.cpp
  IddFileFactoryData.hpp
  IddFileFactoryData.cpp
  ../utilities/UtilitiesAPI.hpp
  ../utilities/core/Checksum.hpp
  ../utilities/core/Checksum.cpp
  ../utilities/core/Path.hpp
  ../utilities/core/Path.cpp
  ../utilities/core/GeneratorApplicationPathHelpers.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/../utilities/core/GeneratorApplicationPathHelpers.cxx
  ../utilities/core/String.hpp        # needed for Path to link properly
  ../utilities/core/String.cpp
  ../utilities/core/Serialization.hpp # needed for Path to link properly
  ../utilities/idd/IddRegex.hpp
  ../utilities/idd/IddRegex.cpp
)

ADD_EXECUTABLE( ${target_name}
  ${${target_name}_SRC}
)

SET_TARGET_PROPERTIES( ${target_name} PROPERTIES COMPILE_DEFINITIONS OPENSTUDIO_DIRECT_INCLUDE )

SET( depends
  ${Boost_LIBRARIES}
  ${QT_LIBS}
)

TARGET_LINK_LIBRARIES( ${target_name}  ${depends} )
