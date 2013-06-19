#include "litesql-gen-ruby-activerecord.hpp"
#include <fstream>

using namespace std;
using namespace xml;

string toActiveRecordType(AT_field_type field_type) {
  switch(field_type) {
       case A_field_type_integer:  return "int";
       case A_field_type_float:    return "float";
       case A_field_type_boolean:  return "bool";
       case A_field_type_date:     return "date";
       case A_field_type_time:     return "timestamp";
       case A_field_type_datetime: return "datetime";
       case A_field_type_blob:     return "blob";
       case A_field_type_string:   return "string";
       default:
         return "unknown";
  }
}

RubyActiveRecordGenerator::RubyActiveRecordGenerator(): CompositeGenerator("ruby-activerecord") 
{ 
  add(new ActiveRecordClassGenerator());
  add(new RubyMigrationsGenerator());
}

bool ActiveRecordClassGenerator::generate(xml::Object* const object)
{
  string fname = getOutputFilename(toLower(object->name + ".rb"));

  ofstream os(fname.c_str());
  
  
  string baseClass = object->parentObject ? object->inherits : "ActiveRecord::Base"; 
  os << "class " << object->name << " < " << baseClass << endl;
  
  for (vector<RelationHandle*>::const_iterator it = object->handles.begin(); it!= object->handles.end(); it++) {
    os  << ((*it)->relate->hasLimit() ? "has_one" : "has_many") 
        << " :" << (*it)->name;
    
    if (!(*it)->name.empty())
      os << ", :through => :" << (*it)->name;
      
      
    os  << endl;
    
  }
  
  os << "end" << endl;
  os.close();
  return true;
}

bool ActiveRecordClassGenerator::generateCode(const ObjectModel* model)
{
  CodeGenerator::generate(model->objects);
  return true;
}

void generateSelfUp(const ObjectModel* model,ostream& os)
{
  string indent("  ");
  os << "def self.up" << endl;
  for (vector<Object*>::const_iterator it = model->objects.begin(); it !=model->objects.end();it++)
  {
    os << indent << "create_table :" << (*it)->getTable() <<  " do |t|" << endl;
  
    for (vector<Field*>::const_iterator fit = (*it)->fields.begin(); fit !=(*it)->fields.end();fit++)
    {
      os  << indent << indent 
          << "t." << toActiveRecordType((*fit)->type) << " :" << (*fit)->name     
          << endl;
    }
    os  << endl;
    
    os  << indent << indent << "t.timestamps" << endl;
    os  << indent << "end" << endl
        << endl;


  }
  os << "end" << endl;
}

void generateSelfDown(const ObjectModel* model,ostream& os)
{
  string indent("  ");
  os << "def self.down" << endl;
  for (vector<Object*>::const_iterator it = model->objects.begin(); it !=model->objects.end();it++)
  {
    os << indent << "drop_table :" << (*it)->getTable() << endl;
  
  }
  os << "end" << endl;
}

bool RubyMigrationsGenerator::generateCode(const ObjectModel* model)
{
  ofstream os(getOutputFilename(toLower(model->db.name + "_migration.rb")).c_str());
  
  os << "class " << "Create" << model->db.name << "  < ActiveRecord::Migration" << endl;
  os << endl;
  generateSelfUp(model,os);
  os << endl;
  generateSelfDown(model,os);
  os << endl;
  os << "end" << endl;

  os.close();
  return true;
}

