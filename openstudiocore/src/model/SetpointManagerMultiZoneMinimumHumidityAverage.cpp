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

#include "SetpointManagerMultiZoneMinimumHumidityAverage.hpp"
#include "SetpointManagerMultiZoneMinimumHumidityAverage_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_MultiZone_MinimumHumidity_Average_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerMultiZoneMinimumHumidityAverage_Impl::SetpointManagerMultiZoneMinimumHumidityAverage_Impl(const IdfObject& idfObject,
                                                                                                           Model_Impl* model,
                                                                                                           bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType());
  }

  SetpointManagerMultiZoneMinimumHumidityAverage_Impl::SetpointManagerMultiZoneMinimumHumidityAverage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                           Model_Impl* model,
                                                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType());
  }

  SetpointManagerMultiZoneMinimumHumidityAverage_Impl::SetpointManagerMultiZoneMinimumHumidityAverage_Impl(const SetpointManagerMultiZoneMinimumHumidityAverage_Impl& other,
                                                                                                           Model_Impl* model,
                                                                                                           bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerMultiZoneMinimumHumidityAverage_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerMultiZoneMinimumHumidityAverage_Impl::iddObjectType() const {
    return SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType();
  }

  double SetpointManagerMultiZoneMinimumHumidityAverage_Impl::minimumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::isMinimumSetpointHumidityRatioDefaulted() const {
    return isEmpty(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio);
  }

  double SetpointManagerMultiZoneMinimumHumidityAverage_Impl::maximumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::isMaximumSetpointHumidityRatioDefaulted() const {
    return isEmpty(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio);
  }

  boost::optional<Node> SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::SetpointNodeorNodeListName);
  }

  std::string SetpointManagerMultiZoneMinimumHumidityAverage_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
    return result;
  }

  void SetpointManagerMultiZoneMinimumHumidityAverage_Impl::resetMinimumSetpointHumidityRatio() {
    bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MinimumSetpointHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
    return result;
  }

  void SetpointManagerMultiZoneMinimumHumidityAverage_Impl::resetMaximumSetpointHumidityRatio() {
    bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::MaximumSetpointHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerMultiZoneMinimumHumidityAverage_Impl::resetSetpointNode() {
    bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

  bool SetpointManagerMultiZoneMinimumHumidityAverage_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable, controlVariable);
    return result;
  }

} // detail

SetpointManagerMultiZoneMinimumHumidityAverage::SetpointManagerMultiZoneMinimumHumidityAverage(const Model& model)
  : SetpointManager(SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>());

  bool ok = setControlVariable("MinimumHumidityRatio");
  OS_ASSERT(ok);
}

IddObjectType SetpointManagerMultiZoneMinimumHumidityAverage::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_MultiZone_MinimumHumidity_Average);
}

std::vector<std::string> SetpointManagerMultiZoneMinimumHumidityAverage::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_MultiZone_MinimumHumidity_AverageFields::ControlVariable);
}

double SetpointManagerMultiZoneMinimumHumidityAverage::minimumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->minimumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::isMinimumSetpointHumidityRatioDefaulted() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->isMinimumSetpointHumidityRatioDefaulted();
}

double SetpointManagerMultiZoneMinimumHumidityAverage::maximumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->maximumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::isMaximumSetpointHumidityRatioDefaulted() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->isMaximumSetpointHumidityRatioDefaulted();
}

boost::optional<Node> SetpointManagerMultiZoneMinimumHumidityAverage::setpointNode() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setpointNode();
}

std::string SetpointManagerMultiZoneMinimumHumidityAverage::controlVariable() const {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->controlVariable();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
}

void SetpointManagerMultiZoneMinimumHumidityAverage::resetMinimumSetpointHumidityRatio() {
  getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->resetMinimumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
}

void SetpointManagerMultiZoneMinimumHumidityAverage::resetMaximumSetpointHumidityRatio() {
  getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->resetMaximumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneMinimumHumidityAverage::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl>()->setControlVariable(controlVariable);
}

/// @cond
SetpointManagerMultiZoneMinimumHumidityAverage::SetpointManagerMultiZoneMinimumHumidityAverage(std::shared_ptr<detail::SetpointManagerMultiZoneMinimumHumidityAverage_Impl> impl)
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

