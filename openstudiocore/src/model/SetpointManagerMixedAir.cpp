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

#include "SetpointManagerMixedAir.hpp"
#include "SetpointManagerMixedAir_Impl.hpp"
#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "FanConstantVolume.hpp"
#include "FanConstantVolume_Impl.hpp"
#include "FanVariableVolume.hpp"
#include "FanVariableVolume_Impl.hpp"
#include "FanOnOff.hpp"
#include "FanOnOff_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "PlantLoop.hpp"
#include <utilities/idd/OS_SetpointManager_MixedAir_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail{

SetpointManagerMixedAir_Impl::SetpointManagerMixedAir_Impl(const IdfObject& idfObject,
                                                           Model_Impl* model,
                                                           bool keepHandle)
  : SetpointManager_Impl(idfObject, model, keepHandle)
{
  OS_ASSERT(idfObject.iddObject().type() == SetpointManagerMixedAir::iddObjectType());
}

SetpointManagerMixedAir_Impl::SetpointManagerMixedAir_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other,Model_Impl* model,bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
{
  OS_ASSERT(other.iddObject().type() == SetpointManagerMixedAir::iddObjectType());
}

SetpointManagerMixedAir_Impl::SetpointManagerMixedAir_Impl(
    const SetpointManagerMixedAir_Impl& other,
    Model_Impl* model,
    bool keepHandles)
  : SetpointManager_Impl(other,model,keepHandles)
{
}

SetpointManagerMixedAir_Impl::~SetpointManagerMixedAir_Impl(){}

const std::vector<std::string>& SetpointManagerMixedAir_Impl::outputVariableNames() const
{
  static std::vector<std::string> result;
  if (result.empty()){
  }
  return result;
}

IddObjectType SetpointManagerMixedAir_Impl::iddObjectType() const {
  return SetpointManagerMixedAir::iddObjectType();
}

bool SetpointManagerMixedAir_Impl::addToNode(Node & node)
{
  bool added = SetpointManager_Impl::addToNode( node );
  if( added ) {
    if( boost::optional<AirLoopHVAC> _airLoop = node.airLoopHVAC() ) {
      std::vector<StraightComponent> fans;
      std::vector<ModelObject> supplyComponents = _airLoop->supplyComponents();

      for( auto it = supplyComponents.begin();
           it != supplyComponents.end();
           ++it )
      {
        if( boost::optional<FanVariableVolume> variableFan = it->optionalCast<FanVariableVolume>() ) {
          fans.insert(fans.begin(), *variableFan);
        }
        else if( boost::optional<FanConstantVolume> constantFan = it->optionalCast<FanConstantVolume>() ) {
          fans.insert(fans.begin(), *constantFan);
        }
      }

      if( !fans.empty() ) {
        StraightComponent fan = fans.front();
        if( OptionalNode inletNode = fan.inletModelObject()->optionalCast<Node>() ) {
          this->setFanInletNode(*inletNode);
        }
        if( OptionalNode outletNode = fan.outletModelObject()->optionalCast<Node>() ) {
          this->setFanOutletNode(*outletNode);
        }
      }

      Node supplyOutletNode = _airLoop->supplyOutletNode();
      this->setReferenceSetpointNode(supplyOutletNode);
      return true;
    }
  }
  return false;
}

ModelObject SetpointManagerMixedAir_Impl::clone(Model model) const
{
  SetpointManagerMixedAir clonedObject = SetpointManager_Impl::clone( model ).cast<SetpointManagerMixedAir>();
  clonedObject.getImpl<detail::SetpointManagerMixedAir_Impl>()->resetReferenceSetpointNode();
  clonedObject.getImpl<detail::SetpointManagerMixedAir_Impl>()->resetFanInletNode();
  clonedObject.getImpl<detail::SetpointManagerMixedAir_Impl>()->resetFanOutletNode();
  return clonedObject;
}

std::string SetpointManagerMixedAir_Impl::controlVariable() const
{
  return getString(OS_SetpointManager_MixedAirFields::ControlVariable).get();
}

bool SetpointManagerMixedAir_Impl::setControlVariable( const std::string& value )
{
  return setString(OS_SetpointManager_MixedAirFields::ControlVariable,value);
}

boost::optional<Node> SetpointManagerMixedAir_Impl::referenceSetpointNode()
{
  return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MixedAirFields::ReferenceSetpointNodeName);
}

void SetpointManagerMixedAir_Impl::setReferenceSetpointNode( Node & node )
{
  bool result = setPointer(OS_SetpointManager_MixedAirFields::ReferenceSetpointNodeName,node.handle());
  OS_ASSERT(result);
}

void SetpointManagerMixedAir_Impl::resetReferenceSetpointNode()
{
  bool result = setString(OS_SetpointManager_MixedAirFields::ReferenceSetpointNodeName,"");
  OS_ASSERT(result);
}

boost::optional<Node> SetpointManagerMixedAir_Impl::fanInletNode()
{
  return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MixedAirFields::FanInletNodeName);
}

void SetpointManagerMixedAir_Impl::setFanInletNode( Node & node )
{
  bool result = setPointer(OS_SetpointManager_MixedAirFields::FanInletNodeName,node.handle());
  OS_ASSERT(result);
}

