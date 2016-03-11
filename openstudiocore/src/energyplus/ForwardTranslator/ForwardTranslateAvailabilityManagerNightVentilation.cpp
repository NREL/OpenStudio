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
#include "../../model/AvailabilityManagerNightVentilation.hpp"
#include "../../model/AvailabilityManagerNightVentilation_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanVariableVolume_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/AvailabilityManager_NightVentilation_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAvailabilityManagerNightVentilation( 
    AvailabilityManagerNightVentilation & modelObject)
{
  IdfObject idfObject(IddObjectType::AvailabilityManager_NightVentilation);
  m_idfObjects.push_back(idfObject);

  boost::optional<AirLoopHVAC> airLoopHVAC;
  if( auto loop = modelObject.loop() ) {
    airLoopHVAC = loop->optionalCast<model::AirLoopHVAC>();
  }

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  // ApplicabilityScheduleName
  {
    auto schedule = modelObject.applicabilitySchedule();
    idfObject.setString(AvailabilityManager_NightVentilationFields::ApplicabilityScheduleName,schedule.name().get());
  }

  if( airLoopHVAC ) {
    // Fan schedules are set to match the availabilitySchedule in the translator
    idfObject.setString(AvailabilityManager_NightVentilationFields::FanScheduleName,airLoopHVAC->availabilitySchedule().name().get());
  }

  // VentilationTemperatureScheduleName
  if( auto schedule = modelObject.ventilationTemperatureSchedule() ) {
    idfObject.setString(AvailabilityManager_NightVentilationFields::VentilationTemperatureScheduleName,schedule->name().get());
  }

  // VentilationTemperatureDifference
  {
    auto value = modelObject.ventilationTemperatureDifference();
    idfObject.setDouble(AvailabilityManager_NightVentilationFields::VentilationTemperatureDifference,value);
  }

  // VentilationTemperatureLowLimit
  {
    auto value = modelObject.ventilationTemperatureLowLimit();
    idfObject.setDouble(AvailabilityManager_NightVentilationFields::VentilationTemperatureLowLimit,value);
  }

  // NightVentingFlowFraction
  {
    auto value = modelObject.nightVentingFlowFraction();
    idfObject.setDouble(AvailabilityManager_NightVentilationFields::NightVentingFlowFraction,value);
  }

  // ControlZoneName
  if( auto zone = modelObject.controlZone() ) {
    idfObject.setString(AvailabilityManager_NightVentilationFields::ControlZoneName,zone->name().get());
  } else {
    if( airLoopHVAC ) {
      auto zones = airLoopHVAC->thermalZones(); 
      if( ! zones.empty() ) {
        auto default_zone = zones.front();
        LOG(Info,modelObject.briefDescription() << " is missing Control Zone Name, defaulting to " << default_zone.briefDescription() << ".");
        idfObject.setString(AvailabilityManager_NightVentilationFields::ControlZoneName,default_zone.name().get());
      }
    }
  }

  return idfObject;
}

} // energyplus
} // openstudio
