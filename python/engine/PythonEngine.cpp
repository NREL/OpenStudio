#include "PythonEngine.hpp"
#include <utilities/core/ApplicationPathHelpers.hpp>
#include "../../src/utilities/core/Filesystem.hpp"
#include "../../src/measure/OSArgument.hpp"
#include "../../src/measure/OSMeasure.hpp"
#include "../../src/measure/ModelMeasure.hpp"
#include "../../src/model/Model.hpp"
#include "../../src/workflow/OSWorkflow.hpp"
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

extern "C" {

PyObject* PyInit__openstudioairflow(void);
PyObject* PyInit__openstudioenergyplus(void);
PyObject* PyInit__openstudioepjson(void);
PyObject* PyInit__openstudiogbxml(void);
PyObject* PyInit__openstudiogltf(void);
PyObject* PyInit__openstudioisomodel(void);
PyObject* PyInit__openstudiomeasure(void);
PyObject* PyInit__openstudiomodelairflow(void);
PyObject* PyInit__openstudiomodelavailabilitymanager(void);
PyObject* PyInit__openstudiomodelcore(void);
PyObject* PyInit__openstudiomodelgenerators(void);
PyObject* PyInit__openstudiomodelgeometry(void);
PyObject* PyInit__openstudiomodelhvac(void);
PyObject* PyInit__openstudiomodelplantequipmentoperationscheme(void);
PyObject* PyInit__openstudiomodel(void);
PyObject* PyInit__openstudiomodelrefrigeration(void);
PyObject* PyInit__openstudiomodelresources(void);
PyObject* PyInit__openstudiomodelsimulation(void);
PyObject* PyInit__openstudiomodelstraightcomponent(void);
PyObject* PyInit__openstudiomodelzonehvac(void);
PyObject* PyInit__openstudioosversion(void);
//PyObject* PyInit__openstudio(void);
PyObject* PyInit__openstudioradiance(void);
PyObject* PyInit__openstudiosdd(void);
PyObject* PyInit__openstudioutilitiesbcl(void);
PyObject* PyInit__openstudioutilitiescore(void);
PyObject* PyInit__openstudioutilitiesdata(void);
PyObject* PyInit__openstudioutilitiesfiletypes(void);
PyObject* PyInit__openstudioutilitiesgeometry(void);
PyObject* PyInit__openstudioutilitiesidd(void);
PyObject* PyInit__openstudioutilitiesidf(void);
PyObject* PyInit__openstudioutilitiesplot(void);
PyObject* PyInit__openstudioutilities(void);
PyObject* PyInit__openstudioutilitiessql(void);
PyObject* PyInit__openstudioutilitiestime(void);
PyObject* PyInit__openstudioutilitiesunits(void);
PyObject* PyInit__openstudioutilitiesxml(void);

}

