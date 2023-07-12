/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

  boost::optional<IdfObject> ForwardTranslator::translateAvailabilityManagerHybridVentilation(AvailabilityManagerHybridVentilation& modelObject) {
    IdfObject idfObject(IddObjectType::AvailabilityManager_HybridVentilation);
    m_idfObjects.push_back(idfObject);

    boost::optional<AirLoopHVAC> airLoopHVAC;
    if (auto loop = modelObject.loop()) {
      airLoopHVAC = loop->optionalCast<model::AirLoopHVAC>();
    }

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // HVACAirLoopName
    if (auto loop = modelObject.loop()) {
      idfObject.setString(AvailabilityManager_HybridVentilationFields::HVACAirLoopName, loop->name().get());
    }

    // ControlledZoneName
    if (auto zone = modelObject.controlledZone()) {
      idfObject.setString(AvailabilityManager_HybridVentilationFields::ControlZoneName, zone->name().get());
    } else {
      if (airLoopHVAC) {
        auto zones = airLoopHVAC->thermalZones();
        if (!zones.empty()) {
          auto default_zone = zones.front();
          LOG(Info, modelObject.briefDescription() << " is missing Control Zone Name, defaulting to " << default_zone.briefDescription() << ".");
          idfObject.setString(AvailabilityManager_HybridVentilationFields::ControlZoneName, default_zone.name().get());
        }
      }
    }

    // VentilationControlModeScheduleName
    {
      auto schedule = modelObject.ventilationControlModeSchedule();
      idfObject.setString(AvailabilityManager_HybridVentilationFields::VentilationControlModeScheduleName, schedule.name().get());
    }

    // UseWeatherFileRainIndicators
    {
      if (modelObject.useWeatherFileRainIndicators()) {
        idfObject.setString(AvailabilityManager_HybridVentilationFields::UseWeatherFileRainIndicators, "Yes");
      } else {
        idfObject.setString(AvailabilityManager_HybridVentilationFields::UseWeatherFileRainIndicators, "No");
      }
    }

    // MaximumWindSpeed
    {
      auto value = modelObject.maximumWindSpeed();
      idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MaximumWindSpeed, value);
    }

    // MinimumOutdoorTemperature
    {
      auto value = modelObject.minimumOutdoorTemperature();
      idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MinimumOutdoorTemperature, value);
    }

    // MaximumOutdoorTemperature
    {
      auto value = modelObject.maximumOutdoorTemperature();
      idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MaximumOutdoorTemperature, value);
    }

    // MinimumOutdoorEnthalpy
    {
      auto value = modelObject.minimumOutdoorEnthalpy();
      idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MinimumOutdoorEnthalpy, value);
    }

    // MaximumOutdoorEnthalpy
    {
      auto value = modelObject.maximumOutdoorEnthalpy();
      idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MaximumOutdoorEnthalpy, value);
    }

    // MinimumOutdoorDewpoint
    {
      auto value = modelObject.minimumOutdoorDewpoint();
      idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MinimumOutdoorDewpoint, value);
    }

    // MaximumOutdoorDewpoint
    {
      auto value = modelObject.maximumOutdoorDewpoint();
      idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MaximumOutdoorDewpoint, value);
    }

    // MinimumOutdoorVentilationAirScheduleName
    {
      auto schedule = modelObject.minimumOutdoorVentilationAirSchedule();
      idfObject.setString(AvailabilityManager_HybridVentilationFields::MinimumOutdoorVentilationAirScheduleName, schedule.name().get());
    }

    // OpeningFactorFunctionofWindSpeedCurveName
    if (auto curve = modelObject.openingFactorFunctionofWindSpeedCurve()) {
      auto idf = translateAndMapModelObject(curve.get());
      OS_ASSERT(idf);
      idfObject.setString(AvailabilityManager_HybridVentilationFields::OpeningFactorFunctionofWindSpeedCurveName, idf->name().get());
    }

    // AirflowNetwork Control Type Schedule Name
    if (auto oSch = modelObject.airflowNetworkControlTypeSchedule()) {
      auto idf = translateAndMapModelObject(oSch.get());
      OS_ASSERT(idf);
      idfObject.setString(AvailabilityManager_HybridVentilationFields::AirflowNetworkControlTypeScheduleName, idf->nameString());
    }

    // Simple Airflow Control Type Schedule Name
    if (auto oSch = modelObject.simpleAirflowControlTypeSchedule()) {
      auto idf = translateAndMapModelObject(oSch.get());
      OS_ASSERT(idf);
      idfObject.setString(AvailabilityManager_HybridVentilationFields::SimpleAirflowControlTypeScheduleName, idf->nameString());
    }

    // ZoneVentilation Object Name
    if (auto oMo = modelObject.zoneVentilationObject()) {
      auto idf = translateAndMapModelObject(oMo.get());
      // Model API shouldn't allow setting a wrong type (other than ZV:DesignFlowRate or ZV:WindAndStackOpenArea) anyways
      // and E+ will throw a clear warning if that were the case, so no need to check here
      OS_ASSERT(idf);
      idfObject.setString(AvailabilityManager_HybridVentilationFields::ZoneVentilationObjectName, idf->nameString());
    }

    // Minimum HVAC Operation Time
    {
      auto value = modelObject.minimumHVACOperationTime();
      idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MinimumHVACOperationTime, value);
    }

    // Minimum Ventilation Time
    {
      auto value = modelObject.minimumVentilationTime();
      idfObject.setDouble(AvailabilityManager_HybridVentilationFields::MinimumVentilationTime, value);
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
