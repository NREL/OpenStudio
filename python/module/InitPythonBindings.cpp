#include "InitPythonBindings.hpp"
#include <embedded_files.hxx>
#include <fmt/format.h>
#include <iostream>
#include <Python.h>
#include <map>
#include <string>
#include <functional>

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
namespace python {

static const std::map<std::string, std::function<PyObject*(void)> > pythonInitFunctions = {
  {"_openstudioairflow", PyInit__openstudioairflow},
  {"_openstudioenergyplus", PyInit__openstudioenergyplus},
  {"_openstudioepjson", PyInit__openstudioepjson},
  {"_openstudiogbxml", PyInit__openstudiogbxml},
  {"_openstudiogltf", PyInit__openstudiogltf},
  {"_openstudioisomodel", PyInit__openstudioisomodel},
  {"_openstudiomeasure", PyInit__openstudiomeasure},
  {"_openstudiomodelcore", PyInit__openstudiomodelcore},
  {"_openstudiomodelsimulation", PyInit__openstudiomodelsimulation},
  {"_openstudiomodelresources", PyInit__openstudiomodelresources},
  {"_openstudiomodelgeometry", PyInit__openstudiomodelgeometry},
  {"_openstudiomodelhvac", PyInit__openstudiomodelhvac},
  {"_openstudiomodelzonehvac", PyInit__openstudiomodelzonehvac},
  {"_openstudiomodelavailabilitymanager", PyInit__openstudiomodelavailabilitymanager},
  {"_openstudiomodelplantequipmentoperationscheme", PyInit__openstudiomodelplantequipmentoperationscheme},
  {"_openstudiomodelstraightcomponent", PyInit__openstudiomodelstraightcomponent},
  {"_openstudiomodelairflow", PyInit__openstudiomodelairflow},
  {"_openstudiomodelrefrigeration", PyInit__openstudiomodelrefrigeration},
  {"_openstudiomodelgenerators", PyInit__openstudiomodelgenerators},
  {"_openstudiomodel", PyInit__openstudiomodel},
  {"_openstudioosversion", PyInit__openstudioosversion},
  {"_openstudioradiance", PyInit__openstudioradiance},
  {"_openstudiosdd", PyInit__openstudiosdd},
  {"_openstudioutilitiesbcl", PyInit__openstudioutilitiesbcl},
  {"_openstudioutilitiescore", PyInit__openstudioutilitiescore},
  {"_openstudioutilitiesdata", PyInit__openstudioutilitiesdata},
  {"_openstudioutilitiesfiletypes", PyInit__openstudioutilitiesfiletypes},
  {"_openstudioutilitiesgeometry", PyInit__openstudioutilitiesgeometry},
  {"_openstudioutilitiesidd", PyInit__openstudioutilitiesidd},
  {"_openstudioutilitiesidf", PyInit__openstudioutilitiesidf},
  {"_openstudioutilitiesplot", PyInit__openstudioutilitiesplot},
  {"_openstudioutilities", PyInit__openstudioutilities},
  {"_openstudioutilitiessql", PyInit__openstudioutilitiessql},
  {"_openstudioutilitiestime", PyInit__openstudioutilitiestime},
  {"_openstudioutilitiesunits", PyInit__openstudioutilitiesunits},
  {"_openstudioutilitiesxml", PyInit__openstudioutilitiesxml}
};

PyObject * initPythonBindings() {
    static struct PyModuleDef openstudioModuleDef = {
      PyModuleDef_HEAD_INIT,
      "openstudio",
      NULL,
      -1,
      NULL,
      NULL,
      NULL,
      NULL,
      NULL
    };

    const auto openstudioModule = PyModule_Create(&openstudioModuleDef);
    [[maybe_unused]] const auto openstudioDict = PyModule_GetDict(openstudioModule);
    const auto sysModules = PyImport_GetModuleDict();

    for (const auto& initPair: pythonInitFunctions) {
      const auto moduleName = initPair.first.c_str();
      const auto initModule = initPair.second;
      PyImport_AddModule(moduleName);
      const auto pyModule = initModule();
      PyDict_SetItemString(sysModules, moduleName, pyModule);
      //Py_DECREF(pyModule);
    }

    auto loadEmbeddedModule = [](const std::string_view name) {
      const auto src = embedded_files::getFileAsString(std::string(fmt::format(":/python/openstudio/{}.py", name)));
      const auto py = Py_CompileString(src.c_str(), std::string(fmt::format("{}.py", name)).c_str(), Py_file_input);
      return PyImport_ExecCodeModule(std::string(name).c_str(), py);
      //Py_DECREF(py);
    };

    auto loadEmbeddedModuleAS = [&loadEmbeddedModule, &openstudioModule](const std::string_view moduleName, const std::string_view asName) {
      const auto m = loadEmbeddedModule(moduleName);
      PyModule_AddObject(openstudioModule, std::string(asName).c_str(), m);
      //Py_DECREF(m);
    };

    const auto utilityModuleNames = {
      "openstudioutilitiescore",
      "openstudioutilitiestime",
      "openstudioutilitiesunits",
      "openstudioutilitiesdata",
      "openstudioutilitiesplot",
      "openstudioutilitiesgeometry",
      "openstudioutilitiessql",
      "openstudioutilitiesxml",
      "openstudioutilitiesbcl",
      "openstudioutilitiesidd",
      "openstudioutilitiesidf",
      "openstudioutilitiesfiletypes",
      "openstudioutilities"
    };

    for (const auto& moduleName: utilityModuleNames) {
      [[maybe_unused]] const auto m = loadEmbeddedModule(moduleName);
      [[maybe_unused]] const auto obj = PyRun_String(fmt::format("from {} import *", moduleName).c_str(), Py_file_input, openstudioDict, openstudioDict);
      //Py_DECREF(obj);
    }

    const auto modelModuleNames = {
      "openstudiomodelcore",
      "openstudiomodelsimulation",
      "openstudiomodelresources",
      "openstudiomodelgeometry",
      "openstudiomodelhvac",
      "openstudiomodelzonehvac",
      "openstudiomodelavailabilitymanager",
      "openstudiomodelplantequipmentoperationscheme",
      "openstudiomodelstraightcomponent",
      "openstudiomodelairflow",
      "openstudiomodelrefrigeration",
      "openstudiomodelgenerators"
    };

    for (const auto& moduleName: modelModuleNames) {
      loadEmbeddedModule(moduleName);
    }

    loadEmbeddedModuleAS("openstudioairflow", "airflow");
    loadEmbeddedModuleAS("openstudioenergyplus", "energyplus");
    loadEmbeddedModuleAS("openstudioepjson", "epjson");
    loadEmbeddedModuleAS("openstudiogbxml", "gbxml");
    loadEmbeddedModuleAS("openstudiogltf","gltf");
    loadEmbeddedModuleAS("openstudioisomodel", "isomodel");
    loadEmbeddedModuleAS("openstudiomeasure", "measure");
    loadEmbeddedModuleAS("openstudioosversion", "osversion");
    loadEmbeddedModuleAS("openstudioradiance", "radiance");
    loadEmbeddedModuleAS("openstudiosdd", "sdd");

    loadEmbeddedModuleAS("openstudiomodel", "model");

    return openstudioModule;
}

}  // namespace python
}  // namespace openstudio
