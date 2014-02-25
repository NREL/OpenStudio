/**********************************************************************
*  Copyright (c) 2014, Alliance for Sustainable Energy.
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

Zone::Zone()
{
  d = new ZoneImpl();
}

Zone::Zone(int nr,unsigned int flags,int ps,int pc,int pk,int pl,std::string relHt,std::string Vol,std::string T0,std::string P0,
           std::string name,int color,int u_Ht,int u_V,int u_T,int u_P,int cdaxis,int cfd,std::string cfdname,std::string X1,
           std::string Y1,std::string H1,std::string X2,std::string Y2,std::string H2,std::string celldx,std::string axialD,int u_aD,int u_L)
{
  d = new ZoneImpl(nr,flags,ps,pc,pk,pl,relHt,Vol,T0,P0,name,color,u_Ht,u_V,u_T,u_P,cdaxis,cfd,cfdname,X1,
    Y1,H1,X2,Y2,H2,celldx,axialD,u_aD,u_L);
}

Zone::Zone(int nr,unsigned int flags,int ps,int pc,int pk,int pl,double relHt,double Vol,double T0,double P0,
           std::string name,int color,int u_Ht,int u_V,int u_T,int u_P,int cdaxis,int cfd,std::string cfdname,double X1,
           double Y1,double H1,double X2,double Y2,double H2,double celldx,double axialD,int u_aD,int u_L)
{
  d = new ZoneImpl(nr,flags,ps,pc,pk,pl,relHt,Vol,T0,P0,name,color,u_Ht,u_V,u_T,u_P,cdaxis,cfd,cfdname,X1,
    Y1,H1,X2,Y2,H2,celldx,axialD,u_aD,u_L);
}

Zone::Zone(const Zone &other) : d(other.d)
{}

Zone::~Zone()
{}

Zone& Zone::operator=(const Zone &other)
{
  d = other.d;
  return *this;
}

bool Zone::operator==(const Zone &other) const
{
  return d==other.d;
}

bool Zone::operator!=(const Zone &other) const
{
  return d!=other.d;
}

int Zone::nr() const
{
  return d->nr();
}

void Zone::setNr(const int nr)
{
  d->setNr(nr);
}

unsigned int Zone::flags() const
{
  return d->flags();
}

void Zone::setFlags(const unsigned int flags)
{
  d->setFlags(flags);
}

int Zone::ps() const
{
  return d->ps();
}

void Zone::setPs(const int ps)
{
  d->setPs(ps);
}

int Zone::pc() const
{
  return d->pc();
}

void Zone::setPc(const int pc)
{
  d->setPc(pc);
}

int Zone::pk() const
{
  return d->pk();
}

void Zone::setPk(const int pk)
{
  d->setPk(pk);
}

int Zone::pl() const
{
  return d->pl();
}

void Zone::setPl(const int pl)
{
  d->setPl(pl);
}

double Zone::relHt() const
{
  return d->relHt();
}

bool Zone::setRelHt(const double relHt)
{
  return d->setRelHt(relHt);
}

bool Zone::setRelHt(const std::string &relHt)
{
  return d->setRelHt(relHt);
}

double Zone::Vol() const
{
  return d->Vol();
}

bool Zone::setVol(const double Vol)
{
  return d->setVol(Vol);
}

bool Zone::setVol(const std::string &Vol)
{
  return d->setVol(Vol);
}

double Zone::T0() const
{
  return d->T0();
}

bool Zone::setT0(const double T0)
{
  return d->setT0(T0);
}

bool Zone::setT0(const std::string &T0)
{
  return d->setT0(T0);
}

double Zone::P0() const
{
  return d->P0();
}

bool Zone::setP0(const double P0)
{
  return d->setP0(P0);
}

bool Zone::setP0(const std::string &P0)
{
  return d->setP0(P0);
}

std::string Zone::name() const
{
  return d->name();
}

void Zone::setName(const std::string &name)
{
  d->setName(name);
}

int Zone::color() const
{
  return d->color();
}

void Zone::setColor(const int color)
{
  d->setColor(color);
}

int Zone::u_Ht() const
{
  return d->u_Ht();
}

void Zone::setU_Ht(const int u_Ht)
{
  d->setU_Ht(u_Ht);
}

int Zone::u_V() const
{
  return d->u_V();
}

void Zone::setU_V(const int u_V)
{
  d->setU_V(u_V);
}

int Zone::u_T() const
{
  return d->u_T();
}

void Zone::setU_T(const int u_T)
{
  d->setU_T(u_T);
}

int Zone::u_P() const
{
  return d->u_P();
}

void Zone::setU_P(const int u_P)
{
  d->setU_P(u_P);
}

int Zone::cdaxis() const
{
  return d->cdaxis();
}

void Zone::setCdaxis(const int cdaxis)
{
  d->setCdaxis(cdaxis);
}

int Zone::cfd() const
{
  return d->cfd();
}

void Zone::setCfd(const int cfd)
{
  d->setCfd(cfd);
}

std::string Zone::cfdname() const
{
  return d->cfdname();
}

void Zone::setCfdname(const std::string cfdname)
{
  d->setCfdname(cfdname);
}

double Zone::X1() const
{
  return d->X1();
}

bool Zone::setX1(const double X1)
{
  return d->setX1(X1);
}

bool Zone::setX1(const std::string &X1)
{
  return d->setX1(X1);
}

double Zone::Y1() const
{
  return d->Y1();
}

bool Zone::setY1(const double Y1)
{
  return d->setY1(Y1);
}

bool Zone::setY1(const std::string &Y1)
{
  return d->setY1(Y1);
}

double Zone::H1() const
{
  return d->H1();
}

bool Zone::setH1(const double H1)
{
  return d->setH1(H1);
}

bool Zone::setH1(const std::string &H1)
{
  return d->setH1(H1);
}

double Zone::X2() const
{
  return d->X2();
}

bool Zone::setX2(const double X2)
{
  return d->setX2(X2);
}

bool Zone::setX2(const std::string &X2)
{
  return d->setX2(X2);
}

double Zone::Y2() const
{
  return d->Y2();
}

bool Zone::setY2(const double Y2)
{
  return d->setY2(Y2);
}

bool Zone::setY2(const std::string &Y2)
{
  return d->setY2(Y2);
}

double Zone::H2() const
{
  return d->H2();
}

bool Zone::setH2(const double H2)
{
  return d->setH2(H2);
}

bool Zone::setH2(const std::string &H2)
{
  return d->setH2(H2);
}

double Zone::celldx() const
{
  return d->celldx();
}

bool Zone::setCelldx(const double celldx)
{
  return d->setCelldx(celldx);
}

bool Zone::setCelldx(const std::string &celldx)
{
  return d->setCelldx(celldx);
}

double Zone::axialD() const
{
  return d->axialD();
}

bool Zone::setAxialD(const double axialD)
{
  return d->setAxialD(axialD);
}

bool Zone::setAxialD(const std::string &axialD)
{
  return d->setAxialD(axialD);
}

int Zone::u_aD() const
{
  return d->u_aD();
}

void Zone::setU_aD(const int u_aD)
{
  return d->setU_aD(u_aD);
}

int Zone::u_L() const
{
  return d->u_L();
}

void Zone::setU_L(const int u_L)
{
  return d->setU_L(u_L);
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
  d->setVariablePressure(b);
}

bool Zone::variablePressure() const
{
  return d->variablePressure();
}

void Zone::setVariableContaminants(bool b)
{
  d->setVariableContaminants(b);
}

bool Zone::variableContaminants() const
{
  return d->variableContaminants();
}

void Zone::setSystem(bool b)
{
  d->setSystem(b);
}

bool Zone::system() const
{
  return d->system();
}

double Zone::ic(const int i) const
{
  return d->ic(i);
}

std::vector<double> Zone::ic() const
{
  return d->ic();
}

bool Zone::setIc(const int i, const double value)
{
  return d->setIc(i,value);
}

bool Zone::setIc(const int i, const std::string &value)
{
  return d->setIc(i,value);
}

bool Zone::setIc(std::vector<double> &ic)
{
  return d->setIc(ic);
}

bool Zone::setIc(std::vector<std::string> &ic)
{
  return d->setIc(ic);
}

Species::Species()
{
  d = new SpeciesImpl;
}

Species::Species(int nr,int sflag,int ntflag,std::string molwt,std::string mdiam,std::string edens,std::string decay,std::string Dm,
                 std::string ccdef,std::string Cp,int ucc,int umd,int ued,int udm,int ucp,std::string name,std::string desc)
{
  d = new SpeciesImpl(nr,sflag,ntflag,molwt,mdiam,edens,decay,Dm,ccdef,Cp,ucc,umd,ued,udm,ucp,name,desc);
}

Species::Species(int nr,int sflag,int ntflag,double molwt,double mdiam,double edens,double decay,double Dm,
                 double ccdef,double Cp,int ucc,int umd,int ued,int udm,int ucp,std::string name,std::string desc)
{
  d = new SpeciesImpl(nr,sflag,ntflag,molwt,mdiam,edens,decay,Dm,ccdef,Cp,ucc,umd,ued,udm,ucp,name,desc);
}

Species::Species(const Species &other) : d(other.d)
{}

Species::~Species()
{}

Species& Species::operator=(const Species &other)
{
  d = other.d;
  return *this;
}

bool Species::operator==(const Species &other) const
{
  return d==other.d;
}

bool Species::operator!=(const Species &other) const
{
  return d!=other.d;
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
  return d->nr();
}

void Species::setNr(const int nr)
{
  d->setNr(nr);
}

int Species::sflag() const
{
  return d->sflag();
}

void Species::setSflag(const int sflag)
{
  d->setSflag(sflag);
}

int Species::ntflag() const
{
  return d->ntflag();
}

void Species::setNtflag(const int ntflag)
{
  d->setNtflag(ntflag);
}

double Species::molwt() const
{
  return d->molwt();
}

bool Species::setMolwt(const double molwt)
{
  return d->setMolwt(molwt);
}

bool Species::setMolwt(const std::string &molwt)
{
  return d->setMolwt(molwt);
}

double Species::mdiam() const
{
  return d->mdiam();
}

bool Species::setMdiam(const double mdiam)
{
  return d->setMdiam(mdiam);
}

bool Species::setMdiam(const std::string &mdiam)
{
  return d->setMdiam(mdiam);
}

double Species::edens() const
{
  return d->edens();
}

bool Species::setEdens(const double edens)
{
  return d->setEdens(edens);
}

bool Species::setEdens(const std::string &edens)
{
  return d->setEdens(edens);
}

double Species::decay() const
{
  return d->decay();
}

bool Species::setDecay(const double decay)
{
  return d->setDecay(decay);
}

bool Species::setDecay(const std::string &decay)
{
  return d->setDecay(decay);
}

double Species::Dm() const
{
  return d->Dm();
}

bool Species::setDm(const double Dm)
{
  return d->setDm(Dm);
}

bool Species::setDm(const std::string &Dm)
{
  return d->setDm(Dm);
}

double Species::ccdef() const
{
  return d->ccdef();
}

bool Species::setCcdef(const double ccdef)
{
  return d->setCcdef(ccdef);
}

bool Species::setCcdef(const std::string &ccdef)
{
  return d->setCcdef(ccdef);
}

double Species::Cp() const
{
  return d->Cp();
}

bool Species::setCp(const double Cp)
{
  return d->setCp(Cp);
}

bool Species::setCp(const std::string &Cp)
{
  return d->setCp(Cp);
}

int Species::ucc() const
{
  return d->ucc();
}

void Species::setUcc(const int ucc)
{
  d->setUcc(ucc);
}

int Species::umd() const
{
  return d->umd();
}

void Species::setUmd(const int umd)
{
  d->setUmd(umd);
}

int Species::ued() const
{
  return d->ued();
}

void Species::setUed(const int ued)
{
  d->setUed(ued);
}

int Species::udm() const
{
  return d->udm();
}

void Species::setUdm(const int udm)
{
  d->setUdm(udm);
}

int Species::ucp() const
{
  return d->ucp();
}

void Species::setUcp(const int ucp)
{
  d->setUcp(ucp);
}

std::string Species::name() const
{
  return d->name();
}

void Species::setName(const std::string &name)
{
  d->setName(name);
}

std::string Species::desc() const
{
  return d->desc();
}

void Species::setDesc(const std::string &desc)
{
  d->setDesc(desc);
}

Ahs::Ahs()
{
  d = new AhsImpl;
}

Ahs::Ahs(int nr,int zone_r,int zone_s,int path_r,int path_s,int path_x,std::string name,std::string desc)
{
  d = new AhsImpl(nr,zone_r,zone_s,path_r,path_s,path_x,name,desc);
}

Ahs::Ahs(const Ahs &other) : d(other.d)
{}

Ahs::~Ahs()
{}

Ahs& Ahs::operator=(const Ahs &other)
{
  d = other.d;
  return *this;
}

bool Ahs::operator==(const Ahs &other) const
{
  return d==other.d;
}

bool Ahs::operator!=(const Ahs &other) const
{
  return d!=other.d;
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
  return d->nr();
}

void Ahs::setNr(const int nr)
{
  d->setNr(nr);
}

int Ahs::zone_r() const
{
  return d->zone_r();
}

void Ahs::setZone_r(const int zone_r)
{
  d->setZone_r(zone_r);
}

int Ahs::zone_s() const
{
  return d->zone_s();
}

void Ahs::setZone_s(const int zone_s)
{
  d->setZone_s(zone_s);
}

int Ahs::path_r() const
{
  return d->path_r();
}

void Ahs::setPath_r(const int path_r)
{
  d->setPath_r(path_r);
}

int Ahs::path_s() const
{
  return d->path_s();
}

void Ahs::setPath_s(const int path_s)
{
  d->setPath_s(path_s);
}

int Ahs::path_x() const
{
  return d->path_x();
}

void Ahs::setPath_x(const int path_x)
{
  d->setPath_x(path_x);
}

std::string Ahs::name() const
{
  return d->name();
}

void Ahs::setName(const std::string &name)
{
  d->setName(name);
}

std::string Ahs::desc() const
{
  return d->desc();
}

void Ahs::setDesc(const std::string &desc)
{
  d->setDesc(desc);
}

Path::Path()
{
  d = new PathImpl;
}

Path::Path(int nr,int flags,int pzn,int pzm,int pe,int pf,int pw,int pa,int ps,int pc,int pld,std::string X,
           std::string Y,std::string relHt,std::string mult,std::string wPset,std::string wPmod,std::string wazm,std::string Fahs,
           std::string Xmax,std::string Xmin,unsigned int icon,unsigned int dir,int u_Ht,int u_XY,int u_dP,
           int u_F,int cfd,std::string cfd_name,int cfd_ptype,int cfd_btype,int cfd_capp)
{
  d = new PathImpl(nr,flags,pzn,pzm,pe,pf,pw,pa,ps,pc,pld,X,Y,relHt,mult,wPset,wPmod,wazm,Fahs,
    Xmax,Xmin,icon,dir,u_Ht,u_XY,u_dP,u_F,cfd,cfd_name,cfd_ptype,cfd_btype,cfd_capp);
}

Path::Path(int nr,int flags,int pzn,int pzm,int pe,int pf,int pw,int pa,int ps,int pc,int pld,double X,
           double Y,double relHt,double mult,double wPset,double wPmod,double wazm,double Fahs,
           double Xmax,double Xmin,unsigned int icon,unsigned int dir,int u_Ht,int u_XY,int u_dP,
           int u_F,int cfd,std::string cfd_name,int cfd_ptype,int cfd_btype,int cfd_capp)
{
  d = new PathImpl(nr,flags,pzn,pzm,pe,pf,pw,pa,ps,pc,pld,X,Y,relHt,mult,wPset,wPmod,wazm,Fahs,
    Xmax,Xmin,icon,dir,u_Ht,u_XY,u_dP,u_F,cfd,cfd_name,cfd_ptype,cfd_btype,cfd_capp);
}

Path::Path(const Path &other) : d(other.d)
{}

Path::~Path()
{}

Path& Path::operator=(const Path &other)
{
  d = other.d;
  return *this;
}

bool Path::operator==(const Path &other) const
{
  return d==other.d;
}

bool Path::operator!=(const Path &other) const
{
  return d!=other.d;
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
  return d->nr();
}

void Path::setNr(const int nr)
{
  d->setNr(nr);
}

int Path::flags() const
{
  return d->flags();
}

void Path::setFlags(const int flags)
{
  d->setFlags(flags);
}

int Path::pzn() const
{
  return d->pzn();
}

void Path::setPzn(const int pzn)
{
  d->setPzn(pzn);
}

int Path::pzm() const
{
  return d->pzm();
}

void Path::setPzm(const int pzm)
{
  d->setPzm(pzm);
}

int Path::pe() const
{
  return d->pe();
}

void Path::setPe(const int pe)
{
  d->setPe(pe);
}

int Path::pf() const
{
  return d->pf();
}

void Path::setPf(const int pf)
{
  d->setPf(pf);
}

int Path::pw() const
{
  return d->pw();
}

void Path::setPw(const int pw)
{
  d->setPw(pw);
}

int Path::pa() const
{
  return d->pa();
}

void Path::setPa(const int pa)
{
  d->setPa(pa);
}

int Path::ps() const
{
  return d->ps();
}

void Path::setPs(const int ps)
{
  d->setPs(ps);
}

int Path::pc() const
{
  return d->pc();
}

void Path::setPc(const int pc)
{
  d->setPc(pc);
}

int Path::pld() const
{
  return d->pld();
}

void Path::setPld(const int pld)
{
  d->setPld(pld);
}

double Path::X() const
{
  return d->X();
}

bool Path::setX(const double X)
{
  return d->setX(X);
}

bool Path::setX(const std::string &X)
{
  return d->setX(X);
}

double Path::Y() const
{
  return d->Y();
}

bool Path::setY(const double Y)
{
  return d->setY(Y);
}

bool Path::setY(const std::string &Y)
{
  return d->setY(Y);
}

double Path::relHt() const
{
  return d->relHt();
}

bool Path::setRelHt(const std::string &relHt)
{
  return d->setRelHt(relHt);
}

bool Path::setRelHt(const double relHt)
{
  return d->setRelHt(relHt);
}

double Path::mult() const
{
  return d->mult();
}

bool Path::setMult(const double mult)
{
  return d->setMult(mult);
}

bool Path::setMult(const std::string &mult)
{
  return d->setMult(mult);
}

double Path::wPset() const
{
  return d->wPset();
}

bool Path::setWPset(const double wPset)
{
  return d->setWPset(wPset);
}

bool Path::setWPset(const std::string &wPset)
{
  return d->setWPset(wPset);
}

double Path::wPmod() const
{
  return d->wPmod();
}

bool Path::setWPmod(const double wPmod)
{
  return d->setWPmod(wPmod);
}

bool Path::setWPmod(const std::string &wPmod)
{
  return d->setWPmod(wPmod);
}

double Path::wazm() const
{
  return d->wazm();
}

bool Path::setWazm(const double wazm)
{
  return d->setWazm(wazm);
}

bool Path::setWazm(const std::string &wazm)
{
  return d->setWazm(wazm);
}

double Path::Fahs() const
{
  return d->Fahs();
}

bool Path::setFahs(const double Fahs)
{
  return d->setFahs(Fahs);
}

bool Path::setFahs(const std::string &Fahs)
{
  return d->setFahs(Fahs);
}

double Path::Xmax() const
{
  return d->Xmax();
}

bool Path::setXmax(const double Xmax)
{
  return d->setXmax(Xmax);
}

bool Path::setXmax(const std::string &Xmax)
{
  return d->setXmax(Xmax);
}

double Path::Xmin() const
{
  return d->Xmin();
}

bool Path::setXmin(const double Xmin)
{
  return d->setXmin(Xmin);
}

bool Path::setXmin(const std::string &Xmin)
{
  return d->setXmin(Xmin);
}

unsigned int Path::icon() const
{
  return d->icon();
}

void Path::setIcon(const unsigned int icon)
{
  d->setIcon(icon);
}

unsigned int Path::dir() const
{
  return d->dir();
}

void Path::setDir(const unsigned int dir)
{
  d->setDir(dir);
}

int Path::u_Ht() const
{
  return d->u_Ht();
}

void Path::setU_Ht(const int u_Ht)
{
  d->setU_Ht(u_Ht);
}

int Path::u_XY() const
{
  return d->u_XY();
}

void Path::setU_XY(const int u_XY)
{
  d->setU_XY(u_XY);
}

int Path::u_dP() const
{
  return d->u_dP();
}

void Path::setU_dP(const int u_dP)
{
  d->setU_dP(u_dP);
}

int Path::u_F() const
{
  return d->u_F();
}

void Path::setU_F(const int u_F)
{
  d->setU_F(u_F);
}

int Path::cfd() const
{
  return d->cfd();
}

void Path::setCfd(const int cfd)
{
  d->setCfd(cfd);
}

std::string Path::cfd_name() const
{
  return d->cfd_name();
}

void Path::setCfd_name(const std::string &cfd_name)
{
  d->setCfd_name(cfd_name);
}

int Path::cfd_ptype() const
{
  return d->cfd_ptype();
}

void Path::setCfd_ptype(const int cfd_ptype)
{
  d->setCfd_ptype(cfd_ptype);
}

int Path::cfd_btype() const
{
  return d->cfd_btype();
}

void Path::setCfd_btype(const int cfd_btype)
{
  d->setCfd_btype(cfd_btype);
}

int Path::cfd_capp() const
{
  return d->cfd_capp();
}

void Path::setCfd_capp(const int cfd_capp)
{
  d->setCfd_capp(cfd_capp);
}

void Path::setWindPressure(bool b)
{
  d->setWindPressure(b);
}

bool Path::windPressure()
{
  return d->windPressure();
}

void Path::setSystem(bool b)
{
  d->setSystem(b);
}

bool Path::system()
{
  return d->system();
}

void Path::setExhaust(bool b)
{
  d->setExhaust(b);
}

bool Path::exhaust()
{
  return d->exhaust();
}

void Path::setRecirculation(bool b)
{
  d->setRecirculation(b);
}

bool Path::recirculation()
{
  return d->recirculation();
}

void Path::setOutsideAir(bool b)
{
  d->setOutsideAir(b);
}

bool Path::outsideAir()
{
  return d->outsideAir();
}

RunControl::RunControl()
{
  d = new RunControlImpl();
}
/*
RunControl::RunControl(int sim_af,int afcalc,int afmaxi,double afrcnvg,double afacnvg,double afrelax,
                       int uac2,double Pres,int uPres,int afslae,int afrseq,int aflmaxi,double aflcnvg,
                       int aflinit,int Tadj,int sim_mf,int ccmaxi,double ccrcnvg,double ccacnvg,
                       double ccrelax,int uccc,int mfnmthd,int mfnrseq,int mfnmaxi,double mfnrcnvg,
                       double mfnacnvg,double mfnrelax,double mfngamma,int uccn,int mftmthd,int mftrseq,
                       int mftmaxi,double mftrcnvg,double mftacnvg,double mftrelax,double mftgamma,int ucct,
                       int mfvmthd,int mfvrseq,int mfvmaxi,double mfvrcnvg,double mfvacnvg,double mfvrelax,
                       int uccv,int mf_solver,int sim_1dz,int sim_1dd,double celldx,int sim_vjt,int udx,
                       int cvode_mth,double cvode_rcnvg,double cvode_acnvg,double cvode_dtmax,int tsdens,
                       double tsrelax,int tsmaxi,int cnvgSS,int densZP,int stackD,int dodMdt,
                       std::string date_st,std::string time_st,std::string date_0,std::string time_0,std::string date_1,
                       std::string time_1,std::string time_step,std::string time_list,std::string time_scrn,int restart,
                       std::string rstdate,std::string rsttime,int list,int doDlg,int pfsave,int zfsave,
                       int zcsave,int achvol,int achsave,int abwsave,int cbwsave,int expsave,
                       int ebwsave,int zaasave,int zbwsave,int rzfsave,int rzmsave,int rz1save,
                       int csmsave,int srfsave,int logsave,std::vector<int> save,std::vector<double> rvals,
                       int BldgFlowZ,int BldgFlowD,int BldgFlowC,int cfd_ctype,double cfd_convcpl,
                       int cfd_var,int cfd_zref,int cfd_imax,int cfd_dtcmo)
{
  d = new RunControlImpl(sim_af,afcalc,afmaxi,afrcnvg,afacnvg,afrelax,uac2,Pres,uPres,afslae,afrseq,
    aflmaxi,aflcnvg,aflinit,Tadj,sim_mf,ccmaxi,ccrcnvg,ccacnvg,ccrelax,uccc,mfnmthd,
    mfnrseq,mfnmaxi,mfnrcnvg,mfnacnvg,mfnrelax,mfngamma,uccn,mftmthd,mftrseq,mftmaxi,
    mftrcnvg,mftacnvg,mftrelax,mftgamma,ucct,mfvmthd,mfvrseq,mfvmaxi,mfvrcnvg,
    mfvacnvg,mfvrelax,uccv,mf_solver,sim_1dz,sim_1dd,celldx,sim_vjt,udx,cvode_mth,
    cvode_rcnvg,cvode_acnvg,cvode_dtmax,tsdens,tsrelax,tsmaxi,cnvgSS,densZP,stackD,
    dodMdt,date_st,time_st,date_0,time_0,date_1,time_1,time_step,time_list,time_scrn,
    restart,rstdate,rsttime,list,doDlg,pfsave,zfsave,zcsave,achvol,achsave,abwsave,
    cbwsave,expsave,ebwsave,zaasave,zbwsave,rzfsave,rzmsave,rz1save,csmsave,srfsave,
    logsave,save,rvals,BldgFlowZ,BldgFlowD,BldgFlowC,cfd_ctype,cfd_convcpl,cfd_var,
    cfd_zref,cfd_imax,cfd_dtcmo);
}

RunControl::RunControl(int sim_af,int afcalc,int afmaxi,std::string afrcnvg,std::string afacnvg,std::string afrelax,
                       int uac2,std::string Pres,int uPres,int afslae,int afrseq,int aflmaxi,std::string aflcnvg,
                       int aflinit,int Tadj,int sim_mf,int ccmaxi,std::string ccrcnvg,std::string ccacnvg,
                       std::string ccrelax,int uccc,int mfnmthd,int mfnrseq,int mfnmaxi,std::string mfnrcnvg,
                       std::string mfnacnvg,std::string mfnrelax,std::string mfngamma,int uccn,int mftmthd,int mftrseq,
                       int mftmaxi,std::string mftrcnvg,std::string mftacnvg,std::string mftrelax,std::string mftgamma,
                       int ucct,int mfvmthd,int mfvrseq,int mfvmaxi,std::string mfvrcnvg,std::string mfvacnvg,
                       std::string mfvrelax,int uccv,int mf_solver,int sim_1dz,int sim_1dd,std::string celldx,
                       int sim_vjt,int udx,int cvode_mth,std::string cvode_rcnvg,std::string cvode_acnvg,
                       std::string cvode_dtmax,int tsdens,std::string tsrelax,int tsmaxi,int cnvgSS,int densZP,
                       int stackD,int dodMdt,std::string date_st,std::string time_st,std::string date_0,std::string time_0,
                       std::string date_1,std::string time_1,std::string time_step,std::string time_list,std::string time_scrn,
                       int restart,std::string rstdate,std::string rsttime,int list,int doDlg,int pfsave,int zfsave,
                       int zcsave,int achvol,int achsave,int abwsave,int cbwsave,int expsave,int ebwsave,
                       int zaasave,int zbwsave,int rzfsave,int rzmsave,int rz1save,int csmsave,int srfsave,
                       int logsave,std::vector<int> save,std::vector<std::string> rvals,int BldgFlowZ,int BldgFlowD,
                       int BldgFlowC,int cfd_ctype,std::string cfd_convcpl,int cfd_var,int cfd_zref,
                       int cfd_imax,int cfd_dtcmo)
{
  d = new RunControlImpl(sim_af,afcalc,afmaxi,afrcnvg,afacnvg,afrelax,uac2,Pres,uPres,afslae,afrseq,
    aflmaxi,aflcnvg,aflinit,Tadj,sim_mf,ccmaxi,ccrcnvg,ccacnvg,ccrelax,uccc,mfnmthd,
    mfnrseq,mfnmaxi,mfnrcnvg,mfnacnvg,mfnrelax,mfngamma,uccn,mftmthd,mftrseq,mftmaxi,
    mftrcnvg,mftacnvg,mftrelax,mftgamma,ucct,mfvmthd,mfvrseq,mfvmaxi,mfvrcnvg,
    mfvacnvg,mfvrelax,uccv,mf_solver,sim_1dz,sim_1dd,celldx,sim_vjt,udx,cvode_mth,
    cvode_rcnvg,cvode_acnvg,cvode_dtmax,tsdens,tsrelax,tsmaxi,cnvgSS,densZP,stackD,
    dodMdt,date_st,time_st,date_0,time_0,date_1,time_1,time_step,time_list,time_scrn,
    restart,rstdate,rsttime,list,doDlg,pfsave,zfsave,zcsave,achvol,achsave,abwsave,
    cbwsave,expsave,ebwsave,zaasave,zbwsave,rzfsave,rzmsave,rz1save,csmsave,srfsave,
    logsave,save,rvals,BldgFlowZ,BldgFlowD,BldgFlowC,cfd_ctype,cfd_convcpl,cfd_var,
    cfd_zref,cfd_imax,cfd_dtcmo);
}*/

