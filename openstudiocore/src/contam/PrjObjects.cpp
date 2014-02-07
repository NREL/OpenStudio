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
#include "PrjObjects.hpp"

#include <utilities/core/Logger.hpp>

namespace openstudio {
namespace contam {

Zone::Zone(int nr,unsigned int flags,int ps,int pc,int pk,int pl,RX relHt,RX Vol,RX T0,RX P0,std::string name,int color,int u_Ht,int u_V,int u_T,int u_P,int cdaxis,int cfd,std::string cfdname,RX X1,RX Y1,RX H1,RX X2,RX Y2,RX H2,RX celldx,RX axialD,int u_aD,int u_L)
{
  d = new ZonePrivate(nr,flags,ps,pc,pk,pl,relHt,Vol,T0,P0,name,color,u_Ht,u_V,u_T,u_P,cdaxis,cfd,cfdname,X1,Y1,H1,X2,Y2,H2,celldx,axialD,u_aD,u_L);
}

int Zone::nr() const
{
  return d->nr;
}

void Zone::setNr(const int nr)
{
  d->nr = nr;
}

unsigned int Zone::flags() const
{
  return d->flags;
}

void Zone::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int Zone::ps() const
{
  return d->ps;
}

void Zone::setPs(const int ps)
{
  d->ps = ps;
}

int Zone::pc() const
{
  return d->pc;
}

void Zone::setPc(const int pc)
{
  d->pc = pc;
}

int Zone::pk() const
{
  return d->pk;
}

void Zone::setPk(const int pk)
{
  d->pk = pk;
}

int Zone::pl() const
{
  return d->pl;
}

void Zone::setPl(const int pl)
{
  d->pl = pl;
}

RX Zone::relHt() const
{
  return d->relHt;
}

void Zone::setRelHt(const RX relHt)
{
  d->relHt = relHt;
}

RX Zone::Vol() const
{
  return d->Vol;
}

void Zone::setVol(const RX Vol)
{
  d->Vol = Vol;
}

RX Zone::T0() const
{
  return d->T0;
}

void Zone::setT0(const RX T0)
{
  d->T0 = T0;
}

RX Zone::P0() const
{
  return d->P0;
}

void Zone::setP0(const RX P0)
{
  d->P0 = P0;
}

std::string Zone::name() const
{
  return d->name;
}

void Zone::setName(const std::string name)
{
  d->name = name;
}

int Zone::color() const
{
  return d->color;
}

void Zone::setColor(const int color)
{
  d->color = color;
}

int Zone::u_Ht() const
{
  return d->u_Ht;
}

void Zone::setU_Ht(const int u_Ht)
{
  d->u_Ht = u_Ht;
}

int Zone::u_V() const
{
  return d->u_V;
}

void Zone::setU_V(const int u_V)
{
  d->u_V = u_V;
}

int Zone::u_T() const
{
  return d->u_T;
}

void Zone::setU_T(const int u_T)
{
  d->u_T = u_T;
}

int Zone::u_P() const
{
  return d->u_P;
}

void Zone::setU_P(const int u_P)
{
  d->u_P = u_P;
}

int Zone::cdaxis() const
{
  return d->cdaxis;
}

void Zone::setCdaxis(const int cdaxis)
{
  d->cdaxis = cdaxis;
}

int Zone::cfd() const
{
  return d->cfd;
}

void Zone::setCfd(const int cfd)
{
  d->cfd = cfd;
}

std::string Zone::cfdname() const
{
  return d->cfdname;
}

void Zone::setCfdname(const std::string cfdname)
{
  d->cfdname = cfdname;
}

RX Zone::X1() const
{
  return d->X1;
}

void Zone::setX1(const RX X1)
{
  d->X1 = X1;
}

RX Zone::Y1() const
{
  return d->Y1;
}

void Zone::setY1(const RX Y1)
{
  d->Y1 = Y1;
}

RX Zone::H1() const
{
  return d->H1;
}

void Zone::setH1(const RX H1)
{
  d->H1 = H1;
}

RX Zone::X2() const
{
  return d->X2;
}

void Zone::setX2(const RX X2)
{
  d->X2 = X2;
}

RX Zone::Y2() const
{
  return d->Y2;
}

void Zone::setY2(const RX Y2)
{
  d->Y2 = Y2;
}

RX Zone::H2() const
{
  return d->H2;
}

void Zone::setH2(const RX H2)
{
  d->H2 = H2;
}

RX Zone::celldx() const
{
  return d->celldx;
}

void Zone::setCelldx(const RX celldx)
{
  d->celldx = celldx;
}

RX Zone::axialD() const
{
  return d->axialD;
}

void Zone::setAxialD(const RX axialD)
{
  d->axialD = axialD;
}

int Zone::u_aD() const
{
  return d->u_aD;
}

void Zone::setU_aD(const int u_aD)
{
  d->u_aD = u_aD;
}

int Zone::u_L() const
{
  return d->u_L;
}

void Zone::setU_L(const int u_L)
{
  d->u_L = u_L;
}

std::string Zone::write()
{
  return d->write();
}

void Zone::read(Reader &input)
{
  d->read(input);
}

void Zone::setVariablePressure(bool b)
{
  if(b)
    d->flags |= VAR_P;
  else
    d->flags &= NVAR_P;
}

bool Zone::variablePressure() const
{
  return d->flags & VAR_P;
}

void Zone::setVariableContaminants(bool b)
{
  if(b)
    d->flags |= VAR_C;
  else
    d->flags &= NVAR_C;
}

bool Zone::variableContaminants() const
{
  return d->flags & VAR_C;
}

void Zone::setSystem(bool b)
{
  if(b)
    d->flags |= SYS_N;
  else
    d->flags &= NSYS_N;
}

bool Zone::system() const
{
  return d->flags & SYS_N;
}

RX Zone::ic(const int i) const
{
  return d->ic[i];
}

std::vector<RX> Zone::ic() const
{
  return d->ic;
}

void Zone::setIc(const int i, const RX value)
{
  d->ic[i] = value;
}

void Zone::setIc(std::vector<RX> ic)
{
  d->ic = ic;
}

Species::Species(int nr,int sflag,int ntflag,RX molwt,RX mdiam,RX edens,RX decay,RX Dm,RX ccdef,RX Cp,int ucc,int umd,int ued,int udm,int ucp,std::string name,std::string desc)
{
  d = new SpeciesPrivate(nr,sflag,ntflag,molwt,mdiam,edens,decay,Dm,ccdef,Cp,ucc,umd,ued,udm,ucp,name,desc);
}

std::string Species::write()
{
  return d->write();
}

void Species::read(Reader &input)
{
  d->read(input);
}

int Species::nr() const
{
  return d->nr;
}

void Species::setNr(const int nr)
{
  d->nr = nr;
}

int Species::sflag() const
{
  return d->sflag;
}

void Species::setSflag(const int sflag)
{
  d->sflag = sflag;
}

int Species::ntflag() const
{
  return d->ntflag;
}

void Species::setNtflag(const int ntflag)
{
  d->ntflag = ntflag;
}

RX Species::molwt() const
{
  return d->molwt;
}

void Species::setMolwt(const RX molwt)
{
  d->molwt = molwt;
}

RX Species::mdiam() const
{
  return d->mdiam;
}

void Species::setMdiam(const RX mdiam)
{
  d->mdiam = mdiam;
}

RX Species::edens() const
{
  return d->edens;
}

void Species::setEdens(const RX edens)
{
  d->edens = edens;
}

RX Species::decay() const
{
  return d->decay;
}

void Species::setDecay(const RX decay)
{
  d->decay = decay;
}

RX Species::Dm() const
{
  return d->Dm;
}

void Species::setDm(const RX Dm)
{
  d->Dm = Dm;
}

RX Species::ccdef() const
{
  return d->ccdef;
}

void Species::setCcdef(const RX ccdef)
{
  d->ccdef = ccdef;
}

RX Species::Cp() const
{
  return d->Cp;
}

void Species::setCp(const RX Cp)
{
  d->Cp = Cp;
}

int Species::ucc() const
{
  return d->ucc;
}

void Species::setUcc(const int ucc)
{
  d->ucc = ucc;
}

int Species::umd() const
{
  return d->umd;
}

void Species::setUmd(const int umd)
{
  d->umd = umd;
}

int Species::ued() const
{
  return d->ued;
}

void Species::setUed(const int ued)
{
  d->ued = ued;
}

int Species::udm() const
{
  return d->udm;
}

void Species::setUdm(const int udm)
{
  d->udm = udm;
}

int Species::ucp() const
{
  return d->ucp;
}

void Species::setUcp(const int ucp)
{
  d->ucp = ucp;
}

std::string Species::name() const
{
  return d->name;
}

void Species::setName(const std::string name)
{
  d->name = name;
}

std::string Species::desc() const
{
  return d->desc;
}

void Species::setDesc(const std::string desc)
{
  d->desc = desc;
}

Ahs::Ahs(int nr,int zone_r,int zone_s,int path_r,int path_s,int path_x,std::string name,std::string desc)
{
  d = new AhsPrivate(nr,zone_r,zone_s,path_r,path_s,path_x,name,desc);
}

std::string Ahs::write()
{
  return d->write();
}

void Ahs::read(Reader &input)
{
  d->read(input);
}

int Ahs::nr() const
{
  return d->nr;
}

void Ahs::setNr(const int nr)
{
  d->nr = nr;
}

int Ahs::zone_r() const
{
  return d->zone_r;
}

void Ahs::setZone_r(const int zone_r)
{
  d->zone_r = zone_r;
}

int Ahs::zone_s() const
{
  return d->zone_s;
}

void Ahs::setZone_s(const int zone_s)
{
  d->zone_s = zone_s;
}

int Ahs::path_r() const
{
  return d->path_r;
}

void Ahs::setPath_r(const int path_r)
{
  d->path_r = path_r;
}

int Ahs::path_s() const
{
  return d->path_s;
}

void Ahs::setPath_s(const int path_s)
{
  d->path_s = path_s;
}

int Ahs::path_x() const
{
  return d->path_x;
}

void Ahs::setPath_x(const int path_x)
{
  d->path_x = path_x;
}

std::string Ahs::name() const
{
  return d->name;
}

void Ahs::setName(const std::string name)
{
  d->name = name;
}

std::string Ahs::desc() const
{
  return d->desc;
}

void Ahs::setDesc(const std::string desc)
{
  d->desc = desc;
}

Path::Path(int nr,int flags,int pzn,int pzm,int pe,int pf,int pw,int pa,int ps,int pc,int pld,RX X,RX Y,RX relHt,RX mult,RX wPset,RX wPmod,RX wazm,RX Fahs,RX Xmax,RX Xmin,unsigned int icon,unsigned int dir,int u_Ht,int u_XY,int u_dP,int u_F,int cfd,std::string cfd_name,int cfd_ptype,int cfd_btype,int cfd_capp)
{
  d = new PathPrivate(nr,flags,pzn,pzm,pe,pf,pw,pa,ps,pc,pld,X,Y,relHt,mult,wPset,wPmod,wazm,Fahs,Xmax,Xmin,icon,dir,u_Ht,u_XY,u_dP,u_F,cfd,cfd_name,cfd_ptype,cfd_btype,cfd_capp);
}

void Path::read(Reader &input)
{
  d->read(input);
}

std::string Path::write()
{
  return d->write();
}

int Path::nr() const
{
  return d->nr;
}

void Path::setNr(const int nr)
{
  d->nr = nr;
}

int Path::flags() const
{
  return d->flags;
}

void Path::setFlags(const int flags)
{
  d->flags = flags;
}

int Path::pzn() const
{
  return d->pzn;
}

void Path::setPzn(const int pzn)
{
  d->pzn = pzn;
}

int Path::pzm() const
{
  return d->pzm;
}

void Path::setPzm(const int pzm)
{
  d->pzm = pzm;
}

int Path::pe() const
{
  return d->pe;
}

void Path::setPe(const int pe)
{
  d->pe = pe;
}

int Path::pf() const
{
  return d->pf;
}

void Path::setPf(const int pf)
{
  d->pf = pf;
}

int Path::pw() const
{
  return d->pw;
}

void Path::setPw(const int pw)
{
  d->pw = pw;
}

int Path::pa() const
{
  return d->pa;
}

void Path::setPa(const int pa)
{
  d->pa = pa;
}

int Path::ps() const
{
  return d->ps;
}

void Path::setPs(const int ps)
{
  d->ps = ps;
}

int Path::pc() const
{
  return d->pc;
}

void Path::setPc(const int pc)
{
  d->pc = pc;
}

int Path::pld() const
{
  return d->pld;
}

void Path::setPld(const int pld)
{
  d->pld = pld;
}

RX Path::X() const
{
  return d->X;
}

void Path::setX(const RX X)
{
  d->X = X;
}

RX Path::Y() const
{
  return d->Y;
}

void Path::setY(const RX Y)
{
  d->Y = Y;
}

RX Path::relHt() const
{
  return d->relHt;
}

void Path::setRelHt(const RX relHt)
{
  d->relHt = relHt;
}

RX Path::mult() const
{
  return d->mult;
}

void Path::setMult(const RX mult)
{
  d->mult = mult;
}

RX Path::wPset() const
{
  return d->wPset;
}

void Path::setWPset(const RX wPset)
{
  d->wPset = wPset;
}

RX Path::wPmod() const
{
  return d->wPmod;
}

void Path::setWPmod(const RX wPmod)
{
  d->wPmod = wPmod;
}

RX Path::wazm() const
{
  return d->wazm;
}

void Path::setWazm(const RX wazm)
{
  d->wazm = wazm;
}

RX Path::Fahs() const
{
  return d->Fahs;
}

void Path::setFahs(const RX Fahs)
{
  d->Fahs = Fahs;
}

RX Path::Xmax() const
{
  return d->Xmax;
}

void Path::setXmax(const RX Xmax)
{
  d->Xmax = Xmax;
}

RX Path::Xmin() const
{
  return d->Xmin;
}

void Path::setXmin(const RX Xmin)
{
  d->Xmin = Xmin;
}

unsigned int Path::icon() const
{
  return d->icon;
}

void Path::setIcon(const unsigned int icon)
{
  d->icon = icon;
}

unsigned int Path::dir() const
{
  return d->dir;
}

void Path::setDir(const unsigned int dir)
{
  d->dir = dir;
}

int Path::u_Ht() const
{
  return d->u_Ht;
}

void Path::setU_Ht(const int u_Ht)
{
  d->u_Ht = u_Ht;
}

int Path::u_XY() const
{
  return d->u_XY;
}

void Path::setU_XY(const int u_XY)
{
  d->u_XY = u_XY;
}

int Path::u_dP() const
{
  return d->u_dP;
}

void Path::setU_dP(const int u_dP)
{
  d->u_dP = u_dP;
}

int Path::u_F() const
{
  return d->u_F;
}

void Path::setU_F(const int u_F)
{
  d->u_F = u_F;
}

int Path::cfd() const
{
  return d->cfd;
}

void Path::setCfd(const int cfd)
{
  d->cfd = cfd;
}

std::string Path::cfd_name() const
{
  return d->cfd_name;
}

void Path::setCfd_name(const std::string cfd_name)
{
  d->cfd_name = cfd_name;
}

int Path::cfd_ptype() const
{
  return d->cfd_ptype;
}

void Path::setCfd_ptype(const int cfd_ptype)
{
  d->cfd_ptype = cfd_ptype;
}

int Path::cfd_btype() const
{
  return d->cfd_btype;
}

void Path::setCfd_btype(const int cfd_btype)
{
  d->cfd_btype = cfd_btype;
}

int Path::cfd_capp() const
{
  return d->cfd_capp;
}

void Path::setCfd_capp(const int cfd_capp)
{
  d->cfd_capp = cfd_capp;
}



void Path::setWindPressure(bool b)
{
  if(b)
    d->flags |= WIND;
  else
    d->flags &= 0xFFFE;
}

bool Path::windPressure()
{
  return d->flags & WIND;
}

void Path::setSystem(bool b)
{
  if(b)
    d->flags |= AHS_S;
  else
    d->flags &= 0xFFF7;
}

bool Path::system()
{
  return d->flags & AHS_S;
}

void Path::setExhaust(bool b)
{
  if(b)
    d->flags |= AHS_X;
  else
    d->flags &= 0xFFBF;
}

bool Path::exhaust()
{
  return d->flags & AHS_X;
}


void Path::setRecirculation(bool b)
{
  if(b)
    d->flags |= AHS_R;
  else
    d->flags &= 0xFFEF;
}

bool Path::recirculation()
{
  return d->flags & AHS_R;
}

void Path::setOutsideAir(bool b)
{
  if(b)
    d->flags |= AHS_O;
  else
    d->flags &= 0xFFDF;
}

bool Path::outsideAir()
{
  return d->flags & AHS_O;
}

RunControl::RunControl(std::string name,std::string version,int echo,std::string prjdesc,int skheight,int skwidth,int def_units,int def_flows,RX def_T,int udefT,RX rel_N,RX wind_H,int uwH,RX wind_Ao,RX wind_a,RX scale,int uScale,int orgRow,int orgCol,int invYaxis,int showGeom,WeatherData ssWeather,WeatherData wptWeather,std::string WTHpath,std::string CTMpath,std::string CVFpath,std::string DVFpath,std::string WPCfile,std::string EWCfile,std::string WPCdesc,RX X0,RX Y0,RX Z0,RX angle,int u_XYZ,RX epsPath,RX epsSpcs,std::string tShift,std::string dStart,std::string dEnd,int useWPCwp,int useWPCmf,int wpctrig,RX latd,RX lgtd,RX Tznr,RX altd,RX Tgrnd,int utg,int u_a,int sim_af,int afcalc,int afmaxi,RX afrcnvg,RX afacnvg,RX afrelax,int uac2,RX Pres,int uPres,int afslae,int afrseq,int aflmaxi,RX aflcnvg,int aflinit,int Tadj,int sim_mf,int ccmaxi,RX ccrcnvg,RX ccacnvg,RX ccrelax,int uccc,int mfnmthd,int mfnrseq,int mfnmaxi,RX mfnrcnvg,RX mfnacnvg,RX mfnrelax,RX mfngamma,int uccn,int mftmthd,int mftrseq,int mftmaxi,RX mftrcnvg,RX mftacnvg,RX mftrelax,RX mftgamma,int ucct,int mfvmthd,int mfvrseq,int mfvmaxi,RX mfvrcnvg,RX mfvacnvg,RX mfvrelax,int uccv,int mf_solver,int sim_1dz,int sim_1dd,RX celldx,int sim_vjt,int udx,int cvode_mth,RX cvode_rcnvg,RX cvode_acnvg,RX cvode_dtmax,int tsdens,RX tsrelax,int tsmaxi,int cnvgSS,int densZP,int stackD,int dodMdt,std::string date_st,std::string time_st,std::string date_0,std::string time_0,std::string date_1,std::string time_1,std::string time_step,std::string time_list,std::string time_scrn,int restart,std::string rstdate,std::string rsttime,int list,int doDlg,int pfsave,int zfsave,int zcsave,int achvol,int achsave,int abwsave,int cbwsave,int expsave,int ebwsave,int zaasave,int zbwsave,int rzfsave,int rzmsave,int rz1save,int csmsave,int srfsave,int logsave,std::vector<int> save,std::vector<RX> rvals,int BldgFlowZ,int BldgFlowD,int BldgFlowC,int cfd_ctype,RX cfd_convcpl,int cfd_var,int cfd_zref,int cfd_imax,int cfd_dtcmo)
{
  d = new RunControlPrivate(name,version,echo,prjdesc,skheight,skwidth,def_units,def_flows,def_T,udefT,rel_N,wind_H,uwH,wind_Ao,wind_a,scale,uScale,orgRow,orgCol,invYaxis,showGeom,ssWeather,wptWeather,WTHpath,CTMpath,CVFpath,DVFpath,WPCfile,EWCfile,WPCdesc,X0,Y0,Z0,angle,u_XYZ,epsPath,epsSpcs,tShift,dStart,dEnd,useWPCwp,useWPCmf,wpctrig,latd,lgtd,Tznr,altd,Tgrnd,utg,u_a,sim_af,afcalc,afmaxi,afrcnvg,afacnvg,afrelax,uac2,Pres,uPres,afslae,afrseq,aflmaxi,aflcnvg,aflinit,Tadj,sim_mf,ccmaxi,ccrcnvg,ccacnvg,ccrelax,uccc,mfnmthd,mfnrseq,mfnmaxi,mfnrcnvg,mfnacnvg,mfnrelax,mfngamma,uccn,mftmthd,mftrseq,mftmaxi,mftrcnvg,mftacnvg,mftrelax,mftgamma,ucct,mfvmthd,mfvrseq,mfvmaxi,mfvrcnvg,mfvacnvg,mfvrelax,uccv,mf_solver,sim_1dz,sim_1dd,celldx,sim_vjt,udx,cvode_mth,cvode_rcnvg,cvode_acnvg,cvode_dtmax,tsdens,tsrelax,tsmaxi,cnvgSS,densZP,stackD,dodMdt,date_st,time_st,date_0,time_0,date_1,time_1,time_step,time_list,time_scrn,restart,rstdate,rsttime,list,doDlg,pfsave,zfsave,zcsave,achvol,achsave,abwsave,cbwsave,expsave,ebwsave,zaasave,zbwsave,rzfsave,rzmsave,rz1save,csmsave,srfsave,logsave,save,rvals,BldgFlowZ,BldgFlowD,BldgFlowC,cfd_ctype,cfd_convcpl,cfd_var,cfd_zref,cfd_imax,cfd_dtcmo);
}

std::string RunControl::write()
{
  return d->write();
}

void RunControl::read(Reader &input)
{
  d->read(input);
}

std::string RunControl::name() const
{
  return d->name;
}

void RunControl::setName(const std::string name)
{
  d->name = name;
}

std::string RunControl::version() const
{
  return d->version;
}

void RunControl::setVersion(const std::string version)
{
  d->version = version;
}

int RunControl::echo() const
{
  return d->echo;
}

void RunControl::setEcho(const int echo)
{
  d->echo = echo;
}

std::string RunControl::prjdesc() const
{
  return d->prjdesc;
}

void RunControl::setPrjdesc(const std::string prjdesc)
{
  d->prjdesc = prjdesc;
}

int RunControl::skheight() const
{
  return d->skheight;
}

void RunControl::setSkheight(const int skheight)
{
  d->skheight = skheight;
}

int RunControl::skwidth() const
{
  return d->skwidth;
}

void RunControl::setSkwidth(const int skwidth)
{
  d->skwidth = skwidth;
}

int RunControl::def_units() const
{
  return d->def_units;
}

void RunControl::setDef_units(const int def_units)
{
  d->def_units = def_units;
}

int RunControl::def_flows() const
{
  return d->def_flows;
}

void RunControl::setDef_flows(const int def_flows)
{
  d->def_flows = def_flows;
}

RX RunControl::def_T() const
{
  return d->def_T;
}

void RunControl::setDef_T(const RX def_T)
{
  d->def_T = def_T;
}

int RunControl::udefT() const
{
  return d->udefT;
}

void RunControl::setUdefT(const int udefT)
{
  d->udefT = udefT;
}

RX RunControl::rel_N() const
{
  return d->rel_N;
}

void RunControl::setRel_N(const RX rel_N)
{
  d->rel_N = rel_N;
}

RX RunControl::wind_H() const
{
  return d->wind_H;
}

void RunControl::setWind_H(const RX wind_H)
{
  d->wind_H = wind_H;
}

int RunControl::uwH() const
{
  return d->uwH;
}

void RunControl::setUwH(const int uwH)
{
  d->uwH = uwH;
}

RX RunControl::wind_Ao() const
{
  return d->wind_Ao;
}

void RunControl::setWind_Ao(const RX wind_Ao)
{
  d->wind_Ao = wind_Ao;
}

RX RunControl::wind_a() const
{
  return d->wind_a;
}

void RunControl::setWind_a(const RX wind_a)
{
  d->wind_a = wind_a;
}

RX RunControl::scale() const
{
  return d->scale;
}

void RunControl::setScale(const RX scale)
{
  d->scale = scale;
}

int RunControl::uScale() const
{
  return d->uScale;
}

void RunControl::setUScale(const int uScale)
{
  d->uScale = uScale;
}

int RunControl::orgRow() const
{
  return d->orgRow;
}

void RunControl::setOrgRow(const int orgRow)
{
  d->orgRow = orgRow;
}

int RunControl::orgCol() const
{
  return d->orgCol;
}

void RunControl::setOrgCol(const int orgCol)
{
  d->orgCol = orgCol;
}

int RunControl::invYaxis() const
{
  return d->invYaxis;
}

void RunControl::setInvYaxis(const int invYaxis)
{
  d->invYaxis = invYaxis;
}

int RunControl::showGeom() const
{
  return d->showGeom;
}

void RunControl::setShowGeom(const int showGeom)
{
  d->showGeom = showGeom;
}

WeatherData RunControl::ssWeather() const
{
  return d->ssWeather;
}

void RunControl::setSsWeather(const WeatherData ssWeather)
{
  d->ssWeather = ssWeather;
}

WeatherData RunControl::wptWeather() const
{
  return d->wptWeather;
}

void RunControl::setWptWeather(const WeatherData wptWeather)
{
  d->wptWeather = wptWeather;
}

std::string RunControl::WTHpath() const
{
  return d->WTHpath;
}

void RunControl::setWTHpath(const std::string WTHpath)
{
  d->WTHpath = WTHpath;
}

std::string RunControl::CTMpath() const
{
  return d->CTMpath;
}

void RunControl::setCTMpath(const std::string CTMpath)
{
  d->CTMpath = CTMpath;
}

std::string RunControl::CVFpath() const
{
  return d->CVFpath;
}

void RunControl::setCVFpath(const std::string CVFpath)
{
  d->CVFpath = CVFpath;
}

std::string RunControl::DVFpath() const
{
  return d->DVFpath;
}

void RunControl::setDVFpath(const std::string DVFpath)
{
  d->DVFpath = DVFpath;
}

std::string RunControl::WPCfile() const
{
  return d->WPCfile;
}

void RunControl::setWPCfile(const std::string WPCfile)
{
  d->WPCfile = WPCfile;
}

std::string RunControl::EWCfile() const
{
  return d->EWCfile;
}

void RunControl::setEWCfile(const std::string EWCfile)
{
  d->EWCfile = EWCfile;
}

std::string RunControl::WPCdesc() const
{
  return d->WPCdesc;
}

void RunControl::setWPCdesc(const std::string WPCdesc)
{
  d->WPCdesc = WPCdesc;
}

RX RunControl::X0() const
{
  return d->X0;
}

void RunControl::setX0(const RX X0)
{
  d->X0 = X0;
}

RX RunControl::Y0() const
{
  return d->Y0;
}

void RunControl::setY0(const RX Y0)
{
  d->Y0 = Y0;
}

RX RunControl::Z0() const
{
  return d->Z0;
}

void RunControl::setZ0(const RX Z0)
{
  d->Z0 = Z0;
}

RX RunControl::angle() const
{
  return d->angle;
}

void RunControl::setAngle(const RX angle)
{
  d->angle = angle;
}

int RunControl::u_XYZ() const
{
  return d->u_XYZ;
}

void RunControl::setU_XYZ(const int u_XYZ)
{
  d->u_XYZ = u_XYZ;
}

RX RunControl::epsPath() const
{
  return d->epsPath;
}

void RunControl::setEpsPath(const RX epsPath)
{
  d->epsPath = epsPath;
}

RX RunControl::epsSpcs() const
{
  return d->epsSpcs;
}

void RunControl::setEpsSpcs(const RX epsSpcs)
{
  d->epsSpcs = epsSpcs;
}

std::string RunControl::tShift() const
{
  return d->tShift;
}

void RunControl::setTShift(const std::string tShift)
{
  d->tShift = tShift;
}

std::string RunControl::dStart() const
{
  return d->dStart;
}

void RunControl::setDStart(const std::string dStart)
{
  d->dStart = dStart;
}

std::string RunControl::dEnd() const
{
  return d->dEnd;
}

void RunControl::setDEnd(const std::string dEnd)
{
  d->dEnd = dEnd;
}

int RunControl::useWPCwp() const
{
  return d->useWPCwp;
}

void RunControl::setUseWPCwp(const int useWPCwp)
{
  d->useWPCwp = useWPCwp;
}

int RunControl::useWPCmf() const
{
  return d->useWPCmf;
}

void RunControl::setUseWPCmf(const int useWPCmf)
{
  d->useWPCmf = useWPCmf;
}

int RunControl::wpctrig() const
{
  return d->wpctrig;
}

void RunControl::setWpctrig(const int wpctrig)
{
  d->wpctrig = wpctrig;
}

RX RunControl::latd() const
{
  return d->latd;
}

void RunControl::setLatd(const RX latd)
{
  d->latd = latd;
}

RX RunControl::lgtd() const
{
  return d->lgtd;
}

void RunControl::setLgtd(const RX lgtd)
{
  d->lgtd = lgtd;
}

RX RunControl::Tznr() const
{
  return d->Tznr;
}

void RunControl::setTznr(const RX Tznr)
{
  d->Tznr = Tznr;
}

RX RunControl::altd() const
{
  return d->altd;
}

void RunControl::setAltd(const RX altd)
{
  d->altd = altd;
}

RX RunControl::Tgrnd() const
{
  return d->Tgrnd;
}

void RunControl::setTgrnd(const RX Tgrnd)
{
  d->Tgrnd = Tgrnd;
}

int RunControl::utg() const
{
  return d->utg;
}

void RunControl::setUtg(const int utg)
{
  d->utg = utg;
}

int RunControl::u_a() const
{
  return d->u_a;
}

void RunControl::setU_a(const int u_a)
{
  d->u_a = u_a;
}

int RunControl::sim_af() const
{
  return d->sim_af;
}

void RunControl::setSim_af(const int sim_af)
{
  d->sim_af = sim_af;
}

int RunControl::afcalc() const
{
  return d->afcalc;
}

void RunControl::setAfcalc(const int afcalc)
{
  d->afcalc = afcalc;
}

int RunControl::afmaxi() const
{
  return d->afmaxi;
}

void RunControl::setAfmaxi(const int afmaxi)
{
  d->afmaxi = afmaxi;
}

RX RunControl::afrcnvg() const
{
  return d->afrcnvg;
}

void RunControl::setAfrcnvg(const RX afrcnvg)
{
  d->afrcnvg = afrcnvg;
}

RX RunControl::afacnvg() const
{
  return d->afacnvg;
}

void RunControl::setAfacnvg(const RX afacnvg)
{
  d->afacnvg = afacnvg;
}

RX RunControl::afrelax() const
{
  return d->afrelax;
}

void RunControl::setAfrelax(const RX afrelax)
{
  d->afrelax = afrelax;
}

int RunControl::uac2() const
{
  return d->uac2;
}

void RunControl::setUac2(const int uac2)
{
  d->uac2 = uac2;
}

RX RunControl::Pres() const
{
  return d->Pres;
}

void RunControl::setPres(const RX Pres)
{
  d->Pres = Pres;
}

int RunControl::uPres() const
{
  return d->uPres;
}

void RunControl::setUPres(const int uPres)
{
  d->uPres = uPres;
}

int RunControl::afslae() const
{
  return d->afslae;
}

void RunControl::setAfslae(const int afslae)
{
  d->afslae = afslae;
}

int RunControl::afrseq() const
{
  return d->afrseq;
}

void RunControl::setAfrseq(const int afrseq)
{
  d->afrseq = afrseq;
}

int RunControl::aflmaxi() const
{
  return d->aflmaxi;
}

void RunControl::setAflmaxi(const int aflmaxi)
{
  d->aflmaxi = aflmaxi;
}

RX RunControl::aflcnvg() const
{
  return d->aflcnvg;
}

void RunControl::setAflcnvg(const RX aflcnvg)
{
  d->aflcnvg = aflcnvg;
}

int RunControl::aflinit() const
{
  return d->aflinit;
}

void RunControl::setAflinit(const int aflinit)
{
  d->aflinit = aflinit;
}

int RunControl::Tadj() const
{
  return d->Tadj;
}

void RunControl::setTadj(const int Tadj)
{
  d->Tadj = Tadj;
}

int RunControl::sim_mf() const
{
  return d->sim_mf;
}

void RunControl::setSim_mf(const int sim_mf)
{
  d->sim_mf = sim_mf;
}

int RunControl::ccmaxi() const
{
  return d->ccmaxi;
}

void RunControl::setCcmaxi(const int ccmaxi)
{
  d->ccmaxi = ccmaxi;
}

RX RunControl::ccrcnvg() const
{
  return d->ccrcnvg;
}

void RunControl::setCcrcnvg(const RX ccrcnvg)
{
  d->ccrcnvg = ccrcnvg;
}

RX RunControl::ccacnvg() const
{
  return d->ccacnvg;
}

void RunControl::setCcacnvg(const RX ccacnvg)
{
  d->ccacnvg = ccacnvg;
}

RX RunControl::ccrelax() const
{
  return d->ccrelax;
}

void RunControl::setCcrelax(const RX ccrelax)
{
  d->ccrelax = ccrelax;
}

int RunControl::uccc() const
{
  return d->uccc;
}

void RunControl::setUccc(const int uccc)
{
  d->uccc = uccc;
}

int RunControl::mfnmthd() const
{
  return d->mfnmthd;
}

void RunControl::setMfnmthd(const int mfnmthd)
{
  d->mfnmthd = mfnmthd;
}

int RunControl::mfnrseq() const
{
  return d->mfnrseq;
}

void RunControl::setMfnrseq(const int mfnrseq)
{
  d->mfnrseq = mfnrseq;
}

int RunControl::mfnmaxi() const
{
  return d->mfnmaxi;
}

void RunControl::setMfnmaxi(const int mfnmaxi)
{
  d->mfnmaxi = mfnmaxi;
}

RX RunControl::mfnrcnvg() const
{
  return d->mfnrcnvg;
}

void RunControl::setMfnrcnvg(const RX mfnrcnvg)
{
  d->mfnrcnvg = mfnrcnvg;
}

RX RunControl::mfnacnvg() const
{
  return d->mfnacnvg;
}

void RunControl::setMfnacnvg(const RX mfnacnvg)
{
  d->mfnacnvg = mfnacnvg;
}

RX RunControl::mfnrelax() const
{
  return d->mfnrelax;
}

void RunControl::setMfnrelax(const RX mfnrelax)
{
  d->mfnrelax = mfnrelax;
}

RX RunControl::mfngamma() const
{
  return d->mfngamma;
}

void RunControl::setMfngamma(const RX mfngamma)
{
  d->mfngamma = mfngamma;
}

int RunControl::uccn() const
{
  return d->uccn;
}

void RunControl::setUccn(const int uccn)
{
  d->uccn = uccn;
}

int RunControl::mftmthd() const
{
  return d->mftmthd;
}

void RunControl::setMftmthd(const int mftmthd)
{
  d->mftmthd = mftmthd;
}

int RunControl::mftrseq() const
{
  return d->mftrseq;
}

void RunControl::setMftrseq(const int mftrseq)
{
  d->mftrseq = mftrseq;
}

int RunControl::mftmaxi() const
{
  return d->mftmaxi;
}

void RunControl::setMftmaxi(const int mftmaxi)
{
  d->mftmaxi = mftmaxi;
}

RX RunControl::mftrcnvg() const
{
  return d->mftrcnvg;
}

void RunControl::setMftrcnvg(const RX mftrcnvg)
{
  d->mftrcnvg = mftrcnvg;
}

RX RunControl::mftacnvg() const
{
  return d->mftacnvg;
}

void RunControl::setMftacnvg(const RX mftacnvg)
{
  d->mftacnvg = mftacnvg;
}

RX RunControl::mftrelax() const
{
  return d->mftrelax;
}

void RunControl::setMftrelax(const RX mftrelax)
{
  d->mftrelax = mftrelax;
}

RX RunControl::mftgamma() const
{
  return d->mftgamma;
}

void RunControl::setMftgamma(const RX mftgamma)
{
  d->mftgamma = mftgamma;
}

int RunControl::ucct() const
{
  return d->ucct;
}

void RunControl::setUcct(const int ucct)
{
  d->ucct = ucct;
}

int RunControl::mfvmthd() const
{
  return d->mfvmthd;
}

void RunControl::setMfvmthd(const int mfvmthd)
{
  d->mfvmthd = mfvmthd;
}

int RunControl::mfvrseq() const
{
  return d->mfvrseq;
}

void RunControl::setMfvrseq(const int mfvrseq)
{
  d->mfvrseq = mfvrseq;
}

int RunControl::mfvmaxi() const
{
  return d->mfvmaxi;
}

void RunControl::setMfvmaxi(const int mfvmaxi)
{
  d->mfvmaxi = mfvmaxi;
}

RX RunControl::mfvrcnvg() const
{
  return d->mfvrcnvg;
}

void RunControl::setMfvrcnvg(const RX mfvrcnvg)
{
  d->mfvrcnvg = mfvrcnvg;
}

RX RunControl::mfvacnvg() const
{
  return d->mfvacnvg;
}

void RunControl::setMfvacnvg(const RX mfvacnvg)
{
  d->mfvacnvg = mfvacnvg;
}

RX RunControl::mfvrelax() const
{
  return d->mfvrelax;
}

void RunControl::setMfvrelax(const RX mfvrelax)
{
  d->mfvrelax = mfvrelax;
}

int RunControl::uccv() const
{
  return d->uccv;
}

void RunControl::setUccv(const int uccv)
{
  d->uccv = uccv;
}

int RunControl::mf_solver() const
{
  return d->mf_solver;
}

void RunControl::setMf_solver(const int mf_solver)
{
  d->mf_solver = mf_solver;
}

int RunControl::sim_1dz() const
{
  return d->sim_1dz;
}

void RunControl::setSim_1dz(const int sim_1dz)
{
  d->sim_1dz = sim_1dz;
}

int RunControl::sim_1dd() const
{
  return d->sim_1dd;
}

void RunControl::setSim_1dd(const int sim_1dd)
{
  d->sim_1dd = sim_1dd;
}

RX RunControl::celldx() const
{
  return d->celldx;
}

void RunControl::setCelldx(const RX celldx)
{
  d->celldx = celldx;
}

int RunControl::sim_vjt() const
{
  return d->sim_vjt;
}

void RunControl::setSim_vjt(const int sim_vjt)
{
  d->sim_vjt = sim_vjt;
}

int RunControl::udx() const
{
  return d->udx;
}

void RunControl::setUdx(const int udx)
{
  d->udx = udx;
}

int RunControl::cvode_mth() const
{
  return d->cvode_mth;
}

void RunControl::setCvode_mth(const int cvode_mth)
{
  d->cvode_mth = cvode_mth;
}

RX RunControl::cvode_rcnvg() const
{
  return d->cvode_rcnvg;
}

void RunControl::setCvode_rcnvg(const RX cvode_rcnvg)
{
  d->cvode_rcnvg = cvode_rcnvg;
}

RX RunControl::cvode_acnvg() const
{
  return d->cvode_acnvg;
}

void RunControl::setCvode_acnvg(const RX cvode_acnvg)
{
  d->cvode_acnvg = cvode_acnvg;
}

RX RunControl::cvode_dtmax() const
{
  return d->cvode_dtmax;
}

void RunControl::setCvode_dtmax(const RX cvode_dtmax)
{
  d->cvode_dtmax = cvode_dtmax;
}

int RunControl::tsdens() const
{
  return d->tsdens;
}

void RunControl::setTsdens(const int tsdens)
{
  d->tsdens = tsdens;
}

RX RunControl::tsrelax() const
{
  return d->tsrelax;
}

void RunControl::setTsrelax(const RX tsrelax)
{
  d->tsrelax = tsrelax;
}

int RunControl::tsmaxi() const
{
  return d->tsmaxi;
}

void RunControl::setTsmaxi(const int tsmaxi)
{
  d->tsmaxi = tsmaxi;
}

int RunControl::cnvgSS() const
{
  return d->cnvgSS;
}

void RunControl::setCnvgSS(const int cnvgSS)
{
  d->cnvgSS = cnvgSS;
}

int RunControl::densZP() const
{
  return d->densZP;
}

void RunControl::setDensZP(const int densZP)
{
  d->densZP = densZP;
}

int RunControl::stackD() const
{
  return d->stackD;
}

void RunControl::setStackD(const int stackD)
{
  d->stackD = stackD;
}

int RunControl::dodMdt() const
{
  return d->dodMdt;
}

void RunControl::setDodMdt(const int dodMdt)
{
  d->dodMdt = dodMdt;
}

std::string RunControl::date_st() const
{
  return d->date_st;
}

void RunControl::setDate_st(const std::string date_st)
{
  d->date_st = date_st;
}

std::string RunControl::time_st() const
{
  return d->time_st;
}

void RunControl::setTime_st(const std::string time_st)
{
  d->time_st = time_st;
}

std::string RunControl::date_0() const
{
  return d->date_0;
}

void RunControl::setDate_0(const std::string date_0)
{
  d->date_0 = date_0;
}

std::string RunControl::time_0() const
{
  return d->time_0;
}

void RunControl::setTime_0(const std::string time_0)
{
  d->time_0 = time_0;
}

std::string RunControl::date_1() const
{
  return d->date_1;
}

void RunControl::setDate_1(const std::string date_1)
{
  d->date_1 = date_1;
}

std::string RunControl::time_1() const
{
  return d->time_1;
}

void RunControl::setTime_1(const std::string time_1)
{
  d->time_1 = time_1;
}

std::string RunControl::time_step() const
{
  return d->time_step;
}

void RunControl::setTime_step(const std::string time_step)
{
  d->time_step = time_step;
}

std::string RunControl::time_list() const
{
  return d->time_list;
}

void RunControl::setTime_list(const std::string time_list)
{
  d->time_list = time_list;
}

std::string RunControl::time_scrn() const
{
  return d->time_scrn;
}

void RunControl::setTime_scrn(const std::string time_scrn)
{
  d->time_scrn = time_scrn;
}

int RunControl::restart() const
{
  return d->restart;
}

void RunControl::setRestart(const int restart)
{
  d->restart = restart;
}

std::string RunControl::rstdate() const
{
  return d->rstdate;
}

void RunControl::setRstdate(const std::string rstdate)
{
  d->rstdate = rstdate;
}

std::string RunControl::rsttime() const
{
  return d->rsttime;
}

void RunControl::setRsttime(const std::string rsttime)
{
  d->rsttime = rsttime;
}

int RunControl::list() const
{
  return d->list;
}

void RunControl::setList(const int list)
{
  d->list = list;
}

int RunControl::doDlg() const
{
  return d->doDlg;
}

void RunControl::setDoDlg(const int doDlg)
{
  d->doDlg = doDlg;
}

int RunControl::pfsave() const
{
  return d->pfsave;
}

void RunControl::setPfsave(const int pfsave)
{
  d->pfsave = pfsave;
}

int RunControl::zfsave() const
{
  return d->zfsave;
}

void RunControl::setZfsave(const int zfsave)
{
  d->zfsave = zfsave;
}

int RunControl::zcsave() const
{
  return d->zcsave;
}

void RunControl::setZcsave(const int zcsave)
{
  d->zcsave = zcsave;
}

int RunControl::achvol() const
{
  return d->achvol;
}

void RunControl::setAchvol(const int achvol)
{
  d->achvol = achvol;
}

int RunControl::achsave() const
{
  return d->achsave;
}

void RunControl::setAchsave(const int achsave)
{
  d->achsave = achsave;
}

int RunControl::abwsave() const
{
  return d->abwsave;
}

void RunControl::setAbwsave(const int abwsave)
{
  d->abwsave = abwsave;
}

int RunControl::cbwsave() const
{
  return d->cbwsave;
}

void RunControl::setCbwsave(const int cbwsave)
{
  d->cbwsave = cbwsave;
}

int RunControl::expsave() const
{
  return d->expsave;
}

void RunControl::setExpsave(const int expsave)
{
  d->expsave = expsave;
}

int RunControl::ebwsave() const
{
  return d->ebwsave;
}

void RunControl::setEbwsave(const int ebwsave)
{
  d->ebwsave = ebwsave;
}

int RunControl::zaasave() const
{
  return d->zaasave;
}

void RunControl::setZaasave(const int zaasave)
{
  d->zaasave = zaasave;
}

int RunControl::zbwsave() const
{
  return d->zbwsave;
}

void RunControl::setZbwsave(const int zbwsave)
{
  d->zbwsave = zbwsave;
}

int RunControl::rzfsave() const
{
  return d->rzfsave;
}

void RunControl::setRzfsave(const int rzfsave)
{
  d->rzfsave = rzfsave;
}

int RunControl::rzmsave() const
{
  return d->rzmsave;
}

void RunControl::setRzmsave(const int rzmsave)
{
  d->rzmsave = rzmsave;
}

int RunControl::rz1save() const
{
  return d->rz1save;
}

void RunControl::setRz1save(const int rz1save)
{
  d->rz1save = rz1save;
}

int RunControl::csmsave() const
{
  return d->csmsave;
}

void RunControl::setCsmsave(const int csmsave)
{
  d->csmsave = csmsave;
}

int RunControl::srfsave() const
{
  return d->srfsave;
}

void RunControl::setSrfsave(const int srfsave)
{
  d->srfsave = srfsave;
}

int RunControl::logsave() const
{
  return d->logsave;
}

void RunControl::setLogsave(const int logsave)
{
  d->logsave = logsave;
}

std::vector<int> RunControl::save() const
{
  return d->save;
}

void RunControl::setSave(const std::vector<int> save)
{
  d->save = save;
}

std::vector<RX> RunControl::rvals() const
{
  return d->rvals;
}

void RunControl::setRvals(const std::vector<RX> rvals)
{
  d->rvals = rvals;
}

int RunControl::BldgFlowZ() const
{
  return d->BldgFlowZ;
}

void RunControl::setBldgFlowZ(const int BldgFlowZ)
{
  d->BldgFlowZ = BldgFlowZ;
}

int RunControl::BldgFlowD() const
{
  return d->BldgFlowD;
}

void RunControl::setBldgFlowD(const int BldgFlowD)
{
  d->BldgFlowD = BldgFlowD;
}

int RunControl::BldgFlowC() const
{
  return d->BldgFlowC;
}

void RunControl::setBldgFlowC(const int BldgFlowC)
{
  d->BldgFlowC = BldgFlowC;
}

int RunControl::cfd_ctype() const
{
  return d->cfd_ctype;
}

void RunControl::setCfd_ctype(const int cfd_ctype)
{
  d->cfd_ctype = cfd_ctype;
}

RX RunControl::cfd_convcpl() const
{
  return d->cfd_convcpl;
}

void RunControl::setCfd_convcpl(const RX cfd_convcpl)
{
  d->cfd_convcpl = cfd_convcpl;
}

int RunControl::cfd_var() const
{
  return d->cfd_var;
}

void RunControl::setCfd_var(const int cfd_var)
{
  d->cfd_var = cfd_var;
}

int RunControl::cfd_zref() const
{
  return d->cfd_zref;
}

void RunControl::setCfd_zref(const int cfd_zref)
{
  d->cfd_zref = cfd_zref;
}

int RunControl::cfd_imax() const
{
  return d->cfd_imax;
}

void RunControl::setCfd_imax(const int cfd_imax)
{
  d->cfd_imax = cfd_imax;
}

int RunControl::cfd_dtcmo() const
{
  return d->cfd_dtcmo;
}

void RunControl::setCfd_dtcmo(const int cfd_dtcmo)
{
  d->cfd_dtcmo = cfd_dtcmo;
}

Level::Level(int nr,RX refht,RX delht,int u_rfht,int u_dlht,std::string name,std::vector<Icon> icons)
{
  d = new LevelPrivate(nr,refht,delht,u_rfht,u_dlht,name,icons);
}

std::string Level::write()
{
  return d->write();
}

void Level::read(Reader &input)
{
  d->read(input);
}

int Level::nr() const
{
  return d->nr;
}

void Level::setNr(const int nr)
{
  d->nr = nr;
}

RX Level::refht() const
{
  return d->refht;
}

void Level::setRefht(const RX refht)
{
  d->refht = refht;
}

RX Level::delht() const
{
  return d->delht;
}

void Level::setDelht(const RX delht)
{
  d->delht = delht;
}

int Level::u_rfht() const
{
  return d->u_rfht;
}

void Level::setU_rfht(const int u_rfht)
{
  d->u_rfht = u_rfht;
}

int Level::u_dlht() const
{
  return d->u_dlht;
}

void Level::setU_dlht(const int u_dlht)
{
  d->u_dlht = u_dlht;
}

std::string Level::name() const
{
  return d->name;
}

void Level::setName(const std::string name)
{
  d->name = name;
}

std::vector<Icon> Level::icons() const
{
  return d->icons;
}

void Level::setIcons(const std::vector<Icon> icons)
{
  d->icons = icons;
}

DaySchedule::DaySchedule(int nr,int shape,int utyp,int ucnv,std::string name,std::string desc,std::vector<SchedulePoint> points)
{
  d = new DaySchedulePrivate(nr,shape,utyp,ucnv,name,desc,points);
}

std::string DaySchedule::write()
{
  return d->write();
}

void DaySchedule::read(Reader &input)
{
  d->read(input);
}

int DaySchedule::nr() const
{
  return d->nr;
}

void DaySchedule::setNr(const int nr)
{
  d->nr = nr;
}

int DaySchedule::shape() const
{
  return d->shape;
}

void DaySchedule::setShape(const int shape)
{
  d->shape = shape;
}

int DaySchedule::utyp() const
{
  return d->utyp;
}

void DaySchedule::setUtyp(const int utyp)
{
  d->utyp = utyp;
}

int DaySchedule::ucnv() const
{
  return d->ucnv;
}

void DaySchedule::setUcnv(const int ucnv)
{
  d->ucnv = ucnv;
}

std::string DaySchedule::name() const
{
  return d->name;
}

void DaySchedule::setName(const std::string name)
{
  d->name = name;
}

std::string DaySchedule::desc() const
{
  return d->desc;
}

void DaySchedule::setDesc(const std::string desc)
{
  d->desc = desc;
}

std::vector<SchedulePoint> DaySchedule::points() const
{
  return d->points;
}

void DaySchedule::setPoints(const std::vector<SchedulePoint> points)
{
  d->points = points;
}

WeekSchedule::WeekSchedule(int nr,int utyp,int ucnv,std::string name,std::string desc,std::vector<int> j)
{
  d = new WeekSchedulePrivate(nr,utyp,ucnv,name,desc,j);
}

std::string WeekSchedule::write()
{
  return d->write();
}

void WeekSchedule::read(Reader &input)
{
  d->read(input);
}

int WeekSchedule::nr() const
{
  return d->nr;
}

void WeekSchedule::setNr(const int nr)
{
  d->nr = nr;
}

int WeekSchedule::utyp() const
{
  return d->utyp;
}

void WeekSchedule::setUtyp(const int utyp)
{
  d->utyp = utyp;
}

int WeekSchedule::ucnv() const
{
  return d->ucnv;
}

void WeekSchedule::setUcnv(const int ucnv)
{
  d->ucnv = ucnv;
}

std::string WeekSchedule::name() const
{
  return d->name;
}

void WeekSchedule::setName(const std::string name)
{
  d->name = name;
}

std::string WeekSchedule::desc() const
{
  return d->desc;
}

void WeekSchedule::setDesc(const std::string desc)
{
  d->desc = desc;
}

std::vector<int> WeekSchedule::j() const
{
  return d->j;
}

void WeekSchedule::setJ(const std::vector<int> j)
{
  d->j = j;
}

WindPressureProfile::WindPressureProfile(int nr,int type,std::string name,std::string desc,std::vector<PressureCoefficientPoint> coeffs)
{
  d = new WindPressureProfilePrivate(nr,type,name,desc,coeffs);
}

std::string WindPressureProfile::write()
{
  return d->write();
}

void WindPressureProfile::read(Reader &input)
{
  d->read(input);
}

int WindPressureProfile::nr() const
{
  return d->nr;
}

void WindPressureProfile::setNr(const int nr)
{
  d->nr = nr;
}

int WindPressureProfile::type() const
{
  return d->type;
}

void WindPressureProfile::setType(const int type)
{
  d->type = type;
}

std::string WindPressureProfile::name() const
{
  return d->name;
}

void WindPressureProfile::setName(const std::string name)
{
  d->name = name;
}

std::string WindPressureProfile::desc() const
{
  return d->desc;
}

void WindPressureProfile::setDesc(const std::string desc)
{
  d->desc = desc;
}

std::vector<PressureCoefficientPoint> WindPressureProfile::coeffs() const
{
  return d->coeffs;
}

void WindPressureProfile::setCoeffs(const std::vector<PressureCoefficientPoint> coeffs)
{
  d->coeffs = coeffs;
}

ControlNode::Type ControlNode::convertTag(std::string string)
{
  std::string tags[37] = {std::string("sns"),std::string("sch"),std::string("set"),std::string("cvf"),std::string("dvf"),std::string("log"),std::string("pas"),std::string("mod"),std::string("hys"),std::string("abs"),std::string("bin"),std::string("dls"),std::string("dlx"),std::string("int"),std::string("rav"),std::string("inv"),std::string("and"),std::string("od"),std::string("xor"),std::string("add"),std::string("sub"),std::string("mul"),std::string("div"),std::string("sum"),std::string("avg"),std::string("max"),std::string("min"),std::string("lls"),std::string("uls"),std::string("lbs"),std::string("ubs"),std::string("llc"),std::string("ulc"),std::string("pc1"),std::string("pi1"),std::string("sup"),std::string("sph")};
  ControlNode::Type type[37] = {CT_SNS,CT_SCH,CT_SET,CT_CVF,CT_DVF,CT_LOG,CT_PAS,CT_MOD,CT_HYS,CT_ABS,CT_BIN,CT_DLS,CT_DLX,CT_INT,CT_RAV,CT_INV,CT_AND,CT_OR,CT_XOR,CT_ADD,CT_SUB,CT_MUL,CT_DIV,CT_SUM,CT_AVG,CT_MAX,CT_MIN,CT_LLS,CT_ULS,CT_LBS,CT_UBS,CT_LLC,CT_ULC,CT_PC1,CT_PI1,CT_SUP,CT_SPH};
  for(int i=0;i<37;i++)
    if(string==tags[i])
      return type[i];
  return ControlNode::UNKNOWN;
}

ControlNode* ControlNode::readElement(Reader &input)
{
  ControlNode *out=NULL;
  int nr = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  int seqnr = input.read<int>(FILELINE);
  unsigned int flags = input.read<unsigned int>(FILELINE);
  int inreq = input.read<int>(FILELINE);
  int n1 = input.read<int>(FILELINE);
  int n2 = input.read<int>(FILELINE);
  std::string name = input.readString(FILELINE);
  std::string desc = input.readLine(FILELINE);
  int kind = convertTag(dataType);
  switch(kind)
  {
  case ControlNode::CT_PAS:
    {
      PasDat *obj = new PasDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_ABS:
    {
      AbsDat *obj = new AbsDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_BIN:
    {
      BinDat *obj = new BinDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_INT:
    {
      IntDat *obj = new IntDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_INV:
    {
      InvDat *obj = new InvDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_AND:
    {
      AndDat *obj = new AndDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_OR:
    {
      OrDat *obj = new OrDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_XOR:
    {
      XorDat *obj = new XorDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_ADD:
    {
      AddDat *obj = new AddDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_SUB:
    {
      SubDat *obj = new SubDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_MUL:
    {
      MulDat *obj = new MulDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_DIV:
    {
      DivDat *obj = new DivDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_LLS:
    {
      LlsDat *obj = new LlsDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_ULS:
    {
      UlsDat *obj = new UlsDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_LLC:
    {
      LlcDat *obj = new LlcDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_ULC:
    {
      UlcDat *obj = new UlcDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_SPH:
    {
      SphDat *obj = new SphDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_SNS:
    {
      SnsDat *obj = new SnsDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_SCH:
    {
      SchDat *obj = new SchDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_SET:
    {
      SetDat *obj = new SetDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_CVF:
    {
      CvfDat *obj = new CvfDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_DVF:
    {
      DvfDat *obj = new DvfDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_LOG:
    {
      LogDat *obj = new LogDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_MOD:
    {
      ModDat *obj = new ModDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_HYS:
    {
      HysDat *obj = new HysDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_DLS:
    {
      DlsDat *obj = new DlsDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_DLX:
    {
      DlxDat *obj = new DlxDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_RAV:
    {
      RavDat *obj = new RavDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_SUM:
    {
      SumAvg *obj = new SumAvg(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_AVG:
    {
      AvgDat *obj = new AvgDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_MAX:
    {
      MaxDat *obj = new MaxDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_MIN:
    {
      MinDat *obj = new MinDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_LBS:
    {
      LbsDat *obj = new LbsDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_UBS:
    {
      UbsDat *obj = new UbsDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_PC1:
    {
      PcDat *obj = new PcDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_PI1:
    {
      PicDat *obj = new PicDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::CT_SUP:
    {
      SupDat *obj = new SupDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = (ControlNode*)obj;
      break;
    }
  case ControlNode::UNKNOWN:
  default:
    std::string mesg = "Unknown control node type '" + dataType + "' at line " + openstudio::toString(input.lineNumber());
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(__FILE__).arg(__LINE__).toStdString();
#endif
    LOG_FREE_AND_THROW("openstudio.contam.Reader",mesg);
  }
  return out;
}

CtrlDat::CtrlDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc)
{
  d = new CtrlDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc);
}

std::string CtrlDat::write()
{
  return d->write(dataType());
}

void CtrlDat::read(Reader &input)
{
  d->read(input);
}

int CtrlDat::nr() const
{
  return d->nr;
}

void CtrlDat::setNr(const int nr)
{
  d->nr = nr;
}

int CtrlDat::seqnr() const
{
  return d->seqnr;
}

void CtrlDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int CtrlDat::flags() const
{
  return d->flags;
}

void CtrlDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int CtrlDat::inreq() const
{
  return d->inreq;
}

void CtrlDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int CtrlDat::n1() const
{
  return d->n1;
}

void CtrlDat::setN1(const int n1)
{
  d->n1 = n1;
}

int CtrlDat::n2() const
{
  return d->n2;
}

void CtrlDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string CtrlDat::name() const
{
  return d->name;
}

void CtrlDat::setName(const std::string name)
{
  d->name = name;
}

std::string CtrlDat::desc() const
{
  return d->desc;
}

void CtrlDat::setDesc(const std::string desc)
{
  d->desc = desc;
}


SnsDat::SnsDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,RX offset,RX scale,RX tau,RX oldsig,int source,int type,int measure,RX X,RX Y,RX relHt,std::string units,std::string species)
{
  d = new SnsDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,offset,scale,tau,oldsig,source,type,measure,X,Y,relHt,units,species);
}

std::string SnsDat::write()
{
  return d->write();
}

void SnsDat::read(Reader &input)
{
  d->read(input);
}

void SnsDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int SnsDat::nr() const
{
  return d->nr;
}

void SnsDat::setNr(const int nr)
{
  d->nr = nr;
}

int SnsDat::seqnr() const
{
  return d->seqnr;
}

void SnsDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int SnsDat::flags() const
{
  return d->flags;
}

void SnsDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int SnsDat::inreq() const
{
  return d->inreq;
}

void SnsDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int SnsDat::n1() const
{
  return d->n1;
}

void SnsDat::setN1(const int n1)
{
  d->n1 = n1;
}

int SnsDat::n2() const
{
  return d->n2;
}

void SnsDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string SnsDat::name() const
{
  return d->name;
}

void SnsDat::setName(const std::string name)
{
  d->name = name;
}

std::string SnsDat::desc() const
{
  return d->desc;
}

void SnsDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX SnsDat::offset() const
{
  return d->offset;
}

void SnsDat::setOffset(const RX offset)
{
  d->offset = offset;
}

RX SnsDat::scale() const
{
  return d->scale;
}

void SnsDat::setScale(const RX scale)
{
  d->scale = scale;
}

RX SnsDat::tau() const
{
  return d->tau;
}

void SnsDat::setTau(const RX tau)
{
  d->tau = tau;
}

RX SnsDat::oldsig() const
{
  return d->oldsig;
}

void SnsDat::setOldsig(const RX oldsig)
{
  d->oldsig = oldsig;
}

int SnsDat::source() const
{
  return d->source;
}

void SnsDat::setSource(const int source)
{
  d->source = source;
}

int SnsDat::type() const
{
  return d->type;
}

void SnsDat::setType(const int type)
{
  d->type = type;
}

int SnsDat::measure() const
{
  return d->measure;
}

void SnsDat::setMeasure(const int measure)
{
  d->measure = measure;
}

RX SnsDat::X() const
{
  return d->X;
}

void SnsDat::setX(const RX X)
{
  d->X = X;
}

RX SnsDat::Y() const
{
  return d->Y;
}

void SnsDat::setY(const RX Y)
{
  d->Y = Y;
}

RX SnsDat::relHt() const
{
  return d->relHt;
}

void SnsDat::setRelHt(const RX relHt)
{
  d->relHt = relHt;
}

std::string SnsDat::units() const
{
  return d->units;
}

void SnsDat::setUnits(const std::string units)
{
  d->units = units;
}

std::string SnsDat::species() const
{
  return d->species;
}

void SnsDat::setSpecies(const std::string species)
{
  d->species = species;
}


SchDat::SchDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,int ps)
{
  d = new SchDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,ps);
}

std::string SchDat::write()
{
  return d->write();
}

void SchDat::read(Reader &input)
{
  d->read(input);
}

void SchDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int SchDat::nr() const
{
  return d->nr;
}

void SchDat::setNr(const int nr)
{
  d->nr = nr;
}

int SchDat::seqnr() const
{
  return d->seqnr;
}

void SchDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int SchDat::flags() const
{
  return d->flags;
}

void SchDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int SchDat::inreq() const
{
  return d->inreq;
}

void SchDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int SchDat::n1() const
{
  return d->n1;
}

void SchDat::setN1(const int n1)
{
  d->n1 = n1;
}

int SchDat::n2() const
{
  return d->n2;
}

void SchDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string SchDat::name() const
{
  return d->name;
}

void SchDat::setName(const std::string name)
{
  d->name = name;
}

std::string SchDat::desc() const
{
  return d->desc;
}

void SchDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

int SchDat::ps() const
{
  return d->ps;
}

void SchDat::setPs(const int ps)
{
  d->ps = ps;
}


SetDat::SetDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,RX value)
{
  d = new SetDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,value);
}

std::string SetDat::write()
{
  return d->write();
}

void SetDat::read(Reader &input)
{
  d->read(input);
}

void SetDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int SetDat::nr() const
{
  return d->nr;
}

void SetDat::setNr(const int nr)
{
  d->nr = nr;
}

int SetDat::seqnr() const
{
  return d->seqnr;
}

void SetDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int SetDat::flags() const
{
  return d->flags;
}

void SetDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int SetDat::inreq() const
{
  return d->inreq;
}

void SetDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int SetDat::n1() const
{
  return d->n1;
}

void SetDat::setN1(const int n1)
{
  d->n1 = n1;
}

int SetDat::n2() const
{
  return d->n2;
}

void SetDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string SetDat::name() const
{
  return d->name;
}

void SetDat::setName(const std::string name)
{
  d->name = name;
}

std::string SetDat::desc() const
{
  return d->desc;
}

void SetDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX SetDat::value() const
{
  return d->value;
}

void SetDat::setValue(const RX value)
{
  d->value = value;
}


CdvDat::CdvDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,std::string valuename)
{
  d = new CdvDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,valuename);
}

std::string CdvDat::write()
{
  return d->write(dataType());
}

void CdvDat::read(Reader &input)
{
  d->read(input);
}

void CdvDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int CdvDat::nr() const
{
  return d->nr;
}

void CdvDat::setNr(const int nr)
{
  d->nr = nr;
}

int CdvDat::seqnr() const
{
  return d->seqnr;
}

void CdvDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int CdvDat::flags() const
{
  return d->flags;
}

void CdvDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int CdvDat::inreq() const
{
  return d->inreq;
}

void CdvDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int CdvDat::n1() const
{
  return d->n1;
}

void CdvDat::setN1(const int n1)
{
  d->n1 = n1;
}

int CdvDat::n2() const
{
  return d->n2;
}

void CdvDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string CdvDat::name() const
{
  return d->name;
}

void CdvDat::setName(const std::string name)
{
  d->name = name;
}

std::string CdvDat::desc() const
{
  return d->desc;
}

void CdvDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

std::string CdvDat::valuename() const
{
  return d->valuename;
}

void CdvDat::setValuename(const std::string valuename)
{
  d->valuename = valuename;
}


LogDat::LogDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,RX offset,RX scale,int udef,std::string header,std::string units)
{
  d = new LogDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,offset,scale,udef,header,units);
}

std::string LogDat::write()
{
  return d->write();
}

void LogDat::read(Reader &input)
{
  d->read(input);
}

void LogDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int LogDat::nr() const
{
  return d->nr;
}

void LogDat::setNr(const int nr)
{
  d->nr = nr;
}

int LogDat::seqnr() const
{
  return d->seqnr;
}

void LogDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int LogDat::flags() const
{
  return d->flags;
}

void LogDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int LogDat::inreq() const
{
  return d->inreq;
}

void LogDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int LogDat::n1() const
{
  return d->n1;
}

void LogDat::setN1(const int n1)
{
  d->n1 = n1;
}

int LogDat::n2() const
{
  return d->n2;
}

void LogDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string LogDat::name() const
{
  return d->name;
}

void LogDat::setName(const std::string name)
{
  d->name = name;
}

std::string LogDat::desc() const
{
  return d->desc;
}

void LogDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX LogDat::offset() const
{
  return d->offset;
}

void LogDat::setOffset(const RX offset)
{
  d->offset = offset;
}

RX LogDat::scale() const
{
  return d->scale;
}

void LogDat::setScale(const RX scale)
{
  d->scale = scale;
}

int LogDat::udef() const
{
  return d->udef;
}

void LogDat::setUdef(const int udef)
{
  d->udef = udef;
}

std::string LogDat::header() const
{
  return d->header;
}

void LogDat::setHeader(const std::string header)
{
  d->header = header;
}

std::string LogDat::units() const
{
  return d->units;
}

void LogDat::setUnits(const std::string units)
{
  d->units = units;
}


ModDat::ModDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,RX offset,RX scale)
{
  d = new ModDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,offset,scale);
}

std::string ModDat::write()
{
  return d->write();
}

void ModDat::read(Reader &input)
{
  d->read(input);
}

void ModDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int ModDat::nr() const
{
  return d->nr;
}

void ModDat::setNr(const int nr)
{
  d->nr = nr;
}

int ModDat::seqnr() const
{
  return d->seqnr;
}

void ModDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int ModDat::flags() const
{
  return d->flags;
}

void ModDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int ModDat::inreq() const
{
  return d->inreq;
}

void ModDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int ModDat::n1() const
{
  return d->n1;
}

void ModDat::setN1(const int n1)
{
  d->n1 = n1;
}

int ModDat::n2() const
{
  return d->n2;
}

void ModDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string ModDat::name() const
{
  return d->name;
}

void ModDat::setName(const std::string name)
{
  d->name = name;
}

std::string ModDat::desc() const
{
  return d->desc;
}

void ModDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX ModDat::offset() const
{
  return d->offset;
}

void ModDat::setOffset(const RX offset)
{
  d->offset = offset;
}

RX ModDat::scale() const
{
  return d->scale;
}

void ModDat::setScale(const RX scale)
{
  d->scale = scale;
}


HysDat::HysDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,RX slack,RX slope,RX oldsig)
{
  d = new HysDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,slack,slope,oldsig);
}

std::string HysDat::write()
{
  return d->write();
}

void HysDat::read(Reader &input)
{
  d->read(input);
}

void HysDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int HysDat::nr() const
{
  return d->nr;
}

void HysDat::setNr(const int nr)
{
  d->nr = nr;
}

int HysDat::seqnr() const
{
  return d->seqnr;
}

void HysDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int HysDat::flags() const
{
  return d->flags;
}

void HysDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int HysDat::inreq() const
{
  return d->inreq;
}

void HysDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int HysDat::n1() const
{
  return d->n1;
}

void HysDat::setN1(const int n1)
{
  d->n1 = n1;
}

int HysDat::n2() const
{
  return d->n2;
}

void HysDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string HysDat::name() const
{
  return d->name;
}

void HysDat::setName(const std::string name)
{
  d->name = name;
}

std::string HysDat::desc() const
{
  return d->desc;
}

void HysDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX HysDat::slack() const
{
  return d->slack;
}

void HysDat::setSlack(const RX slack)
{
  d->slack = slack;
}

RX HysDat::slope() const
{
  return d->slope;
}

void HysDat::setSlope(const RX slope)
{
  d->slope = slope;
}

RX HysDat::oldsig() const
{
  return d->oldsig;
}

void HysDat::setOldsig(const RX oldsig)
{
  d->oldsig = oldsig;
}


DlsDat::DlsDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,int dsincr,int dsdecr)
{
  d = new DlsDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,dsincr,dsdecr);
}

std::string DlsDat::write()
{
  return d->write();
}

void DlsDat::read(Reader &input)
{
  d->read(input);
}

void DlsDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int DlsDat::nr() const
{
  return d->nr;
}

void DlsDat::setNr(const int nr)
{
  d->nr = nr;
}

int DlsDat::seqnr() const
{
  return d->seqnr;
}

void DlsDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int DlsDat::flags() const
{
  return d->flags;
}

void DlsDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int DlsDat::inreq() const
{
  return d->inreq;
}

void DlsDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int DlsDat::n1() const
{
  return d->n1;
}

void DlsDat::setN1(const int n1)
{
  d->n1 = n1;
}

int DlsDat::n2() const
{
  return d->n2;
}

void DlsDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string DlsDat::name() const
{
  return d->name;
}

void DlsDat::setName(const std::string name)
{
  d->name = name;
}

std::string DlsDat::desc() const
{
  return d->desc;
}

void DlsDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

int DlsDat::dsincr() const
{
  return d->dsincr;
}

void DlsDat::setDsincr(const int dsincr)
{
  d->dsincr = dsincr;
}

int DlsDat::dsdecr() const
{
  return d->dsdecr;
}

void DlsDat::setDsdecr(const int dsdecr)
{
  d->dsdecr = dsdecr;
}


DlxDat::DlxDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,int tauincr,int taudecr)
{
  d = new DlxDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,tauincr,taudecr);
}

std::string DlxDat::write()
{
  return d->write();
}

void DlxDat::read(Reader &input)
{
  d->read(input);
}

void DlxDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int DlxDat::nr() const
{
  return d->nr;
}

void DlxDat::setNr(const int nr)
{
  d->nr = nr;
}

int DlxDat::seqnr() const
{
  return d->seqnr;
}

void DlxDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int DlxDat::flags() const
{
  return d->flags;
}

void DlxDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int DlxDat::inreq() const
{
  return d->inreq;
}

void DlxDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int DlxDat::n1() const
{
  return d->n1;
}

void DlxDat::setN1(const int n1)
{
  d->n1 = n1;
}

int DlxDat::n2() const
{
  return d->n2;
}

void DlxDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string DlxDat::name() const
{
  return d->name;
}

void DlxDat::setName(const std::string name)
{
  d->name = name;
}

std::string DlxDat::desc() const
{
  return d->desc;
}

void DlxDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

int DlxDat::tauincr() const
{
  return d->tauincr;
}

void DlxDat::setTauincr(const int tauincr)
{
  d->tauincr = tauincr;
}

int DlxDat::taudecr() const
{
  return d->taudecr;
}

void DlxDat::setTaudecr(const int taudecr)
{
  d->taudecr = taudecr;
}


RavDat::RavDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,int tspan)
{
  d = new RavDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,tspan);
}

std::string RavDat::write()
{
  return d->write();
}

void RavDat::read(Reader &input)
{
  d->read(input);
}

void RavDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int RavDat::nr() const
{
  return d->nr;
}

void RavDat::setNr(const int nr)
{
  d->nr = nr;
}

int RavDat::seqnr() const
{
  return d->seqnr;
}

void RavDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int RavDat::flags() const
{
  return d->flags;
}

void RavDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int RavDat::inreq() const
{
  return d->inreq;
}

void RavDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int RavDat::n1() const
{
  return d->n1;
}

void RavDat::setN1(const int n1)
{
  d->n1 = n1;
}

int RavDat::n2() const
{
  return d->n2;
}

void RavDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string RavDat::name() const
{
  return d->name;
}

void RavDat::setName(const std::string name)
{
  d->name = name;
}

std::string RavDat::desc() const
{
  return d->desc;
}

void RavDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

int RavDat::tspan() const
{
  return d->tspan;
}

void RavDat::setTspan(const int tspan)
{
  d->tspan = tspan;
}


SumAvg::SumAvg(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,std::vector<int> pc)
{
  d = new SumAvgPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,pc);
}

std::string SumAvg::write()
{
  return d->write(dataType());
}

void SumAvg::read(Reader &input)
{
  d->read(input);
}

void SumAvg::readDetails(Reader &input)
{
  d->readDetails(input);
}

int SumAvg::nr() const
{
  return d->nr;
}

void SumAvg::setNr(const int nr)
{
  d->nr = nr;
}

int SumAvg::seqnr() const
{
  return d->seqnr;
}

void SumAvg::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int SumAvg::flags() const
{
  return d->flags;
}

void SumAvg::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int SumAvg::inreq() const
{
  return d->inreq;
}

void SumAvg::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int SumAvg::n1() const
{
  return d->n1;
}

void SumAvg::setN1(const int n1)
{
  d->n1 = n1;
}

int SumAvg::n2() const
{
  return d->n2;
}

void SumAvg::setN2(const int n2)
{
  d->n2 = n2;
}

std::string SumAvg::name() const
{
  return d->name;
}

void SumAvg::setName(const std::string name)
{
  d->name = name;
}

std::string SumAvg::desc() const
{
  return d->desc;
}

void SumAvg::setDesc(const std::string desc)
{
  d->desc = desc;
}

std::vector<int> SumAvg::pc() const
{
  return d->pc;
}

void SumAvg::setPc(const std::vector<int> pc)
{
  d->pc = pc;
}


BanDat::BanDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,RX band)
{
  d = new BanDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,band);
}

std::string BanDat::write()
{
  return d->write(dataType());
}

void BanDat::read(Reader &input)
{
  d->read(input);
}

void BanDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int BanDat::nr() const
{
  return d->nr;
}

void BanDat::setNr(const int nr)
{
  d->nr = nr;
}

int BanDat::seqnr() const
{
  return d->seqnr;
}

void BanDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int BanDat::flags() const
{
  return d->flags;
}

void BanDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int BanDat::inreq() const
{
  return d->inreq;
}

void BanDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int BanDat::n1() const
{
  return d->n1;
}

void BanDat::setN1(const int n1)
{
  d->n1 = n1;
}

int BanDat::n2() const
{
  return d->n2;
}

void BanDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string BanDat::name() const
{
  return d->name;
}

void BanDat::setName(const std::string name)
{
  d->name = name;
}

std::string BanDat::desc() const
{
  return d->desc;
}

void BanDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX BanDat::band() const
{
  return d->band;
}

void BanDat::setBand(const RX band)
{
  d->band = band;
}


PcDat::PcDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,RX kp)
{
  d = new PcDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,kp);
}

std::string PcDat::write()
{
  return d->write();
}

void PcDat::read(Reader &input)
{
  d->read(input);
}

void PcDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PcDat::nr() const
{
  return d->nr;
}

void PcDat::setNr(const int nr)
{
  d->nr = nr;
}

int PcDat::seqnr() const
{
  return d->seqnr;
}

void PcDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int PcDat::flags() const
{
  return d->flags;
}

void PcDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int PcDat::inreq() const
{
  return d->inreq;
}

void PcDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int PcDat::n1() const
{
  return d->n1;
}

void PcDat::setN1(const int n1)
{
  d->n1 = n1;
}

int PcDat::n2() const
{
  return d->n2;
}

void PcDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string PcDat::name() const
{
  return d->name;
}

void PcDat::setName(const std::string name)
{
  d->name = name;
}

std::string PcDat::desc() const
{
  return d->desc;
}

void PcDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PcDat::kp() const
{
  return d->kp;
}

void PcDat::setKp(const RX kp)
{
  d->kp = kp;
}


PicDat::PicDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,RX kp,RX ki,RX oldsig,RX olderr)
{
  d = new PicDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,kp,ki,oldsig,olderr);
}

std::string PicDat::write()
{
  return d->write();
}

void PicDat::read(Reader &input)
{
  d->read(input);
}

void PicDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PicDat::nr() const
{
  return d->nr;
}

void PicDat::setNr(const int nr)
{
  d->nr = nr;
}

int PicDat::seqnr() const
{
  return d->seqnr;
}

void PicDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int PicDat::flags() const
{
  return d->flags;
}

void PicDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int PicDat::inreq() const
{
  return d->inreq;
}

void PicDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int PicDat::n1() const
{
  return d->n1;
}

void PicDat::setN1(const int n1)
{
  d->n1 = n1;
}

int PicDat::n2() const
{
  return d->n2;
}

void PicDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string PicDat::name() const
{
  return d->name;
}

void PicDat::setName(const std::string name)
{
  d->name = name;
}

std::string PicDat::desc() const
{
  return d->desc;
}

void PicDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PicDat::kp() const
{
  return d->kp;
}

void PicDat::setKp(const RX kp)
{
  d->kp = kp;
}

RX PicDat::ki() const
{
  return d->ki;
}

void PicDat::setKi(const RX ki)
{
  d->ki = ki;
}

RX PicDat::oldsig() const
{
  return d->oldsig;
}

void PicDat::setOldsig(const RX oldsig)
{
  d->oldsig = oldsig;
}

RX PicDat::olderr() const
{
  return d->olderr;
}

void PicDat::setOlderr(const RX olderr)
{
  d->olderr = olderr;
}


SupDat::SupDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,int def,int se,int in,int out)
{
  d = new SupDatPrivate(nr,seqnr,flags,inreq,n1,n2,name,desc,def,se,in,out);
}

std::string SupDat::write()
{
  return d->write();
}

void SupDat::read(Reader &input)
{
  d->read(input);
}

void SupDat::readDetails(Reader &input)
{
  d->readDetails(input);
}

int SupDat::nr() const
{
  return d->nr;
}

void SupDat::setNr(const int nr)
{
  d->nr = nr;
}

int SupDat::seqnr() const
{
  return d->seqnr;
}

void SupDat::setSeqnr(const int seqnr)
{
  d->seqnr = seqnr;
}

unsigned int SupDat::flags() const
{
  return d->flags;
}

void SupDat::setFlags(const unsigned int flags)
{
  d->flags = flags;
}

int SupDat::inreq() const
{
  return d->inreq;
}

void SupDat::setInreq(const int inreq)
{
  d->inreq = inreq;
}

int SupDat::n1() const
{
  return d->n1;
}

void SupDat::setN1(const int n1)
{
  d->n1 = n1;
}

int SupDat::n2() const
{
  return d->n2;
}

void SupDat::setN2(const int n2)
{
  d->n2 = n2;
}

std::string SupDat::name() const
{
  return d->name;
}

void SupDat::setName(const std::string name)
{
  d->name = name;
}

std::string SupDat::desc() const
{
  return d->desc;
}

void SupDat::setDesc(const std::string desc)
{
  d->desc = desc;
}

int SupDat::def() const
{
  return d->def;
}

void SupDat::setDef(const int def)
{
  d->def = def;
}

int SupDat::se() const
{
  return d->se;
}

void SupDat::setSe(const int se)
{
  d->se = se;
}

int SupDat::in() const
{
  return d->in;
}

void SupDat::setIn(const int in)
{
  d->in = in;
}

int SupDat::out() const
{
  return d->out;
}

void SupDat::setOut(const int out)
{
  d->out = out;
}

} // contam
} // openstudio
