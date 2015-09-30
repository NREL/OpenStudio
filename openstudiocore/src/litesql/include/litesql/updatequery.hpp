/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#ifndef _litesql_updatequery_hpp
#define _litesql_updatequery_hpp

#include "litesql/utils.hpp"
#include "litesql/expr.hpp"
/** \file updatequery.hpp
    contains UpdateQuery-class. */
namespace litesql {
/** a class that helps creating UPDATE-SQL statements. methods are 
    self-explanatory. */
using namespace std;    
class UpdateQuery {
    string table;
    string _where;
    Split fields;
    Split values;
public:
    UpdateQuery(string t) : table(t), _where("True") {}
    UpdateQuery& where(const Expr& e);
    UpdateQuery& set(FieldType f, string value);
    operator std::string() const;
    std::string asString() const { return this->operator std::string(); }
};
}

#endif
