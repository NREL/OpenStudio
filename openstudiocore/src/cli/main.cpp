#include "RubyInterpreter.hpp"
#include "embedded_files.hpp"
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

  //void Init_openstudioairflow(void);
  //void Init_openstudiomodelcore(void);
  //void Init_openstudiomodelsimulation(void);
  //void Init_openstudioutilitiescore(void);
  ////void Init_openstudioutilitiesplot(void);
  ////void Init_openstudioenergyplus(void);
  ////void Init_openstudiomodeleditor(void);
  ////void Init_openstudioosversion(void);
  //void Init_openstudioutilitiesdata(void);
  //void Init_openstudioutilitiessql(void);
  ////void Init_openstudiogbxml(void);
  ////void Init_openstudiomodelgenerators(void);
  ////void Init_openstudioradiance(void);
  //void Init_openstudioutilitieseconomics(void);
  //void Init_openstudioutilitiestime(void);
  ////void Init_openstudioisomodel(void);
  ////void Init_openstudiomodelgeometry(void);
  ////void Init_openstudiosdd(void);
  //void Init_openstudioutilitiesfiletypes(void);
  //void Init_openstudioutilitiesunits(void);
  ////void Init_openstudiolib(void);
  ////void Init_openstudiomodelhvac(void);
  //void Init_openstudioutilities(void);
  //void Init_openstudioutilitiesgeometry(void);
  ////void Init_openstudiomeasure(void);
  ////void Init_openstudiomodelrefrigeration(void);
  //void Init_openstudioutilitiesbcl(void);
  //void Init_openstudioutilitiesidd(void);
  ////void Init_openstudiomodel(void);
  ////void Init_openstudiomodelresources(void);
  //void Init_openstudioutilitiescloud(void);
  //void Init_openstudioutilitiesidf(void);
}


int main(int argc, char *argv[])
{
  std::cout << "***** Initializing ruby *****\n";
  ruby_sysinit(&argc, &argv);
  {
    RUBY_INIT_STACK;
    ruby_init();
    INIT_CALLS;
    RB_PROVIDES_CALLS;

    //Init_openstudioairflow();
    //Init_openstudiomodelcore();
    //Init_openstudiomodelsimulation();
    //Init_openstudioutilitiescore();
    //Init_openstudioutilitiesplot();
    //Init_openstudioenergyplus();
    //Init_openstudiomodeleditor();
    //Init_openstudioosversion();
    //Init_openstudioutilitiesdata();
    //Init_openstudioutilitiessql();
    ////Init_openstudiogbxml();
    ////Init_openstudiomodelgenerators();
    ////Init_openstudioradiance();
    //Init_openstudioutilitieseconomics();
    //Init_openstudioutilitiestime();
    ////Init_openstudioisomodel();
    ////Init_openstudiomodelgeometry();
    ////Init_openstudiosdd();
    //Init_openstudioutilitiesfiletypes();
    //Init_openstudioutilitiesunits();
    ////Init_openstudiolib();
    ////Init_openstudiomodelhvac();
    //Init_openstudioutilities();
    //Init_openstudioutilitiesgeometry();
    ////Init_openstudiomeasure();
    ////Init_openstudiomodelrefrigeration();
    //Init_openstudioutilitiesbcl();
    //Init_openstudioutilitiesidd();
    ////Init_openstudiomodel();
    ////Init_openstudiomodelresources();
    //Init_openstudioutilitiescloud();
    //Init_openstudioutilitiesidf();
  }
    Init_EmbeddedScripting();

  /////std::cout << "***** Initializing RubyInterpreter Wrapper *****\n";
  /////std::vector<std::string> paths;
  /////RubyInterpreter rubyInterpreter(paths);

  /////std::cout << "***** Shimming Our Kernel::require method *****\n";
  /////auto embedded_extensions_string = embedded_files::getFileAsString(":/embedded_help.rb");
  /////rubyInterpreter.evalString(embedded_extensions_string);
  /////rubyInterpreter.evalString(R"(require 'csv.rb')");


}
