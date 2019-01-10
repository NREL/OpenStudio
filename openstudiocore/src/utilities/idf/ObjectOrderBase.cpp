/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "ObjectOrderBase.hpp"
#include "../math/Permutation.hpp"


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
