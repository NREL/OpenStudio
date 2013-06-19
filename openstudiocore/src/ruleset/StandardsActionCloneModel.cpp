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

#include <ruleset/StandardsActionCloneModel.hpp>
#include <ruleset/StandardsActionCloneModel_Impl.hpp>

#include <ruleset/StandardsFilterAvailableModel_Impl.hpp>

#include <utilities/core/Compare.hpp>

#include <QDomDocument>
#include <QDomElement>

#include <boost/bind.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsActionCloneModel_Impl::StandardsActionCloneModel_Impl(const StandardsFilterAvailableModel& modelToClone,
                                                                 const StandardsRulesetModelType& modelTypeOfClone)
    : StandardsActionClause_Impl(), m_modelToClone(modelToClone), m_modelTypeOfClone(modelTypeOfClone)
  {
    this->connectChild(m_modelToClone);
  }

  StandardsActionCloneModel_Impl::StandardsActionCloneModel_Impl(const StandardsFilterAvailableModel& modelToClone,
                                                                 const StandardsRulesetModelType& modelTypeOfClone,
                                                                 const UUID& uuid, const UUID& versionUUID)
    : StandardsActionClause_Impl(uuid, versionUUID), m_modelToClone(modelToClone), m_modelTypeOfClone(modelTypeOfClone)
  {
    this->connectChild(m_modelToClone);
  }

  StandardsActionCloneModel_Impl::StandardsActionCloneModel_Impl(const QDomElement& element)
    : StandardsActionClause_Impl(element),
      m_modelToClone(StandardsFilterAvailableModel(element.firstChildElement(QString::fromStdString("ModelToClone"))))
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  RulesetObject StandardsActionCloneModel_Impl::clone() const {
    boost::shared_ptr<StandardsActionCloneModel_Impl> impl(new StandardsActionCloneModel_Impl(*this));
    return StandardsActionCloneModel(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsActionCloneModel_Impl::deepClone() const {
    StandardsActionCloneModel result = clone().cast<StandardsActionCloneModel>();
    result.mf_setModelToClone(m_modelToClone.deepClone().cast<StandardsFilterAvailableModel>());
    return result.cast<RulesetObject>();
  }

  std::vector<StandardsFilterClause> StandardsActionCloneModel_Impl::referencedFilters() const {
    StandardsFilterClauseVector result;
    result.push_back(m_modelToClone);
    return result;
  }

  StandardsFilterAvailableModel StandardsActionCloneModel_Impl::modelToClone() const {
    return m_modelToClone;
  }

  StandardsRulesetModelType StandardsActionCloneModel_Impl::modelTypeOfClone() const {
    return m_modelTypeOfClone;
  }

  void StandardsActionCloneModel_Impl::replaceReferencedFilters(
      const std::vector<StandardsFilterClause>& preferredInstances)
  {
    StandardsFilterClauseVector::const_iterator it = std::find_if(
        preferredInstances.begin(),
        preferredInstances.end(),boost::bind(
            uuidsEqual<StandardsFilterClause,StandardsFilterClause>,
            m_modelToClone,
            _1));
    if (it != preferredInstances.end()) { 
      setModelToClone(it->cast<StandardsFilterAvailableModel>()); 
    }
  }

  void StandardsActionCloneModel_Impl::setModelToClone(const StandardsFilterAvailableModel& filter) {
    m_modelToClone = filter;
    this->connectChild(m_modelToClone);
    this->onChange();
  }

  void StandardsActionCloneModel_Impl::setModelTypeOfClone(const StandardsRulesetModelType& value) {
    m_modelTypeOfClone = value;
    this->onChange();
  }

  std::string StandardsActionCloneModel_Impl::print() const {
    std::stringstream ss;
    ss << "    (bind ?clonedModel" << this->printUUID() << "_Id (cloneModel ?" << m_modelToClone.factName() << "_Name ?"
       << m_modelToClone.factName() << "_Id " << m_modelTypeOfClone.valueName()
       << "))" << std::endl;
    ss << "    (assert (AvailableModel (name ?" << m_modelToClone.factName() << "_Name)" << std::endl;
    ss << "                            (id ?clonedModel" << this->printUUID() << "_Id)" << std::endl;
    ss << "                            (modelType " << m_modelTypeOfClone.valueName() << ")))" << std::endl;
    return ss.str();
  }

  std::string StandardsActionCloneModel_Impl::xmlElementName() const
  {
    return StandardsActionCloneModel::xmlElementName();
  }

  void StandardsActionCloneModel_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsActionClause_Impl::writeValues(doc, element);
  }

  bool StandardsActionCloneModel_Impl::equivalentData(const RulesetObject& other) const {
    OptionalStandardsActionCloneModel oActionCloneModel = other.optionalCast<StandardsActionCloneModel>();
    if (oActionCloneModel) {
      if (modelTypeOfClone() == oActionCloneModel->modelTypeOfClone()) {
        return modelToClone().equivalentData(oActionCloneModel->modelToClone());
      }
    }
    return false;
  }

} // detail

/// @cond
std::string StandardsActionCloneModel::xmlElementName()
{
  return "StandardsActionCloneModel";
}
/// @endcond

StandardsActionCloneModel::StandardsActionCloneModel(const StandardsFilterAvailableModel& modelToClone,
                                                     const StandardsRulesetModelType& modelTypeOfClone)
  : StandardsActionClause(boost::shared_ptr<detail::StandardsActionCloneModel_Impl>(
                     new detail::StandardsActionCloneModel_Impl(modelToClone,modelTypeOfClone)))
{
  BOOST_ASSERT(getImpl<detail::StandardsActionCloneModel_Impl>());
}

StandardsActionCloneModel::StandardsActionCloneModel(const StandardsFilterAvailableModel& modelToClone,
                                                     const StandardsRulesetModelType& modelTypeOfClone,
                                                     const UUID& uuid,
                                                     const UUID& versionUUID)
  : StandardsActionClause(boost::shared_ptr<detail::StandardsActionCloneModel_Impl>(
                     new detail::StandardsActionCloneModel_Impl(modelToClone,modelTypeOfClone,uuid,versionUUID)))
{
  BOOST_ASSERT(getImpl<detail::StandardsActionCloneModel_Impl>());
}

StandardsFilterAvailableModel StandardsActionCloneModel::modelToClone() const {
  return getImpl<detail::StandardsActionCloneModel_Impl>()->modelToClone();
}

StandardsRulesetModelType StandardsActionCloneModel::modelTypeOfClone() const {
  return getImpl<detail::StandardsActionCloneModel_Impl>()->modelTypeOfClone();
}

void StandardsActionCloneModel::setModelTypeOfClone(const StandardsRulesetModelType& value) {
  getImpl<detail::StandardsActionCloneModel_Impl>()->setModelTypeOfClone(value);
}

/// @cond
void StandardsActionCloneModel::mf_setModelToClone(const StandardsFilterAvailableModel& filter) {
  return getImpl<detail::StandardsActionCloneModel_Impl>()->setModelToClone(filter);
}

StandardsActionCloneModel::StandardsActionCloneModel(boost::shared_ptr<detail::StandardsActionCloneModel_Impl> impl)
  : StandardsActionClause(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionCloneModel_Impl>());
}
/// @endcond

} // ruleset
} // openstudio
