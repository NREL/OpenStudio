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

  auto timeJob = [this](memJobFunPtr job, auto message) {
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

  // 1. Instantiate seed model
  timeJob(&OSWorkflow::runInitialization, "Initialization");

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

  for (const MeasureType stepType : {MeasureType::ModelMeasure, MeasureType::EnergyPlusMeasure, MeasureType::ReportingMeasure}) {

    if (stepType == MeasureType::EnergyPlusMeasure) {
      timeJob(&OSWorkflow::runTranslator, "Translator");
    }

    if (stepType == MeasureType::ReportingMeasure) {
      timeJob(&OSWorkflow::runEnergyPlus, "E+ Simulation");
      continue;
    }

    if (m_add_timings) {
      m_timers->newTimer(stepType.valueName());
    }
    const auto modelSteps = workflowJSON.getMeasureSteps(stepType);
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

      auto getArguments = [this, &measureType, &scriptPath_, &step,
                           &measureLanguage](openstudio::measure::OSMeasure* measurePtr) -> measure::OSArgumentMap {
        if (!measurePtr) {
          throw std::runtime_error(fmt::format("Could not load measure at '{}'", openstudio::toString(scriptPath_.get())));
        }
        // Initialize arguments which may be model dependent, don't allow arguments method access to real model in case it changes something
        std::vector<measure::OSArgument> arguments;

        fmt::print("measure->name()= '{}'\n", measurePtr->name());

        if (measureType == MeasureType::ModelMeasure) {
          // For computing arguments
          auto modelClone = model.clone(true).cast<model::Model>();
          if (measureLanguage == MeasureLanguage::Ruby) {
            arguments = static_cast<openstudio::measure::ModelMeasure*>(measurePtr)->arguments(modelClone);  // NOLINT
          } else if (measureLanguage == MeasureLanguage::Python) {
            arguments = static_cast<openstudio::measure::PythonModelMeasure*>(measurePtr)->arguments(modelClone);  // NOLINT
          }
        } else if (measureType == MeasureType::EnergyPlusMeasure) {
          auto workspaceClone = workspace_->clone(true).cast<openstudio::Workspace>();
          if (measureLanguage == MeasureLanguage::Ruby) {
            arguments = static_cast<openstudio::measure::EnergyPlusMeasure*>(measurePtr)->arguments(workspaceClone);  // NOLINT
          } else if (measureLanguage == MeasureLanguage::Python) {
            arguments = static_cast<openstudio::measure::PythonEnergyPlusMeasure*>(measurePtr)->arguments(workspaceClone);  // NOLINT
          }
        } else if (measureType == MeasureType::ReportingMeasure) {
          auto modelClone = model.clone(true).cast<model::Model>();
          if (measureLanguage == MeasureLanguage::Ruby) {
            arguments = static_cast<openstudio::measure::ReportingMeasure*>(measurePtr)->arguments(modelClone);  // NOLINT
          } else if (measureLanguage == MeasureLanguage::Python) {
            arguments = static_cast<openstudio::measure::PythonReportingMeasure*>(measurePtr)->arguments(modelClone);  // NOLINT
          }
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
        if (measureLanguage == MeasureLanguage::Ruby) {
          measurePtr = (*thisEngine)->getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);
        } else if (measureLanguage == MeasureLanguage::Python) {
          measurePtr = (*thisEngine)->getAs<openstudio::measure::PythonModelMeasure*>(measureScriptObject);
        }
      } else if (measureType == MeasureType::EnergyPlusMeasure) {
        if (measureLanguage == MeasureLanguage::Ruby) {
          measurePtr = (*thisEngine)->getAs<openstudio::measure::EnergyPlusMeasure*>(measureScriptObject);
        } else if (measureLanguage == MeasureLanguage::Python) {
          measurePtr = (*thisEngine)->getAs<openstudio::measure::PythonEnergyPlusMeasure*>(measureScriptObject);
        }
      } else if (measureType == MeasureType::ReportingMeasure) {
        if (measureLanguage == MeasureLanguage::Ruby) {
          measurePtr = (*thisEngine)->getAs<openstudio::measure::ReportingMeasure*>(measureScriptObject);
        } else if (measureLanguage == MeasureLanguage::Python) {
          measurePtr = (*thisEngine)->getAs<openstudio::measure::PythonReportingMeasure*>(measureScriptObject);
        }
      }

      const auto argmap = getArguments(measurePtr);
      // There is a bug. I can run one measure but not two. The one measure can be either python or ruby
      // I think it might have to do with the operations that must be done to the runner to reset state. maybe?
      if (measureType == MeasureType::ModelMeasure) {
        if (measureLanguage == MeasureLanguage::Ruby) {
          static_cast<openstudio::measure::ModelMeasure*>(measurePtr)->run(model, runner, argmap);
        } else if (measureLanguage == MeasureLanguage::Python) {
          static_cast<openstudio::measure::PythonModelMeasure*>(measurePtr)->run(model, runner, argmap);
        }
      } else if (measureType == MeasureType::EnergyPlusMeasure) {
        if (measureLanguage == MeasureLanguage::Ruby) {
          static_cast<openstudio::measure::EnergyPlusMeasure*>(measurePtr)->run(workspace_.get(), runner, argmap);
        } else if (measureLanguage == MeasureLanguage::Python) {
          static_cast<openstudio::measure::PythonEnergyPlusMeasure*>(measurePtr)->run(workspace_.get(), runner, argmap);
        }
      } else if (measureType == MeasureType::ReportingMeasure) {
        if (measureLanguage == MeasureLanguage::Ruby) {
          static_cast<openstudio::measure::ReportingMeasure*>(measurePtr)->run(runner, argmap);
        } else if (measureLanguage == MeasureLanguage::Python) {
          static_cast<openstudio::measure::PythonReportingMeasure*>(measurePtr)->run(runner, argmap);
        }
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

    if (m_add_timings) {
      m_timers->tockCurrentTimer();
    }
  }  // End for StepType

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
