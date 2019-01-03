/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerSingleZoneReheat.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include <utilities/idd/SetpointManager_SingleZone_Reheat_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerSingleZoneReheat( SetpointManagerSingleZoneReheat & modelObject )
{
  std::string s;
  double n;
  boost::optional<Node> node;

  IdfObject idfObject(IddObjectType::SetpointManager_SingleZone_Reheat);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name().get();
  idfObject.setString(SetpointManager_SingleZone_ReheatFields::Name,s);

  // ControlVariable
  idfObject.setString(SetpointManager_SingleZone_ReheatFields::ControlVariable,"Temperature");

  // MinimumSupplyAirTemperature
  n = modelObject.minimumSupplyAirTemperature();
  idfObject.setDouble(SetpointManager_SingleZone_ReheatFields::MinimumSupplyAirTemperature,n);

  // MaximumSupplyAirTemperature
  n = modelObject.maximumSupplyAirTemperature();
  idfObject.setDouble(SetpointManager_SingleZone_ReheatFields::MaximumSupplyAirTemperature,n);

  // ControlZoneName
  boost::optional<ThermalZone> thermalZone = modelObject.controlZone();
  if( thermalZone )
  {
    idfObject.setString(SetpointManager_SingleZone_ReheatFields::ControlZoneName,thermalZone->name().get());
  }

  // ZoneNodeName
  if( thermalZone )
  {
    node = thermalZone->zoneAirNode();
    idfObject.setString(SetpointManager_SingleZone_ReheatFields::ZoneNodeName,node->name().get());
  }

  // ZoneInletNodeName
  if( thermalZone )
  {
    if( boost::optional<ModelObject> mo = thermalZone->inletPortList().airLoopHVACModelObject() )
    {
      if( (node = mo->optionalCast<Node>()) )
      {
        idfObject.setString(SetpointManager_SingleZone_ReheatFields::ZoneInletNodeName,node->name().get());
      }
    }
  }

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_SingleZone_ReheatFields::SetpointNodeorNodeListName,node->name().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

