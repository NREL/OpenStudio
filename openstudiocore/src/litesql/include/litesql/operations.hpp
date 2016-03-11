/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#ifndef __litesql_operations_hpp
#define __litesql_operations_hpp
#include "litesql/datasource.hpp"
/** \file operations.hpp 
 * select<T>, intersect<T>, union_<T>, except<T> */
namespace litesql {
using namespace std;
/** returns DataSource for accessing objects of type T */
template <class T>
litesql::DataSource<T> select(const litesql::Database & db, 
                              const litesql::Expr &e=litesql::Expr()) {
    return litesql::DataSource<T>(db, e);
}
/** returns DataSource for accessing intersection of two sets of objects 
 * of type T */
template <class T>
litesql::DataSource<T> intersect(const litesql::DataSource<T>& ds1, 
                                 const litesql::DataSource<T>& ds2) {
    std::string sel = ds1.idQuery().asString() + " INTERSECT " 
        + ds2.idQuery().asString();
    return litesql::DataSource<T>(ds1.getDatabase(), T::Id.in(sel));
}
/** returns DataSource for accessing union of two sets of objects of type T */
template <class T>
litesql::DataSource<T> union_(const litesql::DataSource<T>& ds1, 
                              const litesql::DataSource<T>& ds2) {
    std::string sel = ds1.idQuery().asString() + " UNION " 
        + ds2.idQuery().asString();
    return litesql::DataSource<T>(ds1.getDatabase(), T::Id.in(sel));
}
/** returns DataSource for accessing objects of type T that are in first
 *  DataSource but not in second. */
template <class T>
litesql::DataSource<T> except(const litesql::DataSource<T>& ds1, 
                              const litesql::DataSource<T>& ds2) {
    std::string sel = ds1.idQuery().asString() + " EXCEPT " 
        + ds2.idQuery().asString();
    return litesql::DataSource<T>(ds1.getDatabase(), T::Id.in(sel));
}

}
#endif
