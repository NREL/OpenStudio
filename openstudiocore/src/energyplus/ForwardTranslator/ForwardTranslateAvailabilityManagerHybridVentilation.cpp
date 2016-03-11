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
#include "../../model/AvailabilityManagerHybridVentilation.hpp"
#include "../../model/AvailabilityManagerHybridVentilation_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/AvailabilityManager_HybridVentilation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAvailabilityManagerHybridVentilation( 
    AvailabilityManagerHybridVentilation & modelObject)
{
  IdfObject idfObject(IddObjectType::AvailabilityManager_HybridVentilation);
  m_idfObjects.push_back(idfObject);

  boost::optional<AirLoopHVAC> airLoopHVAC;
  if( auto loop = modelObject.loop() ) {
    airLoopHVAC = loop->optionalCast<model::AirLoopHVAC>();
  }

  // Name
  if( auto s = modelObject.name() ) {
    idfObject.setName(*s);
  }

  // HVACAirLoopName
  if( auto loop = modelObject.loop() ) {
    idfObject.setString(AvailabilityManager_HybridVentilationFields::HVACAirLoopName,loop->name().get());
  }

  // ControlledZoneName
  if( auto zone = modelObject.controlledZone() ) {
    idfObject.setString(AvailabilityManager_HybridVentilationFields::ControlledZoneName,zone->name().get());
  } else {
    if( airLoopHVAC ) {
      auto zones = airLoopHVAC->thermalZones(); 
      if( ! zones.empty() ) {
        auto default_zone = zones.front();
        LOG(Info,modelObject.briefDescription() << " is missing Control Zone Name, defaulting to " << default_zone.briefDescription() << ".");
        idfObject.setString(AvailabilityManager_HybridVentilationFields::ControlledZoneName,default_zone.name().get());
      }
    }
  }

  // VentilationControlModeScheduleName
  {
    auto schedule = modelObject.ventilationControlModeSchedule();
    idfObject.setString(AvailabilityManager_HybridVentilationFields::VentilationControlModeScheduleName,schedule.name().get());    
  }

  // UseWeatherFileRainIndicators
  {
    if( modelObject.useWeatherFileRainIndicators() ) {
      idfObject.setString(AvailabilityManager_HybridVentilationFields::UseWeatherFileRainIndicators,"Yes");
    } else {
      idfObject.setString(AvailabilityManager_HybridVentilationFields::UseWeatherFileRainIndicators,"No");
    }
  }

  // MaximumWindSpeed
  {
    auto value = modelObject.maximumWindSpeed();
    idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MaximumWindSpeed,value);
  }

  // MinimumOutdoorTemperature
  {
    auto value = modelObject.minimumOutdoorTemperature();
    idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MinimumOutdoorTemperature,value);
  }

  // MaximumOutdoorTemperature
  {
    auto value = modelObject.maximumOutdoorTemperature();
    idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MaximumOutdoorTemperature,value);
  }

  // MinimumOutdoorEnthalpy
  {
    auto value = modelObject.minimumOutdoorEnthalpy();
    idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MinimumOutdoorEnthalpy,value);
  }

  // MaximumOutdoorEnthalpy
  {
    auto value = modelObject.maximumOutdoorEnthalpy();
    idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MaximumOutdoorEnthalpy,value);
  }

  // MinimumOutdoorDewpoint
  {
    auto value = modelObject.minimumOutdoorDewpoint();
    idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MinimumOutdoorDewpoint,value);
  }

  // MaximumOutdoorDewpoint
  {
    auto value = modelObject.maximumOutdoorDewpoint();
    idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MaximumOutdoorDewpoint,value);
  }

  // MinimumOutdoorVentilationAirScheduleName
  { 
    auto schedule = modelObject.minimumOutdoorVentilationAirSchedule();
    idfObject.setString(AvailabilityManager_HybridVentilationFields::MinimumOutdoorVentilationAirScheduleName,schedule.name().get());
  }

  // OpeningFactorFunctionofWindSpeedCurveName
  if( auto curve = modelObject.openingFactorFunctionofWindSpeedCurve() ) {
    auto idf = translateAndMapModelObject(curve.get());
    OS_ASSERT(idf);
    idfObject.setString(AvailabilityManager_HybridVentilationFields::OpeningFactorFunctionofWindSpeedCurveName,idf->name().get());
  }

  return idfObject;
}

} // energyplus
} // openstudio
