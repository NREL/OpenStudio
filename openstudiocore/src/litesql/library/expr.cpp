/* LiteSQL - Expr-API implementation
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */


#include "litesql/expr.hpp"
#include "litesql/selectquery.hpp"
namespace litesql {

const char* Expr::True ="True";

In::In(const FieldType & fld, const SelectQuery& s)
    : Oper(fld, "in", "(" + s.asString() + ")") {}
And operator&&(const Expr &o1, const Expr &o2) {
    return And(o1,o2);
}
Or operator||(const Expr &o1, const Expr &o2) {
    return Or(o1,o2);
}
Not operator!(const Expr & exp) {
    return Not(exp);
}
Eq operator==(const FieldType& fld, const FieldType& f2) {
    return Eq(fld, f2);
}
Gt operator>(const FieldType& fld, const FieldType& o2) {
    return Gt(fld, o2);
}
GtEq operator>=(const FieldType& fld, const FieldType& o2) {
    return GtEq(fld, o2);
}
Lt operator<(const FieldType& fld, const FieldType& o2) {
    return Lt(fld, o2);
}
LtEq operator<=(const FieldType& fld, const FieldType& o2) {
    return LtEq(fld, o2);
}
NotEq operator!=(const FieldType& fld, const FieldType& f2) {
    return NotEq(fld, f2);
}

}
