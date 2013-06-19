#include "xmlobjects.hpp"
#include "md5.hpp"
#include "litesql.hpp"
#include "logger.hpp"

#include <algorithm>

namespace std {
    template <>
    struct less<xml::Relate*> {
        bool operator()(xml::Relate const* r1, xml::Relate const* r2) {
            if (!r1)
                return true;
            if (!r2)
                return false;
            return *r1 < *r2;
        }
    };
}
namespace xml {

const char* Database::TAG="database";
const char* Object::TAG="object";
const char* Field::TAG="field";
const char* Relation::TAG="relation";
const char* Relate::TAG="relate";
const char* Value::TAG="value";
const char* IndexField::TAG="indexfield";
const char* Index::TAG="index";
const char* Param::TAG="param";
const char* Method::TAG="method";

const Object Object::DEFAULT_BASE("litesql::Persistent","");
    
string validID(const string& s, const string& type="field") {
    if (s.size() == 0) 
        return "empty identifier";
    if (toupper(s[0]) == s[0] && type == "field")
        return "does not begin with lower case letter";
    if (!isalpha(s[0])) 
        return "first character is not alphabet";
    for (size_t i = 1; i < s.size(); i++)
        if (!isalnum(s[i]) && !(s[i] != '_'))
            return "illegal character : " + s[i];
    if (s[s.size()-1] == '_')
        return "ends with an underscore";
    return "";
}
string makeDBName(const string& s) {
    if (s.size() > 31)
        return "_" + md5HexDigest(s);
    return s;
}
static void sanityCheck(Database& db,
                        const vector<Object*>& objects,
                        const vector<Relation*>& relations) {
    using namespace litesql;
    map<string, bool> usedID;
    map<string, bool> objectName;
    string err;
    if (!(err = validID(db.name,"class")).empty()) 
        throw Except("invalid id: database.name : " + db.name);
    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = *objects[i];
        if (!(err = validID(o.name, "class")).empty())
            throw Except("invalid id: object.name : " + o.name);
        if (usedID.find(o.name) != usedID.end())
            throw Except("duplicate id: object.name : " + o.name);
        usedID[o.name] = true;
        objectName[o.name] = true;
        map<string, bool> usedField;
        usedField.clear();
        for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
            Field& f = *o.fields[i2];
            if (!(err = validID(f.name)).empty())
                throw Except("invalid id: object.field.name : " + o.name + "." + f.name);
            if (usedField.find(f.name) != usedField.end())
                throw Except("duplicate id: object.field.name : " + o.name + "." + f.name);
            usedField[f.name] = true;
        }
    }
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = *relations[i];
        string name = r.getName();
        if (!(err = validID(name,"class")).empty())
            throw Except("invalid id: relation.name : " + name);
        if (usedID.find(name) != usedID.end())
            throw Except("duplicate id: relation.name : " + name);
        usedID[name] = true;
        map<string, bool> usedField;
        usedField.clear();
  
        bool defaults = false;
        
        for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
            Field& f = *r.fields[i2];
            if (!(err = validID(f.name)).empty())
                throw Except("invalid id: relation.field.name : " + name + "." + f.name);
            if (usedField.find(f.name) != usedField.end())
                throw Except("duplicate id: relation.field.name : " + name + "." + f.name);
            usedField[f.name] = true;
            if (f.default_.size() > 0)
                defaults = true;
            else if (defaults)
                throw Except("no default-value after field with default value : " + name + "." + f.name);

        }
        usedField.clear();
        bool limits = false;
        bool uniques = false;
        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            Relate& rel = *r.related[i2];
            if (!(err = validID(rel.handle)).empty() && !rel.handle.empty())
                throw Except("invalid id: relation.relate.handle : " + name + "." + rel.handle);
            if (usedField.find(rel.handle) != usedField.end())
                throw Except("duplicate id: relation.relate.handle : " + name + "." + rel.handle);
            if (objectName.find(rel.objectName) == objectName.end())
                throw Except("unknown object: relation.relate.name : " + name + "." + rel.objectName);
            if (!rel.handle.empty())
                usedField[rel.handle] = true;
            if (rel.isUnique())
                uniques = true;
            if (rel.hasLimit())
                limits = true;
            if (uniques && limits)
                throw Except("both 'unique' and 'limit' attributes used in relation " + name);
        }
        if (r.related.size() != 2 && limits)
            throw Except("'limit' attribute used in relation of " + toString(r.related.size()) 
                         + " object(s) " + name);
            
    }   
}

