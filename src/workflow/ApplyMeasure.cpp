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

#include "../utilities/filetypes/WorkflowJSON.hpp"
#include "../utilities/filetypes/RunOptions.hpp"
#include <boost/filesystem/operations.hpp>

namespace openstudio {

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

    //  measure_run_dir = File.join(run_dir, "#{step_index.to_s.rjust(3,'0')}_#{measure_dir_name}")
    //  logger.debug "Creating run directory for measure in #{measure_run_dir}"
    //  FileUtils.mkdir_p measure_run_dir
    //  Dir.chdir measure_run_dir

    if (model.numObjects() > 0) {
      runner.setLastOpenStudioModel(model);
    }
    if (workspace_) {
      runner.setLastEnergyPlusWorkspace(workspace_.get());
    }
    if (!sqlPath.empty()) {
      runner.setLastEnergyPlusSqlFilePath(sqlPath);
    }
    if (!epwPath.empty()) {
      runner.setLastEpwFilePath(epwPath);
    }

    const auto measureDirName = step.measureDirName();
    if (openstudio::filesystem::path(measureDirName).is_absolute()) {
      LOG(Warn, "measure_dir_name should not be a full path. It should be a relative path to the measure directory or the name of the measure "
                "directory containing the measure.rb / measure.py file.");
    }

    if (m_add_timings && m_detailed_timings) {
      m_timers->newTimer(fmt::format("Measure::{}", measureDirName), 2);
    }

    openstudio::filesystem::path curDirPath = boost::filesystem::current_path();
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

    //openstudio::measure::ModelMeasure* modelMeasurePtr = nullptr;
    // TODO: probably want to do that ultimately, then static_cast appropriately
    ScriptEngineInstance* thisEngine = nullptr;
    ScriptObject measureScriptObject;
    openstudio::measure::OSMeasure* measurePtr = nullptr;

    auto getArguments = [this, &measureType, &scriptPath_, &step](openstudio::measure::OSMeasure* measurePtr) -> measure::OSArgumentMap {
      if (!measurePtr) {
        throw std::runtime_error(fmt::format("Could not load measure at '{}'", openstudio::toString(scriptPath_.get())));
      }
      // Initialize arguments which may be model dependent, don't allow arguments method access to real model in case it changes something
      std::vector<measure::OSArgument> arguments;

      LOG(Debug, "measure->name()= '" << measurePtr->name() << "'");

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

      measure::OSArgumentMap argumentMap;
      if (!arguments.empty()) {
        for (const auto& argument : arguments) {
          LOG(Debug, "Argument: " << argument.name());
          argumentMap[argument.name()] = argument.clone();
        }

        //  logger.debug "Iterating over arguments for workflow item '#{measure_dir_name}'"
        auto stepArgs = step.arguments();
        LOG(Debug, "Current step has " << stepArgs.size() << " arguments");
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
#if USE_RUBY_ENGINE
      auto importCmd = fmt::format("require '{}'", openstudio::toString(scriptPath_.get()));
      rubyEngine->exec(importCmd);
      measureScriptObject = rubyEngine->eval(fmt::format("{}.new()", className));
      thisEngine = &rubyEngine;
#else
      throw std::runtime_error("Cannot run a Ruby measure when RubyEngine isn't enabled");
#endif
    } else if (measureLanguage == MeasureLanguage::Python) {
#if USE_PYTHON_ENGINE
      // place measureDirPath in sys.path; do from measure import MeasureName
      // I think this can't work without a "as xxx" otherwise we'll repeatedly try to import a module named 'measure'
      // pythonEngine->pyimport("measure", openstudio::toString(measureDirPath.get()));
      //         auto importCmd = fmt::format(R"python(
      // import sys
      // sys.path.insert(0, r'{}')
      // force_reload = 'measure' in sys.modules
      // import measure
      // if force_reload:
      //     # print("force reload measure")
      //     import importlib
      //     importlib.reload(measure)
      // )python",
      //                                      scriptPath_->parent_path().generic_string());
      auto importCmd = fmt::format(R"python(
import importlib.util
spec = importlib.util.spec_from_file_location('{}', r'{}')
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
)python",
                                   className, scriptPath_->generic_string());
      // fmt::print("\nimportCmd:\n{}\n", importCmd);
      pythonEngine->exec(importCmd);
      // measureScriptObject = pythonEngine->eval(fmt::format("measure.{}()", className));
      measureScriptObject = pythonEngine->eval(fmt::format("module.{}()", className));

      thisEngine = &pythonEngine;
#else
      throw std::runtime_error("Cannot run a Python measure when PythonEngine isn't enabled");
#endif
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

    WorkflowStepResult result = runner.result();
    if (auto stepResult_ = result.stepResult()) {
      LOG(Debug, "Step Result: " << stepResult_->valueName());
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
          LOG(Warn, "Weather file object found in model but no path is given");
        }
      }
    }

    if (m_add_timings && m_detailed_timings) {
      m_timers->tockCurrentTimer();
    }

    boost::filesystem::current_path(curDirPath);

  }  // End for each step

  if (m_add_timings && m_detailed_timings) {
    m_timers->tockCurrentTimer();
  }
}

}  // namespace openstudio
