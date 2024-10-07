/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSWorkflow.hpp"
#include "Util.hpp"
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
#include "../utilities/core/Json.hpp"
#include "../utilities/core/Logger.hpp"
#include "../utilities/data/Variant.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../energyplus/ForwardTranslator.hpp"

#include <fmt/color.h>
#include <fmt/chrono.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <json/json.h>

#include <array>
#include <chrono>
#include <limits>
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

  runner.setRegisterMsgAlsoLogs(true);
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

  runner.setRegisterMsgAlsoLogs(true);

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
    // user supplied CLI flags trump everything
    runOpt_->overrideValuesWith(t_workflowRunOptions.runOptions);
  }

  if (workflowJSON.runOptions()->debug()) {
    fmt::print("workflowJSON={}\n", workflowJSON.string());
  }
}

void OSWorkflow::initializeWeatherFileFromOSW() {
  LOG(Debug, "Initialize the weather file from osw");
  auto epwPath_ = workflowJSON.weatherFile();

  if (epwPath_) {
    LOG(Debug, "Search for weather file defined by osw " << epwPath_.get());
    auto epwFullPath_ = workflowJSON.findFile(epwPath_.get());
    if (!epwFullPath_) {
      auto epwFullPath_ = workflowJSON.findFile(epwPath_->filename());
    }
    if (!epwFullPath_) {
      throw std::runtime_error(fmt::format("Weather file {} specified but cannot be found", epwPath_->string()));
    }

    epwPath = epwFullPath_.get();

    if (auto epwFile_ = openstudio::EpwFile::load(epwPath)) {
      model::WeatherFile::setWeatherFile(model, epwFile_.get());
      runner.setLastEpwFilePath(epwPath);
    } else {
      LOG(Warn, "Could not load weather file from " << epwPath_.get());
    }
  } else {
    LOG(Debug, "No weather file specified in OSW, looking in model");
    updateLastWeatherFileFromModel();
    if (epwPath.empty()) {
      LOG(Warn, "No valid weather file defined in either the osm or osw.");
    }
  }
}

