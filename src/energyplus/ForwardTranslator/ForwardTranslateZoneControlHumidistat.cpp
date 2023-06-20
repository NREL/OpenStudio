/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/ZoneControlHumidistat.hpp"
#include "../../model/Schedule.hpp"
#include <utilities/idd/ZoneControl_Humidistat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneControlHumidistat(ZoneControlHumidistat& modelObject) {
    boost::optional<IdfObject> result;

    auto humidifyingSchedule = modelObject.humidifyingRelativeHumiditySetpointSchedule();
    auto dehumidifyingSchedule = modelObject.dehumidifyingRelativeHumiditySetpointSchedule();

    if (humidifyingSchedule || dehumidifyingSchedule) {
      // Name
      result = createRegisterAndNameIdfObject(openstudio::IddObjectType::ZoneControl_Humidistat, modelObject);

      // Zone Name
      // set by ThermalZone

      // Humidifying Relative Humidity Setpoint Schedule Name
      if (humidifyingSchedule) {
        boost::optional<IdfObject> _humidifyingSchedule = translateAndMapModelObject(humidifyingSchedule.get());

        if (_humidifyingSchedule && _humidifyingSchedule->name()) {
          result->setString(ZoneControl_HumidistatFields::HumidifyingRelativeHumiditySetpointScheduleName, _humidifyingSchedule->name().get());
        }
      }

      // Dehumidifying Relative Humidity Setpoint Schedule Name
      if (dehumidifyingSchedule) {
        boost::optional<IdfObject> _dehumidifyingSchedule = translateAndMapModelObject(dehumidifyingSchedule.get());

        if (_dehumidifyingSchedule && _dehumidifyingSchedule->name()) {
          result->setString(ZoneControl_HumidistatFields::DehumidifyingRelativeHumiditySetpointScheduleName, _dehumidifyingSchedule->name().get());
        }
      }
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
