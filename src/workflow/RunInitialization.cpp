#include "OSWorkflow.hpp"

#include "Util.hpp"

#include "../model/Model.hpp"
#include "../model/WeatherFile.hpp"

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/RunOptions.hpp"

#include "../utilities/core/Filesystem.hpp"

namespace openstudio {

void OSWorkflow::runInitialization() {

  state = State::Initialization;

  detailedTimeBlock("Wiping directories", [this]() {
    if (!workflowJSON.runOptions()->preserveRunDir()) {
      // We don't have a run_dir argument anyways
      auto runDir = workflowJSON.absoluteRunDir();
      if (openstudio::filesystem::is_directory(runDir)) {
        LOG(Debug, "Removing existing run directory: " << runDir);
        openstudio::filesystem::remove_all(runDir);
      }
      openstudio::filesystem::create_directory(runDir);
    }

    {
      auto generatedFilesDir = workflowJSON.absoluteRootDir() / "generated_files";
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

      auto reportsDir = workflowJSON.absoluteRootDir() / "reports";
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
      detailedTimeBlock("Loading seed OSM", [this, &modelFullPath_] { model = openstudio::workflow::util::loadOSM(modelFullPath_.get()); });

      // Initialize the weather file
      if (auto epwPath_ = workflowJSON.weatherFile()) {
        if (auto epwFile_ = openstudio::EpwFile::load(epwPath_.get())) {
          model::WeatherFile::setWeatherFile(model, epwFile_.get());
        } else {
          LOG(Warn, "Could not load weather fle from " << epwPath_.get());
        }
      }
    }
  } else {
    model = openstudio::model::Model{};
  }
}

}  // namespace openstudio
