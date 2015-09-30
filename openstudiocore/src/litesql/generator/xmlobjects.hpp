#ifndef litesql_xmlobjects_hpp
#define litesql_xmlobjects_hpp

#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <map>
#include "litesql-gen.hpp"
#include "litesql/split.hpp"
#include "litesql/string.hpp"

namespace xml {
using namespace std;
using namespace litesql;
string safe(const char *s);
string attribute(const string& name, const string& value);
string endtag(const string& name);
string makeDBName(const string& s);

class Value {
public:
    static const char* TAG;

    string name, value;
    Value(const string& n, const string& v) : name(n), value(v) {}
};

class IndexField {
public:
    static const char* TAG;

    string name;
    IndexField(const string& n) : name(n) {}
};

class Index {
public:
    static const char* TAG;

    vector<IndexField> fields;
    AT_index_unique unique;
    Index(AT_index_unique u) : unique(u) {}

    bool isUnique() const {
        return unique == A_index_unique_true;
    }
};

class Field {
public:
    static const char* TAG;

    string name;
    string fieldTypeName;
    AT_field_type type;
    string default_;
    AT_field_indexed indexed;
    AT_field_unique unique;
    vector<Value> values;
    Field(const string& n, AT_field_type t, string d, AT_field_indexed i, AT_field_unique u) 
        : name(n), fieldTypeName(capitalize(n)), type(t), default_(d), indexed(i), unique(u) {
    }
    void value(const Value& v) {
        values.push_back(v);
    }
    bool isIndexed() const {
        return indexed == A_field_indexed_true;
    }
    bool isUnique() const {
        return unique == A_field_unique_true;
    }
    bool hasQuotedValues() const {
       switch(type) {
       case A_field_type_integer:
       case A_field_type_float: 
       case A_field_type_boolean: 
       case A_field_type_date:
       case A_field_type_time:
       case A_field_type_datetime:
       case A_field_type_blob:
          return false;
       
       case A_field_type_string:
       default:
         return true;
       }
    }
    string getQuotedDefaultValue() const {
        if (hasQuotedValues())
            return "\"" + default_ + "\"";
        if (default_.size() == 0)
            return "0";
        return default_;
    }
    string getSQLType() const {
       switch(type) {
           case A_field_type_integer: return "INTEGER";
           case A_field_type_string: return "TEXT";
           case A_field_type_float: return "FLOAT";
           case A_field_type_boolean: return "INTEGER";
           case A_field_type_date: return "INTEGER";
           case A_field_type_time: return "INTEGER";
           case A_field_type_datetime: return "INTEGER";
           case A_field_type_blob: return "BLOB";
           default: return "";
       }
    }
    string getCPPType() const {
       switch(type) {
           case A_field_type_integer: return "int";
           case A_field_type_string: return "std::string";
           case A_field_type_float: return "float";
           case A_field_type_boolean: return "bool";
           case A_field_type_date: return "litesql::Date";
           case A_field_type_time: return "litesql::Time";
           case A_field_type_datetime: return "litesql::DateTime";
           case A_field_type_blob: return "litesql::Blob";
           default: return "";
       }
    }
};

class Param {
public:
    static const char* TAG;

  string name;
    AT_param_type type;
    Param(const string& n, AT_param_type t) : name(n), type(t) {}
    
};
class Method {
public:
    static const char* TAG;

    string name, returnType;
    vector<Param> params;
    Method(const string& n, const string& rt) 
        : name(n), returnType(rt) {}
    void param(const Param& p) {
        params.push_back(p);
    }
};
class Relation;
class Relate;
class Object;
class RelationHandle {
public:
    string name;
    Relation * relation;
    Relate * relate;
    Object * object;
    vector< pair<Object*,Relate*> > destObjects;

    RelationHandle(const string& n, Relation * r, Relate * rel, Object * o) 
        : name(n), relation(r), relate(rel), object(o) {}
};
class Relate {
public:  
  static const char* TAG;
  
    string objectName;
    string fieldTypeName, fieldName;
    string getMethodName;
    size_t paramPos;
    AT_relate_limit limit;
    AT_relate_unique unique;
    string handle;
    Relate(const string& on, AT_relate_limit l, AT_relate_unique u, const string& h) 
        : objectName(on), limit(l), unique(u), handle(h) {
        if (hasLimit() && isUnique())
            throw logic_error("both limit and unique specified in relate: line " /*+ 
                              toString(yylineno)*/);
    }
    bool hasLimit() const {
        return limit == A_relate_limit_one;
    }
    
    bool hasLimitOne() const {
        return limit == A_relate_limit_one;
    }
    bool isUnique() const {
        return unique == A_relate_unique_true;
    }
    bool operator < (const Relate& r) const {
        return objectName < r.objectName;
    }

};
class Relation {
public:
    static const char* TAG;

