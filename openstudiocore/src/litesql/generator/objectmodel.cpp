// include LiteSQL's header file and generated header file
#include "xmlparser.hpp"
#include "objectmodel.hpp"
#include <string.h>
#include "logger.hpp"

using namespace std;
using namespace xml;

#define xmlStrcasecmp(s1,s2)  ((s1==NULL) ? (s2!=NULL) : strcmp(s1,s2))
#define xmlStrEqual(s1,s2)   (!strcmp(s1,s2))

static const char* toString(AT_field_type t)
{
  switch (t) 
  {
  case A_field_type_boolean:
    return "boolean";
  case A_field_type_integer:
    return "integer";
  case A_field_type_string:
    return "string";
  case A_field_type_float: 
    return "float";
  case A_field_type_time:
    return "time";
  case A_field_type_date:
    return "date";
  case A_field_type_datetime:
    return "datetime";
  case A_field_type_blob: return "blob";

  default: 
    return "unknown";   
  }
}

static const char* toString(AT_relate_limit t)
{
  switch (t) 
  {
  case A_relate_limit_one:
    return "one";
  case A_relate_limit_many:
    return "many";
  case AU_relate_limit:
  default:
    return "unknown";   
  }
}

static AT_field_type field_type(const XML_Char* value)
{
  AT_field_type t;

  if (!xmlStrcasecmp(value,(XML_Char*)"boolean"))
  {
    t = A_field_type_boolean;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"integer"))
  {
    t = A_field_type_integer;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"string"))
  {
    t = A_field_type_string;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"float"))
  {
    t = A_field_type_float;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"time"))
  {
    t = A_field_type_time;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"date"))
  {
    t = A_field_type_date;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"datetime"))
  {
    t = A_field_type_datetime;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"blob"))
  {
    t = A_field_type_blob;
  }
  else
  {
    t = AU_field_type;   
  }
  return t;
}

static AT_field_unique field_unique(const XML_Char* value)
{
  AT_field_unique t;
  if (!xmlStrcasecmp(value,(XML_Char*)"true"))
  {
    t = A_field_unique_true;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"false"))
  {
    t = A_field_unique_false;
  }
  else
  {
    t = AU_field_unique;
  }
  return t;
}

static AT_index_unique index_unique(const XML_Char* value)
{
  AT_index_unique t;
  if (!xmlStrcasecmp(value,(XML_Char*)"true"))
  {
    t = A_index_unique_true;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"false"))
  {
    t = A_index_unique_false;
  }
  else
  {
    t = AU_index_unique;
  }
  return t;
}


static AT_field_indexed field_indexed(const XML_Char* value)
{
  AT_field_indexed t;
  if (!xmlStrcasecmp(value,(XML_Char*)"true"))
  {
    t = A_field_indexed_true;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"false"))
  {
    t = A_field_indexed_false;
  }
  else
  {
    t = AU_field_indexed;
  }
  return t;
}

static AT_relation_unidir relation_unidir(const XML_Char* value)
{
  AT_relation_unidir t;
  if (!xmlStrcasecmp(value,(XML_Char*)"true"))
  {
    t = A_relation_unidir_true;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"false"))
  {
    t = A_relation_unidir_false;
  }
  else
  {
    t = AU_relation_unidir;
  }
  return t;
}

static AT_relate_unique relate_unique(const XML_Char* value)
{
  AT_relate_unique t;
  if (!xmlStrcasecmp(value,(XML_Char*)"true"))
  {
    t = A_relate_unique_true;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"false"))
  {
    t = A_relate_unique_false;
  }
  else
  {
    t = AU_relate_unique;
  }
  return t;
}

static AT_relate_limit relate_limit(const XML_Char* value)
{
  AT_relate_limit t;
  if (!xmlStrcasecmp(value,(XML_Char*)"one"))
  {
    t = A_relate_limit_one;
  }
  else if (!xmlStrcasecmp(value,(XML_Char*)"many"))
  {
    t = A_relate_limit_many;
  }
  else
  {
    t = A_relate_limit_many;
  }
  return t;

}

namespace xml {
class LitesqlParser : public XmlParser {
public:
  LitesqlParser(ObjectModel* model)
    : m_pObjectModel(model),
      m_parseState(ROOT) {};

protected:
  void onStartElement(const XML_Char *fullname,
    const XML_Char **atts);

  void onEndElement(const XML_Char *fullname);
  /** ROOT->DATABASE; 
  *
  *    DATABASE->OBJECT;
  *      OBJECT->FIELD;
  *      OBJECT->METHOD;
  *      FIELD->OBJECT;
  *      METHOD->OBJECT;
  *
  *    DATABASE->RELATION;
  *    RELATION->DATABASE;
  *
  *  DATABASE->ROOT;
  * ERROR;
  */
  enum ParseState { ROOT,
    DATABASE,
    OBJECT,
    FIELD,
    METHOD,
    RELATION,
    INDEX,
    INDEXFIELD,
    INCLUDE,
    UNKNOWN,
    ERROR
  };

private:
  ObjectModel* m_pObjectModel;

