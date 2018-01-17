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

#include "DakotaJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"

#include "../../utilities/time/DateTime.hpp"
#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {

namespace detail {

  DakotaJob::DakotaJob(const UUID &t_uuid,
                       const Tools &t_tools,
                       const JobParams &t_params,
                       const Files &t_files,
                       const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, 
                   JobType::Dakota,
                   t_tools, 
                   "dakota", 
                   t_params,
                   t_files,
                   true,
                   t_restoreData)
  {
    getFiles(t_files, t_params);

    m_description = buildDescription("in");
  }

  DakotaJob::~DakotaJob() {
    shutdownJob();
  }

  std::string DakotaJob::descriptionImpl() const
  {
    return m_description;
  }


  void DakotaJob::getFiles(const Files &t_files, const JobParams &t_params) const
  {
    // This function works with mutable local objects
    // because it's used to cache looked up variables
    if (!m_inFile) {
      try {
        // first, see if we can get a filename that was specifically requested by the job creation
        m_inFile = t_files.getLastByFilename(t_params.get("filename").value);
      } catch (const std::runtime_error &) {
        // if not, try to get any old .in that was passed in
        try {
          m_inFile = t_files.getLastByExtension("in");
        } 
        catch (const std::runtime_error &) {}
      }
    }
  }

  void DakotaJob::basePathChanged()
  {
    m_inFile.reset();
  }

  ToolVersion DakotaJob::getToolVersionImpl(const std::string &t_toolName) const
  {
    if (t_toolName != "dakota") {
      throw std::runtime_error("Invalid tool version request: " + t_toolName);
    }

    Files f = allInputFiles();
    JobParams p = params();
    getFiles(f, p);

    // minimum required version
    ToolVersion tv(5,3,1);

    return tv;
  }

  std::string DakotaJob::detailedDescriptionImpl() const
  {
    return "";
  }


  void DakotaJob::startHandlerImpl() {
    getFiles(allInputFiles(), params());

    if (!m_inFile)
    {
      throw std::runtime_error("No .in file found in input files");
    }

    LOG(Debug,"Starting DakotaJob with in file '" << toString(m_inFile->fullPath) << "'.");
    addRequiredFile(*m_inFile, toPath("dakota.in"));
    addParameter("dakota","-i");
    addParameter("dakota","dakota.in");

    // add additional parameters
    for (const JobParam& param : params().params()) {
      std::string val = param.value;
      if (val.find("-",0) == 0) {
        unsigned pos = val.find(" ",0);
        if (pos < val.size()) {
          std::string::iterator it = val.begin();
          for (unsigned i = 0; i < pos; ++i,++it);
          std::string temp(val.begin(),it);
          LOG(Debug,"Adding dakota parameter: " << temp);
          addParameter("dakota",temp);
          ++it;
          temp = std::string(it,val.end());
          LOG(Debug,"Adding dakota parameter: " << temp);
          addParameter("dakota",temp);
        }
        else {
          LOG(Debug,"Adding dakota parameter: " << val);
          addParameter("dakota",val);
        }
      }
    }
  }

} // detail

} // runmanager
} // openstudio

