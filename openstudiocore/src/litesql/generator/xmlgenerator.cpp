#include "generator.hpp"
#include "objectmodel.hpp"
#include <fstream>

using namespace std;
using namespace xml;
using namespace litesql;

void XmlGenerator::setOutputFilename(const std::string& filename)
{
  m_outputFilename=filename;
}

string attribute(const string& name, const string& value)
{
  string a;
  a.append(name).append("=").append("\"").append(value).append("\" ");
  return a;
}

string endtag(const string& name)
{
  string a;
  a.append("</").append(name).append(">");
  return a;
}

bool generate(ostream& os,xml::Field* field, size_t indent=4)
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

void generate(ostream& os,xml::Method* pMethod,size_t indent=2)
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

bool generate(ostream& os,const std::vector<xml::Object* >& objects,size_t indent=2)
{
  string indent_string(indent,' ');
  for (std::vector<xml::Object* >::const_iterator it = objects.begin();
    it != objects.end();
    it++)
  {
    os << indent_string << "<object " 
       << attribute("name",(*it)->name)
       << ((*it)->inheritsFromDefault() ? "": attribute("inherits",(*it)->inherits))
       << ">" <<endl;
    
    for (vector<xml::Field*>::const_iterator field_it = (*it)->fields.begin();
          field_it != (*it)->fields.end();
          field_it++)
    {
      generate(os,*field_it,indent+2);
    }

    for (vector<xml::Method*>::const_iterator method_it = (*it)->methods.begin();
          method_it  != (*it)->methods.end();
          method_it++)
    {
      generate(os,*method_it,indent+2);
    }
    os << indent_string << endtag("object") <<endl;
  }
  return true;
}

void generate(ostream& os,Relation* relation,size_t indent=4)
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

bool generateDatabase(ostream& os,const ObjectModel* model)
{
  os << "<" << Database::TAG << " " 
     << attribute("name",model->db.name) 
     << attribute("namespace", model->db.nspace) 
     << ">" << endl;

  generate(os,model->objects);
  
  for ( vector<xml::Relation*>::const_iterator it = model->relations.begin();
    it != model->relations.end();
    it++)
  {
    generate(os,*it,2);
  }
  
  os << "</"<<  Database::TAG <<">" << endl;
  return true;
}

bool XmlGenerator::generateCode(const ObjectModel* model)
{
  bool success;
  ofstream ofs(m_outputFilename.c_str());
  ofs << "<?xml version=\"1.0\"?>" << endl
      << "<!DOCTYPE database SYSTEM \"litesql.dtd\">" << endl; 
  success = generateDatabase(ofs,model);

  ofs.close();
  return success;
}

