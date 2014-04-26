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

#include "EnergyPlusJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "WeatherFileFinder.hpp"

#include <utilities/time/DateTime.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <QDir>
#include <QDateTime>

#include <boost/bind.hpp>

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

    // start at default EnergyPlus version of 8.1
    ToolVersion tv(8,1); 
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



