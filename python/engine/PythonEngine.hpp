#ifndef PYTHONENGINE_included
#define PYTHONENGINE_included

#include <ScriptEngine.hpp>

#ifndef PyObject_HEAD
struct _object;
typedef _object PyObject;
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
  // clang-tidy warns https://clang.llvm.org/extra/clang-tidy/checks/misc/definitions-in-headers.html
  __declspec(dllexport) openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]);
}

#endif
