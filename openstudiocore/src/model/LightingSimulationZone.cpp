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

#include "LightingSimulationZone.hpp"
#include "LightingSimulationZone_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "RenderingColor.hpp"
#include "RenderingColor_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include "ModelExtensibleGroup.hpp"
#include <utilities/idd/OS_LightingSimulationZone_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  LightingSimulationZone_Impl::LightingSimulationZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == LightingSimulationZone::iddObjectType());
  }

  LightingSimulationZone_Impl::LightingSimulationZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == LightingSimulationZone::iddObjectType());
  }

  LightingSimulationZone_Impl::LightingSimulationZone_Impl(const LightingSimulationZone_Impl& other,
                                                           Model_Impl* model,
                                                           bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& LightingSimulationZone_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType LightingSimulationZone_Impl::iddObjectType() const {
    return LightingSimulationZone::iddObjectType();
  }

  boost::optional<RenderingColor> LightingSimulationZone_Impl::renderingColor() const
  {
    return getObject<ModelObject>().getModelObjectTarget<RenderingColor>(OS_LightingSimulationZoneFields::GroupRenderingName);
  }

  bool LightingSimulationZone_Impl::setRenderingColor(const RenderingColor& renderingColor)
  {
    return setPointer(OS_LightingSimulationZoneFields::GroupRenderingName, renderingColor.handle());
  }

  void LightingSimulationZone_Impl::resetRenderingColor()
  {
    setString(OS_LightingSimulationZoneFields::GroupRenderingName, "");
  }

  std::vector<Space> LightingSimulationZone_Impl::spaces() const
  {
    std::vector<Space> result;
    
    for (const ModelExtensibleGroup& group : castVector<ModelExtensibleGroup>(extensibleGroups())) 
    {        
      boost::optional<Space> space = group.getModelObjectTarget<Space>(0);
      OS_ASSERT(space);
      result.push_back(*space);
    }

    return result;
  }

  bool LightingSimulationZone_Impl::addSpace(const Space& space)
  {
    boost::optional<std::string> spaceName = space.name();
    if (!spaceName){
      return false;
    }
    std::vector<std::string> values;
    values.push_back(space.name().get());
    IdfExtensibleGroup group = pushExtensibleGroup(values);

    return (!group.empty());
  }

  void LightingSimulationZone_Impl::resetSpaces()
  {
    clearExtensibleGroups();
    OS_ASSERT(numExtensibleGroups() == 0);
  }

} // detail

LightingSimulationZone::LightingSimulationZone(const Model& model)
  : ModelObject(LightingSimulationZone::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::LightingSimulationZone_Impl>());
}

IddObjectType LightingSimulationZone::iddObjectType() {
  IddObjectType result(IddObjectType::OS_LightingSimulationZone);
  return result;
}

boost::optional<RenderingColor> LightingSimulationZone::renderingColor() const
{
  return getImpl<detail::LightingSimulationZone_Impl>()->renderingColor();
}

bool LightingSimulationZone::setRenderingColor(const RenderingColor& renderingColor)
{
  return getImpl<detail::LightingSimulationZone_Impl>()->setRenderingColor(renderingColor);
}

void LightingSimulationZone::resetRenderingColor()
{
  getImpl<detail::LightingSimulationZone_Impl>()->resetRenderingColor();
}

std::vector<Space> LightingSimulationZone::spaces() const
{
  return getImpl<detail::LightingSimulationZone_Impl>()->spaces();
}

bool LightingSimulationZone::addSpace(const Space& space)
{
  return getImpl<detail::LightingSimulationZone_Impl>()->addSpace(space);
}

void LightingSimulationZone::resetSpaces()
{
  getImpl<detail::LightingSimulationZone_Impl>()->resetSpaces();
}

/// @cond
LightingSimulationZone::LightingSimulationZone(std::shared_ptr<detail::LightingSimulationZone_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond


} // model
} // openstudio

