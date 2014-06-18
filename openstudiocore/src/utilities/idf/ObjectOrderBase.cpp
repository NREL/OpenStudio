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

#include "ObjectOrderBase.hpp"
#include "../math/Permutation.hpp"

#include "../core/Assert.hpp"

namespace openstudio {

// CONSTRUCTORS

ObjectOrderBase::ObjectOrderBase() : m_orderByIddEnum(true) {
}

ObjectOrderBase::ObjectOrderBase(bool directOrder) : m_orderByIddEnum(!directOrder)
{}

ObjectOrderBase::ObjectOrderBase(const IddObjectTypeVector& iddOrder) :
    m_orderByIddEnum(false),
    m_iddOrder(iddOrder) {}

// GETTERS AND SETTERS

bool ObjectOrderBase::orderByIddEnum() const {
  return m_orderByIddEnum;
}

void ObjectOrderBase::setOrderByIddEnum() {
  m_iddOrder = boost::none;
  m_orderByIddEnum = true;
}

boost::optional<IddObjectTypeVector> ObjectOrderBase::iddOrder() const {
  return m_iddOrder;
}

void ObjectOrderBase::setIddOrder(const IddObjectTypeVector& order) {
  m_iddOrder = order;
  m_orderByIddEnum = false;
}

bool ObjectOrderBase::push_back(IddObjectType type) {
  if (!m_iddOrder) { return false; }
  m_iddOrder->push_back(type);
  return true;
}

bool ObjectOrderBase::insert(IddObjectType type, IddObjectType insertBeforeType) {
  if (!m_iddOrder) { return false; }
  auto it = getIterator(insertBeforeType);
  m_iddOrder->insert(it,type);
  return true;
}

bool ObjectOrderBase::insert(IddObjectType type, unsigned index) {
  if (!m_iddOrder) { return false; }
  if (index < m_iddOrder->size()) {
    auto it = m_iddOrder->begin();
    for (unsigned i = 0; i < index; ++i, ++it);
    m_iddOrder->insert(it,type);
  }
  else { m_iddOrder->push_back(type); }
  return true;
}

bool ObjectOrderBase::move(IddObjectType type, IddObjectType insertBeforeType) {
  if (!m_iddOrder) { return false; }
  // find type in order
  auto it = getIterator(type);
  if (it == m_iddOrder->end()) { return false; }
  // handle degenerate case
  if (type == insertBeforeType) { return true; }
  // erase type
  m_iddOrder->erase(it);
  // reinsert at given location
  return insert(type,insertBeforeType);
}

bool ObjectOrderBase::move(IddObjectType type, unsigned index) {
  if (!m_iddOrder) { return false; }
  // find type in order
  auto it = getIterator(type);
  if (it == m_iddOrder->end()) { return false; }
  // handle degenerate case
  if ((it - m_iddOrder->begin()) == static_cast<int>(index)) { return true; }
  // erase type
  m_iddOrder->erase(it);
  // reinsert at given index
  return insert(type,index);
}

bool ObjectOrderBase::swap(IddObjectType type1, IddObjectType type2) {
  if (!m_iddOrder) { return false; }
  // ETH@20100408 \todo Would std::swap work? Better?
  auto it1 = getIterator(type1);
  auto it2 = getIterator(type2);
  if ((it1 == m_iddOrder->end()) || (it2 == m_iddOrder->end())) { return false; }
  if (it1 == it2) { return true; }
  *it1 = type2;
  *it2 = type1;
  return true;
}

bool ObjectOrderBase::erase(IddObjectType type) {
  if (!m_iddOrder) { return false; }
  auto it = getIterator(type);
  if (it == m_iddOrder->end()) { return false; }
  m_iddOrder->erase(it);
  return true;
}

void ObjectOrderBase::setDirectOrder() {
  m_orderByIddEnum = false;
  m_iddOrder = boost::none;
}

// SORTING

bool ObjectOrderBase::less(IddObjectType left, IddObjectType right) const {
  if (m_orderByIddEnum) {
    return (left < right);
  }
  else {
    OS_ASSERT(m_iddOrder);
    return (getIterator(left) < getIterator(right));
  }
}

bool ObjectOrderBase::less(boost::optional<IddObjectType> left, 
                           boost::optional<IddObjectType> right) const {
  if (left && right) { return less(*left,*right); }
  if (left) { return true; }
  if (right) { return false; }
  return true;
}

// STATE CHECKING

bool ObjectOrderBase::inOrder(const IddObjectType& type) const {
  if (m_orderByIddEnum) { return true; }
  if (m_iddOrder) {
    auto it = getIterator(type);
    if (it != m_iddOrder->end()) { return true; }
  }
  return false;
}

OptionalUnsigned ObjectOrderBase::indexInOrder(const IddObjectType& type) const {
  if (m_orderByIddEnum) { return static_cast<unsigned>(type.value()); }
  if (m_iddOrder) { 
    auto it = getIterator(type);
    return (it - m_iddOrder->begin());
  }
  return boost::none;
}

// PRIVATE

// assumes that m_iddOrder == true
IddObjectTypeVector::iterator ObjectOrderBase::getIterator(const IddObjectType& type) {
  OS_ASSERT(m_iddOrder);
  return std::find(m_iddOrder->begin(),m_iddOrder->end(),type);
}

IddObjectTypeVector::const_iterator ObjectOrderBase::getIterator(const IddObjectType& type) const {
  OS_ASSERT(m_iddOrder);
  return std::find(m_iddOrder->begin(),m_iddOrder->end(),type);
}

} // openstudio
