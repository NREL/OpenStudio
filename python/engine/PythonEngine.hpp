/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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

  virtual void setupPythonPath(const std::vector<openstudio::path>& includeDirs) override;

  virtual std::string inferMeasureClassName(const openstudio::path& measureScriptPath) override;

  virtual ScriptObject loadMeasure(const openstudio::path& measureScriptPath, std::string_view className) override;

  virtual int numberOfArguments(ScriptObject& methodObject, std::string_view methodName) override;

 protected:
  void* getAs_impl(ScriptObject& obj, const std::type_info&) override;

  bool getAs_impl_bool(ScriptObject& obj) override;
  int getAs_impl_int(ScriptObject& obj) override;
  double getAs_impl_double(ScriptObject& obj) override;
  std::string getAs_impl_string(ScriptObject& obj) override;

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
