/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "OSWorkflow.hpp"

#include "Util.hpp"

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

#include "../utilities/core/ASCIIStrings.hpp"
#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/RunOptions.hpp"
#include <boost/filesystem/operations.hpp>

#include <fmt/format.h>
#include <fmt/ranges.h>

namespace openstudio {

bool isStepMarkedSkip(const std::map<std::string, openstudio::Variant>& stepArgs) {
  bool skip_measure = false;

  // handle skip first
  if (!stepArgs.empty() && stepArgs.contains("__SKIP__")) {
    const openstudio::Variant& argumentValue = stepArgs.at("__SKIP__");
    skip_measure = argumentValue.isTrueish();
  }
  return skip_measure;
}

void OSWorkflow::applyMeasures(MeasureType measureType, bool energyplus_output_requests) {

  if (m_add_timings && m_detailed_timings) {
    m_timers->newTimer(fmt::format("{}:apply_measures", measureType.valueName()), 1);
  }

  LOG(Debug, "Finding measures of type " << measureType.valueName());

  // We actually want the order of the steps preserved, so this won't cut it
  // const auto modelSteps = workflowJSON.getMeasureSteps(stepType);

  for (const auto& stepAndIndex : workflowJSON.getMeasureStepsWithIndex(measureType)) {
    auto stepIndex = stepAndIndex.first;
    auto step = stepAndIndex.second;
    LOG(Debug, "Running step " << stepIndex);

    const auto measureDirName = step.measureDirName();

    auto stepArgs = step.arguments();
    const bool skip_measure = isStepMarkedSkip(stepArgs);
    if (skip_measure || runner.halted()) {
      if (!energyplus_output_requests) {
        if (runner.halted()) {
          LOG(Info, fmt::format("Skipping measure '{}' because simulation halted", measureDirName));
        } else {
          LOG(Info, fmt::format("Skipping measure '{}'", measureDirName));
          // required to update current step
          runner.prepareForMeasureRun();

          WorkflowStepResult result = runner.result();
          runner.incrementStep();
          const auto measureDirPath_ = workflowJSON.findMeasure(measureDirName);
          if (measureDirPath_) {
            BCLMeasure bclMeasure(measureDirPath_.get());
            workflow::util::addResultMeasureInfo(result, bclMeasure);
            const std::string className = bclMeasure.className();
            const auto measureName = step.name().value_or(className);
            output_attributes[measureName]["applicable"] = openstudio::Variant(false);
          } else {
            LOG(Warn, "Could not find measure '" << measureDirName << "', but it's marked as skipped anyways so ignoring.");
            output_attributes[step.name().value_or(measureDirName)]["applicable"] = openstudio::Variant(false);
          }
          result.setStepResult(StepResult::Skip);
        }
      }
      continue;
    }

    if (model.numObjects() > 0) {
      runner.setLastOpenStudioModel(model);
    }
    if (workspace_) {
      runner.setLastEnergyPlusWorkspace(workspace_.get());
    }
    if (!sqlPath.empty()) {
      runner.setLastEnergyPlusSqlFilePath(sqlPath);
    }

    updateLastWeatherFileFromModel();

    if (openstudio::filesystem::path(measureDirName).is_absolute()) {
      LOG(Warn, "measure_dir_name should not be a full path. It should be a relative path to the measure directory or the name of the measure "
                "directory containing the measure.rb / measure.py file.");
    }

    if (m_add_timings && m_detailed_timings) {
      m_timers->newTimer(fmt::format("Measure::{}", measureDirName), 2);
    }
    openstudio::filesystem::path curDirPath = boost::filesystem::current_path();

    auto ensureBlock = [this, &curDirPath](bool stepHasThrown) {
      if (m_add_timings && m_detailed_timings) {
        m_timers->tockCurrentTimer();
        if (stepHasThrown) {
          m_timers->tockCurrentTimer();
        }
      }
      boost::filesystem::current_path(curDirPath);
    };

    auto thisRunDir = workflowJSON.absoluteRunDir() / openstudio::toPath(fmt::format("{:03}_{}", stepIndex, measureDirName));
    LOG(Debug, "Creating run directory for measure in '" << thisRunDir << "'");
    openstudio::filesystem::create_directory(thisRunDir);
    boost::filesystem::current_path(thisRunDir);

    const auto measureDirPath_ = workflowJSON.findMeasure(measureDirName);
    if (!measureDirPath_) {
      LOG_AND_THROW("Could not find measure '" << measureDirName << "'.");
      continue;
    }
    BCLMeasure bclMeasure(measureDirPath_.get());

    const auto scriptPath_ = bclMeasure.primaryScriptPath();
    if (!scriptPath_) {
      LOG(Warn, "Could not find primaryScriptPath '" << measureDirName << "'");
      continue;
    }
    LOG(Debug, fmt::format("Found {} at primaryScriptPath: '{}'", measureDirName, openstudio::toString(scriptPath_.get())));
    const std::string className = bclMeasure.className();
    const auto measureType = bclMeasure.measureType();
    const MeasureLanguage measureLanguage = bclMeasure.measureLanguage();

    // TODO: will add a Logger later
    LOG(Debug, "Class Name: " << className);
    LOG(Debug, "Measure Script Path: " << scriptPath_.get());
    LOG(Debug, "Measure Type: " << bclMeasure.measureType().valueName());
    LOG(Debug, "Measure Language: " << measureLanguage.valueName());

    auto getArguments = [this, &measureType, &stepArgs](openstudio::measure::OSMeasure* measurePtr) -> measure::OSArgumentMap {
      LOG(Debug, "measure->name()= '" << measurePtr->name() << "'");

      measure::OSArgumentMap argumentMap;

      //  logger.debug "Iterating over arguments for workflow item '#{measure_dir_name}'"
      LOG(Debug, "Current step has " << stepArgs.size() << " arguments");

      // Initialize arguments which may be model dependent, don't allow arguments method access to real model in case it changes something
      std::vector<measure::OSArgument> arguments;

      if (measureType == MeasureType::ModelMeasure) {
        // For computing arguments
        auto modelClone = model.clone(true).cast<model::Model>();
        arguments = static_cast<openstudio::measure::ModelMeasure*>(measurePtr)->arguments(modelClone);  // NOLINT
      } else if (measureType == MeasureType::EnergyPlusMeasure) {
        auto workspaceClone = workspace_->clone(true).cast<openstudio::Workspace>();
        arguments = static_cast<openstudio::measure::EnergyPlusMeasure*>(measurePtr)->arguments(workspaceClone);  // NOLINT
      } else if (measureType == MeasureType::ReportingMeasure) {
        auto modelClone = model.clone(true).cast<model::Model>();
        arguments = static_cast<openstudio::measure::ReportingMeasure*>(measurePtr)->arguments(modelClone);  // NOLINT
      }

      if (!arguments.empty()) {
        for (const auto& argument : arguments) {
          // LOG(Debug, "Argument: " << argument);
          argumentMap[argument.name()] = argument.clone();
        }

        if (!stepArgs.empty()) {
          // TODO: I've copied workflow-gem here, but it's wrong. It's trying to issue a warning if an argument is not set, so it should loop on
          // argumentMap instead...
          for (auto const& [argumentName, argumentValue] : stepArgs) {
            if (argumentName == "__SKIP__") {
              continue;
            }
            applyArguments(argumentMap, argumentName, argumentValue);
          }
        }
      }

      std::map<std::string, std::string> formattedArgMap;
      std::transform(argumentMap.begin(), argumentMap.end(), std::inserter(formattedArgMap, formattedArgMap.begin()),
                     [](const auto& p) { return std::make_pair(p.first, p.second.printValue(true)); });
      LOG(Debug, fmt::format("argumentMap={}", formattedArgMap));
      return argumentMap;
    };

    ScriptEngineInstance* thisEngine = nullptr;
    if (measureLanguage == MeasureLanguage::Ruby) {
      // TODO: probably need to do path formatting properly for windows
#if USE_RUBY_ENGINE
      thisEngine = &rubyEngine;
#else
      ensureBlock(true);
      throw std::runtime_error("Cannot run a Ruby measure when RubyEngine isn't enabled");
#endif
    } else if (measureLanguage == MeasureLanguage::Python) {
#if USE_PYTHON_ENGINE
      thisEngine = &pythonEngine;
#else
      ensureBlock(true);
      throw std::runtime_error("Cannot run a Python measure when PythonEngine isn't enabled");
#endif
    }

    ScriptObject measureScriptObject = (*thisEngine)->loadMeasure(*scriptPath_, className);
    if (measureScriptObject.empty()) {
      ensureBlock(true);
      throw std::runtime_error(fmt::format("Failed to load measure '{}' from '{}'\n", className, openstudio::toString(scriptPath_.get())));
    }

    // This pointer will only be valid for as long as the above PythonMeasure is in scope
    // After that, dereferencing the measure pointer will crash the program
    auto* measurePtr = (*thisEngine)->getAs<openstudio::measure::OSMeasure*>(measureScriptObject);
    if (!measurePtr) {
      ensureBlock(true);
      throw std::runtime_error(fmt::format("Could not load measure at '{}'", openstudio::toString(scriptPath_.get())));
    }

    // Patch measure if needed
    bool was_patched = false;
    if (measureType == MeasureType::ReportingMeasure) {
      const int numArgs = (*thisEngine)->numberOfArguments(measureScriptObject, "arguments");
      // fmt::print("numArgs={}\n", numArgs);
      if (numArgs == 0) {
        if (measureLanguage == MeasureLanguage::Ruby) {
          auto msg = fmt::format("Reporting Measure at '{}' is using the old format where the 'arguments' method does not take model. "
                                 " Please consider updating this to `def arguments(model)`.",
                                 scriptPath_->generic_string());
          LOG(Warn, msg);
          auto patchArgumentsCmd = fmt::format(R"ruby(
module {0}Extensions
  def arguments(model=nil)
    super()
  end
end

class {0}
  prepend {0}Extensions
end
)ruby",
                                               className);
          rubyEngine->exec(patchArgumentsCmd);
          was_patched = true;
        } else {
          auto msg = fmt::format("Wrong number of parameters for method `arguments` in reporting_measure '{}' from '{}'\n", className,
                                 scriptPath_->generic_string());
          ensureBlock(true);
          throw std::runtime_error(msg);
        }
      }
    }

    const auto argmap = getArguments(measurePtr);

    // TODO: this doesn't protect! it'll crash if a wrong method is used in the ruby measure for eg
    try {
      if (measureType == MeasureType::ModelMeasure) {
        static_cast<openstudio::measure::ModelMeasure*>(measurePtr)->run(model, runner, argmap);
      } else if (measureType == MeasureType::EnergyPlusMeasure) {
        static_cast<openstudio::measure::EnergyPlusMeasure*>(measurePtr)->run(workspace_.get(), runner, argmap);
      } else if (measureType == MeasureType::ReportingMeasure) {
        if (energyplus_output_requests) {
          LOG(Debug, "Calling measure.energyPlusOutputRequests for '" << measureDirName << "'");

          std::vector<IdfObject> idfObjects;
          idfObjects = static_cast<openstudio::measure::ReportingMeasure*>(measurePtr)->energyPlusOutputRequests(runner, argmap);

          int num_added = 0;
          for (auto& idfObject : idfObjects) {
            if (openstudio::workflow::util::addEnergyPlusOutputRequest(workspace_.get(), idfObject)) {
              ++num_added;
            }
          }
          LOG(Debug, "Finished measure.energyPlusOutputRequests for '" << measureDirName << "', " << num_added << " output requests added");
        } else {
          static_cast<openstudio::measure::ReportingMeasure*>(measurePtr)->run(runner, argmap);
        }
      }
    } catch (const std::exception& e) {
      runner.registerError(e.what());
      if (!energyplus_output_requests) {
        WorkflowStepResult result = runner.result();
        // incrementStep must be called after run
        runner.incrementStep();
        workflow::util::addResultMeasureInfo(result, bclMeasure);
      }
      ensureBlock(true);
      throw std::runtime_error(fmt::format("Runner error: Measure '{}' reported an error with [{}]", scriptPath_->generic_string(), e.what()));
    }

    // if doing output requests we are done now
    if (!energyplus_output_requests) {
      WorkflowStepResult result = runner.result();

      // incrementStep must be called after run
      runner.incrementStep();
      workflow::util::addResultMeasureInfo(result, bclMeasure);
      if (auto errors = result.stepErrors(); !errors.empty()) {
        ensureBlock(true);
        throw std::runtime_error(fmt::format("Measure '{}' reported an error with [{}]", measureDirName, fmt::join(errors, "\n")));
      }

      const auto measureName = step.name().value_or(className);
      auto& measureAttributes = output_attributes[measureName];
      for (const auto& stepValue : result.stepValues()) {
        measureAttributes[stepValue.name()] = stepValue.valueAsVariant();
      }
      auto stepResult_ = result.stepResult();
      if (!stepResult_.has_value()) {
        LOG_AND_THROW("Step Result not set for '" << scriptPath_->generic_string() << "'");
      }

      // Add an applicability flag to all the measure results
      const StepResult stepResult = std::move(*stepResult_);
      LOG(Debug, "Step Result: " << stepResult.valueName());
      measureAttributes["applicable"] = openstudio::Variant(!((stepResult == StepResult::NA) || (stepResult == StepResult::Skip)));

      if (measureType == MeasureType::ModelMeasure) {
        updateLastWeatherFileFromModel();
      }
    }

    if (was_patched) {
      rubyEngine->exec(fmt::format("Object.send(:remove_const, :{}Extensions)", className));
    }

    ensureBlock(false);

  }  // End for each step

  if (m_add_timings && m_detailed_timings) {
    m_timers->tockCurrentTimer();
  }
}

}  // namespace openstudio
