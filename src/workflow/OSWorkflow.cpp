#include "./OSWorkflow.hpp"
#include "../utilities/filetypes/WorkflowStep.hpp"
#include "../utilities/bcl/BCLMeasure.hpp"

#include "../osversion/VersionTranslator.hpp"
#include "../measure/OSMeasure.hpp"
#include "../measure/ModelMeasure.hpp"
#include "../measure/OSArgument.hpp"
#include "../measure/OSRunner.hpp"
#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../utilities/data/Variant.hpp"

#include <fmt/format.h>
#include <fmt/ostream.h>
#include <stdexcept>

namespace openstudio {

OSWorkflow::OSWorkflow(const filesystem::path& oswPath, const ScriptEngineInstance& ruby, const ScriptEngineInstance& python)
  : rubyEngine(ruby), pythonEngine(python), workflowJSON(oswPath) {}

void OSWorkflow::applyArguments(measure::OSArgumentMap& argumentMap, const std::string& argumentName, const openstudio::Variant& argumentValue) {
  fmt::print("Info: Setting argument value '{}' to '{}'\n", argumentName, argumentValue);

  // if (!argumentValue.hasValue()) {
  //   fmt::print("Warn: Value for argument '{}' not set in argument list therefore will use default\n", argumentName);
  //   return;
  // }
  if (argumentMap.contains(argumentName)) {
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
  pythonEngine->registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");

  // -1: gotta init the rest of OpenStudio in ruby, so that Ruleset and co are defined
  fmt::print("here's a model from Ruby:\n");
  rubyEngine->exec("OpenStudio::init_rest_of_openstudio()");
  rubyEngine->exec("puts OpenStudio::Model::Model.new()");

  fmt::print("here's a model from Python:\n");
  pythonEngine->exec("import openstudio; print(openstudio.model.Model())");

  // O. Need to apply measure steps IN ORDER. (eg: OpenStudio Measures before Eplus measures etc)
  // https://github.com/NREL/OpenStudio-workflow-gem/blob/develop/lib/openstudio/workflow/util/measure.rb

  // 1. Instantiate seed model

  auto runInitialization = [this]() -> openstudio::model::Model {
    fmt::print("Debug: Finding and loading the seed file");
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

  // 2. determine ruby or python
  // 3. import measure.(py|rb)
  // 4. instantiate measure
  // 5. run measure
  const auto modelSteps = workflowJSON.getMeasureSteps(openstudio::MeasureType::ModelMeasure);

  for (const auto& step : modelSteps) {
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
    const MeasureType measureType = bclMeasure.measureType();
    const MeasureLanguage measureLanguage = bclMeasure.measureLanguage();

    // For computing arguments
    auto modelClone = model.clone(true).cast<model::Model>();

    // TODO: will add a Logger later
    fmt::print("Class Name: {}\n", className);
    fmt::print("Measure Script Path: {}\n", openstudio::toString(scriptPath_.get()));
    fmt::print("Measure Type: {}\n", measureType.valueName());
    fmt::print("Measure Language: {}\n", measureLanguage.valueName());

    openstudio::measure::ModelMeasure* modelMeasurePtr = nullptr;
    // TODO: probably want to do that ultimately, then static_cast appropriately
    // openstudio::measure::OSMeasure* measurePtr = nullptr;

    if (measureLanguage == MeasureLanguage::Ruby) {
      // TODO: probably need to do path formatting properly for windows
      auto importCmd = fmt::format("require '{}'", openstudio::toString(scriptPath_.get()));
      fmt::print("{}\n", importCmd);
      rubyEngine->exec(importCmd);
      auto rubyMeasure = rubyEngine->eval(fmt::format("{}.new()", className));
      modelMeasurePtr = rubyEngine->getAs<openstudio::measure::ModelMeasure*>(rubyMeasure);
    } else if (measureLanguage == MeasureLanguage::Python) {
      // place measureDirPath in sys.path; do from measure import MeasureName
      // I think this can't work without a "as xxx" otherwise we'll repeatedly try to import a module named 'measure'
      // pythonEngine->pyimport("measure", openstudio::toString(measureDirPath.get()));
      auto importCmd = fmt::format(R"python(
import sys
sys.path.insert(0, '{0}')
from measure import {1}
print(sys.path)
measure = {1}()
print(measure.name())
)python",
                                   openstudio::toString(scriptPath_->parent_path()), className);
      fmt::print("{}\n", importCmd);
      pythonEngine->exec(importCmd);
      auto pythonMeasure = pythonEngine->eval(fmt::format("{}()", className));
      modelMeasurePtr = pythonEngine->getAs<openstudio::measure::ModelMeasure*>(pythonMeasure);
    }
    if (!modelMeasurePtr) {
      throw std::runtime_error(fmt::format("Could not load measure at '{}'", openstudio::toString(scriptPath_.get())));
    }
    // Initialize arguments which may be model dependent, don't allow arguments method access to real model in case it changes something
    std::vector<measure::OSArgument> arguments;

    fmt::print("Calling modelMeasurePtr->name()= '{}'\n", modelMeasurePtr->name());

    if (measureType == MeasureType::ModelMeasure) {
      arguments = modelMeasurePtr->arguments(modelClone);
      // arguments = static_cast<openstudio::measure::ModelMeasure*>(measurePtr)->arguments(modelClone);
    } else if (measureType == MeasureType::EnergyPlusMeasure) {
    } else if (measureType == MeasureType::ReportingMeasure) {
    }

    measure::OSArgumentMap argumentMap;
    if (!arguments.empty()) {
      for (const auto& argument : arguments) {
        fmt::print("Argument: {}\n", argument.name());
        argumentMap[argument.name()] = argument.clone();
      }

      //  logger.debug "Iterating over arguments for workflow item '#{measure_dir_name}'"
      auto stepArgs = step.arguments();
      // handle skip first
      if (!stepArgs.empty() && stepArgs.contains("__SKIP__")) {

        // TODO: I've copied workflow-gem here, but it's wrong. It's trying to issue a warning if an argument is not set, so it should loop on
        // argumentMap instead...
        for (auto const& [argumentName, argumentValue] : stepArgs) {
          applyArguments(argumentMap, argumentName, argumentValue);
        }
      }
    }
  }
}
}  // namespace openstudio