RunControl::RunControl(const RunControl &other) : d(other.d)
{}

RunControl::~RunControl()
{}

RunControl& RunControl::operator=(const RunControl &other)
{
  d = other.d;
  return *this;
}

bool RunControl::operator==(const RunControl &other) const
{
  return d==other.d;
}

bool RunControl::operator!=(const RunControl &other) const
{
  return d!=other.d;
}

std::string RunControl::write()
{
  return d->write();
}

void RunControl::read(Reader &input)
{
  d->read(input);
}

int RunControl::sim_af() const
{
  return d->sim_af();
}

void RunControl::setSim_af(const int sim_af)
{
  d->setSim_af(sim_af);
}

int RunControl::afcalc() const
{
  return d->afcalc();
}

void RunControl::setAfcalc(const int afcalc)
{
  d->setAfcalc(afcalc);
}

int RunControl::afmaxi() const
{
  return d->afmaxi();
}

void RunControl::setAfmaxi(const int afmaxi)
{
  d->setAfmaxi(afmaxi);
}

double RunControl::afrcnvg() const
{
  return d->afrcnvg();
}

bool RunControl::setAfrcnvg(const double afrcnvg)
{
  return d->setAfrcnvg(afrcnvg);
}

bool RunControl::setAfrcnvg(const std::string &afrcnvg)
{
  return d->setAfrcnvg(afrcnvg);
}

