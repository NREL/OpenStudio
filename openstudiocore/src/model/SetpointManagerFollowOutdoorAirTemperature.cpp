/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <model/SetpointManagerFollowOutdoorAirTemperature.hpp>
#include <model/SetpointManagerFollowOutdoorAirTemperature_Impl.hpp>

#include <model/Model.hpp>
#include <model/Node.hpp>
#include <model/Node_Impl.hpp>
#include <model/AirLoopHVAC.hpp>
#include <model/AirLoopHVAC_Impl.hpp>
#include <model/Schedule.hpp>
#include <model/Schedule_Impl.hpp>

#include <utilities/idd/OS_SetpointManager_FollowOutdoorAirTemperature_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

namespace openstudio {

namespace model {

namespace detail{

  SetpointManagerFollowOutdoorAirTemperature_Impl::SetpointManagerFollowOutdoorAirTemperature_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : HVACComponent_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerFollowOutdoorAirTemperature::iddObjectType());
  }

  SetpointManagerFollowOutdoorAirTemperature_Impl::SetpointManagerFollowOutdoorAirTemperature_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
    : HVACComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerFollowOutdoorAirTemperature::iddObjectType());
  }

  SetpointManagerFollowOutdoorAirTemperature_Impl::SetpointManagerFollowOutdoorAirTemperature_Impl(
      const SetpointManagerFollowOutdoorAirTemperature_Impl& other, 
      Model_Impl* model,
      bool keepHandles)
    : HVACComponent_Impl(other,model,keepHandles)
  {
  }

  SetpointManagerFollowOutdoorAirTemperature_Impl::~SetpointManagerFollowOutdoorAirTemperature_Impl(){}

  const std::vector<std::string>& SetpointManagerFollowOutdoorAirTemperature_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SetpointManagerFollowOutdoorAirTemperature_Impl::iddObjectType() const {
    return SetpointManagerFollowOutdoorAirTemperature::iddObjectType();
  }

  boost::optional<ParentObject> SetpointManagerFollowOutdoorAirTemperature_Impl::parent() const {
    NodeVector nodes = getObject<ModelObject>().getModelObjectSources<Node>();
    if (nodes.size() == 1u) {
      return nodes[0];
    }
    return boost::none;
  }

  std::vector<ModelObject> SetpointManagerFollowOutdoorAirTemperature_Impl::children() const
  {
    std::vector<ModelObject> result;
    return result;
  }

  bool SetpointManagerFollowOutdoorAirTemperature_Impl::addToNode(Node & node)
  {
    if( node.model() != this->model() )
    {
      return false;
    } 

    node.removeSetpointManagerMixedAir();

    node.removeSetpointManagerSingleZoneReheat();

    node.removeSetpointManagerScheduled();

    node.removeSetpointManagerFollowOutdoorAirTemperature();

    node.removeSetpointManagerOutdoorAirReset();

    node.removeSetpointManagerWarmest();

    if( OptionalAirLoopHVAC airLoop = node.airLoopHVAC() )
    {
      if( airLoop->supplyComponent(node.handle()) )
      {
        this->setSetpointNode(node);

        return true;
      }
      if(OptionalAirLoopHVACOutdoorAirSystem oaSystem = airLoop->airLoopHVACOutdoorAirSystem())
      {
        if(node == oaSystem->outboardOANode().get())
        {
          return false;
        }

        if(oaSystem->oaComponent(node.handle()))
        {
          this->setSetpointNode(node);
        
          return true;
        }
      }
    }
    else if( boost::optional<PlantLoop> plantLoop = node.plantLoop() )
    {
      if( plantLoop->supplyComponent(node.handle()) )
      {
        this->setSetpointNode(node);

        return true;
      }
    }

    return false;
  }

  std::vector<openstudio::IdfObject> SetpointManagerFollowOutdoorAirTemperature_Impl::remove()
  {
    return HVACComponent_Impl::remove();
  }

  ModelObject SetpointManagerFollowOutdoorAirTemperature_Impl::clone(Model model)
  {
    return HVACComponent_Impl::clone( model );
  }

  boost::optional<Node> SetpointManagerFollowOutdoorAirTemperature_Impl::setpointNode() const
  {
    SetpointManagerFollowOutdoorAirTemperature thisModelObject = this->getObject<SetpointManagerFollowOutdoorAirTemperature>();

    return thisModelObject.getModelObjectTarget<Node>(OS_SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName);
  }

  void SetpointManagerFollowOutdoorAirTemperature_Impl::setSetpointNode( Node & node )
  {
    SetpointManagerFollowOutdoorAirTemperature thisModelObject = this->getObject<SetpointManagerFollowOutdoorAirTemperature>();

    thisModelObject.setPointer(OS_SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName,node.handle());
  }

  std::string SetpointManagerFollowOutdoorAirTemperature_Impl::controlVariable() const
  {
    return this->getString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable).get();
  }

  void SetpointManagerFollowOutdoorAirTemperature_Impl::setControlVariable(const std::string & value)
  {
    if( istringEqual(value,"Temperature") )
    {
      this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable,"Temperature");
    }
    else if( istringEqual(value,"MaximumTemperature") )
    {
      this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable,"MaximumTemperature");
    }
    else if( istringEqual(value,"MinimumTemperature") )
    {
      this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable,"MinimumTemperature");
    }
  }

  void SetpointManagerFollowOutdoorAirTemperature_Impl::setReferenceTemperatureType(const std::string & value)
  {
    if( istringEqual(value,"OutdoorAirWetBulb") )
    {
      this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ReferenceTemperatureType,"OutdoorAirWetBulb");
    }
    else if( istringEqual(value,"OutdoorAirDryBulb") )
    {
      this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ReferenceTemperatureType,"OutdoorAirDryBulb");
    }
  }

  std::string SetpointManagerFollowOutdoorAirTemperature_Impl::referenceTemperatureType() const
  {
    return getString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ReferenceTemperatureType).get();
  }

  double SetpointManagerFollowOutdoorAirTemperature_Impl::offsetTemperatureDifference() const
  {
    return getDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::OffsetTemperatureDifference).get();
  }

  void SetpointManagerFollowOutdoorAirTemperature_Impl::setOffsetTemperatureDifference(double value)
  {
    setDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::OffsetTemperatureDifference,value);
  }

  double SetpointManagerFollowOutdoorAirTemperature_Impl::maximumSetpointTemperature() const
  {
    return getDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::MaximumSetpointTemperature).get();
  }

  void SetpointManagerFollowOutdoorAirTemperature_Impl::setMaximumSetpointTemperature(double value)
  {
    setDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::MaximumSetpointTemperature,value);
  }

  double SetpointManagerFollowOutdoorAirTemperature_Impl::minimumSetpointTemperature() const
  {
    return getDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::MinimumSetpointTemperature).get();
  }

  void SetpointManagerFollowOutdoorAirTemperature_Impl::setMinimumSetpointTemperature(double value)
  {
    setDouble(OS_SetpointManager_FollowOutdoorAirTemperatureFields::MinimumSetpointTemperature,value);
  }

} // detail
  
