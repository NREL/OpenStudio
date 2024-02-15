/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSWorkflow.hpp"

#include "Util.hpp"

namespace openstudio {

void OSWorkflow::runCleanup() {
  state = State::Cleanup;

  LOG(Info, "Beginning cleanup of the run directory");
  openstudio::workflow::util::cleanup(workflowJSON.absoluteRunDir());
  LOG(Info, "Finished cleanup of the run directory");
}

}  // namespace openstudio
