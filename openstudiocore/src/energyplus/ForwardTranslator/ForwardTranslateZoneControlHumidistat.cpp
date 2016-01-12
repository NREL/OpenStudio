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
#include "../../model/ZoneControlHumidistat.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/ZoneControl_Humidistat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateZoneControlHumidistat( ZoneControlHumidistat& modelObject )
{
  boost::optional<IdfObject> result; 

  auto humidifyingSchedule = modelObject.humidifyingRelativeHumiditySetpointSchedule();
  auto dehumidifyingSchedule = modelObject.dehumidifyingRelativeHumiditySetpointSchedule();
  
  if( humidifyingSchedule || dehumidifyingSchedule ) {
    // Name
    result = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneControl_Humidistat, modelObject);

    // Zone Name
    // set by ThermalZone

    // Humidifying Relative Humidity Setpoint Schedule Name
    if( humidifyingSchedule ) {
      boost::optional<IdfObject> _humidifyingSchedule = translateAndMapModelObject(humidifyingSchedule.get());

      if( _humidifyingSchedule && _humidifyingSchedule->name() ) {
        result->setString(ZoneControl_HumidistatFields::HumidifyingRelativeHumiditySetpointScheduleName,_humidifyingSchedule->name().get());
      }
    }

    // Dehumidifying Relative Humidity Setpoint Schedule Name
    if( dehumidifyingSchedule ) {
      boost::optional<IdfObject> _dehumidifyingSchedule = translateAndMapModelObject(dehumidifyingSchedule.get());

      if( _dehumidifyingSchedule && _dehumidifyingSchedule->name() ) {
        result->setString(ZoneControl_HumidistatFields::DehumidifyingRelativeHumiditySetpointScheduleName,_dehumidifyingSchedule->name().get());
      }
    }
  }

  return result;
}

} // energyplus

} // openstudio
