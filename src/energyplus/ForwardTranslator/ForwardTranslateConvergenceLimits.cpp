/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/ConvergenceLimits.hpp"
#include <utilities/idd/ConvergenceLimits_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateConvergenceLimits(ConvergenceLimits& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::ConvergenceLimits);

    if (OptionalInt i = modelObject.minimumSystemTimestep()) {
      idfObject.setInt(ConvergenceLimitsFields::MinimumSystemTimestep, *i);
    } else {
      // EnergyPlus IDD has a note saying 1 is the default but does not actually list it as the default
      // A severe error message is issued if this field is blank
      idfObject.setInt(ConvergenceLimitsFields::MinimumSystemTimestep, 1);
    }

    if (!modelObject.isMaximumHVACIterationsDefaulted()) {
      idfObject.setInt(ConvergenceLimitsFields::MaximumHVACIterations, modelObject.maximumHVACIterations());
    }

    if (!modelObject.isMinimumPlantIterationsDefaulted()) {
      idfObject.setInt(ConvergenceLimitsFields::MinimumPlantIterations, modelObject.minimumPlantIterations());
    }

    if (!modelObject.isMaximumPlantIterationsDefaulted()) {
      idfObject.setInt(ConvergenceLimitsFields::MaximumPlantIterations, modelObject.maximumPlantIterations());
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
