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

#include "SetpointManagerFollowSystemNodeTemperature.hpp"
#include "SetpointManagerFollowSystemNodeTemperature_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"
#include "Model.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_FollowSystemNodeTemperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerFollowSystemNodeTemperature_Impl::SetpointManagerFollowSystemNodeTemperature_Impl(const IdfObject& idfObject,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerFollowSystemNodeTemperature::iddObjectType());
  }

  SetpointManagerFollowSystemNodeTemperature_Impl::SetpointManagerFollowSystemNodeTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerFollowSystemNodeTemperature::iddObjectType());
  }

  SetpointManagerFollowSystemNodeTemperature_Impl::SetpointManagerFollowSystemNodeTemperature_Impl(const SetpointManagerFollowSystemNodeTemperature_Impl& other,
                                                                                                   Model_Impl* model,
                                                                                                   bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerFollowSystemNodeTemperature_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerFollowSystemNodeTemperature_Impl::iddObjectType() const {
    return SetpointManagerFollowSystemNodeTemperature::iddObjectType();
  }

  bool SetpointManagerFollowSystemNodeTemperature_Impl::addToNode(Node & node) {
    bool added = SetpointManager_Impl::addToNode( node );
    if( added ) {
      return added;
    } else if( boost::optional<PlantLoop> plantLoop = node.plantLoop() ) {
      return this->setSetpointNode(node);
    }
    return added;
  }

  ModelObject SetpointManagerFollowSystemNodeTemperature_Impl::clone(Model model) const
  {
    SetpointManagerFollowSystemNodeTemperature clonedObject = SetpointManager_Impl::clone( model ).cast<SetpointManagerFollowSystemNodeTemperature>();
    clonedObject.resetReferenceNode();
    return clonedObject;
  }

  std::string SetpointManagerFollowSystemNodeTemperature_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_FollowSystemNodeTemperatureFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> SetpointManagerFollowSystemNodeTemperature_Impl::referenceNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_FollowSystemNodeTemperatureFields::ReferenceNodeName);
  }

  std::string SetpointManagerFollowSystemNodeTemperature_Impl::referenceTemperatureType() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_FollowSystemNodeTemperatureFields::ReferenceTemperatureType,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerFollowSystemNodeTemperature_Impl::offsetTemperatureDifference() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_FollowSystemNodeTemperatureFields::OffsetTemperatureDifference,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerFollowSystemNodeTemperature_Impl::maximumLimitSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_FollowSystemNodeTemperatureFields::MaximumLimitSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerFollowSystemNodeTemperature_Impl::minimumLimitSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_FollowSystemNodeTemperatureFields::MinimumLimitSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> SetpointManagerFollowSystemNodeTemperature_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_FollowSystemNodeTemperatureFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerFollowSystemNodeTemperature_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_FollowSystemNodeTemperatureFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerFollowSystemNodeTemperature_Impl::setReferenceNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_FollowSystemNodeTemperatureFields::ReferenceNodeName, node.handle());
    return result;
  }

  void SetpointManagerFollowSystemNodeTemperature_Impl::resetReferenceNode() {
    bool result = setString(OS_SetpointManager_FollowSystemNodeTemperatureFields::ReferenceNodeName, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerFollowSystemNodeTemperature_Impl::setReferenceTemperatureType(const std::string& referenceTemperatureType) {
    bool result = setString(OS_SetpointManager_FollowSystemNodeTemperatureFields::ReferenceTemperatureType, referenceTemperatureType);
    return result;
  }

  void SetpointManagerFollowSystemNodeTemperature_Impl::setOffsetTemperatureDifference(double offsetTemperatureDifference) {
    bool result = setDouble(OS_SetpointManager_FollowSystemNodeTemperatureFields::OffsetTemperatureDifference, offsetTemperatureDifference);
    OS_ASSERT(result);
  }

  void SetpointManagerFollowSystemNodeTemperature_Impl::setMaximumLimitSetpointTemperature(double maximumLimitSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_FollowSystemNodeTemperatureFields::MaximumLimitSetpointTemperature, maximumLimitSetpointTemperature);
    OS_ASSERT(result);
  }

  void SetpointManagerFollowSystemNodeTemperature_Impl::setMinimumLimitSetpointTemperature(double minimumLimitSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_FollowSystemNodeTemperatureFields::MinimumLimitSetpointTemperature, minimumLimitSetpointTemperature);
    OS_ASSERT(result);
  }

  bool SetpointManagerFollowSystemNodeTemperature_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_FollowSystemNodeTemperatureFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerFollowSystemNodeTemperature_Impl::resetSetpointNode()
  {
    bool result = setString(OS_SetpointManager_FollowSystemNodeTemperatureFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerFollowSystemNodeTemperature::SetpointManagerFollowSystemNodeTemperature(const Model& model)
  : SetpointManager(SetpointManagerFollowSystemNodeTemperature::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>());

  setReferenceTemperatureType("NodeDryBulb");
  setControlVariable("Temperature");
  setOffsetTemperatureDifference(0);
  setMaximumLimitSetpointTemperature(100);
  setMinimumLimitSetpointTemperature(0);
}

IddObjectType SetpointManagerFollowSystemNodeTemperature::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_FollowSystemNodeTemperature);
}

