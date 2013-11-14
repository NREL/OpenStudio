/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include "ModelToIdfJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "RunManager_Util.hpp"

#include <utilities/time/DateTime.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/WeatherFile.hpp>
#include <model/WeatherFile_Impl.hpp>
#include <energyplus/ForwardTranslator.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/core/Assert.hpp>
#include <utilities/core/URLHelpers.hpp>
#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>
#include <osversion/VersionTranslator.hpp>

#include <QDir>
#include <QDateTime>

#include <boost/bind.hpp>
#include <boost/filesystem/fstream.hpp>

namespace openstudio {
namespace runmanager {
namespace detail {


  ModelToIdfJob::ModelToIdfJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::ModelToIdf, tools, params, files, t_restoreData)
  {
    try {
      m_model = files.getLastByExtension("osm");
      resetFiles(m_files, m_model);
    } catch (const std::runtime_error &) {
    }

    m_description = buildDescription("osm");
  }

  ModelToIdfJob::~ModelToIdfJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void ModelToIdfJob::requestStop()
  {
  }

  bool ModelToIdfJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    if (!t_lastrun)
    {
      return true;
    } else {
      QReadLocker l(&m_mutex);

      boost::optional<FileInfo> model = m_model;
      if (!model)
      {
        try { 
          model = modelFile();
        } catch (const std::runtime_error &) {
        }
      }

      if (model)
      {
        bool change = filesChanged(m_files, *t_lastrun);
        return change;
      } else {
        // if the model file we are using has not been established yet,
        // return outofdate
        return true;
      }
    }
  }

  void ModelToIdfJob::cleanup()
  {
  }

  std::string ModelToIdfJob::description() const
  {
    QReadLocker l(&m_mutex);
    return m_description;
  }

