/* LiteSQL - MySQL-backend implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#include "compatibility.hpp"
#include "mysql.hpp"
#ifdef HAVE_LIBMYSQLCLIENT
#include <string>
namespace litesql {
using namespace std;

MySQL::Result::Result(MYSQL_RES * r) : res(r) {
}
MySQL::Result::~Result() {
    mysql_free_result(res);
}
size_t MySQL::Result::fieldNum() const {
    if (!res)
        return 0;
    return mysql_num_fields(res);
}

Record MySQL::Result::fields() const {
    if (!res)
        return Record();
    MYSQL_FIELD * flds = mysql_fetch_fields(res);
    Record rec;
    rec.reserve(fieldNum());
    for (size_t i = 0; i < fieldNum(); i++)
        rec.push_back(flds[i].name);
    return rec;
}

size_t MySQL::Result::recordNum() const {
    if (!res)
        return 0;
    return mysql_num_rows(res);
}  
Records MySQL::Result::records() const {
    if (!res)
        return Records();
    Records recs;

    recs.resize(recordNum());
    mysql_data_seek(res, 0);
    size_t fieldnum = fieldNum();
    for (size_t i = 0; i < recordNum(); i++) {
        MYSQL_ROW row = mysql_fetch_row(res);
        if (!row)
            break;
        recs[i].reserve(fieldnum);
        for (size_t i2 = 0; i2 < fieldnum; i2++) {
            recs[i].push_back(row[i2] ? row[i2] : "NULL");
        }
    }
    return recs;
}
MySQL::Cursor::Cursor(string host, string user, string passwd, string database,
        int port, string q) : res(NULL) {
    mysql_init(&conn);
    if (!mysql_real_connect(&conn, host.c_str(), user.c_str(), passwd.c_str(),
              database.c_str(), port, NULL, 0)) {
        throw DatabaseError(mysql_error(&conn));
    }

    if (mysql_real_query(&conn, q.c_str(), q.size())) {
        throw SQLError(mysql_error(&conn));
    }
    res = mysql_use_result(&conn);
    fieldNum = mysql_num_fields(res);
}
                                               
Record MySQL::Cursor::fetchOne() {
    MYSQL_ROW row = mysql_fetch_row(res);
    if (!row)
        return Record();
    Record rec;
    rec.reserve(fieldNum);
    for (size_t i = 0; i < fieldNum; i++)
        rec.push_back(row[i] ? row[i] : "NULL");
    return rec;
}
MySQL::Cursor::~Cursor() {
    while (mysql_fetch_row(res) != NULL) {}
    mysql_free_result(res);
    mysql_close(&conn);
}
MySQL::MySQL(const string& connInfo) {
    Split params(connInfo,";");
    host = "localhost";
    int port = 0;
    for (size_t i = 0; i < params.size(); i++) {
        Split param(params[i], "=");
        if (param.size() == 1)
            continue;
        if (param[0] == "host")
            host = param[1];
        else if (param[0] == "database")
            database = param[1];
        else if (param[0] == "password")
            passwd = param[1];
        else if (param[0] == "user")
            user = param[1];
        else if (param[0] == "port")
            port = atoi(param[1]);
    }
    mysql_init(&conn);
    if (!mysql_real_connect(&conn, host.c_str(), user.c_str(), passwd.c_str(),
              database.c_str(), port, NULL, 0)) {
        throw DatabaseError(mysql_error(&conn));
    }
}
bool MySQL::supportsSequences() const {
    return false;
}
string MySQL::getRowIDType() const {
    return "INTEGER PRIMARY KEY AUTO_INCREMENT";
}
string MySQL::getInsertID() const {
    return toString(mysql_insert_id(&conn));
}
void MySQL::begin() const {
  delete execute("BEGIN");
}
void MySQL::commit() const {
  delete execute("COMMIT");
}
void MySQL::rollback() const {
  delete execute("ROLLBACK");
}
Backend::Result* MySQL::execute(string query) const {
    if (mysql_real_query(&conn, query.c_str(), query.size())) {
        throw SQLError(mysql_error(&conn));    
    }
    return new Result(mysql_store_result(&conn));
}
Backend::Cursor* MySQL::cursor(string query) const {
    return new Cursor(host, user, passwd, database, port, query);       
}
MySQL::~MySQL() {
    mysql_close(&conn);
}
}
#endif
