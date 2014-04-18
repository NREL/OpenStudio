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

#include "SlabJob.hpp"
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

  SlabJob::SlabJob(const UUID &t_uuid,
      const Tools &t_tools,
      const JobParams &t_params,
      const Files &t_files,
      const JobState &t_restoreData)
    : ToolBasedJob(t_uuid, JobType::Slab, t_tools, "slab", 
        t_params, t_files, false, t_restoreData)
  {
    getFiles(t_files);

    m_description = buildDescription("idf");
  }

  SlabJob::~SlabJob()
  {
    shutdownJob();
  }

  std::string SlabJob::descriptionImpl() const
  {
    return m_description;
  }

  ToolVersion SlabJob::getToolVersionImpl(const std::string &t_toolName) const
  {
    if (t_toolName != "slab") {
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

  void SlabJob::startHandlerImpl()
  {
    addExpectedOutputFile(openstudio::toPath("slabmerged.idf"));

    getFiles(allInputFiles());

    if (!m_idf)
    {
      throw std::runtime_error("No IDF file found in input files");
    } 

    if (!m_expandedidf)
    {
      throw std::runtime_error("No expanded IDF file found in input files");
    } 

    copyRequiredFiles(*m_idf, "idf", toPath("in.epw"));

    try {
      m_idf->getRequiredFile(toPath("SlabGHT.idd"));
    } catch (const std::runtime_error &) {
      // We did not have an idd set, so we should find one
      m_idf->addRequiredFile( toPath("SlabGHT.idd"), toPath("SlabGHT.idd"));
    }

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
    addRequiredFile(*m_idf, toPath("GHTIn.idf"));
  }

  void SlabJob::endHandlerImpl() 
  {
    openstudio::path outpath = outdir();

    openstudio::path epobjects(outpath / openstudio::toPath("SLABSurfaceTemps.TXT"));

    openstudio::path outfile = outpath / openstudio::toPath("slabmerged.idf");

    LOG(Debug, "Copying file: " <<  openstudio::toString(m_expandedidf->fullPath) << " to " << openstudio::toString(outfile));
    boost::filesystem::remove(outfile);
    boost::filesystem::copy_file(m_expandedidf->fullPath, outfile, boost::filesystem::copy_option::overwrite_if_exists);


    std::fstream f(openstudio::toString(epobjects).c_str(), std::fstream::in|std::fstream::binary);
    f << std::noskipws;
    std::istream_iterator<unsigned char> begin(f);
    std::istream_iterator<unsigned char> end;

    std::fstream f2(openstudio::toString(outfile).c_str(),
        std::fstream::out|std::fstream::app|std::fstream::binary);
    std::ostream_iterator<char> begin2(f2);

    LOG(Debug, "Appending file: " <<  openstudio::toString(epobjects) << " to " << openstudio::toString(outfile));
    std::copy(begin, end, begin2);
  }

  Files SlabJob::outputFilesHandlerImpl() const
  {
    FileInfo fi(outdir() / openstudio::toPath("slabmerged.idf"), "idf");
    fi.requiredFiles = m_expandedidf->requiredFiles;

    Files f;
    f.append(fi);
    return f;
  }

  void SlabJob::basePathChanged()
  {
    m_idf.reset();
    m_expandedidf.reset();
  }


  void SlabJob::getFiles(const Files &t_files) const
  {
    if (!m_idf)
    {
      try {
        m_idf = t_files.getLastByFilename("GHTIn.idf");
        LOG(Debug, "GHT Idf loaded: " << openstudio::toString(m_idf->fullPath));
        m_expandedidf = t_files.getLastByExtension("idf");
        LOG(Debug, "Expanded Idf loaded: " << openstudio::toString(m_expandedidf->fullPath));
      } catch (const std::exception &) {
      }
    }
  }

} // detail
} // runmanager
} // openstudio



