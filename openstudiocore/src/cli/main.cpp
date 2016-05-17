#include "RubyInterpreter.hpp"
#include "GC_Value.hpp"
#include "InitMacros.hxx"
#include <embedded_files.hxx>

#include <iostream>

#ifndef _MSC_VER
#include <dlfcn.h>
#include <dirent.h>
#else
#include <windows.h>
#endif


extern "C" {
  void Init_EmbeddedScripting(void);
  INIT_DECLARATIONS;

  void Init_encdb(void);
  ENC_INIT_DECLARATIONS;

  void Init_transdb(void);
  TRANS_INIT_DECLARATIONS;

  void Init_generator(void);
  void Init_parser(void);
//../Ruby//ext/json/generator/generator.a
//../Ruby//ext/json/parser/parser.a



  //void Init_openstudioairflow(void);
  void Init_openstudiomodelcore(void);
  void Init_openstudiomodelsimulation(void);
  void Init_openstudioutilitiescore(void);
  void Init_openstudioutilitiesplot(void);
  void Init_openstudioenergyplus(void);
  ////void Init_openstudiomodeleditor(void);
  ////void Init_openstudioosversion(void);
  void Init_openstudioutilitiesdata(void);
  void Init_openstudioutilitiessql(void);
  ////void Init_openstudiogbxml(void);
  void Init_openstudiomodelgenerators(void);
  ////void Init_openstudioradiance(void);
  void Init_openstudioutilitieseconomics(void);
  void Init_openstudioutilitiestime(void);
  ////void Init_openstudioisomodel(void);
  void Init_openstudiomodelgeometry(void);
  ////void Init_openstudiosdd(void);
  void Init_openstudioutilitiesfiletypes(void);
  void Init_openstudioutilitiesunits(void);
  ////void Init_openstudiolib(void);
  void Init_openstudiomodelhvac(void);
  void Init_openstudioutilities(void);
  void Init_openstudioutilitiesgeometry(void);
  void Init_openstudiomeasure(void);
  void Init_openstudiomodelrefrigeration(void);
  void Init_openstudioutilitiesbcl(void);
  void Init_openstudioutilitiesidd(void);
  void Init_openstudiomodel(void);
  void Init_openstudiomodelresources(void);
  void Init_openstudioutilitiescloud(void);
  void Init_openstudioutilitiesidf(void);
}

std::vector<std::string> paths;
static RubyInterpreter rubyInterpreter(paths);//(paths);

int main(int argc, char *argv[])
{
  ruby_sysinit(&argc, &argv);
  {
    RUBY_INIT_STACK;
    ruby_init();

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
    swig::GC_VALUE::or_id  = rb_intern("|");
    swig::GC_VALUE::xor_id = rb_intern("^");

    swig::GC_VALUE::lshift_id = rb_intern("<<");
    swig::GC_VALUE::rshift_id = rb_intern(">>");

    INIT_CALLS;

    Init_encdb();
    rb_provide("encdb.so");
    ENC_INIT_CALLS;

    Init_transdb();
    rb_provide("trans/transdb");
    TRANS_INIT_CALLS;

    Init_openstudioutilitiescore();
    rb_provide("openstudioutilitiescore");
    Init_openstudioutilitiestime();
    rb_provide("openstudioutilitiestime");
    Init_openstudioutilitiesdata();
    rb_provide("openstudioutilitiesdata");
    Init_openstudioutilitiesplot();
    rb_provide("openstudioutilitiesplot");
    Init_openstudioutilitiesgeometry();
    rb_provide("openstudioutilitiesgeometry");
    Init_openstudioutilitiessql();
    rb_provide("openstudioutilitiessql");
    Init_openstudioutilitiesbcl();
    rb_provide("openstudioutilitiesbcl");
    Init_openstudioutilitiescloud();
    rb_provide("openstudioutilitiescloud");
    Init_openstudioutilitiesunits();
    rb_provide("openstudioutilitiesunits");
    Init_openstudioutilitiesidd();
    rb_provide("openstudioutilitiesidd");
    Init_openstudioutilitiesidf();
    rb_provide("openstudioutilitiesidf");
    Init_openstudioutilitiesfiletypes();
    rb_provide("openstudioutilitiesfiletypes");
    Init_openstudioutilities();
    rb_provide("openstudioutilities");
    Init_openstudiomodel();
    rb_provide("openstudiomodel");
    Init_openstudiomodelcore();
    rb_provide("openstudiomodelcore");
    Init_openstudiomodelsimulation();
    rb_provide("openstudiomodelsimulation");
    Init_openstudiomodelresources();
    rb_provide("openstudiomodelresources");
    Init_openstudiomodelgeometry();
    rb_provide("openstudiomodelgeometry");
    Init_openstudiomodelhvac();
    rb_provide("openstudiomodelhvac");
    Init_openstudiomodelrefrigeration();
    rb_provide("openstudiomodelrefrigeration");
    Init_openstudiomodelgenerators();
    rb_provide("openstudiomodelgenerators");
    Init_openstudioenergyplus(); // failing on rb_require("openstudiomodel"), don't know why?
    rb_provide("openstudioenergyplus");
    //Init_openstudioradiance();
    //Init_openstudiogbxml();
    //Init_openstudioairflow();
    //Init_openstudioosversion();
    Init_openstudiomeasure();
    rb_provide("openstudiomeasure");
    //Init_openstudiomodeleditor();
    //Init_openstudiolib();
    //Init_openstudioisomodel();
    //Init_openstudiosdd();

    //Init_generator();
    //Init_parser();

    Init_EmbeddedScripting();
  }

  // Need embedded_help for requiring files out of the embedded system
  auto embedded_extensions_string = embedded_files::getFileAsString(":/embedded_help.rb");
  rubyInterpreter.evalString(embedded_extensions_string);

  // chop off the first argument which is the exe path/name
  ruby_set_argv(argc - 1,argv + 1);
  //rubyInterpreter.evalString(R"(require 'irb')");
  //rubyInterpreter.evalString(R"(IRB.start)");

  try{
    rubyInterpreter.evalString("begin \n (require 'openstudio_cli') \n rescue Exception => e \n puts \n puts \"Error: #{e.message}\" \n puts \"Backtrace:\n\t\" + e.backtrace.join(\"\\n\\t\") \n raise \n end");
  } catch (const std::exception& e){
    rubyInterpreter.evalString(R"(STDOUT.flush)");
    std::cout << "Exception: " << e.what() << std::endl; // endl will flush
    return 1;
  } catch (...){
    rubyInterpreter.evalString(R"(STDOUT.flush)");
    std::cout << "Unknown Exception" << std::endl; // endl will flush
    return 1;
  }
  rubyInterpreter.evalString(R"(STDOUT.flush)");
  std::cout << std::flush;
  return 0;
}

extern "C" {
  int rb_hasFile(const char *t_filename) {
    // TODO Consider expanding this to use the path which we have artificially defined in embedded_help.rb
    std::string expandedName = std::string(":/ruby/2.2.0/") + std::string(t_filename) + ".rb";
    return embedded_files::hasFile(expandedName);
  }

  int rb_require_embedded(const char *t_filename) {
    std::string require_script = R"(require ')" + std::string(t_filename) + R"(')";
    rubyInterpreter.evalString(require_script);
    return 0;
  }
}

