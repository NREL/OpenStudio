#include "OSWorkflow.hpp"

#include "../utilities/idf/Workspace.hpp"

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/RunOptions.hpp"
#include "../utilities/core/Filesystem.hpp"

namespace openstudio {

void OSWorkflow::runEnergyPlusMeasures() {
  state = State::EnergyPlusMeasures;

  // Weather file is handled in runInitialization

  LOG(Info, "Beginning to execute EnergyPlus Measures.");
  applyMeasures(MeasureType::EnergyPlusMeasure, false);
  LOG(Info, "Finished applying EnergyPlus Measures.");

  saveIDFToRootDirIfDebug();
}
}  // namespace openstudio
