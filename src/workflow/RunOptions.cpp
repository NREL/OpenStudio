/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "RunOptions.hpp"

#include <fmt/format.h>

namespace openstudio {

void WorkflowRunOptions::debug_print() const {
  fmt::print("\nRunOptions:\n");
  fmt::print("osw_path={}\n", this->osw_path.string());
  fmt::print("debug={}\n", this->debug);
  fmt::print("no_simulation={}\n", this->no_simulation);
  fmt::print("post_process={}\n", this->post_process);
  fmt::print("ep_json={}\n", this->ep_json);
  fmt::print("show_stdout={}\n", this->show_stdout);
  fmt::print("add_timings={}\n", this->add_timings);
  fmt::print("style_stdout={}\n", this->style_stdout);

  fmt::print("socket_port={}\n", this->socket_port);

  this->ft_options.debug_print();

  fmt::print("\n\n");
}

void FtOptions::debug_print() const {
  fmt::print("FtOptions:\n");
  fmt::print("  * runcontrolspecialdays={}\n", runcontrolspecialdays);
  fmt::print("  * ip_tabular_output={}\n", ip_tabular_output);
  fmt::print("  * no_lifecyclecosts={}\n", no_lifecyclecosts);
  fmt::print("  * no_sqlite_output={}\n", no_sqlite_output);
  fmt::print("  * no_html_output={}\n", no_html_output);
  fmt::print("  * no_space_translation={}\n", no_space_translation);
}

}  // namespace openstudio
