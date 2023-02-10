#include "./OSWorkflow.hpp"

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

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <stdexcept>

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

OSWorkflow::OSWorkflow(WorkflowRunOptions t_runOptions, ScriptEngineInstance& ruby, ScriptEngineInstance& python)
  :
#if USE_RUBY_ENGINE
    rubyEngine(ruby),
#endif
#if USE_PYTHON_ENGINE
    pythonEngine(python),
#endif
    workflowJSON(t_runOptions.osw_path),
    runOptions(std::move(t_runOptions)) {
}

void OSWorkflow::applyArguments(measure::OSArgumentMap& argumentMap, const std::string& argumentName, const openstudio::Variant& argumentValue) {
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

measure::OSArgumentMap OSWorkflow::argumentMap(measure::ModelMeasure* measure, const model::Model& model, const openstudio::MeasureStep& step) {
  measure::OSArgumentMap argumentMap;
  auto modelClone = model.clone(true).cast<model::Model>();
  const auto arguments = measure->arguments(modelClone);

  if (!arguments.empty()) {
    for (const auto& argument : arguments) {
      fmt::print("Argument: {}\n", argument.name());
      argumentMap[argument.name()] = argument.clone();
    }

    //  logger.debug "Iterating over arguments for workflow item '#{measure_dir_name}'"
    auto stepArgs = step.arguments();
    fmt::print("Current step has {} arguments\n", stepArgs.size());
    // handle skip first
    if (!stepArgs.empty() && stepArgs.contains("__SKIP__")) {
      // TODO: handling of SKIP is incomplete here, will need to increment the runner and co, and not process the measure

    } else {
      // TODO: I've copied workflow-gem here, but it's wrong. It's trying to issue a warning if an argument is not set, so it should loop on
      // argumentMap instead...
      for (auto const& [argumentName, argumentValue] : stepArgs) {
        applyArguments(argumentMap, argumentName, argumentValue);
      }
    }
  }

  return argumentMap;
};

void OSWorkflow::run() {
  if (runOptions.debug) {
    openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  }
  // 1. Instantiate seed model

  auto getModel = [this]() -> openstudio::model::Model {
    auto seedPath_ = workflowJSON.seedFile();
    if (!seedPath_) {
      return openstudio::model::Model{};
    }

    auto modelFullPath_ = workflowJSON.findFile(seedPath_.get());
    if (!modelFullPath_) {
      throw std::runtime_error(fmt::format("Seed model {} specified in OSW cannot be found", seedPath_.get()));
    }
    openstudio::osversion::VersionTranslator vt;
    auto m_ = vt.loadModel(modelFullPath_.get());
    if (!m_) {
      throw std::runtime_error(fmt::format("Failed to load OSM file {}\n", openstudio::toString(seedPath_.get())));
    }
    return m_.get();
  };

  model::Model model = getModel();

  auto runDir = workflowJSON.absoluteRunDir();
  openstudio::filesystem::remove_all(runDir);
  openstudio::filesystem::create_directory(runDir);


  const auto modelSteps = workflowJSON.getMeasureSteps(MeasureType::ModelMeasure);
  for (const auto& step : modelSteps) {
    unsigned stepIndex = workflowJSON.currentStepIndex();
    fmt::print("\n\nRunning step {}\n", stepIndex);

    const auto measureDirName = step.measureDirName();
    const auto measureDirPath_ = workflowJSON.findMeasure(measureDirName);
    if (!measureDirPath_) {
      fmt::print("Could not find measure '{}'\n", measureDirName);
      continue;
    }
    BCLMeasure bclMeasure(measureDirPath_.get());

    if (bclMeasure.measureLanguage() == MeasureLanguage::Ruby) {
      rubyEngine->applyMeasure(model, runner, bclMeasure, step);
    } else if (bclMeasure.measureLanguage() == MeasureLanguage::Python) {
      pythonEngine->applyMeasure(model, runner, bclMeasure, step);
    }

    WorkflowStepResult result = runner.result();
    if (auto stepResult_ = result.stepResult()) {
      fmt::print("Step Result: {}\n", stepResult_->valueName());
    }
    // incrementStep must be called after run
    runner.incrementStep();
    if (auto errors = result.stepErrors(); !errors.empty()) {
      throw std::runtime_error(fmt::format("Measure {} reported an error with [{}]\n", measureDirName, fmt::join(errors, "\n")));
    }

    if (bclMeasure.measureType() == MeasureType::ModelMeasure) {
      if (auto weatherFile_ = model.weatherFile()) {
        if (auto p_ = weatherFile_->path()) {
          // Probably a workflowJSON.findFile() call...
          // m_epwPath_ = p_;
        } else {
          fmt::print("Weather file object found in model but no path is given\n");
        }
      }
    }
  }  // End for (const auto& step : modelSteps)
}
}  // namespace openstudio
