/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneAirContaminantBalance.hpp"
#include "../../model/ZoneAirContaminantBalance_Impl.hpp"
#include <utilities/idd/ZoneAirContaminantBalance_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneAirContaminantBalance(ZoneAirContaminantBalance& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::ZoneAirContaminantBalance);

    m_idfObjects.push_back(idfObject);

    {
      if (modelObject.carbonDioxideConcentration()) {
        idfObject.setString(ZoneAirContaminantBalanceFields::CarbonDioxideConcentration, "Yes");
      } else {
        idfObject.setString(ZoneAirContaminantBalanceFields::CarbonDioxideConcentration, "No");
      }
    }

    {
      if (auto schedule = modelObject.outdoorCarbonDioxideSchedule()) {
        idfObject.setString(ZoneAirContaminantBalanceFields::OutdoorCarbonDioxideScheduleName, schedule->nameString());
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
