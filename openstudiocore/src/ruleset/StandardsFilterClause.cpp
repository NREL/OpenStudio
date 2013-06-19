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

#include <ruleset/StandardsFilterClause.hpp>
#include <ruleset/StandardsFilterClause_Impl.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsFilterClause_Impl::StandardsFilterClause_Impl()
    : FilterClause_Impl()
  {
  }

  StandardsFilterClause_Impl::StandardsFilterClause_Impl(const UUID& uuid, const UUID& versionUUID)
    : FilterClause_Impl(uuid, versionUUID)
  {
  }

 StandardsFilterClause_Impl::StandardsFilterClause_Impl(const QDomElement& element)
    : FilterClause_Impl(element)
  {
  }

  std::vector<StandardsFilterClause> StandardsFilterClause_Impl::referencedFilters() const {
    return StandardsFilterClauseVector();
  }

  std::vector<StandardsFilterClause> StandardsFilterClause_Impl::impliedFilters() const {
    return StandardsFilterClauseVector();
  }

  void StandardsFilterClause_Impl::replaceReferencedFilters(
      std::vector<StandardsFilterClause>& preferredInstances)
  {
  }

  std::string StandardsFilterClause_Impl::printRetractAction() const {
    std::stringstream ss;
    ss << "    (retract ?" << factName() << ")" << std::endl;
    return ss.str();
  }

  void StandardsFilterClause_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    FilterClause_Impl::writeValues(doc, element);
  }

  bool StandardsFilterClause_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && FilterClause_Impl::equivalentData(other);
    return result;
  }

} // detail

RulesetObject StandardsFilterClause::clone() const {
  return getImpl<detail::StandardsFilterClause_Impl>()->clone();
}

RulesetObject StandardsFilterClause::deepClone() const {
  return getImpl<detail::StandardsFilterClause_Impl>()->deepClone();
}

std::vector<StandardsFilterClause> StandardsFilterClause::referencedFilters() const {
  return getImpl<detail::StandardsFilterClause_Impl>()->referencedFilters();
}

std::vector<StandardsFilterClause> StandardsFilterClause::impliedFilters() const {
  return getImpl<detail::StandardsFilterClause_Impl>()->impliedFilters();
}

std::string StandardsFilterClause::factName() const {
  return getImpl<detail::StandardsFilterClause_Impl>()->factName();
}

void StandardsFilterClause::replaceReferencedFilters(
    std::vector<StandardsFilterClause>& preferredInstances)
{
  getImpl<detail::StandardsFilterClause_Impl>()->replaceReferencedFilters(preferredInstances);
}

std::string StandardsFilterClause::printRetractAction() const {
  return getImpl<detail::StandardsFilterClause_Impl>()->printRetractAction();
}

std::string StandardsFilterClause::print() const {
  return getImpl<detail::StandardsFilterClause_Impl>()->print();
}

/// @cond
StandardsFilterClause::StandardsFilterClause(boost::shared_ptr<detail::StandardsFilterClause_Impl> impl)
  : FilterClause(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsFilterClause_Impl>());
}
/// @endcond

} // ruleset
} // openstudio
