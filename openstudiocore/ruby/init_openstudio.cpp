#include "init_openstudio.hpp"
#include <embedded_files.hxx>

extern "C" {
  //void Init_openstudioairflow(void);
  void Init_openstudiomodelcore(void);
  void Init_openstudiomodelsimulation(void);
  void Init_openstudioutilitiescore(void);
  void Init_openstudioutilitiesplot(void);
  void Init_openstudioenergyplus(void);
  ////void Init_openstudiomodeleditor(void);
  void Init_openstudioosversion(void);
  void Init_openstudioutilitiesdata(void);
  void Init_openstudioutilitiessql(void);
  ////void Init_openstudiogbxml(void);
  void Init_openstudiomodelgenerators(void);
  ////void Init_openstudioradiance(void);
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

void init_openstudio_internal() {
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
  //Init_openstudioenergyplus(); // failing on rb_require("openstudiomodel"), don't know why?
  ////Init_openstudioradiance();
  ////Init_openstudiogbxml();
  ////Init_openstudioairflow();
  //Init_openstudioosversion();
  //Init_openstudiomeasure();
  ////Init_openstudiomodeleditor();
  ////Init_openstudiolib();
  ////Init_openstudioisomodel();
  ////Init_openstudiosdd();
}