    string id, name;
    string table;
    AT_relation_unidir unidir;
    vector<Relate*> related;
    vector<Field*> fields;
    vector<Index*> indices;
    Relation(const string& i, const string& n, AT_relation_unidir ud) 
        : id(i), name(n), unidir(ud) {}
    string getName() const {
        if (name.size() == 0) {
            string result;
            for (size_t i = 0; i < related.size(); i++) 
                result += related[i]->objectName;
            return result + "Relation" + id;
        }
        return name;
    }
    bool isUnidir() const {
        return unidir == A_relation_unidir_true;
    }
    int sameTypes() const {
        map<string, int> names;
        int max = 0;
        for (size_t i = 0; i < related.size(); i++) {
            if (names.find(related[i]->objectName) == names.end()) 
                names[related[i]->objectName] = 0;
            int value =	++names[related[i]->objectName];
            if (value > max)
                max = value;
        }
        return max;
    }
    int countTypes(const string& name) const {
        int res = 0;
        for (size_t i = 0; i < related.size(); i++)
            if (related[i]->objectName == name)
                res++;
        return res;
    }
    string getTable() const {
        Split res(related.size());
        for (size_t i = 0; i < related.size(); i++)
            res.push_back(related[i]->objectName);
        res.push_back(id);

        return makeDBName(res.join("_"));
    }
};
class Object {
public:
    static const Object DEFAULT_BASE;
    static const char* TAG;

    string name, inherits;
    vector<Field*> fields;
    vector<Method*> methods;
    vector<Index*> indices;
    vector<RelationHandle*> handles;
    map<Relation*, vector<Relate*> > relations;
    Object* parentObject;
    vector<Object*> children;

    Object(const string& n, const string& i) : name(n), inherits(i),
        parentObject(NULL) {
        if (i.size() == 0) {
            inherits = "litesql::Persistent";
            fields.push_back(new Field("id", A_field_type_integer, "", 
                         A_field_indexed_false, A_field_unique_false));
            fields.push_back(new Field("type", A_field_type_string, "", 
                        A_field_indexed_false, A_field_unique_false));
        }
    }

    bool inheritsFromDefault() const
    {
      return inherits == "litesql::Persistent";
    }

    size_t getLastFieldOffset() const {
        if (!parentObject)
            return fields.size();
        else return parentObject->getLastFieldOffset() + fields.size();
    }
    void getAllFields(vector<Field*>& flds) const {
        if (parentObject)
            parentObject->getAllFields(flds);
        for (size_t i = 0; i < fields.size(); i++)
            flds.push_back(fields[i]);
    }
    void getChildrenNames(Split & names) const {
        for (size_t i = 0; i < children.size(); i++) {
            names.push_back(children[i]->name);
            children[i]->getChildrenNames(names);
        }
    }
    const Object* getBaseObject() const{
        if (!parentObject)
            return this;
        else
            return parentObject->getBaseObject();
    }
    string getTable() const {
        return makeDBName(name + "_");
    }
    string getSequence() const {
        return makeDBName(name + "_seq");
    }
};
class Database {
public:
  static const char* TAG;
    class Sequence {
    public:
        string name, table;
        string getSQL() {
            return "CREATE SEQUENCE " + name + " START 1 INCREMENT 1";
        }
    };
    class DBField {
    public:
        string name, type, extra;
        bool primaryKey;
        Field* field;
        vector<DBField*> references;
        DBField() : primaryKey(false) {}
        string getSQL(const string& rowIDType) {
            if (primaryKey)
                type = rowIDType;
            return name + " " + type + extra;
        }
    };
    class DBIndex {
    public:
        string name, table;
        bool unique;
        vector<DBField*> fields;
        DBIndex() : unique(false) {}
        string getSQL() {
            litesql::Split flds;
            for (size_t i = 0; i < fields.size(); i++)
                flds.push_back(fields[i]->name);
            string uniqueS;
            if (unique)
                uniqueS = " UNIQUE";
            return "CREATE" + uniqueS + " INDEX " + name + " ON " + table + " (" + flds.join(",") + ")";
        }
    };
    class Table {
    public:
        string name;
        vector<DBField*> fields;
        string getSQL(const string& rowIDType) {
            litesql::Split flds;
            for (size_t i = 0; i < fields.size(); i++)
                flds.push_back(fields[i]->getSQL(rowIDType));
            return "CREATE TABLE " + name + " (" + flds.join(",") + ")";
        }

    };
    vector<Sequence*> sequences;
    vector<DBIndex*> indices;
    vector<Table*> tables;
    string name, include, nspace;

    bool hasNamespace() const { return !nspace.empty(); }
};

void init(Database& db, 
          vector<Object*>& objects,
          vector<Relation*>& relations);


}

#endif