static void initSchema(Database& db,
                vector<Object*>& objects,
                vector<Relation*>& relations) {
    for (size_t i = 0; i < objects.size(); i++) {
        Object& o = *objects[i];
        map<string, Database::DBField*> fldMap;
        Database::Table* tbl = new Database::Table;
        tbl->name = o.getTable();
        db.tables.push_back(tbl);
        
        if (!o.parentObject) {
            Database::Sequence* seq = new Database::Sequence;
            seq->name = o.getSequence();
            seq->table = o.getTable();
            db.sequences.push_back(seq);
        }  else {
            Database::DBField *id = new Database::DBField; 
            id->name = "id_";
            id->type = "INTEGER";
            id->primaryKey = true;
            tbl->fields.push_back(id);
        }

        for (size_t i2 = 0; i2 < o.fields.size(); i2++) {
            Field& f = *o.fields[i2];
            Database::DBField* fld = new Database::DBField;
            fld->name = f.name + "_";
            fldMap[f.name] = fld;
            fld->type = f.getSQLType();
            fld->primaryKey = (f.name == "id");
            if (f.isUnique())
                fld->extra = " UNIQUE";
            fld->field = o.fields[i2];
            tbl->fields.push_back(fld);
            
            if (f.isIndexed()) {
                Database::DBIndex* idx = new Database::DBIndex;
                idx->name = makeDBName(tbl->name + fld->name + "idx");
                idx->table = tbl->name;
                idx->fields.push_back(fld);
                db.indices.push_back(idx);
            }
                
        }
        for (size_t i2 = 0; i2 < o.indices.size(); i2++) {
            const Index& idx = *o.indices[i2];

            litesql::Split fldNames;
            Database::DBIndex* index = new Database::DBIndex;
            for (size_t i3 = 0; i3 < idx.fields.size(); i3++) {
                if (fldMap.find(idx.fields[i3].name) == fldMap.end())
                    throw litesql::Except("Indexfield " + o.name + "." + idx.fields[i3].name + " is invalid.");
                index->fields.push_back(fldMap[idx.fields[i3].name]);
                fldNames.push_back(idx.fields[i3].name);
            }

            index->name = makeDBName(tbl->name + "_" + fldNames.join("_") + "_idx");
            index->table = tbl->name;
            
            string unique = "";
            if (idx.isUnique())
                index->unique = true;
            db.indices.push_back(index);
        }
    }
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& r = *relations[i];
        Database::Table* tbl = new Database::Table;
        db.tables.push_back(tbl);
        tbl->name = r.getTable();
        vector<Database::DBField*> objFields;
        map<string, Database::DBField*> fldMap;
        for (size_t i2 = 0; i2 < r.related.size(); i2++) {
            const xml::Relate& relate = *r.related[i2];
            string extra;
            if (relate.isUnique())
                    extra = " UNIQUE";
            else if (r.related.size() == 2) { 
                if (i2 == 0 && r.related[1]->hasLimit())
                    extra = " UNIQUE";
                if (i2 == 1 && r.related[0]->hasLimit())
                    extra = " UNIQUE";
            }
            Database::DBField* fld = new Database::DBField;
            fld->name = relate.fieldName;
            fld->type = "INTEGER";
            fld->extra = extra;
            tbl->fields.push_back(fld);
            objFields.push_back(fld);
            
            Database::DBIndex* idx = new Database::DBIndex;
            idx->name = makeDBName(tbl->name + fld->name + "idx");
            idx->table = tbl->name;
            idx->fields.push_back(fld);
            db.indices.push_back(idx);
            
        }
        for (size_t i2 = 0; i2 < r.fields.size(); i2++) {
            Field& f = *r.fields[i2];
            Database::DBField* fld = new Database::DBField;
            fld->name = f.name + "_";
            fldMap[f.name] = fld;
            fld->type = f.getSQLType();
            fld->primaryKey = false;
            if (f.isUnique())
                fld->extra = " UNIQUE";
            fld->field = r.fields[i2];
            tbl->fields.push_back(fld);
            
            if (f.isIndexed()) {
                Database::DBIndex* idx = new Database::DBIndex;
                idx->name = makeDBName(tbl->name + fld->name + "idx");
                idx->table = tbl->name;
                idx->fields.push_back(fld);
                db.indices.push_back(idx);
            }
            
        }
    
        if (r.related.size() > 1) {

            Database::DBIndex* idx = new Database::DBIndex;
            idx->name = makeDBName(tbl->name + "_all_idx");
            idx->table = tbl->name;
            for (size_t i2 = 0; i2 < objFields.size(); i2++)
                idx->fields.push_back(objFields[i2]);
            db.indices.push_back(idx);
        }
        for (size_t i2 = 0; i2 < r.indices.size(); i2++) {
            const Index& idx = *r.indices[i2];

            litesql::Split fldNames;
            Database::DBIndex* index = new Database::DBIndex;
            for (size_t i3 = 0; i3 < idx.fields.size(); i3++) {
//                Database::DBField* fld = new Database::DBField;
                if (fldMap.find(idx.fields[i3].name) == fldMap.end())
                    throw litesql::Except("Indexfield " + r.name + "." + idx.fields[i3].name + " is invalid.");
                index->fields.push_back(fldMap[idx.fields[i3].name]);
                fldNames.push_back(idx.fields[i3].name);
            }

            index->name = makeDBName(tbl->name + "_" + fldNames.join("_") + "_idx");
            string unique = "";
            if (idx.isUnique())
                index->unique = true;
            db.indices.push_back(index);
        }
    }

}
void init(Database& db,
          vector<Object*>& objects,
          vector<Relation*>& relations) {
    map<string, Object*> objMap;
    Logger::report("validating XML file\n");
    sanityCheck(db, objects, relations);
    Logger::report("linking XML - objects\n"); 
    // make string -> Object mapping

    for (size_t i = 0; i < objects.size(); i++)
        objMap[objects[i]->name] = objects[i];

    // make Object's class hierarchy mapping (parent and children)

    for (size_t i = 0; i < objects.size(); i++) 
      if (objMap.find(objects[i]->inherits) != objMap.end()) {
            objects[i]->parentObject = objMap[objects[i]->inherits];
            objects[i]->parentObject->children.push_back(objects[i]);
      }
    //for (size_t i = 0; i < objects.size(); i++) 
    //    if (objects[i]->parentObject)
    //        objects[i]->parentObject->children.push_back(objects[i]);

    // sort objects of relations alphabetically (ascii)

    for (size_t i = 0; i < relations.size(); i++) {
        sort(relations[i]->related.begin(), relations[i]->related.end(),
                less<Relate*>());
    }
          
    for (size_t i = 0; i < relations.size(); i++) {
        Relation& rel = *relations[i];
        bool same = rel.sameTypes() > 1;
        
        for (size_t i2 = 0; i2 < rel.related.size(); i2++) {
            Relate& relate = *rel.related[i2];
            Object* obj = objMap[relate.objectName];
            string num;
            if (same)
                num = toString(i2 + 1);
            relate.fieldTypeName = relate.objectName + num;
            relate.fieldName = relate.objectName + toString(i2 + 1);
            if (obj->relations.find(&rel) == obj->relations.end())
                obj->relations[&rel] = vector<Relate*>();

            // make Object -> Relation mapping

            obj->relations[&rel].push_back(&relate);
            if (!relate.handle.empty()) {
                
                // make Object's relation handles

                RelationHandle* handle = new RelationHandle(relate.handle, &rel,
                                                                      &relate, obj);
                for (size_t i3 = 0; i3 < rel.related.size(); i3++) {
                    if (i2 != i3) {
                        Object* o = objMap[rel.related[i3]->objectName];
                        // make RelationHandle -> (Object,Relation) mapping
                        handle->destObjects.push_back(make_pair(o,rel.related[i3]));
                    }
                }
                obj->handles.push_back(handle);
            }
        }
    }
    

    initSchema(db, objects, relations);

}
string safe(const char * s) {
    if (s)
        return s;
    return "";
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


}