double RunControl::afacnvg() const
{
  return d->afacnvg();
}

bool RunControl::setAfacnvg(const double afacnvg)
{
  return d->setAfacnvg(afacnvg);
}

bool RunControl::setAfacnvg(const std::string &afacnvg)
{
  return d->setAfacnvg(afacnvg);
}

double RunControl::afrelax() const
{
  return d->afrelax();
}

bool RunControl::setAfrelax(const double afrelax)
{
  return d->setAfrelax(afrelax);
}

bool RunControl::setAfrelax(const std::string &afrelax)
{
  return d->setAfrelax(afrelax);
}

int RunControl::uac2() const
{
  return d->uac2();
}

void RunControl::setUac2(const int uac2)
{
  d->setUac2(uac2);
}

double RunControl::Pres() const
{
  return d->Pres();
}

bool RunControl::setPres(const double Pres)
{
  return d->setPres(Pres);
}

bool RunControl::setPres(const std::string &Pres)
{
  return d->setPres(Pres);
}

int RunControl::uPres() const
{
  return d->uPres();
}

void RunControl::setUPres(const int uPres)
{
  d->setUPres(uPres);
}

int RunControl::afslae() const
{
  return d->afslae();
}

void RunControl::setAfslae(const int afslae)
{
  d->setAfslae(afslae);
}

