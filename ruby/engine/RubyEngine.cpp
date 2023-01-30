#include "../../src/measure/OSArgument.hpp"
#include "../../src/measure/OSMeasure.hpp"
#include "../../src/measure/ModelMeasure.hpp"
#include "../../src/model/Model.hpp"
#include "RubyEngine.hpp"
#include "InitRubyBindings.hpp"
#include <embedded_files.hxx>
#include <csignal>
#include <stdexcept>
#include <string>
#include <ctime>

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wregister"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

#include <fmt/format.h>

// TODO: We need to revisit this static initialization stuff
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

VALUE initRestOfOpenStudio(...) {
  openstudio::ruby::initExtendedRubyBindings();
  return Qtrue;
};

RubyEngine::RubyEngine(int argc, char* argv[]) : ScriptEngine(argc, argv) {

  ruby_set_argv(argc, argv);
  Init_EmbeddedScripting();

  initRubyEngine();

  openstudio::ruby::initBasicRubyBindings();

  auto rubymodule = rb_define_module("OpenStudio");
  rb_define_module_function(rubymodule, "init_rest_of_openstudio", initRestOfOpenStudio, 0);

  exec("OpenStudio::init_rest_of_openstudio()");

  ruby_set_argv(argc, argv);

  // DLM: this will interpret any strings passed on the command line as UTF-8
  // can we be smarter and detect the correct encoding? use wmain on windows to get utf-16?
  // or we might want to follow ruby and allow '--external-encoding=UTF-8' as an input argument?
  rb_enc_set_default_external(rb_enc_from_encoding(rb_utf8_encoding()));

  registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");
  registerType<openstudio::measure::EnergyPlusMeasure*>("openstudio::measure::EnergyPlusMeasure *");
  registerType<openstudio::measure::ReportingMeasure*>("openstudio::measure::ReportingMeasure *");
  registerType<std::vector<openstudio::measure::OSArgument> *>("openstudio::measure::OSArgumentVector *");
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
  //return static_cast<openstudio::measure::ModelMeasure*>(measurePtr)->arguments(model);  // NOLINT
  std::vector<measure::OSArgument> result;

  return result;
}

void RubyEngine::applyMeasure(model::Model model, measure::OSRunner& runner, const  BCLMeasure& bclMeasure) {
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
  
  openstudio::measure::OSMeasure* measurePtr = nullptr;
  
  auto importCmd = fmt::format("require '{}'", openstudio::toString(scriptPath_.get()));
  exec(importCmd);
  auto measureScriptObject = eval(fmt::format("{}.new()", className));
  
  measure::OSArgumentMap argmap;
  measurePtr = getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);
  static_cast<openstudio::measure::ModelMeasure*>(measurePtr)->run(model);
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
