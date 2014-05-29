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

