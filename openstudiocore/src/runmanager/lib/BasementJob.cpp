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

#include "BasementJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "WeatherFileFinder.hpp"
#include "OpenStudio.hxx"

#include "../../utilities/time/DateTime.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include <utilities/idd/Version_FieldEnums.hxx>

#include <QDir>
#include <QDateTime>

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

    // Start at the currently configured minor version.
    ToolVersion tv(energyPlusVersionMajor(),energyPlusVersionMinor(),energyPlusVersionPatch(),energyPlusBuildSHA()); 

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



