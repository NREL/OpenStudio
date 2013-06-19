/* LiteSQL - Database implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#include "compatibility.hpp"
#include "litesql/database.hpp"
#include "litesql/cursor.hpp"
#include "litesql/except.hpp"
#include "litesql/selectquery.hpp"
#include <map>
#include <algorithm>

namespace litesql {
using namespace std;

void Database::openDatabase() {
   backend = Backend::getBackend(backendType,connInfo);
   if (backend==NULL)
      throw DatabaseError("Unknown backend: " + backendType);
}

void Database::storeSchemaItem(const SchemaItem& s) const {
    delete_("schema_", 
            RawExpr("name_='" + s.name 
                + "' and type_='" + s.type + "'"));
    Record values;

    values.push_back(s.name);
    values.push_back(s.type);
    values.push_back(s.sql);
    insert("schema_", values);
}

vector<Database::SchemaItem> Database::getCurrentSchema() const {
    SelectQuery sel;
    sel.result("name_").result("type_").result("sql_").source("schema_");
    vector<SchemaItem> s;
    Records recs;
    try {
        recs = query(sel);
    } catch (Except e) {
        return vector<Database::SchemaItem>();
    }
    for (size_t i = 0; i < recs.size(); i++) {
        s.push_back(SchemaItem(recs[i][0], recs[i][1], recs[i][2]));
    }
    return s;
}

bool operator ==(const ColumnDefinition& c1,const ColumnDefinition& c2)
{
  return (c1.name == c2.name) && (c1.type==c2.type);
}

typedef vector<ColumnDefinition> ColumnDefinitions;

static ColumnDefinitions getFields(string schema) {
    ColumnDefinitions fields;
    int start = schema.find("(");
    int end = schema.find(")");
    if (start == -1 || end == -1)
        return fields;
    Split tmp(replace(schema.substr(start+1, end-start-1), ", ", ","), ",");
    
    ColumnDefinition field_def;
    for (size_t i = 0; i < tmp.size(); i++) 
    {
      Split s(tmp[i]);
      field_def.name = s[0];
      field_def.type = s[1];
      fields.push_back(field_def);
    }
    return fields;
}


bool Database::addColumn(const string & name,const ColumnDefinition & column_def) const 
{
  query("ALTER TABLE " + name + " ADD COLUMN " + column_def.name +" "+ column_def.type);
  return true;
}

void Database::upgradeTable(string name, 
                            string oldSchema, string newSchema) const {
    ColumnDefinitions oldFields = getFields(oldSchema);
    ColumnDefinitions newFields = getFields(newSchema);

    ColumnDefinitions toAdd(newFields);
    ColumnDefinitions::iterator found;

    for (ColumnDefinitions::iterator it = oldFields.begin();it!=oldFields.end();it++)
    {
      found = find_if(toAdd.begin(),toAdd.end(),ColumnDefinition::EqualName(it->name));
      if (found!=toAdd.end())
      {
        toAdd.erase(found);
      }
    }

    ColumnDefinitions commonFields;
    for (ColumnDefinitions::iterator it = oldFields.begin();it!=oldFields.end();it++)
    {
      found = find_if(newFields.begin(),newFields.end(),ColumnDefinition::EqualName(it->name));
      if (found!=newFields.end())
      {
        commonFields.push_back(*found);
      }
    }

    begin();
    string bkp_name(name+"backup");
    query(" ALTER TABLE " + name + " RENAME TO " + bkp_name); 
    for (ColumnDefinitions::iterator it = toAdd.begin();it!= toAdd.end();it++)
    {
      addColumn(bkp_name,*it);
    }

    query(newSchema);
    // oldfields as ...
    Split cols;
    string s;

    for (ColumnDefinitions::iterator it = commonFields.begin();it!= commonFields.end();it++)
    {
        s = it->name;
        s.append(" AS ");
        s.append(it->name);
        cols.push_back(s);
    }
    
    for (ColumnDefinitions::iterator it = toAdd.begin();it!= toAdd.end();it++)
    {
        s = it->name;
        s.append(" AS ");
        s.append(it->name);
        cols.push_back(s);
    }

    query(" INSERT INTO " + name + " SELECT "+ cols.join(",")+" FROM " + bkp_name); 
    query(" DROP TABLE " + bkp_name); 
    commit();
}

Database::Database(const string& backend, const string& conn) 
     : backendType(backend), connInfo(conn), backend(NULL), verbose(false) {
        openDatabase();
    }
    Database::Database(const Database &op) 
      : backendType(op.backendType), connInfo(op.connInfo), 
        verbose(op.verbose) {
        openDatabase();
    }
    Database::~Database() {
        delete backend;
    }
    void Database::create() const { 
        vector<SchemaItem> s = getSchema(); 
        begin();
        for (size_t i = 0; i < s.size(); i++) {
            query(s[i].sql);
            storeSchemaItem(s[i]);
        }
        commit();
    } 
    void Database::drop() const { 
        vector<SchemaItem> s = getSchema(); 

        for (size_t i = 0; i < s.size(); i++) {
            try {
                begin();
                if (s[i].type == "table")
                    query("DROP TABLE " + s[i].name);
                else if (s[i].type == "sequence")
                    query("DROP SEQUENCE " + s[i].name);
                commit();
            } catch (Except e) {
                rollback();
            }
        }
    }  
   
    bool Database::needsUpgrade() const {
        vector<SchemaItem> cs = getCurrentSchema();
        vector<SchemaItem> s = getSchema();
        map<string, int> items;
        for (size_t i = 0; i < cs.size(); i++) 
            items[cs[i].name] = i;
    
        for (size_t i = 0; i < s.size(); i++) {
            if (items.find(s[i].name) == items.end() 
                    || cs[items[s[i].name]].sql != s[i].sql) 
                return true;
        }
        return false;
    }
    void Database::upgrade() const {
        vector<SchemaItem> cs = getCurrentSchema();
        vector<SchemaItem> s = getSchema();
        map<string, int> items;
        for (size_t i = 0; i < cs.size(); i++) {
            items[cs[i].name] = i;
        }
        begin();
        for (size_t i = 0; i < s.size(); i++) {
            if (items.find(s[i].name) == items.end()) {
                query(s[i].sql);
                storeSchemaItem(s[i]);
                continue;
            }
            if (s[i].type == "table" && cs[items[s[i].name]].sql != s[i].sql) {
                upgradeTable(s[i].name, cs[items[s[i].name]].sql, s[i].sql);
                storeSchemaItem(s[i]);
            }
        }
        commit();
    }
    Records Database::query(const string &q) const {
        if (verbose)
            cerr << q << endl;
        auto_ptr<Backend::Result> r(backend->execute(q));
        return r->records();
    }

    void Database::insert(const string &table, const Record &r,
                          const Split& fields) const {
        string command = "INSERT INTO " + table;
        if (fields.size())
            command += " (" + fields.join(",") + ")";
        command += " VALUES (";
        unsigned int i;
        for (i=0; i < r.size() -1; i++) {
            command += escapeSQL(r[i]) + ",";
        }
        command += escapeSQL(r[i]) + ")";
        query(command);
    }
    string Database::groupInsert(Record tables, Records fields, Records values, 
                     string sequence) const {
        if (verbose) {
            cerr << "groupInsert" << endl;
            for (size_t i = 0; i < tables.size(); i++) {
                cerr << "\t" << tables[i] << endl;
                cerr << "\t\tfields : " << Split::join(fields[i],",") << endl;
                cerr << "\t\tvalues : " << Split::join(values[i],"|") << endl;
            }
        }
        return backend->groupInsert(tables, fields, values, sequence);
    }

    void Database::delete_(const string& table, const Expr& e) const {
        string where = "";
        if (e.asString() != "True")
            where = " WHERE " + e.asString();
        query("DELETE FROM " + table + where);
    }

}
    
