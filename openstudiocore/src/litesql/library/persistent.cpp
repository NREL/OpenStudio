/* LiteSQL - Persistent-class implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#include "compatibility.hpp"
#include "litesql/persistent.hpp"
#include "litesql/updatequery.hpp"
#include <iostream>

namespace litesql {
using namespace std;

const Persistent & Persistent::operator=(const Persistent & p) {
    if (this != &p) { 
        db = p.db;
        inDatabase = p.inDatabase;
        oldKey = p.oldKey;
    }
    return *this;
}

string Persistent::insert(Record& tables, 
                          Records& fieldRecs,
                          Records& values,
                          const string& sequence) {
    if (values[0][0] == "0")
        for(size_t i = 0; i < values.size(); i++)
            values[i][0] = "NULL";
    string key = db->groupInsert(tables, fieldRecs, values, sequence);
    oldKey = atoi(key);
    inDatabase = true;
    return key;
}
void Persistent::update(Updates& updates) {
    for (Updates::iterator i = updates.begin(); i != updates.end(); i++) {
        UpdateQuery uq(i->first);
        uq.where(RawExpr("id_ = '" + toString(oldKey) + "'"));
        bool notEmpty = false;
        for (vector<pair<FieldType, string> >::iterator i2 =
                i->second.begin(); i2 != i->second.end();
             i2++) {
            uq.set(i2->first, i2->second);
            notEmpty = true;
        }
        if (notEmpty)
            db->query(uq);
    }
}
void Persistent::prepareUpdate(Updates& updates, string table) {
    if (updates.find(table) == updates.end()) {
        updates[table] = vector<pair<FieldType, string> >();
    }
}
void Persistent::deleteFromTable(string table, string id) {
    db->query("DELETE FROM " + table + " WHERE id_="+escapeSQL(id));
}
}



