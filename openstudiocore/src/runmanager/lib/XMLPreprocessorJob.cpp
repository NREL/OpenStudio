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

#include "XMLPreprocessorJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include <utilities/idd/Version_FieldEnums.hxx>
#
#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  XMLPreprocessorJob::XMLPreprocessorJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, JobType::XMLPreprocessor, t_tools, "xmlpreprocessor", 
        t_params, t_files, true, t_restoreData)
  {
    getFiles(t_files);

    m_description = buildDescription("xml");
  }

  XMLPreprocessorJob::~XMLPreprocessorJob()
  {
    shutdownJob();
  }

  std::string XMLPreprocessorJob::descriptionImpl() const
  {
    return m_description;
  }

  ToolVersion XMLPreprocessorJob::getToolVersionImpl(const std::string &t_toolName) const
  {
    if (t_toolName != "xmlpreprocessor") {
      throw std::runtime_error("Invalid tool version request: " + t_toolName);
    }

    return ToolVersion();
  }

  void XMLPreprocessorJob::startHandlerImpl()
  {
    getFiles(allInputFiles());
    addRequiredFile(*m_xml, toPath("in.xml"));
    copyRequiredFiles(*m_xml, "idf", toPath("in.epw"));
  }

  void XMLPreprocessorJob::basePathChanged()
  {
    m_xml.reset();
  }

  void XMLPreprocessorJob::getFiles(const Files &t_files) const
  {
    if (!m_xml)
    {
      try {
        m_xml = t_files.getLastByExtension("xml");
        m_xml->addRequiredFile(toPath("include"), toPath("include"));
      } catch (const std::runtime_error &) {
      }
    }
  }

} // detail
} // runmanager
} // openstudio



