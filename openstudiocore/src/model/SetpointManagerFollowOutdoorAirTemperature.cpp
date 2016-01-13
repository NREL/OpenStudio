/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
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

#include "SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "SetpointManagerFollowOutdoorAirTemperature_Impl.hpp"

#include "Model.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "PlantLoop.hpp"

#include <utilities/idd/OS_SetpointManager_FollowOutdoorAirTemperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail{

  SetpointManagerFollowOutdoorAirTemperature_Impl::SetpointManagerFollowOutdoorAirTemperature_Impl(
      const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : SetpointManager_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SetpointManagerFollowOutdoorAirTemperature::iddObjectType());
  }

  SetpointManagerFollowOutdoorAirTemperature_Impl::SetpointManagerFollowOutdoorAirTemperature_Impl(
      const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
    : SetpointManager_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SetpointManagerFollowOutdoorAirTemperature::iddObjectType());
  }

  SetpointManagerFollowOutdoorAirTemperature_Impl::SetpointManagerFollowOutdoorAirTemperature_Impl(
      const SetpointManagerFollowOutdoorAirTemperature_Impl& other, 
      Model_Impl* model,
      bool keepHandles)
    : SetpointManager_Impl(other,model,keepHandles)
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

  bool SetpointManagerFollowOutdoorAirTemperature_Impl::addToNode(Node & node) {
    bool added = SetpointManager_Impl::addToNode( node );
    if( added ) {
      return added;
    } else if( boost::optional<PlantLoop> plantLoop = node.plantLoop() ) {
      return this->setSetpointNode(node);
    }
    return added;
  }

  boost::optional<Node> SetpointManagerFollowOutdoorAirTemperature_Impl::setpointNode() const
  {
    return getObject<ModelObject>().getModelObjectTarget<Node>(OS_SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName);
  }

  bool SetpointManagerFollowOutdoorAirTemperature_Impl::setSetpointNode(const Node & node )
  {
    return setPointer(OS_SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName,node.handle());
  }

  void SetpointManagerFollowOutdoorAirTemperature_Impl::resetSetpointNode()
  {
    bool result = setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName,"");
    OS_ASSERT(result);
  }

  std::string SetpointManagerFollowOutdoorAirTemperature_Impl::controlVariable() const
  {
    return this->getString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable).get();
  }

  bool SetpointManagerFollowOutdoorAirTemperature_Impl::setControlVariable(const std::string & value)
  {
    if( istringEqual(value,"Temperature") )
    {
      return this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable,"Temperature");
    }
    else if( istringEqual(value,"MaximumTemperature") )
    {
      return this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable,"MaximumTemperature");
    }
    else if( istringEqual(value,"MinimumTemperature") )
    {
      return this->setString(OS_SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable,"MinimumTemperature");
    }
    else
    {
      return false;
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
  : SetpointManager(SetpointManagerFollowOutdoorAirTemperature::iddObjectType(),model) 
{
  OS_ASSERT(getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>());

  setControlVariable("Temperature");
  setReferenceTemperatureType("OutdoorAirWetBulb");
  setOffsetTemperatureDifference(1.5);
  setMaximumSetpointTemperature(80.0);
  setMinimumSetpointTemperature(6.0);
}

SetpointManagerFollowOutdoorAirTemperature::SetpointManagerFollowOutdoorAirTemperature(std::shared_ptr<detail::SetpointManagerFollowOutdoorAirTemperature_Impl> p)
  : SetpointManager(p)
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

bool SetpointManagerFollowOutdoorAirTemperature::setControlVariable(const std::string & value)
{
  return getImpl<detail::SetpointManagerFollowOutdoorAirTemperature_Impl>()->setControlVariable(value);
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

