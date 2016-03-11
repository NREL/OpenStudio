/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

/** \file mysql.hpp
 * MySQL MySQL::Cursor MySQL::Backend */
#ifndef _litesql_mysql_hpp
#define _litesql_mysql_hpp

#ifdef HAVE_LIBMYSQLCLIENT

#include <mysql.h>
#include "litesql/except.hpp"
#include "litesql/types.hpp"
#include "litesql/string.hpp"
#include "litesql/backend.hpp"

#include <string>
namespace litesql {
using namespace std;
/** MySQL - backend */
class MySQL : public Backend {
    mutable MYSQL conn;
    string host, user, passwd, database;
    int port;
public:
    /** MySQL - result */
    class Result : public Backend::Result {
        MYSQL_RES * res;
    public:
        Result(MYSQL_RES * res);
        virtual ~Result();
        virtual size_t fieldNum() const;
        virtual Record fields() const;
        virtual size_t recordNum() const;
        virtual Records records() const;
        const Records& recordsRef() const;
    };
    /** MySQL - cursor, Note: because MySQL-does not support multiple
     * cursors in single connection, this class actually makes one connection
     * per cursor */
    class Cursor : public Backend::Cursor {
        MYSQL_RES * res;
        MYSQL conn;
        size_t fieldNum;
    public:
        Cursor(string host, string user, string passwd, string database, 
               int port, string q);
        virtual Record fetchOne();
        virtual ~Cursor();
    };
    MySQL(const string& connInfo);
    virtual bool supportsSequences() const;
    virtual string getRowIDType() const;
    virtual string getInsertID() const;
    virtual void begin() const;
    virtual void commit() const;
    virtual void rollback() const;
    Backend::Result* execute(string query) const;
    Backend::Cursor* cursor(string query) const;
    virtual ~MySQL();
};
}
#endif
#endif
