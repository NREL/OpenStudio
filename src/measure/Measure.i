#ifndef MEASURE_I
#define MEASURE_I

#ifdef SWIGPYTHON
%module(directors="1") openstudiomeasure
#endif


#define UTILITIES_API
#define MODEL_API
#define STANDARDSINTERFACE_API
#define MEASURE_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%ignore openstudio::detail;
%ignore openstudio::model::detail;
%ignore openstudio::measure::detail;

#if defined(SWIGCSHARP)
  // Help in overload resolution preferring std::string over char const *
  %ignore openstudio::measure::OSArgument::setValue(char const*);
  %ignore openstudio::measure::OSArgument::setDefaultValue(char const*);
#elif defined(SWIGPYTHON)
  // Avoid triggering a SWIG warning: 'print' is a python keyword
  %rename(toString) openstudio::measure::OSArgument::print;
  %rename(toString) openstudio::measure::OSOutput::print;
#endif

%{
  #include <measure/OSArgument.hpp>
  #include <measure/OSOutput.hpp>
  #include <measure/OSRunner.hpp>
  #include <measure/OSMeasure.hpp>
  #include <measure/OSMeasureInfoGetter.hpp>

  #include <measure/EnergyPlusMeasure.hpp>
  #include <measure/ModelMeasure.hpp>
  #include <measure/ReportingMeasure.hpp>

  #include <model/Component.hpp>
  #include <model/ConcreteModelObjects.hpp>

  #include <utilities/core/Path.hpp>
  #include <utilities/bcl/BCLMeasure.hpp>

  using namespace openstudio;
  using namespace openstudio::model;
  using namespace openstudio::measure;
%}


%ignore std::vector<openstudio::measure::OSOutput>::vector(size_type);
%ignore std::vector<openstudio::measure::OSOutput>::resize(size_type);
%template(OSOutputVector) std::vector<openstudio::measure::OSOutput>;
%template(OptionalOSOutput) boost::optional<openstudio::measure::OSOutput>;

%feature("director") OSMeasure;
%feature("director") ModelMeasure;
%feature("director") EnergyPlusMeasure;
%feature("director") ReportingMeasure;
%feature("director") OSRunner;
%feature("director") OSArgument;
%feature("director") MeasureInfoBinding;

//user scripts
%template(OSArgumentVector) std::vector<openstudio::measure::OSArgument>;
%template(OptionalOSArgument) boost::optional<openstudio::measure::OSArgument>;
%template(OSArgumentMap) std::map<std::string, openstudio::measure::OSArgument>;

%include <measure/OSArgument.hpp>
%include <measure/OSOutput.hpp>
%include <measure/OSRunner.hpp>
%include <measure/OSMeasure.hpp>
%include <measure/ModelMeasure.hpp>
%include <measure/EnergyPlusMeasure.hpp>
%include <measure/ReportingMeasure.hpp>