void OSWorkflow::updateLastWeatherFileFromModel() {
  LOG(Debug, "Find model's weather file and update LastEpwFilePath");
  if (auto epwFile_ = model.weatherFile()) {
    if (auto epwPath_ = epwFile_->path()) {
      LOG(Debug, "Search for weather file " << epwPath_.get());
      auto epwFullPath_ = workflowJSON.findFile(epwPath_.get());
      if (!epwFullPath_) {
        auto epwFullPath_ = workflowJSON.findFile(epwPath_->filename());
      }
      if (!epwFullPath_) {
        throw std::runtime_error(fmt::format("Weather file {} specified but cannot be found", epwPath_->string()));
      }

      epwPath = epwFullPath_.get();
      runner.setLastEpwFilePath(epwPath);

      return;
    }
  }

  LOG(Debug, "Weather file is not defined by the model");
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

void standardFormatterWithStringSeverity(boost::log::record_view const& rec, boost::log::formatting_ostream& strm) {

  static constexpr std::array<std::string_view, 6> logLevelStrs = {"Trace", "Debug", "Info", "Warn", "Error", "Fatal"};
  // static constexpr std::array<std::string_view, 6> logLevelStrs = {"TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL"};

  LogLevel logLevel = Trace;
  if (auto logLevel_ = boost::log::extract<LogLevel>("Severity", rec)) {
    logLevel = logLevel_.get();
  }

  // if (auto pt_ = boost::log::extract<boost::posix_time::ptime>("TimeStamp", rec)) {
  //  // TimeStamp as  [%H:%M:%S.%f]
  //   strm << "[" << boost::posix_time::to_simple_string(pt_.get().time_of_day()) << "] ";
  // }
  strm << "[" << boost::log::extract<LogChannel>("Channel", rec) << "] <"  //
       << logLevelStrs[static_cast<size_t>(logLevel) - static_cast<size_t>(LogLevel::Trace)]
       << "> "
       // Finally, the record message
       << rec[boost::log::expressions::smessage];
}

bool OSWorkflow::run() {

  // If the user passed something like `openstudio --loglevel Trace run --debug -w workflow.osw`, we retain the Trace
  LogLevel oriLogLevel = openstudio::Logger::instance().standardOutLogger().logLevel().value_or(Warn);
  LogLevel targetLogLevel = oriLogLevel;
  if (workflowJSON.runOptions()->debug() && oriLogLevel > Debug) {
    targetLogLevel = Debug;
  }

  openstudio::Logger::instance().addTimeStampToLogger();  // Needed for run.log formatting
  openstudio::Logger::instance().standardOutLogger().setFormatter(&standardFormatterWithStringSeverity);

  // TODO: ideally we want stdErr logger to always receive Error and Fatal
  // and stdOut logger should receive all the others. This is definitely doable (cf LogSink::updateFilter) but now is not the time.
  if (!m_show_stdout) {
    openstudio::Logger::instance().standardOutLogger().setLogLevel(Error);  // Still show errors
  } else if (oriLogLevel != targetLogLevel) {
    openstudio::Logger::instance().standardOutLogger().setLogLevel(targetLogLevel);
  }

  // Need to recreate the runDir as fast as possible, so I can direct a file log sink there
  bool hasDeletedRunDir = false;
  auto runDirPath = workflowJSON.absoluteRunDir();

  if (runDirPath == workflowJSON.oswDir()) {
    workflowJSON.runOptions()->setPreserveRunDir(true);
  }

  if (!workflowJSON.runOptions()->preserveRunDir() && !m_post_process_only) {
    // We don't have a run_dir argument anyways
    if (openstudio::filesystem::is_directory(runDirPath)) {
      hasDeletedRunDir = true;
      openstudio::filesystem::remove_all(runDirPath);
    }
  }
  if (!openstudio::filesystem::is_directory(runDirPath)) {
    openstudio::filesystem::create_directory(runDirPath);
  }

  FileLogSink logFile(runDirPath / "run.log");
  constexpr bool use_workflow_gem_fmt = true;
  constexpr bool include_channel = true;  // or workflowJSON.runOptions()->debug();
  logFile.useWorkflowGemFormatter(use_workflow_gem_fmt, include_channel);
  logFile.setLogLevel(targetLogLevel);

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
                 "└{0:─^{2}}┘",
                 "", std::string("Starting State ") + message, 80);
      fmt::print("\n");
    }
    if (m_add_timings) {
      m_timers->newTimer(message);
    }
    (this->*job)();
    if (m_add_timings) {
      m_timers->tockCurrentTimer();
    }
    if (m_style_stdout) {
      fmt::print(fmt::fg(fmt::color::green), "{0:#^80}", std::string("Returned from State ") + message);
      fmt::print("\n");
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

  std::string lastFatalError;

  for (auto& [jobName, jobInfo] : jobMap) {
    LOG(Debug, fmt::format("{} - selected = {}\n", jobName, jobInfo.selected));
    if (jobInfo.selected) {
      try {
        timeJob(jobInfo.jobFun, std::string{jobName});
      } catch (std::exception& e) {
        if (m_add_timings) {
          m_timers->tockCurrentTimer();
        }
        lastFatalError = fmt::format("Found error in state '{}' with message: '{}'", jobName, e.what());
        LOG(Error, lastFatalError);
        // Allow continuing anyways if it fails in reporting measures
        if (jobName != "ReportingMeasures") {
          state = State::Errored;
          break;
        }
      }
    } else {
      LOG(Info, "Skipping job " << jobName);
    }
  }

  // TODO: Is this really necessary? Seems like it's done before already (in RunPreProcess)
  if (workspace_) {
    // Save final IDF
    if (m_add_timings) {
      m_timers->newTimer("Save IDF");
    }
    workspace_->save(runDirPath / "in.idf", true);
    if (m_add_timings) {
      m_timers->tockCurrentTimer();
    }
  }

  if (!workflowJSON.runOptions()->fast()) {
    communicateResults();
  }

  if (!lastFatalError.empty()) {
    // Because we allow RunReportingMeasures to fail so the workflow continues with the RunPostProcess / RunCleanup
    // but we still want to return a failure
    state = State::Errored;
    fmt::print(stderr, "Failed to run workflow. Last Error:\n  {}\n", lastFatalError);
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
    state = State::Finished;
  }

  if (m_add_timings) {
    fmt::print("\nTiming:\n\n{}\n", m_timers->timeReport());
    // TODO: create profile.json in the run folder
  }

  return (state == State::Finished);
}

