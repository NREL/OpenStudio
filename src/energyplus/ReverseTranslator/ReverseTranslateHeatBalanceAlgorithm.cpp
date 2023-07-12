/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/HeatBalanceAlgorithm.hpp"
#include "../../model/HeatBalanceAlgorithm_Impl.hpp"

#include <utilities/idd/HeatBalanceAlgorithm_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<model::ModelObject> ReverseTranslator::translateHeatBalanceAlgorithm(const WorkspaceObject& workspaceObject) {
    OS_ASSERT(workspaceObject.iddObject().type() == IddObjectType::HeatBalanceAlgorithm);

    auto heatBalanceAlgorithm = m_model.getUniqueModelObject<HeatBalanceAlgorithm>();

    OptionalString s = workspaceObject.getString(HeatBalanceAlgorithmFields::Algorithm, false, true);
    if (s) {
      heatBalanceAlgorithm.setAlgorithm(*s);
    }

    OptionalDouble d = workspaceObject.getDouble(HeatBalanceAlgorithmFields::SurfaceTemperatureUpperLimit);
    if (d) {
      heatBalanceAlgorithm.setSurfaceTemperatureUpperLimit(*d);
    }

    d = workspaceObject.getDouble(HeatBalanceAlgorithmFields::MinimumSurfaceConvectionHeatTransferCoefficientValue);
    if (d) {
      heatBalanceAlgorithm.setMinimumSurfaceConvectionHeatTransferCoefficientValue(*d);
    }

    d = workspaceObject.getDouble(HeatBalanceAlgorithmFields::MaximumSurfaceConvectionHeatTransferCoefficientValue);
    if (d) {
      heatBalanceAlgorithm.setMaximumSurfaceConvectionHeatTransferCoefficientValue(*d);
    }

    return heatBalanceAlgorithm.cast<ModelObject>();
  }

}  // namespace energyplus
}  // namespace openstudio
