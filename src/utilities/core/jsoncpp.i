#ifndef JSONCPP_I
#define JSONCPP_I

#if defined SWIGCSHARP
  // ignores toJSON/fromJSON globally
  %rename("$ignore", regextarget=1, fullname=1) "openstudio::.*::toJSON$";
  %rename("$ignore", regextarget=1, fullname=1) "openstudio::.*::fromJSON$";
  %rename("$ignore", regextarget=1, fullname=1) "openstudio::.*::valueAsJSON$";
#else
%{
  #include <json/value.h>
%}
#endif

#if defined SWIGPYTHON
%fragment("JsonToDict", "header", fragment="SWIG_FromCharPtrAndSize") {
  SWIGINTERN PyObject* SWIG_From_JsonValue(const Json::Value& value) {
    // PyErr_WarnEx(PyExc_UserWarning, "Translating a Json::Value to a PyObject", 1);  // Debugging

    switch(value.type()) {
    case Json::nullValue:
      return Py_None;

    case Json::booleanValue:
      return value.asBool() ? Py_True : Py_False;

    case Json::intValue:
    case Json::uintValue:
      return PyLong_FromLongLong(value.asInt64());

    case Json::realValue:
      return PyFloat_FromDouble(value.asDouble());

    case Json::stringValue: {
      // return PyUnicode_FromString(value.asCString());
      const auto str = value.asString();
      return SWIG_FromCharPtrAndSize(str.data(), str.size());
    }

    case Json::arrayValue:{
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

    case Json::objectValue: {
      PyObject* result = PyDict_New();
      for( const auto& id : value.getMemberNames()) {
        // recursive call
        auto val = SWIG_From_JsonValue(value[id]);
        PyDict_SetItemString(result, id.c_str(), val);
        Py_DECREF(val);
      }
      return result;
    }

    default:
      assert(false);
    }

    return Py_None;
  }
}

%typemap(out, fragment="JsonToDict") Json::Value {
  $result = SWIG_From_JsonValue($1);
}


#endif

#if defined SWIGRUBY

%fragment("<ruby_encoding>", "header") %{
#include <ruby/encoding.h>
%}

%fragment("string_to_ruby_utf8_symbol", "header", fragment="<ruby_encoding>") {
  SWIGINTERN VALUE SWIG_String_to_ruby_utf8_symbol(const std::string& str) {
    return ID2SYM(rb_intern3(str.data(), str.size(), rb_utf8_encoding()));
  }
}

%fragment("JsonToDict", "header", fragment="SWIG_FromCharPtrAndSize", fragment="string_to_ruby_utf8_symbol") {

  SWIGINTERN VALUE SWIG_From_JsonValue(const Json::Value& value) {

    switch(value.type()) {
    case Json::nullValue:
      return Qnil;

    case Json::booleanValue:
      return value.asBool() ? Qtrue : Qfalse;

    case Json::intValue:
    case Json::uintValue:
      return INT2NUM(value.asInt64());

    case Json::realValue:
      return DOUBLE2NUM(value.asDouble());

    case Json::stringValue: {
      // return PyUnicode_FromString(value.asCString());
      const auto str = value.asString();
      return SWIG_FromCharPtrAndSize(str.data(), str.size());
    }
    case Json::arrayValue: {
      VALUE result = rb_ary_new2(value.size());
      for( const auto& arrayElement : value) {
        rb_ary_push(result, SWIG_From_JsonValue(arrayElement));
      }
      return result;
    }
    case Json::objectValue: {
      VALUE result = rb_hash_new();
      for( const auto& id : value.getMemberNames()) {
        rb_hash_aset(result,
                    SWIG_String_to_ruby_utf8_symbol(id),
                    SWIG_From_JsonValue(value[id])
        );
      }
      return result;
    }
    default:
      assert(false);
    }

    return Qnil;
  }
}

%typemap(out, fragment="JsonToDict") Json::Value {
  $result = SWIG_From_JsonValue($1);
}
#endif

#endif // JSONCPP_I
