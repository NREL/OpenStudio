#ifndef generator_hpp
#define generator_hpp

#include <vector>
#include <ostream>

#include "objectmodel.hpp"


namespace litesql {
  class CodeGenerator {
  public:
    enum generation_mode_t { REFRESH=0,OVERWRITE };
    
    virtual ~CodeGenerator();
    
    virtual void setOutputDirectory(const std::string& directory);
    virtual const std::string& getOutputDirectory() const;
    
    void setGenerationMode(generation_mode_t mode) { m_generationMode = mode; };
    generation_mode_t getGenerationMode() const { return m_generationMode; };
    
    std::string getOutputFilename(const std::string& name) const;
    
    virtual const char* getTarget() const;
    virtual bool generateCode(const ObjectModel* model)=0;
    
    CodeGenerator* const getParentGenerator() const { return m_pParentGenerator; };
    void setParentGenerator(CodeGenerator* parent)  { m_pParentGenerator=parent; };
    
    bool generate(const std::vector<xml::Object* >& objects);
    bool generate(const std::vector<xml::Relation* >& relations);
    virtual bool generate(xml::Object* const object) {return true;};
    virtual bool generate(xml::Relation* const relation){return true;};
    
    bool generate(const std::vector<xml::Object* >& objects,std::ostream& os,size_t indent=2);
    bool generate(const std::vector<xml::Relation* >& relations,std::ostream& os,size_t indent=2);
    //virtual void generate(std::ostream& os,const ObjectModel* model,size_t indent=0);
    
    virtual bool generate(xml::Object* const object    ,std::ostream& os, size_t indent=2) {return true;};
    //virtual void generate(std::ostream& os,xml::Field* field     , size_t indent=4){};
    //virtual void generate(std::ostream& os,xml::Method* pMethod  , size_t indent=4){};
    
    virtual bool generate(xml::Relation* const relation,std::ostream& os,size_t indent=4){return true;};
    
    //  static CodeGenerator* create(const char* target);
    
  protected:
    CodeGenerator(const char* target)
    : m_target(target),m_generationMode(REFRESH) {};
    
  private:
    const char* m_target;
    generation_mode_t m_generationMode;
    std::string m_directory;
    CodeGenerator* m_pParentGenerator;
  };
  
  class CompositeGenerator : public CodeGenerator {
  public:
    CompositeGenerator(): CodeGenerator("composite") { };
    CompositeGenerator(const char* target) : CodeGenerator(target) {};
        
    void setOutputDirectory(const std::string& directory);
    const std::string& getOutputDirectory() const;
    void add(CodeGenerator* g);
    bool generateCode(const ObjectModel* model);
  private:
    std::vector<CodeGenerator*> generators;
  };
  
}

#endif //#ifndef generator_hpp

