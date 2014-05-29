/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "WorkspaceObjectOrder.hpp"

#include "../idd/IddObject.hpp"

#include "../math/Permutation.hpp"

#include "../core/Assert.hpp"

namespace openstudio {

namespace detail {
  // CONSTRUCTORS

  WorkspaceObjectOrder_Impl::WorkspaceObjectOrder_Impl(const ObjectGetter& objectGetter) 
    : ObjectOrderBase(), m_objectGetter(objectGetter) {}

  WorkspaceObjectOrder_Impl::WorkspaceObjectOrder_Impl(const std::vector<Handle>& directOrder, 
                                                       const ObjectGetter& objectGetter) 
    : ObjectOrderBase(true), m_objectGetter(objectGetter), m_directOrder(directOrder) {}

  WorkspaceObjectOrder_Impl::WorkspaceObjectOrder_Impl(
      const std::vector<IddObjectType>& iddOrder,const ObjectGetter& objectGetter) 
    : ObjectOrderBase(iddOrder), m_objectGetter(objectGetter) {}

  // GETTERS AND SETTERS

  bool WorkspaceObjectOrder_Impl::isDirectOrder() const { return m_directOrder; }

  boost::optional< std::vector<Handle> > WorkspaceObjectOrder_Impl::directOrder() const { 
    return m_directOrder; 
  }
    
  void WorkspaceObjectOrder_Impl::setDirectOrder(const std::vector<Handle>& order) {
    /// \todo Elements in order should be unique? Algorithms will still work if not true, but
    /// may get unexpected results.
    ObjectOrderBase::setDirectOrder();
    m_directOrder = order;
  }

  bool WorkspaceObjectOrder_Impl::push_back(const Handle& handle) {
    if (!m_directOrder) { return false; }
    m_directOrder->push_back(handle);
    return true;
  }

  bool WorkspaceObjectOrder_Impl::insert(const Handle& handle, const Handle& insertBeforeHandle) {
    if (!m_directOrder) { return false; }
    auto it = getIterator(insertBeforeHandle);
    m_directOrder->insert(it,handle);
    return true;
  }
   
  bool WorkspaceObjectOrder_Impl::insert(const Handle& handle,unsigned index) {
    if (!m_directOrder) { return false; }
    if (index < m_directOrder->size()) {
      auto it = m_directOrder->begin();
      for (unsigned i = 0; i < index; ++i, ++it);
      m_directOrder->insert(it,handle);
      return true;
    }
    else { 
      m_directOrder->push_back(handle); 
      return true;
    }
  }
    
  bool WorkspaceObjectOrder_Impl::move(const Handle& handle, const Handle& insertBeforeHandle) {
    if (!m_directOrder) { return false; }
    // find type in order
    auto it = getIterator(handle);
    if (it == m_directOrder->end()) { return false; }
    // handle degenerate case
    if (handle == insertBeforeHandle) { return true; }
    // erase handle
    m_directOrder->erase(it);
    // reinsert at given location
    return insert(handle,insertBeforeHandle);
  }

  bool WorkspaceObjectOrder_Impl::move(const Handle& handle, unsigned index) {
    if (!m_directOrder) { return false; }
    // find type in order
    auto it = getIterator(handle);
    if (it == m_directOrder->end()) { return false; }
    // handle degenerate case
    if ((it - m_directOrder->begin()) == static_cast<int>(index)) { return true; }
    // erase handle
    m_directOrder->erase(it);
    // reinsert at given location
    return insert(handle,index);
  }

  bool WorkspaceObjectOrder_Impl::swap(const Handle& handle1, const Handle& handle2) {
    if (!m_directOrder) { return false; }
    auto it1 = getIterator(handle1);
    auto it2 = getIterator(handle2);
    if ((it1 == m_directOrder->end()) || (it2 == m_directOrder->end())) { return false; }
    if (it1 == it2) { return true; }
    *it1 = handle2;
    *it2 = handle1;
    return true;
  }

