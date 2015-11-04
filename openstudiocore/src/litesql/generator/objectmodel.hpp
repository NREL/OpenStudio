#ifndef objectmodel_hpp
#define objectmodel_hpp

#include <string>
#include <vector>

#include "xmlobjects.hpp"

namespace litesql {
/** the litesql object model */ 
class ObjectModel {
public:
   ~ObjectModel() {};
   /** load model from xml-file (uses a  specialized XmlParser) */
   bool loadFromFile(const std::string& filename);

   xml::Database db;   

   std::vector<xml::Object* > objects;
   std::vector<xml::Relation* > relations;
};
}
#endif //#ifndef objectmodel_hpp

