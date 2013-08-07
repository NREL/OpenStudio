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

#include <ruleset/ModelObjectActionClause.hpp>
#include <ruleset/ModelObjectActionClause_Impl.hpp>

#include <model/ModelObject.hpp>

namespace openstudio {
namespace ruleset {

namespace detail {

  ModelObjectActionClause_Impl::ModelObjectActionClause_Impl()
    : ActionClause_Impl()
  {
  }

  ModelObjectActionClause_Impl::ModelObjectActionClause_Impl(const UUID& uuid, const UUID& versionUUID)
    : ActionClause_Impl(uuid, versionUUID)
  {
  }

  ModelObjectActionClause_Impl::ModelObjectActionClause_Impl(const QDomElement& element)
    : ActionClause_Impl(element)
  {
  }

  void ModelObjectActionClause_Impl::writeValues(QDomDocument& doc, QDomElement& element) const
  {
    ActionClause_Impl::writeValues(doc, element);
  }

  bool ModelObjectActionClause_Impl::equivalentData(const RulesetObject& other) const {
    bool result = true;
    result = result && ActionClause_Impl::equivalentData(other);
    return result;
  }

} // detail

/// @cond
ModelObjectActionClause::ModelObjectActionClause(boost::shared_ptr<detail::ModelObjectActionClause_Impl> impl)
  : ActionClause(impl)
{
  OS_ASSERT(getImpl<detail::ModelObjectActionClause_Impl>());
}
/// @endcond

bool ModelObjectActionClause::apply(openstudio::model::ModelObject& modelObject) const
{
  return getImpl<detail::ModelObjectActionClause_Impl>()->apply(modelObject);
}


} // ruleset
} // openstudio
