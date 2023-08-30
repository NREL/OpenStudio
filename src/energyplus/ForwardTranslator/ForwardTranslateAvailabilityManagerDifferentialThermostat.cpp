/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/AvailabilityManagerDifferentialThermostat.hpp"
#include "../../model/AvailabilityManagerDifferentialThermostat_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/AvailabilityManager_DifferentialThermostat_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateAvailabilityManagerDifferentialThermostat(AvailabilityManagerDifferentialThermostat& modelObject) {
    IdfObject idfObject(IddObjectType::AvailabilityManager_DifferentialThermostat);
    m_idfObjects.push_back(idfObject);

    // Name
    if (auto s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // HotNodeName
    if (auto node = modelObject.hotNode()) {
      idfObject.setString(AvailabilityManager_DifferentialThermostatFields::HotNodeName, node->name().get());
    }

    // ColdNodeName
    if (auto node = modelObject.coldNode()) {
      idfObject.setString(AvailabilityManager_DifferentialThermostatFields::ColdNodeName, node->name().get());
    }

    // TemperatureDifferenceOnLimit
    {
      auto value = modelObject.temperatureDifferenceOnLimit();
      idfObject.setDouble(AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOnLimit, value);
    }

    // TemperatureDifferenceOffLimit
    {
      auto value = modelObject.temperatureDifferenceOffLimit();
      idfObject.setDouble(AvailabilityManager_DifferentialThermostatFields::TemperatureDifferenceOffLimit, value);
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
