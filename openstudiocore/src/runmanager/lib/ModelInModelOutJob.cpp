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

#include "ModelInModelOutJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "RunManager_Util.hpp"
#include "MergeJobError.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/WeatherFile.hpp"
#include "../../model/WeatherFile_Impl.hpp"
#include "../../energyplus/ForwardTranslator.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>

#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {


  ModelInModelOutJob::ModelInModelOutJob(
          const UUID &t_uuid,
          JobType t_type,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, t_type, tools, params, files, t_restoreData)
  {
    try {
      m_model = files.getLastByExtension("osm");
      resetFiles(m_files, m_model);
    } catch (const std::runtime_error &) {
    }

    m_description = buildDescription("osm");
  }

  ModelInModelOutJob::~ModelInModelOutJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void ModelInModelOutJob::requestStop()
  {
  }

  bool ModelInModelOutJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    if (!t_lastrun)
    {
      return true;
    } else {
      QReadLocker l(&m_mutex);
      if (m_model)
      {
        return filesChanged(m_files, *t_lastrun);
      } else {
        // if the model file we are using has not been establised yet,
        // return outofdate
        return true;
      }
    }
  }

  void ModelInModelOutJob::cleanup()
  {
  }

  std::string ModelInModelOutJob::description() const
  {
    QReadLocker l(&m_mutex);
    return m_description;
  }

  void ModelInModelOutJob::startImpl(const std::shared_ptr<ProcessCreator> &)
  {
    openstudio::path outpath = outdir(true);

    QWriteLocker l(&m_mutex);
    if (!m_model)
    {
      m_model = modelFile();
      resetFiles(m_files, m_model);
    }

    std::vector<std::shared_ptr<ModelInModelOutJob> > mergedJobs = m_mergedJobs;
    boost::optional<FileInfo> model = m_model;

    LOG(Info, "ModelInModelOut starting, filename: " << toString(m_model->fullPath));
    LOG(Info, "ModelInModelOut starting, outdir: " << toString(outpath));
    LOG(Info, "ModelInModelOut starting, num merged jobs: " << m_mergedJobs.size());

    m_lastrun = QDateTime::currentDateTime();
    JobErrors errors;
    errors.result = ruleset::OSResultValue::Success;
    l.unlock();

    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Starting));

    emitStarted();
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Processing));

    try {
      boost::filesystem::create_directories(outpath);

      model::OptionalModel m = model::Model::load(model->fullPath);

      if (!m)
      {
        errors.addError(ErrorType::Error, "Unable to load model: " + toString(model->fullPath));
        errors.result = ruleset::OSResultValue::Fail;
      } else {
        LOG(Info, "ModelInModelOut executing primary job");
        model::Model outmodel = modelToModelRun(*m);

        for (const auto & mergedJob : mergedJobs)
        {
          LOG(Info, "ModelInModelOut executing merged job");
          outmodel = mergedJob->modelToModelRun(outmodel);
        }

        openstudio::path outFile = outpath / toPath("out.osm");
        if (!outmodel.save(outFile,true))
        {
          errors.addError(ErrorType::Error, "Error while writing final output file");
          errors.result = ruleset::OSResultValue::Fail;
        }
      }
    } catch (const std::exception &e) {
      errors.addError(ErrorType::Error, "Error with processing: " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
    }

    emitOutputFileChanged(RunManager_Util::dirFile(outpath / openstudio::toPath("out.osm")));
    setErrors(errors);
  }

  std::string ModelInModelOutJob::getOutput() const
  {
    return "";
  }

  FileInfo ModelInModelOutJob::modelFile() const
  {
    if (!m_model)
    {
      return allInputFiles().getLastByExtension("osm");
    } else {
      return *m_model;
    }
  }

  void ModelInModelOutJob::basePathChanged()
  {
    m_model.reset();
    resetFiles(m_files);
  }

  Files ModelInModelOutJob::outputFilesImpl() const
  {
    Files outfiles;

    try {
      FileInfo osm(outdir() / toPath("out.osm"), "osm");
      osm.requiredFiles = modelFile().requiredFiles;

      outfiles.append(osm);
    } catch (const std::exception &) {
      //output file cannot be generated yet
    }
    return outfiles;
  }

  void ModelInModelOutJob::mergeJobImpl(const std::shared_ptr<Job_Impl> &t_parent, const std::shared_ptr<Job_Impl> &t_job) 
  {
    QWriteLocker l(&m_mutex);
    std::shared_ptr<ModelInModelOutJob> mimojob = std::dynamic_pointer_cast<ModelInModelOutJob>(t_job);

    if (!mimojob)
    {
      throw MergeJobError("Mismatched job types");
    }

    if (mimojob->jobType() != t_parent->jobType())
    {
      throw MergeJobError("Mismatched job types");
    }


    if (t_parent->finishedJob() == t_job)
    {
      throw MergeJobError("RHS is finished job - refusing to merge");
    }

    LOG(Info, "Merging Job " << openstudio::toString(t_job->uuid()) << " into " << openstudio::toString(uuid()));
    
    removeChild(t_job);
    std::vector<std::shared_ptr<Job_Impl> > children = t_job->children();
    std::for_each(children.begin(), children.end(), std::bind(&Job_Impl::addChild, t_parent, std::placeholders::_1));

    std::vector<std::shared_ptr<ModelInModelOutJob> > existing_merged_jobs = mimojob->m_mergedJobs;
    mimojob->m_mergedJobs.clear();

    m_mergedJobs.push_back(mimojob);

    // make sure we merge the already merged jobs too
    m_mergedJobs.insert(m_mergedJobs.begin(), existing_merged_jobs.begin(), existing_merged_jobs.end());
  }

} // detail
} // runmanager
} // openstudio



