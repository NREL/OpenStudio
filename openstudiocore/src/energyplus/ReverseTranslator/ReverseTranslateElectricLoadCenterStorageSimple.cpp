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

#include "../ReverseTranslator.hpp"

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

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateElectricLoadCenterStorageSimple( const WorkspaceObject & workspaceObject )
{

  OptionalModelObject result,temp;
  OptionalDouble d;
  boost::optional<WorkspaceObject> owo;
  OptionalString optS;
  
  
  // TODO: The availability schedule is in the ElectricLoadCenter:Generators (list) in E+, here it's carried by the generator itself
  // Should also get the Rated Thermal To Electrical Power Ratio in the list

  //Generator:MicroTurbine,
  //    Capstone C65,            !- Name

  openstudio::model::ElectricLoadCenterStorageSimple elcStorSimple( m_model );
  
  // Name
  optS = workspaceObject.name();
  if(optS)
  {
    elcStorSimple.setName(*optS);
  }

  // AvailabilityScheduleName
  if( (owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_SimpleFields::AvailabilityScheduleName)) )
  {
    if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
    {
      if( boost::optional<Schedule> schedule = mo->optionalCast<Schedule>() )
      {
        elcStorSimple.setAvailabilitySchedule(schedule.get());
      }
    }
  }
  
  // ZoneName
  if( (owo = workspaceObject.getTarget(ElectricLoadCenter_Storage_SimpleFields::ZoneName)) )
  {
    if( boost::optional<ModelObject> mo = translateAndMapWorkspaceObject(owo.get()) )
    {
      if( boost::optional<ThermalZone> thermalZone = mo->optionalCast<ThermalZone>() )
      {
        elcStorSimple.setThermalZone(thermalZone.get());
      }
    }
  }

  // Radiative Fraction, defaults (double)
  d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::RadiativeFractionforZoneHeatGains);
  if(d)
  {
    elcStorSimple.setRadiativeFractionforZoneHeatGains(*d);
  }
     
  // nominalEnergeticEfficiencyforCharging, defaults
  d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::NominalEnergeticEfficiencyforCharging);
  if(d)
  {
    elcStorSimple.setNominalEnergeticEfficiencyforCharging(*d);
  }
  
  // nominalEnergeticEfficiencyforDischarging, defaults
  d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::NominalDischargingEnergeticEfficiency);
  if(d)
  {
    elcStorSimple.setNominalDischargingEnergeticEfficiency(*d);
  }
  
  // maximumStorageCapacity, required, assigned in ctor
  d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumStorageCapacity);
  if(d) {
    elcStorSimple.setMaximumStorageCapacity(*d);
  } else {
    LOG(Error, workspaceObject.briefDescription() << " does not have a required maximum Storage Capacity");
  }
  
  // maximumPowerforDischarging, required, assigned in ctor
  d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforDischarging);
  if(d) {
    elcStorSimple.setMaximumPowerforDischarging(*d);
  } else {
    LOG(Error, workspaceObject.briefDescription() << " does not have a required maximum power for discharging");
  }
  
  // maximumPowerforCharging, required, assigned in ctor
  d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::MaximumPowerforCharging);
  if(d) {
    elcStorSimple.setMaximumPowerforCharging(*d);
  } else {
    LOG(Error, workspaceObject.briefDescription() << " does not have a required maximum power for charging");
  }
  
  // initialStateofCharge 
  d = workspaceObject.getDouble(ElectricLoadCenter_Storage_SimpleFields::InitialStateofCharge);
  if(d)
  {
    elcStorSimple.setInitialStateofCharge(*d);
  }
  
  result=elcStorSimple;
  return result;
}

} // energyplus

} // openstudio
