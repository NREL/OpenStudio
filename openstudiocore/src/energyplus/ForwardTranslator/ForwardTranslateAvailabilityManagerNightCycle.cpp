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
#include "../../model/AvailabilityManagerNightCycle.hpp"
#include "../../model/AvailabilityManagerNightCycle_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/FanVariableVolume.hpp"
#include "../../model/FanVariableVolume_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/AvailabilityManager_NightCycle_FieldEnums.hxx>
#include <utilities/idd/ZoneList_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAvailabilityManagerNightCycle( 
    AvailabilityManagerNightCycle & modelObject)
{
  IdfObject idfObject(IddObjectType::AvailabilityManager_NightCycle);
  m_idfObjects.push_back(idfObject);

  boost::optional<AirLoopHVAC> airLoopHVAC;
  if( auto loop = modelObject.loop() ) {
    airLoopHVAC = loop->optionalCast<model::AirLoopHVAC>();
  }

  {
    auto schedule = modelObject.model().alwaysOnDiscreteSchedule();
    idfObject.setString(AvailabilityManager_NightCycleFields::ApplicabilityScheduleName,schedule.name().get());
  }

  if( airLoopHVAC ) {
    // Fan schedules are set to match the availabilitySchedule in the translator
    idfObject.setString(AvailabilityManager_NightCycleFields::FanScheduleName,airLoopHVAC->availabilitySchedule().name().get());
  }

  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  {
    auto value = modelObject.controlType();
    idfObject.setString(AvailabilityManager_NightCycleFields::ControlType,value);
  }

  {
    auto value = modelObject.thermostatTolerance();
    idfObject.setDouble(AvailabilityManager_NightCycleFields::ThermostatTolerance,value);
  }

  {
    auto value = modelObject.cyclingRunTime();
    idfObject.setDouble(AvailabilityManager_NightCycleFields::CyclingRunTime,value);
  }

  if( auto tz = modelObject.controlThermalZone() ) {
    idfObject.setString(AvailabilityManager_NightCycleFields::ControlZoneName,tz->name().get());
  }
  
  return idfObject;
}

} // energyplus
} // openstudio
