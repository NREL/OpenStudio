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

  // TODO: create the runner with our WorkflowJSON (workflow gem uses datapoint/analysis too?!)

  // TODO: Validate the OSW measures if the flag is set to true, (the default state)
  // Note JM 2022-11-07: Is it better to try and load all measures once, instead of crashing later?

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

  LOG(Debug, "Getting the initial weather file");
  // Initialize the weather file
  auto epwPath_ = workflowJSON.weatherFile();
  bool alreadySetInModel = false;
  if (!epwPath_) {
    LOG(Debug, "No weather file specified in OSW, looking in model");
    if (auto epwFile_ = model.weatherFile()) {
      epwPath_ = epwFile_->path();
      alreadySetInModel = true;
    }
  }

  if (epwPath_) {
    LOG(Debug, "Search for weather file " << epwPath_.get());
    auto epwFullPath_ = workflowJSON.findFile(epwPath_.get());
    if (!epwFullPath_) {
      auto epwFullPath_ = workflowJSON.findFile(epwPath_->filename());
    }
    if (!epwFullPath_) {
      throw std::runtime_error(fmt::format("Weather file {} specified but cannot be found", epwPath_->string()));
    }

    epwPath = epwFullPath_.get();

    if (!alreadySetInModel) {
      if (auto epwFile_ = openstudio::EpwFile::load(epwPath)) {
        model::WeatherFile::setWeatherFile(model, epwFile_.get());
      } else {
        LOG(Warn, "Could not load weather file from " << epwPath_.get());
      }
    }

  } else {
    LOG(Debug, "No valid weather file defined in either the osm or osw");
  }

  // Tell the workflowJSON we have started, it'll log the start time and reset the stepResults
  workflowJSON.start();
}

}  // namespace openstudio