namespace openstudio {

namespace measure {
  class ModelMeasure;
  class EnergyPlusMeasure;
  class ReportingMeasure;
}

void addToPythonPath(const openstudio::path& includePath) {
  if (!includePath.empty()) {
    PyObject* sys = PyImport_ImportModule("sys");
    PyObject* sysPath = PyObject_GetAttrString(sys, "path");
    // fmt::print("Prepending '{}' to sys.path\n", includePath);
    PyObject* unicodeIncludePath = PyUnicode_FromString(includePath.string().c_str());
    PyList_Insert(sysPath, 0, unicodeIncludePath);
  }
}

void PythonEngine::pyimport(const std::string& importName, const std::string& includePath) {
  addToPythonPath(includePath);
  PyImport_ImportModule(importName.c_str());
}

void PythonEngine::setupPythonPath(const std::vector<openstudio::path>& includeDirs, const openstudio::path& pythonHomeDir) {
  for (const auto& includeDir : includeDirs) {
    addToPythonPath(includeDir);
  }
  if (!pythonHomeDir.empty()) {
    wchar_t* a = Py_DecodeLocale(pythonHomeDir.generic_string().c_str(), nullptr);
    Py_SetPythonHome(a);
  }
}

PythonEngine::PythonEngine(int argc, char* argv[]) : ScriptEngine(argc, argv), program(Py_DecodeLocale(pythonProgramName, nullptr)) {

  PyImport_AppendInittab("_openstudioairflow", PyInit__openstudioairflow);
  PyImport_AppendInittab("_openstudioenergyplus", PyInit__openstudioenergyplus);
  PyImport_AppendInittab("_openstudioepjson", PyInit__openstudioepjson);
  PyImport_AppendInittab("_openstudiogbxml", PyInit__openstudiogbxml);
  PyImport_AppendInittab("_openstudiogltf", PyInit__openstudiogltf);
  PyImport_AppendInittab("_openstudioisomodel", PyInit__openstudioisomodel);
  PyImport_AppendInittab("_openstudiomeasure", PyInit__openstudiomeasure);
  PyImport_AppendInittab("_openstudiomodelairflow", PyInit__openstudiomodelairflow);
  PyImport_AppendInittab("_openstudiomodelavailabilitymanager", PyInit__openstudiomodelavailabilitymanager);
  PyImport_AppendInittab("_openstudiomodelcore", PyInit__openstudiomodelcore);
  PyImport_AppendInittab("_openstudiomodelgenerators", PyInit__openstudiomodelgenerators);
  PyImport_AppendInittab("_openstudiomodelgeometry", PyInit__openstudiomodelgeometry);
  PyImport_AppendInittab("_openstudiomodelhvac", PyInit__openstudiomodelhvac);
  PyImport_AppendInittab("_openstudiomodelplantequipmentoperationscheme", PyInit__openstudiomodelplantequipmentoperationscheme);
  PyImport_AppendInittab("_openstudiomodel", PyInit__openstudiomodel);
  PyImport_AppendInittab("_openstudiomodelrefrigeration", PyInit__openstudiomodelrefrigeration);
  PyImport_AppendInittab("_openstudiomodelresources", PyInit__openstudiomodelresources);
  PyImport_AppendInittab("_openstudiomodelsimulation", PyInit__openstudiomodelsimulation);
  PyImport_AppendInittab("_openstudiomodelstraightcomponent", PyInit__openstudiomodelstraightcomponent);
  PyImport_AppendInittab("_openstudiomodelzonehvac", PyInit__openstudiomodelzonehvac);
  PyImport_AppendInittab("_openstudioosversion", PyInit__openstudioosversion);
//PyImport_AppendInittab("_openstudio", PyInit__openstudio);
  PyImport_AppendInittab("_openstudioradiance", PyInit__openstudioradiance);
  PyImport_AppendInittab("_openstudiosdd", PyInit__openstudiosdd);
  PyImport_AppendInittab("_openstudioutilitiesbcl", PyInit__openstudioutilitiesbcl);
  PyImport_AppendInittab("_openstudioutilitiescore", PyInit__openstudioutilitiescore);
  PyImport_AppendInittab("_openstudioutilitiesdata", PyInit__openstudioutilitiesdata);
  PyImport_AppendInittab("_openstudioutilitiesfiletypes", PyInit__openstudioutilitiesfiletypes);
  PyImport_AppendInittab("_openstudioutilitiesgeometry", PyInit__openstudioutilitiesgeometry);
  PyImport_AppendInittab("_openstudioutilitiesidd", PyInit__openstudioutilitiesidd);
  PyImport_AppendInittab("_openstudioutilitiesidf", PyInit__openstudioutilitiesidf);
  PyImport_AppendInittab("_openstudioutilitiesplot", PyInit__openstudioutilitiesplot);
  PyImport_AppendInittab("_openstudioutilities", PyInit__openstudioutilities);
  PyImport_AppendInittab("_openstudioutilitiessql", PyInit__openstudioutilitiessql);
  PyImport_AppendInittab("_openstudioutilitiestime", PyInit__openstudioutilitiestime);
  PyImport_AppendInittab("_openstudioutilitiesunits", PyInit__openstudioutilitiesunits);
  PyImport_AppendInittab("_openstudioutilitiesxml", PyInit__openstudioutilitiesxml);

  // this frozen flag tells Python that the package and library have been frozen for embedding, so it shouldn't warn about missing prefixes
  Py_FrozenFlag = 1;

  // Set the PYTHONPATH / PYTHONHOME to the E+ shipped standard library
  // I think we need to set the python path before initializing the library
  auto pathToPythonPackages = getEnergyPlusDirectory() / "python_standard_lib";
  wchar_t* a = Py_DecodeLocale(pathToPythonPackages.make_preferred().string().c_str(), nullptr);
  Py_SetPath(a);
  Py_SetPythonHome(a);

  Py_SetProgramName(program);  // optional but recommended

  Py_Initialize();
  PyObject* m = PyImport_AddModule("__main__");
  if (m == nullptr) {
    throw std::runtime_error("Unable to add module __main__ for python script execution");
  }
  m_globalDict = PyModule_GetDict(m);

  importOpenStudio();

  registerType<openstudio::measure::ModelMeasure*>("openstudio::measure::ModelMeasure *");
  registerType<openstudio::measure::EnergyPlusMeasure*>("openstudio::measure::EnergyPlusMeasure *");
  registerType<openstudio::measure::ReportingMeasure*>("openstudio::measure::ReportingMeasure *");
  registerType<std::vector<openstudio::measure::OSArgument> *>("openstudio::measure::OSArgumentVector *");
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

  const auto& type_name = getRegisteredTypeName(ti);

  void* return_value = nullptr;

  auto* type = SWIG_Python_TypeQuery(type_name.c_str());

  if (!type) {
    throw std::runtime_error("Unable to find type in SWIG");
  }

  const auto result = SWIG_Python_ConvertPtr(val.obj_, &return_value, type, 0);

  if (!SWIG_IsOK(result)) {
    throw std::runtime_error("Error getting object from SWIG/Python");
  }

  return return_value;
}

std::vector<measure::OSArgument> PythonEngine::getArguments(openstudio::measure::OSMeasure*, const model::Model&) {
  std::vector<measure::OSArgument> result;

  return result;
}

void PythonEngine::applyMeasure(model::Model& model, measure::OSRunner& runner, const BCLMeasure& bclMeasure, const MeasureStep& step) {
  std::cout << "PythonEngine::applyMeasure" << std::endl;

  const auto scriptPath_ = bclMeasure.primaryScriptPath();
  if (!scriptPath_) {
    fmt::print("Could not find primaryScript for Measure '{}'\n", openstudio::toString(bclMeasure.directory()));
    return;
  }

  const std::string className = bclMeasure.className();
  const MeasureLanguage measureLanguage = bclMeasure.measureLanguage();
  
  // TODO: will add a Logger later
  fmt::print("Class Name: {}\n", className);
  fmt::print("Measure Script Path: {}\n", openstudio::toString(scriptPath_.get()));
  fmt::print("Measure Type: {}\n", bclMeasure.measureType().valueName());
  fmt::print("Measure Language: {}\n", measureLanguage.valueName());
  
  auto importCmd = fmt::format(R"python(
import importlib.util
spec = importlib.util.spec_from_file_location('{}', r'{}')
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
)python", className, scriptPath_->generic_string());

  exec(importCmd);

  auto measureScriptObject = eval(fmt::format("module.{}()", className));
  auto measure = getAs<openstudio::measure::ModelMeasure*>(measureScriptObject);
  auto args = OSWorkflow::argumentMap(measure, model, step);
  measure->run(model, runner, args);
}

}  // namespace openstudio

extern "C"
{
  openstudio::ScriptEngine* makeScriptEngine(int argc, char* argv[]) {
    return new openstudio::PythonEngine(argc, argv);
  }
}
