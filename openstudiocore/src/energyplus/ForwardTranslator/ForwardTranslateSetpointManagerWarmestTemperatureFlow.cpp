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
#include "../../model/SetpointManagerWarmestTemperatureFlow.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PortList.hpp"
#include <utilities/idd/SetpointManager_WarmestTemperatureFlow_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerWarmestTemperatureFlow( SetpointManagerWarmestTemperatureFlow & modelObject )
{
  std::string s;
  double n;
  boost::optional<Node> node;

  IdfObject idfObject(IddObjectType::SetpointManager_WarmestTemperatureFlow);

  m_idfObjects.push_back(idfObject);

  // Name
  s = modelObject.name().get();
  idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::Name,s);

  // ControlVariable
  idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::ControlVariable,"Temperature");

  // HVACAirLoopName
  if( auto node = modelObject.setpointNode() ) {
    if( auto airLoop = node->airLoopHVAC() ) {
      idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::HVACAirLoopName,airLoop->name().get());
    }
  }

  // Strategy
  s = modelObject.strategy();
  idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::Strategy,s);

  // MinimumSupplyAirTemperature
  n = modelObject.minimumSetpointTemperature();
  idfObject.setDouble(SetpointManager_WarmestTemperatureFlowFields::MinimumSetpointTemperature,n);

  // MaximumSupplyAirTemperature
  n = modelObject.maximumSetpointTemperature();
  idfObject.setDouble(SetpointManager_WarmestTemperatureFlowFields::MaximumSetpointTemperature,n);

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node ) {
    idfObject.setString(SetpointManager_WarmestTemperatureFlowFields::SetpointNodeorNodeListName,node->name().get());
  }

  // MinimumTurndownRatio
  n = modelObject.minimumTurndownRatio();
  idfObject.setDouble(SetpointManager_WarmestTemperatureFlowFields::MinimumTurndownRatio,n);
  
  return idfObject;
}

} // energyplus

} // openstudio

