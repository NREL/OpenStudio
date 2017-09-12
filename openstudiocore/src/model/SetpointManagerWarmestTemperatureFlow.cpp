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

#include "SetpointManagerWarmestTemperatureFlow.hpp"
#include "SetpointManagerWarmestTemperatureFlow_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_WarmestTemperatureFlow_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerWarmestTemperatureFlow_Impl::SetpointManagerWarmestTemperatureFlow_Impl(const IdfObject& idfObject,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerWarmestTemperatureFlow::iddObjectType());
  }

  SetpointManagerWarmestTemperatureFlow_Impl::SetpointManagerWarmestTemperatureFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerWarmestTemperatureFlow::iddObjectType());
  }

  SetpointManagerWarmestTemperatureFlow_Impl::SetpointManagerWarmestTemperatureFlow_Impl(const SetpointManagerWarmestTemperatureFlow_Impl& other,
                                                                                         Model_Impl* model,
                                                                                         bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerWarmestTemperatureFlow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerWarmestTemperatureFlow_Impl::iddObjectType() const {
    return SetpointManagerWarmestTemperatureFlow::iddObjectType();
  }

  std::string SetpointManagerWarmestTemperatureFlow_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_WarmestTemperatureFlowFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerWarmestTemperatureFlow_Impl::minimumSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MinimumSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerWarmestTemperatureFlow_Impl::maximumSetpointTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MaximumSetpointTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  std::string SetpointManagerWarmestTemperatureFlow_Impl::strategy() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_WarmestTemperatureFlowFields::Strategy,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerWarmestTemperatureFlow_Impl::minimumTurndownRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MinimumTurndownRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerWarmestTemperatureFlow_Impl::setControlVariable(const std::string & controlVariable) {
    bool result = setString(OS_SetpointManager_WarmestTemperatureFlowFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerWarmestTemperatureFlow_Impl::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MinimumSetpointTemperature, minimumSetpointTemperature);
    return result;
  }

  bool SetpointManagerWarmestTemperatureFlow_Impl::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
    bool result = setDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MaximumSetpointTemperature, maximumSetpointTemperature);
    return result;
  }

  bool SetpointManagerWarmestTemperatureFlow_Impl::setStrategy(std::string strategy) {
    bool result = setString(OS_SetpointManager_WarmestTemperatureFlowFields::Strategy, strategy);
    return result;
  }

  bool SetpointManagerWarmestTemperatureFlow_Impl::setMinimumTurndownRatio(double minimumTurndownRatio) {
    bool result = setDouble(OS_SetpointManager_WarmestTemperatureFlowFields::MinimumTurndownRatio, minimumTurndownRatio);
    return result;
  }

  //std::string SetpointManagerWarmestTemperatureFlow_Impl::controlVariable() const {
  //}

  //bool SetpointManagerWarmestTemperatureFlow_Impl::setControlVariable( const std::string & value ) {
  //}

  boost::optional<Node> SetpointManagerWarmestTemperatureFlow_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_WarmestTemperatureFlowFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerWarmestTemperatureFlow_Impl::setSetpointNode( const Node & node ) {
    bool result = setPointer(OS_SetpointManager_WarmestTemperatureFlowFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerWarmestTemperatureFlow_Impl::resetSetpointNode() {
    setString(OS_SetpointManager_WarmestTemperatureFlowFields::SetpointNodeorNodeListName,"");
  }

} // detail

SetpointManagerWarmestTemperatureFlow::SetpointManagerWarmestTemperatureFlow(const Model& model)
  : SetpointManager(SetpointManagerWarmestTemperatureFlow::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>());

  setControlVariable("Temperature");
  setMinimumSetpointTemperature(12.0);
  setMaximumSetpointTemperature(18.0);
  setStrategy("TemperatureFirst");
  setMinimumTurndownRatio(0.2);
}

IddObjectType SetpointManagerWarmestTemperatureFlow::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_WarmestTemperatureFlow);
}

std::vector<std::string> SetpointManagerWarmestTemperatureFlow::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_WarmestTemperatureFlowFields::ControlVariable);
}

std::vector<std::string> SetpointManagerWarmestTemperatureFlow::strategyValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_WarmestTemperatureFlowFields::Strategy);
}

std::string SetpointManagerWarmestTemperatureFlow::controlVariable() const {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->controlVariable();
}

double SetpointManagerWarmestTemperatureFlow::minimumSetpointTemperature() const {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->minimumSetpointTemperature();
}

double SetpointManagerWarmestTemperatureFlow::maximumSetpointTemperature() const {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->maximumSetpointTemperature();
}

std::string SetpointManagerWarmestTemperatureFlow::strategy() const {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->strategy();
}

double SetpointManagerWarmestTemperatureFlow::minimumTurndownRatio() const {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->minimumTurndownRatio();
}

bool SetpointManagerWarmestTemperatureFlow::setControlVariable(const std::string & controlVariable) {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerWarmestTemperatureFlow::setMinimumSetpointTemperature(double minimumSetpointTemperature) {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setMinimumSetpointTemperature(minimumSetpointTemperature);
}

bool SetpointManagerWarmestTemperatureFlow::setMaximumSetpointTemperature(double maximumSetpointTemperature) {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setMaximumSetpointTemperature(maximumSetpointTemperature);
}

bool SetpointManagerWarmestTemperatureFlow::setStrategy(std::string strategy) {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setStrategy(strategy);
}

bool SetpointManagerWarmestTemperatureFlow::setMinimumTurndownRatio(double minimumTurndownRatio) {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setMinimumTurndownRatio(minimumTurndownRatio);
}

boost::optional<Node> SetpointManagerWarmestTemperatureFlow::setpointNode() const {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setpointNode();
}

bool SetpointManagerWarmestTemperatureFlow::setSetpointNode( const Node & node ) {
  return getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->setSetpointNode(node);
}

void SetpointManagerWarmestTemperatureFlow::resetSetpointNode() {
  getImpl<detail::SetpointManagerWarmestTemperatureFlow_Impl>()->resetSetpointNode();
}

/// @cond
SetpointManagerWarmestTemperatureFlow::SetpointManagerWarmestTemperatureFlow(std::shared_ptr<detail::SetpointManagerWarmestTemperatureFlow_Impl> impl)
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

