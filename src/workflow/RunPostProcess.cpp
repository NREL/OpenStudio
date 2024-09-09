/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSWorkflow.hpp"

#include "Util.hpp"
#include "workflow/Util.hpp"

namespace openstudio {

void OSWorkflow::runPostProcess() {
  state = State::PostProcess;

  if (!workflowJSON.runOptions()->fast()) {
    LOG(Info, "Gathering reports");
    openstudio::workflow::util::gatherReports(workflowJSON.absoluteRunDir(), workflowJSON.absoluteRootDir());
    LOG(Info, "Finished gathering reports");
  }
  // If no points have been exported, skip file creation
  if (runner.alfalfa().getPoints().size() > 0) {
    LOG(Info, "Saving Alfalfa points");
    auto savePath = workflowJSON.absoluteRunDir() / "alfalfa.json";
    runner.alfalfa().saveAs(savePath);
    LOG(Info, "Finished saving Alfalfa points");
  }
}

}  // namespace openstudio
