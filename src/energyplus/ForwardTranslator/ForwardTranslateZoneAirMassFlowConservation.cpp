/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/ZoneAirMassFlowConservation.hpp"
#include "../../model/ZoneAirMassFlowConservation_Impl.hpp"

#include <utilities/idd/ZoneAirMassFlowConservation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateZoneAirMassFlowConservation(ZoneAirMassFlowConservation& modelObject) {
    // Makes sure the modelObject gets put in the map, and that the new idfObject gets put in
    // the final file. Also set's the idfObject's name.
    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::ZoneAirMassFlowConservation, modelObject);

    idfObject.setString(ZoneAirMassFlowConservationFields::AdjustZoneMixingandReturnForAirMassFlowBalance,
                        modelObject.adjustZoneMixingandReturnForAirMassFlowBalance());

    idfObject.setString(ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod, modelObject.infiltrationBalancingMethod());

    idfObject.setString(ZoneAirMassFlowConservationFields::InfiltrationBalancingZones, modelObject.infiltrationBalancingZones());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
