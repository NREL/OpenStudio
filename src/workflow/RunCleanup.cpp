/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSWorkflow.hpp"

#include "Util.hpp"

namespace openstudio {

void OSWorkflow::runCleanup() {
  state = State::Cleanup;

  openstudio::workflow::util::cleanup(workflowJSON.absoluteRunDir());
}

}  // namespace openstudio
