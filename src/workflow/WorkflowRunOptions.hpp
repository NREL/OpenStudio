/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef WORKFLOW_RUNOPTIONS_HPP
#define WORKFLOW_RUNOPTIONS_HPP

#include "../utilities/core/Filesystem.hpp"
#include "../utilities/filetypes/RunOptions.hpp"

namespace openstudio {

struct WorkflowRunOptions
{
 public:
  void debug_print() const;

  // TODO: move all these to the RunOption class?
  // Alter the jobs to be run
  bool no_simulation = false;
  bool post_process_only = false;

  // stdout stuff
  bool show_stdout = false;
  bool add_timings = false;
  bool style_stdout = false;

  // TODO: Remove
  unsigned socket_port = 0;

  openstudio::path osw_path = "./workflow.osw";

  RunOptions runOptions;
  /*
    bool m_debug = false;
    bool m_epjson = false;
    bool m_fast = false;
    bool m_preserveRunDir = false;
    bool m_skipExpandObjects = false;
    bool m_skipEnergyPlusPreprocess = false;
    ForwardTranslatorOptions m_forwardTranslatorOptions;
    boost::optional<CustomOutputAdapter> m_customOutputAdapter
*/
};

}  // namespace openstudio

#endif  // WORKFLOW_RUNOPTIONS_HPP
