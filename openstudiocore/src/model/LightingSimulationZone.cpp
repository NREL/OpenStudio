/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
  : ModelObject(std::move(impl))
{}
/// @endcond


} // model
} // openstudio

