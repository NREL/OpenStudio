/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
    idfObject.setString(AvailabilityManager_HybridVentilationFields::ControlZoneName,zone->name().get());
  } else {
    if( airLoopHVAC ) {
      auto zones = airLoopHVAC->thermalZones();
      if( ! zones.empty() ) {
        auto default_zone = zones.front();
        LOG(Info,modelObject.briefDescription() << " is missing Control Zone Name, defaulting to " << default_zone.briefDescription() << ".");
        idfObject.setString(AvailabilityManager_HybridVentilationFields::ControlZoneName,default_zone.name().get());
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
