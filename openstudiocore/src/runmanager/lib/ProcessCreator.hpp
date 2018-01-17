/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef RUNMANAGER_LIB_PROCESSCREATOR_HPP
#define RUNMANAGER_LIB_PROCESSCREATOR_HPP

#include "Process.hpp"
#include "ToolInfo.hpp"
#include "../../utilities/core/UUID.hpp"

namespace openstudio {
namespace runmanager {

  /// Interface for Creating processes. Originally abstracted such that the Job did not need to
  /// care if the processes were running locally or remotely
  class ProcessCreator : public QObject
  {
    Q_OBJECT;

    public:
      /** \param[in] t_tooltoexecute The tool to execute locally
       *  \param[in] t_requiredFiles The vector of files needed to execute the tool
       *  \param[in] t_parameters The vector of parameters to pass to tool during execution
       *  \param[in] t_outdir Directory to watch for output files to be created 
       *  \param[in] t_expectedOutputFiles Output files in various directories to explicitly watch
       *  \param[in] t_stdin Input to send to the process over stdin after it has started
       *  \param[in] t_basePath Base path from which required files should be evaluated if the required file
       *                        is a relative path and does not reside in the tool path
       *  \returns the created Process */
      virtual std::shared_ptr<Process> createProcess(
          const ToolInfo &t_tooltoexecute,
          const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
          const std::vector<std::string> &t_parameters,
          const openstudio::path &t_outdir,
          const std::vector<openstudio::path> &t_expectedOutputFiles,
          const std::string &t_stdin,
          const openstudio::path &t_basePath) = 0;



  };

}
}

#endif // RUNMANAGER_LIB_PROCESSCREATOR_HPP