std::vector<std::string> SetpointManagerFollowSystemNodeTemperature::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_FollowSystemNodeTemperatureFields::ControlVariable);
}

std::vector<std::string> SetpointManagerFollowSystemNodeTemperature::referenceTemperatureTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_FollowSystemNodeTemperatureFields::ReferenceTemperatureType);
}

std::string SetpointManagerFollowSystemNodeTemperature::controlVariable() const {
  return getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->controlVariable();
}

boost::optional<Node> SetpointManagerFollowSystemNodeTemperature::referenceNode() const {
  return getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->referenceNode();
}

std::string SetpointManagerFollowSystemNodeTemperature::referenceTemperatureType() const {
  return getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->referenceTemperatureType();
}

double SetpointManagerFollowSystemNodeTemperature::offsetTemperatureDifference() const {
  return getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->offsetTemperatureDifference();
}

double SetpointManagerFollowSystemNodeTemperature::maximumLimitSetpointTemperature() const {
  return getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->maximumLimitSetpointTemperature();
}

double SetpointManagerFollowSystemNodeTemperature::minimumLimitSetpointTemperature() const {
  return getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->minimumLimitSetpointTemperature();
}

boost::optional<Node> SetpointManagerFollowSystemNodeTemperature::setpointNode() const {
  return getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->setpointNode();
}

bool SetpointManagerFollowSystemNodeTemperature::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerFollowSystemNodeTemperature::setReferenceNode(const Node& node) {
  return getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->setReferenceNode(node);
}

void SetpointManagerFollowSystemNodeTemperature::resetReferenceNode() {
  getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->resetReferenceNode();
}

bool SetpointManagerFollowSystemNodeTemperature::setReferenceTemperatureType(const std::string& referenceTemperatureType) {
  return getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->setReferenceTemperatureType(referenceTemperatureType);
}

void SetpointManagerFollowSystemNodeTemperature::setOffsetTemperatureDifference(double offsetTemperatureDifference) {
  getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->setOffsetTemperatureDifference(offsetTemperatureDifference);
}

void SetpointManagerFollowSystemNodeTemperature::setMaximumLimitSetpointTemperature(double maximumLimitSetpointTemperature) {
  getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->setMaximumLimitSetpointTemperature(maximumLimitSetpointTemperature);
}

void SetpointManagerFollowSystemNodeTemperature::setMinimumLimitSetpointTemperature(double minimumLimitSetpointTemperature) {
  getImpl<detail::SetpointManagerFollowSystemNodeTemperature_Impl>()->setMinimumLimitSetpointTemperature(minimumLimitSetpointTemperature);
}

/// @cond
SetpointManagerFollowSystemNodeTemperature::SetpointManagerFollowSystemNodeTemperature(std::shared_ptr<detail::SetpointManagerFollowSystemNodeTemperature_Impl> impl)
  : SetpointManager(impl)
{}
/// @endcond

} // model
} // openstudio

