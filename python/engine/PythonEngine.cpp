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
#include <type_traits>

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

  if (includePath.empty()) {
    return;
  }

  // fmt::print("Prepending '{}' to sys.path\n", includePath);

  PyObject* unicodeIncludePath = nullptr;
  if constexpr (std::is_same_v<typename openstudio::path::value_type, wchar_t>) {
    const std::wstring ws = includePath.generic_wstring();
    unicodeIncludePath = PyUnicode_FromWideChar(ws.c_str(), static_cast<Py_ssize_t>(ws.size()));  // New reference
  } else {
    const std::string s = includePath.generic_string();
    unicodeIncludePath = PyUnicode_FromString(s.c_str());  // New reference
  }

  if (unicodeIncludePath == nullptr) {
    throw std::runtime_error(fmt::format("Unable to convert path '{}' for addition to sys.path in Python", includePath.generic_string()));
  }

  PyObject* sysPath = PySys_GetObject("path");  // Borrowed reference
  int ret = PyList_Insert(sysPath, 0, unicodeIncludePath);
  Py_DECREF(unicodeIncludePath);
  if (ret != 0) {
    throw std::runtime_error(fmt::format("Unable to add path '{}' to the sys.path in Python", includePath.generic_string()));
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
  auto pathToPythonPackages = getEnergyPlusDirectory() / "python_lib";

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
#if defined(__APPLE__)
  // RTLD_LOCAL is import an Apple so that that Python and Ruby do not conflict
  const std::string set_dlflags_cmd = R"(
import sys
import os
pre_os_dl_open_flags = sys.getdlopenflags()
sys.setdlopenflags(os.RTLD_LOCAL)
  )";

  exec(set_dlflags_cmd.c_str());
#endif

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

#if defined(__APPLE__)
  // Reset the dlopen flags to the value prior to importOpenStudio
  const std::string reset_dlflags_cmd = R"(
sys.setdlopenflags(pre_os_dl_open_flags)
  )";

  exec(reset_dlflags_cmd.c_str());
#endif
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

  const auto& type_name = getRegisteredTypeName(ti);

  auto* type = SWIG_Python_TypeQuery(type_name.c_str());

  if (!type) {
    throw std::runtime_error("Unable to find type in SWIG");
  }

  const auto result = SWIG_Python_ConvertPtr(val.obj_, &return_value, type, 0);

  if (!SWIG_IsOK(result)) {
    throw std::runtime_error(fmt::format("Error getting object from SWIG/Python of supposed type {}, {}\n", ti.name(), type_name.c_str()));
  }

  return return_value;
}

bool PythonEngine::getAs_impl_bool(ScriptObject& obj) {
  auto val = std::any_cast<PythonObject>(obj.object);
  if (!PyBool_Check(val.obj_)) {
    throw std::runtime_error("PyObject is not a bool");
  }
  return static_cast<bool>(PyObject_IsTrue(val.obj_));
}

int PythonEngine::getAs_impl_int(ScriptObject& obj) {
  auto val = std::any_cast<PythonObject>(obj.object);
  if (!PyLong_Check(val.obj_)) {
    throw std::runtime_error("PyObject is not a PyLong");
  }

  return static_cast<int>(PyLong_AsLong(val.obj_));
}

double PythonEngine::getAs_impl_double(ScriptObject& obj) {
  auto val = std::any_cast<PythonObject>(obj.object);
  if (!PyFloat_Check(val.obj_)) {
    throw std::runtime_error("PyObject is not a PyFloat");
  }

  return PyFloat_AsDouble(val.obj_);
}

std::string PythonEngine::getAs_impl_string(ScriptObject& obj) {
  auto val = std::any_cast<PythonObject>(obj.object);

  if (!PyUnicode_Check(val.obj_)) {
    throw std::runtime_error("PyObject is not a String");
  }

  Py_ssize_t size = 0;
  char const* pc = PyUnicode_AsUTF8AndSize(val.obj_, &size);  // No decref needed
  if (!pc) {
    throw std::runtime_error("Unable to convert to std::string in SWIG Python");
  }

  return std::string{pc, static_cast<size_t>(size)};
}

std::string PythonEngine::inferMeasureClassName(const openstudio::path& measureScriptPath) {

  auto inferClassNameCmd = fmt::format(R"python(
import importlib.util
import inspect
spec = importlib.util.spec_from_file_location(f"throwaway", "{}")

module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
class_members = inspect.getmembers(module, lambda x: inspect.isclass(x) and issubclass(x, openstudio.measure.OSMeasure))
assert len(class_members) == 1
measure_name, measure_typeinfo = class_members[0]
)python",
                                       measureScriptPath.generic_string());

  std::string className;
  try {
    exec(inferClassNameCmd);
    ScriptObject measureClassNameObject = eval("measure_name");
    className = getAs<std::string>(measureClassNameObject);
  } catch (const std::runtime_error& e) {
    auto msg = fmt::format("Failed to infer measure name from {}: {}", measureScriptPath.generic_string(), e.what());
    fmt::print(stderr, "{}\n", msg);
  }

  return className;
}

// Ideally this would return a openstudio::measure::OSMeasure* or a shared_ptr<openstudio::measure::OSMeasure> but this poses memory management
// issue for the underlying ScriptObject (and VALUE or PyObject), so just return the ScriptObject
ScriptObject PythonEngine::loadMeasure(const openstudio::path& measureScriptPath, std::string_view className) {

  ScriptObject result;

  auto importCmd = fmt::format(R"python(
import importlib.util
spec = importlib.util.spec_from_file_location('{}', r'{}')
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
)python",
                               className, measureScriptPath.generic_string());

  // fmt::print("\nimportCmd:\n{}\n", importCmd);
  try {
    exec(importCmd);
  } catch (const std::runtime_error& e) {
    auto msg = fmt::format("Failed to load measure '{}' from '{}': {}", className, measureScriptPath.generic_string(), e.what());
    fmt::print(stderr, "{}\n", msg);
  }

  try {
    result = eval(fmt::format("module.{}()", className));
  } catch (const std::runtime_error& e) {
    auto msg = fmt::format("Failed to instantiate measure '{}' from '{}': {}", className, measureScriptPath.generic_string(), e.what());
    fmt::print(stderr, "{}\n", msg);
  }

  return result;
}

int PythonEngine::numberOfArguments(ScriptObject& classInstanceObject, std::string_view methodName) {

  int numberOfArguments = -1;

  auto val = std::any_cast<PythonObject>(classInstanceObject.object);
  if (PyObject_HasAttrString(val.obj_, methodName.data()) == 0) {
    // FAILED
    return numberOfArguments;
  }

  PyObject* method = PyObject_GetAttrString(val.obj_, methodName.data());  // New reference
  if (PyMethod_Check(method)) {
    PyObject* func = PyMethod_Function(method);   // Borrowed
    if (auto* code = PyFunction_GetCode(func)) {  // Borrowed
      auto* co = (PyCodeObject*)code;
      numberOfArguments = co->co_argcount - 1;  // This includes `self`
    }
  } else if (PyFunction_Check(method)) {
    // Shouldn't enter this block here
    if (auto code = PyFunction_GetCode(method)) {
      auto* co = (PyCodeObject*)code;
      numberOfArguments = co->co_argcount;
    }
  }
  Py_DECREF(method);
  return numberOfArguments;
}

}  // namespace openstudio

extern "C"
{
  openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]) {
    return new openstudio::PythonEngine(argc, argv);
  }
}
