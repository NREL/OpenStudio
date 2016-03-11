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

#include "../ForwardTranslator.hpp"
#include "../../model/SetpointManagerSingleZoneOneStageHeating.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include <utilities/idd/SetpointManager_SingleZone_OneStageHeating_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerSingleZoneOneStageHeating( SetpointManagerSingleZoneOneStageHeating & modelObject )
{
  std::string s;
  double n;
  boost::optional<Node> node;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_SingleZone_OneStageHeating, modelObject);

  // HeatingStageOnSupplyAirSetpointTemperature
  n = modelObject.heatingStageOnSupplyAirSetpointTemperature();
  idfObject.setDouble(SetpointManager_SingleZone_OneStageHeatingFields::HeatingStageOnSupplyAirSetpointTemperature,n);

  // HeatingStageOffSupplyAirSetpointTemperature
  n = modelObject.heatingStageOffSupplyAirSetpointTemperature();
  idfObject.setDouble(SetpointManager_SingleZone_OneStageHeatingFields::HeatingStageOffSupplyAirSetpointTemperature,n);

  // ControlZoneName
  boost::optional<ThermalZone> thermalZone = modelObject.controlZone();
  if( thermalZone )
  {
    idfObject.setString(SetpointManager_SingleZone_OneStageHeatingFields::ControlZoneName,thermalZone->name().get());
  }

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_SingleZone_OneStageHeatingFields::SetpointNodeorNodeListName,node->name().get());
  }
  
  return idfObject;
}

} // energyplus

} // openstudio

