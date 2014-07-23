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

#include "../ReverseTranslator.hpp"

#include "../../model/ThermostatSetpointDualSetpoint.hpp"
#include "../../model/ThermostatSetpointDualSetpoint_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include <utilities/idd/ThermostatSetpoint_DualSetpoint_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateThermostatSetpointDualSetpoint( const WorkspaceObject & workspaceObject )
{
OptionalModelObject result,temp;
  OptionalSchedule schedule;

  ThermostatSetpointDualSetpoint tsds(m_model);

  OptionalWorkspaceObject owo = workspaceObject.getTarget(ThermostatSetpoint_DualSetpointFields::HeatingSetpointTemperatureScheduleName);
  if(!owo)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find Schedule: ");
    return result;
  }
  temp = translateAndMapWorkspaceObject( *owo);
  if(temp)
  {
    schedule=temp->optionalCast<Schedule>();
    if(schedule){
      tsds.setHeatingSchedule( *schedule );
    }
  }

  owo = workspaceObject.getTarget(ThermostatSetpoint_DualSetpointFields::CoolingSetpointTemperatureScheduleName);
  if(!owo)
  {
    LOG(Error, "Error importing object: "
             << workspaceObject.briefDescription()
             << " Can't find Schedule: ");
    return result;
  }
  temp = translateAndMapWorkspaceObject( *owo);
  if(temp)
  {
    schedule=temp->optionalCast<Schedule>();
    if(schedule){
      tsds.setCoolingSchedule( *schedule );
    }
  }

  result = tsds;
  return result;
}

} // energyplus

} // openstudio

