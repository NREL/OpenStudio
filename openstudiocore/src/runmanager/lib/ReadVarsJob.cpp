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

#include <cstring>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "ReadVarsJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"

#include "../../utilities/time/DateTime.hpp"

#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  ReadVarsJob::ReadVarsJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, JobType::ReadVars, t_tools, "readvars", t_params, t_files, true, t_restoreData)
  {
  }

  ReadVarsJob::~ReadVarsJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  ToolVersion ReadVarsJob::getToolVersionImpl(const std::string &t_toolName) const
  {
    if (t_toolName != "readvars") {
      throw std::runtime_error("Invalid tool version request: " + t_toolName);
    }

    return ToolVersion();
  }

  std::string ReadVarsJob::descriptionImpl() const
  {
    // Build an appropriate description string and return it
    return "ReadVars" ;
  }

  void ReadVarsJob::startHandlerImpl()
  {
    getFiles(allInputFiles());

    if (m_eso)
    {

      // copy m_eso to "eplusout.eso"
      addRequiredFile(*m_eso, toPath("eplusout.eso"));

      // if have an rvi file
      if (m_rvi){

        // how can I add a parameter now?
        //m_params.append(JobParams("in.rvi"));

        // DLM@20100909: I would rather this just be named whatever it was...
        addRequiredFile(*m_rvi, toPath("in.rvi"));
      }
    }
  }

  void ReadVarsJob::basePathChanged()
  {
    m_eso.reset();
  }

  void ReadVarsJob::getFiles(const Files &t_files)
  {
    if (!m_eso)
    {
      try {
        m_eso = t_files.getLastByExtension("eso");
      } catch (const std::exception &) {
      }
    }
    if (!m_rvi)
    {
      try {
        m_rvi = t_files.getLastByExtension("rvi");
      } catch (const std::exception &) {
      }
    }
  }


} // detail
} // runmanager
} // openstudio



