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

#include "../../model/Model.hpp"
#include "../../model/ElectricLoadCenterStorageSimple.hpp"
#include "../../model/ElectricLoadCenterStorageSimple_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include <utilities/idd/ElectricLoadCenter_Storage_Simple_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterStorageSimple(model::ElectricLoadCenterStorageSimple & modelObject)
{
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Storage_Simple, modelObject);

  if (modelObject.name()) {
    idfObject.setString(ElectricLoadCenter_Storage_SimpleFields::Name, modelObject.name().get());
  }

  // Availability Schedule, defaults to model.alwaysOnDiscrete
  if (modelObject.availabilitySchedule().name()) {
    idfObject.setString(ElectricLoadCenter_Storage_SimpleFields::AvailabilityScheduleName, modelObject.availabilitySchedule().name().get());
  }

  // ZoneName
  if (modelObject.thermalZone() && modelObject.thermalZone().get().name()) {
    idfObject.setString(ElectricLoadCenter_Storage_SimpleFields::ZoneName, modelObject.thermalZone().get().name().get());
  }

  // Radiative Fraction, defaults (double)
  {
    double value = modelObject.radiativeFractionforZoneHeatGains();
    idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::RadiativeFractionforZoneHeatGains, value);
  }
  
  // nominalEnergeticEfficiencyforCharging, defaults
  {
    double value = modelObject.nominalEnergeticEfficiencyforCharging();
    idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::NominalEnergeticEfficiencyforCharging, value);
  }
  
  // nominalEnergeticEfficiencyforDischarging, defaults
  {
    double value = modelObject.nominalDischargingEnergeticEfficiency();
    idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::NominalDischargingEnergeticEfficiency, value);
  }
  
  // maximumStorageCapacity, required, assigned in ctor
  {
    double value = modelObject.maximumStorageCapacity();
    idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumStorageCapacity, value);
  }
  
  // maximumPowerforDischarging, required, assigned in ctor
  {
    double value = modelObject.maximumPowerforDischarging();
    idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforDischarging, value);
  }
  
  // maximumPowerforCharging, required, assigned in ctor
  {
    double value = modelObject.maximumPowerforCharging();
    idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforCharging, value);
  }
  
  // initialStateofCharge
  {
    double value = modelObject.initialStateofCharge();
    idfObject.setDouble(ElectricLoadCenter_Storage_SimpleFields::InitialStateofCharge, value);
  }
  

  return idfObject;
}

} // energyplus

} // openstudio

