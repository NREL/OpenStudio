/* LiteSQL - SelectQuery implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#include "compatibility.hpp"
#include "litesql/selectquery.hpp"
using namespace std;
namespace litesql {
SelectQuery & SelectQuery::distinct(bool d) { 
    _distinct = d; 
    return *this;
}
SelectQuery & SelectQuery::limit(int value) { 
    _limit = value;	
    return *this;
}
SelectQuery & SelectQuery::offset(int value) { 
    _offset = value; 
    return *this;
}
SelectQuery & SelectQuery::result(std::string r) { 
    _results.push_back(r); 
    return *this;
}
SelectQuery & SelectQuery::clearResults() {
    _results.clear();
    return *this;
}
SelectQuery & SelectQuery::source(std::string s, std::string alias) {
    if (!alias.empty())
        s += " AS " + alias;
    _sources.push_back(s);
    return *this;
}
SelectQuery & SelectQuery::where(const Expr & w) { 
    _where = (RawExpr(_where) && w).asString();	
    return *this;
}
SelectQuery & SelectQuery::where(std::string w) { 
    _where = (RawExpr(_where) && RawExpr(w)).asString();
    return *this;
}
SelectQuery & SelectQuery::groupBy(std::string gb) { 
    _groupBy.push_back(gb);	
    return *this;
}
SelectQuery & SelectQuery::having(const Expr & h) { 
    _having = h.asString(); 
    return *this;
}
SelectQuery & SelectQuery::having(std::string h) { 
    _having = h;
    return *this;
}
SelectQuery & SelectQuery::orderBy(std::string ob, bool ascending) { 
    std::string value = ob;
    if (!ascending)
        value += " DESC";
    _orderBy.push_back(value); 
    return *this;
}
SelectQuery::operator string() const {
    std::string res = "SELECT ";
    if (_distinct)
        res += "DISTINCT ";
    res += _results.join(",");
    res += " FROM ";
    res += _sources.join(",");
    if (_where != "True")
        res += " WHERE " + _where;
    if (_groupBy.size() > 0)
        res += " GROUP BY " + _groupBy.join(",");
    if (!_having.empty())
        res += " HAVING " + _having;
    if (_orderBy.size() > 0)
        res += " ORDER BY " + _orderBy.join(",");
    if (_limit) 
        res += " LIMIT " + toString(_limit);
    if (_offset) 
            res += " OFFSET " + toString(_offset);
    return res;
}
}
