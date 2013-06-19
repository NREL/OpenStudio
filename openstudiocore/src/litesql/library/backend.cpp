/* LiteSQL - Common routines for backends
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#include <map>

#include "compatibility.hpp"
#include "litesql/backend.hpp"
#include "litesql/string.hpp"
#include "litesql/types.hpp"

#ifdef HAVE_LIBMYSQLCLIENT
#include "mysql.hpp"
#endif

#ifdef HAVE_LIBPQ
#include "postgresql.hpp"
#endif

#ifdef HAVE_LIBSQLITE3
#include "sqlite3.hpp"
#endif

#ifdef HAVE_ODBC
#include "odbc_backend.hpp"
#endif

using namespace litesql;
using namespace std;    

string Backend::groupInsert(Record tables, Records fields, Records values,
                   string sequence) const {
    string id = values[0][0];
    
    if (supportsSequences() && values[0][0] == "NULL") {
      Result * r = execute("SELECT nextval('" + sequence + "');");
      id = r->records()[0][0];
      delete r;
    } 
    for (int i = tables.size()-1; i >= 0; i--) {
      string fieldString = Split::join(fields[i],",");
        string valueString;
        if (!values[i].empty())
            values[i][0] = id;
        Split valueSplit(values[i]);
        for (size_t i2 = 0; i2 < valueSplit.size(); i2++)
            valueSplit[i2] = escapeSQL(valueSplit[i2]);
        valueString = valueSplit.join(",");
        string query = "INSERT INTO " + tables[i] + " (" + fieldString
            + ") VALUES (" + valueString + ");";
        delete execute(query);
        if (!supportsSequences() && id == "NULL") 
            id = getInsertID();
        
    }
    return id;
}

Backend* Backend::getBackend(const string & backendType,const string& connInfo)
{
  Backend* backend;

#ifdef HAVE_LIBMYSQLCLIENT
  if (backendType == "mysql") {
    backend = new MySQL(connInfo);
  } else
#endif
#ifdef HAVE_LIBPQ
    if (backendType == "postgresql") {
      backend = new PostgreSQL(connInfo);
    } else
#endif
#ifdef HAVE_ODBC
      if (backendType == "odbc") {
        backend = new ODBCBackend(connInfo);
      } else
#endif
#ifdef HAVE_LIBSQLITE3
        if (backendType == "sqlite3") {
          backend = new SQLite3(connInfo);
        } else
#endif
        { 
          backend = NULL;
        };
        return backend;
}
