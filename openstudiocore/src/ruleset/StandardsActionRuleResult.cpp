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

#include <ruleset/StandardsActionRuleResult.hpp>
#include <ruleset/StandardsActionRuleResult_Impl.hpp>

#include <ruleset/StandardsFilterAvailableObject.hpp>
#include <ruleset/StandardsFilterObjectAttribute.hpp>
#include <ruleset/StandardsFilterObjectAttribute_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Compare.hpp>

#include <QDomDocument>
#include <QDomElement>

#include <boost/bind.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsActionRuleResult_Impl::StandardsActionRuleResult_Impl(
      const StandardsFilterObjectAttribute& attributeTest)
    : StandardsActionClause_Impl(), m_attributeTest(attributeTest)
  {
    this->connectChild(attributeTest);
    mf_updateImpliedFilters();
  }

  StandardsActionRuleResult_Impl::StandardsActionRuleResult_Impl(
      const StandardsFilterObjectAttribute& attributeTest,
      const UUID& uuid, 
      const UUID& versionUUID)
    : StandardsActionClause_Impl(uuid, versionUUID), m_attributeTest(attributeTest)
  {
    this->connectChild(attributeTest);
    mf_updateImpliedFilters();
  }

  StandardsActionRuleResult_Impl::StandardsActionRuleResult_Impl(const QDomElement& element)
    : StandardsActionClause_Impl(element),
      m_attributeTest(Clause::factoryFromXml(element.firstChildElement(QString::fromStdString("AttributeTest")))->cast<StandardsFilterObjectAttribute>())
  {
    BOOST_ASSERT(!element.isNull());
  }

  std::vector<StandardsFilterClause> StandardsActionRuleResult_Impl::referencedFilters() const {
    StandardsFilterClauseVector result;
    result.push_back(m_attributeTest);
    return result;
  }

  std::vector<StandardsFilterClause> StandardsActionRuleResult_Impl::impliedFilters() const {
    StandardsFilterClauseVector result;
    if (m_model) { result.push_back(*m_model); }
    return result;
  }

  std::string StandardsActionRuleResult_Impl::modelFactName() const {
    if (m_model) { return m_model->factName(); }

    BOOST_ASSERT(m_attributeTest.object().model());
    return m_attributeTest.object().model()->factName();
  }

  StandardsFilterObjectAttribute StandardsActionRuleResult_Impl::attributeTest() const {
    return m_attributeTest;
  }

  void StandardsActionRuleResult_Impl::replaceReferencedFilters(
      const std::vector<StandardsFilterClause>& preferredInstances)
  {
    StandardsFilterClauseVector::const_iterator it = std::find_if(
        preferredInstances.begin(),
        preferredInstances.end(),boost::bind(
            uuidsEqual<StandardsFilterClause,StandardsFilterClause>,
            m_attributeTest,
            _1));
    if (it != preferredInstances.end()) { 
      setAttributeTest(it->cast<StandardsFilterObjectAttribute>()); 
    }
  }

  void StandardsActionRuleResult_Impl::setAttributeTest(const StandardsFilterObjectAttribute& filter) {
    this->disconnectChild(m_attributeTest);
    m_attributeTest = filter;
    this->connectChild(m_attributeTest);
    mf_updateImpliedFilters();
    this->onChange();
  }

  void StandardsActionRuleResult_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsActionClause_Impl::writeValues(doc, element);
  }

  bool StandardsActionRuleResult_Impl::equivalentData(const RulesetObject& other) const {
    // should be called from derived classes only ...
    StandardsActionRuleResult otherRuleResult = other.cast<StandardsActionRuleResult>();
    return m_attributeTest.equivalentData(otherRuleResult.attributeTest());
  }

  void StandardsActionRuleResult_Impl::mf_updateImpliedFilters() {
    if (m_model) { this->disconnectChild(*m_model); }

    if (!m_attributeTest.object().model()) {
      m_model = StandardsFilterAvailableModel(m_attributeTest.object().modelId());
      this->connectChild(*m_model);
    }
  }

} // detail

StandardsFilterObjectAttribute StandardsActionRuleResult::attributeTest() const {
  return getImpl<detail::StandardsActionRuleResult_Impl>()->attributeTest();
}

std::string StandardsActionRuleResult::modelFactName() const {
  return getImpl<detail::StandardsActionRuleResult_Impl>()->modelFactName();
}

/// @cond
void StandardsActionRuleResult::mf_setAttributeTest(const StandardsFilterObjectAttribute& filter) {
  getImpl<detail::StandardsActionRuleResult_Impl>()->setAttributeTest(filter);
}

StandardsActionRuleResult::StandardsActionRuleResult(boost::shared_ptr<detail::StandardsActionRuleResult_Impl> impl)
  : StandardsActionClause(impl)
{}
/// @endcond

} // ruleset
} // openstudio
