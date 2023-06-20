/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/HeatBalanceAlgorithm.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/HeatBalanceAlgorithm_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatBalanceAlgorithm(HeatBalanceAlgorithm& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::HeatBalanceAlgorithm);

    idfObject.setString(HeatBalanceAlgorithmFields::Algorithm, modelObject.algorithm());

    idfObject.setDouble(HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit, modelObject.surfaceTemperatureUpperLimit());

    if (!modelObject.isMinimumSurfaceConvectionHeatTransferCoefficientValueDefaulted()) {
      idfObject.setDouble(HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue,
                          modelObject.minimumSurfaceConvectionHeatTransferCoefficientValue());
    }

    if (!modelObject.isMaximumSurfaceConvectionHeatTransferCoefficientValueDefaulted()) {
      idfObject.setDouble(HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue,
                          modelObject.maximumSurfaceConvectionHeatTransferCoefficientValue());
    }

    m_idfObjects.push_back(idfObject);

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
