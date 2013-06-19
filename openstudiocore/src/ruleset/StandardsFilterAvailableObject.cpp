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

#include <ruleset/StandardsFilterAvailableObject.hpp>
#include <ruleset/StandardsFilterAvailableObject_Impl.hpp>

#include <ruleset/StandardsFilterAvailableModel_Impl.hpp>

#include <utilities/core/Compare.hpp>

#include <QDomDocument>
#include <QDomElement>

#include <boost/bind.hpp>

#include <iomanip>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsFilterAvailableObject_Impl::StandardsFilterAvailableObject_Impl(
      const StandardsFilterAvailableModel& model,
      const standardsinterface::ObjectType& objectType)
    : StandardsFilterClause_Impl(), m_model(model), m_objectType(objectType)
  {
    this->connectChild(*m_model);
  }

  StandardsFilterAvailableObject_Impl::StandardsFilterAvailableObject_Impl(
      const StandardsFilterAvailableModel& model,
      const standardsinterface::ObjectType& objectType,
      const UUID& uuid,
      const UUID& versionUUID)
    : StandardsFilterClause_Impl(uuid, versionUUID), m_model(model), m_objectType(objectType)
  {
    this->connectChild(*m_model);
  }

  StandardsFilterAvailableObject_Impl::StandardsFilterAvailableObject_Impl(
      const standardsinterface::ObjectType& objectType)
    : StandardsFilterClause_Impl(), m_objectType(objectType)
  {}

  StandardsFilterAvailableObject_Impl::StandardsFilterAvailableObject_Impl(
      const standardsinterface::ObjectType& objectType,
      const UUID& uuid,
      const UUID& versionUUID)
    : StandardsFilterClause_Impl(uuid, versionUUID), m_objectType(objectType)
  {}

  StandardsFilterAvailableObject_Impl::StandardsFilterAvailableObject_Impl(
      const QDomElement& element)
    : StandardsFilterClause_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  RulesetObject StandardsFilterAvailableObject_Impl::clone() const {
    boost::shared_ptr<StandardsFilterAvailableObject_Impl> impl(new StandardsFilterAvailableObject_Impl(*this));
    return StandardsFilterAvailableObject(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsFilterAvailableObject_Impl::deepClone() const {
    StandardsFilterAvailableObject result = clone().cast<StandardsFilterAvailableObject>();
    if (m_model) { result.mf_setModel(m_model->deepClone().cast<StandardsFilterAvailableModel>()); }
    return result.cast<RulesetObject>();
  }

  std::string StandardsFilterAvailableObject_Impl::factName() const {
    return ("availableObject" + this->printUUID());
  }

  std::vector<StandardsFilterClause> StandardsFilterAvailableObject_Impl::referencedFilters() const {
    StandardsFilterClauseVector result;
    if (m_model) { result.push_back(*m_model); }
    return result;
  }

  std::string StandardsFilterAvailableObject_Impl::modelId() const {
    std::stringstream ss;
    if (m_model) {
      ss << "?" << m_model->factName() << "_Id";
    }
    else {
      ss << "?" << factName() << "_ModelId";
    }
    return ss.str();
  }

  boost::optional<StandardsFilterAvailableModel> StandardsFilterAvailableObject_Impl::model() const {
    return m_model;
  }

  standardsinterface::ObjectType StandardsFilterAvailableObject_Impl::objectType() const {
    return m_objectType;
  }

  void StandardsFilterAvailableObject_Impl::replaceReferencedFilters(
      std::vector<StandardsFilterClause>& preferredInstances)
  {
    if (m_model) {
      StandardsFilterClauseVector::const_iterator it = std::find_if(
          preferredInstances.begin(),
          preferredInstances.end(),boost::bind(
              uuidsEqual<StandardsFilterClause,StandardsFilterClause>,
              *m_model,
              _1));
      if (it != preferredInstances.end()) { 
        setModel(it->cast<StandardsFilterAvailableModel>()); 
      }
    }
  }

  void StandardsFilterAvailableObject_Impl::setModel(const boost::optional<StandardsFilterAvailableModel>& model) {
    if (m_model){ this->disconnectChild(*m_model); }
    m_model = model;
    if (m_model){ this->connectChild(*m_model); }
    this->onChange();
  }

  std::string StandardsFilterAvailableObject_Impl::print() const {
    std::stringstream ss;
    ss << "    ?" << factName() << " <- (AvailableObject ";
    unsigned indentSize(ss.str().length());
    ss << "(modelId " << modelId() << ")" << std::endl;
    ss << std::setw(indentSize) << " " << "(objectType ?" << factName() << "_ObjectType&\"" << m_objectType.valueName() << "\")" << std::endl;
    ss << std::setw(indentSize) << " " << "(objectName ?" << factName() << "_ObjectName)" << std::endl;
    ss << std::setw(indentSize) << " " << "(objectId ?" << factName() << "_ObjectId))" << std::endl;
    return ss.str();
  }

  std::string StandardsFilterAvailableObject_Impl::xmlElementName() const
  {
    return StandardsFilterAvailableObject::xmlElementName();
  }

  void StandardsFilterAvailableObject_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsFilterClause_Impl::writeValues(doc, element);
  }

  bool StandardsFilterAvailableObject_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;

    result = result && StandardsFilterClause_Impl::equivalentData(other);

    if (result){
      OptionalStandardsFilterAvailableObject oAvailableObject = other.optionalCast<StandardsFilterAvailableObject>();
      if (oAvailableObject) {
        if (m_objectType == oAvailableObject->objectType())
        {
          return ((!m_model && !oAvailableObject->model()) ||
                  (m_model && oAvailableObject->model() && m_model->equivalentData(*oAvailableObject->model())));
        }
      }
    }
    return false;
  }

} // detail

