/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSWorkflow.hpp"

#include "Util.hpp"

#include "../model/Model.hpp"
#include "../model/WeatherFile.hpp"
#include "../model/FileOperations.hpp"

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/RunOptions.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Filesystem.hpp"

#include <fmt/chrono.h>

namespace openstudio {

void OSWorkflow::runInitialization() {

  state = State::Initialization;

  auto rootDir = workflowJSON.absoluteRootDir();
  LOG(Debug, "The root_dir for the datapoint is " << rootDir);

  detailedTimeBlock("Wiping directories", [this, &rootDir]() {
    {
      auto generatedFilesDir = rootDir / "generated_files";
      if (openstudio::filesystem::is_directory(generatedFilesDir)) {
        LOG(Debug, "Removing existing generated files directory: " << generatedFilesDir);
        openstudio::filesystem::remove_all(generatedFilesDir);
      }
      LOG(Debug, "Creating generated files directory: " << generatedFilesDir);
      openstudio::filesystem::create_directory(generatedFilesDir);

      // insert the generated files directory in the first spot so all generated ExternalFiles go here
      auto fps = workflowJSON.filePaths();
      workflowJSON.resetFilePaths();
      workflowJSON.addFilePath(generatedFilesDir);
      for (const auto& fp : fps) {
        workflowJSON.addFilePath(fp);
      }

      auto reportsDir = rootDir / "reports";
      if (openstudio::filesystem::is_directory(reportsDir)) {
        LOG(Debug, "Removing existing reports directory: " << reportsDir);
        openstudio::filesystem::remove_all(reportsDir);
      }
    }
  });

  // TODO: Validate the OSW measures if the flag is set to true, (the default state)
  // There isn't a 'verify_osw' key in the RunOptions, so always do it for now. Maybe don't if `fast`?
  {
    LOG(Info, "Attempting to validate the measure workflow");

    if (!workflowJSON.validateMeasures()) {
      LOG_AND_THROW("Workflow is invalid");
    }

    LOG(Info, "Validated the measure workflow");
  }

  LOG(Debug, "Finding and loading the seed file");
  auto seedPath_ = workflowJSON.seedFile();
  if (seedPath_) {
    auto modelFullPath_ = workflowJSON.findFile(seedPath_.get());
    if (!modelFullPath_) {
      state = State::Errored;
      throw std::runtime_error(fmt::format("Seed model {} specified in OSW cannot be found", seedPath_->string()));
    }

    if (modelFullPath_->extension() == openstudio::filesystem::path(".idf")) {
      if (m_add_timings && m_detailed_timings) {
        m_timers->newTimer("    Loading seed IDF");
      }
      detailedTimeBlock("Loading seed IDF", [this, &modelFullPath_] { workspace_ = openstudio::workflow::util::loadIDF(modelFullPath_.get()); });

    } else {
      detailedTimeBlock("Loading seed OSM (VersionTranslation)",
                        [this, &modelFullPath_] { model = openstudio::workflow::util::loadOSM(modelFullPath_.get()); });
    }
  } else {
    model = openstudio::model::Model{};
    openstudio::model::initializeModelObjects(model);
  }

  initializeWeatherFileFromOSW();

  // Set a clone of the WorkflowJSON for the model, so that it finds the filePaths (such as generated_files we added above)
  model.setWorkflowJSON(workflowJSON.clone());

  // Tell the workflowJSON we have started, it'll log the start time and reset the stepResults
  workflowJSON.start();
}

}  // namespace openstudio
