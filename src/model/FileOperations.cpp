/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../utilities/core/FilesystemHelpers.hpp"

#include <OpenStudio.hxx>

namespace openstudio {
namespace model {
  bool replaceDir(const openstudio::path& sourceDir, const openstudio::path& destinationDir) {
    bool result = true;

    // Path **must** exist for `canonical()` to work, `weakly_canonical()` doesn't require this
    const auto src = openstudio::filesystem::weakly_canonical(sourceDir);
    const auto dest = openstudio::filesystem::weakly_canonical(destinationDir);

    // If both are the same canonical paths, we do nothing
    if (src == dest) {
      LOG_FREE(Warn, "replaceDir",
               "Refusing to copy directory '" << toString(sourceDir) << "' only itself '" << toString(destinationDir) << "' canonical path: '"
                                              << toString(src) << "'");
      return true;  // there is no error, just nothing to do
    }

    // If the source dir doesn't exist, there's a problem
    if (!openstudio::filesystem::exists(src) || !openstudio::filesystem::is_directory(src)) {
      LOG_FREE(Error, "replaceDir", "Source directory does not exist: " << toString(src));
      return false;
    }

    // If the dest dir exists, remove all entries in it
    if (openstudio::filesystem::exists(dest)) {
      if (openstudio::filesystem::is_directory(dest)) {
        for (openstudio::filesystem::directory_iterator end_dir_it, it(dest); it != end_dir_it; ++it) {
          openstudio::filesystem::path path = it->path();
          try {
            openstudio::filesystem::remove_all(path);
          } catch (const std::exception& e) {
            LOG_FREE(Error, "replaceDir", "Error deleting: " << toString(path) << e.what());
            result = false;
          }
        }
      } else {
        LOG_FREE(Error, "replaceDir", "Destination exists and is not a directory, aborting: " << toString(dest));
        return false;
      }
    }

    // Now we recreate it, and it must work!
    if (!openstudio::filesystem::exists(dest)) {
      try {
        if (!openstudio::filesystem::create_directory(dest)) {
          LOG_FREE(Error, "replaceDir", "Could not create destination directory: " << toString(dest));
          return false;
        }
      } catch (const std::exception& e) {
        LOG_FREE(Error, "replaceDir", "Error creating directory: " << toString(dest) << e.what());
        return false;
      }
    }

    for (const auto& dirEnt : openstudio::filesystem::recursive_directory_iterator{src}) {
      const auto& path = dirEnt.path();
      auto relativePathStr = path.string();
      boost::replace_first(relativePathStr, src.string(), "");

      try {
        openstudio::filesystem::copy(path, dest / relativePathStr);
      } catch (const std::exception& e) {
        LOG_FREE(Error, "replaceDir",
                 "Error copying from: " << toString(path) << " to: " << toString(dest / relativePathStr) << " Description: " << e.what());
        result = false;
      }
    }

    return result;
  }

  bool removeDir(const openstudio::path& path) {
    try {
      const auto count = openstudio::filesystem::remove_all(path);
      LOG_FREE(Debug, "removeDir", "Removed " << count << " files");
    } catch (const std::exception& e) {
      LOG_FREE(Error, "removeDir", "Error removing from: " << toString(path) << " Description: " << e.what());
      return false;
    }

    return true;
  }

  openstudio::path createModelTempDir() {
    const auto result = openstudio::filesystem::create_temporary_directory("osmodel");
    LOG_FREE(Info, "createModelTempDir", "Created directory '" << toString(result) << "'");
    OS_ASSERT(!result.empty());
    return result;
  }

  bool initializeModelTempDir(const openstudio::path& osmPath, const openstudio::path& modelTempDir) {
    bool result = true;

    if (osmPath.empty() || !openstudio::filesystem::exists(osmPath)) {

      LOG_FREE(Debug, "initializeModelTempDir", "OSM path '" << toString(osmPath) << "' is empty or does not exist");
      result = false;

    } else {
      openstudio::path fileName = osmPath.filename();
      openstudio::path destination = modelTempDir / fileName;
      LOG_FREE(Debug, "initializeModelTempDir", "Copying '" << toString(osmPath) << "' to '" << toString(destination) << "'");

      // copy osm file
      bool test = true;
      try {
        openstudio::filesystem::copy_file(osmPath, destination);
        test = true;
      } catch (const std::exception& e) {
        LOG_FREE(Error, "initializeModelTempDir",
                 "Could not copy osm from '" << toString(osmPath) << "' to '" << toString(destination) << "' error" << e.what());
        test = false;
      }

      // Copy all files from existing resources dir into temp dir when opening
      openstudio::path sourceDir = getCompanionFolder(osmPath);
      openstudio::path destDir = modelTempDir / toPath("resources");
      if (openstudio::filesystem::exists(sourceDir)) {
        LOG_FREE(Debug, "initializeModelTempDir", "Copying '" << toString(sourceDir) << "' to '" << toString(destDir) << "'");

        test = replaceDir(sourceDir, destDir);
        if (!test) {
          LOG_FREE(Error, "initializeModelTempDir", "Could not copy '" << toString(sourceDir) << "' to '" << toString(destDir) << "'");
          result = false;
        }
      }
    }

    return result;
  }

  bool updateModelTempDir(openstudio::model::Model& model, const openstudio::path& modelTempDir) {
    bool modified = false;

    // Create folder layout
    openstudio::path runpath = modelTempDir / openstudio::toPath("resources/run");
    openstudio::path scriptspath = modelTempDir / openstudio::toPath("resources/measures");
    openstudio::path filespath = modelTempDir / openstudio::toPath("resources/files");

    if (!openstudio::filesystem::exists(runpath)) {
      openstudio::filesystem::create_directories(runpath);
      modified = true;
    }

    if (!openstudio::filesystem::exists(scriptspath)) {
      openstudio::filesystem::create_directories(scriptspath);
      modified = true;
    }

    if (!openstudio::filesystem::exists(filespath)) {
      openstudio::filesystem::create_directories(filespath);
      modified = true;
    }

    // weather file is fixed in OSDocument::fixWeatherFileOnOpen

    return modified;
  }

