#ifndef JSONCPP_I
#define JSONCPP_I

%{
  #include <json/value.h>
%}

#if defined SWIGPYTHON
%fragment("JsonToDict","header", fragment="SWIG_FromCharPtrAndSize") {
  inline PyObject* SWIG_From_JsonValue(const Json::Value& value) {

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
              // TODO: this should do a recursive call to convert n (which is Json::Value) to a python type...
              auto val = SWIG_From_JsonValue(arrayElement);
              // PyList_Append(result, val);
              PyList_SetItem(result, idx++, val);
          }
          return result;

      } else if (value.isObject()) {
          PyObject* result = PyDict_New();
          for( const auto& id : value.getMemberNames()) {
              // TODO: this should do a recursive call to convert *$1[id] (which is a Json::Value) to a python type...
              auto val = SWIG_From_JsonValue(value[id]);
              PyDict_SetItemString(result, id.c_str(), val);
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

#if defined SWIGRUBY
%fragment("JsonToDict","header", fragment="SWIG_FromCharPtrAndSize") {
  inline VALUE SWIG_From_JsonValue(const Json::Value& value) {

      if (value.isBool()) {
        return value.asBool() ? Qtrue : Qfalse;
      } else if (value.isIntegral()) {
          return INT2NUM(value.asInt64());
      } else if (value.isNumeric()) {
         return DOUBLE2NUM(value.asDouble());
      } else if (value.isString()) {
         const auto str = value.asString();
         return SWIG_FromCharPtrAndSize(str.data(), str.size());
      } else if (value.isArray()) {
          VALUE result = rb_ary_new2(value.size());
          for( const auto& arrayElement : value) {
              rb_ary_push(result, SWIG_From_JsonValue(arrayElement));
          }
          return result;

      } else if (value.isObject()) {
          VALUE result = rb_hash_new();
          for( const auto& id : value.getMemberNames()) {
              rb_hash_aset(result, ID2SYM(rb_intern(id.data())), SWIG_From_JsonValue(value[id]));
          }
          return result;
      }

      return rb_hash_new();
  }
}

%typemap(out, fragment="JsonToDict") Json::Value {
  $result = SWIG_From_JsonValue($1);
}
#endif

#endif // JSONCPP_I
