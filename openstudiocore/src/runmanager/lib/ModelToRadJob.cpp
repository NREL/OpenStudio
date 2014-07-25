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
#include <cmath>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "ModelToRadJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "RunManager_Util.hpp"

#include "../../utilities/geometry/Transformation.hpp"
#include "../../utilities/time/DateTime.hpp"
#include "../../utilities/sql/SqlFile.hpp"

#include "../../model/Model.hpp"

#include "../../radiance/ForwardTranslator.hpp"

#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"

#include <QDir>
#include <QDateTime>

#include <boost/algorithm/string/regex.hpp>

namespace openstudio {
namespace runmanager {
namespace detail {


  ModelToRadJob::ModelToRadJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::ModelToRad, tools, params, files, t_restoreData)
  {
    try {
      m_model = files.getLastByExtension("osm");
      resetFiles(m_files, m_model);
    } catch (const std::runtime_error &) {
    }

    m_description = buildDescription("osm");
  }

  ModelToRadJob::~ModelToRadJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void ModelToRadJob::requestStop()
  {
  }

  bool ModelToRadJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    if (!t_lastrun)
    {
      return true;
    } else {
      QReadLocker l(&m_mutex);
      if (m_model && m_sql)
      {
        return filesChanged(m_files, *t_lastrun);
      } else {
        // if the model file we are using has not been established yet,
        // return outofdate
        return true;
      }
    }
  }

  void ModelToRadJob::cleanup()
  {
  }

  std::string ModelToRadJob::description() const
  {
    QReadLocker l(&m_mutex);
    return m_description;
  }



  void ModelToRadJob::startImpl(const std::shared_ptr<ProcessCreator> &)
  {
    openstudio::path outpath = outdir();
    QWriteLocker l(&m_mutex);
    JobErrors errors;
    errors.result = ruleset::OSResultValue::Success;
    
    try {
      if (!m_model)
      {
        m_model = modelFile();
      }
      if (!m_sql)
      {
        m_sql = sqlFile();
      }

      resetFiles(m_files, m_model);
    } catch (const std::runtime_error &e) {
      errors.result = ruleset::OSResultValue::Fail;
      errors.addError(ErrorType::Error, e.what());
    }

    if (!m_sql || !m_model)
    {
      errors.result = ruleset::OSResultValue::Fail;
      errors.addError(ErrorType::Error, "Unable to find required model or sql file");
    }

    LOG(Info, "ModelToRad starting, filename: " << toString(m_model->fullPath));
    LOG(Info, "ModelToRad starting, outdir: " << toString(outpath));

    l.unlock();

    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Starting));

    emitStarted();
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Processing));

    if (errors.result == ruleset::OSResultValue::Fail)
    {
      setErrors(errors);
      return;
    }


    try {
      boost::filesystem::create_directories(outpath);

      //
      // setup
      //
      LOG(Debug, "Working Directory: " + openstudio::toString(outpath));

      // get model
      boost::optional<openstudio::IdfFile> idf = openstudio::IdfFile::load(m_model->fullPath);
      openstudio::model::Model model = openstudio::model::Model(idf.get());

      // load the sql file
      openstudio::SqlFile sqlFile(m_sql->fullPath);

      if (!sqlFile.connectionOpen())
      {
        LOG(Error, "SqlFile connection is not open");
        errors.result = ruleset::OSResultValue::Fail;
        errors.addError(ErrorType::Error, "SqlFile collection is not open");
        setErrors(errors);
        return;
      }

      // set the sql file
      model.setSqlFile(sqlFile);
      if (!model.sqlFile())
      {
        LOG(Error, "SqlFile is not set on model");
        errors.result = ruleset::OSResultValue::Fail;
        errors.addError(ErrorType::Error, "SqlFile is not set on model");
        setErrors(errors);
        return;
      }

      openstudio::radiance::ForwardTranslator ft;
      std::vector<openstudio::path> outfiles = ft.translateModel(outpath, model);
      
      // capture translator errors and warnings?
      //ft.errors();
      //ft.warnings();
      
      Files outfileinfos;

      for (std::vector<openstudio::path>::const_iterator itr = outfiles.begin();
          itr != outfiles.end();
          ++itr)
      {
        FileInfo fi = RunManager_Util::dirFile(*itr);
        LOG(Info, "Output file generated: " << openstudio::toString(fi.fullPath));
        emitOutputFileChanged(fi);
        outfileinfos.append(fi);
      }


      std::vector<openstudio::LogMessage> logwarnings = ft.warnings();
      std::vector<openstudio::LogMessage> logerrors = ft.errors();

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

      if (!errors.errors().empty())
      {
        errors.result = ruleset::OSResultValue::Fail;
      }
      setErrors(errors);

      l.relock();
      m_outputfiles = outfileinfos;

    } catch (const std::runtime_error &e) {
      errors.addError(ErrorType::Error, "Error with conversion (runtime_error): " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
    }
    catch (const std::exception &e) {
      errors.addError(ErrorType::Error, "Error with conversion: " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
    }

    setErrors(errors);
  }

  std::string ModelToRadJob::getOutput() const
  {
    return "";
  }

  void ModelToRadJob::basePathChanged()
  {
    m_model.reset();
  }

  FileInfo ModelToRadJob::modelFile() const
  {
    if (!m_model)
    {
      std::shared_ptr<Job_Impl> p = parent();
      while (p)
      {
        if (p->jobType() == JobType::ModelToRadPreProcess)
        {
          // We have a parent that is a ModelToRadPreProcess job - so we want the
          // radiance job that *it* started with, which has not been pared down
          return p->allInputFiles().getLastByExtension("osm");
        }

        p = p->parent();
      }

      // if we make it through the above loop, we just want to return the last osm
      // in the input list
      return allInputFiles().getLastByExtension("osm");
    } else {
      return *m_model;
    }
  }

  FileInfo ModelToRadJob::sqlFile() const
  {
    if (!m_sql)
    {
      return allInputFiles().getLastByExtension("sql");
    } else {
      return *m_sql;
    }
  }

  Files ModelToRadJob::outputFilesImpl() const
  {
    openstudio::path outpath = outdir();

    QReadLocker l(&m_mutex);

    if (!boost::filesystem::exists(outpath / toPath("model.rad")) || !m_outputfiles)
    {
      // no output file has been generated yet
      return Files();
    }


    return *m_outputfiles;
  }


} // detail
} // runmanager
} // openstudio



