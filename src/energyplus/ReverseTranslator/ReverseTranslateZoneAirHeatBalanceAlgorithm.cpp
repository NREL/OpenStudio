/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/ZoneAirHeatBalanceAlgorithm.hpp"
#include "../../model/ZoneAirHeatBalanceAlgorithm_Impl.hpp"
#include <utilities/idd/ZoneAirHeatBalanceAlgorithm_FieldEnums.hxx>
#include <utilities/idd/OS_ZoneAirHeatBalanceAlgorithm_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneAirHeatBalanceAlgorithm(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneAirHeatBalanceAlgorithm) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneAirHeatBalanceAlgorithm");
      return boost::none;
    }

    auto mo = m_model.getUniqueModelObject<ZoneAirHeatBalanceAlgorithm>();

    if (auto algorithm_ = workspaceObject.getString(ZoneAirHeatBalanceAlgorithmFields::Algorithm, false, true)) {
      mo.setAlgorithm(algorithm_.get());
    }

    if (auto doSpaceHeatBalanceforSizing_ = workspaceObject.getString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSizing, false, true)) {
      if (istringEqual("Yes", doSpaceHeatBalanceforSizing_.get())) {
        mo.setDoSpaceHeatBalanceforSizing(true);
      } else {
        mo.setDoSpaceHeatBalanceforSizing(false);
      }
    }

    if (auto doSpaceHeatBalanceforSimulation_ =
          workspaceObject.getString(ZoneAirHeatBalanceAlgorithmFields::DoSpaceHeatBalanceforSimulation, false, true)) {
      if (istringEqual("Yes", doSpaceHeatBalanceforSimulation_.get())) {
        mo.setDoSpaceHeatBalanceforSimulation(true);
      } else {
        mo.setDoSpaceHeatBalanceforSimulation(false);
      }
    }

    return mo;
  }

}  // namespace energyplus

}  // namespace openstudio
