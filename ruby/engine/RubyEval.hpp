#ifndef RUBYEVAL_included
#define RUBYEVAL_included

#include "ruby.h"
#include "./RubyException.hpp"

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
    VALUE errval = rb_eval_string("$!.to_s");
    char* str = StringValuePtr(errval);
    std::string err(str);
    VALUE locval = rb_eval_string("$@.to_s");
    str = StringValuePtr(locval);
    std::string loc(str);
    throw RubyException(err, loc);
  }

  return result;
}

}  // namespace openstudio

#endif  // RUBYEVAL_included
