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

void OSWorkflow::run() {
  if (runOptions.debug) {
    openstudio::Logger::instance().standardOutLogger().setLogLevel(Debug);
  }
  // 1. Instantiate seed model

  auto runInitialization = [this]() -> openstudio::model::Model {
    fmt::print("Debug: Finding and loading the seed file\n");
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

  model::Model model = runInitialization();

  auto runDir = workflowJSON.absoluteRunDir();
  openstudio::filesystem::remove_all(runDir);
  openstudio::filesystem::create_directory(runDir);

  // 2. determine ruby or python
  // 3. import measure.(py|rb)
  // 4. instantiate measure
  // 5. run measure

  // TODO: need to add the ReportingMeasure to the Mix
  // TODO: need to run the idf through EnergyPlus
  // TODO: need to create subdirectories for steps
  // TODO: need to do some cleanup, like removing old sim files, maybe reports, etc
  // TODO: need to implement some of the options/flags like --postprocess_only
  // TODO: need to merge workflowJSON flags with flags from command line (eg: FT options)
  // TODO: need to modify utilities/RunOptions.hpp instead of duplicating some of that work in workflow

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
        rubyEngine->applyMeasure(model, runner, bclMeasure);
        rubyEngine.reset();
      } else if (bclMeasure.measureLanguage() == MeasureLanguage::Python) {
        pythonEngine->applyMeasure(model, runner, bclMeasure);
        pythonEngine.reset();
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