  bool WorkspaceObjectOrder_Impl::erase(const Handle& handle) {
    if (!m_directOrder) { return false; }
    auto it = getIterator(handle);
    if (it == m_directOrder->end()) { return false; }
    m_directOrder->erase(it);
    return true;
  }

  void WorkspaceObjectOrder_Impl::setOrderByIddEnum() {
    ObjectOrderBase::setOrderByIddEnum();
    m_directOrder = boost::none;
  }

  void WorkspaceObjectOrder_Impl::setIddOrder(const std::vector<IddObjectType>& order) {
    ObjectOrderBase::setIddOrder(order);
    m_directOrder = boost::none;
  }

  // SORTING

  /** Predicate for external sorters (for instance, std::set). */
  bool WorkspaceObjectOrder_Impl::less(const Handle& left,const Handle& right) const {
    if (!m_directOrder) {
      return ObjectOrderBase::less(getIddObjectType(left),getIddObjectType(right));
    }
    else {
      return (getIterator(left) < getIterator(right));
    }
  }

  /** Predicate for external sorters (for instance, std::set). */
  bool WorkspaceObjectOrder_Impl::less(const WorkspaceObject& left, 
                                       const WorkspaceObject& right) const 
  {
    if (!m_directOrder) {
      return ObjectOrderBase::less(left.iddObject().type(),right.iddObject().type());
    }
    else {
      return (getIterator(left) < getIterator(right));
    }
  }

  bool WorkspaceObjectOrder_Impl::less(IddObjectType left, IddObjectType right) const {
    if (!m_directOrder) {
      return ObjectOrderBase::less(left,right);
    }
    else {
      return (getIterator(left) < getIterator(right));
    }
  }

  std::vector<Handle> WorkspaceObjectOrder_Impl::sort(const std::vector<Handle>& handles) const {
    HandleVector result(handles);
    std::sort(result.begin(), result.end(), std::bind(&WorkspaceObjectOrder_Impl::less_Handle, this, std::placeholders::_1, std::placeholders::_2));
    return result;
  }

  std::vector<WorkspaceObject> WorkspaceObjectOrder_Impl::sort(
      const std::vector<WorkspaceObject>& objects) const 
  {
    WorkspaceObjectVector result(objects);
    std::sort(result.begin(), result.end(), std::bind(&WorkspaceObjectOrder_Impl::less_WorkspaceObject, this, std::placeholders::_1, std::placeholders::_2));
    return result;
  }

  /// returns empty vector if not all handles can be converted to objects
  std::vector<WorkspaceObject> WorkspaceObjectOrder_Impl::sortedObjects(const std::vector<Handle>& handles) const {
    if (isDirectOrder()) {
      // easier to sort handles
      HandleVector sorted = sort(handles);
      return getObjects(sorted);
    }
    else {
      // easier to sort objects, and have to get them anyway
      WorkspaceObjectVector objects = getObjects(handles);
      return sort(objects);
    }
  }
  /// returns empty vector if not all objects can be converted to handles
  std::vector<Handle> WorkspaceObjectOrder_Impl::sortedHandles(
      const std::vector<WorkspaceObject>& objects) const 
  {
    if (isDirectOrder()) {
      // easier to sort handles, get first and sort
      HandleVector handles = getHandles<WorkspaceObject>(objects);
      return sort(handles);
    }
    else {
      // easier to sort objects, sort, then get handles
      WorkspaceObjectVector sorted = sort(objects);
      return getHandles<WorkspaceObject>(sorted);
    }
  }

  // STATE CHECKING

  /** Returns whether order of handle is directly specified. */
  bool WorkspaceObjectOrder_Impl::inOrder(const Handle& handle) const {
    if (m_directOrder) {
      auto loc = getIterator(handle);
      if (loc != m_directOrder->end()) { return true; }
    }
    return false;
  }

