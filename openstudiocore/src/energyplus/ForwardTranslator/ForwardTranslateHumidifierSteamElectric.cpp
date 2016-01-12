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
#include "../../model/HumidifierSteamElectric.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/Humidifier_Steam_Electric_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateHumidifierSteamElectric( HumidifierSteamElectric & modelObject )
{
  OptionalDouble d;
  OptionalModelObject temp;

  // Name
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Humidifier_Steam_Electric, modelObject);

  // Availability Schedule Name
  if( (temp = modelObject.availabilitySchedule()) )
  {
    if( boost::optional<IdfObject> _schedule = translateAndMapModelObject(temp.get()) )
    {
      idfObject.setString(Humidifier_Steam_ElectricFields::AvailabilityScheduleName,_schedule->name().get());
    }
  }

  // Rated Capacity
  if( modelObject.isRatedCapacityAutosized() ) {
    idfObject.setString(Humidifier_Steam_ElectricFields::RatedCapacity,"Autosize");
  } if( (d = modelObject.ratedCapacity()) ) {
    idfObject.setDouble(Humidifier_Steam_ElectricFields::RatedCapacity,d.get());
  }

  // Rated Power
  if( (d = modelObject.ratedPower()) )
  {
    idfObject.setDouble(Humidifier_Steam_ElectricFields::RatedPower,d.get());
  }
  else if( modelObject.isRatedPowerAutosized() )
  {
    idfObject.setString(Humidifier_Steam_ElectricFields::RatedPower,"Autosize");
  }

  // Rated Fan Power
  if( (d = modelObject.ratedFanPower()) )
  {
    idfObject.setDouble(Humidifier_Steam_ElectricFields::RatedFanPower,d.get());
  }

  // Standby Power
  if( (d = modelObject.standbyPower()) )
  {
    idfObject.setDouble(Humidifier_Steam_ElectricFields::StandbyPower,d.get());
  }

  // Air Inlet Node Name
  temp = modelObject.inletModelObject();
  if(temp)
  {
    idfObject.setString(Humidifier_Steam_ElectricFields::AirInletNodeName,temp->name().get());
  }

  // Air Outlet Node Name
  temp = modelObject.outletModelObject();
  if(temp)
  {
    idfObject.setString(Humidifier_Steam_ElectricFields::AirOutletNodeName,temp->name().get());
  }

  // Water Storage Tank Name
  // not currently used

  return idfObject;
}

} // energyplus

} // openstudio