int RunControl::afrseq() const
{
  return d->afrseq();
}

void RunControl::setAfrseq(const int afrseq)
{
  d->setAfrseq(afrseq);
}

int RunControl::aflmaxi() const
{
  return d->aflmaxi();
}

void RunControl::setAflmaxi(const int aflmaxi)
{
  d->setAflmaxi(aflmaxi);
}

double RunControl::aflcnvg() const
{
  return d->aflcnvg();
}

bool RunControl::setAflcnvg(const double aflcnvg)
{
  return d->setAflcnvg(aflcnvg);
}

bool RunControl::setAflcnvg(const std::string &aflcnvg)
{
  return d->setAflcnvg(aflcnvg);
}

int RunControl::aflinit() const
{
  return d->aflinit();
}

void RunControl::setAflinit(const int aflinit)
{
  d->setAflinit(aflinit);
}

int RunControl::Tadj() const
{
  return d->Tadj();
}

void RunControl::setTadj(const int Tadj)
{
  d->setTadj(Tadj);
}

int RunControl::sim_mf() const
{
  return d->sim_mf();
}

void RunControl::setSim_mf(const int sim_mf)
{
  d->setSim_mf(sim_mf);
}

int RunControl::ccmaxi() const
{
  return d->ccmaxi();
}

void RunControl::setCcmaxi(const int ccmaxi)
{
  d->setCcmaxi(ccmaxi);
}

