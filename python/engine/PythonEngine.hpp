#ifndef PYTHONENGINE_included
#define PYTHONENGINE_included

#include <ScriptEngine.hpp>
#include <ScriptEngineAPI.hpp>

#ifndef PyObject_HEAD
struct _object;
using PyObject = _object;
#endif

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

  virtual void setupPythonPath(const std::vector<openstudio::path>& includeDirs, const openstudio::path& pythonHomeDir) override;

  virtual std::vector<measure::OSArgument> getArguments(openstudio::measure::OSMeasure*, const model::Model&) override;
  virtual void applyMeasure(model::Model&, measure::OSRunner&, const BCLMeasure&, const MeasureStep&) override;

 protected:
  void* getAs_impl(ScriptObject& obj, const std::type_info&) override;
  void importOpenStudio();
  void pyimport(const std::string& importName, const std::string& includePath);

 private:
  wchar_t* program;
  PyObject* m_globalDict;
};

}  // namespace openstudio

extern "C"
{
  SCRIPTENGINE_API openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]);
}

#endif
