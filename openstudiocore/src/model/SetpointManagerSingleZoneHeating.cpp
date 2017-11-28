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

#include "SetpointManagerSingleZoneHeating.hpp"
#include "SetpointManagerSingleZoneHeating_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_SetpointManager_SingleZone_Heating_FieldEnums.hxx>
#include "../utilities/units/Unit.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SetpointManagerSingleZoneHeating_Impl::SetpointManagerSingleZoneHeating_Impl(const IdfObject& idfObject,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : SetpointManager_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneHeating::iddObjectType());
  }

  SetpointManagerSingleZoneHeating_Impl::SetpointManagerSingleZoneHeating_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneHeating::iddObjectType());
  }

  SetpointManagerSingleZoneHeating_Impl::SetpointManagerSingleZoneHeating_Impl(const SetpointManagerSingleZoneHeating_Impl& other,
                                                                               Model_Impl* model,
                                                                               bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& SetpointManagerSingleZoneHeating_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerSingleZoneHeating_Impl::iddObjectType() const {
    return SetpointManagerSingleZoneHeating::iddObjectType();
  }

  double SetpointManagerSingleZoneHeating_Impl::minimumSupplyAirTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_SingleZone_HeatingFields::MinimumSupplyAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerSingleZoneHeating_Impl::maximumSupplyAirTemperature() const {
    boost::optional<double> value = getDouble(OS_SetpointManager_SingleZone_HeatingFields::MaximumSupplyAirTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneHeating_Impl::controlZone() const {
    return getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_HeatingFields::ControlZoneName);
  }

  void SetpointManagerSingleZoneHeating_Impl::setMinimumSupplyAirTemperature(double minimumSupplyAirTemperature) {
    bool result = setDouble(OS_SetpointManager_SingleZone_HeatingFields::MinimumSupplyAirTemperature, minimumSupplyAirTemperature);
    OS_ASSERT(result);
  }

  void SetpointManagerSingleZoneHeating_Impl::setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature) {
    bool result = setDouble(OS_SetpointManager_SingleZone_HeatingFields::MaximumSupplyAirTemperature, maximumSupplyAirTemperature);
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneHeating_Impl::setControlZone(const ThermalZone& thermalZone) {
    bool result = setPointer(OS_SetpointManager_SingleZone_HeatingFields::ControlZoneName, thermalZone.handle());
    return result;
  }

  void SetpointManagerSingleZoneHeating_Impl::resetControlZone() {
    bool result = setString(OS_SetpointManager_SingleZone_HeatingFields::ControlZoneName, "");
    OS_ASSERT(result);
  }

  std::string SetpointManagerSingleZoneHeating_Impl::controlVariable() const {
   boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_HeatingFields::ControlVariable,true);
   OS_ASSERT(value);
   return value.get();
  }

  bool SetpointManagerSingleZoneHeating_Impl::setControlVariable( const std::string & value ) {
   return setString(OS_SetpointManager_SingleZone_HeatingFields::ControlVariable, value);
  }

  boost::optional<Node> SetpointManagerSingleZoneHeating_Impl::setpointNode() const {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_HeatingFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerSingleZoneHeating_Impl::setSetpointNode( const Node & node )  {
   return setPointer(OS_SetpointManager_SingleZone_HeatingFields::SetpointNodeorNodeListName, node.handle());
  }

  void SetpointManagerSingleZoneHeating_Impl::resetSetpointNode() {
    bool result = setString(OS_SetpointManager_SingleZone_HeatingFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

  bool SetpointManagerSingleZoneHeating_Impl::addToNode(Node & node)
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

} // detail

SetpointManagerSingleZoneHeating::SetpointManagerSingleZoneHeating(const Model& model)
  : SetpointManager(SetpointManagerSingleZoneHeating::iddObjectType(),model)
{
  auto impl = getImpl<detail::SetpointManagerSingleZoneHeating_Impl>();
  OS_ASSERT(impl);

  setMinimumSupplyAirTemperature(-99.0);
  setMaximumSupplyAirTemperature(99.0);
  setControlVariable("Temperature");
}

IddObjectType SetpointManagerSingleZoneHeating::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_SingleZone_Heating);
}

double SetpointManagerSingleZoneHeating::minimumSupplyAirTemperature() const {
  return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->minimumSupplyAirTemperature();
}

double SetpointManagerSingleZoneHeating::maximumSupplyAirTemperature() const {
  return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->maximumSupplyAirTemperature();
}

boost::optional<ThermalZone> SetpointManagerSingleZoneHeating::controlZone() const {
  return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->controlZone();
}

void SetpointManagerSingleZoneHeating::setMinimumSupplyAirTemperature(double minimumSupplyAirTemperature) {
  getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->setMinimumSupplyAirTemperature(minimumSupplyAirTemperature);
}

void SetpointManagerSingleZoneHeating::setMaximumSupplyAirTemperature(double maximumSupplyAirTemperature) {
  getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->setMaximumSupplyAirTemperature(maximumSupplyAirTemperature);
}

bool SetpointManagerSingleZoneHeating::setControlZone(const ThermalZone& thermalZone) {
  return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->setControlZone(thermalZone);
}

void SetpointManagerSingleZoneHeating::resetControlZone() {
  getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->resetControlZone();
}

bool SetpointManagerSingleZoneHeating::setControlVariable( const std::string & value ) {
  return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->setControlVariable(value);
}

std::string SetpointManagerSingleZoneHeating::controlVariable() const {
  return getImpl<detail::SetpointManagerSingleZoneHeating_Impl>()->controlVariable();
}

/// @cond
SetpointManagerSingleZoneHeating::SetpointManagerSingleZoneHeating(std::shared_ptr<detail::SetpointManagerSingleZoneHeating_Impl> impl)
  : SetpointManager(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

