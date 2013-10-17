#ifndef CONTAM_I
#define CONTAM_I

#ifdef SWIGPYTHON
%module openstudiocontam
#endif


#define UTILITIES_API
#define CONTAM_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  //#include <contam/ReverseTranslator.hpp>
  #include <contam/ForwardTranslator.hpp>
  using namespace openstudio::contam::prj;
  using namespace openstudio::contam;
  using namespace openstudio;
  
  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
  
//  #include <QDomDocument>
//  #include <QDomElement>
%}

// #ifdef SWIGCSHARP
//%rename(ContamReverseTranslator) openstudio::contam::ReverseTranslator;
%rename(ContamForwardTranslator) openstudio::contam::ForwardTranslator;
// #endif

%include <contam/PrjSubobjects.hpp>
// Without the next ignore line, VC++ gives an interesting error:
//   fatal error C1061: compiler limit : blocks nested too deeply
%ignore RunControl(STRING name=STRING_INIT,STRING version=STRING_INIT,int echo=0,STRING prjdesc=STRING_INIT,int skheight=0,int skwidth=0,int def_units=0,int def_flows=0,RX def_T=RX_INIT(0),int udefT=0,RX rel_N=RX_INIT(0),RX wind_H=RX_INIT(0),int uwH=0,RX wind_Ao=RX_INIT(0),RX wind_a=RX_INIT(0),RX scale=RX_INIT(0),int uScale=0,int orgRow=0,int orgCol=0,int invYaxis=0,int showGeom=0,WeatherData ssWeather=WeatherData(),WeatherData wptWeather=WeatherData(),STRING WTHpath=STRING_INIT,STRING CTMpath=STRING_INIT,STRING CVFpath=STRING_INIT,STRING DVFpath=STRING_INIT,STRING WPCfile=STRING_INIT,STRING EWCfile=STRING_INIT,STRING WPCdesc=STRING_INIT,RX X0=RX_INIT(0),RX Y0=RX_INIT(0),RX Z0=RX_INIT(0),RX angle=RX_INIT(0),int u_XYZ=0,RX epsPath=RX_INIT(0),RX epsSpcs=RX_INIT(0),STRING tShift=STRING_INIT,STRING dStart=STRING_INIT,STRING dEnd=STRING_INIT,int useWPCwp=0,int useWPCmf=0,int wpctrig=0,RX latd=RX_INIT(0),RX lgtd=RX_INIT(0),RX Tznr=RX_INIT(0),RX altd=RX_INIT(0),RX Tgrnd=RX_INIT(0),int utg=0,int u_a=0,int sim_af=0,int afcalc=0,int afmaxi=0,RX afrcnvg=RX_INIT(0),RX afacnvg=RX_INIT(0),RX afrelax=RX_INIT(0),int uac2=0,RX Pres=RX_INIT(0),int uPres=0,int afslae=0,int afrseq=0,int aflmaxi=0,RX aflcnvg=RX_INIT(0),int aflinit=0,int Tadj=0,int sim_mf=0,int ccmaxi=0,RX ccrcnvg=RX_INIT(0),RX ccacnvg=RX_INIT(0),RX ccrelax=RX_INIT(0),int uccc=0,int mfnmthd=0,int mfnrseq=0,int mfnmaxi=0,RX mfnrcnvg=RX_INIT(0),RX mfnacnvg=RX_INIT(0),RX mfnrelax=RX_INIT(0),RX mfngamma=RX_INIT(0),int uccn=0,int mftmthd=0,int mftrseq=0,int mftmaxi=0,RX mftrcnvg=RX_INIT(0),RX mftacnvg=RX_INIT(0),RX mftrelax=RX_INIT(0),RX mftgamma=RX_INIT(0),int ucct=0,int mfvmthd=0,int mfvrseq=0,int mfvmaxi=0,RX mfvrcnvg=RX_INIT(0),RX mfvacnvg=RX_INIT(0),RX mfvrelax=RX_INIT(0),int uccv=0,int mf_solver=0,int sim_1dz=0,int sim_1dd=0,RX celldx=RX_INIT(0),int sim_vjt=0,int udx=0,int cvode_mth=0,RX cvode_rcnvg=RX_INIT(0),RX cvode_acnvg=RX_INIT(0),RX cvode_dtmax=RX_INIT(0),int tsdens=0,RX tsrelax=RX_INIT(0),int tsmaxi=0,int cnvgSS=0,int densZP=0,int stackD=0,int dodMdt=0,STRING date_st=STRING_INIT,STRING time_st=STRING_INIT,STRING date_0=STRING_INIT,STRING time_0=STRING_INIT,STRING date_1=STRING_INIT,STRING time_1=STRING_INIT,STRING time_step=STRING_INIT,STRING time_list=STRING_INIT,STRING time_scrn=STRING_INIT,int restart=0,STRING rstdate=STRING_INIT,STRING rsttime=STRING_INIT,int list=0,int doDlg=0,int pfsave=0,int zfsave=0,int zcsave=0,int achvol=0,int achsave=0,int abwsave=0,int cbwsave=0,int expsave=0,int ebwsave=0,int zaasave=0,int zbwsave=0,int rzfsave=0,int rzmsave=0,int rz1save=0,int csmsave=0,int srfsave=0,int logsave=0,std::vector<int> save=std::vector<int>(),std::vector<RX> rvals=std::vector<RX>(),int BldgFlowZ=0,int BldgFlowD=0,int BldgFlowC=0,int cfd_ctype=0,RX cfd_convcpl=RX_INIT(0),int cfd_var=0,int cfd_zref=0,int cfd_imax=0,int cfd_dtcmo=0);
%include <contam/PrjPublic.hpp>
%ignore Model(Reader &input);
%include <contam/PrjModel.hpp>
//%include <contam/ReverseTranslator.hpp>
%include <contam/ForwardTranslator.hpp>

#endif //CONTAM_I 