%extend openstudio::measure::OSArgument {
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

%extend openstudio::measure::OSOutput {
  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

#if defined SWIGRUBY
  %feature("director:except") {
    // Look at openstudio::evalString for a possible approach to reporting more info or a callstack.
    // This will at least protect from calls to bad ruby code.
    throw Swig::DirectorMethodException($error);
  }

  %ignore OSMeasureInfoGetter;

  %include <measure/OSMeasureInfoGetter.hpp>

  %init %{
    rb_eval_string("OpenStudio::Measure.instance_eval { def getInfo(measure,model=OpenStudio::Model::OptionalModel.new,workspace=OpenStudio::OptionalWorkspace.new) eval(OpenStudio::Measure::infoExtractorRubyFunction); return infoExtractor(measure,OpenStudio::Model::OptionalModel.new(model),OpenStudio::OptionalWorkspace.new(workspace)); end }");
  %}

#endif

#if defined SWIGPYTHON
  %feature("director:except") {

#if 1
    if ($error != nullptr) {

      PyObject *exc_type = nullptr;
      PyObject *exc_value = nullptr;
      PyObject *exc_tb = nullptr;
      PyErr_Fetch(&exc_type, &exc_value, &exc_tb);
      PyErr_NormalizeException(&exc_type, &exc_value, &exc_tb);
      PyObject *str_exc_value = PyObject_Repr(exc_value); // Now a unicode object
      PyObject *pyStr2 = PyUnicode_AsEncodedString(str_exc_value, "utf-8", "Error ~");
      Py_DECREF(str_exc_value);
      char *strExcValue = PyBytes_AsString(pyStr2); // NOLINT(hicpp-signed-bitwise)
      Py_DECREF(pyStr2);

      std::string err_msg = "In method '$symname': `" + std::string{strExcValue} + "`";

      // See if we can get a full traceback.
      // Calls into python, and does the same as capturing the exception in `e`
      // then `print(traceback.format_exception(e.type, e.value, e.tb))`
      PyObject *pModuleName = PyUnicode_DecodeFSDefault("traceback");
      PyObject *pyth_module = PyImport_Import(pModuleName);
      Py_DECREF(pModuleName);

      if (pyth_module == nullptr) {
          err_msg += "\nCannot find 'traceback' module, this is weird";
          Swig::DirectorMethodException::raise(err_msg.c_str());
      }

      PyObject *pyth_func = PyObject_GetAttrString(pyth_module, "format_exception");
      Py_DECREF(pyth_module); // PyImport_Import returns a new reference, decrement it

      if (pyth_func || PyCallable_Check(pyth_func)) {

          PyObject *pyth_val = PyObject_CallFunction(pyth_func, "OOO", exc_type, exc_value, exc_tb);

          // traceback.format_exception returns a list, so iterate on that
          if (!pyth_val || !PyList_Check(pyth_val)) { // NOLINT(hicpp-signed-bitwise)
              err_msg += "\nIn reportPythonError(), traceback.format_exception did not return a list.";
              Swig::DirectorMethodException::raise(err_msg.c_str());
          }

          unsigned long numVals = PyList_Size(pyth_val);
          if (numVals == 0) {
              err_msg += "\nNo traceback available";
              Swig::DirectorMethodException::raise(err_msg.c_str());
          }

          err_msg += "\nPython traceback follows:\n```";

          for (unsigned long itemNum = 0; itemNum < numVals; itemNum++) {
              PyObject *item = PyList_GetItem(pyth_val, itemNum);
              if (PyUnicode_Check(item)) { // NOLINT(hicpp-signed-bitwise) -- something inside Python code causes warning
                  std::string traceback_line = PyUnicode_AsUTF8(item);
                  if (!traceback_line.empty() && traceback_line[traceback_line.length() - 1] == '\n') {
                      traceback_line.erase(traceback_line.length() - 1);
                  }
                  err_msg += "\n" + traceback_line;
              }
              // PyList_GetItem returns a borrowed reference, do not decrement
          }

          err_msg += "\n```";

          // PyList_Size returns a borrowed reference, do not decrement
          Py_DECREF(pyth_val); // PyObject_CallFunction returns new reference, decrement
      }
      Py_DECREF(pyth_func); // PyObject_GetAttrString returns a new reference, decrement it
      Swig::DirectorMethodException::raise(err_msg.c_str());
    }
#else
    if ($error != NULL) {
      PyObject *exc, *val, *tb;
      PyErr_Fetch(&exc, &val, &tb);
      PyErr_NormalizeException(&exc, &val, &tb);
      std::string err_msg("In method '$symname': ");

      PyObject* exc_str = PyObject_GetAttrString(exc, "__name__");
      err_msg += PyUnicode_AsUTF8(exc_str);
      Py_XDECREF(exc_str);

      if (val != NULL)
      {
        PyObject* val_str = PyObject_Str(val);
        err_msg += ": ";
        err_msg += PyUnicode_AsUTF8(val_str);
        Py_XDECREF(val_str);
      }

      Py_XDECREF(exc);
      Py_XDECREF(val);
      Py_XDECREF(tb);

      Swig::DirectorMethodException::raise(err_msg.c_str());
    }
#endif
  }
#endif

#endif // MEASURE_I
