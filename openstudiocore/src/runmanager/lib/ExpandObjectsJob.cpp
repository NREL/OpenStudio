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

#include "ExpandObjectsJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include <utilities/idd/Version_FieldEnums.hxx>

#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  ExpandObjectsJob::ExpandObjectsJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, JobType::ExpandObjects, t_tools, "expandobjects", 
        t_params, t_files, false, t_restoreData)
  {
    getFiles(t_files);

    m_description = buildDescription("idf");
  }

  ExpandObjectsJob::~ExpandObjectsJob()
  {
    shutdownJob();
  }

  std::string ExpandObjectsJob::descriptionImpl() const
  {
    return m_description;
  }

  ToolVersion ExpandObjectsJob::getToolVersionImpl(const std::string &t_toolName) const
  {
    if (t_toolName != "expandobjects") {
      throw std::runtime_error("Invalid tool version request: " + t_toolName);
    }

    Files f = allInputFiles();
    getFiles(f);

    // start at default EnergyPlus version of 7.0
    ToolVersion tv(7,0); 

    if (m_idf){
      if (boost::filesystem::exists(m_idf->fullPath))
      {
        OptionalIdfFile idf = IdfFile::load(m_idf->fullPath);
        if (idf){
          boost::optional<std::string> filelocationname;
          boost::optional<std::string> weatherfilename;
          WeatherFileFinder::extractDetails(*idf, tv, filelocationname, weatherfilename);
        } else {
          throw std::runtime_error("Unable to load IDF " + toString(m_idf->fullPath) + " file could not be parsed");
        }
      } else {
        throw std::runtime_error("Unable to load IDF " + toString(m_idf->fullPath) + " file does not exist");
      }
    }

    return tv;
  }

  void ExpandObjectsJob::startHandlerImpl()
  {
    addExpectedOutputFile(openstudio::toPath("expanded.idf"));
    getFiles(allInputFiles());
    // copy idf to "in.idf"
    copyRequiredFiles(*m_idf, "idf", toPath("in.epw"));
    addRequiredFile(*m_idf, toPath("in.idf"));
  }

  void ExpandObjectsJob::basePathChanged()
  {
    m_idf.reset();
  }

  void ExpandObjectsJob::getFiles(const Files &t_files) const
  {
    if (!m_idf)
    {
      try {
        m_idf = t_files.getLastByExtension("idf");


        if (!m_idf->hasRequiredFile(toPath("Energy+.idd")))
        {
          m_idf->addRequiredFile( toPath("Energy+.idd"), toPath("Energy+.idd"));
        }

      } catch (const std::runtime_error &) {
        // no idf set
      }
    }
  }

} // detail
} // runmanager
} // openstudio



