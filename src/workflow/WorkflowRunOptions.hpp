/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
