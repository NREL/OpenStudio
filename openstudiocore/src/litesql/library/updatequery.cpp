/* LiteSQL - UpdateQuery implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */
#include "compatibility.hpp"
#include "litesql/updatequery.hpp"
namespace litesql {
using namespace std;
UpdateQuery& UpdateQuery::where(const Expr& e) {
    _where = (RawExpr(_where) && e).asString();
    return *this;
}
UpdateQuery& UpdateQuery::set(FieldType f, string value) {
    fields.push_back(f.name());
    values.push_back(escapeSQL(value));
    return *this;
}
UpdateQuery::operator string() const {
    string q = "UPDATE " + table + " SET ";
    Split sets;
    for (size_t i = 0; i < fields.size(); i++)
        sets.push_back(fields[i] + "=" + values[i]);
    q += sets.join(",");
    if (_where.size())
        q += " WHERE " + _where;
    return q;
}

}
