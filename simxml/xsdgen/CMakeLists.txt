
SET(xsdgen_source
  ComplexType.hpp
  ComplexType.cpp
  Element.hpp
  Element.cpp  
  SimpleType.hpp
  SimpleType.cpp
  SimXmlGenerator.hpp
  SimXmlGenerator.cpp
  ../xmllib/QDomNodeModel
  ../xmllib/QDomNodeModel.cpp
  XsdFile.hpp
  XsdFile.cpp
  xsdgen.cpp
)

ADD_EXECUTABLE(xsdgen ${xsdgen_source})
TARGET_LINK_LIBRARIES(xsdgen ${QT_LIBS} )