Json::Value outputAttributesToJSON(const std::map<std::string, std::map<std::string, openstudio::Variant>>& output_attributes,
                                   bool sanitize = false) {
  Json::Value root(Json::objectValue);
  for (const auto& [oriMeasureName, argMap] : output_attributes) {
    const std::string measureName = sanitize ? openstudio::workflow::util::sanitizeKey(oriMeasureName) : oriMeasureName;
    Json::Value measureValues(Json::objectValue);
    for (const auto& [oriArgName, variantValue] : argMap) {
      const std::string argName = sanitize ? openstudio::workflow::util::sanitizeKey(oriArgName) : oriArgName;
      if (variantValue.variantType() == VariantType::String) {
        measureValues[argName] = variantValue.valueAsString();
      } else if (variantValue.variantType() == VariantType::Double) {
        measureValues[argName] = variantValue.valueAsDouble();
      } else if (variantValue.variantType() == VariantType::Integer) {
        measureValues[argName] = variantValue.valueAsInteger();
      } else if (variantValue.variantType() == VariantType::Boolean) {
        measureValues[argName] = variantValue.valueAsBoolean();
      }
    }

    root[measureName] = measureValues;
  }
  return root;
}

void OSWorkflow::communicateMeasureAttributes() const {

  const Json::Value root = outputAttributesToJSON(output_attributes, false);
  Json::StreamWriterBuilder wbuilder;
  // mimic the old StyledWriter behavior:
  wbuilder["indentation"] = "  ";

  const std::string result = Json::writeString(wbuilder, root);

  auto jsonPath = workflowJSON.absoluteRunDir() / "measure_attributes.json";
  openstudio::filesystem::ofstream file(jsonPath);
  OS_ASSERT(file.is_open());
  file << result;
  file.close();
}

