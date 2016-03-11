/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

/** \file datasource.hpp
 * selectObjectQuery<T> and DataSource<T> */
#ifndef litesql_datasource_hpp
#define litesql_datasource_hpp
#include <set>
#include <string>
#include "litesql/database.hpp"
#include "litesql/selectquery.hpp"
#include "litesql/expr.hpp"
namespace litesql {
using namespace std;
/** returns SelectQuery which selects objects of type T 
 *  \param fdatas fields of class T
    \param e optional filter expression */
SelectQuery selectObjectQuery(const vector<FieldType>& fdatas, 
                               const Expr & e=Expr());
   
/** returns SelectQuery which selects objects of type T 
    \param e optional filter expression */
template <class T>
SelectQuery selectObjectQuery(const Expr & e=Expr()) {
    std::vector<FieldType> fdatas;
    T::getFieldTypes(fdatas);
    return selectObjectQuery(fdatas, e);
}
/** template class which holds SelectQuery for selecting objects of type T */
template <class T>
class DataSource {
private:
    /** database reference, used to create cursors */
    const Database & db;
    /** selection query */
    SelectQuery sel;
public:
    /** \param db_ database reference
        \param e selection filter */
    DataSource(const Database & db_, const Expr& e = Expr()) 
        : db(db_), sel(selectObjectQuery<T>(e)) {
    }
    /** \param db_ database reference
        \param sel_ selection query */
    DataSource(const Database & db_, const SelectQuery& sel_) 
        : db(db_), sel(sel_) {
    }

    /** returns database reference */
    const Database & getDatabase() const {
        return db;
    }
    /** returns SelectQuery which selects ID-numbers of objects */
    SelectQuery idQuery() const {
        SelectQuery idq(sel);
        idq.clearResults();
        idq.result(T::Id.fullName());
        return idq;
    }
    /** returns number of objects in result set */
    size_t count() const {
       SelectQuery cq(sel);
       cq.clearResults();
       cq.limit(0).offset(0);
       cq.result("count(*)");
       return atoi(db.query(cq)[0][0]);
    }
    /** returns SelectQuery which selects objects */
    SelectQuery objectQuery() const {
        return sel;
    }
    /** returns cursor for query */
    Cursor<T> cursor() const {
        return db.template cursor<T>(sel);
    }
    /** returns first object in result set. throw exception if none found 
     \return object of type T */
    T one() const {
        return *cursor();
    }
    /** returns all objects in result set. */
    vector<T> all() const {
        return cursor().dump();
    }
    /** modifies SelectQuery to order result set 
        \param f field to order by
        \param asc ascending order 
        \return *this, methods can be chained */
    DataSource& orderBy(FieldType f, bool asc=true) {
        sel.orderBy(f.fullName(), asc);
        return *this;
    }
    /** modifies SelectQuery to order result set by external table
        \param id foreign key field used to join table with query
        \param f field to order by
        \param asc ascending order 
        \return *this, methods can be chained */
    DataSource& orderByRelation(FieldType id, FieldType f, bool asc=true) {
        sel.source(id.table());
        sel.where(id == T::Id);
        sel.orderBy(f.fullName(), asc);
        return *this;
    }
};

}
#endif
