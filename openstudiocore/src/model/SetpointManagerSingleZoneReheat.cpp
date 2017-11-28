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

#include "SetpointManagerSingleZoneReheat.hpp"
#include "SetpointManagerSingleZoneReheat_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "../utilities/core/Assert.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

namespace openstudio {

namespace model {

namespace detail{

  SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SetpointManager_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerSingleZoneReheat::iddObjectType());
  }

  SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerSingleZoneReheat::iddObjectType());
  }

  SetpointManagerSingleZoneReheat_Impl::SetpointManagerSingleZoneReheat_Impl(
      const SetpointManagerSingleZoneReheat_Impl& other,
      Model_Impl* model,
      bool keepHandles)
    : SetpointManager_Impl(other,model,keepHandles)
  {
  }

  SetpointManagerSingleZoneReheat_Impl::~SetpointManagerSingleZoneReheat_Impl(){}

  const std::vector<std::string>& SetpointManagerSingleZoneReheat_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerSingleZoneReheat_Impl::iddObjectType() const {
    return SetpointManagerSingleZoneReheat::iddObjectType();
  }

  bool SetpointManagerSingleZoneReheat_Impl::addToNode(Node & node)
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

  ModelObject SetpointManagerSingleZoneReheat_Impl::clone(Model model) const
  {
    SetpointManagerSingleZoneReheat clonedObject = SetpointManager_Impl::clone( model ).cast<SetpointManagerSingleZoneReheat>();
    clonedObject.resetControlZone();
    return clonedObject;
  }

  std::string SetpointManagerSingleZoneReheat_Impl::controlVariable() const {
    boost::optional<std::string> value = getString(OS_SetpointManager_SingleZone_ReheatFields::ControlVariable,true);
    OS_ASSERT(value);
    return value.get();
  }

  double SetpointManagerSingleZoneReheat_Impl::minimumSupplyAirTemperature()
  {
    return getDouble(OS_SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature).get();
  }

  void SetpointManagerSingleZoneReheat_Impl::setMinimumSupplyAirTemperature( double value )
  {
    setDouble(OS_SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature,value);
  }

  double SetpointManagerSingleZoneReheat_Impl::maximumSupplyAirTemperature()
  {
    return getDouble(OS_SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature).get();
  }

  void SetpointManagerSingleZoneReheat_Impl::setMaximumSupplyAirTemperature( double value )
  {
    setDouble(OS_SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature,value);
  }

  boost::optional<Node> SetpointManagerSingleZoneReheat_Impl::setpointNode() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerSingleZoneReheat_Impl::setSetpointNode( const Node & node )
  {
   return setPointer(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName, node.handle());
  }

  void SetpointManagerSingleZoneReheat_Impl::resetSetpointNode()
  {
    bool result = setString(OS_SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

  boost::optional<ThermalZone> SetpointManagerSingleZoneReheat_Impl::controlZone()
  {
    SetpointManagerSingleZoneReheat thisModelObject = this->getObject<SetpointManagerSingleZoneReheat>();

    return thisModelObject.getModelObjectTarget<ThermalZone>(OS_SetpointManager_SingleZone_ReheatFields::ControlZoneName);
  }

  void SetpointManagerSingleZoneReheat_Impl::setControlZone(ThermalZone & thermalZone)
  {
    SetpointManagerSingleZoneReheat thisModelObject = this->getObject<SetpointManagerSingleZoneReheat>();

    thisModelObject.setPointer(OS_SetpointManager_SingleZone_ReheatFields::ControlZoneName,thermalZone.handle());
  }

  bool SetpointManagerSingleZoneReheat_Impl::setControlVariable(const std::string& controlVariable) {
    return setString(OS_SetpointManager_SingleZone_ReheatFields::ControlVariable, controlVariable);
  }

  boost::optional<ModelObject> SetpointManagerSingleZoneReheat_Impl::controlZoneAsModelObject() {
    OptionalModelObject result;
    OptionalThermalZone intermediate = controlZone();
    if (intermediate) {
      result = *intermediate;
    }
    return result;
  }

  bool SetpointManagerSingleZoneReheat_Impl::setControlZoneAsModelObject(const boost::optional<ModelObject>& modelObject) {
    if (modelObject) {
      OptionalThermalZone intermediate = modelObject->optionalCast<ThermalZone>();
      if (intermediate) {
        setControlZone(*intermediate);
        return true;
      }
    }
    return false;
  }

  void SetpointManagerSingleZoneReheat_Impl::resetControlZone()
  {
    setString(OS_SetpointManager_SingleZone_ReheatFields::ControlZoneName,"");
  }

} // detail

SetpointManagerSingleZoneReheat::SetpointManagerSingleZoneReheat(const Model& model)
  : SetpointManager(SetpointManagerSingleZoneReheat::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerSingleZoneReheat_Impl>());

  setMinimumSupplyAirTemperature(-99.0);

  setMaximumSupplyAirTemperature(99.0);
}

SetpointManagerSingleZoneReheat::SetpointManagerSingleZoneReheat(std::shared_ptr<detail::SetpointManagerSingleZoneReheat_Impl> p)
  : SetpointManager(std::move(p))
{
}

std::vector<std::string> SetpointManagerSingleZoneReheat::controlVariableValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SetpointManager_SingleZone_ReheatFields::ControlVariable);
}

IddObjectType SetpointManagerSingleZoneReheat::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SetpointManager_SingleZone_Reheat);
  return result;
}

double SetpointManagerSingleZoneReheat::minimumSupplyAirTemperature()
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->minimumSupplyAirTemperature();
}

void SetpointManagerSingleZoneReheat::setMinimumSupplyAirTemperature( double value )
{
  getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setMinimumSupplyAirTemperature(value);
}

double SetpointManagerSingleZoneReheat::maximumSupplyAirTemperature()
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->maximumSupplyAirTemperature();
}

void SetpointManagerSingleZoneReheat::setMaximumSupplyAirTemperature( double value )
{
  getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setMaximumSupplyAirTemperature(value);
}

boost::optional<Node> SetpointManagerSingleZoneReheat::setpointNode() const
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setpointNode();
}

boost::optional<ThermalZone> SetpointManagerSingleZoneReheat::controlZone()
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->controlZone();
}

void SetpointManagerSingleZoneReheat::setControlZone(ThermalZone & thermalZone)
{
  getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setControlZone(thermalZone);
}

void SetpointManagerSingleZoneReheat::resetControlZone()
{
  getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->resetControlZone();
}

std::string SetpointManagerSingleZoneReheat::controlVariable() const
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->controlVariable();
}

bool SetpointManagerSingleZoneReheat::setControlVariable( const std::string& controlVariable )
{
  return getImpl<detail::SetpointManagerSingleZoneReheat_Impl>()->setControlVariable(controlVariable);
}

} // model
} // openstudio
