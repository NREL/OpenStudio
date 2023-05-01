#include "InitPythonBindings.hpp"
#include "PythonAPI.hpp"

extern "C" {
  PYTHON_API PyObject * PyInit_openstudio(void) {
    return openstudio::python::initPythonBindings();
  }
}
