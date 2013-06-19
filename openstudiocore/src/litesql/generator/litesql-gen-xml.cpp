#include "litesql-gen-xml.hpp"

#include <fstream>

using namespace std;
using namespace xml;
using namespace litesql;

void XmlGenerator::setOutputFilename(const std::string& filename)
{
  m_outputFilename=filename;
}


bool generate(xml::Field* const field, ostream& os,size_t indent)
{
  string indent_string(indent,' ');
       
  os  << indent_string << "<field " 
      << attribute("name",field->name) 
      << attribute("type",toString(field->type)) 
      << (field->default_.empty() ? "":attribute("default",field->default_)) 
      << (field->isIndexed() ? attribute("indexed","true") : "") 
      << (field->isUnique() ? attribute("unique","true") : "") 
      ;
  if (field->values.empty())
  {
    os << "/>" <<endl;
  }
  else 
  {
    os << ">" <<endl;
    for (std::vector<xml::Value >::const_iterator it = field->values.begin();
      it != field->values.end();
      it++)
  {
    os << indent_string <<  ' ' << ' ' << "<value " 
      << attribute("name", it->name)
      << attribute("value", it->value)
      << "/>" <<endl;
    }
    os << indent_string << endtag("field") <<endl;

  }
  return true;
}

void generate(xml::Method* const pMethod,ostream& os,size_t indent)
{
  string indent_string(indent,' ');
  
  os << indent_string << "<method " << attribute("name",pMethod->name );
  
  if (pMethod->params.empty())
  {
    os << "/>" <<endl;
  }
  else
  {
    os << ">" <<endl;
    for (std::vector<xml::Param >::const_iterator it = pMethod->params.begin();
    it != pMethod->params.end();
    it++)
    {
      os << indent_string << "  " << "<param " << attribute("type",(*it).type) << attribute("name",(*it).name) << "/>" << endl;
    }
    os << endtag("method") <<endl;
    
  }
}

bool XmlGenerator::generate(xml::Object* const object,ostream& os,size_t indent)
{
  string indent_string(indent,' ');
  os << indent_string << "<object " 
    << attribute("name",object->name)
    << (object->inheritsFromDefault() ? "": attribute("inherits",object->inherits))
    << ">" <<endl;

  for (vector<xml::Field*>::const_iterator field_it = object->fields.begin();
    field_it != object->fields.end();
    field_it++)
  {
    if (((*field_it)->name!="id") && ((*field_it)->name!="type"))
    {
      ::generate(*field_it,os,indent+2);
    }
  }

  for (vector<xml::Method*>::const_iterator method_it = object->methods.begin();
    method_it  != object->methods.end();
    method_it++)
  {
    ::generate(*method_it,os,indent+2);
  }
  os << indent_string << endtag("object") <<endl;
  return true;
}

bool XmlGenerator::generate(Relation* const relation,ostream& os,size_t indent)
{
  string indent_string(indent,' ');
  os << indent_string << "<relation " 
    << attribute("name",relation->name)
    << attribute("id",relation->id)
    << (relation->isUnidir() ? attribute("unidir","true"):"");

  if (relation->related.empty())
  {
    os << "/>" ;
  }
  else
  {
    os << ">" << endl;
    for(vector<xml::Relate*>::const_iterator it = relation->related.begin();
      it != relation->related.end();
      it++)
    {
      os << indent_string << "  " 
        <<  "<relate " 
        <<  attribute("object",(*it)->objectName)
        <<  ((*it)->hasLimit() ? attribute("limit",toString((*it)->limit)):"")
        <<  attribute("handle",(*it)->handle)
        <<  ((*it)->isUnique() ? attribute("unique","true"):"")
        
        <<  "/>" 
        <<endl;
    
    }
    os << indent_string << endtag("relation");
  }

  os << endl;
  return true;
  /*<relation id="Mother" unidir="true">
        <relate object="Person" limit="many" handle="mother"/>
        <relate object="Person" limit="one"/>
    </relation>
    <relation id="Father" unidir="true">
        <relate object="Person" limit="many" handle="father"/>
        <relate object="Person" limit="one"/>
    </relation>
    <relation id="Siblings">
        <relate object="Person" handle="siblings"/>
        <relate object="Person"/>
    </relation>
    <relation id="Children" unidir="true">
        <relate object="Person" handle="children"/>
        <relate object="Person"/>
    </relation>
    <object name="Role"/>
    <object name="Student" inherits="Role"/>
    <object name="Employee" inherits="Role"/>
    <relation id="Roles" name="RoleRelation">
        <relate object="Person" handle="roles" limit="one"/>
        <relate object="Role" handle="person"/>
    </relation>
    */

}

bool XmlGenerator::generateDatabase(ostream& os,const ObjectModel* model)
{
  os << "<" << Database::TAG << " " 
     << attribute("name",model->db.name) 
     << attribute("namespace", model->db.nspace) 
     << ">" << endl;

  CodeGenerator::generate(model->objects,os,2);
  CodeGenerator::generate(model->relations,os,2);
  
  os << "</"<<  Database::TAG <<">" << endl;
  return true;
}

bool XmlGenerator::generateCode(const ObjectModel* model)
{
  bool success;
  ofstream ofs(getOutputFilename(m_outputFilename).c_str());
  ofs << "<?xml version=\"1.0\"?>" << endl
      << "<!DOCTYPE database SYSTEM \"litesql.dtd\">" << endl; 
  success = generateDatabase(ofs,model);

  ofs.close();
  return success;
}

