/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "PythonEngine.hpp"
#include <utilities/core/ApplicationPathHelpers.hpp>
#include "../../src/utilities/core/Filesystem.hpp"
#include <fmt/format.h>
#include <stdexcept>
#include <string>
#include <iostream>

#ifdef __GNUC__
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include <PythonConfig.hxx>
#include <Python.h>
#include <SWIGPythonRuntime.hxx>

#ifdef __GNUC__
#  pragma GCC diagnostic pop
#endif

namespace openstudio {

void addToPythonPath(const openstudio::path& includePath) {
  if (!includePath.empty()) {
    PyObject* sys = PyImport_ImportModule("sys");
    PyObject* sysPath = PyObject_GetAttrString(sys, "path");
    Py_DECREF(sys);  // PyImport_ImportModule returns a new reference, decrement it

    // fmt::print("Prepending '{}' to sys.path\n", includePath);
    PyObject* unicodeIncludePath = PyUnicode_FromString(includePath.string().c_str());
    PyList_Insert(sysPath, 0, unicodeIncludePath);
    Py_DECREF(sysPath);  // PyObject_GetAttrString returns a new reference, decrement it
  }
}

void PythonEngine::pyimport(const std::string& importName, const std::string& includePath) {
  addToPythonPath(includePath);
  PyImport_ImportModule(importName.c_str());
}

void PythonEngine::setupPythonPath(const std::vector<openstudio::path>& includeDirs) {

  // Iterate in reverse order since addToPythonPath always inserts at pos 0
  // --python_path path1 --python_path path2  =>  includeDirs = ["path1", "path2"]
  // std::ranges::reverse_view needs modern compilers
  for (auto it = includeDirs.rbegin(); it != includeDirs.rend(); it++) {
    addToPythonPath(*it);
  }
}

PythonEngine::PythonEngine(int argc, char* argv[]) : ScriptEngine(argc, argv), program(Py_DecodeLocale(pythonProgramName, nullptr)) {
  // TODO: modernize and use PyConfig (new in 3.8): https://docs.python.org/3/c-api/init_config.html

  // this frozen flag tells Python that the package and library have been frozen for embedding, so it shouldn't warn about missing prefixes
  Py_FrozenFlag = 1;

  // Path to the E+ shipped standard library
  auto pathToPythonPackages = getEnergyPlusDirectory() / "python_standard_lib";

  // The PYTHONPATH / PYTHONHOME should be set before initializing Python
  // If this Py_SetPath is called before Py_Initialize, then Py_GetPath won't attempt to compute a default search path
  // The default search path is affected by the Py_SetPythonHome
  // * if the user passed --python_home, we use that as the Python Home, and do not use Py_SetPath. But later we add the E+ standard_lib anyways
  //   so it takes precedence (to limit incompatibility issues...)
  // * If the user didn't pass it, we use Py_SetPath set to the E+ standard_lib

  std::vector<std::string> args(argv, std::next(argv, static_cast<std::ptrdiff_t>(argc)));
  bool pythonHomePassed = false;
  auto it = std::find(args.cbegin(), args.cend(), "--python_home");
  if (it != args.cend()) {
    openstudio::path pythonHomeDir(*std::next(it));
    wchar_t* h = Py_DecodeLocale(pythonHomeDir.make_preferred().string().c_str(), nullptr);
    Py_SetPythonHome(h);
    pythonHomePassed = true;
  } else {
    wchar_t* a = Py_DecodeLocale(pathToPythonPackages.make_preferred().string().c_str(), nullptr);
    Py_SetPath(a);
  }

  Py_SetProgramName(program);  // optional but recommended

  Py_Initialize();

  if (pythonHomePassed) {
    addToPythonPath(pathToPythonPackages);
  }
#if defined(__APPLE__) || defined(__linux___) || defined(__unix__)
  addToPythonPath(pathToPythonPackages / "lib-dynload");
#endif

  PyObject* m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }
  m_globalDict = PyModule_GetDict(m);

  //PyRun_SimpleString("from time import time,ctime\n"
  //                   "print('Today is', ctime(time()))\n");
  importOpenStudio();
}

PythonEngine::~PythonEngine() {
  if (Py_FinalizeEx() < 0) {
    exit(120);
  }
  PyMem_RawFree(program);
}

void PythonEngine::importOpenStudio() {
  // generic_string() converts to a POSIX path, with forward slashes, so that pyimport doesn't choke on backslashes understood as escape sequence
  if (moduleIsRunningFromBuildDirectory()) {
    const auto bindingsDir = getOpenStudioModuleDirectory();
    pyimport("openstudiodev", bindingsDir.generic_string());
  } else {
    const auto bindingsDir = getOpenStudioModuleDirectory() / "../Python";
    pyimport("openstudio", bindingsDir.generic_string());
  }
  // Somehow that doesn't suffice to register it...
  exec("import openstudio");
}

struct PythonObject
{
  PythonObject() = default;

  PythonObject(PyObject* obj) noexcept : obj_(obj) {
    if (obj_) {
      Py_INCREF(obj_);
    }
  }

  PythonObject(const PythonObject& other) noexcept : obj_(other.obj_) {
    if (obj_) {
      Py_INCREF(obj_);
    }
  }

  PythonObject(PythonObject&& other) noexcept : obj_(other.obj_) {
    // no reason to inc/dec, we just stole the ref counted object
    // from other
    other.obj_ = nullptr;
  }

  PythonObject& operator=(const PythonObject& rhs) noexcept {
    if (&rhs != this) {
      obj_ = rhs.obj_;

      if (obj_) {
        Py_INCREF(obj_);
      }
    }

    return *this;
  }

  PythonObject& operator=(PythonObject&& rhs) noexcept {
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

  PyObject* obj_ = nullptr;
};

void PythonEngine::exec(std::string_view sv) {
  std::string command{sv};

  PyObject* v = PyRun_String(command.c_str(), Py_file_input, m_globalDict, m_globalDict);
  // PyObject* v = PyRun_SimpleString(command.c_str());
  if (v == nullptr) {
    PyErr_Print();
    throw std::runtime_error("Error executing Python code");
  }

  Py_DECREF(v);
}

ScriptObject PythonEngine::eval(std::string_view sv) {
  std::string command{sv};

  PyObject* v = PyRun_String(command.c_str(), Py_eval_input, m_globalDict, m_globalDict);

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

void* PythonEngine::getAs_impl(ScriptObject& obj, const std::type_info& ti) {

  auto val = std::any_cast<PythonObject>(obj.object);

  void* return_value = nullptr;

  if (ti == typeid(std::string*)) {
    // TODO: this sucks, and probably needs a PyDecref or two
    Py_ssize_t size = 0;
    char const* pc = PyUnicode_AsUTF8AndSize(val.obj_, &size);
    if (pc) {
      return_value = new std::string(pc, size);
    } else {
      throw std::runtime_error("Unable to convert to std::string in SWIG Python");
    }
  } else {
    const auto& type_name = getRegisteredTypeName(ti);

    auto* type = SWIG_Python_TypeQuery(type_name.c_str());

    if (!type) {
      throw std::runtime_error("Unable to find type in SWIG");
    }

    const auto result = SWIG_Python_ConvertPtr(val.obj_, &return_value, type, 0);

    if (!SWIG_IsOK(result)) {
      throw std::runtime_error(fmt::format("Error getting object from SWIG/Python of supposed type {}, {}\n", ti.name(), type_name.c_str()));
    }
  }

  return return_value;
}
}  // namespace openstudio

extern "C"
{
  openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]) {
    return new openstudio::PythonEngine(argc, argv);
  }
}
