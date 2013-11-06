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

#include <openstudio_lib/FileOperations.hpp>
#include <openstudio_lib/FileOperations.hpp>

#include <analysisdriver/SimpleProject.hpp>
#include <analysisdriver/AnalysisRunOptions.hpp>

#include <analysis/Analysis.hpp>
#include <analysis/Problem.hpp>
#include <analysis/DataPoint.hpp>

#include <runmanager/lib/RunManager.hpp>
#include <runmanager/lib/RubyJobUtils.hpp>
#include <runmanager/lib/WorkItem.hpp>
#include <runmanager/lib/Workflow.hpp>
#include <runmanager/lib/JobFactory.hpp>

#include <model/Model.hpp>
#include <model/WeatherFile.hpp>
#include <model/WeatherFile_Impl.hpp>

#include <osversion/VersionTranslator.hpp>

#include <energyplus/ReverseTranslator.hpp>

#include <utilities/filetypes/EpwFile.hpp>
#include <utilities/core/ApplicationPathHelpers.hpp>
#include <utilities/plot/ProgressBar.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/core/PathHelpers.hpp>
#include <utilities/core/Assert.hpp>

#include <QDir>
#include <QMessageBox>
#include <QTemporaryFile>

#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>

#include "OSAppBase.hpp"

namespace openstudio {

  bool removeDir(const QString &dirName)
  {
    bool result = true;
    bool test = true;

    LOG_FREE(Info, "removeDir", "removeDir '" << toString(dirName) << "'");

    QDir dir(dirName);
    if (dir.exists(dirName)) {

      Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
      {
        if (info.isDir()) {
          test = removeDir(info.absoluteFilePath());
          if (!test){
            LOG_FREE(Error, "removeDir", "Failed to remove directory '" << toString(info.absoluteFilePath()) << "'");
            result = false;

            // DLM: do we really want to give up here?
            //return result;
          }
        }else{
          test = QFile::remove(info.absoluteFilePath());
          if (!test){
            LOG_FREE(Error, "removeDir", "Failed to remove file '" << toString(info.absoluteFilePath()) << "'");
            result = false;

            // DLM: do we really want to give up here?
            //return result;
          }
        }
      }
      test = dir.rmdir(dirName);
      if (!test){
        LOG_FREE(Error, "removeDir", "Failed to remove directory '" << toString(dirName) << "'");
        result = false;

        // DLM: do we really want to give up here?
        //return result;
      }
    }
    
    return result;
  }

