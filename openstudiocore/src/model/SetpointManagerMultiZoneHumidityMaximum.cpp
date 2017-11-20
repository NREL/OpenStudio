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

#include "SetpointManagerMultiZoneHumidityMaximum.hpp"
#include "SetpointManagerMultiZoneHumidityMaximum_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_SetpointManager_MultiZone_Humidity_Maximum_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerMultiZoneHumidityMaximum_Impl::SetpointManagerMultiZoneHumidityMaximum_Impl(const IdfObject& idfObject,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMultiZoneHumidityMaximum::iddObjectType());
  }

  SetpointManagerMultiZoneHumidityMaximum_Impl::SetpointManagerMultiZoneHumidityMaximum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerMultiZoneHumidityMaximum::iddObjectType());
  }

  SetpointManagerMultiZoneHumidityMaximum_Impl::SetpointManagerMultiZoneHumidityMaximum_Impl(const SetpointManagerMultiZoneHumidityMaximum_Impl& other,
                                                                                             Model_Impl* model,
                                                                                             bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerMultiZoneHumidityMaximum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerMultiZoneHumidityMaximum_Impl::iddObjectType() const {
    return SetpointManagerMultiZoneHumidityMaximum::iddObjectType();
  }

  std::string SetpointManagerMultiZoneHumidityMaximum_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_MultiZone_Humidity_MaximumFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerMultiZoneHumidityMaximum_Impl::minimumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MinimumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerMultiZoneHumidityMaximum_Impl::maximumSetpointHumidityRatio() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MaximumSetpointHumidityRatio,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> SetpointManagerMultiZoneHumidityMaximum_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MultiZone_Humidity_MaximumFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerMultiZoneHumidityMaximum_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MaximumFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerMultiZoneHumidityMaximum_Impl::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MinimumSetpointHumidityRatio, minimumSetpointHumidityRatio);
    return result;
  }

  bool SetpointManagerMultiZoneHumidityMaximum_Impl::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
    bool result = setDouble(OS_SetpointManager_MultiZone_Humidity_MaximumFields::MaximumSetpointHumidityRatio, maximumSetpointHumidityRatio);
    return result;
  }

  bool SetpointManagerMultiZoneHumidityMaximum_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_MultiZone_Humidity_MaximumFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerMultiZoneHumidityMaximum_Impl::resetSetpointNode() {
    bool result = setString(OS_SetpointManager_MultiZone_Humidity_MaximumFields::SetpointNodeorNodeListName, "");
    OS_ASSERT(result);
  }

} // detail

SetpointManagerMultiZoneHumidityMaximum::SetpointManagerMultiZoneHumidityMaximum(const Model& model)
  : SetpointManager(SetpointManagerMultiZoneHumidityMaximum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>());

  setControlVariable("MaximumHumidityRatio");
  setMinimumSetpointHumidityRatio(0.008);
  setMaximumSetpointHumidityRatio(0.015);
}

IddObjectType SetpointManagerMultiZoneHumidityMaximum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_MultiZone_Humidity_Maximum);
}

std::vector<std::string> SetpointManagerMultiZoneHumidityMaximum::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_MultiZone_Humidity_MaximumFields::ControlVariable);
}

std::string SetpointManagerMultiZoneHumidityMaximum::controlVariable() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->controlVariable();
}

double SetpointManagerMultiZoneHumidityMaximum::minimumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->minimumSetpointHumidityRatio();
}

double SetpointManagerMultiZoneHumidityMaximum::maximumSetpointHumidityRatio() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->maximumSetpointHumidityRatio();
}

boost::optional<Node> SetpointManagerMultiZoneHumidityMaximum::setpointNode() const {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setpointNode();
}

bool SetpointManagerMultiZoneHumidityMaximum::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerMultiZoneHumidityMaximum::setMinimumSetpointHumidityRatio(double minimumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setMinimumSetpointHumidityRatio(minimumSetpointHumidityRatio);
}

bool SetpointManagerMultiZoneHumidityMaximum::setMaximumSetpointHumidityRatio(double maximumSetpointHumidityRatio) {
  return getImpl<detail::SetpointManagerMultiZoneHumidityMaximum_Impl>()->setMaximumSetpointHumidityRatio(maximumSetpointHumidityRatio);
}

/// @cond
SetpointManagerMultiZoneHumidityMaximum::SetpointManagerMultiZoneHumidityMaximum(std::shared_ptr<detail::SetpointManagerMultiZoneHumidityMaximum_Impl> impl)
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

