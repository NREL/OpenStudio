#ifndef litesql_gen_cpp_hpp
#define litesql_gen_cpp_hpp

#include "generator.hpp"

namespace litesql {
  
  class CppGenerator : public CodeGenerator {
  public:
    CppGenerator(): CodeGenerator("c++") { };
    bool generateCode(const ObjectModel* model);
    
    void setOutputSourcesDirectory(const std::string& directory);
    void setOutputIncludesDirectory(const std::string& directory);
    
    const std::string& getOutputSourcesDirectory() const;
    const std::string& getOutputIncludesDirectory() const;
    
    std::string getOutputSourcesFilename(const std::string& name) const;
    std::string getOutputIncludesFilename(const std::string& name) const;
    
  private:
    std::string m_sources_directory;
    std::string m_includes_directory;
    
    void writeCPPClasses(const ObjectModel* model);
    FILE *hpp, *cpp;
  };
  
}
#endif
