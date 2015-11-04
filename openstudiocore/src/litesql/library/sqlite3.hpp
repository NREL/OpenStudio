/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

/** \file sqlite3.hpp
 * SQLite3 SQLite3::Result SQLite3::Cursor */
#ifndef _litesql_sqlite3_hpp
#define _litesql_sqlite3_hpp
#ifdef HAVE_LIBSQLITE3
#include "litesql/except.hpp"
#include "litesql/types.hpp"
#include "litesql/string.hpp"
#include "litesql/backend.hpp"

#include <string>
struct sqlite3;
struct sqlite3_stmt;

namespace litesql {
using namespace std;
/** SQLite3 - backend */
class SQLite3 : public Backend {
    sqlite3 *db;
    mutable bool transaction;
protected:
    void throwError(int status) const;     
public:
    /** SQLite3 - result */
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
        sqlite3 * db;
        sqlite3_stmt * stmt;
        const SQLite3& owner;
    public:
        Cursor(sqlite3 * db, sqlite3_stmt * s, const SQLite3& owner);
        virtual Record fetchOne();
        virtual ~Cursor();
    };
    SQLite3(const string& database);
    virtual bool supportsSequences() const;
    virtual string getInsertID() const;
    virtual void begin() const;
    virtual void commit() const;
    virtual void rollback() const;
    Backend::Result* execute(string query) const;
    Backend::Cursor* cursor(string query) const;
    virtual ~SQLite3();
};
}
#endif
#endif
