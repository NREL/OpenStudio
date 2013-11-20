/**********************************************************************
*  Copyright (c) 2013, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/
#ifndef PRJPUBLIC_H
#define PRJPUBLIC_H

#include "PrjDefines.hpp"
#include "PrjPrivate.hpp"
#include <QExplicitlySharedDataPointer>
#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {

class CONTAM_API Zone
{
public:
  enum Flags {
    // Zone flag bit 1; variable pressure
    VAR_P=0x0001,NVAR_P=0xFFFE,
    // Zone flag bit 2; variable contaminants
    VAR_C=0x0002,NVAR_C=0xFFFD,
    // Zone flag bit 3; variable temperature
    VAR_T=0x0004,NVAR_T=0xFFFB,
    // Zone flag bit 4; system zone: to avoid zone volume check
    SYS_N=0x0008,NSYS_N=0xFFF7,
    // Zone flag bit 5; un|conditioned space: to compute air change rate
    UNCZN =0x0010 , // flags | UNCZN to indicate unconditioned zone
    SETCZN=0xFFEF, // flags & SETCZN to indicate conditioned zone
    // Zone flag bit 6; CFD zone
    CFDZN =0x0020, // flags | CFDZN to set CFD zone / flags & CFDZN to test for CFD zone
    NCFDZN=0xFFDF,   // flags & NCFDZN to unset CFD zone
    FLAG_N=0x003F};   // all zone flag bits, used in PrjRead()

    Zone(int nr=0,unsigned int flags=0,int ps=0,int pc=0,int pk=0,int pl=0,RX relHt=RX_INIT(0),RX Vol=RX_INIT(0),RX T0=RX_INIT(0),RX P0=RX_INIT(0),std::string name=std::string(),int color=0,int u_Ht=0,int u_V=0,int u_T=0,int u_P=0,int cdaxis=0,int cfd=0,std::string cfdname=std::string(),RX X1=RX_INIT(0),RX Y1=RX_INIT(0),RX H1=RX_INIT(0),RX X2=RX_INIT(0),RX Y2=RX_INIT(0),RX H2=RX_INIT(0),RX celldx=RX_INIT(0),RX axialD=RX_INIT(0),int u_aD=0,int u_L=0);
    void read(Reader &input);
    std::string write();
    int nr() const;
    void setNr(const int nr);
    unsigned int flags() const;
    void setFlags(const unsigned int flags);
    int ps() const;
    void setPs(const int ps);
    int pc() const;
    void setPc(const int pc);
    int pk() const;
    void setPk(const int pk);
    int pl() const;
    void setPl(const int pl);
    RX relHt() const;
    void setRelHt(const RX relHt);
    RX Vol() const;
    void setVol(const RX Vol);
    RX T0() const;
    void setT0(const RX T0);
    RX P0() const;
    void setP0(const RX P0);
    std::string name() const;
    void setName(const std::string name);
    int color() const;
    void setColor(const int color);
    int u_Ht() const;
    void setU_Ht(const int u_Ht);
    int u_V() const;
    void setU_V(const int u_V);
    int u_T() const;
    void setU_T(const int u_T);
    int u_P() const;
    void setU_P(const int u_P);
    int cdaxis() const;
    void setCdaxis(const int cdaxis);
    int cfd() const;
    void setCfd(const int cfd);
    std::string cfdname() const;
    void setCfdname(const std::string cfdname);
    RX X1() const;
    void setX1(const RX X1);
    RX Y1() const;
    void setY1(const RX Y1);
    RX H1() const;
    void setH1(const RX H1);
    RX X2() const;
    void setX2(const RX X2);
    RX Y2() const;
    void setY2(const RX Y2);
    RX H2() const;
    void setH2(const RX H2);
    RX celldx() const;
    void setCelldx(const RX celldx);
    RX axialD() const;
    void setAxialD(const RX axialD);
    int u_aD() const;
    void setU_aD(const int u_aD);
    int u_L() const;
    void setU_L(const int u_L);

    // Custom getters/setters
    void setVariablePressure(bool b);
    bool variablePressure() const;
    void setVariableContaminants(bool b);
    bool variableContaminants() const;
    void setSystem(bool b);
    bool system() const;
    RX ic(const int i) const;
    std::vector<RX> ic() const;
    void setIc(const int i, const RX value);
    void setIc(std::vector<RX> ic);

private:
  QExplicitlySharedDataPointer<ZonePrivate> d;
};

class CONTAM_API Species
{
public:
  Species(int nr=0,int sflag=0,int ntflag=0,RX molwt=RX_INIT(0),RX mdiam=RX_INIT(0),RX edens=RX_INIT(0),RX decay=RX_INIT(0),RX Dm=RX_INIT(0),RX ccdef=RX_INIT(0),RX Cp=RX_INIT(0),int ucc=0,int umd=0,int ued=0,int udm=0,int ucp=0,std::string name=std::string(),std::string desc=std::string());
  void read(Reader &input);
  std::string write();
  int nr() const;
  void setNr(const int nr);
  int sflag() const;
  void setSflag(const int sflag);
  int ntflag() const;
  void setNtflag(const int ntflag);
  RX molwt() const;
  void setMolwt(const RX molwt);
  RX mdiam() const;
  void setMdiam(const RX mdiam);
  RX edens() const;
  void setEdens(const RX edens);
  RX decay() const;
  void setDecay(const RX decay);
  RX Dm() const;
  void setDm(const RX Dm);
  RX ccdef() const;
  void setCcdef(const RX ccdef);
  RX Cp() const;
  void setCp(const RX Cp);
  int ucc() const;
  void setUcc(const int ucc);
  int umd() const;
  void setUmd(const int umd);
  int ued() const;
  void setUed(const int ued);
  int udm() const;
  void setUdm(const int udm);
  int ucp() const;
  void setUcp(const int ucp);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
private:
  QExplicitlySharedDataPointer<SpeciesPrivate> d;
};

class CONTAM_API Ahs
{
public:
  Ahs(int nr=0,int zone_r=0,int zone_s=0,int path_r=0,int path_s=0,int path_x=0,std::string name=std::string(),std::string desc=std::string());
  void read(Reader &input);
  std::string write();
  int nr() const;
  void setNr(const int nr);
  int zone_r() const;
  void setZone_r(const int zone_r);
  int zone_s() const;
  void setZone_s(const int zone_s);
  int path_r() const;
  void setPath_r(const int path_r);
  int path_s() const;
  void setPath_s(const int path_s);
  int path_x() const;
  void setPath_x(const int path_x);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
private:
  QExplicitlySharedDataPointer<AhsPrivate> d;
};

class CONTAM_API Path
{
public:
  enum Flags {
    WIND=0x0001, // Path is subject to wind pressure
    WPC_P=0x0002, // Path uses WPC file pressure
    WPC_C=0x0004, // Path uses WPC file contaminants
    WPC_F=0x0006, // Path uses WPC pressure or contaminants
    AHS_S=0x0008, // Path is a system (supply or return) flow path
    AHS_R=0x0010, // Path is a recirculation flow path
    AHS_O=0x0020, // Path is an outside air flow path
    AHS_X=0x0040, // Path is an exhaust flow path
    AHS_P=0x0078, // Path is any AHS path
    LIM_P=0x0080, // Path is pressure limited
    LIM_F=0x0100, // Path is flow limited
    FAN_F=0x0200}; // Path is a constant flow fan element

    Path(int nr=0,int flags=0,int pzn=0,int pzm=0,int pe=0,int pf=0,int pw=0,int pa=0,int ps=0,int pc=0,int pld=0,RX X=RX_INIT(0),RX Y=RX_INIT(0),RX relHt=RX_INIT(0),RX mult=RX_INIT(0),RX wPset=RX_INIT(0),RX wPmod=RX_INIT(0),RX wazm=RX_INIT(0),RX Fahs=RX_INIT(0),RX Xmax=RX_INIT(0),RX Xmin=RX_INIT(0),unsigned int icon=0,unsigned int dir=0,int u_Ht=0,int u_XY=0,int u_dP=0,int u_F=0,int cfd=0,std::string cfd_name=std::string(),int cfd_ptype=0,int cfd_btype=0,int cfd_capp=0);

    void read(Reader &input);
    std::string write();
    void setWindPressure(bool b);
    bool windPressure();
    void setSystem(bool b);
    bool system();
    void setExhaust(bool b);
    bool exhaust();
    void setRecirculation(bool b);
    bool recirculation();
    void setOutsideAir(bool b);
    bool outsideAir();

    int nr() const;
    void setNr(const int nr);
    int flags() const;
    void setFlags(const int flags);
    int pzn() const;
    void setPzn(const int pzn);
    int pzm() const;
    void setPzm(const int pzm);
    int pe() const;
    void setPe(const int pe);
    int pf() const;
    void setPf(const int pf);
    int pw() const;
    void setPw(const int pw);
    int pa() const;
    void setPa(const int pa);
    int ps() const;
    void setPs(const int ps);
    int pc() const;
    void setPc(const int pc);
    int pld() const;
    void setPld(const int pld);
    RX X() const;
    void setX(const RX X);
    RX Y() const;
    void setY(const RX Y);
    RX relHt() const;
    void setRelHt(const RX relHt);
    RX mult() const;
    void setMult(const RX mult);
    RX wPset() const;
    void setWPset(const RX wPset);
    RX wPmod() const;
    void setWPmod(const RX wPmod);
    RX wazm() const;
    void setWazm(const RX wazm);
    RX Fahs() const;
    void setFahs(const RX Fahs);
    RX Xmax() const;
    void setXmax(const RX Xmax);
    RX Xmin() const;
    void setXmin(const RX Xmin);
    unsigned int icon() const;
    void setIcon(const unsigned int icon);
    unsigned int dir() const;
    void setDir(const unsigned int dir);
    int u_Ht() const;
    void setU_Ht(const int u_Ht);
    int u_XY() const;
    void setU_XY(const int u_XY);
    int u_dP() const;
    void setU_dP(const int u_dP);
    int u_F() const;
    void setU_F(const int u_F);
    int cfd() const;
    void setCfd(const int cfd);
    std::string cfd_name() const;
    void setCfd_name(const std::string cfd_name);
    int cfd_ptype() const;
    void setCfd_ptype(const int cfd_ptype);
    int cfd_btype() const;
    void setCfd_btype(const int cfd_btype);
    int cfd_capp() const;
    void setCfd_capp(const int cfd_capp);

private:
  QExplicitlySharedDataPointer<PathPrivate> d;
};

class CONTAM_API RunControl
{
public:
  RunControl(std::string name=std::string(),std::string version=std::string(),int echo=0,std::string prjdesc=std::string(),int skheight=0,int skwidth=0,int def_units=0,int def_flows=0,RX def_T=RX_INIT(0),int udefT=0,RX rel_N=RX_INIT(0),RX wind_H=RX_INIT(0),int uwH=0,RX wind_Ao=RX_INIT(0),RX wind_a=RX_INIT(0),RX scale=RX_INIT(0),int uScale=0,int orgRow=0,int orgCol=0,int invYaxis=0,int showGeom=0,WeatherData ssWeather=WeatherData(),WeatherData wptWeather=WeatherData(),std::string WTHpath=std::string(),std::string CTMpath=std::string(),std::string CVFpath=std::string(),std::string DVFpath=std::string(),std::string WPCfile=std::string(),std::string EWCfile=std::string(),std::string WPCdesc=std::string(),RX X0=RX_INIT(0),RX Y0=RX_INIT(0),RX Z0=RX_INIT(0),RX angle=RX_INIT(0),int u_XYZ=0,RX epsPath=RX_INIT(0),RX epsSpcs=RX_INIT(0),std::string tShift=std::string(),std::string dStart=std::string(),std::string dEnd=std::string(),int useWPCwp=0,int useWPCmf=0,int wpctrig=0,RX latd=RX_INIT(0),RX lgtd=RX_INIT(0),RX Tznr=RX_INIT(0),RX altd=RX_INIT(0),RX Tgrnd=RX_INIT(0),int utg=0,int u_a=0,int sim_af=0,int afcalc=0,int afmaxi=0,RX afrcnvg=RX_INIT(0),RX afacnvg=RX_INIT(0),RX afrelax=RX_INIT(0),int uac2=0,RX Pres=RX_INIT(0),int uPres=0,int afslae=0,int afrseq=0,int aflmaxi=0,RX aflcnvg=RX_INIT(0),int aflinit=0,int Tadj=0,int sim_mf=0,int ccmaxi=0,RX ccrcnvg=RX_INIT(0),RX ccacnvg=RX_INIT(0),RX ccrelax=RX_INIT(0),int uccc=0,int mfnmthd=0,int mfnrseq=0,int mfnmaxi=0,RX mfnrcnvg=RX_INIT(0),RX mfnacnvg=RX_INIT(0),RX mfnrelax=RX_INIT(0),RX mfngamma=RX_INIT(0),int uccn=0,int mftmthd=0,int mftrseq=0,int mftmaxi=0,RX mftrcnvg=RX_INIT(0),RX mftacnvg=RX_INIT(0),RX mftrelax=RX_INIT(0),RX mftgamma=RX_INIT(0),int ucct=0,int mfvmthd=0,int mfvrseq=0,int mfvmaxi=0,RX mfvrcnvg=RX_INIT(0),RX mfvacnvg=RX_INIT(0),RX mfvrelax=RX_INIT(0),int uccv=0,int mf_solver=0,int sim_1dz=0,int sim_1dd=0,RX celldx=RX_INIT(0),int sim_vjt=0,int udx=0,int cvode_mth=0,RX cvode_rcnvg=RX_INIT(0),RX cvode_acnvg=RX_INIT(0),RX cvode_dtmax=RX_INIT(0),int tsdens=0,RX tsrelax=RX_INIT(0),int tsmaxi=0,int cnvgSS=0,int densZP=0,int stackD=0,int dodMdt=0,std::string date_st=std::string(),std::string time_st=std::string(),std::string date_0=std::string(),std::string time_0=std::string(),std::string date_1=std::string(),std::string time_1=std::string(),std::string time_step=std::string(),std::string time_list=std::string(),std::string time_scrn=std::string(),int restart=0,std::string rstdate=std::string(),std::string rsttime=std::string(),int list=0,int doDlg=0,int pfsave=0,int zfsave=0,int zcsave=0,int achvol=0,int achsave=0,int abwsave=0,int cbwsave=0,int expsave=0,int ebwsave=0,int zaasave=0,int zbwsave=0,int rzfsave=0,int rzmsave=0,int rz1save=0,int csmsave=0,int srfsave=0,int logsave=0,std::vector<int> save=std::vector<int>(),std::vector<RX> rvals=std::vector<RX>(),int BldgFlowZ=0,int BldgFlowD=0,int BldgFlowC=0,int cfd_ctype=0,RX cfd_convcpl=RX_INIT(0),int cfd_var=0,int cfd_zref=0,int cfd_imax=0,int cfd_dtcmo=0);
  void read(Reader &input);
  std::string write();
  std::string name() const;
  void setName(const std::string name);
  std::string version() const;
  void setVersion(const std::string version);
  int echo() const;
  void setEcho(const int echo);
  std::string prjdesc() const;
  void setPrjdesc(const std::string prjdesc);
  int skheight() const;
  void setSkheight(const int skheight);
  int skwidth() const;
  void setSkwidth(const int skwidth);
  int def_units() const;
  void setDef_units(const int def_units);
  int def_flows() const;
  void setDef_flows(const int def_flows);
  RX def_T() const;
  void setDef_T(const RX def_T);
  int udefT() const;
  void setUdefT(const int udefT);
  RX rel_N() const;
  void setRel_N(const RX rel_N);
  RX wind_H() const;
  void setWind_H(const RX wind_H);
  int uwH() const;
  void setUwH(const int uwH);
  RX wind_Ao() const;
  void setWind_Ao(const RX wind_Ao);
  RX wind_a() const;
  void setWind_a(const RX wind_a);
  RX scale() const;
  void setScale(const RX scale);
  int uScale() const;
  void setUScale(const int uScale);
  int orgRow() const;
  void setOrgRow(const int orgRow);
  int orgCol() const;
  void setOrgCol(const int orgCol);
  int invYaxis() const;
  void setInvYaxis(const int invYaxis);
  int showGeom() const;
  void setShowGeom(const int showGeom);
  WeatherData ssWeather() const;
  void setSsWeather(const WeatherData ssWeather);
  WeatherData wptWeather() const;
  void setWptWeather(const WeatherData wptWeather);
  std::string WTHpath() const;
  void setWTHpath(const std::string WTHpath);
  std::string CTMpath() const;
  void setCTMpath(const std::string CTMpath);
  std::string CVFpath() const;
  void setCVFpath(const std::string CVFpath);
  std::string DVFpath() const;
  void setDVFpath(const std::string DVFpath);
  std::string WPCfile() const;
  void setWPCfile(const std::string WPCfile);
  std::string EWCfile() const;
  void setEWCfile(const std::string EWCfile);
  std::string WPCdesc() const;
  void setWPCdesc(const std::string WPCdesc);
  RX X0() const;
  void setX0(const RX X0);
  RX Y0() const;
  void setY0(const RX Y0);
  RX Z0() const;
  void setZ0(const RX Z0);
  RX angle() const;
  void setAngle(const RX angle);
  int u_XYZ() const;
  void setU_XYZ(const int u_XYZ);
  RX epsPath() const;
  void setEpsPath(const RX epsPath);
  RX epsSpcs() const;
  void setEpsSpcs(const RX epsSpcs);
  std::string tShift() const;
  void setTShift(const std::string tShift);
  std::string dStart() const;
  void setDStart(const std::string dStart);
  std::string dEnd() const;
  void setDEnd(const std::string dEnd);
  int useWPCwp() const;
  void setUseWPCwp(const int useWPCwp);
  int useWPCmf() const;
  void setUseWPCmf(const int useWPCmf);
  int wpctrig() const;
  void setWpctrig(const int wpctrig);
  RX latd() const;
  void setLatd(const RX latd);
  RX lgtd() const;
  void setLgtd(const RX lgtd);
  RX Tznr() const;
  void setTznr(const RX Tznr);
  RX altd() const;
  void setAltd(const RX altd);
  RX Tgrnd() const;
  void setTgrnd(const RX Tgrnd);
  int utg() const;
  void setUtg(const int utg);
  int u_a() const;
  void setU_a(const int u_a);
  int sim_af() const;
  void setSim_af(const int sim_af);
  int afcalc() const;
  void setAfcalc(const int afcalc);
  int afmaxi() const;
  void setAfmaxi(const int afmaxi);
  RX afrcnvg() const;
  void setAfrcnvg(const RX afrcnvg);
  RX afacnvg() const;
  void setAfacnvg(const RX afacnvg);
  RX afrelax() const;
  void setAfrelax(const RX afrelax);
  int uac2() const;
  void setUac2(const int uac2);
  RX Pres() const;
  void setPres(const RX Pres);
  int uPres() const;
  void setUPres(const int uPres);
  int afslae() const;
  void setAfslae(const int afslae);
  int afrseq() const;
  void setAfrseq(const int afrseq);
  int aflmaxi() const;
  void setAflmaxi(const int aflmaxi);
  RX aflcnvg() const;
  void setAflcnvg(const RX aflcnvg);
  int aflinit() const;
  void setAflinit(const int aflinit);
  int Tadj() const;
  void setTadj(const int Tadj);
  int sim_mf() const;
  void setSim_mf(const int sim_mf);
  int ccmaxi() const;
  void setCcmaxi(const int ccmaxi);
  RX ccrcnvg() const;
  void setCcrcnvg(const RX ccrcnvg);
  RX ccacnvg() const;
  void setCcacnvg(const RX ccacnvg);
  RX ccrelax() const;
  void setCcrelax(const RX ccrelax);
  int uccc() const;
  void setUccc(const int uccc);
  int mfnmthd() const;
  void setMfnmthd(const int mfnmthd);
  int mfnrseq() const;
  void setMfnrseq(const int mfnrseq);
  int mfnmaxi() const;
  void setMfnmaxi(const int mfnmaxi);
  RX mfnrcnvg() const;
  void setMfnrcnvg(const RX mfnrcnvg);
  RX mfnacnvg() const;
  void setMfnacnvg(const RX mfnacnvg);
  RX mfnrelax() const;
  void setMfnrelax(const RX mfnrelax);
  RX mfngamma() const;
  void setMfngamma(const RX mfngamma);
  int uccn() const;
  void setUccn(const int uccn);
  int mftmthd() const;
  void setMftmthd(const int mftmthd);
  int mftrseq() const;
  void setMftrseq(const int mftrseq);
  int mftmaxi() const;
  void setMftmaxi(const int mftmaxi);
  RX mftrcnvg() const;
  void setMftrcnvg(const RX mftrcnvg);
  RX mftacnvg() const;
  void setMftacnvg(const RX mftacnvg);
  RX mftrelax() const;
  void setMftrelax(const RX mftrelax);
  RX mftgamma() const;
  void setMftgamma(const RX mftgamma);
  int ucct() const;
  void setUcct(const int ucct);
  int mfvmthd() const;
  void setMfvmthd(const int mfvmthd);
  int mfvrseq() const;
  void setMfvrseq(const int mfvrseq);
  int mfvmaxi() const;
  void setMfvmaxi(const int mfvmaxi);
  RX mfvrcnvg() const;
  void setMfvrcnvg(const RX mfvrcnvg);
  RX mfvacnvg() const;
  void setMfvacnvg(const RX mfvacnvg);
  RX mfvrelax() const;
  void setMfvrelax(const RX mfvrelax);
  int uccv() const;
  void setUccv(const int uccv);
  int mf_solver() const;
  void setMf_solver(const int mf_solver);
  int sim_1dz() const;
  void setSim_1dz(const int sim_1dz);
  int sim_1dd() const;
  void setSim_1dd(const int sim_1dd);
  RX celldx() const;
  void setCelldx(const RX celldx);
  int sim_vjt() const;
  void setSim_vjt(const int sim_vjt);
  int udx() const;
  void setUdx(const int udx);
  int cvode_mth() const;
  void setCvode_mth(const int cvode_mth);
  RX cvode_rcnvg() const;
  void setCvode_rcnvg(const RX cvode_rcnvg);
  RX cvode_acnvg() const;
  void setCvode_acnvg(const RX cvode_acnvg);
  RX cvode_dtmax() const;
  void setCvode_dtmax(const RX cvode_dtmax);
  int tsdens() const;
  void setTsdens(const int tsdens);
  RX tsrelax() const;
  void setTsrelax(const RX tsrelax);
  int tsmaxi() const;
  void setTsmaxi(const int tsmaxi);
  int cnvgSS() const;
  void setCnvgSS(const int cnvgSS);
  int densZP() const;
  void setDensZP(const int densZP);
  int stackD() const;
  void setStackD(const int stackD);
  int dodMdt() const;
  void setDodMdt(const int dodMdt);
  std::string date_st() const;
  void setDate_st(const std::string date_st);
  std::string time_st() const;
  void setTime_st(const std::string time_st);
  std::string date_0() const;
  void setDate_0(const std::string date_0);
  std::string time_0() const;
  void setTime_0(const std::string time_0);
  std::string date_1() const;
  void setDate_1(const std::string date_1);
  std::string time_1() const;
  void setTime_1(const std::string time_1);
  std::string time_step() const;
  void setTime_step(const std::string time_step);
  std::string time_list() const;
  void setTime_list(const std::string time_list);
  std::string time_scrn() const;
  void setTime_scrn(const std::string time_scrn);
  int restart() const;
  void setRestart(const int restart);
  std::string rstdate() const;
  void setRstdate(const std::string rstdate);
  std::string rsttime() const;
  void setRsttime(const std::string rsttime);
  int list() const;
  void setList(const int list);
  int doDlg() const;
  void setDoDlg(const int doDlg);
  int pfsave() const;
  void setPfsave(const int pfsave);
  int zfsave() const;
  void setZfsave(const int zfsave);
  int zcsave() const;
  void setZcsave(const int zcsave);
  int achvol() const;
  void setAchvol(const int achvol);
  int achsave() const;
  void setAchsave(const int achsave);
  int abwsave() const;
  void setAbwsave(const int abwsave);
  int cbwsave() const;
  void setCbwsave(const int cbwsave);
  int expsave() const;
  void setExpsave(const int expsave);
  int ebwsave() const;
  void setEbwsave(const int ebwsave);
  int zaasave() const;
  void setZaasave(const int zaasave);
  int zbwsave() const;
  void setZbwsave(const int zbwsave);
  int rzfsave() const;
  void setRzfsave(const int rzfsave);
  int rzmsave() const;
  void setRzmsave(const int rzmsave);
  int rz1save() const;
  void setRz1save(const int rz1save);
  int csmsave() const;
  void setCsmsave(const int csmsave);
  int srfsave() const;
  void setSrfsave(const int srfsave);
  int logsave() const;
  void setLogsave(const int logsave);
  std::vector<int> save() const;
  void setSave(const std::vector<int> save);
  std::vector<RX> rvals() const;
  void setRvals(const std::vector<RX> rvals);
  int BldgFlowZ() const;
  void setBldgFlowZ(const int BldgFlowZ);
  int BldgFlowD() const;
  void setBldgFlowD(const int BldgFlowD);
  int BldgFlowC() const;
  void setBldgFlowC(const int BldgFlowC);
  int cfd_ctype() const;
  void setCfd_ctype(const int cfd_ctype);
  RX cfd_convcpl() const;
  void setCfd_convcpl(const RX cfd_convcpl);
  int cfd_var() const;
  void setCfd_var(const int cfd_var);
  int cfd_zref() const;
  void setCfd_zref(const int cfd_zref);
  int cfd_imax() const;
  void setCfd_imax(const int cfd_imax);
  int cfd_dtcmo() const;
  void setCfd_dtcmo(const int cfd_dtcmo);
private:
  QExplicitlySharedDataPointer<RunControlPrivate> d;
};

class CONTAM_API Level
{
public:
  Level(int nr=0,RX refht=RX_INIT(0),RX delht=RX_INIT(0),int u_rfht=0,int u_dlht=0,std::string name=std::string(),std::vector<Icon> icons=std::vector<Icon>());
  void read(Reader &input);
  std::string write();
  int nr() const;
  void setNr(const int nr);
  RX refht() const;
  void setRefht(const RX refht);
  RX delht() const;
  void setDelht(const RX delht);
  int u_rfht() const;
  void setU_rfht(const int u_rfht);
  int u_dlht() const;
  void setU_dlht(const int u_dlht);
  std::string name() const;
  void setName(const std::string name);
  std::vector<Icon> icons() const;
  void setIcons(const std::vector<Icon> icons);
private:
  QExplicitlySharedDataPointer<LevelPrivate> d;
};

class CONTAM_API DaySchedule
{
public:
  DaySchedule(int nr=0,int shape=0,int utyp=0,int ucnv=0,std::string name=std::string(),std::string desc=std::string(),std::vector<SchedulePoint> points=std::vector<SchedulePoint>());
  void read(Reader &input);
  std::string write();
  int nr() const;
  void setNr(const int nr);
  int shape() const;
  void setShape(const int shape);
  int utyp() const;
  void setUtyp(const int utyp);
  int ucnv() const;
  void setUcnv(const int ucnv);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  std::vector<SchedulePoint> points() const;
  void setPoints(const std::vector<SchedulePoint> points);
private:
  QExplicitlySharedDataPointer<DaySchedulePrivate> d;
};

class CONTAM_API WeekSchedule
{
public:
  WeekSchedule(int nr=0,int utyp=0,int ucnv=0,std::string name=std::string(),std::string desc=std::string(),std::vector<int> j=std::vector<int>());
  void read(Reader &input);
  std::string write();
  int nr() const;
  void setNr(const int nr);
  int utyp() const;
  void setUtyp(const int utyp);
  int ucnv() const;
  void setUcnv(const int ucnv);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  std::vector<int> j() const;
  void setJ(const std::vector<int> j);
private:
  QExplicitlySharedDataPointer<WeekSchedulePrivate> d;
};

class CONTAM_API WindPressureProfile
{
public:
  WindPressureProfile(int nr=0,int type=0,std::string name=std::string(),std::string desc=std::string(),std::vector<PressureCoefficientPoint> coeffs=std::vector<PressureCoefficientPoint>());
  void read(Reader &input);
  std::string write();
  int nr() const;
  void setNr(const int nr);
  int type() const;
  void setType(const int type);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  std::vector<PressureCoefficientPoint> coeffs() const;
  void setCoeffs(const std::vector<PressureCoefficientPoint> coeffs);
private:
  QExplicitlySharedDataPointer<WindPressureProfilePrivate> d;
};

class CONTAM_API ControlNode
{
public:
  enum Type {CT_SNS=0,CT_SCH=1,CT_SET=2,CT_CVF=3,CT_DVF=4,CT_LOG=5,CT_PAS=6,CT_MOD=7,CT_HYS=8,CT_ABS=9,CT_BIN=10,CT_DLS=11,CT_DLX=12,CT_INT=13,CT_RAV=14,CT_INV=15,CT_AND=16,CT_OR=17,CT_XOR=18,CT_ADD=19,CT_SUB=20,CT_MUL=21,CT_DIV=22,CT_SUM=23,CT_AVG=24,CT_MAX=25,CT_MIN=26,CT_LLS=27,CT_ULS=28,CT_LBS=29,CT_UBS=30,CT_LLC=31,CT_ULC=32,CT_PC1=33,CT_PI1=34,CT_SUP=35,CT_SPH=37,UNKNOWN};
  virtual ~ControlNode(){}
  virtual std::string write()=0;
  static Type convertTag(std::string string);
  static ControlNode* readElement(Reader &input);
  virtual void recompute(){}
  virtual int nr() const = 0;
  virtual void setNr(const int nr) = 0;
  virtual std::string dataType() const = 0;
  virtual int seqnr() const = 0;
  virtual void setSeqnr(const int seqnr) = 0;
  virtual unsigned int flags() const = 0;
  virtual void setFlags(const unsigned int flags) = 0;
  virtual int inreq() const = 0;
  virtual void setInreq(const int inreq) = 0;
  virtual int n1() const = 0;
  virtual void setN1(const int n1) = 0;
  virtual int n2() const = 0;
  virtual void setN2(const int n2) = 0;
  virtual std::string name() const = 0;
  virtual void setName(const std::string name) = 0;
  virtual std::string desc() const = 0;
  virtual void setDesc(const std::string desc) = 0;
};

class CONTAM_API CtrlDat : public ControlNode
{
public:
  CtrlDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string());
  void read(Reader &input);
  std::string write();
  int nr() const;
  void setNr(const int nr);
  virtual std::string dataType() const=0;
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
private:
  QExplicitlySharedDataPointer<CtrlDatPrivate> d;
};

class CONTAM_API PasDat : public CtrlDat
{
public:
  PasDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "pas";}
};

class CONTAM_API AbsDat : public CtrlDat
{
public:
  AbsDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "abs";}
};

class CONTAM_API BinDat : public CtrlDat
{
public:
  BinDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "bin";}
};

class CONTAM_API IntDat : public CtrlDat
{
public:
  IntDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "int";}
};

class CONTAM_API InvDat : public CtrlDat
{
public:
  InvDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "inv";}
};

class CONTAM_API AndDat : public CtrlDat
{
public:
  AndDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "and";}
};

class CONTAM_API OrDat : public CtrlDat
{
public:
  OrDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "od";}
};

class CONTAM_API XorDat : public CtrlDat
{
public:
  XorDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "xor";}
};

class CONTAM_API AddDat : public CtrlDat
{
public:
  AddDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "add";}
};

class CONTAM_API SubDat : public CtrlDat
{
public:
  SubDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "sub";}
};

class CONTAM_API MulDat : public CtrlDat
{
public:
  MulDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "mul";}
};

class CONTAM_API DivDat : public CtrlDat
{
public:
  DivDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "div";}
};

class CONTAM_API LlsDat : public CtrlDat
{
public:
  LlsDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "lls";}
};

class CONTAM_API UlsDat : public CtrlDat
{
public:
  UlsDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "uls";}
};

class CONTAM_API LlcDat : public CtrlDat
{
public:
  LlcDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "llc";}
};

class CONTAM_API UlcDat : public CtrlDat
{
public:
  UlcDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "ulc";}
};

class CONTAM_API SphDat : public CtrlDat
{
public:
  SphDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string()) : CtrlDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
  {}
  std::string dataType() const {return "sph";}
};

class CONTAM_API SnsDat : public ControlNode
{
public:
  SnsDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX offset=RX_INIT(0),RX scale=RX_INIT(0),RX tau=RX_INIT(0),RX oldsig=RX_INIT(0),int source=0,int type=0,int measure=0,RX X=RX_INIT(0),RX Y=RX_INIT(0),RX relHt=RX_INIT(0),std::string units=std::string(),std::string species=std::string());
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "sns";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX offset() const;
  void setOffset(const RX offset);
  RX scale() const;
  void setScale(const RX scale);
  RX tau() const;
  void setTau(const RX tau);
  RX oldsig() const;
  void setOldsig(const RX oldsig);
  int source() const;
  void setSource(const int source);
  int type() const;
  void setType(const int type);
  int measure() const;
  void setMeasure(const int measure);
  RX X() const;
  void setX(const RX X);
  RX Y() const;
  void setY(const RX Y);
  RX relHt() const;
  void setRelHt(const RX relHt);
  std::string units() const;
  void setUnits(const std::string units);
  std::string species() const;
  void setSpecies(const std::string species);
private:
  QExplicitlySharedDataPointer<SnsDatPrivate> d;
};

class CONTAM_API SchDat : public ControlNode
{
public:
  SchDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),int ps=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "sch";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  int ps() const;
  void setPs(const int ps);
private:
  QExplicitlySharedDataPointer<SchDatPrivate> d;
};

class CONTAM_API SetDat : public ControlNode
{
public:
  SetDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX value=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "set";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX value() const;
  void setValue(const RX value);
private:
  QExplicitlySharedDataPointer<SetDatPrivate> d;
};

class CONTAM_API CdvDat : public ControlNode
{
public:
  CdvDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),std::string valuename=std::string());
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  virtual std::string dataType() const=0;
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  std::string valuename() const;
  void setValuename(const std::string valuename);
private:
  QExplicitlySharedDataPointer<CdvDatPrivate> d;
};

class CONTAM_API CvfDat : public CdvDat
{
public:
  CvfDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string(),std::string valuename=std::string()) : CdvDat(nr,seqnr,flags,inreq,n1,n2,name,
    desc,valuename)
  {}
  std::string dataType() const {return "cvf";}

};

class CONTAM_API DvfDat : public CdvDat
{
public:
  DvfDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string(),std::string valuename=std::string()) : CdvDat(nr,seqnr,flags,inreq,n1,n2,name,
    desc,valuename)
  {}
  std::string dataType() const {return "dvf";}

};

class CONTAM_API LogDat : public ControlNode
{
public:
  LogDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX offset=RX_INIT(0),RX scale=RX_INIT(0),int udef=0,std::string header=std::string(),std::string units=std::string());
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "log";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX offset() const;
  void setOffset(const RX offset);
  RX scale() const;
  void setScale(const RX scale);
  int udef() const;
  void setUdef(const int udef);
  std::string header() const;
  void setHeader(const std::string header);
  std::string units() const;
  void setUnits(const std::string units);
private:
  QExplicitlySharedDataPointer<LogDatPrivate> d;
};

class CONTAM_API ModDat : public ControlNode
{
public:
  ModDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX offset=RX_INIT(0),RX scale=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "mod";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX offset() const;
  void setOffset(const RX offset);
  RX scale() const;
  void setScale(const RX scale);
private:
  QExplicitlySharedDataPointer<ModDatPrivate> d;
};

class CONTAM_API HysDat : public ControlNode
{
public:
  HysDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX slack=RX_INIT(0),RX slope=RX_INIT(0),RX oldsig=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "hys";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX slack() const;
  void setSlack(const RX slack);
  RX slope() const;
  void setSlope(const RX slope);
  RX oldsig() const;
  void setOldsig(const RX oldsig);
private:
  QExplicitlySharedDataPointer<HysDatPrivate> d;
};

class CONTAM_API DlsDat : public ControlNode
{
public:
  DlsDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),int dsincr=0,int dsdecr=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "dls";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  int dsincr() const;
  void setDsincr(const int dsincr);
  int dsdecr() const;
  void setDsdecr(const int dsdecr);
private:
  QExplicitlySharedDataPointer<DlsDatPrivate> d;
};

class CONTAM_API DlxDat : public ControlNode
{
public:
  DlxDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),int tauincr=0,int taudecr=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const{return "dlx";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  int tauincr() const;
  void setTauincr(const int tauincr);
  int taudecr() const;
  void setTaudecr(const int taudecr);
private:
  QExplicitlySharedDataPointer<DlxDatPrivate> d;
};

class CONTAM_API RavDat : public ControlNode
{
public:
  RavDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),int tspan=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "rav";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  int tspan() const;
  void setTspan(const int tspan);
private:
  QExplicitlySharedDataPointer<RavDatPrivate> d;
};

class CONTAM_API SumAvg : public ControlNode
{
public:
  SumAvg(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),std::vector<int> pc=std::vector<int>());
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  virtual std::string dataType() const {return "sum";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  std::vector<int> pc() const;
  void setPc(const std::vector<int> pc);
private:
  QExplicitlySharedDataPointer<SumAvgPrivate> d;
};

class CONTAM_API AvgDat : public SumAvg
{
public:
  AvgDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string(),std::vector<int> pc=std::vector<int>()) : SumAvg(nr,seqnr,flags,inreq,n1,
    n2,name,desc,pc)
  {}
  std::string dataType() const {return "avg";}
};

class CONTAM_API MaxDat : public SumAvg
{
public:
  MaxDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string(),std::vector<int> pc=std::vector<int>()) : SumAvg(nr,seqnr,flags,inreq,n1,
    n2,name,desc,pc)
  {}
  std::string dataType() const {return "max";}
};

class CONTAM_API MinDat : public SumAvg
{
public:
  MinDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string(),std::vector<int> pc=std::vector<int>()) : SumAvg(nr,seqnr,flags,inreq,n1,
    n2,name,desc,pc)
  {}
  std::string dataType() const {return "min";}
};

class CONTAM_API BanDat : public ControlNode
{
public:
  BanDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX band=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  virtual std::string dataType() const=0;
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX band() const;
  void setBand(const RX band);
private:
  QExplicitlySharedDataPointer<BanDatPrivate> d;
};

class CONTAM_API LbsDat : public BanDat
{
public:
  LbsDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string(),RX band=RX_INIT(0)) : BanDat(nr,seqnr,flags,inreq,n1,n2,name,desc,band)
  {}
  std::string dataType() const {return "lbs";}
};

class CONTAM_API UbsDat : public BanDat
{
public:
  UbsDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),
    std::string desc=std::string(),RX band=RX_INIT(0)) : BanDat(nr,seqnr,flags,inreq,n1,n2,name,desc,band)
  {}
  virtual std::string dataType() const {return "ubs";}
};

class CONTAM_API PcDat : public ControlNode
{
public:
  PcDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX kp=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "pc1";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX kp() const;
  void setKp(const RX kp);
private:
  QExplicitlySharedDataPointer<PcDatPrivate> d;
};

class CONTAM_API PicDat : public ControlNode
{
public:
  PicDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),RX kp=RX_INIT(0),RX ki=RX_INIT(0),RX oldsig=RX_INIT(0),RX olderr=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "pi1";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX kp() const;
  void setKp(const RX kp);
  RX ki() const;
  void setKi(const RX ki);
  RX oldsig() const;
  void setOldsig(const RX oldsig);
  RX olderr() const;
  void setOlderr(const RX olderr);
private:
  QExplicitlySharedDataPointer<PicDatPrivate> d;
};

class CONTAM_API SupDat : public ControlNode
{
public:
  SupDat(int nr=0,int seqnr=0,unsigned int flags=0,int inreq=0,int n1=0,int n2=0,std::string name=std::string(),std::string desc=std::string(),int def=0,int se=0,int in=0,int out=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  std::string dataType() const {return "sup";}
  int seqnr() const;
  void setSeqnr(const int seqnr);
  unsigned int flags() const;
  void setFlags(const unsigned int flags);
  int inreq() const;
  void setInreq(const int inreq);
  int n1() const;
  void setN1(const int n1);
  int n2() const;
  void setN2(const int n2);
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  int def() const;
  void setDef(const int def);
  int se() const;
  void setSe(const int se);
  int in() const;
  void setIn(const int in);
  int out() const;
  void setOut(const int out);
private:
  QExplicitlySharedDataPointer<SupDatPrivate> d;
};

class CONTAM_API AirflowElement
{
public:
  enum Type {PL_ORFC=0,PL_LEAK1=1,PL_LEAK2=2,PL_LEAK3=3,PL_CONN=4,PL_QCN=5,PL_FCN=6,PL_TEST1=7,PL_TEST2=8,PL_CRACK=9,PL_STAIR=10,PL_SHAFT=11,PL_BDQ=12,PL_BDF=13,QFR_QAB=14,QFR_QAF=15,QFR_CRACK=16,QFR_TEST2=17,DR_DOOR=18,DR_PL2=19,FN_CMF=20,FN_CVF=21,FN_FAN=22,CS_FSP=23,CS_QSP=24,CS_PSF=25,CS_PSQ=26,AF_SUP=27,UNKNOWN};
  virtual ~AirflowElement(){}
  virtual std::string write()=0;
  static Type convertTag(std::string string);
  static AirflowElement* readElement(Reader &input);
  virtual void recompute(){}
  virtual int nr() const = 0;
  virtual void setNr(const int nr) = 0;
  virtual int icon() const = 0;
  virtual void setIcon(const int icon) = 0;
  virtual std::string dataType() const = 0;
  //    virtual void setDataType(const std::string dataType) = 0;
  virtual std::string name() const = 0;
  virtual void setName(const std::string name) = 0;
  virtual std::string desc() const = 0;
  virtual void setDesc(const std::string desc) = 0;
};

class CONTAM_API PlrOrf : public AirflowElement
{
public:
  PlrOrf(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX area=RX_INIT(0),RX dia=RX_INIT(0),RX coef=RX_INIT(0),RX Re=RX_INIT(0),int u_A=0,int u_D=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_orfc";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX area() const;
  void setArea(const RX area);
  RX dia() const;
  void setDia(const RX dia);
  RX coef() const;
  void setCoef(const RX coef);
  RX Re() const;
  void setRe(const RX Re);
  int u_A() const;
  void setU_A(const int u_A);
  int u_D() const;
  void setU_D(const int u_D);
private:
  QExplicitlySharedDataPointer<PlrOrfPrivate> d;
};

class CONTAM_API PlrLeak : public AirflowElement
{
public:
  PlrLeak(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX coef=RX_INIT(0),RX pres=RX_INIT(0),RX area1=RX_INIT(0),RX area2=RX_INIT(0),RX area3=RX_INIT(0),int u_A1=0,int u_A2=0,int u_A3=0,int u_dP=0);
  void read(Reader &input);
  virtual std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  virtual std::string dataType() const = 0;
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX coef() const;
  void setCoef(const RX coef);
  RX pres() const;
  void setPres(const RX pres);
  RX area1() const;
  void setArea1(const RX area1);
  RX area2() const;
  void setArea2(const RX area2);
  RX area3() const;
  void setArea3(const RX area3);
  int u_A1() const;
  void setU_A1(const int u_A1);
  int u_A2() const;
  void setU_A2(const int u_A2);
  int u_A3() const;
  void setU_A3(const int u_A3);
  int u_dP() const;
  void setU_dP(const int u_dP);
private:
  QExplicitlySharedDataPointer<PlrLeakPrivate> d;
};

class CONTAM_API PlrLeak1 : public PlrLeak
{
public:
  PlrLeak1(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),
    RX expt=RX_INIT(0),RX coef=RX_INIT(0),RX pres=RX_INIT(0),RX area1=RX_INIT(0),RX area2=RX_INIT(0),RX area3=RX_INIT(0),
    int u_A1=0,int u_A2=0,int u_A3=0,int u_dP=0) : PlrLeak(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,
    area3,u_A1,u_A2,u_A3,u_dP)
  {}
  std::string dataType() const {return "plr_leak1";}
};

class CONTAM_API PlrLeak2 : public PlrLeak
{
public:
  PlrLeak2(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),
    RX expt=RX_INIT(0),RX coef=RX_INIT(0),RX pres=RX_INIT(0),RX area1=RX_INIT(0),RX area2=RX_INIT(0),RX area3=RX_INIT(0),
    int u_A1=0,int u_A2=0,int u_A3=0,int u_dP=0) : PlrLeak(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,
    area3,u_A1,u_A2,u_A3,u_dP)
  {}
  std::string dataType() const {return "plr_leak2";}
};

class CONTAM_API PlrLeak3 : public PlrLeak
{
public:
  PlrLeak3(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),
    RX expt=RX_INIT(0),RX coef=RX_INIT(0),RX pres=RX_INIT(0),RX area1=RX_INIT(0),RX area2=RX_INIT(0),RX area3=RX_INIT(0),
    int u_A1=0,int u_A2=0,int u_A3=0,int u_dP=0) : PlrLeak(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,
    area3,u_A1,u_A2,u_A3,u_dP)
  {}
  std::string dataType() const {return "plr_leak3";}
};

class CONTAM_API PlrConn : public AirflowElement
{
public:
  PlrConn(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX area=RX_INIT(0),RX coef=RX_INIT(0),int u_A=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_conn";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX area() const;
  void setArea(const RX area);
  RX coef() const;
  void setCoef(const RX coef);
  int u_A() const;
  void setU_A(const int u_A);
private:
  QExplicitlySharedDataPointer<PlrConnPrivate> d;
};

class CONTAM_API PlrQcn : public AirflowElement
{
public:
  PlrQcn(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_qcn";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
private:
  QExplicitlySharedDataPointer<PlrQcnPrivate> d;
};

class CONTAM_API PlrFcn : public AirflowElement
{
public:
  PlrFcn(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_fcn";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
private:
  QExplicitlySharedDataPointer<PlrFcnPrivate> d;
};

class CONTAM_API PlrTest1 : public AirflowElement
{
public:
  PlrTest1(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX dP=RX_INIT(0),RX Flow=RX_INIT(0),int u_P=0,int u_F=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_test1";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX dP() const;
  void setDP(const RX dP);
  RX Flow() const;
  void setFlow(const RX Flow);
  int u_P() const;
  void setU_P(const int u_P);
  int u_F() const;
  void setU_F(const int u_F);
private:
  QExplicitlySharedDataPointer<PlrTest1Private> d;
};

class CONTAM_API PlrTest2 : public AirflowElement
{
public:
  PlrTest2(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX dP1=RX_INIT(0),RX F1=RX_INIT(0),RX dP2=RX_INIT(0),RX F2=RX_INIT(0),int u_P1=0,int u_F1=0,int u_P2=0,int u_F2=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_test2";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX dP1() const;
  void setDP1(const RX dP1);
  RX F1() const;
  void setF1(const RX F1);
  RX dP2() const;
  void setDP2(const RX dP2);
  RX F2() const;
  void setF2(const RX F2);
  int u_P1() const;
  void setU_P1(const int u_P1);
  int u_F1() const;
  void setU_F1(const int u_F1);
  int u_P2() const;
  void setU_P2(const int u_P2);
  int u_F2() const;
  void setU_F2(const int u_F2);
private:
  QExplicitlySharedDataPointer<PlrTest2Private> d;
};

class CONTAM_API PlrCrack : public AirflowElement
{
public:
  PlrCrack(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX length=RX_INIT(0),RX width=RX_INIT(0),int u_L=0,int u_W=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_crack";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX length() const;
  void setLength(const RX length);
  RX width() const;
  void setWidth(const RX width);
  int u_L() const;
  void setU_L(const int u_L);
  int u_W() const;
  void setU_W(const int u_W);
private:
  QExplicitlySharedDataPointer<PlrCrackPrivate> d;
};

class CONTAM_API PlrStair : public AirflowElement
{
public:
  PlrStair(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX Ht=RX_INIT(0),RX Area=RX_INIT(0),RX peo=RX_INIT(0),int tread=0,int u_A=0,int u_D=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_stair";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX Ht() const;
  void setHt(const RX Ht);
  RX Area() const;
  void setArea(const RX Area);
  RX peo() const;
  void setPeo(const RX peo);
  int tread() const;
  void setTread(const int tread);
  int u_A() const;
  void setU_A(const int u_A);
  int u_D() const;
  void setU_D(const int u_D);
private:
  QExplicitlySharedDataPointer<PlrStairPrivate> d;
};

class CONTAM_API PlrShaft : public AirflowElement
{
public:
  PlrShaft(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX Ht=RX_INIT(0),RX area=RX_INIT(0),RX perim=RX_INIT(0),RX rough=RX_INIT(0),int u_A=0,int u_D=0,int u_P=0,int u_R=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_shaft";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX Ht() const;
  void setHt(const RX Ht);
  RX area() const;
  void setArea(const RX area);
  RX perim() const;
  void setPerim(const RX perim);
  RX rough() const;
  void setRough(const RX rough);
  int u_A() const;
  void setU_A(const int u_A);
  int u_D() const;
  void setU_D(const int u_D);
  int u_P() const;
  void setU_P(const int u_P);
  int u_R() const;
  void setU_R(const int u_R);
private:
  QExplicitlySharedDataPointer<PlrShaftPrivate> d;
};

class CONTAM_API PlrBdq : public AirflowElement
{
public:
  PlrBdq(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX Cp=RX_INIT(0),RX xp=RX_INIT(0),RX Cn=RX_INIT(0),RX xn=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_bdq";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX Cp() const;
  void setCp(const RX Cp);
  RX xp() const;
  void setXp(const RX xp);
  RX Cn() const;
  void setCn(const RX Cn);
  RX xn() const;
  void setXn(const RX xn);
private:
  QExplicitlySharedDataPointer<PlrBdqPrivate> d;
};

class CONTAM_API PlrBdf : public AirflowElement
{
public:
  PlrBdf(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX Cp=RX_INIT(0),RX xp=RX_INIT(0),RX Cn=RX_INIT(0),RX xn=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "plr_bdf";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX Cp() const;
  void setCp(const RX Cp);
  RX xp() const;
  void setXp(const RX xp);
  RX Cn() const;
  void setCn(const RX Cn);
  RX xn() const;
  void setXn(const RX xn);
private:
  QExplicitlySharedDataPointer<PlrBdfPrivate> d;
};

class CONTAM_API QfrQab : public AirflowElement
{
public:
  QfrQab(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX a=RX_INIT(0),RX b=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "qfr_qab";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX a() const;
  void setA(const RX a);
  RX b() const;
  void setB(const RX b);
private:
  QExplicitlySharedDataPointer<QfrQabPrivate> d;
};

class CONTAM_API QfrFab : public AirflowElement
{
public:
  QfrFab(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX a=RX_INIT(0),RX b=RX_INIT(0));
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "qfr_fab";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX a() const;
  void setA(const RX a);
  RX b() const;
  void setB(const RX b);
private:
  QExplicitlySharedDataPointer<QfrFabPrivate> d;
};

class CONTAM_API QfrCrack : public AirflowElement
{
public:
  QfrCrack(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX a=RX_INIT(0),RX b=RX_INIT(0),RX length=RX_INIT(0),RX width=RX_INIT(0),RX depth=RX_INIT(0),int nB=0,int u_L=0,int u_W=0,int u_D=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "qfr_crack";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX a() const;
  void setA(const RX a);
  RX b() const;
  void setB(const RX b);
  RX length() const;
  void setLength(const RX length);
  RX width() const;
  void setWidth(const RX width);
  RX depth() const;
  void setDepth(const RX depth);
  int nB() const;
  void setNB(const int nB);
  int u_L() const;
  void setU_L(const int u_L);
  int u_W() const;
  void setU_W(const int u_W);
  int u_D() const;
  void setU_D(const int u_D);
private:
  QExplicitlySharedDataPointer<QfrCrackPrivate> d;
};

class CONTAM_API QfrTest2 : public AirflowElement
{
public:
  QfrTest2(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX a=RX_INIT(0),RX b=RX_INIT(0),RX dP1=RX_INIT(0),RX F1=RX_INIT(0),RX dP2=RX_INIT(0),RX F2=RX_INIT(0),int u_P1=0,int u_F1=0,int u_P2=0,int u_F2=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "qfr_test2";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX a() const;
  void setA(const RX a);
  RX b() const;
  void setB(const RX b);
  RX dP1() const;
  void setDP1(const RX dP1);
  RX F1() const;
  void setF1(const RX F1);
  RX dP2() const;
  void setDP2(const RX dP2);
  RX F2() const;
  void setF2(const RX F2);
  int u_P1() const;
  void setU_P1(const int u_P1);
  int u_F1() const;
  void setU_F1(const int u_F1);
  int u_P2() const;
  void setU_P2(const int u_P2);
  int u_F2() const;
  void setU_F2(const int u_F2);
private:
  QExplicitlySharedDataPointer<QfrTest2Private> d;
};

class CONTAM_API AfeDor : public AirflowElement
{
public:
  AfeDor(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX dTmin=RX_INIT(0),RX ht=RX_INIT(0),RX wd=RX_INIT(0),RX cd=RX_INIT(0),int u_T=0,int u_H=0,int u_W=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "dor_door";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX dTmin() const;
  void setDTmin(const RX dTmin);
  RX ht() const;
  void setHt(const RX ht);
  RX wd() const;
  void setWd(const RX wd);
  RX cd() const;
  void setCd(const RX cd);
  int u_T() const;
  void setU_T(const int u_T);
  int u_H() const;
  void setU_H(const int u_H);
  int u_W() const;
  void setU_W(const int u_W);
private:
  QExplicitlySharedDataPointer<AfeDorPrivate> d;
};

class CONTAM_API DrPl2 : public AirflowElement
{
public:
  DrPl2(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX dH=RX_INIT(0),RX ht=RX_INIT(0),RX wd=RX_INIT(0),RX cd=RX_INIT(0),int u_H=0,int u_W=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "dor_pl2";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX dH() const;
  void setDH(const RX dH);
  RX ht() const;
  void setHt(const RX ht);
  RX wd() const;
  void setWd(const RX wd);
  RX cd() const;
  void setCd(const RX cd);
  int u_H() const;
  void setU_H(const int u_H);
  int u_W() const;
  void setU_W(const int u_W);
private:
  QExplicitlySharedDataPointer<DrPl2Private> d;
};

class CONTAM_API AfeCmf : public AirflowElement
{
public:
  AfeCmf(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX Flow=RX_INIT(0),int u_F=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "fan_cmf";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX Flow() const;
  void setFlow(const RX Flow);
  int u_F() const;
  void setU_F(const int u_F);
private:
  QExplicitlySharedDataPointer<AfeCmfPrivate> d;
};

class CONTAM_API AfeCvf : public AirflowElement
{
public:
  AfeCvf(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX Flow=RX_INIT(0),int u_F=0);
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "fan_cvf";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX Flow() const;
  void setFlow(const RX Flow);
  int u_F() const;
  void setU_F(const int u_F);
private:
  QExplicitlySharedDataPointer<AfeCvfPrivate> d;
};

class CONTAM_API AfeFan : public AirflowElement
{
public:
  AfeFan(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),RX lam=RX_INIT(0),RX turb=RX_INIT(0),RX expt=RX_INIT(0),RX rdens=RX_INIT(0),RX fdf=RX_INIT(0),RX sop=RX_INIT(0),RX off=RX_INIT(0),std::vector<RX> fpc=std::vector<RX>(),RX Sarea=RX_INIT(0),int u_Sa=0,std::vector<FanDataPoint> data=std::vector<FanDataPoint>());
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "fan_fan";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  RX lam() const;
  void setLam(const RX lam);
  RX turb() const;
  void setTurb(const RX turb);
  RX expt() const;
  void setExpt(const RX expt);
  RX rdens() const;
  void setRdens(const RX rdens);
  RX fdf() const;
  void setFdf(const RX fdf);
  RX sop() const;
  void setSop(const RX sop);
  RX off() const;
  void setOff(const RX off);
  std::vector<RX> fpc() const;
  void setFpc(const std::vector<RX> fpc);
  RX Sarea() const;
  void setSarea(const RX Sarea);
  int u_Sa() const;
  void setU_Sa(const int u_Sa);
  std::vector<FanDataPoint> data() const;
  void setData(const std::vector<FanDataPoint> data);
private:
  QExplicitlySharedDataPointer<AfeFanPrivate> d;
};

class CONTAM_API AfeCsf : public AirflowElement
{
public:
  AfeCsf(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),int u_x=0,int u_y=0,std::vector<DataPoint> data=std::vector<DataPoint>());
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const = 0;
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  int u_x() const;
  void setU_x(const int u_x);
  int u_y() const;
  void setU_y(const int u_y);
  std::vector<DataPoint> data() const;
  void setData(const std::vector<DataPoint> data);
private:
  QExplicitlySharedDataPointer<AfeCsfPrivate> d;
};

class CONTAM_API AfeFsp : public AfeCsf
{
public:
  AfeFsp(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),int u_x=0,
    int u_y=0,std::vector<DataPoint> data=std::vector<DataPoint>()) : AfeCsf(nr,icon,name,desc,u_x,u_y,data)
  {}
  std::string dataType() const {return "csf_fsp";}
};

class CONTAM_API AfeQsp : public AfeCsf
{
public:
  AfeQsp(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),int u_x=0,
    int u_y=0,std::vector<DataPoint> data=std::vector<DataPoint>()) : AfeCsf(nr,icon,name,desc,u_x,u_y,data)
  {}
  std::string dataType() const {return "csf_qsp";}
};

class CONTAM_API AfePsf : public AfeCsf
{
public:
  AfePsf(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),int u_x=0,
    int u_y=0,std::vector<DataPoint> data=std::vector<DataPoint>()) : AfeCsf(nr,icon,name,desc,u_x,u_y,data)
  {}
  std::string dataType() const {return "csf_psf";}
};

class CONTAM_API AfePsq : public AfeCsf
{
public:
  AfePsq(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),int u_x=0,
    int u_y=0,std::vector<DataPoint> data=std::vector<DataPoint>()) : AfeCsf(nr,icon,name,desc,u_x,u_y,data)
  {}
  std::string dataType() const {return "csf_psq";}
};

class CONTAM_API AfeSup : public AirflowElement
{
public:
  AfeSup(int nr=0,int icon=0,std::string name=std::string(),std::string desc=std::string(),int sched=0,int u_H=0,std::vector<AirflowSubelementData> subelements=std::vector<AirflowSubelementData>());
  void read(Reader &input);
  std::string write();
  void readDetails(Reader &input);
  int nr() const;
  void setNr(const int nr);
  int icon() const;
  void setIcon(const int icon);
  std::string dataType() const {return "sup_afe";}
  std::string name() const;
  void setName(const std::string name);
  std::string desc() const;
  void setDesc(const std::string desc);
  int sched() const;
  void setSched(const int sched);
  int u_H() const;
  void setU_H(const int u_H);
  std::vector<AirflowSubelementData> subelements() const;
  void setSubelements(const std::vector<AirflowSubelementData> subelements);
private:
  QExplicitlySharedDataPointer<AfeSupPrivate> d;
};

} // contam
} // openstudio

#endif
