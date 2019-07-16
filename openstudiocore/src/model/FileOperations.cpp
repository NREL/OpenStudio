/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "FileOperations.hpp"

#include "Model.hpp"
#include "Component.hpp"

#include "Building.hpp"
#include "Building_Impl.hpp"
#include "Facility.hpp"
#include "Facility_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"
#include "LifeCycleCostParameters.hpp"
#include "LifeCycleCostParameters_Impl.hpp"
#include "Model_Impl.hpp"
#include "WeatherFile.hpp"
#include "WeatherFile_Impl.hpp"
#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "SizingParameters.hpp"
#include "SizingParameters_Impl.hpp"
#include "Timestep.hpp"
#include "Timestep_Impl.hpp"
#include "ShadowCalculation.hpp"
#include "ShadowCalculation_Impl.hpp"
#include "HeatBalanceAlgorithm.hpp"
#include "HeatBalanceAlgorithm_Impl.hpp"
#include "RunPeriod.hpp"
#include "RunPeriod_Impl.hpp"
#include "ConstructionBase.hpp"
#include "ConstructionBase_Impl.hpp"
#include "Material.hpp"
#include "Material_Impl.hpp"
#include "BuildingStory.hpp"
#include "BuildingStory_Impl.hpp"
#include "LightingSimulationZone.hpp"
#include "LightingSimulationZone_Impl.hpp"
#include "SpaceType.hpp"
#include "SpaceType_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "BuildingUnit.hpp"
#include "BuildingUnit_Impl.hpp"
#include "RenderingColor.hpp"
#include "StandardsInformationConstruction.hpp"
#include "StandardsInformationMaterial.hpp"

#include "../utilities/filetypes/EpwFile.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/plot/ProgressBar.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/Assert.hpp"

#include <OpenStudio.hxx>

#include <QDir>
#include <QMessageBox>
#include <QTemporaryFile>




namespace openstudio {
namespace model {