void OSWorkflow::runExtractInputsAndOutputs() const {
  const Json::Value results = outputAttributesToJSON(output_attributes, true);
  Json::StreamWriterBuilder wbuilder;
  wbuilder["indentation"] = "  ";

  {
    const std::string result = Json::writeString(wbuilder, results);

    auto jsonPath = workflowJSON.absoluteRunDir() / "results.json";
    openstudio::filesystem::ofstream file(jsonPath);
    OS_ASSERT(file.is_open());
    file << result;
    file.close();
  }

  const auto osa_abs_path = workflowJSON.absoluteRootDir().parent_path() / "analysis.json";
  if (!openstudio::filesystem::is_regular_file(osa_abs_path)) {
    return;
  }

  std::ifstream ifs(openstudio::toSystemFilename(osa_abs_path));

  Json::CharReaderBuilder rbuilder;
  std::string formattedErrors;

  Json::Value analysis_json;
  const bool parsingSuccessful = Json::parseFromStream(rbuilder, ifs, &analysis_json, &formattedErrors);
  if (!parsingSuccessful) {
    LOG_AND_THROW("OSA Analysis JSON '" << toString(osa_abs_path) << "' cannot be processed, " << formattedErrors);
  }

  if (!openstudio::checkKeyAndType(analysis_json, "analysis", Json::objectValue)) {
    return;
  }

  if (!openstudio::checkKeyAndType(analysis_json["analysis"], "output_variables", Json::arrayValue)) {
    return;
  }

  Json::Value objectiveFunctions(Json::objectValue);

  auto& outputVars = analysis_json["analysis"]["output_variables"];
  for (const auto& variable : outputVars) {
    if (openstudio::checkKeyAndType(variable, "objective_function", Json::booleanValue) && variable["objective_function"].asBool()) {
      assertKeyAndType(variable, "name", Json::stringValue);
      assertKeyAndType(variable, "objective_function_index", Json::intValue);
      const std::string name = variable["name"].asString();
      const int idx = variable["objective_function_index"].asInt() + 1;

      LOG(Info, "Looking for objective function " << name);

      // Splitting on a `.` feels very unrealiable
      const size_t pos = name.find('.');
      if (pos == std::string::npos) {
        LOG(Warn, "Objective function name='" << name << "' does not contain a dot (`.`)");
        continue;
      }
      const std::string measureName = name.substr(0, pos);
      const std::string argName = name.substr(pos + 1);
      if (results.isMember(measureName) && results[measureName].isMember(argName)) {
        objectiveFunctions[fmt::format("objective_function_{}", idx)] = results[measureName][argName];

        if (openstudio::checkKeyAndType(variable, "objective_function_target", Json::realValue)) {
          LOG(Info, "Found objective function target for " << name);
          objectiveFunctions[fmt::format("objective_function_target_{}", idx)] = variable["objective_function_target"].asDouble();
        }

        if (openstudio::checkKeyAndType(variable, "scaling_factor", Json::realValue)) {
          LOG(Info, "Found scaling factor for " << name);
          objectiveFunctions[fmt::format("scaling_factor_{}", idx)] = variable["scaling_factor"].asDouble();
        }

        if (openstudio::checkKeyAndType(variable, "objective_function_group", Json::realValue)) {
          LOG(Info, "Found objective function group for " << name);
          objectiveFunctions[fmt::format("objective_function_group_{}", idx)] = variable["objective_function_group"].asDouble();
        }

      } else {
        LOG(Warn, "No results for objective function " << name);
        objectiveFunctions[fmt::format("objective_function_{}", idx)] = std::numeric_limits<double>::max();
        objectiveFunctions[fmt::format("objective_function_target_{}", idx)] = Json::nullValue;
        objectiveFunctions[fmt::format("scaling_factor_{}", idx)] = Json::nullValue;
        objectiveFunctions[fmt::format("objective_function_group_{}", idx)] = Json::nullValue;
      }
    }
  }

  {
    const std::string objectives = Json::writeString(wbuilder, objectiveFunctions);

    auto objectivesJsonPath = workflowJSON.absoluteRunDir() / "objectives.json";
    openstudio::filesystem::ofstream file(objectivesJsonPath);
    OS_ASSERT(file.is_open());
    file << objectives;
    file.close();
  }
}

void OSWorkflow::communicateResults() const {
  if (!workflowJSON.runOptions()->skipZipResults()) {
    if (m_add_timings) {
      m_timers->newTimer("Zip datapoint");
    }
    openstudio::workflow::util::zipResults(workflowJSON.absoluteRunDir());
    if (m_add_timings) {
      m_timers->tockCurrentTimer();
    }
  }

  const Json::Value root = outputAttributesToJSON(output_attributes, true);
  Json::StreamWriterBuilder wbuilder;
  // mimic the old StyledWriter behavior:
  wbuilder["indentation"] = "  ";

  const std::string result = Json::writeString(wbuilder, root);

  auto jsonPath = workflowJSON.absoluteRunDir() / "data_point_out.json";
  openstudio::filesystem::ofstream file(jsonPath);
  OS_ASSERT(file.is_open());
  file << result;
  file.close();
}

}  // namespace openstudio
