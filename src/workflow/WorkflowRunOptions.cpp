/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WorkflowRunOptions.hpp"

#include <fmt/format.h>

namespace openstudio {

void WorkflowRunOptions::debug_print() const {
  fmt::print("\nWorkflowRunOptions:\n");
  fmt::print("osw_path={}\n", this->osw_path.string());
  fmt::print("no_simulation={}\n", this->no_simulation);
  fmt::print("show_stdout={}\n", this->show_stdout);
  fmt::print("add_timings={}\n", this->add_timings);
  fmt::print("style_stdout={}\n", this->style_stdout);
  fmt::print("socket_port={}\n", this->socket_port);

  fmt::print("\nrunOptions={}\n", this->runOptions.string());

  fmt::print("\n\n");
}

}  // namespace openstudio
