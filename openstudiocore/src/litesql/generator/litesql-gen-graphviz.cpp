#include "litesql-gen-graphviz.hpp"
#include <fstream>

using namespace std;
using namespace xml;

bool GraphvizGenerator::generate(Object* const object    ,ostream& os, size_t indent)
{
  string indents(indent,' ');
  os << indents << '"' << object->name << '"';
  
  if (object->parentObject)
    os << " -> \"" << object->inherits << "\"";
  
  os << ';' << endl;
  return true;
}

bool GraphvizGenerator::generate(Relation* const relation,ostream& os,size_t indent)
{
  string indents(indent,' ');
  Relation& r = *relation;
  for (size_t i2 = 0; i2 < r.related.size(); i2++) {
    Relate& rel = *r.related[i2];
    string extra;
    for (size_t i3 = 0; i3 < r.related.size(); i3++) {
      if (i3 == i2) 
        continue;

      Relate& destRel = *r.related[i3];
      if (rel.handle.size() > 0) {
        extra = " [label=\"" + rel.handle + "\"]";
        os << indents << "\"" << rel.objectName << "\" -> \""<< destRel.objectName <<"\""<< " [label=\"" << rel.handle << "\"]" <<";" <<endl; 
      }
    }
  }
  return true;
}

bool GraphvizGenerator::generateCode(const ObjectModel* model)
{

  string fname = getOutputFilename(toLower(model->db.name + ".dot"));

  ofstream os(fname.c_str());
  os << "digraph database {" << endl
     << "  node[shape=box,color=black];" << endl
     << "  subgraph inheritance {" << endl
     << "    edge[style=dashed,dir=forward,arrowhead=normal];" << endl;
    
  CodeGenerator::generate(model->objects,os,4);

  os << "  }" << endl
     << "  subgraph relations {" << endl
     << "    edge[dir=forward,arrowhead=vee];" << endl;
  
  CodeGenerator::generate(model->relations,os,4);
  
  os << "  }" << endl
     << "}" << endl;

  os.close();
  return true;
}