  bool removeDir(const QString &dirName)
  {
    bool result = true;

    LOG_FREE(Info, "removeDir", "removeDir '" << toString(dirName) << "'");

    QDir dir(dirName);
    if (dir.exists(dirName)) {
      bool test = true;

      for (const QFileInfo& info : dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden | QDir::AllDirs | QDir::Files, QDir::DirsFirst))
      {
        if (info.isDir()) {
          test = removeDir(info.absoluteFilePath());
          if (!test){
            LOG_FREE(Error, "removeDir", "Failed to remove directory '" << toString(info.absoluteFilePath()) << "'");
            result = false;

            // DLM: do we really want to give up here?
            //return result;
          }
        } else{
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

    QDir srcDir(srcPath);
    QDir dstDir(dstPath);

    if (srcDir.canonicalPath() == dstDir.canonicalPath()){
      LOG_FREE(Warn, "synchDirStructures", "Cannot synch destination '" << toString(dstPath) << "' with source '" << toString(srcPath) << "' because they resolve to the same location");
      return true; // already synched
    }

    LOG_FREE(Info, "synchDirStructures", "Synching destination '" << toString(dstPath) << "' with source '" << toString(srcPath) << "'");

    // remove all files in dst as well as any directories in dst that are not in src
    for (const QFileInfo &dstItemInfo : dstDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
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

        } else{

          // directory does not exist in source
          test = removeDir(dstItemPath);
          if (!test){
            LOG_FREE(Error, "synchDirStructures", "Failed to remove directory '" << toString(dstItemPath) << "'");
            result = false;

            // DLM: do we really want to give up here?
            //return result;
          }
        }

      } else if (dstItemInfo.isFile()){
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
    for (const QFileInfo &srcItemInfo : srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot))
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

#ifdef Q_OS_WIN

#include <windows.h>

QString longPathName(const QString& path)
{
  if (path.isEmpty()){
    return QString();
  }
  QString maybeShort = QDir::toNativeSeparators(path);
  QByteArray shortName = maybeShort.toLocal8Bit();
  char longPath[MAX_PATH];
  int err = GetLongPathName(shortName.constData(), longPath, MAX_PATH);
  (void)err;
  return QDir::fromNativeSeparators(QString::fromLocal8Bit(longPath));
}

#else

QString longPathName(const QString& path)
{
  return path;
}

#endif

  openstudio::path createModelTempDir()
  {
    openstudio::path result;
    auto tempFile = new QTemporaryFile();

    if (tempFile->open())
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

    if (osmPath.empty() || !openstudio::filesystem::exists(osmPath)){

      LOG_FREE(Debug, "initializeModelTempDir", "OSM path '" << toString(osmPath) << "' is empty or does not exist");
      result = false;

    } else{
      openstudio::path fileName = osmPath.filename();
      openstudio::path destination = modelTempDir / fileName;
      LOG_FREE(Debug, "initializeModelTempDir", "Copying '" << toString(osmPath) << "' to '" << toString(destination) << "'");

      // copy osm file
      bool test = QFile::copy(toQString(osmPath), toQString(destination));
      if (!test){
        LOG_FREE(Error, "initializeModelTempDir", "Could not copy '" << toString(osmPath) << "' to '" << toString(destination) << "'");
      }

      // Copy all files from existing resources dir into temp dir when opening
      openstudio::path sourceDir = getCompanionFolder(osmPath);
      openstudio::path destDir = modelTempDir / toPath("resources");
      if (openstudio::filesystem::exists(sourceDir)){
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
    openstudio::path scriptspath = modelTempDir / openstudio::toPath("resources/measures");
    openstudio::path filespath = modelTempDir / openstudio::toPath("resources/files");

    if (!openstudio::filesystem::exists(runpath))
    {
      openstudio::filesystem::create_directories(runpath);
      modified = true;
    }

    if (!openstudio::filesystem::exists(scriptspath))
    {
      openstudio::filesystem::create_directories(scriptspath);
      modified = true;
    }

    if (!openstudio::filesystem::exists(filespath))
    {
      openstudio::filesystem::create_directories(filespath);
      modified = true;
    }

    // weather file is fixed in OSDocument::fixWeatherFileOnOpen

    return modified;
  }

  bool attachWorkflow(openstudio::model::Model& model, const openstudio::path& modelTempDir)
  {
    boost::optional<WorkflowJSON> workflowJSON;
    bool result = false;

    openstudio::path oswPath = modelTempDir / openstudio::toPath("resources/workflow.osw");
    if (boost::filesystem::exists(oswPath)){
      workflowJSON = WorkflowJSON::load(oswPath);
      if (workflowJSON){
        result = true;
        LOG_FREE(Debug, "attachWorkflow", "Opened existing workflow.osw file");
      } else{
        LOG_FREE(Error, "attachWorkflow", "Could not open existing workflow.osw file");
      }
    }

    if (!workflowJSON){
      workflowJSON = WorkflowJSON();
      workflowJSON->setOswPath(oswPath);
      workflowJSON->save();
    }
    OS_ASSERT(workflowJSON);
    model.setWorkflowJSON(*workflowJSON);

    return result;
  }

  openstudio::path initializeModel(openstudio::model::Model model)
  {
    return initializeModel(model, openstudio::path());
  }

  openstudio::path initializeModel(openstudio::model::Model model, const openstudio::path& savedPath)
  {
    openstudio::path modelTempDir = createModelTempDir();

    if (!savedPath.empty() && exists(savedPath)){
      initializeModelTempDir(savedPath, modelTempDir);
    }

    updateModelTempDir(model, modelTempDir);
    attachWorkflow(model, modelTempDir);

    return modelTempDir;
  }

  void initializeModelObjects(openstudio::model::Model model)
  {
    // These objects used to be added to the model as you clicked through the App's tabs,
    // resulting in a uncertain set of model changes.  With these changes, every model will
    // always have the following objects after opening in the app.
    openstudio::model::Building building = model.getUniqueModelObject<openstudio::model::Building>();
    openstudio::model::Facility facility = model.getUniqueModelObject<openstudio::model::Facility>();
    openstudio::model::Site site = model.getUniqueModelObject<openstudio::model::Site>();

    // from simulation tab
    //model.getUniqueModelObject<openstudio::model::RadianceParameters>();
    model.getUniqueModelObject<openstudio::model::SimulationControl>();
    model.getUniqueModelObject<openstudio::model::SizingParameters>();
    //model.getUniqueModelObject<openstudio::model::ProgramControl>();
    model.getUniqueModelObject<openstudio::model::Timestep>();
    //model.getUniqueModelObject<openstudio::model::ReportingTolerances>();
    //model.getUniqueModelObject<openstudio::model::ConvergenceLimits>();
    model.getUniqueModelObject<openstudio::model::ShadowCalculation>();
    //model.getUniqueModelObject<openstudio::model::SurfaceConvectionAlgorithmInside>();
    //model.getUniqueModelObject<openstudio::model::SurfaceConvectionAlgorithmOutside>();
    model.getUniqueModelObject<openstudio::model::HeatBalanceAlgorithm>();
    //model.getUniqueModelObject<openstudio::model::ZoneAirHeatBalanceAlgorithm>();
    //model.getUniqueModelObject<openstudio::model::ZoneAirContaminantBalance>();
    //model.getUniqueModelObject<openstudio::model::ZoneCapacitanceMultiplierResearchSpecial>();
    model.getUniqueModelObject<openstudio::model::RunPeriod>();

    openstudio::model::LifeCycleCostParameters lifeCycleCostParameters = model.getUniqueModelObject<openstudio::model::LifeCycleCostParameters>();

    for (auto& object : model.objects()){
      if (object.optionalCast<model::ConstructionBase>()){
        object.cast<model::ConstructionBase>().standardsInformation();
        object.cast<model::ConstructionBase>().renderingColor();
      }else if (object.optionalCast<model::Material>()){
        object.cast<model::Material>().standardsInformation();
      }else if (object.optionalCast<model::BuildingStory>()){
        object.cast<model::BuildingStory>().renderingColor();
      }else if (object.optionalCast<model::LightingSimulationZone>()){
        object.cast<model::LightingSimulationZone>().renderingColor();
      }else if (object.optionalCast<model::SpaceType>()){
        object.cast<model::SpaceType>().renderingColor();
      }else if (object.optionalCast<model::ThermalZone>()){
        object.cast<model::ThermalZone>().renderingColor();
      }else if (object.optionalCast<model::BuildingUnit>()){
        object.cast<model::BuildingUnit>().renderingColor();
      }
    }
  }

  bool saveModelTempDir(const openstudio::path& modelTempDir, const openstudio::path& osmPath)
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
    openstudio::path srcPath = modelTempDir / osmPath.filename();
    test = QFile::copy(toQString(srcPath), toQString(osmPath));
    if (!test){
      LOG_FREE(Error, "saveModelTempDir", "Could not copy osm from '" << toString(srcPath) << "' to '" << toString(osmPath) << "'");
    }

    if (QFileInfo(toQString(osmPath)).exists()) {

      // copy resources
      openstudio::path srcDir = modelTempDir / toPath("resources");
      // Get the companion directory
      openstudio::path dstDir = getCompanionFolder(osmPath);

      LOG_FREE(Debug, "saveModelTempDir", "Copying " << toString(srcDir) << " to " << toString(dstDir));

      test = copyDir(toQString(srcDir), toQString(dstDir));
      if (!test){
        LOG_FREE(Error, "saveModelTempDir", "Could not copy '" << toString(srcDir) << "' to '" << toString(dstDir) << "'");
      }

    }

    return test;
  }

  bool saveModel(openstudio::model::Model model, const openstudio::path& osmPath, const openstudio::path& modelTempDir)
  {
    // set the workflow's path
    openstudio::path oswPath = modelTempDir / openstudio::toPath("resources/workflow.osw");
    boost::optional<openstudio::path> currentOswPath = model.workflowJSON().oswPath();
    if (!currentOswPath || (oswPath != currentOswPath.get())){
      model.workflowJSON().setOswPath(oswPath);
    }

    // set the seed name
    openstudio::path seedFile = toPath("..") / osmPath.filename();
    boost::optional<openstudio::path> currentSeedFile = model.workflowJSON().seedFile();
    if (!currentSeedFile || (seedFile != currentSeedFile.get())){
      model.workflowJSON().setSeedFile(seedFile);
    }

    // save the osw, do before temp dirs get copied
    model.workflowJSON().save();

    // save osm to temp directory, saveModelTempDir will copy to real location
    openstudio::path tempModelPath = modelTempDir / osmPath.filename();
    bool modelSaved = model.save(tempModelPath, true);

    if (modelSaved){
      LOG_FREE(Debug, "saveModel", "Saved model to '" << toString(tempModelPath) << "'");
    } else{
      LOG_FREE(Error, "saveModel", "Failed to save model to '" << toString(tempModelPath) << "'");
    }

    bool tempDirSaved = saveModelTempDir(modelTempDir, osmPath);

    return (modelSaved && tempDirSaved);
  }

  void removeModelTempDir(const openstudio::path& modelTempDir)
  {
    removeDir(toQString(modelTempDir));
  }

} // model
} // openstudio

