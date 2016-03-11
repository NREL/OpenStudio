/* LiteSQL 
 * 
 * The list of contributors at http://litesql.sf.net/ 
 * 
 * See LICENSE for copyright information. */


#ifndef __litesql_relation_hpp
#define __litesql_relation_hpp
#include <vector>
#include <string>
/** \file relation.hpp
    See \ref usage_using_relations */
namespace litesql {
/** base class of relation handles attached to Persistent objects */
template <class T> 
class RelationHandle {
protected:
    const T* owner;
    RelationHandle(const T& o) : owner(&o) {}    
};

}

#endif
