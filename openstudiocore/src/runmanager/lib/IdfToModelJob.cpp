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

#include "IdfToModelJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "RunManager_Util.hpp"

#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/WeatherFile.hpp"
#include "../../model/WeatherFile_Impl.hpp"

#include "../../energyplus/ReverseTranslator.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/time/DateTime.hpp"


#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {


  IdfToModelJob::IdfToModelJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::IdfToModel, tools, params, files, t_restoreData)
  {
    try {
      m_idf = files.getLastByExtension("idf");
      resetFiles(m_files, m_idf);
    } catch (const std::runtime_error &) {
    }

    m_description = buildDescription("idf");
  }

  IdfToModelJob::~IdfToModelJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void IdfToModelJob::requestStop()
  {
  }

  bool IdfToModelJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    if (!t_lastrun)
    {
      return true;
    } else {
      QReadLocker l(&m_mutex);
      if (m_idf)
      {
        return filesChanged(m_files, *t_lastrun);
      } else {
        // if the model file we are using has not been establised yet,
        // return outofdate
        return true;
      }
    }
  }

  void IdfToModelJob::cleanup()
  {
  }

  std::string IdfToModelJob::description() const
  {
    QReadLocker l(&m_mutex);
    return m_description;
  }

  void IdfToModelJob::startImpl(const std::shared_ptr<ProcessCreator> &)
  {
    openstudio::path outpath = outdir(true);

    QWriteLocker l(&m_mutex);
    if (!m_idf)
    {
      m_idf = idfFile();
      resetFiles(m_files, m_idf);
    }

    LOG(Info, "IdfToModel starting, filename: " << toString(m_idf->fullPath));
    LOG(Info, "IdfToModel starting, outdir: " << toString(outpath));

    JobErrors errors;
    errors.result = ruleset::OSResultValue::Success;
    l.unlock();

    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Starting));

    emitStarted();
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Processing));

    openstudio::energyplus::ReverseTranslator rt;

    try {
      boost::filesystem::create_directories(outpath);

      LOG(Debug, "Loading input file: " + toString(m_idf->fullPath));

      boost::optional<openstudio::model::Model> model;
      boost::optional<openstudio::Workspace> workspace = openstudio::Workspace::load(m_idf->fullPath);
      if (workspace){
        model = rt.translateWorkspace(*workspace);
      }

      OS_ASSERT(model);

      openstudio::path outfile = outpath / openstudio::toPath("out.osm");

      LOG(Debug, "Saving to path: " + toString(outfile));
      model->save(outfile, true);
      emitOutputFileChanged(RunManager_Util::dirFile(outfile));

    } catch (const std::exception &e) {
      LOG(Error, std::string("Error with IdfToModel translation: ") + e.what());
      errors.addError(ErrorType::Error, "Error with conversion: " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
    }

    std::vector<openstudio::LogMessage> logwarnings = rt.warnings();
    std::vector<openstudio::LogMessage> logerrors = rt.errors();

    for (std::vector<openstudio::LogMessage>::const_iterator itr = logwarnings.begin();
        itr != logwarnings.end();
        ++itr)
    {
      errors.addError(ErrorType::Warning, itr->logMessage());
    }

    for (std::vector<openstudio::LogMessage>::const_iterator itr = logerrors.begin();
        itr != logerrors.end();
        ++itr)
    {
      errors.addError(ErrorType::Error, itr->logMessage());
    }

    setErrors(errors);
  }

  std::string IdfToModelJob::getOutput() const
  {
    return "";
  }

  void IdfToModelJob::basePathChanged()
  {
    m_idf.reset();
    resetFiles(m_files);
  }

  FileInfo IdfToModelJob::idfFile() const
  {
    if (!m_idf)
    {
      return allInputFiles().getLastByExtension("idf");
    } else {
      return *m_idf;
    }
  }

  /*
  std::vector<FileInfo> IdfToModelJob::inputFiles() const
  {
    std::vector<FileInfo> infiles;
    infiles.push_back(modelFile());
    return infiles;
  }
  */

  Files IdfToModelJob::outputFilesImpl() const
  {
    openstudio::path outpath = outdir();

    if (!boost::filesystem::exists(outpath / toPath("out.osm")))
    {
      // no output file has been generated yet
      return Files();
    }

    QWriteLocker l(&m_mutex);

    if (!m_outputfiles)
    {
      // check if model has a weather file object
      boost::optional<QUrl> weatherFilePath;
      try {
        FileInfo idfFile = this->idfFile();

        try {
          std::pair<QUrl, openstudio::path> f = idfFile.getRequiredFile(toPath("in.epw"));
          LOG(Debug, "Setting user defined epw: " << toString(f.first.toString()));
          weatherFilePath = f.first;
        } catch (const std::exception &) {
        }

        // Specify the set of files we created so that the next Job in the chain (if there is one)
        // is able to pick them up
        Files outfiles;

        FileInfo osm(outpath / toPath("out.osm"), "osm");

        if (weatherFilePath){
          osm.addRequiredFile(*weatherFilePath, toPath("in.epw"));
        }else{
          LOG(Warn, "No weather file specified");
        }

        outfiles.append(osm);
        m_outputfiles = outfiles;
      } catch (const std::exception &) {
        LOG(Warn, "OSM file not yet available, outputfiles not known");
        return Files();
      }
    }

    return *m_outputfiles;
  }


} // detail
} // runmanager
} // openstudio



