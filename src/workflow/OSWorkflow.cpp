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
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/data/Variant.hpp"
#include "../utilities/core/Filesystem.hpp"
#include "../utilities/core/Logger.hpp"
#include "../energyplus/ForwardTranslator.hpp"

#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
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
    fmt::print("Original workflowJSON={}\n", workflowJSON.string());
    t_workflowRunOptions.debug_print();
    fmt::print("m_no_simulation={}, m_post_process_only={}, m_show_stdout={}, m_add_timings={}, m_style_stdout={}", m_no_simulation,
               m_post_process_only, m_show_stdout, m_add_timings, m_style_stdout);
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
  fmt::print("Info: Setting argument value '{}' to '{}'\n", argumentName, argumentValue);

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
  detailedTimeBlock("Saving IDF", [this, &savePath]() {
    // TODO: workflow gem was actually serializating via model.to_s for speed...
    workspace_->save(savePath, true);
  });
  LOG(Info, "Saved IDF as " << savePath);
}

void OSWorkflow::run() {
  if (workflowJSON.runOptions()->debug()) {
    openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  }

#if USE_RUBY_ENGINE
  // rubyEngine->exec("puts 'Hello from Ruby'");
  rubyEngine->registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");
  rubyEngine->registerType<openstudio::measure::EnergyPlusMeasure*>("openstudio::measure::EnergyPlusMeasure *");
  rubyEngine->registerType<openstudio::measure::ReportingMeasure*>("openstudio::measure::ReportingMeasure *");
  // -1: gotta init the rest of OpenStudio in ruby, so that Ruleset and co are defined
  rubyEngine->exec("OpenStudio::init_rest_of_openstudio()");
#endif
#if USE_PYTHON_ENGINE
  // pythonEngine->exec("print('Hello from Python')");
  pythonEngine->registerType<openstudio::measure::PythonModelMeasure*>("openstudio::measure::PythonModelMeasure *");
  pythonEngine->registerType<openstudio::measure::PythonEnergyPlusMeasure*>("openstudio::measure::PythonEnergyPlusMeasure *");
  pythonEngine->registerType<openstudio::measure::PythonReportingMeasure*>("openstudio::measure::PythonReportingMeasure *");
#endif

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
    fmt::print("{} - selected = {}\n", jobName, jobInfo.selected);
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
  workspace_->save(workflowJSON.absoluteRunDir() / "in.idf");
  if (m_add_timings) {
    m_timers->tockCurrentTimer();
  }

  // Save workflow
  if (m_add_timings) {
    m_timers->newTimer("Save workflow");
  }
  workflowJSON.saveAs(workflowJSON.absoluteOutPath());
  if (workflowJSON.runOptions()->debug()) {
    fmt::print("workflowJSON={}\n", workflowJSON.string());
  }
  if (m_add_timings) {
    m_timers->tockCurrentTimer();
  }

  if (m_add_timings) {
    fmt::print("\nTiming:\n\n{}\n", m_timers->timeReport());
  }
}
}  // namespace openstudio
