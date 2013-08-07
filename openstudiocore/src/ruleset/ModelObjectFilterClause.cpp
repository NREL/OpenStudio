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

#include <ruleset/ModelObjectFilterClause.hpp>
#include <ruleset/ModelObjectFilterClause_Impl.hpp>

#include <model/ModelObject.hpp>

#include <QDomDocument>
#include <QDomElement>

namespace openstudio {
namespace ruleset {

namespace detail {

  ModelObjectFilterClause_Impl::ModelObjectFilterClause_Impl()
    : FilterClause_Impl()
  {}

  ModelObjectFilterClause_Impl::ModelObjectFilterClause_Impl(const UUID& uuid, const UUID& versionUUID)
    : FilterClause_Impl(uuid, versionUUID)
  {}

  ModelObjectFilterClause_Impl::ModelObjectFilterClause_Impl(const QDomElement& element)
    : FilterClause_Impl(element)
  {}

  void ModelObjectFilterClause_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    FilterClause_Impl::writeValues(doc, element);
  }

  bool ModelObjectFilterClause_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && FilterClause_Impl::equivalentData(other);
    return result;
  }

} // detail

/// @cond
ModelObjectFilterClause::ModelObjectFilterClause(boost::shared_ptr<detail::ModelObjectFilterClause_Impl> impl)
  : FilterClause(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectFilterClause_Impl>());
}
/// @endcond

bool ModelObjectFilterClause::check(model::ModelObject& modelObject) const
{
  return getImpl<detail::ModelObjectFilterClause_Impl>()->check(modelObject);
}

} // ruleset
} // openstudio
