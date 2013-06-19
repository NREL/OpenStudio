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

#include <ruleset/StandardsActionRuleSatisfied.hpp>
#include <ruleset/StandardsActionRuleSatisfied_Impl.hpp>

#include <ruleset/StandardsRule.hpp>
#include <ruleset/StandardsFilterAvailableObject.hpp>
#include <ruleset/StandardsFilterObjectAttribute_Impl.hpp>
#include <ruleset/StandardsFilterObjectNumericalAttribute.hpp>
#include <ruleset/StandardsFilterObjectNumericalAttribute_Impl.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsActionRuleSatisfied_Impl::StandardsActionRuleSatisfied_Impl(const StandardsFilterObjectAttribute& attributeTest)
    : StandardsActionRuleResult_Impl(attributeTest)
  {}

  StandardsActionRuleSatisfied_Impl::StandardsActionRuleSatisfied_Impl(
      const StandardsFilterObjectAttribute& attributeTest,
      const UUID& uuid, 
      const UUID& versionUUID)
    : StandardsActionRuleResult_Impl(attributeTest, uuid, versionUUID)
  {}

  StandardsActionRuleSatisfied_Impl::StandardsActionRuleSatisfied_Impl(const QDomElement& element)
    : StandardsActionRuleResult_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  RulesetObject StandardsActionRuleSatisfied_Impl::clone() const {
    boost::shared_ptr<StandardsActionRuleSatisfied_Impl> impl(new StandardsActionRuleSatisfied_Impl(*this));
    return StandardsActionRuleSatisfied(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsActionRuleSatisfied_Impl::deepClone() const {
    StandardsActionRuleSatisfied result = clone().cast<StandardsActionRuleSatisfied>();
    result.mf_setAttributeTest(attributeTest().deepClone().cast<StandardsFilterObjectAttribute>());
    return result.cast<RulesetObject>();
  }

  std::string StandardsActionRuleSatisfied_Impl::print() const {
    std::stringstream ss;
    ss << "    (addTextToReport \"Pass Messages\" \"Object '\" ?" << m_attributeTest.object().factName() 
       << "_ObjectName \"' of type '" << m_attributeTest.object().objectType().valueName() 
       << "' satisfies a rule that requires " << m_attributeTest.printForMessage() 
       << ", since the attribute equals '\" ?" << m_attributeTest.factName() << "_Value \"";
    OptionalStandardsFilterObjectNumericalAttribute oNumericalTest = attributeTest().optionalCast<StandardsFilterObjectNumericalAttribute>();
    if (oNumericalTest) { ss << " " << oNumericalTest->units(); }
    ss << "'.\")" << std::endl;
    return ss.str();
  }

  std::string StandardsActionRuleSatisfied_Impl::print(const StandardsRule& rule) const {
    std::stringstream ss;
    ss << "    (addTextToReport \"Pass Messages\" \"Rule '" << rule.name() 
       << "' satisfied by object '\" ?" << m_attributeTest.object().factName() 
       << "_ObjectName \"' of type '" << m_attributeTest.object().objectType().valueName() 
       << "' , because " << m_attributeTest.printForMessage() << ", specifically the attribute "
       << "equals '\" ?" << m_attributeTest.factName() << "_Value \"";
    OptionalStandardsFilterObjectNumericalAttribute oNumericalTest = attributeTest().optionalCast<StandardsFilterObjectNumericalAttribute>();
    if (oNumericalTest) { ss << " " << oNumericalTest->units(); }
    ss << "'.\")" << std::endl;
    return ss.str();
  }

  std::string StandardsActionRuleSatisfied_Impl::xmlElementName() const
  {
    return StandardsActionRuleSatisfied::xmlElementName();
  }

  void StandardsActionRuleSatisfied_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsActionRuleResult_Impl::writeValues(doc, element);
  }

  bool StandardsActionRuleSatisfied_Impl::equivalentData(const RulesetObject& other) const {
    OptionalStandardsActionRuleSatisfied oCandidate = other.optionalCast<StandardsActionRuleSatisfied>();
    if (oCandidate) {
      return StandardsActionRuleResult_Impl::equivalentData(other);
    }
    return false;
  }

} // detail

/// @cond
std::string StandardsActionRuleSatisfied::xmlElementName()
{
  return "StandardsActionRuleSatisfied";
}
/// @endcond

StandardsActionRuleSatisfied::StandardsActionRuleSatisfied(const StandardsFilterObjectAttribute& attributeTest)
  : StandardsActionRuleResult(boost::shared_ptr<detail::StandardsActionRuleSatisfied_Impl>(
                         new detail::StandardsActionRuleSatisfied_Impl(attributeTest)))
{}

StandardsActionRuleSatisfied::StandardsActionRuleSatisfied(const StandardsFilterObjectAttribute& attributeTest,
                                                           const UUID& uuid, const UUID& versionUUID)
  : StandardsActionRuleResult(boost::shared_ptr<detail::StandardsActionRuleSatisfied_Impl>(
                         new detail::StandardsActionRuleSatisfied_Impl(attributeTest, uuid, versionUUID)))
{}

/// @cond
StandardsActionRuleSatisfied::StandardsActionRuleSatisfied(boost::shared_ptr<detail::StandardsActionRuleSatisfied_Impl> impl)
  : StandardsActionRuleResult(impl)
{}
/// @endcond

} // ruleset
} // openstudio
