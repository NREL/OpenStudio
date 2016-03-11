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

#include "SetpointManagerFollowGroundTemperature.hpp"
#include "SetpointManagerFollowGroundTemperature_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_FollowGroundTemperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerFollowGroundTemperature_Impl::SetpointManagerFollowGroundTemperature_Impl(const IdfObject& idfObject,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerFollowGroundTemperature::iddObjectType());
  }

  SetpointManagerFollowGroundTemperature_Impl::SetpointManagerFollowGroundTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerFollowGroundTemperature::iddObjectType());
  }

  SetpointManagerFollowGroundTemperature_Impl::SetpointManagerFollowGroundTemperature_Impl(const SetpointManagerFollowGroundTemperature_Impl& other,
                                                                                           Model_Impl* model,
                                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerFollowGroundTemperature_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerFollowGroundTemperature_Impl::iddObjectType() const {
    return SetpointManagerFollowGroundTemperature::iddObjectType();
  }

  bool SetpointManagerFollowGroundTemperature_Impl::addToNode(Node & node) {
    bool added = SetpointManager_Impl::addToNode( node );
    if( added ) {
      return added;
    } else if( boost::optional<PlantLoop> plantLoop = node.plantLoop() ) {
      return this->setSetpointNode(node);
    }
    return added;
  }

  std::string SetpointManagerFollowGroundTemperature_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_FollowGroundTemperatureFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerFollowGroundTemperature_Impl::offsetTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_FollowGroundTemperatureFields::OffsetTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerFollowGroundTemperature_Impl::maximumSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_FollowGroundTemperatureFields::MaximumSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerFollowGroundTemperature_Impl::minimumSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_FollowGroundTemperatureFields::MinimumSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> SetpointManagerFollowGroundTemperature_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_FollowGroundTemperatureFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerFollowGroundTemperature_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_FollowGroundTemperatureFields::ControlVariable, controlVariable);
    return result;
  }

  void SetpointManagerFollowGroundTemperature_Impl::setOffsetTemperatureDifference(double offsetTemperatureDifference) {
    bool result = setDouble(OS_SetpointManager_FollowGroundTemperatureFields::OffsetTemperatureDifference, offsetTemperatureDifference);
    OS_ASSERT(result);
  }

  void SetpointManagerFollowGroundTemperature_Impl::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_FollowGroundTemperatureFields::MaximumSetpointTemperature, maximumSetpointTemperature);
    OS_ASSERT(result);
  }

  void SetpointManagerFollowGroundTemperature_Impl::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_FollowGroundTemperatureFields::MinimumSetpointTemperature, minimumSetpointTemperature);
    OS_ASSERT(result);
  }

  bool SetpointManagerFollowGroundTemperature_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_FollowGroundTemperatureFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerFollowGroundTemperature_Impl::resetSetpointNode()
  {
    bool result = setString(OS_SetpointManager_FollowGroundTemperatureFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerFollowGroundTemperature::SetpointManagerFollowGroundTemperature(const Model& model)
  : SetpointManager(SetpointManagerFollowGroundTemperature::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>());

  setControlVariable("Temperature");
  setOffsetTemperatureDifference(0);
  setMaximumSetpointTemperature(80);
  setMinimumSetpointTemperature(10);
}

IddObjectType SetpointManagerFollowGroundTemperature::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_FollowGroundTemperature);
}

std::vector<std::string> SetpointManagerFollowGroundTemperature::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_FollowGroundTemperatureFields::ControlVariable);
}

std::string SetpointManagerFollowGroundTemperature::controlVariable() const {
  return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->controlVariable();
}

double SetpointManagerFollowGroundTemperature::offsetTemperatureDifference() const {
  return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->offsetTemperatureDifference();
}

double SetpointManagerFollowGroundTemperature::maximumSetpointTemperature() const {
  return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->maximumSetpointTemperature();
}

double SetpointManagerFollowGroundTemperature::minimumSetpointTemperature() const {
  return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->minimumSetpointTemperature();
}

boost::optional<Node> SetpointManagerFollowGroundTemperature::setpointNode() const {
  return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->setpointNode();
}

bool SetpointManagerFollowGroundTemperature::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->setControlVariable(controlVariable);
}

void SetpointManagerFollowGroundTemperature::setOffsetTemperatureDifference(double offsetTemperatureDifference) {
  getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->setOffsetTemperatureDifference(offsetTemperatureDifference);
}

void SetpointManagerFollowGroundTemperature::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
  getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->setMaximumSetpointTemperature(maximumSetpointTemperature);
}

void SetpointManagerFollowGroundTemperature::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
  getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->setMinimumSetpointTemperature(minimumSetpointTemperature);
}

/// @cond
SetpointManagerFollowGroundTemperature::SetpointManagerFollowGroundTemperature(std::shared_ptr<detail::SetpointManagerFollowGroundTemperature_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

