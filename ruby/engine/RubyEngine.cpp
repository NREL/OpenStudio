#include "RubyEngine.hpp"
#include "InitRubyBindings.hpp"
#include <embedded_files.hxx>
#include <rubyengine_export.h>
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

}  // namespace openstudio

extern "C"
{
  RUBYENGINE_EXPORT openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]) {
    return new openstudio::RubyEngine(argc, argv);
  }

  int rb_hasFile(const char* t_filename) {
    // TODO Consider expanding this to use the path which we have artificially defined in embedded_help.rb
    std::string expandedName = std::string(":/ruby/2.7.0/") + std::string(t_filename) + ".rb";
    return embedded_files::hasFile(expandedName);
  }

  int rb_require_embedded(const char* t_filename) {
    std::string require_script = R"(require ')" + std::string(t_filename) + R"(')";
    openstudio::evalString(require_script);
    return 0;
  }
}
