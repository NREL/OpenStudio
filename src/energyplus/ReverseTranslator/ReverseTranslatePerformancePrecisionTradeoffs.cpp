/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/PerformancePrecisionTradeoffs.hpp"
#include "../../model/PerformancePrecisionTradeoffs_Impl.hpp"

#include <utilities/idd/PerformancePrecisionTradeoffs_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translatePerformancePrecisionTradeoffs(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::PerformancePrecisionTradeoffs) {
      LOG(Error, "WorkspaceObject is not IddObjectType: PerformancePrecisionTradeoffs");
      return boost::none;
    }

    PerformancePrecisionTradeoffs performancePrecisionTradeoffs = m_model.getUniqueModelObject<PerformancePrecisionTradeoffs>();

    OptionalString optS = workspaceObject.getString(PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        performancePrecisionTradeoffs.setUseCoilDirectSolutions(false);
      } else {
        performancePrecisionTradeoffs.setUseCoilDirectSolutions(true);
      }
    }

    if ((optS = workspaceObject.getString(PerformancePrecisionTradeoffsFields::ZoneRadiantExchangeAlgorithm))) {
      performancePrecisionTradeoffs.setZoneRadiantExchangeAlgorithm(optS.get());
    }

    if ((optS = workspaceObject.getString(PerformancePrecisionTradeoffsFields::OverrideMode))) {
      performancePrecisionTradeoffs.setOverrideMode(optS.get());
    }

    if (OptionalDouble optD = workspaceObject.getDouble(PerformancePrecisionTradeoffsFields::MaxZoneTempDiff)) {
      performancePrecisionTradeoffs.setMaxZoneTempDiff(optD.get());
    }

    if (OptionalDouble optD = workspaceObject.getDouble(PerformancePrecisionTradeoffsFields::MaxAllowedDelTemp)) {
      performancePrecisionTradeoffs.setMaxAllowedDelTemp(optD.get());
    }

    optS = workspaceObject.getString(PerformancePrecisionTradeoffsFields::UseRepresentativeSurfacesforCalculations);
    if (optS) {
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        performancePrecisionTradeoffs.setUseRepresentativeSurfacesforCalculations(false);
      } else {
        performancePrecisionTradeoffs.setUseRepresentativeSurfacesforCalculations(true);
      }
    }

    return performancePrecisionTradeoffs;
  }

}  // namespace energyplus

}  // namespace openstudio
