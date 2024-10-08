/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef RUBYEVAL_included
#define RUBYEVAL_included

#include <ruby.h>
#include "RubyException.hpp"

namespace openstudio {

inline VALUE evaluateSimpleImpl(VALUE arg) {
  return rb_eval_string(StringValuePtr(arg));
}

inline VALUE evalString(const std::string& t_str) {
  VALUE val = rb_str_new2(t_str.c_str());
  int error;
  // save and restore the current working directory in case the call to ruby upsets it
  VALUE result = rb_protect(evaluateSimpleImpl, val, &error);
  if (error != 0) {

    // VALUE errval = rb_eval_string("$!.to_s");
    // char* str = StringValuePtr(errval);
    // std::string err(str);

    VALUE errinfo = rb_errinfo();
    VALUE exception_class = rb_obj_class(errinfo);
    VALUE classNameValue = rb_class_name(exception_class);
    std::string className(StringValuePtr(classNameValue));

    VALUE errstr = rb_obj_as_string(errinfo);
    std::string errMessage(StringValuePtr(errstr));

    std::string totalErr = className + ": " + errMessage;

    // I just **cannot** figure out a way to get the error location in C, without calling $@.to_s. Seems nothing is available here
    int locationError;
    VALUE locval = rb_eval_string_protect("$@.to_s", &locationError);
    std::string loc;
    if (locationError == 0) {
      loc = StringValuePtr(locval);
    } else {
      loc = "Failed to get VM location";
    }

    // Generally speaking, the backtrace is there, but not for the case where it's a stack too deep error
    const ID ID_backtrace = rb_intern_const("backtrace");
    if (exception_class != rb_eSysStackError && rb_respond_to(errinfo, ID_backtrace)) {
      // std::vector<std::string> backtrace_lines;
      std::string btlines;
      /*volatile*/ VALUE backtrace;
      if (!NIL_P(backtrace = rb_funcall(errinfo, ID_backtrace, 0))) {
        VALUE backtracejoin = rb_ary_join(backtrace, rb_str_new2("\n"));
        btlines = StringValuePtr(backtracejoin);

        // Get the backing C array of the ruby array
        // VALUE* elements = RARRAY_PTR(backtrace);
        // for (long c = 0; c < RARRAY_LEN(backtrace); ++c) {
        //   VALUE entry = elements[c];
        //   [[maybe_unused]] char* backtrace_line = RSTRING_PTR(entry);
        //   char* backtrace_line2 = StringValuePtr(entry);
        //   backtrace_lines.emplace_back(backtrace_line2);
        // }
      }

      if (!btlines.empty()) {
        loc += "\n\nTraceback:\n" + btlines;
      }
    } else {
    }

    // Clear the last error, we raise it on our side only, so Ruby needs to be cleared.
    // Otherwise later API calls might read the old value and think another exception has occurred
    rb_set_errinfo(Qnil);

    throw RubyException(totalErr, loc);
  }

  return result;
}

}  // namespace openstudio

#endif  // RUBYEVAL_included
