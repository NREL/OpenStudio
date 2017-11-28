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

#include "SetpointManagerSingleZoneHumidityMinimum.hpp"
#include "SetpointManagerSingleZoneHumidityMinimum_Impl.hpp"

#include "Node.hpp"
#include "Node_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Model.hpp"
#include "AirLoopHVAC.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_SingleZone_Humidity_Minimum_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerSingleZoneHumidityMinimum_Impl::SetpointManagerSingleZoneHumidityMinimum_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneHumidityMinimum::iddObjectType());
  }

  SetpointManagerSingleZoneHumidityMinimum_Impl::SetpointManagerSingleZoneHumidityMinimum_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneHumidityMinimum::iddObjectType());
  }

  SetpointManagerSingleZoneHumidityMinimum_Impl::SetpointManagerSingleZoneHumidityMinimum_Impl(const SetpointManagerSingleZoneHumidityMinimum_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerSingleZoneHumidityMinimum_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerSingleZoneHumidityMinimum_Impl::iddObjectType() const {
    return SetpointManagerSingleZoneHumidityMinimum::iddObjectType();
  }

  bool SetpointManagerSingleZoneHumidityMinimum_Impl::addToNode(Node & node)
  {
    bool added = SetpointManager_Impl::addToNode( node );
    if( added ) {
      if( boost::optional<AirLoopHVAC> _airLoop = node.airLoopHVAC() ) {
        ModelObjectVector modelObjectVector = _airLoop->demandComponents(openstudio::IddObjectType::OS_ThermalZone);
        if( !modelObjectVector.empty() ) {
          ModelObject mo = modelObjectVector.front();
          ThermalZone thermalZone = mo.cast<ThermalZone>();
          this->setControlZone(thermalZone);
        }
        return true;
      }
    }
    return false;
  }

  ModelObject SetpointManagerSingleZoneHumidityMinimum_Impl::clone(Model model) const
  {
    SetpointManagerSingleZoneHumidityMinimum clonedObject = SetpointManager_Impl::clone( model ).cast<SetpointManagerSingleZoneHumidityMinimum>();
    clonedObject.resetControlZone();
    return clonedObject;
  }

  std::string SetpointManagerSingleZoneHumidityMinimum_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<Node> SetpointManagerSingleZoneHumidityMinimum_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName);
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneHumidityMinimum_Impl::controlZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneName);
  }

  bool SetpointManagerSingleZoneHumidityMinimum_Impl::setControlVariable(const std::string& controlVariable) {
    bool result = setString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable, controlVariable);
    return result;
  }

  bool SetpointManagerSingleZoneHumidityMinimum_Impl::setSetpointNode(const Node& node) {
    bool result = setPointer(OS_SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName, node.handle());
    return result;
  }

  void SetpointManagerSingleZoneHumidityMinimum_Impl::resetSetpointNode() {
    bool result = setString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneHumidityMinimum_Impl::setControlZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneName, thermalZone.handle());
    return result;
  }

  void SetpointManagerSingleZoneHumidityMinimum_Impl::resetControlZone()
  {
    setString(OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlZoneName,"");
  }

} // detail

SetpointManagerSingleZoneHumidityMinimum::SetpointManagerSingleZoneHumidityMinimum(const Model& model)
  : SetpointManager(SetpointManagerSingleZoneHumidityMinimum::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>());

  bool ok = setControlVariable("MinimumHumidityRatio");
  OS_ASSERT(ok);
}

IddObjectType SetpointManagerSingleZoneHumidityMinimum::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_SingleZone_Humidity_Minimum);
}

std::vector<std::string> SetpointManagerSingleZoneHumidityMinimum::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_SingleZone_Humidity_MinimumFields::ControlVariable);
}

std::string SetpointManagerSingleZoneHumidityMinimum::controlVariable() const {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->controlVariable();
}

boost::optional<Node> SetpointManagerSingleZoneHumidityMinimum::setpointNode() const {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->setpointNode();
}

boost::optional<ThermalZone> SetpointManagerSingleZoneHumidityMinimum::controlZone() const {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->controlZone();
}

bool SetpointManagerSingleZoneHumidityMinimum::setControlVariable(const std::string& controlVariable) {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->setControlVariable(controlVariable);
}

bool SetpointManagerSingleZoneHumidityMinimum::setControlZone(const ThermalZone& thermalZone) {
  return getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->setControlZone(thermalZone);
}

void SetpointManagerSingleZoneHumidityMinimum::resetControlZone() {
  getImpl<detail::SetpointManagerSingleZoneHumidityMinimum_Impl>()->resetControlZone();
}

/// @cond
SetpointManagerSingleZoneHumidityMinimum::SetpointManagerSingleZoneHumidityMinimum(std::shared_ptr<detail::SetpointManagerSingleZoneHumidityMinimum_Impl> impl)
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

