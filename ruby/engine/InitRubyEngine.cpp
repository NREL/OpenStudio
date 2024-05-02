/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "./RubyEngine.hpp"
#include "./InitRubyEngine.hpp"
#include "./GC_Value.hpp"
#include <RubyInterpreter.hpp>
#include <iostream>
#include <embedded_files.hxx>

extern "C"
{
  // static void Init_builtin_prelude(void);
  // void Init_builtin_gem_prelude(void);
  struct ruby_cmdline_options;
  typedef struct ruby_cmdline_options ruby_cmdline_options_t;
  void Init_ruby_description(ruby_cmdline_options_t*);
}

namespace openstudio {

void RubyEngine::initRubyEngine() {
  RUBY_INIT_STACK;
  ruby_init();

  Init_ruby_description(nullptr);

  ruby_gc_set_params();
  ruby_init_loadpath();

  Init_enc();
  // rb_enc_set_default_internal(rb_enc_from_encoding(rb_utf8_encoding()));
  // rb_enc_set_default_external(rb_enc_from_encoding(rb_utf8_encoding()));

  Init_ext(); /* load statically linked extensions before rubygems */
  // Init_extra_exts();
  rb_call_builtin_inits();

  swig::GC_VALUE::hash_id = rb_intern("hash");
  swig::GC_VALUE::lt_id = rb_intern("<");
  swig::GC_VALUE::gt_id = rb_intern(">");
  swig::GC_VALUE::eq_id = rb_intern("==");
  swig::GC_VALUE::le_id = rb_intern("<=");
  swig::GC_VALUE::ge_id = rb_intern(">=");

  swig::GC_VALUE::pos_id = rb_intern("+@");
  swig::GC_VALUE::neg_id = rb_intern("-@");
  swig::GC_VALUE::inv_id = rb_intern("~");

  swig::GC_VALUE::add_id = rb_intern("+");
  swig::GC_VALUE::sub_id = rb_intern("-");
  swig::GC_VALUE::mul_id = rb_intern("*");
  swig::GC_VALUE::div_id = rb_intern("/");
  swig::GC_VALUE::mod_id = rb_intern("%");

  swig::GC_VALUE::and_id = rb_intern("&");
  swig::GC_VALUE::or_id = rb_intern("|");
  swig::GC_VALUE::xor_id = rb_intern("^");

  swig::GC_VALUE::lshift_id = rb_intern("<<");
  swig::GC_VALUE::rshift_id = rb_intern(">>");

  // rb_provide("rbconfig.so");

  // Need embedded_help for requiring files out of the embedded system
  auto embedded_extensions_string = embedded_files::getFileAsString(":/embedded_help.rb");

  try {
    rubyInterpreter.evalString(embedded_extensions_string);
  } catch (const std::exception& e) {
    rubyInterpreter.evalString(R"(STDOUT.flush)");
    std::cout << "Exception in embedded_help: " << e.what() << std::endl;  // endl will flush
    ruby_cleanup(1);
    throw;
  } catch (...) {
    rubyInterpreter.evalString(R"(STDOUT.flush)");
    std::cout << "Unknown Exception in embedded_help" << std::endl;  // endl will flush
    ruby_cleanup(1);
    throw;
  }
}

}  // namespace openstudio