double RunControl::ccrcnvg() const
{
  return d->ccrcnvg();
}

bool RunControl::setCcrcnvg(const double ccrcnvg)
{
  return d->setCcrcnvg(ccrcnvg);
}

bool RunControl::setCcrcnvg(const std::string &ccrcnvg)
{
  return d->setCcrcnvg(ccrcnvg);
}

double RunControl::ccacnvg() const
{
  return d->ccacnvg();
}

bool RunControl::setCcacnvg(const double ccacnvg)
{
  return d->setCcacnvg(ccacnvg);
}

bool RunControl::setCcacnvg(const std::string &ccacnvg)
{
  return d->setCcacnvg(ccacnvg);
}

double RunControl::ccrelax() const
{
  return d->ccrelax();
}

bool RunControl::setCcrelax(const double ccrelax)
{
  return d->setCcrelax(ccrelax);
}

bool RunControl::setCcrelax(const std::string &ccrelax)
{
  return d->setCcrelax(ccrelax);
}

int RunControl::uccc() const
{
  return d->uccc();
}

void RunControl::setUccc(const int uccc)
{
  d->setUccc(uccc);
}

int RunControl::mfnmthd() const
{
  return d->mfnmthd();
}

void RunControl::setMfnmthd(const int mfnmthd)
{
  d->setMfnmthd(mfnmthd);
}

int RunControl::mfnrseq() const
{
  return d->mfnrseq();
}

void RunControl::setMfnrseq(const int mfnrseq)
{
  d->setMfnrseq(mfnrseq);
}

int RunControl::mfnmaxi() const
{
  return d->mfnmaxi();
}

void RunControl::setMfnmaxi(const int mfnmaxi)
{
  d->setMfnmaxi(mfnmaxi);
}

double RunControl::mfnrcnvg() const
{
  return d->mfnrcnvg();
}

bool RunControl::setMfnrcnvg(const double mfnrcnvg)
{
  return d->setMfnrcnvg(mfnrcnvg);
}

bool RunControl::setMfnrcnvg(const std::string &mfnrcnvg)
{
  return d->setMfnrcnvg(mfnrcnvg);
}

double RunControl::mfnacnvg() const
{
  return d->mfnacnvg();
}

bool RunControl::setMfnacnvg(const double mfnacnvg)
{
  return d->setMfnacnvg(mfnacnvg);
}

bool RunControl::setMfnacnvg(const std::string &mfnacnvg)
{
  return d->setMfnacnvg(mfnacnvg);
}

double RunControl::mfnrelax() const
{
  return d->mfnrelax();
}

bool RunControl::setMfnrelax(const double mfnrelax)
{
  return d->setMfnrelax(mfnrelax);
}

bool RunControl::setMfnrelax(const std::string &mfnrelax)
{
  return d->setMfnrelax(mfnrelax);
}

double RunControl::mfngamma() const
{
  return d->mfngamma();
}

bool RunControl::setMfngamma(const double mfngamma)
{
  return d->setMfngamma(mfngamma);
}

bool RunControl::setMfngamma(const std::string &mfngamma)
{
  return d->setMfngamma(mfngamma);
}

int RunControl::uccn() const
{
  return d->uccn();
}

void RunControl::setUccn(const int uccn)
{
  d->setUccn(uccn);
}

int RunControl::mftmthd() const
{
  return d->mftmthd();
}

void RunControl::setMftmthd(const int mftmthd)
{
  d->setMftmthd(mftmthd);
}

int RunControl::mftrseq() const
{
  return d->mftrseq();
}

void RunControl::setMftrseq(const int mftrseq)
{
  d->setMftrseq(mftrseq);
}

int RunControl::mftmaxi() const
{
  return d->mftmaxi();
}

void RunControl::setMftmaxi(const int mftmaxi)
{
  d->setMftmaxi(mftmaxi);
}

double RunControl::mftrcnvg() const
{
  return d->mftrcnvg();
}

bool RunControl::setMftrcnvg(const double mftrcnvg)
{
  return d->setMftrcnvg(mftrcnvg);
}

bool RunControl::setMftrcnvg(const std::string &mftrcnvg)
{
  return d->setMftrcnvg(mftrcnvg);
}

double RunControl::mftacnvg() const
{
  return d->mftacnvg();
}

bool RunControl::setMftacnvg(const double mftacnvg)
{
  return d->setMftacnvg(mftacnvg);
}

bool RunControl::setMftacnvg(const std::string &mftacnvg)
{
  return d->setMftacnvg(mftacnvg);
}

double RunControl::mftrelax() const
{
  return d->mftrelax();
}

bool RunControl::setMftrelax(const double mftrelax)
{
  return d->setMftrelax(mftrelax);
}

bool RunControl::setMftrelax(const std::string &mftrelax)
{
  return d->setMftrelax(mftrelax);
}

double RunControl::mftgamma() const
{
  return d->mftgamma();
}

bool RunControl::setMftgamma(const double mftgamma)
{
  return d->setMftgamma(mftgamma);
}

bool RunControl::setMftgamma(const std::string &mftgamma)
{
  return d->setMftgamma(mftgamma);
}

int RunControl::ucct() const
{
  return d->ucct();
}

void RunControl::setUcct(const int ucct)
{
  d->setUcct(ucct);
}

int RunControl::mfvmthd() const
{
  return d->mfvmthd();
}

void RunControl::setMfvmthd(const int mfvmthd)
{
  d->setMfvmthd(mfvmthd);
}

int RunControl::mfvrseq() const
{
  return d->mfvrseq();
}

void RunControl::setMfvrseq(const int mfvrseq)
{
  d->setMfvrseq(mfvrseq);
}

int RunControl::mfvmaxi() const
{
  return d->mfvmaxi();
}

void RunControl::setMfvmaxi(const int mfvmaxi)
{
  d->setMfvmaxi(mfvmaxi);
}

double RunControl::mfvrcnvg() const
{
  return d->mfvrcnvg();
}

bool RunControl::setMfvrcnvg(const double mfvrcnvg)
{
  return d->setMfvrcnvg(mfvrcnvg);
}

bool RunControl::setMfvrcnvg(const std::string &mfvrcnvg)
{
  return d->setMfvrcnvg(mfvrcnvg);
}

double RunControl::mfvacnvg() const
{
  return d->mfvacnvg();
}

bool RunControl::setMfvacnvg(const double mfvacnvg)
{
  return d->setMfvacnvg(mfvacnvg);
}

bool RunControl::setMfvacnvg(const std::string &mfvacnvg)
{
  return d->setMfvacnvg(mfvacnvg);
}

double RunControl::mfvrelax() const
{
  return d->mfvrelax();
}

bool RunControl::setMfvrelax(const double mfvrelax)
{
  return d->setMfvrelax(mfvrelax);
}

bool RunControl::setMfvrelax(const std::string &mfvrelax)
{
  return d->setMfvrelax(mfvrelax);
}

int RunControl::uccv() const
{
  return d->uccv();
}

void RunControl::setUccv(const int uccv)
{
  d->setUccv(uccv);
}

int RunControl::mf_solver() const
{
  return d->mf_solver();
}

void RunControl::setMf_solver(const int mf_solver)
{
  d->setMf_solver(mf_solver);
}

int RunControl::sim_1dz() const
{
  return d->sim_1dz();
}

void RunControl::setSim_1dz(const int sim_1dz)
{
  d->setSim_1dz(sim_1dz);
}

int RunControl::sim_1dd() const
{
  return d->sim_1dd();
}

void RunControl::setSim_1dd(const int sim_1dd)
{
  d->setSim_1dd(sim_1dd);
}

double RunControl::celldx() const
{
  return d->celldx();
}

bool RunControl::setCelldx(const double celldx)
{
  return d->setCelldx(celldx);
}

bool RunControl::setCelldx(const std::string &celldx)
{
  return d->setCelldx(celldx);
}