void SetpointManagerMixedAir_Impl::resetFanInletNode()
{
  bool result = setString(OS_SetpointManager_MixedAirFields::FanInletNodeName,"");
  OS_ASSERT(result);
}

boost::optional<Node> SetpointManagerMixedAir_Impl::fanOutletNode()
{
  return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MixedAirFields::FanOutletNodeName);
}

void SetpointManagerMixedAir_Impl::setFanOutletNode( Node & node )
{
  bool result = setPointer(OS_SetpointManager_MixedAirFields::FanOutletNodeName,node.handle());
  OS_ASSERT(result);
}

void SetpointManagerMixedAir_Impl::resetFanOutletNode()
{
  bool result = setString(OS_SetpointManager_MixedAirFields::FanOutletNodeName,"");
  OS_ASSERT(result);
}

boost::optional<Node> SetpointManagerMixedAir_Impl::setpointNode() const
{
  return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_MixedAirFields::SetpointNodeorNodeListName);
}

bool SetpointManagerMixedAir_Impl::setSetpointNode( const Node & node )
{
  return setPointer(OS_SetpointManager_MixedAirFields::SetpointNodeorNodeListName, node.handle());
}

void SetpointManagerMixedAir_Impl::resetSetpointNode()
{
  bool result = setString(OS_SetpointManager_MixedAirFields::SetpointNodeorNodeListName,"");
  OS_ASSERT(result);
}

} // detail

SetpointManagerMixedAir::SetpointManagerMixedAir(const Model& model)
  : SetpointManager(SetpointManagerMixedAir::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::SetpointManagerMixedAir_Impl>());
  setControlVariable("Temperature");
}

SetpointManagerMixedAir::SetpointManagerMixedAir(
    std::shared_ptr<detail::SetpointManagerMixedAir_Impl> p)
  : SetpointManager(std::move(p))
{}

IddObjectType SetpointManagerMixedAir::iddObjectType() {
  IddObjectType result(IddObjectType::OS_SetpointManager_MixedAir);
  return result;
}

std::string SetpointManagerMixedAir::controlVariable() const
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->controlVariable();
}

bool SetpointManagerMixedAir::setControlVariable( const std::string& controlVariable )
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->setControlVariable(controlVariable);
}

boost::optional<Node> SetpointManagerMixedAir::referenceSetpointNode()
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->referenceSetpointNode();
}

void SetpointManagerMixedAir::setReferenceSetpointNode( Node & node )
{
  getImpl<detail::SetpointManagerMixedAir_Impl>()->setReferenceSetpointNode(node);
}

boost::optional<Node> SetpointManagerMixedAir::fanInletNode()
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->fanInletNode();
}

void SetpointManagerMixedAir::setFanInletNode( Node & node )
{
  getImpl<detail::SetpointManagerMixedAir_Impl>()->setFanInletNode(node);
}

boost::optional<Node> SetpointManagerMixedAir::fanOutletNode()
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->fanOutletNode();
}

void SetpointManagerMixedAir::setFanOutletNode( Node & node )
{
  getImpl<detail::SetpointManagerMixedAir_Impl>()->setFanOutletNode(node);
}

boost::optional<Node> SetpointManagerMixedAir::setpointNode() const
{
  return getImpl<detail::SetpointManagerMixedAir_Impl>()->setpointNode();
}

void SetpointManagerMixedAir::updateFanInletOutletNodes(AirLoopHVAC & airLoopHVAC)
{
  boost::optional<Node> fanInletNode;
  boost::optional<Node> fanOutletNode;

  std::vector<StraightComponent> fans;

  std::vector<ModelObject> supplyComponents = airLoopHVAC.supplyComponents();

  for( const auto & supplyComponent : supplyComponents )
  {
    if( boost::optional<FanVariableVolume> variableFan = supplyComponent.optionalCast<FanVariableVolume>() ) {
      fans.insert(fans.begin(), *variableFan);
    }
    else if( boost::optional<FanConstantVolume> constantFan = supplyComponent.optionalCast<FanConstantVolume>() ) {
      fans.insert(fans.begin(), *constantFan);
    }
    else if( boost::optional<FanOnOff> onOffFan = supplyComponent.optionalCast<FanOnOff>() ) {
      fans.insert(fans.begin(), *onOffFan);
    }
  }

  if( fans.size() > 0 )
  {
    boost::optional<ModelObject> mo;

    mo = fans.front().inletModelObject();
    if( mo )
    {
      fanInletNode = mo->optionalCast<Node>();
    }

    mo = fans.front().outletModelObject();
    if( mo )
    {
      fanOutletNode = mo->optionalCast<Node>();
    }
  }

  if( fanInletNode && fanOutletNode )
  {
    std::vector<model::Node> nodes = subsetCastVector<model::Node>(airLoopHVAC.supplyComponents());

    for( auto & node : nodes )
    {
      std::vector<SetpointManagerMixedAir> setpointManagers = subsetCastVector<SetpointManagerMixedAir>(node.setpointManagers());
      if( ! setpointManagers.empty() ) {
        SetpointManagerMixedAir spm = setpointManagers.front();
        spm.setFanInletNode(fanInletNode.get());
        spm.setFanOutletNode(fanOutletNode.get());
      }
    }
  }
}

} // model

} // openstudio

