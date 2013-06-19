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

#include <ruleset/RulesetOption.hpp>
#include <ruleset/RulesetOption_Impl.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  RulesetOption_Impl::RulesetOption_Impl()
    : RulesetObject_Impl()
  {
  }

  RulesetOption_Impl::RulesetOption_Impl(const UUID& uuid, const UUID& versionUUID)
    : RulesetObject_Impl(uuid, versionUUID)
  {
  }

  bool RulesetOption_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && RulesetObject_Impl::equivalentData(other);
    return result;
  }

} // detail

RulesetOption::RulesetOption(const QDomElement& element)
  : RulesetObject(boost::shared_ptr<detail::RulesetOption_Impl>())
{
  BOOST_ASSERT(getImpl<detail::RulesetOption_Impl>());
}

/// @cond
RulesetOption::RulesetOption(const boost::shared_ptr<detail::RulesetOption_Impl>& impl)
  : RulesetObject(impl)
{
  BOOST_ASSERT(getImpl<detail::RulesetOption_Impl>());
}
/// @endcond

RulesetOption RulesetOption::clone() const {
  return this->getImpl<detail::RulesetOption_Impl>()->clone();
}

} // ruleset
} // openstudio
