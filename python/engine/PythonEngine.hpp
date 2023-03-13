#ifndef PYTHONENGINE_included
#define PYTHONENGINE_included

#include <ScriptEngine.hpp>
#include <ScriptEngineAPI.hpp>

#ifndef PyObject_HEAD
struct _object;
using PyObject = _object;
#endif

namespace openstudio {

namespace measure {
class OSMeasure;
}

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

  // Prefer this method, it is faster and less complex than the one where you infer the class name
  virtual measure::OSMeasure* loadMeasureKnownClassName(const openstudio::path& measureScriptPath, std::string_view className) override;

  virtual std::pair<std::string, measure::OSMeasure*> loadMeasureInferClassName(const openstudio::path& measureScriptPath) override;

  virtual void setupPythonPath(const std::vector<openstudio::path>& includeDirs, const openstudio::path& pythonHomeDir) override;

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
