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
// #include <model/Schedule.hpp>
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

  IdfObject idfObject(IddObjectType::SetpointManager_OutdoorAirPretreat);

  // Name
  s = modelObject.name();
  idfObject.setString(SetpointManager_OutdoorAirPretreatFields::Name,s.get());

  // ControlVariable
  s = modelObject.controlVariable();
  if( s )
  {
    idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ControlVariable,s.get());
  }

  // SetpointatOutdoorLowTemperature
  d = modelObject.setpointatOutdoorLowTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::SetpointatOutdoorLowTemperature,d.get());
  }

  // OutdoorLowTemperature
  d = modelObject.outdoorLowTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::OutdoorLowTemperature,d.get());
  } 

  // SetpointatOutdoorHighTemperature
  d = modelObject.setpointatOutdoorHighTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::SetpointatOutdoorHighTemperature,d.get());
  }

  // OutdoorHighTemperature
  d = modelObject.outdoorHighTemperature();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::OutdoorHighTemperature,d.get());
  } 

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_OutdoorAirPretreatFields::SetpointNodeorNodeListName,node->name().get());
  }

  // Schedule
  boost::optional<model::Schedule> schedule = modelObject.schedule();
  if( schedule )
  {
    boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get());
    if( _schedule && _schedule->name() ){
      idfObject.setString(SetpointManager_OutdoorAirPretreatFields::ScheduleName,_schedule->name().get());
    }
  }

  // SetpointatOutdoorLowTemperature2
  d = modelObject.setpointatOutdoorLowTemperature2();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::SetpointatOutdoorLowTemperature2,d.get());
  }

  // OutdoorLowTemperature2
  d = modelObject.outdoorLowTemperature2();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::OutdoorLowTemperature2,d.get());
  }

  // SetpointatOutdoorHighTemperature2
  d = modelObject.setpointatOutdoorHighTemperature2();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::SetpointatOutdoorHighTemperature2,d.get());
  }

  // OutdoorHighTemperature2
  d = modelObject.outdoorHighTemperature2();
  if( d )
  {
    idfObject.setDouble(SetpointManager_OutdoorAirPretreatFields::OutdoorHighTemperature2,d.get());
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus

} // openstudio

