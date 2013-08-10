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

#include <ruleset/ActionClause.hpp>
#include <ruleset/ActionClause_Impl.hpp>

#include <ruleset/FilterClause.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  ActionClause_Impl::ActionClause_Impl()
    : Clause_Impl()
  {}

  ActionClause_Impl::ActionClause_Impl(const UUID& uuid, const UUID& versionUUID)
    : Clause_Impl(uuid, versionUUID)
  {}

  ActionClause_Impl::ActionClause_Impl(const QDomElement& element)
    : Clause_Impl(element)
  {}

  void ActionClause_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    Clause_Impl::writeValues(doc, element);
  }

  bool ActionClause_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && Clause_Impl::equivalentData(other);
    return result;
  }

} // detail

/// @cond
ActionClause::ActionClause(boost::shared_ptr<detail::ActionClause_Impl> impl)
  : Clause(impl)
{
  OS_ASSERT(getImpl<detail::ActionClause_Impl>());
}
/// @endcond

} // ruleset
} // openstudio
