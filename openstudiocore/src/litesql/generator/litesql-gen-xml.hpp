#ifndef xmlgenerator_hpp
#define xmlgenerator_hpp

#include <ostream>
#include "generator.hpp"

namespace litesql {
  class XmlGenerator : public CodeGenerator {
  public:
    XmlGenerator(): CodeGenerator("xml") { };
    virtual void setOutputFilename(const std::string& filename);
    bool generateCode(const ObjectModel* model);
    
  protected:
    bool generate(xml::Object *const object     , std::ostream& os , size_t indent=2);
    bool generate(xml::Relation* const relation , std::ostream& os , size_t indent=4);

  private:
    bool generateDatabase(std::ostream& os,const ObjectModel* model);
    std::string m_outputFilename;
  };
}

#endif //#ifndef xmlgenerator_hpp

