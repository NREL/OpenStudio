/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "RubyEngine.hpp"
#include "../bindings/InitRubyBindings.hpp"
#include "../interpreter/RubyException.hpp"
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

  ruby_set_argv(argc, argv);

  // DLM: this will interpret any strings passed on the command line as UTF-8
  // can we be smarter and detect the correct encoding? use wmain on windows to get utf-16?
  // or we might want to follow ruby and allow '--external-encoding=UTF-8' as an input argument?
  rb_enc_set_default_external(rb_enc_from_encoding(rb_utf8_encoding()));
}

void RubyEngine::setupEmbeddedGems(const std::vector<openstudio::path>& includeDirs, const std::vector<openstudio::path>& gemPathDirs,
                                   const openstudio::path& gemHomeDir, const openstudio::path& bundleGemFilePath,
                                   const openstudio::path& bundleGemDirPath, const std::string& bundleWithoutGroups) {
  openstudio::ruby::setupEmbeddedGems(includeDirs, gemPathDirs, gemHomeDir, bundleGemFilePath, bundleGemDirPath, bundleWithoutGroups);
}

RubyEngine::~RubyEngine() {
  // ruby_cleanup calls ruby_finalize
  int ex = ruby_cleanup(0);
  if (ex != 0) {
    // fmt::print("RubyEngine return code was {}\n", ex);
    exit(ex);
  }
  //ruby_finalize();
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

  void* return_value = nullptr;

  const auto& type_name = getRegisteredTypeName(ti);

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

bool RubyEngine::getAs_impl_bool(ScriptObject& obj) {
  auto val = std::any_cast<VALUE>(obj.object);
  return RB_TEST(val);
}

int RubyEngine::getAs_impl_int(ScriptObject& obj) {
  auto val = std::any_cast<VALUE>(obj.object);
  if (!RB_FIXNUM_P(val)) {
    throw std::runtime_error("VALUE is not a FIXNUM");
  }

  return RB_NUM2INT(val);
}
double RubyEngine::getAs_impl_double(ScriptObject& obj) {
  auto val = std::any_cast<VALUE>(obj.object);
  if (!RB_FLOAT_TYPE_P(val)) {
    throw std::runtime_error("VALUE is not a FLOAT");
  }

  return RB_NUM2INT(val);
}

std::string RubyEngine::getAs_impl_string(ScriptObject& obj) {
  auto val = std::any_cast<VALUE>(obj.object);

  if (!RB_TYPE_P(val, RUBY_T_STRING)) {
    throw std::runtime_error("VALUE is not a String");
  }

  char* cstr = StringValuePtr(val);
  size_t size = RSTRING_LEN(val);  // + 1;  From trial and eror, if I had + 1 I get a string with one two many size
  return std::string{cstr, size};
}

std::string RubyEngine::inferMeasureClassName(const openstudio::path& measureScriptPath) {

  auto inferClassNameCmd = fmt::format(R"ruby(
ObjectSpace.garbage_collect
ObjectSpace.garbage_collect
# Measure should be at root level (not inside a module) so we can just get constants
measurePath = '{}'
prev = Object.constants
# puts "prev = #{{prev}}"
load measurePath # need load in case have seen this script before
just_defined = Object.constants - prev
# puts "just_defined = #{{just_defined}}"
just_defined.select!{{|c| Object.const_get(c).ancestors.include?(OpenStudio::Measure::OSMeasure)}}
# puts "just_defined, filtered = #{{just_defined}}"

if just_defined.empty?
  raise "Unable to extract OpenStudio::Measure::OSMeasure object from " +
       measurePath + ". The script should contain a class that derives " +
      "from OpenStudio::Measure::OSMeasure and should close with a line stating " +
      "the class name followed by .new.registerWithApplication."
end
if just_defined.size > 1
  raise "Found more than one OSMeasure at #{{measurePath}}: #{{just_defined}}"
end
c = just_defined[0]
class_info = Object.const_get(c)
$measure_name = class_info.to_s

# Undef what we loaded
just_defined.each {{|x| Object.send(:remove_const, x) }}
ObjectSpace.garbage_collect
ObjectSpace.garbage_collect
)ruby",
                                       measureScriptPath.generic_string());

  std::string className;

  try {
    exec(inferClassNameCmd);
    ScriptObject measureClassNameObject = eval("$measure_name");
    // measureClassNameObject = rubyEngine->eval(fmt::format("{}.new()", className));
    // ScriptObject measureClassNameObject = rubyEngine->eval(inferClassName);
    className = getAs<std::string>(measureClassNameObject);
  } catch (const RubyException& e) {
    auto msg = fmt::format("Failed to infer measure name from {}: {}\nlocation={}", measureScriptPath.generic_string(), e.what(), e.location());
    fmt::print(stderr, "{}\n", msg);
  }

  return className;
}

// Ideally this would return a openstudio::measure::OSMeasure* or a shared_ptr<openstudio::measure::OSMeasure> but this poses memory management
// issue for the underlying ScriptObject (and VALUE or PyObject), so just return the ScriptObject
ScriptObject RubyEngine::loadMeasure(const openstudio::path& measureScriptPath, std::string_view className) {

  ScriptObject result;
  try {
    auto importCmd = fmt::format("load '{}'", measureScriptPath.generic_string());
    exec(importCmd);
  } catch (const RubyException& e) {
    auto msg =
      fmt::format("Failed to load measure '{}' from '{}': {}\nlocation={}", className, measureScriptPath.generic_string(), e.what(), e.location());
    fmt::print(stderr, "{}\n", msg);
  }
  try {
    result = eval(fmt::format("{}.new()", className));
  } catch (const RubyException& e) {
    auto msg = fmt::format("Failed to instantiate measure '{}' from '{}': {}\nlocation={}", className, measureScriptPath.generic_string(), e.what(),
                           e.location());
    fmt::print(stderr, "{}\n", msg);
  }
  return result;
}

int RubyEngine::numberOfArguments(ScriptObject& methodObject, std::string_view methodName) {
  auto val = std::any_cast<VALUE>(methodObject.object);
  ID method_id = rb_intern(methodName.data());
  // Ruby IS SO WEIRD. This will return n for methods that take a fixed number of arguments, and -n - 1 where n is the number of required arguments...
  // def f() => 0
  // def f(a) => 1
  // def f(a = nil) => -1! Because WHY NOT, right?
  // def f(a, c = nil) => -2
  // def f(a, b = nil, c = nil) => also -2
  return rb_obj_method_arity(val, method_id);
}

}  // namespace openstudio

extern "C"
{
  openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]) {
    return new openstudio::RubyEngine(argc, argv);
  }

  int rb_hasFile(const char* t_filename) {
    // TODO Consider expanding this to use the path which we have artificially defined in embedded_help.rb
    std::string expandedName = fmt::format(":/ruby/3.2.0/{}.rb", t_filename);
    return embedded_files::hasFile(expandedName) ? 1 : 0;
  }

  int rb_require_embedded(const char* t_filename) {
    std::string require_script = fmt::format("require '{}'", t_filename);
    openstudio::evalString(require_script);
    return 0;
  }
}