  Object * obj;
  Relation * rel;
  Field * fld;
  Field * rel_fld;
  Method * mtd;
  Index * idx;
  IndexField* idxField;

  ParseState m_parseState;
  vector<ParseState> history;
};

}

void LitesqlParser::onStartElement(const XML_Char *fullname,
                                   const XML_Char **atts)
{
  //   Logger::report("starting " <<fullname );
  history.push_back(m_parseState);

  if (xmlStrEqual(fullname,(XML_Char*) Database::TAG ))
  {
    if (m_parseState!=ROOT)
    {
      m_parseState = ERROR;
    }
    else
    {
      m_parseState = DATABASE;
      m_pObjectModel->db.name = safe((char*)xmlGetAttrValue(atts,"name"));
      m_pObjectModel->db.include = safe((char*)xmlGetAttrValue(atts,"include"));
      m_pObjectModel->db.nspace = safe((char*)xmlGetAttrValue(atts,"namespace"));
      Logger::report("database = " + m_pObjectModel->db.name);
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Object::TAG))
  {
    if (m_parseState!=DATABASE)
    {
      m_parseState = ERROR;
    }
    else
    {
      m_pObjectModel->objects.push_back(obj = new Object(    (char*)xmlGetAttrValue(atts,"name"), 
        safe((char*)xmlGetAttrValue(atts,"inherits"))));
      Logger::report("object = " + obj->name);
      m_parseState = OBJECT; 

    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Field::TAG))
  {
    Field* pNewField = new Field(   (char*)xmlGetAttrValue(atts,"name"), 
                                    field_type(xmlGetAttrValue(atts,"type")),
                                    safe(  (char*)xmlGetAttrValue(atts,"default")),
                                    field_indexed(xmlGetAttrValue(atts,"indexed")),
                                    field_unique(xmlGetAttrValue(atts,"unique"))
      );

    switch(m_parseState)
    {
    case OBJECT:
      if (!obj) {
        Logger::error("parsing field inside object, but currentObject == NULL ");
      }
      else {
        Logger::report("field = ",(char*)xmlGetAttrValue(atts,"name") );
        obj->fields.push_back(fld = pNewField);
      };
      m_parseState = FIELD;
      break;

    case RELATION:
      if (!rel) {
        Logger::error("parsing field inside realtion, but currentRelation == NULL ");
      }
      else
      {
        Logger::report("field = ",(char*)xmlGetAttrValue(atts,"name") );
        rel->fields.push_back(rel_fld = pNewField);
      }
      m_parseState = FIELD;
      break;

    default:
      delete pNewField;
      m_parseState = ERROR;
    }
  }
  else if (xmlStrEqual(fullname,(XML_Char*)Index::TAG))
  {
    Index* pIdx = new Index(index_unique(xmlGetAttrValue(atts,"unique")));
    
    switch (m_parseState)
    {
    case OBJECT:
      idx = pIdx;
      obj->indices.push_back(idx);
      m_parseState = INDEX;
      break;

    case RELATION:
      idx = pIdx;
      rel->indices.push_back(idx);
      m_parseState = INDEX;
      break;

    default:
      delete pIdx;
      m_parseState = ERROR;
    }
  }
  else if (xmlStrEqual(fullname,(XML_Char*)IndexField::TAG))
  {
    if (m_parseState!=INDEX)
    {
      m_parseState = ERROR;
    }
    else
    {
      IndexField idxField((char*)xmlGetAttrValue(atts,"name"));
      idx->fields.push_back(idxField);
    }    
    
    m_parseState = INDEXFIELD;
  }
  else if (xmlStrEqual(fullname,(XML_Char*)Value::TAG))
  {
    if (m_parseState!=FIELD)
    {
      m_parseState = ERROR;
    }
    else
    {
      if (fld) 
      {
        fld->value(Value((char*)xmlGetAttrValue(atts,"name"), (char*)xmlGetAttrValue(atts,"value")));
      }
      Logger::report("value = " );
    }

  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Method::TAG))
  {
    if (m_parseState!=OBJECT)
    {
      m_parseState = ERROR;
    }
    else
    {
      obj->methods.push_back(
        mtd = new Method( safe((char*)xmlGetAttrValue(atts,"name")), 
        safe((char*)xmlGetAttrValue(atts,"returntype")) 
        )
        );
      m_parseState= METHOD;
      Logger::report("method = " );
    }

  }
  else if (xmlStrEqual(fullname,(XML_Char*)Param::TAG))
  {
    if (m_parseState!=METHOD)
    {
      m_parseState = ERROR;
    }
    else
    {
      mtd->param(Param((char*)xmlGetAttrValue(atts,"name"),(char*)xmlGetAttrValue(atts,"type")));
    }
  }
  else if (xmlStrEqual(fullname,(XML_Char*)Relation::TAG))
  {
    if (m_parseState!=DATABASE)
    {
      m_parseState = ERROR;
    }
    else
    {
      m_pObjectModel->relations.push_back(rel = new Relation(  safe((char*)xmlGetAttrValue(atts,"id")), 
        safe((char*)xmlGetAttrValue(atts,"name")),
        relation_unidir(xmlGetAttrValue(atts,"unidir"))));

      Logger::report( "relation = " + rel->getName());

      m_parseState = RELATION;
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Relate::TAG))
  {
    if (m_parseState!=RELATION)
    {
      m_parseState = ERROR;
    }
    else
    {
      rel->related.push_back(
        new Relate( safe((char*)xmlGetAttrValue(atts,"object")), 
        relate_limit(xmlGetAttrValue(atts,"limit")), 
        relate_unique(xmlGetAttrValue(atts,"unique")), 
        safe((char*)xmlGetAttrValue(atts,"handle"))));
      Logger::report("relate = " );
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)"include"))
  {
    string filename((char*)xmlGetAttrValue(atts,"name"));
    if (m_parseState!=DATABASE)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("include \"" + filename + '"' );
      ObjectModel includedModel;
      if (!includedModel.loadFromFile(filename)) 
      {
        Logger::report("error on parsing included file \"" + filename + '"' );
      }
      m_pObjectModel->objects.insert(m_pObjectModel->objects.end(),includedModel.objects.begin(),includedModel.objects.end());
      m_pObjectModel->relations.insert(m_pObjectModel->relations.end(),includedModel.relations.begin(),includedModel.relations.end());
      m_parseState = INCLUDE;
    }
  } 
  else
  {
    m_parseState = UNKNOWN;
    Logger::error("unknown = ",fullname );
  } 
}

