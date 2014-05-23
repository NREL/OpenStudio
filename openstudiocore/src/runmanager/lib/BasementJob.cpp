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

#include "BasementJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "WeatherFileFinder.hpp"

#include <utilities/time/DateTime.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idd/Version_FieldEnums.hxx>

#include <QDir>
#include <QDateTime>

#include <boost/bind.hpp>

namespace openstudio {
namespace runmanager {
namespace detail {

  BasementJob::BasementJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, JobType::Basement, t_tools, "basement", 
        t_params, t_files, false, t_restoreData)
  {
    getFiles(t_files);

    m_description = buildDescription("idf");
  }

  BasementJob::~BasementJob()
  {
    shutdownJob();
  }

  std::string BasementJob::descriptionImpl() const
  {
    return m_description;
  }

  ToolVersion BasementJob::getToolVersionImpl(const std::string &t_toolName) const
  {
    if (t_toolName != "basement") {
      throw std::runtime_error("Invalid tool version request: " + t_toolName);
    }

    Files f = allInputFiles();
    getFiles(f);

    // start at default EnergyPlus version of 8.1
    ToolVersion tv(8,1); 

    if (m_expandedidf){
      if (boost::filesystem::exists(m_expandedidf->fullPath))
      {
        OptionalIdfFile idf = IdfFile::load(m_expandedidf->fullPath);
        if (idf){
          WeatherFileFinder::extractDetails(*idf, tv, m_filelocationname, m_weatherfilename);
        } else {
          throw std::runtime_error("Unable to load IDF " + toString(m_expandedidf->fullPath) + " file could not be parsed");
        }
      } else {
        throw std::runtime_error("Unable to load IDF " + toString(m_expandedidf->fullPath) + " file does not exist");
      }
    }

    return tv;
  }

  void BasementJob::startHandlerImpl()
  {
    addExpectedOutputFile(openstudio::toPath("basementmerged.idf"));

    getFiles(allInputFiles());

    if (!m_idf)
    {
      throw std::runtime_error("No IDF file found in input files");
    } 

    if (!m_expandedidf)
    {
      throw std::runtime_error("No expanded IDF file found in input files");
    } 

    try {
      m_idf->getRequiredFile(toPath("BasementGHT.idd"));
    } catch (const std::runtime_error &) {
      // We did not have an idd set, so we should find one
      m_idf->addRequiredFile( toPath("BasementGHT.idd"), toPath("BasementGHT.idd"));
    }

    copyRequiredFiles(*m_idf, "idf", toPath("in.epw"));

    try {
      m_idf->getRequiredFile(toPath("in.epw")); 
    } catch (const std::runtime_error &) {
      openstudio::path epw = WeatherFileFinder::find(allParams(), m_filelocationname, m_weatherfilename);

      if (!epw.empty())
      {
        m_idf->addRequiredFile(epw, toPath("in.epw"));
      }
    }


    // And add the now fully outfited idf with required idd and such
    addRequiredFile(*m_idf, toPath("BasementGHTIn.idf"));
  }

  void BasementJob::endHandlerImpl() 
  {
    openstudio::path epobjects(outdir() / openstudio::toPath("EPObjects.TXT"));

    openstudio::path outfile = outdir() / openstudio::toPath("basementmerged.idf");

    boost::filesystem::remove(outfile);
    boost::filesystem::copy_file(m_expandedidf->fullPath, outfile, boost::filesystem::copy_option::overwrite_if_exists);


    std::fstream f(openstudio::toString(epobjects).c_str(), std::fstream::in|std::fstream::binary);
    f << std::noskipws;
    std::istream_iterator<unsigned char> begin(f);
    std::istream_iterator<unsigned char> end;

    std::fstream f2(openstudio::toString(outfile).c_str(),
        std::fstream::out|std::fstream::app|std::fstream::binary);
    std::ostream_iterator<char> begin2(f2);

    std::copy(begin, end, begin2);
  }

  Files BasementJob::outputFilesHandlerImpl() const
  {
    FileInfo fi(outdir() / openstudio::toPath("basementmerged.idf"), "idf");
    fi.requiredFiles = m_expandedidf->requiredFiles;

    Files f;
    f.append(fi);
    return f;
  }


  void BasementJob::getFiles(const Files &t_files) const
  {
    if (!m_idf)
    {
      try {
        m_idf = t_files.getLastByFilename("BasementGHTIn.idf");
        m_expandedidf = t_files.getLastByExtension("idf");
      } catch (const std::runtime_error&) {
      }
    }
  }

  void BasementJob::basePathChanged()
  {
    m_idf.reset();
    m_expandedidf.reset();
  }

} // detail
} // runmanager
} // openstudio



