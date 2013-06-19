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

#include <ruleset/StandardsFilterRulesetOption.hpp>
#include <ruleset/StandardsFilterRulesetOption_Impl.hpp>
#include <ruleset/StandardsRulesetOption_Impl.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsFilterRulesetOption_Impl::StandardsFilterRulesetOption_Impl(
      const StandardsRulesetOption& option)
    : StandardsFilterClause_Impl(), m_option(option)
  {
    this->connectChild(m_option);
  }

  StandardsFilterRulesetOption_Impl::StandardsFilterRulesetOption_Impl(
      const StandardsRulesetOption& option,
      const UUID& uuid, 
      const UUID& versionUUID)
    : StandardsFilterClause_Impl(uuid, versionUUID), m_option(option)
  {
    this->connectChild(m_option);
  }

  StandardsFilterRulesetOption_Impl::StandardsFilterRulesetOption_Impl(const QDomElement& element)
    : StandardsFilterClause_Impl(element),
      m_option(StandardsRulesetOption(element.firstChildElement(QString::fromStdString("Option"))))
  {
    BOOST_ASSERT(!element.isNull());
    BOOST_ASSERT(element.tagName() == toQString(this->xmlElementName()));
    this->connectChild(m_option);
  }

  RulesetObject StandardsFilterRulesetOption_Impl::clone() const {
    boost::shared_ptr<StandardsFilterRulesetOption_Impl> impl(new StandardsFilterRulesetOption_Impl(*this));
    return StandardsFilterRulesetOption(impl).cast<RulesetObject>();
  }

  RulesetObject StandardsFilterRulesetOption_Impl::deepClone() const {
    StandardsFilterRulesetOption result = clone().cast<StandardsFilterRulesetOption>();
    RulesetOption rulesetOption = this->option().clone();
    result.setOption(rulesetOption.cast<StandardsRulesetOption>());
    return result.cast<RulesetObject>();
  }

  std::string StandardsFilterRulesetOption_Impl::factName() const {
    return (m_option.name() + this->printUUID());
  }

  StandardsRulesetOption StandardsFilterRulesetOption_Impl::option() const {
    return m_option;
  }

  void StandardsFilterRulesetOption_Impl::setOption(const StandardsRulesetOption& option) {
    this->disconnectChild(m_option);
    m_option = option;
    this->connectChild(m_option);
    this->onChange();
  }

  std::string StandardsFilterRulesetOption_Impl::print() const {
    std::stringstream ss;
    ss << "    ?" << factName() << " <- ";
    ss << m_option.printValue();
    return ss.str();
  }

  std::string StandardsFilterRulesetOption_Impl::xmlElementName() const
  {
    return StandardsFilterRulesetOption::xmlElementName();
  }

  void StandardsFilterRulesetOption_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    StandardsFilterClause_Impl::writeValues(doc, element);
  }

  bool StandardsFilterRulesetOption_Impl::equivalentData(const RulesetObject& other) const {
    OptionalStandardsFilterRulesetOption oRulesetOption = other.optionalCast<StandardsFilterRulesetOption>();
    if (oRulesetOption) {
      return m_option.equivalentData(oRulesetOption->option());
    }
    return false;
  }

} // detail

/// @cond
std::string StandardsFilterRulesetOption::xmlElementName()
{
  return "StandardsFilterRulesetOption";
}
/// @endcond

StandardsFilterRulesetOption::StandardsFilterRulesetOption(const StandardsRulesetOption& option)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterRulesetOption_Impl>(new
                     detail::StandardsFilterRulesetOption_Impl(option)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterRulesetOption_Impl>());
}

StandardsFilterRulesetOption::StandardsFilterRulesetOption(const StandardsRulesetOption& option,
                                                           const UUID& uuid, const UUID& versionUUID)
  : StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterRulesetOption_Impl>(new
                     detail::StandardsFilterRulesetOption_Impl(option, uuid, versionUUID)))
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterRulesetOption_Impl>());
}

StandardsRulesetOption StandardsFilterRulesetOption::option() const {
  return getImpl<detail::StandardsFilterRulesetOption_Impl>()->option();
}

void StandardsFilterRulesetOption::setOption(const StandardsRulesetOption& option) {
  getImpl<detail::StandardsFilterRulesetOption_Impl>()->setOption(option);
}

/// @cond
StandardsFilterRulesetOption::StandardsFilterRulesetOption(
    boost::shared_ptr<detail::StandardsFilterRulesetOption_Impl> impl)
  : StandardsFilterClause(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterRulesetOption_Impl>());
}
/// @endcond

} // ruleset
} // openstudio
