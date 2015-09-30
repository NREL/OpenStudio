/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */

#ifndef _litesql_cursor_hpp
#define _litesql_cursor_hpp

#include <stdio.h>
#include "litesql/types.hpp"
#include "litesql/backend.hpp"
/** \file cursor.hpp
    Contains class Cursor */
namespace litesql {
using namespace std;
class Database;
/** used to iterate results of SQL statement, creates 
    objects of type T from retrieved records. 
    See \ref usage_cursor */    
template <class T>
class Cursor {
private:
    /** Database - reference which is passed to created(T) - objects */
    const Database& db;
    /** implementation of cursor */
    Backend::Cursor * cursor;
    /** the result set iterated is through */
    bool done;
    /** got data to access */
    bool dataReady;
    /** current record */
    Record currentRow;
public:
    Cursor(const Database& db, Backend::Cursor * c);
    /** deletes Backend::Cursor */
    ~Cursor();
    /** steps to next record */
    Cursor<T> & operator++();
    /** steps to next record */
    Cursor<T> & operator++(int) { return operator++();}
    /** returns the rest of the result set in vector */
    vector<T> dump();
    /** returns current record */
    T operator*();
    /** returns true if there are records left in the result set */
    inline bool rowsLeft() { return !done; }
};

template <class T>
Cursor<T>::Cursor(const Database& db_, Backend::Cursor * c)
    : db(db_), cursor(c), done(false), dataReady(false) {
    operator++();
}
template <class T>
Cursor<T>::~Cursor() {
    delete cursor;
}
template <class T>
Cursor<T> & Cursor<T>::operator++() {
    if (done)
        return *this;
    currentRow = cursor->fetchOne();
    if (currentRow.size() == 0) {
        done = true;
        dataReady = false;
    }
    else
        dataReady = true;
        
    return *this;
}

template <class T>
vector<T> Cursor<T>::dump() {
    vector<T> res;
    for (;!done;operator++())
        res.push_back(operator*());
    return res;
}
template <class T>
T Cursor<T>::operator*() {
    Record rec;

    if (!dataReady)
        throw NotFound();

    return T(db,currentRow);
}

}

#endif
