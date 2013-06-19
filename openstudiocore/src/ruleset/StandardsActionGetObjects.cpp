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

#include <ruleset/StandardsActionGetObjects.hpp>
#include <ruleset/StandardsActionGetObjects_Impl.hpp>

#include <ruleset/StandardsFilterAvailableModel_Impl.hpp>
#include <ruleset/StandardsFilterAvailableObject.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  // IMPL

  StandardsActionGetObjects_Impl::StandardsActionGetObjects_Impl(
      const StandardsFilterAvailableObject& object)
    : StandardsActionClause_Impl(), m_model(object.model()), m_objectType(object.objectType())
  {
    if (m_model) { this->connectChild(*m_model); }
  }

  StandardsActionGetObjects_Impl::StandardsActionGetObjects_Impl(
      const boost::optional<StandardsFilterAvailableModel>& model,
      const standardsinterface::ObjectType& objectType,
      const UUID& uuid, 
      const UUID& versionUUID)
    : StandardsActionClause_Impl(uuid, versionUUID), m_model(model), m_objectType(objectType)
  {
    if (m_model) { this->connectChild(*m_model); }
  }

  StandardsActionGetObjects_Impl::StandardsActionGetObjects_Impl(const QDomElement& element)
    : StandardsActionClause_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  RulesetObject StandardsActionGetObjects_Impl::clone() const {
    boost::shared_ptr<StandardsActionGetObjects_Impl> impl(new StandardsActionGetObjects_Impl(*this));
    return StandardsActionGetObjects(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsActionGetObjects_Impl::deepClone() const {
    StandardsActionGetObjects result = clone().cast<StandardsActionGetObjects>();
    if (m_model) { result.mf_setModel(m_model->deepClone().cast<StandardsFilterAvailableModel>()); }
    return result.cast<RulesetObject>();
  }

  std::vector<StandardsFilterClause> StandardsActionGetObjects_Impl::referencedFilters() const {
    StandardsFilterClauseVector result;
    if (m_model) { result.push_back(*m_model); }
    return result;
  }

  standardsinterface::ObjectType StandardsActionGetObjects_Impl::objectType() const {
    return m_objectType;
  }

  void StandardsActionGetObjects_Impl::setModel(const boost::optional<StandardsFilterAvailableModel>& model) {
    if (m_model){
      this->disconnectChild(*m_model);
    }
    m_model = model;
    if (m_model){
      this->connectChild(*m_model);
    }
    this->onChange();
  }

  std::string StandardsActionGetObjects_Impl::print() const {
    std::stringstream ss;
    if (m_model) {
      ss << "    (assert (GetObjects (modelId ?" << m_model->factName() << "_Id)" << std::endl;
    }
    else {
      ss << "    (assert (GetObjects (modelId \"\")" << std::endl; // get objectType for all models
    }
    ss << "                        (objectType \"" << m_objectType.valueName() << "\")))" << std::endl;
    return ss.str();
  }

  std::string StandardsActionGetObjects_Impl::xmlElementName() const
  {
    return StandardsActionGetObjects::xmlElementName();
  }

  void StandardsActionGetObjects_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsActionClause_Impl::writeValues(doc, element);
  }

  bool StandardsActionGetObjects_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && StandardsActionClause_Impl::equivalentData(other);
    
    if (result){
      OptionalStandardsActionGetObjects oGetObjects = other.optionalCast<StandardsActionGetObjects>();
      if (oGetObjects) {
        if (m_objectType == oGetObjects->objectType()) {
          OptionalStandardsFilterAvailableModel otherModel = oGetObjects->getImpl<StandardsActionGetObjects_Impl>()->m_model;
          return ((!m_model && !otherModel) || 
                  (m_model && otherModel && m_model->equivalentData(*otherModel)));
        }        
      }
    }

    return false;
  }

  // OBJECT

  std::string StandardsActionGetObjects::xmlElementName()
  {
    return "StandardsActionGetObjects";
  }

  StandardsActionGetObjects::StandardsActionGetObjects(const StandardsFilterAvailableObject& object)
    : StandardsActionClause(boost::shared_ptr<StandardsActionGetObjects_Impl>(
                                new StandardsActionGetObjects_Impl(object)))
  {}

  StandardsActionGetObjects::StandardsActionGetObjects(
      const boost::optional<StandardsFilterAvailableModel>& model,
      const standardsinterface::ObjectType& objectType,
      const UUID& uuid,
      const UUID& versionUUID)
    : StandardsActionClause(boost::shared_ptr<StandardsActionGetObjects_Impl>(
                                new StandardsActionGetObjects_Impl(model, objectType, uuid, versionUUID)))
  {}

  standardsinterface::ObjectType StandardsActionGetObjects::objectType() const {
    return getImpl<StandardsActionGetObjects_Impl>()->objectType();
  }

  void StandardsActionGetObjects::mf_setModel(const boost::optional<StandardsFilterAvailableModel>& model) {
    getImpl<StandardsActionGetObjects_Impl>()->setModel(model);
  }

  StandardsActionGetObjects::StandardsActionGetObjects(boost::shared_ptr<detail::StandardsActionGetObjects_Impl> impl)
    : StandardsActionClause(impl)
  {}

} // detail

} // ruleset
} // openstudio