int RunControl::sim_vjt() const
{
  return d->sim_vjt();
}

void RunControl::setSim_vjt(const int sim_vjt)
{
  d->setSim_vjt(sim_vjt);
}

int RunControl::udx() const
{
  return d->udx();
}

void RunControl::setUdx(const int udx)
{
  d->setUdx(udx);
}

int RunControl::cvode_mth() const
{
  return d->cvode_mth();
}

void RunControl::setCvode_mth(const int cvode_mth)
{
  d->setCvode_mth(cvode_mth);
}

double RunControl::cvode_rcnvg() const
{
  return d->cvode_rcnvg();
}

bool RunControl::setCvode_rcnvg(const double cvode_rcnvg)
{
  return d->setCvode_rcnvg(cvode_rcnvg);
}

bool RunControl::setCvode_rcnvg(const std::string &cvode_rcnvg)
{
  return d->setCvode_rcnvg(cvode_rcnvg);
}

double RunControl::cvode_acnvg() const
{
  return d->cvode_acnvg();
}

bool RunControl::setCvode_acnvg(const double cvode_acnvg)
{
  return d->setCvode_acnvg(cvode_acnvg);
}

bool RunControl::setCvode_acnvg(const std::string &cvode_acnvg)
{
  return d->setCvode_acnvg(cvode_acnvg);
}

double RunControl::cvode_dtmax() const
{
  return d->cvode_dtmax();
}

bool RunControl::setCvode_dtmax(const double cvode_dtmax)
{
  return d->setCvode_dtmax(cvode_dtmax);
}

bool RunControl::setCvode_dtmax(const std::string &cvode_dtmax)
{
  return d->setCvode_dtmax(cvode_dtmax);
}

int RunControl::tsdens() const
{
  return d->tsdens();
}

void RunControl::setTsdens(const int tsdens)
{
  d->setTsdens(tsdens);
}

double RunControl::tsrelax() const
{
  return d->tsrelax();
}

bool RunControl::setTsrelax(const double tsrelax)
{
  return d->setTsrelax(tsrelax);
}

bool RunControl::setTsrelax(const std::string &tsrelax)
{
  return d->setTsrelax(tsrelax);
}

int RunControl::tsmaxi() const
{
  return d->tsmaxi();
}

void RunControl::setTsmaxi(const int tsmaxi)
{
  d->setTsmaxi(tsmaxi);
}

int RunControl::cnvgSS() const
{
  return d->cnvgSS();
}

void RunControl::setCnvgSS(const int cnvgSS)
{
  d->setCnvgSS(cnvgSS);
}

int RunControl::densZP() const
{
  return d->densZP();
}

void RunControl::setDensZP(const int densZP)
{
  d->setDensZP(densZP);
}

int RunControl::stackD() const
{
  return d->stackD();
}

void RunControl::setStackD(const int stackD)
{
  d->setStackD(stackD);
}

int RunControl::dodMdt() const
{
  return d->dodMdt();
}

void RunControl::setDodMdt(const int dodMdt)
{
  d->setDodMdt(dodMdt);
}

std::string RunControl::date_st() const
{
  return d->date_st();
}

void RunControl::setDate_st(const std::string &date_st)
{
  d->setDate_st(date_st);
}

std::string RunControl::time_st() const
{
  return d->time_st();
}

void RunControl::setTime_st(const std::string &time_st)
{
  d->setTime_st(time_st);
}

std::string RunControl::date_0() const
{
  return d->date_0();
}

void RunControl::setDate_0(const std::string &date_0)
{
  d->setDate_0(date_0);
}

std::string RunControl::time_0() const
{
  return d->time_0();
}

void RunControl::setTime_0(const std::string &time_0)
{
  d->setTime_0(time_0);
}

std::string RunControl::date_1() const
{
  return d->date_1();
}

void RunControl::setDate_1(const std::string &date_1)
{
  d->setDate_1(date_1);
}

std::string RunControl::time_1() const
{
  return d->time_1();
}

void RunControl::setTime_1(const std::string &time_1)
{
  d->setTime_1(time_1);
}

std::string RunControl::time_step() const
{
  return d->time_step();
}

void RunControl::setTime_step(const std::string &time_step)
{
  d->setTime_step(time_step);
}

std::string RunControl::time_list() const
{
  return d->time_list();
}

void RunControl::setTime_list(const std::string &time_list)
{
  d->setTime_list(time_list);
}

std::string RunControl::time_scrn() const
{
  return d->time_scrn();
}

void RunControl::setTime_scrn(const std::string &time_scrn)
{
  d->setTime_scrn(time_scrn);
}

int RunControl::restart() const
{
  return d->restart();
}

void RunControl::setRestart(const int restart)
{
  d->setRestart(restart);
}

std::string RunControl::rstdate() const
{
  return d->rstdate();
}

void RunControl::setRstdate(const std::string &rstdate)
{
  d->setRstdate(rstdate);
}

std::string RunControl::rsttime() const
{
  return d->rsttime();
}

void RunControl::setRsttime(const std::string &rsttime)
{
  d->setRsttime(rsttime);
}

int RunControl::list() const
{
  return d->list();
}

void RunControl::setList(const int list)
{
  d->setList(list);
}

int RunControl::doDlg() const
{
  return d->doDlg();
}

void RunControl::setDoDlg(const int doDlg)
{
  d->setDoDlg(doDlg);
}

int RunControl::pfsave() const
{
  return d->pfsave();
}

void RunControl::setPfsave(const int pfsave)
{
  d->setPfsave(pfsave);
}

int RunControl::zfsave() const
{
  return d->zfsave();
}

void RunControl::setZfsave(const int zfsave)
{
  d->setZfsave(zfsave);
}

int RunControl::zcsave() const
{
  return d->zcsave();
}

void RunControl::setZcsave(const int zcsave)
{
  d->setZcsave(zcsave);
}

int RunControl::achvol() const
{
  return d->achvol();
}

void RunControl::setAchvol(const int achvol)
{
  d->setAchvol(achvol);
}

int RunControl::achsave() const
{
  return d->achsave();
}

void RunControl::setAchsave(const int achsave)
{
  d->setAchsave(achsave);
}

int RunControl::abwsave() const
{
  return d->abwsave();
}

void RunControl::setAbwsave(const int abwsave)
{
  d->setAbwsave(abwsave);
}

int RunControl::cbwsave() const
{
  return d->cbwsave();
}

void RunControl::setCbwsave(const int cbwsave)
{
  d->setCbwsave(cbwsave);
}

int RunControl::expsave() const
{
  return d->expsave();
}

void RunControl::setExpsave(const int expsave)
{
  d->setExpsave(expsave);
}

int RunControl::ebwsave() const
{
  return d->ebwsave();
}

void RunControl::setEbwsave(const int ebwsave)
{
  d->setEbwsave(ebwsave);
}

int RunControl::zaasave() const
{
  return d->zaasave();
}

void RunControl::setZaasave(const int zaasave)
{
  d->setZaasave(zaasave);
}

int RunControl::zbwsave() const
{
  return d->zbwsave();
}

void RunControl::setZbwsave(const int zbwsave)
{
  d->setZbwsave(zbwsave);
}

int RunControl::rzfsave() const
{
  return d->rzfsave();
}

void RunControl::setRzfsave(const int rzfsave)
{
  d->setRzfsave(rzfsave);
}

int RunControl::rzmsave() const
{
  return d->rzmsave();
}

void RunControl::setRzmsave(const int rzmsave)
{
  d->setRzmsave(rzmsave);
}

int RunControl::rz1save() const
{
  return d->rz1save();
}

void RunControl::setRz1save(const int rz1save)
{
  d->setRz1save(rz1save);
}

int RunControl::csmsave() const
{
  return d->csmsave();
}

void RunControl::setCsmsave(const int csmsave)
{
  d->setCsmsave(csmsave);
}

int RunControl::srfsave() const
{
  return d->srfsave();
}

void RunControl::setSrfsave(const int srfsave)
{
  d->setSrfsave(srfsave);
}

int RunControl::logsave() const
{
  return d->logsave();
}

void RunControl::setLogsave(const int logsave)
{
  d->setLogsave(logsave);
}

std::vector<int> RunControl::save() const
{
  return d->save();
}

void RunControl::setSave(const std::vector<int> &save)
{
  d->setSave(save);
}

std::vector<double> RunControl::rvals() const
{
  return d->rvals();
}