  bool synchDirStructures(const QString &srcPath, const QString &dstPath)
  {
    bool result = true;
    bool test = true;

    LOG_FREE(Info, "synchDirStructures", "Synching destination '" << toString(dstPath) << "' with source '" << toString(srcPath) << "'");

    QDir dstDir(dstPath);

    // remove all files in dst as well as any directories in dst that are not in src
    Q_FOREACH(const QFileInfo &dstItemInfo, dstDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
    {
      QString srcItemPath = srcPath + "/" + dstItemInfo.fileName();
      QString dstItemPath = dstPath + "/" + dstItemInfo.fileName();

      QFileInfo srcItemInfo = QFileInfo(srcItemPath);

      if (dstItemInfo.isDir()){

        if (srcItemInfo.exists() && srcItemInfo.isDir()){

          // directory also exists in source
          test = synchDirStructures(srcItemPath, dstItemPath);
          if (!test){
            LOG_FREE(Error, "synchDirStructures", "Failed to synch destination '" << toString(dstItemPath) << "' with source '" << toString(srcItemPath) << "'");
            result = false;

            // DLM: do we really want to give up here?
            //return result;
          }

        }else{

          // directory does not exist in source
          test = removeDir(dstItemPath);
          if (!test){
            LOG_FREE(Error, "synchDirStructures", "Failed to remove directory '" << toString(dstItemPath) << "'");
            result = false;

            // DLM: do we really want to give up here?
            //return result;
          }
        }

      }else if (dstItemInfo.isFile()){
        test = QFile::remove(dstItemInfo.absoluteFilePath());
        if (!test){
          LOG_FREE(Error, "synchDirStructures", "Failed to remove file '" << toString(dstItemInfo.absoluteFilePath()) << "'");
          result = false;

          // DLM: do we really want to give up here?
          //return result;
        }
      }
    }

    return result;
  }

  bool copyDir(const QString &srcPath, const QString &dstPath)
  {
    bool result = true;
    bool test = true;

    LOG_FREE(Info, "copyDir", "copyDir '" << toString(srcPath) << "' to '" << toString(dstPath) << "'");

    // ensure directory exists
    QFileInfo dstInfo(dstPath);
    QString fileName = dstInfo.fileName();
    if (!dstInfo.exists() || !dstInfo.isDir()){
      QDir parentDstDir(QFileInfo(dstPath).path());
      LOG_FREE(Info, "copyDir", "Creating directory named = '" << toString(fileName) << "' in parentDstDir = '" << toString(parentDstDir.path()) << "'");
      if (!parentDstDir.mkpath(fileName)){
        LOG_FREE(Error, "copyDir", "Failed to create directory = '" << toString(fileName) << "' in parentDstDir = '" << toString(parentDstDir.path()) << "'");
        return false;
      }
    }

    QDir srcDir(srcPath);

    // remove all files in dst as well as any directories in dst that are not in src
    test = synchDirStructures(srcPath, dstPath);
    if (!test){
      LOG_FREE(Error, "copyDir", "Failed to synch destination '" << toString(dstPath) << "' with source '" << toString(srcPath) << "'");
      result = false;
    }
    
    // copy all files in src to dst
    Q_FOREACH(const QFileInfo &srcItemInfo, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
    {
      QString srcItemPath = srcPath + "/" + srcItemInfo.fileName();
      QString dstItemPath = dstPath + "/" + srcItemInfo.fileName();

      QFileInfo dstItemInfo = QFileInfo(dstItemPath);

      if (srcItemInfo.isDir()) {
        test = copyDir(srcItemPath, dstItemPath);
        if (!test){
          LOG_FREE(Error, "copyDir", "Failed to copy directory '" << toString(srcItemPath) << "' to '" << toString(dstItemPath) << "'");
          result = false;

          // DLM: do we really want to give up here?
          //return false;
        }
      } else if (srcItemInfo.isFile()) {
        test = QFile::copy(srcItemPath, dstItemPath);
        if (!test){
          LOG_FREE(Error, "copyDir", "Failed to copy file '" << toString(srcItemPath) << "' to '" << toString(dstItemPath) << "'");
          result = false;

          // DLM: do we really want to give up here?
          //return false;
        }
      } 
    }

    return result;
  }

  openstudio::path createModelTempDir()
  {
    openstudio::path result;
    QTemporaryFile * tempFile = new QTemporaryFile();

    if( tempFile->open() )
    {
      QString path = longPathName(tempFile->fileName());

      // must close tempFile so you can create directory with same name on Windows
      delete tempFile;

      QDir modelTempDir(path);
      LOG_FREE(Info, "createModelTempDir", "Creating directory '" << toString(modelTempDir.path()) << "'");
      bool test = modelTempDir.mkpath(modelTempDir.path());
      OS_ASSERT(test);

      result = toPath(path);

      return result;
    }

    delete tempFile;
     
    OS_ASSERT(false);

    return openstudio::path();
  } 

  bool initializeModelTempDir(const openstudio::path& osmPath, const openstudio::path& modelTempDir)
  {
    bool result = true;
    bool test = true;

    if( osmPath.empty() || !boost::filesystem::exists(osmPath)){
     
      LOG_FREE(Debug, "initializeModelTempDir", "OSM path '" << toString(osmPath) << "' is empty or does not exist");
      result = false;

    }else{

      LOG_FREE(Debug, "initializeModelTempDir", "Copying '" << toString(osmPath) << "' to '" << toString(modelTempDir / toPath("in.osm")) << "'");
   
      // copy osm file
      test = QFile::copy(toQString(osmPath), toQString(modelTempDir / toPath("in.osm")));
      if (!test){
        LOG_FREE(Error, "initializeModelTempDir", "Could not copy '" << toString(osmPath) << "' to '" << toString(modelTempDir / toPath("in.osm")) << "'");
      }

      // Copy all files from existing resources dir into temp dir when opening
      openstudio::path sourceDir = osmPath.parent_path() / toPath(osmPath.stem());
      openstudio::path destDir = modelTempDir / toPath("resources");
      if (boost::filesystem::exists(sourceDir)){
        LOG_FREE(Debug, "initializeModelTempDir", "Copying '" << toString(sourceDir) << "' to '" << toString(destDir) << "'");

        test = copyDir(toQString(sourceDir), toQString(destDir));
        if (!test){
          LOG_FREE(Error, "initializeModelTempDir", "Could not copy '" << toString(sourceDir) << "' to '" << toString(destDir) << "'");
          result = false;
        }
      }
    }

    return result;
  }

  bool updateModelTempDir(openstudio::model::Model& model, const openstudio::path& modelTempDir)
  {
    bool modified = false;

    // Create folder layout
    openstudio::path runpath = modelTempDir / openstudio::toPath("resources/run");
    openstudio::path scriptspath = modelTempDir / openstudio::toPath("resources/scripts");
    openstudio::path filespath = modelTempDir / openstudio::toPath("resources/files");

    if (!boost::filesystem::exists(runpath)) 
    {
      boost::filesystem::create_directories(runpath);
      modified = true;
    }

    if (!boost::filesystem::exists(scriptspath)) 
    {
      boost::filesystem::create_directories(scriptspath);
      modified = true;
    }

    if (!boost::filesystem::exists(filespath)) 
    {
      boost::filesystem::create_directories(filespath);
      modified = true;
    }

    boost::optional<model::WeatherFile> weatherFile = model.getOptionalUniqueModelObject<model::WeatherFile>();
    if (weatherFile){
      LOG_FREE(Debug, "updateModelTempDir", "existing weather file found in osm");
      boost::optional<openstudio::path> epwPath = weatherFile->path();
      if (epwPath){
        LOG_FREE(Debug, "updateModelTempDir", "existing weather file path: " << toString(*epwPath));
        if (epwPath->is_complete() || (!epwPath->empty() && toString(*epwPath->begin()) != "files"))
        {
          LOG_FREE(Debug, "updateModelTempDir", "existing weather file path is not relative to osmfolder: " << toString(modelTempDir));
          openstudio::path newPath = modelTempDir / toPath("resources/files") / toPath(epwPath->filename());
          try {
            boost::filesystem::copy_file(*epwPath, newPath, boost::filesystem::copy_option::overwrite_if_exists);
            EpwFile epwFile(newPath);
            boost::optional<openstudio::model::WeatherFile> newweatherfile = openstudio::model::WeatherFile::setWeatherFile(model, epwFile);
            newweatherfile->makeUrlRelative(modelTempDir / toPath("resources"));
            LOG_FREE(Debug, "updateModelTempDir", "existing weather file moved to new location: " << toString(newPath));
            modified = true;
          } catch (...) {
            LOG_FREE(Error, "updateModelTempDir", "Unable to copy file from " << toString(*epwPath) << " to " << toString(newPath));
          }
        }
      }
    }

    return modified;
  }

  void saveModelTempDir(const openstudio::path& modelTempDir, const openstudio::path& osmPath)
  {
    bool test = true;

    // must remove file, QFile::copy does not overwrite
    QFileInfo osmInfo(toQString(osmPath));
    if (osmInfo.exists() && osmInfo.isFile()){
      test = QFile::remove(toQString(osmPath));
      if (!test){
        LOG_FREE(Error, "saveModelTempDir", "Could not remove previous osm at '" << toString(osmPath) << "'");
      }
    }

    // copy osm file
    openstudio::path srcPath = modelTempDir / toPath("in.osm");
    test = QFile::copy(toQString(srcPath), toQString(osmPath));
    if (!test){
      LOG_FREE(Error, "saveModelTempDir", "Could not copy osm from '" << toString(srcPath) << "' to '" << toString(osmPath) << "'");
    }

    if( QFileInfo(toQString(osmPath)).exists() ) {

      // copy resources
      openstudio::path srcDir = modelTempDir / toPath("resources");
      openstudio::path dstDir =  osmPath.parent_path() / toPath(osmPath.stem());

      openstudio::path srcproject = srcDir / toPath("project.osp");
      openstudio::path destproject = dstDir / toPath("project.osp");

//      LOG_FREE(Debug, "saveModelTempDir", "copying project file: " << toString(srcproject) << " to " << toString(destproject));
//      QFile::copy(toQString(srcproject), toQString(destproject));

      LOG_FREE(Debug, "saveModelTempDir", "Copying " << toString(srcDir) << " to " << toString(dstDir));

      test = copyDir(toQString(srcDir), toQString(dstDir));
      if (!test){
        LOG_FREE(Error, "saveModelTempDir", "Could not copy '" << toString(srcDir) << "' to '" << toString(dstDir) << "'");
      }

      // TODO: Open all osps just copied over to make the stored paths look reasonable.

    }
  }

  openstudio::path saveModel(openstudio::model::Model model, const openstudio::path& osmPath, const openstudio::path& modelTempDir)
  {

    openstudio::path modelPath = osmPath;

    if (getFileExtension(osmPath).empty()) {
      modelPath = setFileExtension(osmPath,modelFileExtension(),false,true);
    }

    // save osm to temp directory, saveModelTempDir will copy to real location
    openstudio::path tempModelPath = modelTempDir / toPath("in.osm");
    Workspace(model).save(tempModelPath,true); 

    LOG_FREE(Debug, "saveModel", "Saved model to '" << toString(tempModelPath) << "'");

    // DLM: eventually put saveRunManagerDatabase here, needs to happen before saveModelTempDir

    // DLM: evenually add this back in too
    //saveModelTempDir(modelTempDir, modelPath);
    
    return modelPath;
  }

  void removeModelTempDir(const openstudio::path& modelTempDir)
  {
    removeDir(toQString(modelTempDir));
  }

  bool saveRunManagerDatabase(const openstudio::path& osmPath, const openstudio::path& modelTempDir,                               
                              bool useRadianceForDaylightingCalculations,
                              QWidget* parent)
  {
    return saveRunManagerDatabase(osmPath, modelTempDir, std::map<openstudio::path,std::vector<ruleset::UserScriptInfo> >(), 
        useRadianceForDaylightingCalculations, parent);
  }

  bool saveRunManagerDatabase(const openstudio::path& osmPath, const openstudio::path& modelTempDir, 
                              const std::map<openstudio::path,std::vector<ruleset::UserScriptInfo> >& userScriptsByFolder, 
                              bool useRadianceForDaylightingCalculations,
                              QWidget* parent)
  {
    bool newToolsFound = false;

    openstudio::path rmdbPath = modelTempDir / toPath("resources/run.db");
    openstudio::path scriptsDir = modelTempDir / toPath("resources/scripts");
    openstudio::path simulationDir = openstudio::toPath("run");

    try {
      // this run db exists just to hold the jobs to be loaded from later, it is not unpaused
      openstudio::runmanager::RunManager rm(rmdbPath, true, true, false);

      // this should not be necessary as rm was constructed with truncate set to true?
      rm.clearJobs();

      openstudio::runmanager::ConfigOptions co = rm.getConfigOptions();

      newToolsFound = co.findTools(true, true, true, true);
      rm.setConfigOptions(co);

      openstudio::runmanager::Workflow wf;

      bool ruby_installed = rm.getConfigOptions().getTools().getAllByName("ruby").tools().size() > 0;
      
      openstudio::path radiancePath;

      std::vector<openstudio::runmanager::ToolInfo> rad = rm.getConfigOptions().getTools().getAllByName("rad").tools();

      if (!rad.empty())
      {
        radiancePath = rad.back().localBinPath.parent_path();
      } 

      bool ruby_jobs_skipped = wf.addStandardWorkflow(scriptsDir, ruby_installed, getOpenStudioRubyIncludePath(), 
                                                      userScriptsByFolder, 
                                                      useRadianceForDaylightingCalculations, 
                                                      radiancePath,
                                                      modelTempDir / toPath("resources"), true);
      if (ruby_jobs_skipped)
      {
        QMessageBox::information(parent, 
            "No Ruby Interpreter",
            "Measures were provided but no ruby interpreter could be found. Scripts will not be run.",
            QMessageBox::Ok);
      }

      openstudio::runmanager::Tools tools = rm.getConfigOptions().getTools();
      wf.add(tools);
      wf.addParam(runmanager::JobParam("flatoutdir"));

      openstudio::runmanager::Job j = wf.create(simulationDir, toPath("../in.osm"));

      rm.enqueue(j, true);

    } catch (const std::exception &e) {
      QMessageBox::information(parent, 
          "Error Initiating Simulation",
          e.what(),
          QMessageBox::Ok);
    }

    return newToolsFound;
  }

  void pauseRunManager(openstudio::runmanager::RunManager& rm)
  {
    rm.setPaused(true);

    std::vector<openstudio::runmanager::Job> jobs = rm.getJobs();

    for (std::vector<openstudio::runmanager::Job>::iterator itr = jobs.begin();
         itr != jobs.end();
         ++itr)
    {
      itr->setCanceled(true);
    }

    for (std::vector<openstudio::runmanager::Job>::iterator itr = jobs.begin();
         itr != jobs.end();
         ++itr)
    { 
      itr->requestStop();
    }

    for (std::vector<openstudio::runmanager::Job>::iterator itr = jobs.begin();
         itr != jobs.end();
         ++itr)
    {
      itr->waitForFinished();
    }
  }

  void startRunManager(openstudio::runmanager::RunManager& rm, const openstudio::path& osmPath, const openstudio::path& modelTempDir,
      bool useRadianceForDaylightingCalculations, QWidget* parent)
  {
//    openstudio::path rmdbPath = modelTempDir / toPath("resources/run.db");
    openstudio::path simulationDir = toPath("run");

    try {

      // disconnect signals from current jobs before clearing jobs
      std::vector<openstudio::runmanager::Job> jobs = rm.getJobs();

      for (std::vector<openstudio::runmanager::Job>::iterator itr = jobs.begin();
           itr != jobs.end();
           ++itr)
      {
        itr->disconnect();
      }

      rm.setPaused(true);

      // clear current jobs
      rm.clearJobs();

      if (boost::filesystem::exists(modelTempDir / toPath("resources") / simulationDir)) {
        boost::filesystem::remove_all(modelTempDir / toPath("resources") / simulationDir);
      }
      
      boost::optional<analysisdriver::SimpleProject> p = OSAppBase::instance()->project();
      if (p)
      {
        openstudio::runmanager::ConfigOptions co(true);

        analysis::Problem prob = p->analysis().problem();
        analysisdriver::AnalysisRunOptions runOptions = standardRunOptions(*p);
        std::vector<runmanager::WorkItem> workitems(prob.createWorkflow(p->baselineDataPoint(), runOptions.rubyIncludeDirectory()).toWorkItems());

        if (useRadianceForDaylightingCalculations)
        {
          std::vector<openstudio::runmanager::ToolInfo> rad = co.getTools().getAllByName("rad").tools();

          if (!rad.empty())
          {
            openstudio::path radiancePath = rad.back().localBinPath.parent_path();

            bool modeltoidffound(false);
            for (std::vector<runmanager::WorkItem>::iterator itr = workitems.begin();
                itr != workitems.end();
                ++itr)
            {
              if (itr->type == openstudio::runmanager::JobType::ModelToIdf)
              {
                workitems.insert(itr, runmanager::Workflow::radianceDaylightCalculations(runOptions.rubyIncludeDirectory(), radiancePath));
                modeltoidffound = true;
                break;
              }
            }

            assert(modeltoidffound);
          } else {
            QMessageBox::information(parent, 
                "Error with initiating simulation.",
                "Radiance simulation requested but radiance was not found.\nRadiance simulation will be skipped.",
                QMessageBox::Ok);
          }
        }

        runmanager::Workflow wf(workitems);
        wf.add(co.getTools());
        runmanager::Job j = wf.create(modelTempDir / toPath("resources") / simulationDir, modelTempDir / openstudio::toPath("in.osm"));
        runmanager::JobFactory::optimizeJobTree(j);
        rm.enqueue(j, true);
      }

      // connect signals to new jobs
      jobs = rm.getJobs();

      for (std::vector<openstudio::runmanager::Job>::iterator itr = jobs.begin();
          itr != jobs.end();
          ++itr)
      {
        if (parent){
          bool isConnected = itr->connect(SIGNAL(outputDataAdded(const openstudio::UUID &, const std::string &)), 
                                          parent, SLOT(outputDataAdded(const openstudio::UUID &, const std::string &)));
          OS_ASSERT(isConnected);
        }

        if (!itr->parent())
        {
          // need to reset the base path of the top level job so it can find its osm in the saved
          // location
          
          // run in model dir
          //itr->setBasePath(osmPath.parent_path() / osmPath.stem());

          // run in temp dir
          itr->setBasePath(modelTempDir / toPath("resources"));

          if (parent){         
            bool isConnected = itr->connect(SIGNAL(treeChanged(const openstudio::UUID &)), parent, SLOT(treeChanged(const openstudio::UUID &)));
            OS_ASSERT(isConnected);
          }
        }
      }

      rm.setPaused(false);

    } catch (const std::exception &e) {
      QMessageBox::information(parent, 
          "Error Initiating Simulation",
          e.what(),
          QMessageBox::Ok);
    }
  }

  bool usesRadianceForDaylightCalculations(openstudio::runmanager::RunManager rm)
  {
    std::vector<openstudio::runmanager::Job> jobs = rm.getJobs();

    for (std::vector<openstudio::runmanager::Job>::const_iterator itr = jobs.begin();
         itr != jobs.end();
         ++itr)
    {
      if (itr->jobType() == openstudio::runmanager::JobType::Ruby)
      {
        if (!itr->allInputFiles().getAllByFilename("DaylightCalculations.rb").files().empty())
        {
          return true;
        }
      }
    }

    return false;
  }


  boost::optional<openstudio::model::Model> modelFromOSM(const openstudio::path& path, openstudio::osversion::VersionTranslator& versionTranslator, openstudio::ProgressBar* progressBar)
  {
    return versionTranslator.loadModel(path, progressBar);
  }

  boost::optional<openstudio::model::Model> modelFromIDF(const openstudio::path& path, openstudio::energyplus::ReverseTranslator& reverseTranslator, openstudio::ProgressBar* progressBar)
  { 
    return reverseTranslator.loadModel(path, progressBar);
  }

} // openstudio

