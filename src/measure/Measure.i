#ifndef MEASURE_I
#define MEASURE_I

#ifdef SWIGPYTHON
%module(directors="1") openstudiomeasure
#endif


%include <utilities/UtilitiesAPI.hpp>
#define MODEL_API
#define STANDARDSINTERFACE_API
#define MEASURE_API
#define ALFALFA_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>
%import <alfalfa/Alfalfa.i>

%ignore openstudio::detail;
%ignore openstudio::model::detail;
%ignore openstudio::measure::detail;

#if defined(SWIGCSHARP)
  // Help in overload resolution preferring std::string over char const *
  %ignore openstudio::measure::OSArgument::setValue(char const*);
  %ignore openstudio::measure::OSArgument::setDefaultValue(char const*);

  // Ignore Json::Value return type (toJSON / fromJSON / valueAsJSON are globally ignored already)
  %ignore openstudio::measure::OSArgument::defaultValueAsJSON;
  %ignore openstudio::measure::OSArgument::domainAsJSON;
  %ignore openstudio::measure::OSRunner::getArgumentValues;
  %ignore openstudio::measure::OSRunner::getPastStepValuesForMeasure;
  %ignore openstudio::measure::OSRunner::getPastStepValuesForName;


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
    // Mimic openstudio::evalString for a possible approach to reporting more info or a callstack.
    VALUE errinfo = rb_errinfo();
    VALUE exception_class = rb_obj_class(errinfo);
    VALUE classNameValue = rb_class_name(exception_class);
    std::string className(StringValuePtr(classNameValue));

    VALUE errstr = rb_obj_as_string(errinfo);
    std::string errMessage(StringValuePtr(errstr));

    std::string totalErr = className + ": " + errMessage;

    // Generally speaking, the backtrace is there, but not for the case where it's a stack too deep error
    std::string loc;
    const ID ID_backtrace = rb_intern_const("backtrace");
    if (exception_class != rb_eSysStackError && rb_respond_to(errinfo, ID_backtrace)) {
      /*volatile*/ VALUE backtrace;
      if (!NIL_P(backtrace = rb_funcall(errinfo, ID_backtrace, 0))) {
        VALUE backtracejoin = rb_ary_join(backtrace, rb_str_new2("\n"));
        const std::string btlines = StringValuePtr(backtracejoin);
        if (!btlines.empty()) {
          loc += "\nTraceback (most recent call last):\n" + btlines;
        }
      }
    }
    if (loc.empty()) {
      // In case we couldn't produce the backtrace (SystemStackError: stack level too deep), fall back on this:
      // I just **cannot** figure out a way to get the error location in C, without calling $@.to_s. Seems nothing is available here
      // $@ is an array, seems to be already ordered from most recent to older
      int locationError;
      VALUE btArray = rb_eval_string_protect("$@", &locationError); // "$@.reverse"
      if (locationError == 0) {
        // Get the backing C array of the ruby array
        VALUE* elements = RARRAY_PTR(btArray);
        long arrayLen = RARRAY_LEN(btArray);
        const long back_trace_limit = 20;
        loc += "\nTraceback (most recent call last):";

        if (arrayLen > back_trace_limit) {
          long omit = arrayLen - back_trace_limit;

          const long initial_back_trace_limit_when_too_long = 8;
          for (long c = arrayLen - 1 ; c > arrayLen - initial_back_trace_limit_when_too_long; --c) {
            VALUE entry = elements[c];
            std::string backtrace_line = RSTRING_PTR(entry);
            loc += "\n" + std::string(backtrace_line);
          }
          loc += "\n\t... " + std::to_string(omit) + " levels...";

          arrayLen = back_trace_limit - initial_back_trace_limit_when_too_long;
        }
        for (long c = 0; c < arrayLen; ++c) {
          VALUE entry = elements[c];
          std::string backtrace_line = RSTRING_PTR(entry);
          loc += "\n" + std::string(backtrace_line);
        }

      } else {
        loc = "Failed to get VM location";
      }
    }

    totalErr += "\n" + loc;
    throw Swig::DirectorMethodException(totalErr.c_str());
  }

  %ignore OSMeasureInfoGetter;

  %include <measure/OSMeasureInfoGetter.hpp>

  %init %{
    rb_eval_string("OpenStudio::Measure.instance_eval { def getInfo(measure,model=OpenStudio::Model::OptionalModel.new,workspace=OpenStudio::OptionalWorkspace.new) eval(OpenStudio::Measure::infoExtractorRubyFunction); return infoExtractor(measure,OpenStudio::Model::OptionalModel.new(model),OpenStudio::OptionalWorkspace.new(workspace)); end }");
  %}

#endif

#if defined SWIGPYTHON
  %feature("director:except") {

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
          err_msg += "\nCannot find 'traceback' module, this should not happen";
          throw Swig::DirectorMethodException(err_msg.c_str());
      }

      PyObject *pyth_func = PyObject_GetAttrString(pyth_module, "format_exception");
      Py_DECREF(pyth_module); // PyImport_Import returns a new reference, decrement it

      if (pyth_func || PyCallable_Check(pyth_func)) {

          PyObject *pyth_val = PyObject_CallFunction(pyth_func, "OOO", exc_type, exc_value, exc_tb);

          // traceback.format_exception returns a list, so iterate on that
          if (!pyth_val || !PyList_Check(pyth_val)) { // NOLINT(hicpp-signed-bitwise)
              err_msg += "\ntraceback.format_exception did not return a list.";
          } else {
            Py_ssize_t numVals = PyList_Size(pyth_val);
            if (numVals == 0) {
                err_msg += "\nNo traceback available";
            } else {
              // err_msg += "\nPython traceback follows:\n```";
              err_msg += '\n';
              for (Py_ssize_t itemNum = 0; itemNum < numVals; itemNum++) {
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

              // err_msg += "\n```";
            }
          }

          // PyList_Size returns a borrowed reference, do not decrement
          Py_DECREF(pyth_val); // PyObject_CallFunction returns new reference, decrement
      }
      Py_DECREF(pyth_func); // PyObject_GetAttrString returns a new reference, decrement it
      throw Swig::DirectorMethodException(err_msg.c_str());
    }
  }
#endif

#endif // MEASURE_I
