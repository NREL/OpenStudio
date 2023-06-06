/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/PerformancePrecisionTradeoffs.hpp"
#include "../../model/PerformancePrecisionTradeoffs_Impl.hpp"

#include <utilities/idd/PerformancePrecisionTradeoffs_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePerformancePrecisionTradeoffs(PerformancePrecisionTradeoffs& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::PerformancePrecisionTradeoffs);

    m_idfObjects.push_back(idfObject);

    if (modelObject.useCoilDirectSolutions()) {
      idfObject.setString(PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions, "Yes");
    } else {
      idfObject.setString(PerformancePrecisionTradeoffsFields::UseCoilDirectSolutions, "No");
    }

    if (!modelObject.isZoneRadiantExchangeAlgorithmDefaulted()) {
      idfObject.setString(PerformancePrecisionTradeoffsFields::ZoneRadiantExchangeAlgorithm, modelObject.zoneRadiantExchangeAlgorithm());
    }

    if (!modelObject.isOverrideModeDefaulted()) {
      idfObject.setString(PerformancePrecisionTradeoffsFields::OverrideMode, modelObject.overrideMode());
    }

    if (!modelObject.isMaxZoneTempDiffDefaulted()) {
      idfObject.setDouble(PerformancePrecisionTradeoffsFields::MaxZoneTempDiff, modelObject.maxZoneTempDiff());
    }

    if (!modelObject.isMaxAllowedDelTempDefaulted()) {
      idfObject.setDouble(PerformancePrecisionTradeoffsFields::MaxAllowedDelTemp, modelObject.maxAllowedDelTemp());
    }

    if (modelObject.useRepresentativeSurfacesforCalculations()) {
      idfObject.setString(PerformancePrecisionTradeoffsFields::UseRepresentativeSurfacesforCalculations, "Yes");
    } else {
      idfObject.setString(PerformancePrecisionTradeoffsFields::UseRepresentativeSurfacesforCalculations, "No");
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