/// @cond
std::string StandardsFilterAvailableObject::xmlElementName()
{
  return "StandardsFilterAvailableObject";
}
/// @endcond

StandardsFilterAvailableObject::StandardsFilterAvailableObject(
    const StandardsFilterAvailableModel& model,
    const standardsinterface::ObjectType& objectType)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterAvailableObject_Impl>(
                     new detail::StandardsFilterAvailableObject_Impl(model,objectType)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableObject_Impl>());
}

StandardsFilterAvailableObject::StandardsFilterAvailableObject(
    const StandardsFilterAvailableModel& model,
    const standardsinterface::ObjectType& objectType,
    const UUID& uuid,
    const UUID& versionUUID)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterAvailableObject_Impl>(
                     new detail::StandardsFilterAvailableObject_Impl(model,objectType, uuid, versionUUID)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableObject_Impl>());
}

StandardsFilterAvailableObject::StandardsFilterAvailableObject(
    const standardsinterface::ObjectType& objectType)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterAvailableObject_Impl>(
                     new detail::StandardsFilterAvailableObject_Impl(objectType)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableObject_Impl>());
}

StandardsFilterAvailableObject::StandardsFilterAvailableObject(
    const standardsinterface::ObjectType& objectType,
    const UUID& uuid,
    const UUID& versionUUID)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterAvailableObject_Impl>(
                     new detail::StandardsFilterAvailableObject_Impl(objectType, uuid, versionUUID)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableObject_Impl>());
}

StandardsFilterAvailableObject::StandardsFilterAvailableObject(const QDomElement& element)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterAvailableObject_Impl>(
                     new detail::StandardsFilterAvailableObject_Impl(element)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableObject_Impl>());
}

std::string StandardsFilterAvailableObject::modelId() const {
  return getImpl<detail::StandardsFilterAvailableObject_Impl>()->modelId();
}

boost::optional<StandardsFilterAvailableModel> StandardsFilterAvailableObject::model() const {
  return getImpl<detail::StandardsFilterAvailableObject_Impl>()->model();
}

standardsinterface::ObjectType StandardsFilterAvailableObject::objectType() const {
  return getImpl<detail::StandardsFilterAvailableObject_Impl>()->objectType();
}

/// @cond
void StandardsFilterAvailableObject::mf_setModel(const boost::optional<StandardsFilterAvailableModel>& model) {
  getImpl<detail::StandardsFilterAvailableObject_Impl>()->setModel(model);
}

StandardsFilterAvailableObject::StandardsFilterAvailableObject(
    boost::shared_ptr<detail::StandardsFilterAvailableObject_Impl> impl)
  : StandardsFilterClause(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterAvailableObject_Impl>());
}
/// @endcond

} // ruleset
} // openstudio
