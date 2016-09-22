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
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/OS_BuildingUnit_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

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
    bool ok = setString(OS_BuildingUnitFields::RenderingColor, "");
    OS_ASSERT(ok);
  }

  boost::optional<std::string> BuildingUnit_Impl::buildingUnitType() const
  {
    return getString(OS_BuildingUnitFields::BuildingUnitType, true, true);
  }

  bool BuildingUnit_Impl::setBuildingUnitType(const std::string& buildingUnitType)
  {
    bool ok = setString(OS_BuildingUnitFields::BuildingUnitType, buildingUnitType);
    return ok;
  }

  void BuildingUnit_Impl::resetBuildingUnitType()
  {
    bool ok = setString(OS_BuildingUnitFields::BuildingUnitType, "");
    OS_ASSERT(ok);
  }

  std::vector<Space> BuildingUnit_Impl::spaces() const
  {
    return getObject<ModelObject>().getModelObjectSources<Space>(Space::iddObjectType());
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

std::vector<std::string> BuildingUnit::buildingUnitTypeValues()
{
  return getIddKeyNames(
    IddFactory::instance().getObject(iddObjectType()).get(),
    OS_BuildingUnitFields::BuildingUnitType
  );
}

boost::optional<std::string> BuildingUnit::buildingUnitType() const
{
  return getImpl<detail::BuildingUnit_Impl>()->buildingUnitType();
}

bool BuildingUnit::setBuildingUnitType(const std::string& buildingUnitType)
{
  return getImpl<detail::BuildingUnit_Impl>()->setBuildingUnitType(buildingUnitType);
}

void BuildingUnit::resetBuildingUnitType()
{
  getImpl<detail::BuildingUnit_Impl>()->resetBuildingUnitType();
}

std::vector<Space> BuildingUnit::spaces() const
{
  return getImpl<detail::BuildingUnit_Impl>()->spaces();
}

/// @cond
BuildingUnit::BuildingUnit(std::shared_ptr<detail::BuildingUnit_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} //model
} //openstudio