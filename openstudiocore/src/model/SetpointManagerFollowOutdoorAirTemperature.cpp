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
  : SetpointManager(std::move(p))
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

