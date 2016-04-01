#include "RubyInterpreter.hpp"
#include "embedded_files.hpp"
#include "GC_Value.hpp"
#include "InitMacros.hxx"

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
  void Init_big5(void);
  void Init_cp949(void);
  void Init_emacs_mule(void);
  void Init_euc_jp(void);
  void Init_euc_kr(void);
  void Init_euc_tw(void);
  void Init_gb2312(void);
  void Init_gb18030(void);
  void Init_gbk(void);
  void Init_iso_8859_1(void);
  void Init_iso_8859_2(void);
  void Init_iso_8859_3(void);
  void Init_iso_8859_4(void);
  void Init_iso_8859_5(void);
  void Init_iso_8859_6(void);
  void Init_iso_8859_7(void);
  void Init_iso_8859_8(void);
  void Init_iso_8859_9(void);
  void Init_iso_8859_10(void);
  void Init_iso_8859_11(void);
  void Init_iso_8859_13(void);
  void Init_iso_8859_14(void);
  void Init_iso_8859_15(void);
  void Init_iso_8859_16(void);
  void Init_koi8_r(void);
  void Init_koi8_u(void);
  void Init_shift_jis(void);
  void Init_utf_16be(void);
  void Init_utf_16le(void);
  void Init_utf_32be(void);
  void Init_utf_32le(void);
  void Init_windows_31j(void);
  void Init_windows_1250(void);
  void Init_windows_1251(void);
  void Init_windows_1252(void);

  //void Init_openstudioairflow(void);
  void Init_openstudiomodelcore(void);
  void Init_openstudiomodelsimulation(void);
  void Init_openstudioutilitiescore(void);
  void Init_openstudioutilitiesplot(void);
  ////void Init_openstudioenergyplus(void);
  ////void Init_openstudiomodeleditor(void);
  ////void Init_openstudioosversion(void);
  void Init_openstudioutilitiesdata(void);
  void Init_openstudioutilitiessql(void);
  ////void Init_openstudiogbxml(void);
  ////void Init_openstudiomodelgenerators(void);
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
  ////void Init_openstudiomeasure(void);
  void Init_openstudiomodelrefrigeration(void);
  void Init_openstudioutilitiesbcl(void);
  void Init_openstudioutilitiesidd(void);
  void Init_openstudiomodel(void);
  void Init_openstudiomodelresources(void);
  void Init_openstudioutilitiescloud(void);
  void Init_openstudioutilitiesidf(void);
}


int main(int argc, char *argv[])
{
  std::cout << "***** Initializing ruby *****\n";
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

    RB_PROVIDES_CALLS;


  Init_encdb();
  rb_provide("encdb.so");
  Init_big5();
  Init_cp949();
  Init_emacs_mule();
  Init_euc_jp();
  Init_euc_kr();
  Init_euc_tw();
  Init_gb2312();
  Init_gb18030();
  Init_gbk();
  Init_iso_8859_1();
  Init_iso_8859_2();
  Init_iso_8859_3();
  Init_iso_8859_4();
  Init_iso_8859_5();
  Init_iso_8859_6();
  Init_iso_8859_7();
  Init_iso_8859_8();
  Init_iso_8859_9();
  Init_iso_8859_10();
  Init_iso_8859_11();
  Init_iso_8859_13();
  Init_iso_8859_14();
  Init_iso_8859_15();
  Init_iso_8859_16();
  Init_koi8_r();
  Init_koi8_u();
  Init_shift_jis();
  Init_utf_16be();
  Init_utf_16le();
  Init_utf_32be();
  Init_utf_32le();
  Init_windows_31j();
  Init_windows_1250();
  Init_windows_1251();
  Init_windows_1252();

  Init_openstudioutilitiescore();
  Init_openstudioutilitiestime();
  Init_openstudioutilitiesdata();
  Init_openstudioutilitiesplot();
  Init_openstudioutilitiesgeometry();
  Init_openstudioutilitiessql();
  Init_openstudioutilitiesbcl();
  Init_openstudioutilitiescloud();
  Init_openstudioutilitiesunits();
  Init_openstudioutilitiesidd();
  Init_openstudioutilitiesidf();
  Init_openstudioutilitiesfiletypes();
  Init_openstudioutilities();
  Init_openstudiomodel();
  Init_openstudiomodelcore();
  Init_openstudiomodelsimulation();
  Init_openstudiomodelresources();
  Init_openstudiomodelgeometry();
  Init_openstudiomodelhvac();
  Init_openstudiomodelrefrigeration();
  //Init_openstudiomodelgenerators();
  //Init_openstudioenergyplus();
  //Init_openstudioradiance();
  //Init_openstudiogbxml();
  //Init_openstudioairflow();
  //Init_openstudioosversion();
  //Init_openstudiomeasure();
  //Init_openstudiomodeleditor();
  //Init_openstudiolib();
  //Init_openstudioisomodel();
  //Init_openstudiosdd();
  }
    Init_EmbeddedScripting();

  /////std::cout << "***** Initializing RubyInterpreter Wrapper *****\n";
  std::vector<std::string> paths;
  RubyInterpreter rubyInterpreter(paths);

  /////std::cout << "***** Shimming Our Kernel::require method *****\n";
  // Need embedded_help for requiring files out of the embedded system
  auto embedded_extensions_string = embedded_files::getFileAsString(":/embedded_help.rb");
  rubyInterpreter.evalString(embedded_extensions_string);
  // Load whatever you want from the standard library
  rubyInterpreter.evalString(R"(require 'csv.rb')");
  // Use whatever you want from the OpenStudio API
  rubyInterpreter.evalString(R"(OpenStudio::Model::Model.new().save('testmodel.osm',true))");


}
