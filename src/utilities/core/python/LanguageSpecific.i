#ifndef UTILITIES_PYTHON_LANGUAGESPECIFIC_I
#define UTILITIES_PYTHON_LANGUAGESPECIFIC_I

%include <wchar.i>
%include <cwstring.i>
%include <std_wstring.i>
%include <std_set.i>
%include <std_shared_ptr.i>
%include <boost_shared_ptr.i>

%begin %{
#ifdef _MSC_VER
#define HAVE_ROUND
#endif
%}

%fragment("JsonToDict","header", fragment="SWIG_FromCharPtrAndSize") {
  PyObject* SWIG_From_JsonValue(const Json::Value& value) {

      if (value.isBool()) {
        return value.asBool() ? Py_True : Py_False;
      } else if (value.isIntegral()) {
          return PyLong_FromLongLong(value.asInt64());
      } else if (value.isNumeric()) {
         return PyFloat_FromDouble(value.asDouble());
      } else if (value.isString()) {
         // return PyUnicode_FromString(value.asCString());
         const auto str = value.asString();
         return SWIG_FromCharPtrAndSize(str.data(), str.size());
      } else if (value.isArray()) {
          PyObject* result = PyList_New(value.size());
          Py_ssize_t idx = 0;
          for( const auto& arrayElement : value) {
              auto val = SWIG_From_JsonValue(arrayElement);
              PyList_SetItem(result, idx++, val); // This steals the reference so no decref
          }
          return result;

      } else if (value.isObject()) {
          PyObject* result = PyDict_New();
          for( const auto& id : value.getMemberNames()) {
              auto val = SWIG_From_JsonValue(value[id]);
              PyDict_SetItemString(result, id.c_str(), val);  // Does **not** steal so decref
              Py_DECREF(val);
          }
          return result;
      }

      return PyDict_New();
  }
}
%typemap(out, fragment="JsonToDict") Json::Value {
  $result = SWIG_From_JsonValue($1);
}

#endif
