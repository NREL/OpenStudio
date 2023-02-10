#include "../../src/measure/OSArgument.hpp"
#include "../../src/measure/OSMeasure.hpp"
#include "../../src/measure/ModelMeasure.hpp"
#include "../../src/model/Model.hpp"
#include "../../src/workflow/OSWorkflow.hpp"
#include "RubyEngine.hpp"
#include "RubyEval.hpp"
#include "InitRubyBindings.hpp"
#include "InitRubyEngine.hpp"
#include <SWIGRubyRuntime.hxx>
#include <embedded_files.hxx>
#include <csignal>
#include <stdexcept>
#include <string>
#include <ctime>
#include <fmt/format.h>

static int argc = 0;
static char** argv = nullptr;

unsigned init() {
  ruby_sysinit(&argc, &argv);
  ruby_setup();
  return 0u;
}

static unsigned i = init();

extern "C"
{
  void Init_EmbeddedScripting(void);
}

namespace openstudio {

namespace measure {
  class ModelMeasure;
  class EnergyPlusMeasure;
  class ReportingMeasure;
  class OSArgument;
}

RubyEngine::RubyEngine(int argc, char* argv[]) : ScriptEngine(argc, argv) {
  ruby_set_argv(argc, argv);
  Init_EmbeddedScripting();
  initRubyEngine();
  openstudio::ruby::initBasicRubyBindings();
  openstudio::ruby::initExtendedRubyBindings();

  registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");
  registerType<openstudio::measure::EnergyPlusMeasure*>("openstudio::measure::EnergyPlusMeasure *");
  registerType<openstudio::measure::ReportingMeasure*>("openstudio::measure::ReportingMeasure *");
}

void RubyEngine::setupEmbeddedGems(const std::vector<openstudio::path>& includeDirs, const std::vector<openstudio::path>& gemPathDirs,
                                   const openstudio::path& gemHomeDir, const openstudio::path& bundleGemFilePath,
                                   const openstudio::path& bundleGemDirPath, const std::string& bundleWithoutGroups) {
  openstudio::ruby::setupEmbeddedGems(includeDirs, gemPathDirs, gemHomeDir, bundleGemFilePath, bundleGemDirPath, bundleWithoutGroups);
}

RubyEngine::~RubyEngine() {
  ruby_finalize();
}

ScriptObject RubyEngine::eval(std::string_view sv) {
  std::string str{sv};
  return ScriptObject{openstudio::evalString(str)};
}

void RubyEngine::exec(std::string_view sv) {
  std::string str{sv};
  [[maybe_unused]] const auto result = openstudio::evalString(str);
}

// convert the underlying object to the correct type, then return it as a void *
// so the above template function can provide it back to the caller.
void* RubyEngine::getAs_impl(ScriptObject& obj, const std::type_info& ti) {
  auto val = std::any_cast<VALUE>(obj.object);

  const auto& type_name = getRegisteredTypeName(ti);

  void* return_value = nullptr;

  auto* type = SWIG_TypeQuery(type_name.c_str());

  if (!type) {
    throw std::runtime_error("Unable to find type in SWIG");
  }

  const auto result = SWIG_ConvertPtr(val, &return_value, type, 0);

  if (!SWIG_IsOK(result)) {
    throw std::runtime_error("Error getting object from SWIG/Ruby");
  }

  return return_value;
}

std::vector<measure::OSArgument> RubyEngine::getArguments(openstudio::measure::OSMeasure* measurePtr, const model::Model& model) {
  return static_cast<openstudio::measure::ModelMeasure*>(measurePtr)->arguments(model);  // NOLINT
}

void RubyEngine::applyMeasure(model::Model& model, measure::OSRunner& runner, const BCLMeasure& bclMeasure, const MeasureStep& step) {
  const auto scriptPath_ = bclMeasure.primaryScriptPath();
  if (!scriptPath_) {
    fmt::print("Could not find primaryScript for Measure '{}'\n", openstudio::toString(bclMeasure.directory()));
    return;
  }

  const std::string className = bclMeasure.className();
  const MeasureLanguage measureLanguage = bclMeasure.measureLanguage();
  
  // TODO: will add a Logger later
  fmt::print("Class Name: {}\n", className);
  fmt::print("Measure Script Path: {}\n", openstudio::toString(scriptPath_.get()));
  fmt::print("Measure Type: {}\n", bclMeasure.measureType().valueName());
  fmt::print("Measure Language: {}\n", measureLanguage.valueName());
  
  auto importCmd = fmt::format("require '{}'", openstudio::toString(scriptPath_.get()));
  exec(importCmd);

  auto measureScriptObject = eval(fmt::format("{}.new()", className));
  auto measure = getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);
  auto args = OSWorkflow::argumentMap(measure, model, step);
  measure->run(model, runner, args);
}

}  // namespace openstudio

extern "C"
{
  openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]) {
    return new openstudio::RubyEngine(argc, argv);
  }

  int rb_hasFile(const char* t_filename) {
    // TODO Consider expanding this to use the path which we have artificially defined in embedded_help.rb
    std::string expandedName = fmt::format(":/ruby/2.7.0/{}.rb", t_filename);
    return embedded_files::hasFile(expandedName) ? 1 : 0;
  }

  int rb_require_embedded(const char* t_filename) {
    std::string require_script = fmt::format("require '{}'", t_filename);
    openstudio::evalString(require_script);
    return 0;
  }
}