  /** Returns index of handle in order, if its order is directly specified. */
  boost::optional<unsigned> WorkspaceObjectOrder_Impl::indexInOrder(const Handle& handle) const {
    if (m_directOrder) {
      auto loc = getIterator(handle);
      if (loc != m_directOrder->end()) { 
        return (loc - m_directOrder->begin()); 
      }
    }
    return boost::none;
  }

  // PROTECTED

  void WorkspaceObjectOrder_Impl::setObjectGetter(const ObjectGetter& getter) {
    m_objectGetter = getter;
  }

  // PRIVATE

  // only call when m_directOrder == true
  std::vector<Handle>::iterator WorkspaceObjectOrder_Impl::getIterator(const Handle& handle) {
    OS_ASSERT(m_directOrder);
    return std::find(m_directOrder->begin(),m_directOrder->end(),handle);
  }

  std::vector<Handle>::iterator WorkspaceObjectOrder_Impl::getIterator(IddObjectType type) {
    OS_ASSERT(m_directOrder);
    for (auto it = m_directOrder->begin(), 
         itEnd = m_directOrder->end(); it != itEnd; ++ it) 
    {
      if (getIddObjectType(*it) == type) { return it; }
    }
    return m_directOrder->end();
  }

  std::vector<Handle>::iterator WorkspaceObjectOrder_Impl::getIterator(
      const WorkspaceObject& object) 
  {
    OS_ASSERT(m_directOrder);
    return std::find(m_directOrder->begin(),m_directOrder->end(),object.handle());
  }

  std::vector<Handle>::const_iterator WorkspaceObjectOrder_Impl::getIterator(
      const Handle& handle) const 
  {
    OS_ASSERT(m_directOrder);
    return std::find(m_directOrder->begin(),m_directOrder->end(),handle);
  }

  std::vector<Handle>::const_iterator WorkspaceObjectOrder_Impl::getIterator(IddObjectType type) const {
    OS_ASSERT(m_directOrder);
    for (auto it = m_directOrder->begin(), 
         itEnd = m_directOrder->end(); it != itEnd; ++ it) {
      if (getIddObjectType(*it) == type) { return it; }
    }
    return m_directOrder->end();
  }

  std::vector<Handle>::const_iterator WorkspaceObjectOrder_Impl::getIterator(
      const WorkspaceObject& object) const 
  {
    OS_ASSERT(m_directOrder);
    return std::find(m_directOrder->begin(),m_directOrder->end(),object.handle());
  }

  boost::optional<IddObjectType> WorkspaceObjectOrder_Impl::getIddObjectType(
      const Handle& handle) const 
  {
    OS_ASSERT(m_objectGetter);
    OptionalWorkspaceObject object = m_objectGetter(handle);
    if (object) { return object->iddObject().type(); }
    else { return boost::none; }
  } 

  std::vector<WorkspaceObject> WorkspaceObjectOrder_Impl::getObjects(
      const std::vector<Handle>& handles) const 
  {
    WorkspaceObjectVector objects;
    // loop through handles and try to find objects
    for (const Handle& handle : handles) {
      OS_ASSERT(m_objectGetter);
      OptionalWorkspaceObject object = m_objectGetter(handle);
      if (object) { objects.push_back(*object); }
    }
    return objects;
  }

  // ETH@20100409 std::bind seems to need non-overloaded functions
  // These are (ugly) wrappers to accommodate.
  bool WorkspaceObjectOrder_Impl::less_Handle(const Handle& left, const Handle& right) const {
    return less(left,right);
  }

  bool WorkspaceObjectOrder_Impl::less_WorkspaceObject(const WorkspaceObject& left, 
                                                       const WorkspaceObject& right) const 
  { return less(left,right); }

