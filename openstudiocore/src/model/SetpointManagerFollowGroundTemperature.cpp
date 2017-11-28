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

  std::string SetpointManagerFollowGroundTemperature_Impl::referenceGroundTemperatureObjectType() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_FollowGroundTemperatureFields::ReferenceGroundTemperatureObjectType,true);
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

  bool SetpointManagerFollowGroundTemperature_Impl::setReferenceGroundTemperatureObjectType(const std::string& groundTemperatureObjType) {
    bool result = setString(OS_SetpointManager_FollowGroundTemperatureFields::ReferenceGroundTemperatureObjectType, groundTemperatureObjType);
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

std::vector<std::string> SetpointManagerFollowGroundTemperature::referenceGroundTemperatureObjectTypeValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_FollowGroundTemperatureFields::ReferenceGroundTemperatureObjectType);
}

std::string SetpointManagerFollowGroundTemperature::controlVariable() const {
  return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->controlVariable();
}

std::string SetpointManagerFollowGroundTemperature::referenceGroundTemperatureObjectType() const {
  return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->referenceGroundTemperatureObjectType();
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

bool SetpointManagerFollowGroundTemperature::setReferenceGroundTemperatureObjectType(const std::string& groundTemperatureObjType) {
  return getImpl<detail::SetpointManagerFollowGroundTemperature_Impl>()->setReferenceGroundTemperatureObjectType(groundTemperatureObjType);
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
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

