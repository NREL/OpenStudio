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

  openstudio::workflow::util::gatherReports(workflowJSON.absoluteRunDir(), workflowJSON.absoluteRootDir());
}

}  // namespace openstudio