SetpointManagerFollowOutdoorAirTemperature::SetpointManagerFollowOutdoorAirTemperature(const Model& model)
  : HVACComponent(SetpointManagerFollowOutdoorAirTemperature::iddObjectType(),model) 
{
  OS_ASSERT(getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>());

  setControlVariable("Temperature");
  setReferenceTemperatureType("OutdoorAirWetBulb");
  setOffsetTemperatureDifference(1.5);
  setMaximumSetpointTemperature(80.0);
  setMinimumSetpointTemperature(6.0);
}

SetpointManagerFollowOutdoorAirTemperature::SetpointManagerFollowOutdoorAirTemperature(boost::shared_ptr<detail::SetpointManagerFollowOutdoorAirTemperature_Impl> p)
  : HVACComponent(p)
{
}

IddObjectType SetpointManagerFollowOutdoorAirTemperature::iddObjectType() {
  return IddObjectType(IddObjectType::OS_SetpointManager_FollowOutdoorAirTemperature);
}

boost::optional<Node> SetpointManagerFollowOutdoorAirTemperature::setpointNode() const
{
  return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setpointNode();
}

std::string SetpointManagerFollowOutdoorAirTemperature::controlVariable() const
{
  return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->controlVariable();
}

void SetpointManagerFollowOutdoorAirTemperature::setControlVariable(const std::string & value)
{
  getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setControlVariable(value);
}

std::string SetpointManagerFollowOutdoorAirTemperature::referenceTemperatureType() const
{
  return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->referenceTemperatureType();
}

void SetpointManagerFollowOutdoorAirTemperature::setReferenceTemperatureType(const std::string & value)
{
  getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setReferenceTemperatureType(value);
}

double SetpointManagerFollowOutdoorAirTemperature::offsetTemperatureDifference() const
{
  return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->offsetTemperatureDifference();
}

void SetpointManagerFollowOutdoorAirTemperature::setOffsetTemperatureDifference(double value)
{
  getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setOffsetTemperatureDifference(value);
}

double SetpointManagerFollowOutdoorAirTemperature::maximumSetpointTemperature() const
{
  return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->maximumSetpointTemperature();
}

void SetpointManagerFollowOutdoorAirTemperature::setMaximumSetpointTemperature(double value)
{
  getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setMaximumSetpointTemperature(value);
}

double SetpointManagerFollowOutdoorAirTemperature::minimumSetpointTemperature() const
{
  return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->minimumSetpointTemperature();
}

void SetpointManagerFollowOutdoorAirTemperature::setMinimumSetpointTemperature(double value)
{
  getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setMinimumSetpointTemperature(value);
}

} // model

} // openstudio