bool RunControl::setRvals(const std::vector<double> &rvals)
{
  return d->setRvals(rvals);
}

bool RunControl::setRvals(const std::vector<std::string> &rvals)
{
  return d->setRvals(rvals);
}

int RunControl::BldgFlowZ() const
{
  return d->BldgFlowZ();
}

void RunControl::setBldgFlowZ(const int BldgFlowZ)
{
  d->setBldgFlowZ(BldgFlowZ);
}

int RunControl::BldgFlowD() const
{
  return d->BldgFlowD();
}

void RunControl::setBldgFlowD(const int BldgFlowD)
{
  d->setBldgFlowD(BldgFlowD);
}

int RunControl::BldgFlowC() const
{
  return d->BldgFlowC();
}

void RunControl::setBldgFlowC(const int BldgFlowC)
{
  d->setBldgFlowC(BldgFlowC);
}

int RunControl::cfd_ctype() const
{
  return d->cfd_ctype();
}

void RunControl::setCfd_ctype(const int cfd_ctype)
{
  d->setCfd_ctype(cfd_ctype);
}

double RunControl::cfd_convcpl() const
{
  return d->cfd_convcpl();
}

bool RunControl::setCfd_convcpl(const double cfd_convcpl)
{
  return d->setCfd_convcpl(cfd_convcpl);
}

bool RunControl::setCfd_convcpl(const std::string &cfd_convcpl)
{
  return d->setCfd_convcpl(cfd_convcpl);
}

int RunControl::cfd_var() const
{
  return d->cfd_var();
}

void RunControl::setCfd_var(const int cfd_var)
{
  d->setCfd_var(cfd_var);
}

int RunControl::cfd_zref() const
{
  return d->cfd_zref();
}

void RunControl::setCfd_zref(const int cfd_zref)
{
  d->setCfd_zref(cfd_zref);
}

int RunControl::cfd_imax() const
{
  return d->cfd_imax();
}

void RunControl::setCfd_imax(const int cfd_imax)
{
  d->setCfd_imax(cfd_imax);
}

int RunControl::cfd_dtcmo() const
{
  return d->cfd_dtcmo();
}

void RunControl::setCfd_dtcmo(const int cfd_dtcmo)
{
  d->setCfd_dtcmo(cfd_dtcmo);
}

Level::Level()
{
  d = new LevelImpl;
}

Level::Level(int nr,double refht,double delht,int u_rfht,int u_dlht,std::string name,std::vector<Icon> icons)
{
  d = new LevelImpl(nr,refht,delht,u_rfht,u_dlht,name,icons);
}

Level::Level(int nr,std::string refht,std::string delht,int u_rfht,int u_dlht,std::string name,std::vector<Icon> icons)
{
  d = new LevelImpl(nr,refht,delht,u_rfht,u_dlht,name,icons);
}

Level::Level(const Level &other) : d(other.d)
{}

Level::~Level()
{}

Level& Level::operator=(const Level &other)
{
  d = other.d;
  return *this;
}

bool Level::operator==(const Level &other) const
{
  return d==other.d;
}

bool Level::operator!=(const Level &other) const
{
  return d!=other.d;
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
  return d->nr();
}

void Level::setNr(const int nr)
{
  d->setNr(nr);
}

double Level::refht() const
{
  return d->refht();
}

bool Level::setRefht(const double refht)
{
  return d->setRefht(refht);
}

bool Level::setRefht(const std::string &refht)
{
  return d->setRefht(refht);
}

double Level::delht() const
{
  return d->delht();
}

bool Level::setDelht(const double delht)
{
  return d->setDelht(delht);
}

bool Level::setDelht(const std::string &delht)
{
  return d->setDelht(delht);
}

int Level::u_rfht() const
{
  return d->u_rfht();
}

void Level::setU_rfht(const int u_rfht)
{
  d->setU_rfht(u_rfht);
}

int Level::u_dlht() const
{
  return d->u_dlht();
}

void Level::setU_dlht(const int u_dlht)
{
  d->setU_dlht(u_dlht);
}

std::string Level::name() const
{
  return d->name();
}

void Level::setName(const std::string &name)
{
  d->setName(name);
}

std::vector<Icon> Level::icons() const
{
  return d->icons();
}

void Level::setIcons(const std::vector<Icon> &icons)
{
  d->setIcons(icons);
}

DaySchedule::DaySchedule()
{
  d = new DayScheduleImpl();
}

DaySchedule::DaySchedule(int nr,int shape,int utyp,int ucnv,std::string name,std::string desc,
                         std::vector<SchedulePoint> points)
{
  d = new DayScheduleImpl(nr,shape,utyp,ucnv,name,desc,points);
}

DaySchedule::DaySchedule(const DaySchedule &other) : d(other.d)
{}

DaySchedule::~DaySchedule()
{}

DaySchedule& DaySchedule::operator=(const DaySchedule &other)
{
  d = other.d;
  return *this;
}

bool DaySchedule::operator==(const DaySchedule &other) const
{
  return d==other.d;
}

bool DaySchedule::operator!=(const DaySchedule &other) const
{
  return d!=other.d;
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
  return d->nr();
}

void DaySchedule::setNr(const int nr)
{
  d->setNr(nr);
}

int DaySchedule::shape() const
{
  return d->shape();
}

void DaySchedule::setShape(const int shape)
{
  d->setShape(shape);
}

int DaySchedule::utyp() const
{
  return d->utyp();
}

void DaySchedule::setUtyp(const int utyp)
{
  d->setUtyp(utyp);
}

int DaySchedule::ucnv() const
{
  return d->ucnv();
}

void DaySchedule::setUcnv(const int ucnv)
{
  d->setUcnv(ucnv);
}

std::string DaySchedule::name() const
{
  return d->name();
}

void DaySchedule::setName(const std::string &name)
{
  d->setName(name);
}

std::string DaySchedule::desc() const
{
  return d->desc();
}

void DaySchedule::setDesc(const std::string &desc)
{
  d->setDesc(desc);
}

std::vector<SchedulePoint> DaySchedule::points() const
{
  return d->points();
}

void DaySchedule::setPoints(const std::vector<SchedulePoint> &points)
{
  d->setPoints(points);
}

WeekSchedule::WeekSchedule()
{
  d = new WeekScheduleImpl();
}

WeekSchedule::WeekSchedule(int nr,int utyp,int ucnv,std::string name,std::string desc,std::vector<int> j)
{
  d = new WeekScheduleImpl(nr,utyp,ucnv,name,desc,j);
}

WeekSchedule::WeekSchedule(const WeekSchedule &other) : d(other.d)
{}

WeekSchedule::~WeekSchedule()
{}

WeekSchedule& WeekSchedule::operator=(const WeekSchedule &other)
{
  d = other.d;
  return *this;
}

bool WeekSchedule::operator==(const WeekSchedule &other) const
{
  return d==other.d;
}

bool WeekSchedule::operator!=(const WeekSchedule &other) const
{
  return d!=other.d;
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
  return d->nr();
}

void WeekSchedule::setNr(const int nr)
{
  d->setNr(nr);
}

int WeekSchedule::utyp() const
{
  return d->utyp();
}

void WeekSchedule::setUtyp(const int utyp)
{
  d->setUtyp(utyp);
}

int WeekSchedule::ucnv() const
{
  return d->ucnv();
}

void WeekSchedule::setUcnv(const int ucnv)
{
  d->setUcnv(ucnv);
}

std::string WeekSchedule::name() const
{
  return d->name();
}

void WeekSchedule::setName(const std::string &name)
{
  d->setName(name);
}

std::string WeekSchedule::desc() const
{
  return d->desc();
}

void WeekSchedule::setDesc(const std::string &desc)
{
  d->setDesc(desc);
}

std::vector<int> WeekSchedule::j() const
{
  return d->j();
}

void WeekSchedule::setJ(const std::vector<int> &j)
{
  d->setJ(j);
}

WindPressureProfile::WindPressureProfile()
{
  d = new WindPressureProfileImpl();
}

WindPressureProfile::WindPressureProfile(int nr,int type,std::string name,std::string desc,
                                         std::vector<PressureCoefficientPoint> coeffs)
{
  d = new WindPressureProfileImpl(nr,type,name,desc,coeffs);
}

WindPressureProfile::WindPressureProfile(const WindPressureProfile &other) : d(other.d)
{}

WindPressureProfile::~WindPressureProfile()
{}

