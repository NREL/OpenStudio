/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "BuildingUnit.hpp"
#include "BuildingUnit_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "RenderingColor.hpp"
#include "RenderingColor_Impl.hpp"

#include <utilities/idd/OS_BuildingUnit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {
namespace model {
namespace detail {

BuildingUnit_Impl::BuildingUnit_Impl(const IdfObject &idfObject, Model_Impl *model, bool keepHandle)
  : ModelObject_Impl(idfObject, model, keepHandle)
{
    OS_ASSERT(idfObject.iddObject().type() == BuildingUnit::iddObjectType());
}

BuildingUnit_Impl::BuildingUnit_Impl(const openstudio::detail::WorkspaceObject_Impl &other,
                                     Model_Impl *model,
                                     bool keepHandle)
  : ModelObject_Impl(other, model, keepHandle)
{
    OS_ASSERT(other.iddObject().type() == BuildingUnit::iddObjectType());
}

BuildingUnit_Impl::BuildingUnit_Impl(const BuildingUnit_Impl &other,
                                     Model_Impl *model,
                                     bool keepHandle)
  : ModelObject_Impl(other, model, keepHandle)
{}

const std::vector<std::string>& BuildingUnit_Impl::outputVariableNames() const
{
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
}

IddObjectType BuildingUnit_Impl::iddObjectType() const {
    return BuildingUnit::iddObjectType();
}

boost::optional<RenderingColor> BuildingUnit_Impl::renderingColor() const
{
    return getObject<ModelObject>().getModelObjectTarget<RenderingColor>(OS_BuildingUnitFields::RenderingColor);
}

bool BuildingUnit_Impl::setRenderingColor(const RenderingColor& renderingColor)
{
    return setPointer(OS_BuildingUnitFields::RenderingColor, renderingColor.handle());
}

void BuildingUnit_Impl::resetRenderingColor()
{
    setString(OS_BuildingUnitFields::RenderingColor, "");
}

} //detail

BuildingUnit::BuildingUnit(const Model &model)
  : ModelObject(BuildingUnit::iddObjectType(), model)
{
    OS_ASSERT(getImpl<detail::BuildingUnit_Impl>());
}

IddObjectType BuildingUnit::iddObjectType() {
    IddObjectType result(IddObjectType::OS_BuildingUnit);
    return result;
}

boost::optional<RenderingColor> BuildingUnit::renderingColor() const
{
    return getImpl<detail::BuildingUnit_Impl>()->renderingColor();
}

bool BuildingUnit::setRenderingColor(const RenderingColor& renderingColor)
{
    return getImpl<detail::BuildingUnit_Impl>()->setRenderingColor(renderingColor);
}

void BuildingUnit::resetRenderingColor()
{
    getImpl<detail::BuildingUnit_Impl>()->resetRenderingColor();
}

/// @cond
BuildingUnit::BuildingUnit(std::shared_ptr<detail::BuildingUnit_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} //model
} //openstudio