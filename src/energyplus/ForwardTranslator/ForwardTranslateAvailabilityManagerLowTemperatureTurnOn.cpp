/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/AvailabilityManagerLowTemperatureTurnOn.hpp"
#include "../../model/Node.hpp"
#include <utilities/idd/AvailabilityManager_LowTemperatureTurnOn_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateAvailabilityManagerLowTemperatureTurnOn(AvailabilityManagerLowTemperatureTurnOn& modelObject) {
    IdfObject idfObject(IddObjectType::AvailabilityManager_LowTemperatureTurnOn);
    m_idfObjects.push_back(idfObject);

    // Name
    if (boost::optional<std::string> s = modelObject.name()) {
      idfObject.setName(*s);
    }

    // Sensor Node Name
    if (boost::optional<Node> node = modelObject.sensorNode()) {
      idfObject.setString(AvailabilityManager_LowTemperatureTurnOnFields::SensorNodeName, node->name().get());
    } else {
      // E+ will crash if this is missing (tested on 8.8.0)
      LOG(Error, modelObject.briefDescription() << " doesn't have a 'Sensor Node' (required)");
    }

    // Temperature
    {
      double value = modelObject.temperature();
      idfObject.setDouble(AvailabilityManager_LowTemperatureTurnOnFields::Temperature, value);
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
