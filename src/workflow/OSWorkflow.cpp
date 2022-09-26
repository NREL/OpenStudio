#include "./OSWorkflow.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"

#include "../osversion/VersionTranslator.hpp"
#include "../measure/OSMeasure.hpp"
#include "../measure/ModelMeasure.hpp"
#include "../measure/EnergyPlusMeasure.hpp"
#include "../measure/ReportingMeasure.hpp"
#include "../measure/OSArgument.hpp"
#include "../measure/OSRunner.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/data/Variant.hpp"
#include "../energyplus/ForwardTranslator.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <stdexcept>

namespace openstudio {

OSWorkflow::OSWorkflow(const filesystem::path& oswPath, ScriptEngineInstance& ruby, ScriptEngineInstance& python)
  : rubyEngine(ruby), pythonEngine(python), workflowJSON(oswPath) {}

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
  rubyEngine->exec("puts 'Hello from Ruby'");
  pythonEngine->exec("print('Hello from Python')");

  rubyEngine->registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");
  rubyEngine->registerType<openstudio::measure::EnergyPlusMeasure*>("openstudio::measure::EnergyPlusMeasure *");
  pythonEngine->registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");

  // -1: gotta init the rest of OpenStudio in ruby, so that Ruleset and co are defined
  rubyEngine->exec("OpenStudio::init_rest_of_openstudio()");

  //// O. Need to apply measure steps IN ORDER. (eg: OpenStudio Measures before Eplus measures etc)
  //// https://github.com/NREL/OpenStudio-workflow-gem/blob/develop/lib/openstudio/workflow/util/measure.rb

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
  boost::optional<openstudio::Workspace> workspace_;

  // 2. determine ruby or python
  // 3. import measure.(py|rb)
  // 4. instantiate measure
  // 5. run measure

