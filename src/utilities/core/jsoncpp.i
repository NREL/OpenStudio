#ifndef JSONCPP_I
#define JSONCPP_I

#if defined SWIGCSHARP
  // ignores toJSON/fromJSON globally
  %rename("$ignore", regextarget=1, fullname=1) "openstudio::.*::toJSON$";
  %rename("$ignore", regextarget=1, fullname=1) "openstudio::.*::fromJSON$";
#else
%{
  #include <json/value.h>
%}
#endif

#if defined SWIGPYTHON
%fragment("JsonToDict", "header", fragment="SWIG_FromCharPtrAndSize") {
  SWIGINTERN PyObject* SWIG_From_JsonValue(const Json::Value& value) {
    // PyErr_WarnEx(PyExc_UserWarning, "Translating a Json::Value to a PyObject", 1);  // Debugging

    if (value.isNull()) {
      return Py_None;
    }

    if (value.isBool()) {
      return value.asBool() ? Py_True : Py_False;
    }

    if (value.isDouble()) {
       return PyFloat_FromDouble(value.asDouble());
    }

    if (value.isIntegral()) {
      return PyLong_FromLongLong(value.asInt64());
    }

    if (value.isNumeric()) {
       return PyFloat_FromDouble(value.asDouble());
    }

    if (value.isString()) {
      // return PyUnicode_FromString(value.asCString());
      const auto str = value.asString();
      return SWIG_FromCharPtrAndSize(str.data(), str.size());
    }

    if (value.isArray()) {
      PyObject* result = PyList_New(value.size());
      Py_ssize_t idx = 0;
      for( const auto& arrayElement : value) {
        // recursive call
        auto val = SWIG_From_JsonValue(arrayElement);
        // PyList_Append(result, val);
        PyList_SetItem(result, idx++, val);
      }
      return result;
    }

    if (value.isObject()) {
      PyObject* result = PyDict_New();
      for( const auto& id : value.getMemberNames()) {
        // recursive call
        auto val = SWIG_From_JsonValue(value[id]);
        PyDict_SetItemString(result, id.c_str(), val);
        Py_DECREF(val);
      }
      return result;
    }

    return Py_None;
  }
}

%typemap(out, fragment="JsonToDict") Json::Value {
  $result = SWIG_From_JsonValue($1);
}


#endif

#if defined SWIGRUBY
%fragment("JsonToDict","header", fragment="SWIG_FromCharPtrAndSize") {
  SWIGINTERN VALUE SWIG_From_JsonValue(const Json::Value& value) {

    if (value.isNull()) {
      return Qnil;
    }

    if (value.isBool()) {
      return value.asBool() ? Qtrue : Qfalse;
    }

    if (value.isDouble()) {
      return DOUBLE2NUM(value.asDouble());
    }

    if (value.isIntegral()) {
      return INT2NUM(value.asInt64());
    }

    if (value.isNumeric()) {
     return DOUBLE2NUM(value.asDouble());
    }

    if (value.isString()) {
      const auto str = value.asString();
      return SWIG_FromCharPtrAndSize(str.data(), str.size());
    }

    if (value.isArray()) {
      VALUE result = rb_ary_new2(value.size());
      for( const auto& arrayElement : value) {
        rb_ary_push(result, SWIG_From_JsonValue(arrayElement));
      }
      return result;
    }

    if (value.isObject()) {
      VALUE result = rb_hash_new();
      for( const auto& id : value.getMemberNames()) {
        rb_hash_aset(result, ID2SYM(rb_intern(id.data())), SWIG_From_JsonValue(value[id]));
      }
      return result;
    }

    return Qnil;
  }
}

%typemap(out, fragment="JsonToDict") Json::Value {
  $result = SWIG_From_JsonValue($1);
}
#endif

#endif // JSONCPP_I
