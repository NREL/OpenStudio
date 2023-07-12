/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ConvergenceLimits.hpp"
#include "../../model/ConvergenceLimits_Impl.hpp"

#include <utilities/idd/ConvergenceLimits_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/core/Optional.hpp"

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<model::ModelObject> ReverseTranslator::translateConvergenceLimits(const WorkspaceObject& workspaceObject) {
    OS_ASSERT(workspaceObject.iddObject().type() == IddObjectType::ConvergenceLimits);

    OptionalInt i;

    auto convergenceLimits = m_model.getUniqueModelObject<ConvergenceLimits>();

    if ((i = workspaceObject.getInt(ConvergenceLimitsFields::MinimumSystemTimestep))) {
      convergenceLimits.setMinimumSystemTimestep(*i);
    }

    if ((i = workspaceObject.getInt(ConvergenceLimitsFields::MaximumHVACIterations))) {
      convergenceLimits.setMaximumHVACIterations(*i);
    }

    if ((i = workspaceObject.getInt(ConvergenceLimitsFields::MinimumPlantIterations))) {
      convergenceLimits.setMinimumPlantIterations(*i);
    }

    if ((i = workspaceObject.getInt(ConvergenceLimitsFields::MaximumPlantIterations))) {
      convergenceLimits.setMaximumPlantIterations(*i);
    }

    return convergenceLimits;
  }

}  // namespace energyplus
}  // namespace openstudio
