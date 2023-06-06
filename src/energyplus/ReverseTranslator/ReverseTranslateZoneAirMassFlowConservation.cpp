/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ZoneAirMassFlowConservation.hpp"
#include "../../model/ZoneAirMassFlowConservation_Impl.hpp"
#include "../../model/Model.hpp"

#include <utilities/idd/ZoneAirMassFlowConservation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneAirMassFlowConservation(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneAirMassFlowConservation) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneAirMassFlowConservation");
      return boost::none;
    }

    auto result = m_model.getUniqueModelObject<ZoneAirMassFlowConservation>();

    OptionalString s = workspaceObject.getString(openstudio::ZoneAirMassFlowConservationFields::AdjustZoneMixingandReturnForAirMassFlowBalance, true);
    if (s) {
      result.setAdjustZoneMixingandReturnForAirMassFlowBalance(s.get());
    }

    s = workspaceObject.getString(openstudio::ZoneAirMassFlowConservationFields::InfiltrationBalancingMethod, true);
    if (s) {
      result.setInfiltrationBalancingMethod(*s);
    }

    s = workspaceObject.getString(openstudio::ZoneAirMassFlowConservationFields::InfiltrationBalancingZones, true);
    if (s) {
      result.setInfiltrationBalancingZones(*s);
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