  bool WorkspaceObjectOrder_Impl::less_IddObjectType(IddObjectType left, 
                                                     IddObjectType right) const 
  { return less(left,right); }

} // detail

// GETTERS AND SETTERS

bool WorkspaceObjectOrder::orderByIddEnum() const {
  return m_impl->orderByIddEnum();
}

void WorkspaceObjectOrder::setOrderByIddEnum() {
  m_impl->setOrderByIddEnum();
}

boost::optional< std::vector<IddObjectType> > WorkspaceObjectOrder::iddOrder() const {
  return m_impl->iddOrder();
}

void WorkspaceObjectOrder::setIddOrder(const std::vector<IddObjectType>& order) {
  m_impl->setIddOrder(order);
}

bool WorkspaceObjectOrder::push_back(IddObjectType type) {
  return m_impl->ObjectOrderBase::push_back(type);
}

bool WorkspaceObjectOrder::insert(IddObjectType type, IddObjectType insertBeforeType) {
  return m_impl->ObjectOrderBase::insert(type,insertBeforeType);
}

bool WorkspaceObjectOrder::insert(IddObjectType type, unsigned index) {
  return m_impl->ObjectOrderBase::insert(type,index);
}

bool WorkspaceObjectOrder::move(IddObjectType type, IddObjectType insertBeforeType) {
  return m_impl->ObjectOrderBase::move(type,insertBeforeType);
}

bool WorkspaceObjectOrder::move(IddObjectType type, unsigned index) {
  return m_impl->ObjectOrderBase::move(type,index);
}

bool WorkspaceObjectOrder::swap(IddObjectType type1, IddObjectType type2) {
  return m_impl->ObjectOrderBase::swap(type1,type2);
}

bool WorkspaceObjectOrder::erase(IddObjectType type) {
  return m_impl->ObjectOrderBase::erase(type);
}


bool WorkspaceObjectOrder::isDirectOrder() const {
  return m_impl->isDirectOrder();
}

boost::optional< std::vector<Handle> > WorkspaceObjectOrder::directOrder() const {
  return m_impl->directOrder();
}

void WorkspaceObjectOrder::setDirectOrder(const std::vector<Handle>& order) {
  m_impl->setDirectOrder(order);
}

bool WorkspaceObjectOrder::push_back(const Handle& handle) {
  return m_impl->push_back(handle);
}

bool WorkspaceObjectOrder::insert(const Handle& handle, const Handle& insertBeforeHandle) {
  return m_impl->insert(handle,insertBeforeHandle);
}

bool WorkspaceObjectOrder::insert(const Handle& handle,unsigned index) {
  return m_impl->insert(handle,index);
}

bool WorkspaceObjectOrder::move(const Handle& handle, const Handle& insertBeforeHandle) {
  return m_impl->move(handle,insertBeforeHandle);
}

bool WorkspaceObjectOrder::move(const Handle& handle, unsigned index) {
  return m_impl->move(handle,index);
}

bool WorkspaceObjectOrder::swap(const Handle& handle1, const Handle& handle2) {
  return m_impl->swap(handle1,handle2);
}

bool WorkspaceObjectOrder::erase(const Handle& handle) {
  return m_impl->erase(handle);
}

// SORTING

std::vector<Handle> WorkspaceObjectOrder::sort(const std::vector<Handle>& handles) const {
  return m_impl->sort(handles);
}

std::vector<WorkspaceObject> WorkspaceObjectOrder::sort(
    const std::vector<WorkspaceObject>& objects) const
{
  return m_impl->sort(objects);
}

// STATE CHECKING

bool WorkspaceObjectOrder::inOrder(const Handle& handle) const {
  return m_impl->inOrder(handle);
}

boost::optional<unsigned> WorkspaceObjectOrder::indexInOrder(const Handle& handle) const {
  return m_impl->indexInOrder(handle);
}

// PROTECTED

// CONSTRUCTORS

WorkspaceObjectOrder::WorkspaceObjectOrder(
    const std::shared_ptr<detail::WorkspaceObjectOrder_Impl>& impl) 
  : m_impl(impl) {}

} // openstudio
