/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>
#include <model/SetpointManagerOutdoorAirPretreat.hpp>
#include <model/Node.hpp>
#include <utilities/idd/SetpointManager_OutdoorAirPretreat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerOutdoorAirPretreat( SetpointManagerOutdoorAirPretreat & modelObject )
{
  boost::optional<Node> node;
  boost::optional<std::string> s;
  boost::optional<double> d;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_OutdoorAirPretreat, modelObject);

  // ControlVariable
  s = modelObject.controlVariable();
  if( s )
  {
    idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ControlVariable,s.get());
  }

  // Minimum Setpoint Temperature
  d = modelObject.minimumSetpointTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MinimumSetpointTemperature,d.get());
  }

  // Maximum Setpoint Temperature
  d = modelObject.maximumSetpointTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MaximumSetpointTemperature,d.get());
  }

  // Minimum Setpoint Humidity Ratio
  d = modelObject.minimumSetpointHumidityRatio();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MinimumSetpointHumidityRatio,d.get());
  }

  // Maximum Setpoint Humidity Ratio
  d = modelObject.maximumSetpointHumidityRatio();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::MaximumSetpointHumidityRatio,d.get());
  }

  // Reference Setpoint Node Name
  node = modelObject.referenceSetpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ReferenceSetpointNodeName,node->name().get());
  }

  // Mixed Air Stream Node Name
  node = modelObject.mixedAirStreamNode();
  if( node )
  {
    idfObject.setString(SetpointManager_OutdoorAirPretreatFields::MixedAirStreamNodeName,node->name().get());
  }

  // Outdoor Air Stream Node Name
  node = modelObject.outdoorAirStreamNode();
  if( node )
  {
    idfObject.setString(SetpointManager_OutdoorAirPretreatFields::OutdoorAirStreamNodeName,node->name().get());
  }

  // Return Air Stream Node Name
  node = modelObject.returnAirStreamNode();
  if( node )
  {
    idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ReturnAirStreamNodeName,node->name().get());
  }

  // Setpoint Node or NodeList Name
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_OutdoorAirPretreatFields::SetpointNodeorNodeListName,node->name().get());
  }

  return idfObject;
}

} // energyplus

} // openstudio

