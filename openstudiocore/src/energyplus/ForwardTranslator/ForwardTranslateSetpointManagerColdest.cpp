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
#include "../../model/SetpointManagerColdest.hpp"
#include "../../model/Node.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include <utilities/idd/SetpointManager_Coldest_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSetpointManagerColdest( SetpointManagerColdest & modelObject )
{
  boost::optional<Node> node;
  boost::optional<double> value;
  std::string s;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SetpointManager_Coldest, modelObject);

  // ControlVariable
  s = modelObject.controlVariable();
  idfObject.setString(SetpointManager_ColdestFields::ControlVariable,s);

  // MinimumSetpointTemperature 
  value = modelObject.minimumSetpointTemperature();
  if( value )
  {
    idfObject.setDouble(SetpointManager_ColdestFields::MinimumSetpointTemperature,value.get());
  } 

  // MaximumSetpointTemperature 
  value = modelObject.maximumSetpointTemperature();
  if( value )
  {
    idfObject.setDouble(SetpointManager_ColdestFields::MaximumSetpointTemperature,value.get());
  } 

  // Strategy
  s = modelObject.strategy();
  idfObject.setString(SetpointManager_ColdestFields::Strategy,s);

  // SetpointNodeorNodeListName
  node = modelObject.setpointNode();
  if( node )
  {
    idfObject.setString(SetpointManager_ColdestFields::SetpointNodeorNodeListName,node->name().get());

    // HVACAirLoopName
    if( boost::optional<model::AirLoopHVAC> airLoop = node->airLoopHVAC() )
    {
      idfObject.setString(SetpointManager_ColdestFields::HVACAirLoopName,airLoop->name().get());
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

