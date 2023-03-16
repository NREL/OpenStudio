#include "OSWorkflow.hpp"
#include "WorkflowRunOptions.hpp"

#include "../osversion/VersionTranslator.hpp"
#include "../measure/OSMeasure.hpp"
#include "../measure/ModelMeasure.hpp"
#include "../measure/EnergyPlusMeasure.hpp"
#include "../measure/ReportingMeasure.hpp"
#include "../measure/OSArgument.hpp"
#include "../measure/OSRunner.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/core/FileLogSink.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/data/Variant.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../energyplus/ForwardTranslator.hpp"
#include "workflow/Util.hpp"

#include <fmt/color.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ostream.h>

#include <array>
#include <chrono>
#include <string_view>
#include <stdexcept>

// TODO: this should really in Variant.hpp, but I'm getting pwned by the fact that ruby defines int128_t as a macro, no time to track it down
template <>
struct fmt::formatter<openstudio::Variant> : fmt::ostream_formatter
{
};

template <>
struct fmt::formatter<openstudio::path> : fmt::ostream_formatter
{
};

namespace openstudio {

OSWorkflow::OSWorkflow(const filesystem::path& oswPath, ScriptEngineInstance& ruby, ScriptEngineInstance& python)
  :
#if USE_RUBY_ENGINE
    rubyEngine(ruby),
#endif
#if USE_PYTHON_ENGINE
    pythonEngine(python),
#endif
    workflowJSON(oswPath) {
}

OSWorkflow::OSWorkflow(const WorkflowRunOptions& t_workflowRunOptions, ScriptEngineInstance& ruby, ScriptEngineInstance& python)
  :
#if USE_RUBY_ENGINE
    rubyEngine(ruby),
#endif
#if USE_PYTHON_ENGINE
    pythonEngine(python),
#endif
    workflowJSON(t_workflowRunOptions.osw_path),
    m_no_simulation(t_workflowRunOptions.no_simulation),
    m_post_process_only(t_workflowRunOptions.post_process_only),
    m_show_stdout(t_workflowRunOptions.show_stdout),
    m_add_timings(t_workflowRunOptions.add_timings),
    m_style_stdout(t_workflowRunOptions.style_stdout) {

  if (m_add_timings) {
    m_timers = std::make_unique<workflow::util::TimerCollection>();
  }

  if (t_workflowRunOptions.runOptions.debug() || (workflowJSON.runOptions() && workflowJSON.runOptions()->debug())) {
    LOG(Debug, fmt::format("Original workflowJSON={}\n", workflowJSON.string()));
    t_workflowRunOptions.debug_print();
  }
  auto runOpt_ = workflowJSON.runOptions();
  if (!runOpt_) {
    workflowJSON.setRunOptions(t_workflowRunOptions.runOptions);
  } else {
    auto ori_ftOptions = runOpt_->forwardTranslatorOptions();
    workflowJSON.setRunOptions(t_workflowRunOptions.runOptions);
    // user supplied CLI flags trump everything
    ori_ftOptions.overrideValuesWith(t_workflowRunOptions.runOptions.forwardTranslatorOptions());
    workflowJSON.runOptions()->setForwardTranslatorOptions(ori_ftOptions);
  }

  if (workflowJSON.runOptions()->debug()) {
    fmt::print("workflowJSON={}\n", workflowJSON.string());
  }
}

void OSWorkflow::applyArguments(measure::OSArgumentMap& argumentMap, const std::string& argumentName, const openstudio::Variant& argumentValue) {
  LOG(Info, "Setting argument value '" << argumentName << "' to '" << argumentValue << "'");

  // if (!argumentValue.hasValue()) {
  //   fmt::print("Warn: Value for argument '{}' not set in argument list therefore will use default\n", argumentName);
  //   return;
  // }
  if (!argumentMap.contains(argumentName)) {
    throw std::runtime_error(fmt::format("Could not find argument '{}' in argument_map\n", argumentName));
  }

  auto& v = argumentMap.at(argumentName);

  bool value_set = false;
  VariantType variantType = argumentValue.variantType();

  if (variantType == VariantType::String) {
    value_set = v.setValue(argumentValue.valueAsString());
  } else if (variantType == VariantType::Double) {
    value_set = v.setValue(argumentValue.valueAsDouble());
  } else if (variantType == VariantType::Integer) {
    value_set = v.setValue(argumentValue.valueAsInteger());
  } else if (variantType == VariantType::Boolean) {
    value_set = v.setValue(argumentValue.valueAsBoolean());
  }

  if (!value_set) {
    throw std::runtime_error(fmt::format("Could not set argument '{}' to value '{}': passed argument has type '{}' while '{}' was expected",
                                         argumentName, argumentValue, argumentValue.variantType().valueName(), v.type().valueName()));
  }
}

void OSWorkflow::saveOSMToRootDirIfDebug() {
  if (!workflowJSON.runOptions() || !workflowJSON.runOptions()->debug()) {
    return;
  }

  LOG(Info, "Saving OSM to Root Directory");
  auto savePath = workflowJSON.absoluteRootDir() / "in.osm";
  detailedTimeBlock("Saving OSM", [this, &savePath]() {
    // TODO: workflow gem was actually serializating via model.to_s for speed...
    model.save(savePath, true);
  });
  LOG(Info, "Saved OSM as " << savePath);
}

void OSWorkflow::saveIDFToRootDirIfDebug() {
  if (!workflowJSON.runOptions() || !workflowJSON.runOptions()->debug()) {
    return;
  }
  LOG(Info, "Saving IDF to Root Directory");
  auto savePath = workflowJSON.absoluteRootDir() / "in.idf";
  detailedTimeBlock("Saving IDF To Root Directory (debug)", [this, &savePath]() {
    // TODO: workflow gem was actually serializating via model.to_s for speed...
    workspace_->save(savePath, true);
  });
  LOG(Info, "Saved IDF as " << savePath);
}

void OSWorkflow::run() {

  if (!m_show_stdout) {
    openstudio::Logger::instance().standardOutLogger().disable();
  } else if (workflowJSON.runOptions()->debug()) {
    openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  }

  // Need to recreate the runDir as fast as possible, so I can direct a file log sink there
  bool hasDeletedRunDir = false;
  auto runDirPath = workflowJSON.absoluteRunDir();
  if (!workflowJSON.runOptions()->preserveRunDir()) {
    // We don't have a run_dir argument anyways
    if (openstudio::filesystem::is_directory(runDirPath)) {
      hasDeletedRunDir = true;
      openstudio::filesystem::remove_all(runDirPath);
    }
    openstudio::filesystem::create_directory(runDirPath);
  }
  FileLogSink logFile(runDirPath / "run.log");
  logFile.setLogLevel(Debug);

  if (hasDeletedRunDir) {
    LOG(Debug, "Removing existing run directory: " << runDirPath);
  }

  // Communicate that the workflow has been started
  LOG(Debug, "Registering that the workflow has started with the adapter");
  {
    // @output_adapter.communicate_started
    openstudio::filesystem::ofstream file(runDirPath / "started.job");
    OS_ASSERT(file.is_open());
    file << fmt::format("Started Workflow {}\n", std::chrono::system_clock::now());
    file.close();
  }

  // bool no_simulation = false;
  // bool post_process = false;
  // bool ep_json = false;
  // bool show_stdout = false;
  // bool add_timings = false;
  // bool style_stdout = false;
  // unsigned socket_port = 0;

  //// O. Need to apply measure steps IN ORDER. (eg: OpenStudio Measures before Eplus measures etc)
  //// https://github.com/NREL/OpenStudio-workflow-gem/blob/develop/lib/openstudio/workflow/util/measure.rb

  auto timeJob = [this](memJobFunPtr job, const std::string& message) {
    if (m_style_stdout) {
      fmt::print(fmt::fg(fmt::color::green),
                 "\n"
                 "┌{0:─^{2}}┐\n"
                 "│{1: ^{2}}│\n"
                 "└{0:─^{2}}┘\n",
                 "", std::string("Starting State ") + message, 80);
    }
    if (m_add_timings) {
      m_timers->newTimer(message);
    }
    (this->*job)();
    if (m_add_timings) {
      m_timers->tockCurrentTimer();
    }
    if (m_style_stdout) {
      fmt::print(fmt::fg(fmt::color::green), "{0:#^80}\n", std::string("Returned from State ") + message);
    }
  };

  struct JobInfo
  {
    memJobFunPtr jobFun;
    bool selected = true;
  };

  struct JobMap
  {
    std::array<std::pair<std::string_view, JobInfo>, 9> data;

    [[nodiscard]] JobInfo& at(const std::string_view& key) {
      auto itr = std::find_if(std::begin(data), std::end(data), [&key](const auto& v) { return v.first == key; });
      if (itr != std::end(data)) {
        return itr->second;
      } else {
        throw std::range_error("Not Found");
      }
    }

    // Class methods
    auto cbegin() const {
      return data.cbegin();
    }
    auto cend() const {
      return data.cend();
    }
    auto begin() const {
      return cbegin();
    }
    auto end() const {
      return cend();
    }
    auto begin() {
      return data.begin();
    }
    auto end() {
      return data.end();
    }
  };

  // Can't use a regular map, it's not retaining order
  static constexpr std::array<std::pair<std::string_view, JobInfo>, 9> known_jobs{{
    {"Initialization", {&OSWorkflow::runInitialization, true}},
    {"OpenStudioMeasures", {&OSWorkflow::runOpenStudioMeasures, true}},
    {"Translator", {&OSWorkflow::runTranslator, true}},
    {"EnergyPlusMeasures", {&OSWorkflow::runEnergyPlusMeasures, true}},
    {"PreProcess", {&OSWorkflow::runPreProcess, true}},
    {"EnergyPlus", {&OSWorkflow::runEnergyPlus, true}},
    {"ReportingMeasures", {&OSWorkflow::runReportingMeasures, true}},
    {"PostProcess", {&OSWorkflow::runPostProcess, true}},
    {"Cleanup", {&OSWorkflow::runCleanup, true}},
  }};

  JobMap jobMap{{known_jobs}};

  if (m_no_simulation) {
    jobMap.at("Initialization").selected = true;
    jobMap.at("OpenStudioMeasures").selected = true;
    jobMap.at("Translator").selected = true;
    jobMap.at("EnergyPlusMeasures").selected = true;
    jobMap.at("PreProcess").selected = true;
    jobMap.at("EnergyPlus").selected = false;
    jobMap.at("ReportingMeasures").selected = false;
    jobMap.at("PostProcess").selected = true;
    jobMap.at("Cleanup").selected = true;

  } else if (m_post_process_only) {
    jobMap.at("Initialization").selected = true;
    jobMap.at("OpenStudioMeasures").selected = false;
    jobMap.at("Translator").selected = false;
    jobMap.at("EnergyPlusMeasures").selected = false;
    jobMap.at("PreProcess").selected = false;
    jobMap.at("EnergyPlus").selected = false;
    jobMap.at("ReportingMeasures").selected = true;
    jobMap.at("PostProcess").selected = true;
    jobMap.at("Cleanup").selected = true;

    workflowJSON.runOptions()->setPreserveRunDir(true);
  }

  if (!workflowJSON.runOptions()->cleanup() || workflowJSON.runOptions()->debug()) {
    jobMap.at("Cleanup").selected = false;
  }

  for (auto& [jobName, jobInfo] : jobMap) {
    LOG(Debug, fmt::format("{} - selected = {}\n", jobName, jobInfo.selected));
    if (jobInfo.selected) {
      timeJob(jobInfo.jobFun, std::string{jobName});
    } else {
      LOG(Info, "Skipping job " << jobName);
    }
  }

  // Save final IDF
  if (m_add_timings) {
    m_timers->newTimer("Save IDF");
  }
  workspace_->save(runDirPath / "in.idf", true);  // TODO: Is this really necessary? Seems like it's done before already
  if (m_add_timings) {
    m_timers->tockCurrentTimer();
  }

  if (!workflowJSON.runOptions()->fast()) {
    if (m_add_timings) {
      m_timers->newTimer("Zip datapoint");
    }
    openstudio::workflow::util::zipResults(runDirPath);
    if (m_add_timings) {
      m_timers->tockCurrentTimer();
    }
  }

  if (state == State::Errored) {
    workflowJSON.setCompletedStatus("Fail");
  } else {
    // completed status will already be set if workflow was halted
    if (!workflowJSON.completedStatus()) {
      workflowJSON.setCompletedStatus("Success");
    } else if (workflowJSON.completedStatus().get() == "Fail") {
      state = State::Errored;
    }
  }

  if (!workflowJSON.runOptions()->fast()) {
    // Save workflow
    if (m_add_timings) {
      m_timers->newTimer("Save WorkflowJSON out.osw");
    }
    workflowJSON.saveAs(workflowJSON.absoluteOutPath());
    if (workflowJSON.runOptions()->debug()) {
      fmt::print("workflowJSON={}\n", workflowJSON.string());
    }
    if (m_add_timings) {
      m_timers->tockCurrentTimer();
    }
  }

  if (state == State::Errored) {
    // TODO: communicate_failure
    openstudio::filesystem::ofstream file(runDirPath / "failed.job");
    OS_ASSERT(file.is_open());
    file << fmt::format("Failed Workflow {}\n", std::chrono::system_clock::now());
    file.close();
  } else {
    // TODO: communicate_complete
    openstudio::filesystem::ofstream file(runDirPath / "finished.job");
    OS_ASSERT(file.is_open());
    file << fmt::format("Finished Workflow {}\n", std::chrono::system_clock::now());
    file.close();
  }

  if (m_add_timings) {
    fmt::print("\nTiming:\n\n{}\n", m_timers->timeReport());

    // TODO: create profile.json in the run folder
  }
}
}  // namespace openstudio
