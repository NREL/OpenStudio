/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */


#ifndef litesql_selectquery_hpp
#define litesql_selectquery_hpp

#include "litesql/utils.hpp"
#include "litesql/expr.hpp"
/** \file selectquery.hpp
    contains SelectQuery-class. See \ref usage_selecting_persistents */
namespace litesql {
/** a class that helps creating SELECT-SQL statements. methods are 
    self-explanatory. */
class SelectQuery {
    bool _distinct;
    int _limit, _offset;
    Split _results;
    Split _sources;
    std::string _where; 
    Split _groupBy;
    std::string _having;
    Split _orderBy;
public:
    SelectQuery() : _distinct(false), _limit(0), _offset(0), _where("True") {}
    SelectQuery & distinct(bool d) ;
    SelectQuery & limit(int value);
    SelectQuery & offset(int value);
    SelectQuery & result(std::string r);
    SelectQuery & clearResults();
    SelectQuery & source(std::string s, std::string alias="");
    SelectQuery & where(const Expr & w);
    SelectQuery & where(std::string w);
    SelectQuery & groupBy(std::string gb);
    SelectQuery & having(const Expr & h);
    SelectQuery & having(std::string h);
    SelectQuery & orderBy(std::string ob, bool ascending=true);
    operator std::string() const;
    std::string asString() const { return this->operator std::string(); }
};
}

#endif
