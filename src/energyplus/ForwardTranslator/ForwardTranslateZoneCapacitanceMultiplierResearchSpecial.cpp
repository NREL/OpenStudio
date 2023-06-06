/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/ZoneCapacitanceMultiplierResearchSpecial.hpp"

#include <utilities/idd/ZoneCapacitanceMultiplier_ResearchSpecial_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateZoneCapacitanceMultiplierResearchSpecial(ZoneCapacitanceMultiplierResearchSpecial& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::ZoneCapacitanceMultiplier_ResearchSpecial);

    m_idfObjects.push_back(idfObject);

    // This is a unique model object *in OpenStudio*
    idfObject.setName("Zone Capacitance Multiplier Research Special");

    if (!modelObject.isTemperatureCapacityMultiplierDefaulted()) {
      idfObject.setDouble(ZoneCapacitanceMultiplier_ResearchSpecialFields::TemperatureCapacityMultiplier,
                          modelObject.temperatureCapacityMultiplier());
    }

    if (!modelObject.isHumidityCapacityMultiplierDefaulted()) {
      idfObject.setDouble(ZoneCapacitanceMultiplier_ResearchSpecialFields::HumidityCapacityMultiplier, modelObject.humidityCapacityMultiplier());
    }

    if (!modelObject.isCarbonDioxideCapacityMultiplierDefaulted()) {
      idfObject.setDouble(ZoneCapacitanceMultiplier_ResearchSpecialFields::CarbonDioxideCapacityMultiplier,
                          modelObject.carbonDioxideCapacityMultiplier());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