WindPressureProfile& WindPressureProfile::operator=(const WindPressureProfile &other)
{
  d = other.d;
  return *this;
}

bool WindPressureProfile::operator==(const WindPressureProfile &other) const
{
  return d==other.d;
}

bool WindPressureProfile::operator!=(const WindPressureProfile &other) const
{
  return d!=other.d;
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
  return d->nr();
}

void WindPressureProfile::setNr(const int nr)
{
  d->setNr(nr);
}

int WindPressureProfile::type() const
{
  return d->type();
}

void WindPressureProfile::setType(const int type)
{
  d->setType(type);
}

std::string WindPressureProfile::name() const
{
  return d->name();
}

void WindPressureProfile::setName(const std::string &name)
{
  d->setName(name);
}

std::string WindPressureProfile::desc() const
{
  return d->desc();
}

void WindPressureProfile::setDesc(const std::string &desc)
{
  d->setDesc(desc);
}

std::vector<PressureCoefficientPoint> WindPressureProfile::coeffs() const
{
  return d->coeffs();
}

void WindPressureProfile::setCoeffs(const std::vector<PressureCoefficientPoint> &coeffs)
{
  d->setCoeffs(coeffs);
}

ControlNode::Type ControlNode::convertTag(std::string string)
{
  std::string tags[37] = {std::string("sns"),std::string("sch"),std::string("set"),
    std::string("cvf"),std::string("dvf"),std::string("log"),std::string("pas"),
    std::string("mod"),std::string("hys"),std::string("abs"),std::string("bin"),
    std::string("dls"),std::string("dlx"),std::string("int"),std::string("rav"),
    std::string("inv"),std::string("and"),std::string("od"),std::string("xor"),
    std::string("add"),std::string("sub"),std::string("mul"),std::string("div"),
    std::string("sum"),std::string("avg"),std::string("max"),std::string("min"),
    std::string("lls"),std::string("uls"),std::string("lbs"),std::string("ubs"),
    std::string("llc"),std::string("ulc"),std::string("pc1"),std::string("pi1"),
    std::string("sup"),std::string("sph")};
  ControlNode::Type type[37] = {CT_SNS,CT_SCH,CT_SET,CT_CVF,CT_DVF,CT_LOG,CT_PAS,CT_MOD,
    CT_HYS,CT_ABS,CT_BIN,CT_DLS,CT_DLX,CT_INT,CT_RAV,CT_INV,CT_AND,CT_OR,CT_XOR,CT_ADD,
    CT_SUB,CT_MUL,CT_DIV,CT_SUM,CT_AVG,CT_MAX,CT_MIN,CT_LLS,CT_ULS,CT_LBS,CT_UBS,CT_LLC,
    CT_ULC,CT_PC1,CT_PI1,CT_SUP,CT_SPH};
  for(int i=0;i<37;i++)
    if(string==tags[i])
      return type[i];
  return ControlNode::UNKNOWN;
}

ControlNode* ControlNode::readElement(Reader &input)
{
  ControlNode *out=NULL;
  int nr = input.read<int>();
  std::string dataType = input.readString();
  int seqnr = input.read<int>();
  unsigned int flags = input.read<unsigned int>();
  int inreq = input.read<int>();
  int n1 = input.read<int>();
  int n2 = input.read<int>();
  std::string name = input.readString();
  std::string desc = input.readLine();
  int kind = convertTag(dataType);
  switch(kind)
  {
  case ControlNode::CT_CVF:
    {
      CvfDat *obj = new CvfDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = static_cast<ControlNode*>(obj);
      break;
    }
  case ControlNode::CT_DVF:
    {
      DvfDat *obj = new DvfDat(nr,seqnr,flags,inreq,n1,n2,name,desc);
      obj->readDetails(input);
      out = static_cast<ControlNode*>(obj);
      break;
    }
  case ControlNode::CT_PAS:
  case ControlNode::CT_ABS:
  case ControlNode::CT_BIN:
  case ControlNode::CT_INT:
  case ControlNode::CT_INV:
  case ControlNode::CT_AND:
  case ControlNode::CT_OR:
  case ControlNode::CT_XOR:
  case ControlNode::CT_ADD:
  case ControlNode::CT_SUB:
  case ControlNode::CT_MUL:
  case ControlNode::CT_DIV:
  case ControlNode::CT_LLS:
  case ControlNode::CT_ULS:
  case ControlNode::CT_LLC:
  case ControlNode::CT_ULC:
  case ControlNode::CT_SPH:
  case ControlNode::CT_SNS:
  case ControlNode::CT_SCH:
  case ControlNode::CT_SET:
  case ControlNode::CT_LOG:
  case ControlNode::CT_MOD:
  case ControlNode::CT_HYS:
  case ControlNode::CT_DLS:
  case ControlNode::CT_DLX:
  case ControlNode::CT_RAV:
  case ControlNode::CT_SUM:
  case ControlNode::CT_AVG:
  case ControlNode::CT_MAX:
  case ControlNode::CT_MIN:
  case ControlNode::CT_LBS:
  case ControlNode::CT_UBS:
  case ControlNode::CT_PC1:
  case ControlNode::CT_PI1:
  case ControlNode::CT_SUP:
    {
      std::string mesg = "Control node type '" + dataType + "' is not supported.";
      LOG_FREE_AND_THROW("openstudio.contam.Reader",mesg);
    }
  case ControlNode::UNKNOWN:
  default:
    std::string mesg = "Unknown control node type '" + dataType + "' at line " + openstudio::toString(input.lineNumber());
    LOG_FREE_AND_THROW("openstudio.contam.Reader",mesg);
  }
  return out;
}

CdvDat::CdvDat()
{
  d = new CdvDatImpl();
}

CdvDat::CdvDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc)
{
  d = new CdvDatImpl(nr,seqnr,flags,inreq,n1,n2,name,desc);
}

CdvDat::CdvDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,
               std::string valuename)
{
  d = new CdvDatImpl(nr,seqnr,flags,inreq,n1,n2,name,desc,valuename);
}

CdvDat::CdvDat(const CdvDat &other) : d(other.d)
{}

CdvDat::~CdvDat()
{}

CdvDat& CdvDat::operator=(const CdvDat &other)
{
  d = other.d;
  return *this;
}

bool CdvDat::operator==(const CdvDat &other) const
{
  return d==other.d;
}

bool CdvDat::operator!=(const CdvDat &other) const
{
  return d!=other.d;
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
  return d->nr();
}

void CdvDat::setNr(const int nr)
{
  d->setNr(nr);
}

int CdvDat::seqnr() const
{
  return d->seqnr();
}

void CdvDat::setSeqnr(const int seqnr)
{
  d->setSeqnr(seqnr);
}

unsigned int CdvDat::flags() const
{
  return d->flags();
}

void CdvDat::setFlags(const unsigned int flags)
{
  d->setFlags(flags);
}

int CdvDat::inreq() const
{
  return d->inreq();
}

void CdvDat::setInreq(const int inreq)
{
  d->setInreq(inreq);
}

int CdvDat::n1() const
{
  return d->n1();
}

void CdvDat::setN1(const int n1)
{
  d->setN1(n1);
}

int CdvDat::n2() const
{
  return d->n2();
}

void CdvDat::setN2(const int n2)
{
  d->setN2(n2);
}

std::string CdvDat::name() const
{
  return d->name();
}

void CdvDat::setName(const std::string &name)
{
  d->setName(name);
}

std::string CdvDat::desc() const
{
  return d->desc();
}

void CdvDat::setDesc(const std::string &desc)
{
  d->setDesc(desc);
}

std::string CdvDat::valuename() const
{
  return d->valuename();
}

void CdvDat::setValuename(const std::string &valuename)
{
  d->setValuename(valuename);
}

CvfDat::CvfDat() : CdvDat()
{}

CvfDat::CvfDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc)
: CdvDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
{}

CvfDat::CvfDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,
               std::string valuename) : CdvDat(nr,seqnr,flags,inreq,n1,n2,name,desc,valuename)
{}

DvfDat::DvfDat() : CdvDat()
{}

DvfDat::DvfDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc)
: CdvDat(nr,seqnr,flags,inreq,n1,n2,name,desc)
{}

DvfDat::DvfDat(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,
               std::string valuename) : CdvDat(nr,seqnr,flags,inreq,n1,n2,name,desc,valuename)
{}

} // contam
} // openstudio
