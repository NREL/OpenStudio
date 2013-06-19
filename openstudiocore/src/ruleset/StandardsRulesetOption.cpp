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

#include <ruleset/StandardsRulesetOption.hpp>
#include <ruleset/StandardsRulesetOption_Impl.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsRulesetOption_Impl::StandardsRulesetOption_Impl()
    : RulesetOption_Impl()
  {
  }

  StandardsRulesetOption_Impl::StandardsRulesetOption_Impl(
      const UUID& uuid, const UUID& versionUUID)
    : RulesetOption_Impl(uuid, versionUUID)
  {
  }

  bool StandardsRulesetOption_Impl::equivalentData(const RulesetObject& other) const {
    // assume called from derived class
    return RulesetOption_Impl::equivalentData(other);
  }

  bool StandardsRulesetOption_Impl::equivalentOptionType(const StandardsRulesetOption& other) const
  {
    // assume called from derived class
    return true;
  }

} // detail

StandardsRulesetOption::StandardsRulesetOption(const QDomElement& element)
  : RulesetOption(boost::shared_ptr<detail::StandardsRulesetOption_Impl>())
{
  BOOST_ASSERT(getImpl<detail::StandardsRulesetOption_Impl>());
}

/// @cond
StandardsRulesetOption::StandardsRulesetOption(const boost::shared_ptr<detail::StandardsRulesetOption_Impl>& impl)
  : RulesetOption(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsRulesetOption_Impl>());
}
/// @endcond

std::string StandardsRulesetOption::name() const {
  return this->getImpl<detail::StandardsRulesetOption_Impl>()->name();
}

std::string StandardsRulesetOption::factName() const {
  return this->getImpl<detail::StandardsRulesetOption_Impl>()->factName();
}

std::string StandardsRulesetOption::printDefinition() const {
  return this->getImpl<detail::StandardsRulesetOption_Impl>()->printDefinition();
}

std::string StandardsRulesetOption::printValue() const {
  return this->getImpl<detail::StandardsRulesetOption_Impl>()->printValue();
}

bool StandardsRulesetOption::equivalentOptionType(const StandardsRulesetOption& other) const {
  return getImpl<detail::StandardsRulesetOption_Impl>()->equivalentOptionType(other);
}

std::ostream& operator<<(std::ostream& os, const StandardsRulesetOption& option) {
  os << option.printDefinition() << std::endl << option.printValue();
  return os;
}

} // ruleset
} // openstudio
