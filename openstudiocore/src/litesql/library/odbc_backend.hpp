/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

/** \file sqlite3.hpp
 * SQLite3 SQLite3::Result SQLite3::Cursor */
#ifndef _litesql_odbc_backend_hpp
#define _litesql_odbc_backend_hpp
#ifdef HAVE_ODBC
//#include <windows.h>
//#include <sql.h>
//#include <sqlext.h>


#include "litesql/except.hpp"
#include "litesql/types.hpp"
#include "litesql/string.hpp"
#include "litesql/backend.hpp"

#include <string>

namespace litesql {
using namespace std;
/** ODBC - backend */
class ODBCBackend : public Backend {
    mutable bool transaction;
protected:
    void throwError(int status) const;     
public:
    /** ODBC - result */
    class Result : public Backend::Result {
    public:
        Records recs;
        Record flds;
        Result() {}
        virtual size_t fieldNum() const;
        virtual Record fields() const;
        virtual size_t recordNum() const;
        virtual Records records() const;
        const Records& recordsRef() const;
    };
    /** SQLite3 - cursor */
    class Cursor : public Backend::Cursor {
        //HDBC* db;
        //HSTMT* stmt;
        const ODBCBackend& owner;
    public:
        Cursor(/*HDBC* db, HSTMT* s,*/ const ODBCBackend& owner);
        virtual Record fetchOne();
        virtual ~Cursor();
    };
    ODBCBackend(const string& database);
    virtual bool supportsSequences() const;
    virtual string getInsertID() const;
    virtual void begin() const;
    virtual void commit() const;
    virtual void rollback() const;
    Backend::Result* execute(string query) const;
    Backend::Cursor* cursor(string query) const;
    virtual ~ODBCBackend();
};
}
#endif
#endif
