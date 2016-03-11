/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#ifndef _litesql_types_hpp
#define _litesql_types_hpp
#include <string>
#include <vector>
#include "litesql/utils.hpp"
#include "litesql/except.hpp"
/** \file types.hpp
    contains class Record and typedef Records */

namespace litesql
{
class Database;
/** SQL data row wrapper. */
class Record : public std::vector<std::string>
{
public: 
    Record() : std::vector<std::string>() {}
    /** defined for compatibility with Persistent */
    Record(const Database & db, const Record& r ) { *this = r;}
};
/** shortcut */
typedef std::vector<Record> Records;

}
#endif
