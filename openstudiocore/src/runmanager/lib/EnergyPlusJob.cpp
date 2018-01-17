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

#include "EnergyPlusJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "WeatherFileFinder.hpp"
#include "OpenStudio.hxx"

#include "../../utilities/time/DateTime.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  EnergyPlusJob::EnergyPlusJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, JobType::EnergyPlus,
        t_tools, "energyplus", 
        t_params,
        t_files,
        true,
        t_restoreData)
  {
    getFiles(t_files, t_params);

    m_description = buildDescription("idf");
  }

  EnergyPlusJob::~EnergyPlusJob()
  {
    shutdownJob();
  }

  std::string EnergyPlusJob::descriptionImpl() const
  {
    return m_description;
  }


  void EnergyPlusJob::getFiles(const Files &t_files, const JobParams &t_params) const
  {
    // This function works with mutable local objects
    // because it's used to cache looked up variables
    if (!m_idf)
    {
      if (t_params.has("filename") && !t_params.get("filename").children.empty())
      {
        // first, see if we can get a filename that was specifically requested by the job creation
        std::string filename = t_params.get("filename").children[0].value;

        LOG(Info, "Looking for filename: " << filename); 

        std::vector<FileInfo> files = t_files.getAllByFilename(filename).files();
        if (!files.empty())
        {
          m_idf = files.back();
        } else {
          // a filename param was provided, but the file hasn't been generated ... yet
        }
      } else {
        // if not, try to get any old IDF that was passed in
        std::vector<FileInfo> files = t_files.getAllByExtension("idf").files();
        if (!files.empty())
        {
          m_idf = files.back();
        } else {
          // there are not any idf files at all
        }
      }
    }
  }

  void EnergyPlusJob::basePathChanged()
  {
    m_idf.reset();
  }

  ToolVersion EnergyPlusJob::getToolVersionImpl(const std::string &t_toolName) const
  {
    if (t_toolName != "energyplus") {
      throw std::runtime_error("Invalid tool version request: " + t_toolName);
    }

    Files f = allInputFiles();
    JobParams p = params();
    getFiles(f, p);

    // Start at the currently configured minor version.
    ToolVersion tv(energyPlusVersionMajor(),energyPlusVersionMinor(),energyPlusVersionPatch(),energyPlusBuildSHA()); 
    if (m_idf){
      if (boost::filesystem::exists(m_idf->fullPath))
      {
        OptionalIdfFile idf = IdfFile::load(m_idf->fullPath);
        if (idf){
          WeatherFileFinder::extractDetails(*idf, tv, m_filelocationname, m_weatherfilename);
        } else {
          throw std::runtime_error("Unable to load IDF " + toString(m_idf->fullPath) + " file could not be parsed");
        }
      } else {
        throw std::runtime_error("Unable to load IDF " + toString(m_idf->fullPath) + " file does not exist");
      }
    }

    return tv;
  }

  std::string EnergyPlusJob::detailedDescriptionImpl() const
  {
    if (m_idf)
    {
      if (m_idf->hasRequiredFile(toPath("in.epw")))
      {
        return toString(m_idf->getRequiredFile(toPath("in.epw")).first.toString());
      } 
    }

    // Fall through case
    return "";
  }


  void EnergyPlusJob::startHandlerImpl()
  {
    getFiles(allInputFiles(), params());

    if (!m_idf)
    {
      throw std::runtime_error("No IDF file found in input files");
    } 

    if (!m_idf->hasRequiredFile(toPath("Energy+.idd")))
    {
      // We did not have an idd set, so we should find one
      m_idf->addRequiredFile( toPath("Energy+.idd"), toPath("Energy+.idd"));
    }

    if (!m_idf->hasRequiredFile(toPath("in.epw")))
    {
      openstudio::path epw = WeatherFileFinder::find(allParams(), m_filelocationname, m_weatherfilename);

      if (!epw.empty())
      {
        m_idf->addRequiredFile(epw, toPath("in.epw"));
      }
    }


    // And add the now fully outfited idf with required idd and such
    addRequiredFile(*m_idf, toPath("in.idf"));

  }



} // detail
} // runmanager
} // openstudio



