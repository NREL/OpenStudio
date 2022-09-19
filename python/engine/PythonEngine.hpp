#ifndef PYTHONENGINE_included
#define PYTHONENGINE_included

#include <memory>
#include <ScriptEngine.hpp>

namespace openstudio {

class PythonEngine final : public ScriptEngine
{
 public:
  PythonEngine(int argc, char* argv[]);
  ~PythonEngine() override;

  PythonEngine(const PythonEngine&) = delete;
  PythonEngine(PythonEngine&&) = delete;
  PythonEngine& operator=(const PythonEngine&) = delete;
  PythonEngine& operator=(PythonEngine&&) = delete;

  ScriptObject eval(std::string_view sv) override;
  void exec(std::string_view sv) override;

 protected:

  void* getAs_impl(ScriptObject& obj, const std::type_info&) override;
  void importOpenStudio();

 private:
  wchar_t* program;
};

}  // namespace openstudio

extern "C" {
openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]) {
  return new openstudio::PythonEngine(argc, argv);
}

}

#endif
