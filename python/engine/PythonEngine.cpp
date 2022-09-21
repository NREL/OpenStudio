#include "PythonEngine.hpp"
#include <utilities/core/ApplicationPathHelpers.hpp>
#include <fmt/format.h>
#include <stdexcept>
#include <string>

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include <PythonConfig.hxx>
#include <Python.h>
#include <SWIGPythonRuntime.hxx>

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif


namespace openstudio {

static constexpr auto importScript = R"(
import sys
sys.path.insert(0, '{}')
import {}
)";

PythonEngine::PythonEngine(int argc, char *argv[])
    : ScriptEngine(argc, argv), program(Py_DecodeLocale(pythonProgramName, nullptr)) {
  Py_SetProgramName(program); // optional but recommended

  Py_Initialize();
  importOpenStudio();
}

PythonEngine::~PythonEngine() {
  if (Py_FinalizeEx() < 0) {
    exit(120);
  }
  PyMem_RawFree(program);
}

void PythonEngine::importOpenStudio() {
  if (moduleIsRunningFromBuildDirectory()) {
    const auto bindingsDir = getOpenStudioModuleDirectory();
    exec(fmt::format(importScript, bindingsDir.string(), "openstudiodev"));
  } else {
    const auto bindingsDir = getOpenStudioModuleDirectory() / "../Python";
    exec(fmt::format(importScript, bindingsDir.string(), "openstudio"));
  }
}

struct PythonObject {
  PythonObject() = default;

  PythonObject(PyObject *obj) noexcept : obj_(obj) {
    if (obj_) {
      Py_INCREF(obj_);
    }
  }

  PythonObject(const PythonObject &other) noexcept : obj_(other.obj_) {
    if (obj_) {
      Py_INCREF(obj_);
    }
  }

  PythonObject(PythonObject &&other) noexcept : obj_(other.obj_) {
    // no reason to inc/dec, we just stole the ref counted object
    // from other
    other.obj_ = nullptr;
  }

  PythonObject &operator=(const PythonObject &rhs) noexcept {
    if (&rhs != this) {
      obj_ = rhs.obj_;

      if (obj_) {
        Py_INCREF(obj_);
      }
    }

    return *this;
  }

  PythonObject &operator=(PythonObject &&rhs) noexcept {
    if (&rhs != this) {
      obj_ = rhs.obj_;
      rhs.obj_ = nullptr;
    }

    return *this;
  }

  ~PythonObject() {
    if (obj_) {
      Py_DECREF(obj_);
    }
  }

  PyObject *obj_ = nullptr;
};

void PythonEngine::exec(std::string_view sv) {
  std::string command{sv};

  PyObject *m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }

  PyObject *globalDict = PyModule_GetDict(m);

  PyObject *v = PyRun_String(command.c_str(), Py_file_input, globalDict, globalDict);
  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  Py_DECREF(v);
}

ScriptObject PythonEngine::eval(std::string_view sv) {
  std::string command{sv};

  PyObject *m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }

  PyObject *d = PyModule_GetDict(m);

  PyObject *v = PyRun_String(command.c_str(), Py_eval_input, d, d);
  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  // share in ownership
  PythonObject return_value(v);

  // decref count returned from Python
  Py_DECREF(v);

  return ScriptObject{return_value};
}

void *PythonEngine::getAs_impl(ScriptObject &obj, const std::type_info &ti) {

  auto val = std::any_cast<PythonObject>(obj.object);

  const auto &type_name = getRegisteredTypeName(ti);

  void *return_value = nullptr;

  auto *type = SWIG_Python_TypeQuery(type_name.c_str());

  if (!type) {
    throw std::runtime_error("Unable to find type in SWIG");
  }

  const auto result = SWIG_Python_ConvertPtr(val.obj_, &return_value, type, 0);

  if (!SWIG_IsOK(result)) {
    throw std::runtime_error("Error getting object from SWIG/Python");
  }

  return return_value;
}
} // namespace openstudio
