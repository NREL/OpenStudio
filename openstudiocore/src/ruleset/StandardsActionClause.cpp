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

#include <ruleset/StandardsActionClause.hpp>
#include <ruleset/StandardsActionClause_Impl.hpp>

#include <ruleset/StandardsFilterClause.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  StandardsActionClause_Impl::StandardsActionClause_Impl()
    : ActionClause_Impl()
  {}

  StandardsActionClause_Impl::StandardsActionClause_Impl(const UUID& uuid, const UUID& versionUUID)
    : ActionClause_Impl(uuid, versionUUID)
  {}

  StandardsActionClause_Impl::StandardsActionClause_Impl(const QDomElement& element)
    : ActionClause_Impl(element)
  {
    BOOST_ASSERT(!element.isNull());
  }

  std::vector<StandardsFilterClause> StandardsActionClause_Impl::referencedFilters() const {
    return StandardsFilterClauseVector();
  }

  std::vector<StandardsFilterClause> StandardsActionClause_Impl::impliedFilters() const {
    return StandardsFilterClauseVector();
  }

  void StandardsActionClause_Impl::replaceReferencedFilters(
      const std::vector<StandardsFilterClause>& preferredInstances) 
  {
  }

  std::string StandardsActionClause_Impl::print(const StandardsRule& rule) const {
    // default implementation
    return this->print();
  }

  void StandardsActionClause_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    ActionClause_Impl::writeValues(doc, element);
  }

  bool StandardsActionClause_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && ActionClause_Impl::equivalentData(other);
    return result;
  }

} // detail

/// @cond
StandardsActionClause::StandardsActionClause(boost::shared_ptr<detail::StandardsActionClause_Impl> impl)
  : ActionClause(impl)
{
  BOOST_ASSERT(getImpl<detail::StandardsActionClause_Impl>());
}
/// @endcond

RulesetObject StandardsActionClause::clone() const {
  return getImpl<detail::StandardsActionClause_Impl>()->clone();
}

RulesetObject StandardsActionClause::deepClone() const {
  return getImpl<detail::StandardsActionClause_Impl>()->deepClone();
}

std::vector<StandardsFilterClause> StandardsActionClause::referencedFilters() const {
  return getImpl<detail::StandardsActionClause_Impl>()->referencedFilters();
}

std::vector<StandardsFilterClause> StandardsActionClause::impliedFilters() const {
  return getImpl<detail::StandardsActionClause_Impl>()->impliedFilters();
}

void StandardsActionClause::replaceReferencedFilters(
    const std::vector<StandardsFilterClause>& preferredInstances) 
{
  getImpl<detail::StandardsActionClause_Impl>()->replaceReferencedFilters(preferredInstances);
}

std::string StandardsActionClause::print() const {
  return getImpl<detail::StandardsActionClause_Impl>()->print();
}

std::string StandardsActionClause::print(const StandardsRule& rule) const {
  return getImpl<detail::StandardsActionClause_Impl>()->print(rule);
}

} // ruleset
} // openstudio
