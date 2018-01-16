/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerFollowOutdoorAirTemperature.hpp"
#include "../../model/Node.hpp"
#include <utilities/idd/SetpointManager_FollowOutdoorAirTemperature_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerFollowOutdoorAirTemperature(
                              SetpointManagerFollowOutdoorAirTemperature & modelObject )
{
  boost::optional<Node> node;
  boost::optional<std::string> s;
  boost::optional<double> d;

  IdfObject idfObject(IddObjectType::SetpointManager_FollowOutdoorAirTemperature);

  // Name
  s = modelObject.name();
  if( s )
  {
    idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::Name,s.get());
  }

  // ControlVariable
  s = modelObject.controlVariable();
  if( s )
  {
    idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::ControlVariable,s.get());
  }

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::SetpointNodeorNodeListName,node->name().get());
  }

  // ReferenceTemperatureType
  s = modelObject.referenceTemperatureType();
  if( s )
  {
    idfObject.setString(SetpointManager_FollowOutdoorAirTemperatureFields::ReferenceTemperatureType,s.get());
  }

  // OffsetTemperatureDifference
  d = modelObject.offsetTemperatureDifference();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowOutdoorAirTemperatureFields::OffsetTemperatureDifference,d.get());
  }

  // MaximumSetpointTemperature
  d = modelObject.maximumSetpointTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowOutdoorAirTemperatureFields::MaximumSetpointTemperature,d.get());
  }

  // MinimumSetpointTemperature
  d = modelObject.minimumSetpointTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_FollowOutdoorAirTemperatureFields::MinimumSetpointTemperature,d.get());
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