  bool attachWorkflow(openstudio::model::Model& model, const openstudio::path& modelTempDir) {
    boost::optional<WorkflowJSON> workflowJSON;
    bool result = false;

    openstudio::path oswPath = modelTempDir / openstudio::toPath("resources/workflow.osw");
    if (boost::filesystem::exists(oswPath)) {
      workflowJSON = WorkflowJSON::load(oswPath);
      if (workflowJSON) {
        result = true;
        LOG_FREE(Debug, "attachWorkflow", "Opened existing workflow.osw file");
      } else {
        LOG_FREE(Error, "attachWorkflow", "Could not open existing workflow.osw file");
      }
    }

    if (!workflowJSON) {
      workflowJSON = WorkflowJSON();
      workflowJSON->setOswPath(oswPath);
      workflowJSON->save();
    }
    OS_ASSERT(workflowJSON);
    model.setWorkflowJSON(*workflowJSON);

    return result;
  }

  openstudio::path initializeModel(openstudio::model::Model model) {
    return initializeModel(model, openstudio::path());
  }

  openstudio::path initializeModel(openstudio::model::Model model, const openstudio::path& savedPath) {
    openstudio::path modelTempDir = createModelTempDir();

    if (!savedPath.empty() && exists(savedPath)) {
      initializeModelTempDir(savedPath, modelTempDir);
    }

    updateModelTempDir(model, modelTempDir);
    attachWorkflow(model, modelTempDir);

    return modelTempDir;
  }

  void initializeModelObjects(openstudio::model::Model model) {
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

    for (auto& object : model.objects()) {
      if (object.optionalCast<model::ConstructionBase>()) {
        object.cast<model::ConstructionBase>().standardsInformation();
        object.cast<model::ConstructionBase>().renderingColor();
      } else if (object.optionalCast<model::Material>()) {
        object.cast<model::Material>().standardsInformation();
      } else if (object.optionalCast<model::BuildingStory>()) {
        object.cast<model::BuildingStory>().renderingColor();
      } else if (object.optionalCast<model::LightingSimulationZone>()) {
        object.cast<model::LightingSimulationZone>().renderingColor();
      } else if (object.optionalCast<model::SpaceType>()) {
        object.cast<model::SpaceType>().renderingColor();
      } else if (object.optionalCast<model::ThermalZone>()) {
        object.cast<model::ThermalZone>().renderingColor();
      } else if (object.optionalCast<model::BuildingUnit>()) {
        object.cast<model::BuildingUnit>().renderingColor();
      }
    }
  }

  bool saveModelTempDir(const openstudio::path& modelTempDir, const openstudio::path& osmPath) {
    bool test = true;

    // copy osm file
    openstudio::path srcPath = modelTempDir / osmPath.filename();

    try {
      openstudio::filesystem::copy_file(srcPath, osmPath, openstudio::filesystem::copy_option::overwrite_if_exists);
      test = true;
    } catch (const std::exception& e) {
      LOG_FREE(Error, "saveModelTempDir", "Could not copy osm from '" << toString(srcPath) << "' to '" << toString(osmPath) << "' error" << e.what());
      test = false;
    }

    if (openstudio::filesystem::is_regular_file(osmPath)) {
      // copy resources
      openstudio::path srcDir = modelTempDir / toPath("resources");
      // Get the companion directory
      openstudio::path dstDir = getCompanionFolder(osmPath);

      LOG_FREE(Debug, "saveModelTempDir", "Copying " << toString(srcDir) << " to " << toString(dstDir));

      test = replaceDir(srcDir, dstDir);
      if (!test) {
        LOG_FREE(Error, "saveModelTempDir", "Could not copy '" << toString(srcDir) << "' to '" << toString(dstDir) << "'");
      }
    }

    return test;
  }

  bool saveModel(openstudio::model::Model model, const openstudio::path& osmPath, const openstudio::path& modelTempDir) {
    // set the workflow's path
    openstudio::path oswPath = modelTempDir / openstudio::toPath("resources/workflow.osw");
    boost::optional<openstudio::path> currentOswPath = model.workflowJSON().oswPath();
    if (!currentOswPath || (oswPath != currentOswPath.get())) {
      model.workflowJSON().setOswPath(oswPath);
    }

    // set the seed name
    openstudio::path seedFile = toPath("..") / osmPath.filename();
    boost::optional<openstudio::path> currentSeedFile = model.workflowJSON().seedFile();
    if (!currentSeedFile || (seedFile != currentSeedFile.get())) {
      model.workflowJSON().setSeedFile(seedFile);
    }

    // save the osw, do before temp dirs get copied
    model.workflowJSON().save();

    // save osm to temp directory, saveModelTempDir will copy to real location
    openstudio::path tempModelPath = modelTempDir / osmPath.filename();
    bool modelSaved = model.save(tempModelPath, true);

    if (modelSaved) {
      LOG_FREE(Debug, "saveModel", "Saved model to '" << toString(tempModelPath) << "'");
    } else {
      LOG_FREE(Error, "saveModel", "Failed to save model to '" << toString(tempModelPath) << "'");
    }

    bool tempDirSaved = saveModelTempDir(modelTempDir, osmPath);

    return (modelSaved && tempDirSaved);
  }

  void removeModelTempDir(const openstudio::path& modelTempDir) {
    removeDir(modelTempDir);
  }

}  // namespace model
}  // namespace openstudio
