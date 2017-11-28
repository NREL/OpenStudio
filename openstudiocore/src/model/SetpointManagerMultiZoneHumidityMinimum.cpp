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

#include "SetpointManagerMultiZoneHumidityMinimum.hpp"
#include "SetpointManagerMultiZoneHumidityMinimum_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_MultiZone_Humidity_Minimum_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerMultiZoneHumidityMinimum_Impl::SetpointManagerMultiZoneHumidityMinimum_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneHumidityMinimum::iddObjectType());
  }

  SetpointManagerMultiZoneHumidityMinimum_Impl::SetpointManagerMultiZoneHumidityMinimum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneHumidityMinimum::iddObjectType());
  }

  SetpointManagerMultiZoneHumidityMinimum_Impl::SetpointManagerMultiZoneHumidityMinimum_Impl(const SetpointManagerMultiZoneHumidityMinimum_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerMultiZoneHumidityMinimum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerMultiZoneHumidityMinimum_Impl::iddObjectType() const {
    return SetpointManagerMultiZoneHumidityMinimum::iddObjectType();
  }

  double SetpointManagerMultiZoneHumidityMinimum_Impl::minimumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::isMinimumSetpointHumidityRatioDefaulted() const {
    return isEmpty(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio);
  }

  double SetpointManagerMultiZoneHumidityMinimum_Impl::maximumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::isMaximumSetpointHumidityRatioDefaulted() const {
    return isEmpty(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio);
  }

  boost::optional<Node> SetpointManagerMultiZoneHumidityMinimum_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_Humidity_MinimumFields::SetpointNodeorNodeListName);
  }

  std::string SetpointManagerMultiZoneHumidityMinimum_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
    return result;
  }

  void SetpointManagerMultiZoneHumidityMinimum_Impl::resetMinimumSetpointHumidityRatio() {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MinimumSetpointHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
    return result;
  }

  void SetpointManagerMultiZoneHumidityMinimum_Impl::resetMaximumSetpointHumidityRatio() {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::MaximumSetpointHumidityRatio, "");
    OS_ASSERT(result);
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_MultiZone_Humidity_MinimumFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerMultiZoneHumidityMinimum_Impl::resetSetpointNode() {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

  bool SetpointManagerMultiZoneHumidityMinimum_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable, controlVariable);
    return result;
  }

} // detail

SetpointManagerMultiZoneHumidityMinimum::SetpointManagerMultiZoneHumidityMinimum(const Model& model)
  : SetpointManager(SetpointManagerMultiZoneHumidityMinimum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>());

  bool ok = setControlVariable("MinimumHumidityRatio");
  OS_ASSERT(ok);
}

IddObjectType SetpointManagerMultiZoneHumidityMinimum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_MultiZone_Humidity_Minimum);
}

std::vector<std::string> SetpointManagerMultiZoneHumidityMinimum::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_MultiZone_Humidity_MinimumFields::ControlVariable);
}

double SetpointManagerMultiZoneHumidityMinimum::minimumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->minimumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneHumidityMinimum::isMinimumSetpointHumidityRatioDefaulted() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->isMinimumSetpointHumidityRatioDefaulted();
}

double SetpointManagerMultiZoneHumidityMinimum::maximumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->maximumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneHumidityMinimum::isMaximumSetpointHumidityRatioDefaulted() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->isMaximumSetpointHumidityRatioDefaulted();
}

boost::optional<Node> SetpointManagerMultiZoneHumidityMinimum::setpointNode() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setpointNode();
}

std::string SetpointManagerMultiZoneHumidityMinimum::controlVariable() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->controlVariable();
}

bool SetpointManagerMultiZoneHumidityMinimum::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
}

void SetpointManagerMultiZoneHumidityMinimum::resetMinimumSetpointHumidityRatio() {
  getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->resetMinimumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneHumidityMinimum::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
}

void SetpointManagerMultiZoneHumidityMinimum::resetMaximumSetpointHumidityRatio() {
  getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->resetMaximumSetpointHumidityRatio();
}

bool SetpointManagerMultiZoneHumidityMinimum::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMinimum_Impl>()->setControlVariable(controlVariable);
}

/// @cond
SetpointManagerMultiZoneHumidityMinimum::SetpointManagerMultiZoneHumidityMinimum(std::shared_ptr<detail::SetpointManagerMultiZoneHumidityMinimum_Impl> impl)
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

