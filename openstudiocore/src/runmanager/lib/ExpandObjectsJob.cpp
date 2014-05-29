/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

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



