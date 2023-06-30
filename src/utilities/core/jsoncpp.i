#ifndef JSONCPP_I
#define JSONCPP_I

#if defined SWIGCSHARP
%ignore openstudio::*::toJSON() const;
#else
%{
  #include <json/value.h>
%}
#endif

#if defined SWIGPYTHON
/*******************************************************************************
 *                                 TYPEMAP: OUT                                *
 *******************************************************************************/
%fragment("JsonToDict", "header", fragment="SWIG_FromCharPtrAndSize") {
  SWIGINTERN PyObject* SWIG_From_JsonValue(const Json::Value& value) {
    // PyErr_WarnEx(PyExc_UserWarning, "Translating a Json::Value to a PyObject", 1);  // Debugging

    if (value.isNull()) {
      return Py_None;
    }

    if (value.isBool()) {
      return value.asBool() ? Py_True : Py_False;
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

/*******************************************************************************
 *                                 TYPEMAP: IN                                 *
 *******************************************************************************/
%fragment("ToJsonValue", "header") {
  SWIGINTERN Json::Value SWIG_to_JsonValue(PyObject * obj) {
    PyErr_WarnEx(PyExc_UserWarning, "Constructing a Json::Value", 1);  // Debugging
    if (obj == Py_None) {
      return Json::Value{Json::nullValue};
    }

    if (PyBool_Check(obj)) {
      bool b = PyObject_IsTrue(obj) == 1;
      return Json::Value{b};
    }

    if (PyFloat_Check(obj)) {
      auto d = PyFloat_AsDouble(obj);
      return Json::Value{d};
    }

    if (PyNumber_Check(obj)) { // or PyLong_Check
      std::int64_t i = PyLong_AsLongLong(obj);
      return Json::Value{i};
    }

    if (PyUnicode_Check(obj)) {
      const char *p = PyUnicode_AsUTF8(obj);
      return Json::Value{p};
    }

    if (PyList_Check(obj)) {
      auto result = Json::Value(Json::arrayValue);
      size_t n = PyList_Size(obj);
      for (size_t i = 0; i < n; ++i) {
        result.append(SWIG_to_JsonValue(PyList_GetItem(obj, i)));
      }
      return result;
    }

    if (PyTuple_Check(obj)) {
      auto result = Json::Value(Json::arrayValue);
      size_t n = PyTuple_Size(obj);
      for (size_t i = 0; i < n; ++i) {
        result.append(SWIG_to_JsonValue(PyTuple_GetItem(obj, i)));
      }
      return result;
    }

    if (PyAnySet_Check(obj)) {
      auto result = Json::Value(Json::arrayValue);
      PyObject *iter;
      if ((iter = PyObject_GetIter(obj)) == nullptr) {
        return result;
      }
      PyObject *item;
      while ((item = PyIter_Next(iter)) != nullptr) {
        result.append(SWIG_to_JsonValue(item));
        Py_DECREF(item);
      }
      Py_DECREF(iter);
      return result;
    }

    if (PyDict_Check(obj)) {
      auto result = Json::Value(Json::objectValue);
      PyObject *key;
      PyObject *value;
      Py_ssize_t pos = 0;

      while (PyDict_Next(obj, &pos, &key, &value)) {
        if (!PyUnicode_Check(key)) {
          std::invalid_argument("Object keys must be strings");
        }
        const char * jsonKey = PyUnicode_AsUTF8(key);
        result[jsonKey] = SWIG_to_JsonValue(value);
      }
      return result;
    }

    return Json::Value{Json::nullValue};
  }

}

%typemap(in, fragment="ToJsonValue") Json::Value {
  void* argp = 0;
  int res = SWIG_ConvertPtr($input, &argp, $descriptor(Json::Value *), $disown |  0 );
  if (SWIG_IsOK(res)) {
    PyErr_WarnEx(PyExc_UserWarning, "reusing a Json::Value", 1);
    Json::Value * temp = %reinterpret_cast(argp, $ltype *);
    $1 = *temp;
  } else {
    $1 = SWIG_to_JsonValue($input);
  }
}

%typemap(in, fragment="ToJsonValue") Json::Value const *, Json::Value const & {
  void* argp = 0;
  int res = SWIG_ConvertPtr($input, &argp, $descriptor, $disown |  0 );
  if (SWIG_IsOK(res)) {
    PyErr_WarnEx(PyExc_UserWarning, "reusing a Json::Value", 1);
    $1 = %reinterpret_cast(argp, $ltype);
  } else {
    $1 = new Json::Value(SWIG_to_JsonValue($input));
  }
}

#endif

#if defined SWIGRUBY
/*******************************************************************************
 *                                 TYPEMAP: OUT                                *
 *******************************************************************************/
%fragment("JsonToDict","header", fragment="SWIG_FromCharPtrAndSize") {
  SWIGINTERN VALUE SWIG_From_JsonValue(const Json::Value& value) {

    if (value.isNull()) {
      return Qnil;
    }

    if (value.isBool()) {
      return value.asBool() ? Qtrue : Qfalse;
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

/*******************************************************************************
 *                                 TYPEMAP: IN                                 *
 *******************************************************************************/
%fragment("ToJsonValue","header") {

  SWIGINTERN Json::Value SWIG_to_JsonValue(VALUE obj) {

    if (RB_TYPE_P(obj, T_NIL)) {
      return Json::Value{Json::nullValue};
    }

    if (RB_TYPE_P(obj, T_TRUE)) {
      return Json::Value(true);
    }

    if (RB_TYPE_P(obj, T_FALSE)) {
      return Json::Value(false);
    }

    if (RB_TYPE_P(obj, T_FIXNUM)) {
      return Json::Value(NUM2INT(obj));
    }

    if (RB_TYPE_P(obj, T_BIGNUM)) {
      return Json::Value(static_cast<std::int64_t>(NUM2LL(obj)));
    }

    if (RB_TYPE_P(obj, T_FLOAT)) {
      return Json::Value(NUM2DBL(obj));
    }

    if (RB_TYPE_P(obj, T_SYMBOL)) {
      return Json::Value(rb_id2name(SYM2ID(obj)));
    }

    if (RB_TYPE_P(obj, T_STRING)) {
      // This is potentially not null terminated...
      return Json::Value(StringValuePtr(obj));
    }

    if (RB_TYPE_P(obj, T_ARRAY)) {
      auto result = Json::Value(Json::arrayValue);

      VALUE* elements = RARRAY_PTR(obj);
      for (long c = 0; c < RARRAY_LEN(obj); ++c) {
        VALUE entry = elements[c];
        result.append(SWIG_to_JsonValue(entry));
      }
      return result;
    }


    if (RB_TYPE_P(obj, T_HASH)) {
      auto result = Json::Value(Json::objectValue);

      VALUE keys = rb_funcall(obj, rb_intern("keys"), 0);
      VALUE* elements = RARRAY_PTR(keys);
      for (long c = 0; c < RARRAY_LEN(keys); ++c) {
        VALUE key = elements[c];
        std::string jsonKey;
        if (RB_TYPE_P(key, T_SYMBOL)) {
           jsonKey = std::string(rb_id2name(SYM2ID(key)));
        } else if (RB_TYPE_P(key, T_STRING)) {
          // This is potentially not null terminated...
          jsonKey = std::string(StringValuePtr(key));
        } else {
          throw std::runtime_error("Object keys must be strings or keys");
        }
        VALUE value = rb_hash_aref(obj, key);
        result[jsonKey] = SWIG_to_JsonValue(value);
      }
      return result;
    }

    return Json::Value{Json::nullValue};
  }
}

%typemap(in, fragment="ToJsonValue") Json::Value {
  void* argp = 0;
  int res = SWIG_ConvertPtr($input, &argp, $descriptor(Json::Value *), $disown |  0 );
  if (SWIG_IsOK(res)) {
    Json::Value * temp = %reinterpret_cast(argp, $ltype *);
    $1 = *temp;
  } else {
    $1 = SWIG_to_JsonValue($input);
  }
}

%typemap(in, fragment="ToJsonValue") Json::Value const *, Json::Value const & {
  void* argp = 0;
  int res = SWIG_ConvertPtr($input, &argp, $descriptor, $disown |  0 );
  if (SWIG_IsOK(res)) {
    $1 = %reinterpret_cast(argp, $ltype);
  } else {
    $1 = new Json::Value(SWIG_to_JsonValue($input));
  }
}
#endif

#endif // JSONCPP_I
