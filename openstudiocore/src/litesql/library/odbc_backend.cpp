/* LiteSQL - ODBCBackend-backend implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * Few windows specific bugs fixed by Axel Schmidt.
 * 
 * See LICENSE for copyright information. */
#include "compatibility.hpp"
#include "odbc_backend.hpp"

#include <string>
#ifdef HAVE_ODBC
#ifdef _MSC_VER
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

#else
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#endif

using namespace litesql;
using namespace std;

size_t ODBCBackend::Result::fieldNum() const {
    return flds.size();
}
Record ODBCBackend::Result::fields() const {
    return flds;
}

size_t ODBCBackend::Result::recordNum() const {
    return recs.size();
}  
Records ODBCBackend::Result::records() const {
    return recs;
}
ODBCBackend::Cursor::Cursor(/*ODBCBackend * d, ODBCBackend_stmt * s,*/ const ODBCBackend& o) 
    : /*db(d), stmt(s), */owner(o) {}

Record ODBCBackend::Cursor::fetchOne() {
/*
bool busy = false;
    do 
    {
    	int status = ODBCBackend_step(stmt);
        
        switch(status)
        {
        case SQLITE_ERROR: case SQLITE_MISUSE: {
                std::string error = ODBCBackend_errmsg(db);
                throw UnknownError("step failed: " +toString(status)  + error);
            }
        case SQLITE_DONE: return Record(); break;
        case SQLITE_BUSY: case SQLITE_LOCKED:
            usleep(5000);
            busy = true;
            break;
        case SQLITE_ROW:
            busy = false;
            break;
        }
    } while (busy);
*/    Record rec;
/*    int columnNum = ODBCBackend_data_count(stmt);
    rec.reserve(columnNum);
    for (int i = 0; i < columnNum; i++) {
        if (ODBCBackend_column_type(stmt, i) == SQLITE_NULL)
            rec.push_back("NULL");
        else
            rec.push_back((char*) ODBCBackend_column_text(stmt, i));
    }
*/
    return rec;
}

ODBCBackend::Cursor::~Cursor() {
//    if (stmt) {
//        ODBCBackend_finalize(stmt);
//    }
}

ODBCBackend::ODBCBackend(const string& connInfo) : /*db(NULL),*/ transaction(false) {
    Split params(connInfo,";");
    string database;
    for (size_t i = 0; i < params.size(); i++) {
        Split param(params[i], "=");
        if (param.size() == 1)
            continue;
        if (param[0] == "database")
            database = param[1];
    }
    if (database.size() == 0)
        throw DatabaseError("no database-param specified");

/*    if (ODBCBackend_open(database.c_str(), &db)) {
        throw DatabaseError(ODBCBackend_errmsg(db));
    }
*/
}
bool ODBCBackend::supportsSequences() const {
    return false;
}
string ODBCBackend::getInsertID() const {
    return toString(""/*ODBCBackend_last_insert_rowid(db)*/);
}
void ODBCBackend::begin() const {
    if (!transaction) {
        delete execute("BEGIN;");
        transaction = true;
    }
}
void ODBCBackend::commit() const {
    if (transaction) {
        delete execute("COMMIT;");
        transaction = false;
    }
}
void ODBCBackend::rollback() const {
    if (transaction) {
        delete execute("ROLLBACK;");
        transaction = false;
    }
}

static int callback(void *r, int argc, char **argv, char **azColName) {
/*
ODBCBackend::Result * res = (ODBCBackend::Result*) r;
    if (res->flds.size() == 0) 
        for (int i = 0; i < argc; i++)
            res->flds.push_back(azColName[i]);
    Record rec; 
    for (int i = 0; i < argc; i++) 
        rec.push_back(argv[i] ? argv[i] : "NULL");   
    res->recs.push_back(rec);
*/
   return 0;
}
void ODBCBackend::throwError(int status) const {
/*
string error = ODBCBackend_errmsg(db);
    error = toString(status) + "=status code : " + error;
*/
   switch(status) {
    //case SQLITE_ERROR: throw SQLError(error);
    //case SQLITE_INTERNAL: throw InternalError(error);
    //case SQLITE_NOMEM: throw MemoryError(error);
    //case SQLITE_FULL: throw InsertionError(error);
    default: throw UnknownError("compile failed: "/* + error*/);
    }
}

Backend::Result* ODBCBackend::execute(string query) const {
    Result * r = new Result;
    char * errMsg;
    int status;
    //do {
    //    status = ODBCBackend_exec(db, query.c_str(), callback, r, &errMsg);
    //    switch(status) {         
    //        case SQLITE_BUSY: 
    //        case SQLITE_LOCKED: 
    //            usleep(250000); 
    //            break; 
    //        case SQLITE_OK: break;
    //        default: throwError(status); 
    //    }
    //} while (status != SQLITE_OK); 
    return r;    
}
Backend::Cursor* ODBCBackend::cursor(string query) const {
    while (1) {
//        HSTMT stmt;
        //int status = ODBCBackend_prepare(db, query.c_str(), query.size(), 
        //                             &stmt, NULL);
//if (status != SQLITE_OK || stmt == NULL) {
//            string error = ODBCBackend_errmsg(db);
//            error = toString(status) + "=status code : " + error;
//            switch(status) {
//            case SQLITE_BUSY: 
//            case SQLITE_LOCKED: 
//                usleep(250000); 
//                break;
//            default: throwError(status);
//            }
//        }
//        else
            return new Cursor(/*db, stmt,*/ *this);
    }
    
}
ODBCBackend::~ODBCBackend() {
    //if (db)
    //    ODBCBackend_close(db);
}

#endif
