
SET(xmllib_source
  QDomNodeModel
  QDomNodeModel.cpp
  XmlDoc.hpp
  XmlDoc.cpp
  XmlElement.hpp
  XmlElement.cpp
)

ADD_LIBRARY(xmllib STATIC ${xmllib_source})






