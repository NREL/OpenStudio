/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <ruleset/StandardsFilterObjectAttribute.hpp>
#include <ruleset/StandardsFilterObjectAttribute_Impl.hpp>
#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>

#include <utilities/core/Compare.hpp>

#include <QDomDocument>
#include <QDomElement>

#include <boost/bind.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsFilterObjectAttribute_Impl::StandardsFilterObjectAttribute_Impl(
      const StandardsFilterAvailableObject& object)
    : StandardsFilterClause_Impl(), m_object(object)
  {
    this->connectChild(m_object);
  }

  StandardsFilterObjectAttribute_Impl::StandardsFilterObjectAttribute_Impl(
      const StandardsFilterAvailableObject& object,
      const UUID& uuid, 
      const UUID& versionUUID)
    : StandardsFilterClause_Impl(uuid, versionUUID), m_object(object)
  {
    this->connectChild(m_object);
  }

  StandardsFilterObjectAttribute_Impl::StandardsFilterObjectAttribute_Impl(
      const QDomElement& element)
    : StandardsFilterClause_Impl(element),
      m_object(StandardsFilterAvailableObject(element.firstChildElement(QString::fromStdString("Object"))))
  {
    BOOST_ASSERT(!element.isNull());
    this->connectChild(m_object);
  }

  std::vector<StandardsFilterClause> StandardsFilterObjectAttribute_Impl::referencedFilters() const {
    StandardsFilterClauseVector result;
    result.push_back(m_object);
    return result;
  }

  StandardsFilterAvailableObject StandardsFilterObjectAttribute_Impl::object() const {
    return m_object;
  }

  void StandardsFilterObjectAttribute_Impl::replaceReferencedFilters(
      std::vector<StandardsFilterClause>& preferredInstances)
  {
    StandardsFilterClauseVector::const_iterator it = std::find_if(
        preferredInstances.begin(),
        preferredInstances.end(),boost::bind(
            uuidsEqual<StandardsFilterClause,StandardsFilterClause>,
            m_object,
            _1));
    if (it != preferredInstances.end()) { 
      setObject(it->cast<StandardsFilterAvailableObject>()); 
    }
  }

  void StandardsFilterObjectAttribute_Impl::setObject(const StandardsFilterAvailableObject& object) {
    this->disconnectChild(m_object);
    m_object = object;
    this->connectChild(m_object);
    this->onChange();
  }

  void StandardsFilterObjectAttribute_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsFilterClause_Impl::writeValues(doc, element);
  }

  bool StandardsFilterObjectAttribute_Impl::equivalentData(const RulesetObject& other) const {
    // assume called from derived class
    StandardsFilterObjectAttribute otherObjectAttribute = other.cast<StandardsFilterObjectAttribute>();
    if (m_object.equivalentData(otherObjectAttribute.object())) {
      return StandardsFilterClause_Impl::equivalentData(other);
    }
    return false;
  }

  bool StandardsFilterObjectAttribute_Impl::equivalentAttributeType(
      const StandardsFilterObjectAttribute& other) const
  {
    // assume called from derived class
    StandardsFilterObjectAttribute otherObjectAttribute = other.cast<StandardsFilterObjectAttribute>();
    return m_object.equivalentData(otherObjectAttribute.object());
  }

} // detail

std::string StandardsFilterObjectAttribute::shortDescription() const {
  return getImpl<detail::StandardsFilterObjectAttribute_Impl>()->shortDescription();
}

StandardsFilterAvailableObject StandardsFilterObjectAttribute::object() const {
  return getImpl<detail::StandardsFilterObjectAttribute_Impl>()->object();
}

void StandardsFilterObjectAttribute::negate() {
  getImpl<detail::StandardsFilterObjectAttribute_Impl>()->negate();
}

bool StandardsFilterObjectAttribute::setValue(const std::string& value) {
  return getImpl<detail::StandardsFilterObjectAttribute_Impl>()->setValue(value);
}

std::string StandardsFilterObjectAttribute::printForMessage() const {
  return getImpl<detail::StandardsFilterObjectAttribute_Impl>()->printForMessage();
}

bool StandardsFilterObjectAttribute::equivalentAttributeType(
    const StandardsFilterObjectAttribute& other) const 
{
  return getImpl<detail::StandardsFilterObjectAttribute_Impl>()->equivalentAttributeType(other);
}

/// @cond
void StandardsFilterObjectAttribute::mf_setObject(const StandardsFilterAvailableObject& object) {
  getImpl<detail::StandardsFilterObjectAttribute_Impl>()->setObject(object);
}

StandardsFilterObjectAttribute::StandardsFilterObjectAttribute(
    boost::shared_ptr<detail::StandardsFilterObjectAttribute_Impl> impl)
  : StandardsFilterClause(impl)
{}
/// @endcond

bool objectTypeLess(const StandardsFilterObjectAttribute& left, 
                    const StandardsFilterObjectAttribute& right,
                    const std::vector<standardsinterface::ObjectType>& order)
{
  standardsinterface::ObjectTypeVector::const_iterator leftIt = 
      std::find(order.begin(),order.end(),left.object().objectType());
  standardsinterface::ObjectTypeVector::const_iterator rightIt = 
      std::find(order.begin(),order.end(),right.object().objectType());
  return (leftIt < rightIt);
}

} // ruleset
} // openstudio
