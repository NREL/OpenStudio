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



