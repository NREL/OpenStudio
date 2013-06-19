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

#include <ruleset/StandardsActionRuleViolated.hpp>
#include <ruleset/StandardsActionRuleViolated_Impl.hpp>

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

  StandardsActionRuleViolated_Impl::StandardsActionRuleViolated_Impl(
      const StandardsFilterObjectAttribute& attributeTest)
    : StandardsActionRuleResult_Impl(attributeTest)
  {}

  StandardsActionRuleViolated_Impl::StandardsActionRuleViolated_Impl(
      const StandardsFilterObjectAttribute& attributeTest,
      const UUID& uuid, 
      const UUID& versionUUID)
    : StandardsActionRuleResult_Impl(attributeTest, uuid, versionUUID)
  {}

  StandardsActionRuleViolated_Impl::StandardsActionRuleViolated_Impl(const QDomElement& element)
    : StandardsActionRuleResult_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
  }

  RulesetObject StandardsActionRuleViolated_Impl::clone() const {
    boost::shared_ptr<StandardsActionRuleViolated_Impl> impl(new StandardsActionRuleViolated_Impl(*this));
    return StandardsActionRuleViolated(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsActionRuleViolated_Impl::deepClone() const {
    StandardsActionRuleViolated result = clone().cast<StandardsActionRuleViolated>();
    result.mf_setAttributeTest(attributeTest().deepClone().cast<StandardsFilterObjectAttribute>());
    return result.cast<RulesetObject>();
  }

  std::string StandardsActionRuleViolated_Impl::print() const {
    std::stringstream ss;
    StandardsFilterObjectAttribute negatedAttributeTest = attributeTest().clone().cast<StandardsFilterObjectAttribute>();
    negatedAttributeTest.negate();
    ss << "    (setOverallResultFlag \"Fail\")" << std::endl;
    ss << "    (addTextToReport \"Fail Messages\" \"Object '\" ?" << m_attributeTest.object().factName() 
       << "_ObjectName \"' of type '" << m_attributeTest.object().objectType().valueName() 
       << "' violates a rule that requires " << negatedAttributeTest.printForMessage()
       << ", since the attribute equals '\" ?" << m_attributeTest.factName() << "_Value \"";
    OptionalStandardsFilterObjectNumericalAttribute oNumericalTest = attributeTest().optionalCast<StandardsFilterObjectNumericalAttribute>();
    if (oNumericalTest) { ss << " " << oNumericalTest->units(); }
    ss << "'.\")" << std::endl;
    return ss.str();
  }

  std::string StandardsActionRuleViolated_Impl::print(const StandardsRule& rule) const {
    std::stringstream ss;
    StandardsFilterObjectAttribute negatedAttributeTest = attributeTest().clone().cast<StandardsFilterObjectAttribute>();
    negatedAttributeTest.negate();
    ss << "    (setOverallResultFlag \"Fail\")" << std::endl;
    ss << "    (addTextToReport \"Fail Messages\" \"Rule '" << rule.name() 
       << "' is violated by object '\" ?" << m_attributeTest.object().factName() 
       << "_ObjectName \"' of type '" << m_attributeTest.object().objectType().valueName() 
       << "' , because the rule requires " << negatedAttributeTest.printForMessage()
       << ", but the attribute equals '\" ?" << m_attributeTest.factName() << "_Value \"";
    OptionalStandardsFilterObjectNumericalAttribute oNumericalTest = attributeTest().optionalCast<StandardsFilterObjectNumericalAttribute>();
    if (oNumericalTest) { ss << " " << oNumericalTest->units(); }
    ss << "'.\")" << std::endl;
    return ss.str();
  }

  std::string StandardsActionRuleViolated_Impl::xmlElementName() const
  {
    return StandardsActionRuleViolated::xmlElementName();
  }

  void StandardsActionRuleViolated_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsActionRuleResult_Impl::writeValues(doc, element);
  }

  bool StandardsActionRuleViolated_Impl::equivalentData(const RulesetObject& other) const {
    OptionalStandardsActionRuleViolated oCandidate = other.optionalCast<StandardsActionRuleViolated>();
    if (oCandidate) {
      return StandardsActionRuleResult_Impl::equivalentData(other);
    }
    return false;
  }

} // detail

/// @cond
std::string StandardsActionRuleViolated::xmlElementName()
{
  return "StandardsActionRuleViolated";
}
/// @endcond

StandardsActionRuleViolated::StandardsActionRuleViolated(const StandardsFilterObjectAttribute& attributeTest)
  : StandardsActionRuleResult(boost::shared_ptr<detail::StandardsActionRuleViolated_Impl>(
                         new detail::StandardsActionRuleViolated_Impl(attributeTest)))
{}

StandardsActionRuleViolated::StandardsActionRuleViolated(const StandardsFilterObjectAttribute& attributeTest,
                                                         const UUID& uuid, const UUID& versionUUID)
  : StandardsActionRuleResult(boost::shared_ptr<detail::StandardsActionRuleViolated_Impl>(
                         new detail::StandardsActionRuleViolated_Impl(attributeTest, uuid, versionUUID)))
{}

/// @cond
StandardsActionRuleViolated::StandardsActionRuleViolated(boost::shared_ptr<detail::StandardsActionRuleViolated_Impl> impl)
  : StandardsActionRuleResult(impl)
{}
/// @endcond

} // ruleset
} // openstudio