  for (openstudio::MeasureType stepType : {openstudio::MeasureType::ModelMeasure, openstudio::MeasureType::EnergyPlusMeasure}) {
    const auto modelSteps = workflowJSON.getMeasureSteps(stepType);
    if (stepType == MeasureType::EnergyPlusMeasure) {
      openstudio::energyplus::ForwardTranslator ft;
      workspace_ = ft.translateModel(model);
    }

    for (const auto& step : modelSteps) {
      unsigned stepIndex = workflowJSON.currentStepIndex();
      fmt::print("\n\nRunning step {}\n", stepIndex);

      //  measure_run_dir = File.join(run_dir, "#{step_index.to_s.rjust(3,'0')}_#{measure_dir_name}")
      //  logger.debug "Creating run directory for measure in #{measure_run_dir}"
      //  FileUtils.mkdir_p measure_run_dir
      //  Dir.chdir measure_run_dir

      const auto measureDirName = step.measureDirName();
      const auto measureDirPath_ = workflowJSON.findMeasure(measureDirName);
      if (!measureDirPath_) {
        fmt::print("Could not find measure '{}'\n", measureDirName);
        continue;
      }
      BCLMeasure bclMeasure(measureDirPath_.get());

      const auto scriptPath_ = bclMeasure.primaryScriptPath();
      if (!scriptPath_) {
        fmt::print("Could not find primaryScriptPath '{}'\n", measureDirName);
        continue;
      }
      fmt::print("Found {} at primaryScriptPath: '{}'\n", measureDirName, openstudio::toString(scriptPath_.get()));
      const std::string className = bclMeasure.className();
      const auto measureType = bclMeasure.measureType();
      const MeasureLanguage measureLanguage = bclMeasure.measureLanguage();

      // TODO: will add a Logger later
      fmt::print("Class Name: {}\n", className);
      fmt::print("Measure Script Path: {}\n", openstudio::toString(scriptPath_.get()));
      fmt::print("Measure Type: {}\n", bclMeasure.measureType().valueName());
      fmt::print("Measure Language: {}\n", measureLanguage.valueName());

      //openstudio::measure::ModelMeasure* modelMeasurePtr = nullptr;
      // TODO: probably want to do that ultimately, then static_cast appropriately
      ScriptEngineInstance* thisEngine;
      ScriptObject measureScriptObject;
      openstudio::measure::OSMeasure* measurePtr = nullptr;

      auto getArguments = [&](measure::OSMeasure* measure) -> measure::OSArgumentMap {
        if (!measure) {
          throw std::runtime_error(fmt::format("Could not load measure at '{}'", openstudio::toString(scriptPath_.get())));
        }
        // Initialize arguments which may be model dependent, don't allow arguments method access to real model in case it changes something
        std::vector<measure::OSArgument> arguments;

        fmt::print("measure->name()= '{}'\n", measure->name());

        if (measureType == MeasureType::ModelMeasure) {
          // For computing arguments
          auto modelClone = model.clone(true).cast<model::Model>();
          arguments = static_cast<openstudio::measure::ModelMeasure*>(measure)->arguments(modelClone);
        } else if (measureType == MeasureType::EnergyPlusMeasure) {
          auto workspaceClone = workspace_->clone(true).cast<openstudio::Workspace>();
          arguments = static_cast<openstudio::measure::EnergyPlusMeasure*>(measure)->arguments(workspaceClone);
        } else if (measureType == MeasureType::ReportingMeasure) {
          auto modelClone = model.clone(true).cast<model::Model>();
          arguments = static_cast<openstudio::measure::ReportingMeasure*>(measure)->arguments(modelClone);
        }

        measure::OSArgumentMap argumentMap;
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

      if (measureLanguage == MeasureLanguage::Ruby) {
        // TODO: probably need to do path formatting properly for windows
        auto importCmd = fmt::format("require '{}'", openstudio::toString(scriptPath_.get()));
        rubyEngine->exec(importCmd);
        measureScriptObject = rubyEngine->eval(fmt::format("{}.new()", className));
        thisEngine = &rubyEngine;
      } else if (measureLanguage == MeasureLanguage::Python) {
        // place measureDirPath in sys.path; do from measure import MeasureName
        // I think this can't work without a "as xxx" otherwise we'll repeatedly try to import a module named 'measure'
        // pythonEngine->pyimport("measure", openstudio::toString(measureDirPath.get()));
        auto importCmd = fmt::format(R"python(
import sys
sys.path.insert(0, '{0}')
import measure
)python",
                                     openstudio::toString(scriptPath_->parent_path()), className);
        pythonEngine->exec(importCmd);
        measureScriptObject = pythonEngine->eval(fmt::format("measure.{}()", className));
        thisEngine = &pythonEngine;
      }

      // This pointer will only be valid for as long as the above PythonMeasure is in scope
      // After that, dereferencing the measure pointer will crash the program
      if (measureType == MeasureType::ModelMeasure) {
        measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);
      } else if (measureType == MeasureType::EnergyPlusMeasure) {
        measurePtr = (*thisEngine)->getAs<openstudio::measure::EnergyPlusMeasure*>(measureScriptObject);
      } else if (measureType == MeasureType::ReportingMeasure) {
        measurePtr = (*thisEngine)->getAs<openstudio::measure::ReportingMeasure*>(measureScriptObject);
      }

      const auto argmap = getArguments(measurePtr);
      // There is a bug. I can run one measure but not two. The one measure can be either python or ruby
      // I think it might have to do with the operations that must be done to the runner to reset state. maybe?
      if (measureType == MeasureType::ModelMeasure) {
        static_cast<openstudio::measure::ModelMeasure*>(measurePtr)->run(model, runner, argmap);
      } else if (measureType == MeasureType::EnergyPlusMeasure) {
        static_cast<openstudio::measure::EnergyPlusMeasure*>(measurePtr)->run(workspace_.get(), runner, argmap);
      } else if (measureType == MeasureType::ReportingMeasure) {
        static_cast<openstudio::measure::ReportingMeasure*>(measurePtr)->run(runner, argmap);
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

      if (measureType == MeasureType::ModelMeasure) {
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
}
}  // namespace openstudio
