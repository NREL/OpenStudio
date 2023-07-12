/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/ZoneAirHeatBalanceAlgorithm.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/ZoneAirHeatBalanceAlgorithm_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneAirHeatBalanceAlgorithm(ZoneAirHeatBalanceAlgorithm& modelObject) {

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::ZoneAirHeatBalanceAlgorithm, modelObject);
    idfObject.setString(ZoneAirHeatBalanceAlgorithmFields::Algorithm, modelObject.algorithm());

    if (modelObject.doSpaceHeatBalanceforSizing()) {
      idfObject.setString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing, "Yes");
    } else {
      idfObject.setString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing, "No");
    }

    if (modelObject.doSpaceHeatBalanceforSimulation()) {
      idfObject.setString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation, "Yes");
    } else {
      idfObject.setString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation, "No");
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