  void ModelToIdfJob::startImpl(const boost::shared_ptr<ProcessCreator> &)
  {
    openstudio::path outpath = outdir(true);

    QWriteLocker l(&m_mutex);
    if (!m_model)
    {
      m_model = modelFile();
      resetFiles(m_files, m_model);
    }

    LOG(Info, "ModelToIdf starting, filename: " << toString(m_model->fullPath));
    LOG(Info, "ModelToIdf starting, outdir: " << toString(outpath));

    m_lastrun = QDateTime::currentDateTime();
    JobErrors errors;
    errors.result = ruleset::OSResultValue::Success;
    l.unlock();

    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Starting));

    emitStarted();
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Processing));

    openstudio::energyplus::ForwardTranslator ft;

    try {
      boost::filesystem::create_directories(outpath);

      osversion::VersionTranslator translator;
      model::OptionalModel m = translator.loadModel(m_model->fullPath);

      if (!m)
      {
        errors.addError(ErrorType::Error, "Unable to load model: " + toString(m_model->fullPath));
        errors.result = ruleset::OSResultValue::Fail;
      } else {

        // temp code
        if (allParams().has("keepRunControlSpecialDays"))
        {
          ft.setKeepRunControlSpecialDays(true);
        }

        openstudio::Workspace workspace = ft.translateModel(*m);

        if (workspace.numObjects() > 0){
          boost::filesystem::ofstream ofs(outpath / openstudio::toPath("in.idf"));
          workspace.toIdfFile().print(ofs);
          ofs.flush();
          ofs.close();
          OS_ASSERT(boost::filesystem::exists(outpath / openstudio::toPath("in.idf")));
        } else {
          errors.addError(ErrorType::Error, "Converted OSM didn't create any objects, output idf not created");
        }

      }
    } catch (const std::exception &e) {
      LOG(Error, std::string("Error with ModelToIdf translation: ") + e.what());
      errors.addError(ErrorType::Error, "Error with conversion: " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
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

    emitOutputFileChanged(RunManager_Util::dirFile(outpath / openstudio::toPath("in.idf")));
    setErrors(errors);
  }

  std::string ModelToIdfJob::getOutput() const
  {
    return "";
  }

  void ModelToIdfJob::basePathChanged()
  {
    m_model.reset();
  }

  FileInfo ModelToIdfJob::modelFile() const
  {
    if (!m_model)
    {
      return allInputFiles().getLastByExtension("osm");
    } else {
      return *m_model;
    }
  }

  /*
  std::vector<FileInfo> ModelToIdfJob::inputFiles() const
  {
    std::vector<FileInfo> infiles;
    infiles.push_back(modelFile());
    return infiles;
  }
  */

  Files ModelToIdfJob::outputFilesImpl() const
  {
    openstudio::path outpath = outdir();

    if (!boost::filesystem::exists(outpath / toPath("in.idf")))
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
        FileInfo modelFile = this->modelFile();


        if (modelFile.hasRequiredFile(toPath("in.epw")))
        {
          std::pair<QUrl, openstudio::path> f = modelFile.getRequiredFile(toPath("in.epw"));
          LOG(Debug, "Setting user defined epw: " << toString(f.first.toString()));
          weatherFilePath = f.first;
        }


        if (modelFile.exists){
          // read model file
          OptionalIdfFile osm = IdfFile::load(modelFile.fullPath);
          if (osm){
            // create model
            Workspace workspace(*osm);
            model::Model model(workspace);
            // check if model has a weather file object
            model::OptionalWeatherFile weatherFile = model.getOptionalUniqueModelObject<model::WeatherFile>();
            if (weatherFile){
              // check if model has a weather file url
              boost::optional<openstudio::path> p = weatherFile->path();
              if (p){
                LOG(Debug, "Looking for weather file at ModelObject specified location: " << openstudio::toString(*p));
                openstudio::path wp = *p;
                openstudio::path basepath = getBasePath();
                if (!basepath.empty())
                {
                  wp = boost::filesystem::complete(wp, basepath);
                } 
                LOG(Debug, "Completed weatherfile location: " << openstudio::toString(wp));

                if (!boost::filesystem::exists(wp)) {
                  if (allParams().has("epwdir"))
                  {
                    // try prepending params "epwdir"
                    JobParam epwDirParam = allParams().get("epwdir");
                    if (epwDirParam.children.size() == 1) {
                      wp = toPath(epwDirParam.children[0].value) / *p;
                    }
                  }
                }

                if (!boost::filesystem::exists(wp))
                {
                  if (openstudio::toString(modelFile.fullPath.extension()) == ".osm")
                  {
                    openstudio::path osmdir = modelFile.fullPath.parent_path() / boost::filesystem::basename(modelFile.fullPath);
                    LOG(Debug, "Attempting to use OSM base path to find weather file");
                    wp = osmdir / *p;
                  }
                }

                if (!boost::filesystem::exists(wp))
                {
                  if (openstudio::toString(modelFile.fullPath.extension()) == ".osm")
                  {
                    openstudio::path osmdir = modelFile.fullPath.parent_path() / openstudio::toPath("resources");
                    LOG(Debug, "Attempting to use OSM parent path / resources to find weather file");
                    wp = osmdir / *p;
                  }
                }

                if (!boost::filesystem::exists(wp))
                {
                  if (openstudio::toString(modelFile.fullPath.extension()) == ".osm")
                  {
                    openstudio::path osmdir = modelFile.fullPath.parent_path();
                    LOG(Debug, "Attempting to use OSM parent path to find weather file");
                    wp = osmdir / *p;
                  }
                }

                if (!boost::filesystem::exists(wp))
                {
                  if (openstudio::toString(modelFile.fullPath.extension()) == ".osm")
                  {
                    openstudio::path osmdir = modelFile.fullPath.parent_path();
                    LOG(Debug, "Attempting to use required files to find weather file");
                    std::vector<std::pair<QUrl, openstudio::path> > requiredFiles = modelFile.requiredFiles;

                    for (std::vector<std::pair<QUrl, openstudio::path> >::const_iterator itr = requiredFiles.begin();
                         itr != requiredFiles.end();
                         ++itr)
                    {
                      LOG(Debug, "Trying required file: " << openstudio::toString(itr->second) << " for " << openstudio::toString(p->filename()));
                      if (itr->second.filename() == p->filename())
                      {
                        openstudio::path requiredFile = openstudio::toPath(itr->first.toLocalFile());
                        wp = openstudio::toPath(itr->first.toLocalFile()).parent_path().parent_path() / *p;
                        break;
                      }
                    }

                  }
                }

                if (!boost::filesystem::exists(wp))
                {
                  if (openstudio::toString(modelFile.fullPath.extension()) == ".osm")
                  {
                    openstudio::path osmdir = modelFile.fullPath.parent_path();
                    LOG(Debug, "Attempting to find weather file that already exists as in.epw");
                    wp /= openstudio::toPath("in.epw");
                  }
                }

                if (boost::filesystem::exists(wp)){
                  if (weatherFilePath && (checksum(*weatherFilePath) != checksum(wp))){
                    LOG(Warn, "Possible conflicting weather files found, using '" << toString(weatherFilePath->toString()) << "' which was specified by the user");
                  } else {
                    weatherFilePath = toURL(wp);
                  }
                }


              }
            }
          }
        }

        // check if there is an existing epw with same basename
        path existingWeatherFilePath = modelFile.fullPath;
        existingWeatherFilePath.replace_extension(toPath("epw").string());
        if (boost::filesystem::exists(existingWeatherFilePath)){
          if (weatherFilePath && (checksum(*weatherFilePath) != checksum(existingWeatherFilePath))){
            LOG(Warn, "Possible conflicting weather files found, using '" << toString(weatherFilePath->toString()) << "'");
          }else{
            weatherFilePath = QUrl::fromLocalFile(toQString(existingWeatherFilePath.native_file_string()));
          }
        }

        // check if there is an existing epw named in.epw
        existingWeatherFilePath.remove_filename();
        existingWeatherFilePath = existingWeatherFilePath / toPath("in.epw");
        if (boost::filesystem::exists(existingWeatherFilePath)){
          if (weatherFilePath && (checksum(*weatherFilePath) != checksum(existingWeatherFilePath))){
            LOG(Warn, "Possible conflicting weather files found, using '" << toString(weatherFilePath->toString()) << "'");
          }else{
            weatherFilePath = QUrl::fromLocalFile(toQString(existingWeatherFilePath.native_file_string()));
          }
        }

        // Specify the set of files we created so that the next Job in the chain (if there is one)
        // is able to pick them up
        Files outfiles;

        FileInfo idf(outpath / toPath("in.idf"), "idf");

        if (weatherFilePath){
          LOG(Debug, "Final selected weatherFile: " << openstudio::toString(*weatherFilePath));
          idf.addRequiredFile(*weatherFilePath, toPath("in.epw"));
        }else{
          LOG(Warn, "No weather file specified");
        }

        outfiles.append(idf);
        m_outputfiles = outfiles;
      } catch (const std::runtime_error &) {
        LOG(Warn, "OSM file not yet available, outputfiles not known");
        return Files();
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



