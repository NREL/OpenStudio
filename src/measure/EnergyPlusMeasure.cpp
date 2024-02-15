/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EnergyPlusMeasure.hpp"

#include "OSArgument.hpp"
#include "OSOutput.hpp"
#include "OSRunner.hpp"

#include "../utilities/idf/Workspace.hpp"

namespace openstudio {
namespace measure {

  EnergyPlusMeasure::EnergyPlusMeasure() : OSMeasure(MeasureType::EnergyPlusMeasure){};

  std::vector<OSArgument> EnergyPlusMeasure::arguments(const openstudio::Workspace& /*workspace*/) const {
    return {};
  }

  std::vector<OSOutput> EnergyPlusMeasure::outputs() const {
    return {};
  }

  bool EnergyPlusMeasure::run(openstudio::Workspace& /*workspace*/, OSRunner& runner,
                              const std::map<std::string, OSArgument>& /*user_arguments*/) const {
    runner.prepareForMeasureRun(*this);
    return true;
  }

}  // namespace measure
}  // namespace openstudio
