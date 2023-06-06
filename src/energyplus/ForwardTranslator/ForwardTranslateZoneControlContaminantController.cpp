/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneControlContaminantController.hpp"
#include "../../model/ZoneControlContaminantController_Impl.hpp"
#include <utilities/idd/ZoneControl_ContaminantController_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneControlContaminantController(ZoneControlContaminantController& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::ZoneControl_ContaminantController);

    m_idfObjects.push_back(idfObject);

    {
      auto s = modelObject.name();
      idfObject.setName(s.get());
    }

    if (auto tz = modelObject.getImpl<model::detail::ZoneControlContaminantController_Impl>()->controlledZone()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::ZoneName, tz->name().get());
    }

    if (auto s = modelObject.carbonDioxideControlAvailabilitySchedule()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::CarbonDioxideControlAvailabilityScheduleName, s->name().get());
    }

    if (auto s = modelObject.carbonDioxideSetpointSchedule()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::CarbonDioxideSetpointScheduleName, s->name().get());
    }

    if (auto s = modelObject.minimumCarbonDioxideConcentrationSchedule()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::MinimumCarbonDioxideConcentrationScheduleName, s->name().get());
    }

    if (auto s = modelObject.genericContaminantControlAvailabilitySchedule()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::GenericContaminantControlAvailabilityScheduleName, s->name().get());
    }

    if (auto s = modelObject.genericContaminantSetpointSchedule()) {
      idfObject.setString(ZoneControl_ContaminantControllerFields::GenericContaminantSetpointScheduleName, s->name().get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
