SET( target_name GenerateDataDictionary )

# source files
SET( ${target_name}_SRC
  main.cpp
  GenerateDataDictionary.hpp
  GenerateDataDictionary.cpp
  ../utilities/UtilitiesAPI.hpp
  ../utilities/core/Path.hpp
  ../utilities/core/Path.cpp
  ../utilities/core/GeneratorApplicationPathHelpers.hpp
  ${CMAKE_CURRENT_BINARY_DIR}/../utilities/core/GeneratorApplicationPathHelpers.cxx
  ../utilities/core/String.hpp        # needed for Path to link properly
  ../utilities/core/String.cpp
  ../utilities/core/Serialization.hpp # needed for Path to link properly
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
