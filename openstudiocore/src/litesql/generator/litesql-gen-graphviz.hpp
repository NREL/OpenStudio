#ifndef _litesql_gen_graphviz_hpp
#define _litesql_gen_graphviz_hpp

#include "generator.hpp"

namespace litesql {
  
  class GraphvizGenerator : public CodeGenerator {
  public:
    GraphvizGenerator(): CodeGenerator("graphviz") { };
    bool generateCode(const ObjectModel* model);
  protected:   
    bool generate(xml::Object* const object     , std::ostream& os , size_t indent);
    bool generate(xml::Relation* const relation , std::ostream& os , size_t indent);
  };
}

#endif