void LitesqlParser::onEndElement(const XML_Char *fullname)
{
  //  Logger::report("ending ",fullname); 
  if (xmlStrEqual(fullname,(XML_Char*)Database::TAG))
  {
    if (m_parseState!=DATABASE)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end database " );
      m_parseState = ROOT;
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Object::TAG))
  {
    if (m_parseState!=OBJECT)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end object " );
      obj = NULL;
      fld= NULL;
      rel = NULL;
      rel_fld = NULL;
      m_parseState = DATABASE;
    }
  } 
  else if (xmlStrEqual(fullname,xml::Field::TAG))
  {
    if (m_parseState!=FIELD)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end field" );
      fld = NULL;
      m_parseState = history.back();
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Index::TAG))
  {
    if (m_parseState!=INDEX)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end index" );
      idx = NULL;
      m_parseState = history.back();
    }
  }
  else if (xmlStrEqual(fullname,(XML_Char*)IndexField::TAG))
  {
    if (m_parseState!=INDEXFIELD)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end indexfield" );
      m_parseState = history.back();
    }
  }
  else if (xmlStrEqual(fullname,(XML_Char*)Value::TAG))
  {
    if (m_parseState!=FIELD)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end value" );
    }

  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Method::TAG))
  {
    if (m_parseState!=METHOD)
    {
      m_parseState = ERROR;
    }
    else
    {
      m_parseState = OBJECT;
      Logger::report("end method" );
    }

  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Param::TAG))
  {
    if (m_parseState!=METHOD)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end param" );
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Relation::TAG))
  {
    if (m_parseState!=RELATION)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end relation " );
      rel = NULL;
      rel_fld = NULL;
      m_parseState = DATABASE;
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)Relate::TAG))
  {
    if (m_parseState!=RELATION)
    {
      m_parseState = ERROR;
    }
    else
    {
      rel_fld = NULL;
      Logger::report("end relate" );
    }
  } 
  else if (xmlStrEqual(fullname,(XML_Char*)"include"))
  {
    if (m_parseState!=INCLUDE)
    {
      m_parseState = ERROR;
    }
    else
    {
      Logger::report("end include " );
      m_parseState = DATABASE;
    }
  } 
  else 
  {
    m_parseState = history.back();
    Logger::error( "end unknown " );
  }

  history.pop_back();
}

bool ObjectModel::loadFromFile(const std::string& filename)
{
  LitesqlParser parser(this);

  bool successfulParsed = parser.parseFile(filename);
  if (successfulParsed)
  {
    xml::init(db,objects,relations);
  }
  return successfulParsed;
}
