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
#include "PrjObjectsImpl.hpp"

namespace openstudio {
namespace contam {
namespace detail {

void ZoneImpl::setDefaults()
{
  m_nr = 0;
  m_flags = 0;
  m_ps = 0;
  m_pc = 0;
  m_pk = 0;
  m_pl = 0;
  m_relHt = PRJFLOAT("0.0");
  m_Vol = PRJFLOAT("0.0");
  m_T0 = PRJFLOAT("0.0");
  m_P0 = PRJFLOAT("0.0");
  m_color = 0;
  m_u_Ht = 0;
  m_u_V = 0;
  m_u_T = 0;
  m_u_P = 0;
  m_cdaxis = 0;
  m_cfd = 0;
  m_X1 = PRJFLOAT("0.0");
  m_Y1 = PRJFLOAT("0.0");
  m_H1 = PRJFLOAT("0.0");
  m_X2 = PRJFLOAT("0.0");
  m_Y2 = PRJFLOAT("0.0");
  m_H2 = PRJFLOAT("0.0");
  m_celldx = PRJFLOAT("0.0");
  m_axialD = PRJFLOAT("0.0");
  m_u_aD = 0;
  m_u_L = 0;
}

ZoneImpl::ZoneImpl()
{
  setDefaults();
}

ZoneImpl::ZoneImpl(int nr,unsigned int flags,int ps,int pc,int pk,int pl,std::string relHt,std::string Vol,std::string T0,
  std::string P0,std::string name,int color,int u_Ht,int u_V,int u_T,int u_P,int cdaxis,int cfd,
  std::string cfdname,std::string X1,std::string Y1,std::string H1,std::string X2,std::string Y2,std::string H2,std::string celldx,
  std::string axialD,int u_aD,int u_L)
{
  setDefaults();
  setNr(nr);
  setFlags(flags);
  setPs(ps);
  setPc(pc);
  setPk(pk);
  setPl(pl);
  setRelHt(relHt);
  setVol(Vol);
  setT0(T0);
  setP0(P0);
  setName(name);
  setColor(color);
  setU_Ht(u_Ht);
  setU_V(u_V);
  setU_T(u_T);
  setU_P(u_P);
  setCdaxis(cdaxis);
  setCfd(cfd);
  setCfdname(cfdname);
  setX1(X1);
  setY1(Y1);
  setH1(H1);
  setX2(X2);
  setY2(Y2);
  setH2(H2);
  setCelldx(celldx);
  setAxialD(axialD);
  setU_aD(u_aD);
  setU_L(u_L);
}

ZoneImpl::ZoneImpl(int nr,unsigned int flags,int ps,int pc,int pk,int pl,double relHt,double Vol,double T0,
  double P0,std::string name,int color,int u_Ht,int u_V,int u_T,int u_P,int cdaxis,int cfd,
  std::string cfdname,double X1,double Y1,double H1,double X2,double Y2,double H2,double celldx,
  double axialD,int u_aD,int u_L)
{
  setDefaults();
  setNr(nr);
  setFlags(flags);
  setPs(ps);
  setPc(pc);
  setPk(pk);
  setPl(pl);
  setRelHt(relHt);
  setVol(Vol);
  setT0(T0);
  setP0(P0);
  setName(name);
  setColor(color);
  setU_Ht(u_Ht);
  setU_V(u_V);
  setU_T(u_T);
  setU_P(u_P);
  setCdaxis(cdaxis);
  setCfd(cfd);
  setCfdname(cfdname);
  setX1(X1);
  setY1(Y1);
  setH1(H1);
  setX2(X2);
  setY2(Y2);
  setH2(H2);
  setCelldx(celldx);
  setAxialD(axialD);
  setU_aD(u_aD);
  setU_L(u_L);
}

void ZoneImpl::read(Reader &input)
{
  setNr(input.readInt());
  setFlags(input.readInt());
  setPs(input.readInt());
  setPc(input.readInt());
  setPk(input.readInt());
  setPl(input.readInt());
  setRelHt(input.readNumber<std::string>());
  setVol(input.readNumber<std::string>());
  setT0(input.readNumber<std::string>());
  setP0(input.readNumber<std::string>());
  setName(input.readString());
  setColor(input.readInt());
  setU_Ht(input.readInt());
  setU_V(input.readInt());
  setU_T(input.readInt());
  setU_P(input.readInt());
  setCdaxis(input.readInt());
  setCfd(input.readInt());
  if(cfd())
  {
    setCfdname(input.readString());
  }
  else if(cdaxis())
  {
    input.readString();  // Read "1D:"
    setX1(input.readNumber<std::string>());
    setY1(input.readNumber<std::string>());
    setH1(input.readNumber<std::string>());
    setX2(input.readNumber<std::string>());
    setY2(input.readNumber<std::string>());
    setH2(input.readNumber<std::string>());
    setCelldx(input.readNumber<std::string>());
    setAxialD(input.readNumber<std::string>());
    setU_aD(input.readInt());
    setU_L(input.readInt());
  }
}

std::string ZoneImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_flags) + ' ' + ANY_TO_STR(m_ps) + ' ' + ANY_TO_STR(m_pc) + ' '
    + ANY_TO_STR(m_pk) + ' ' + ANY_TO_STR(m_pl) + ' ' + ANY_TO_STR(m_relHt) + ' '
    + ANY_TO_STR(m_Vol) + ' ' + ANY_TO_STR(m_T0) + ' ' + ANY_TO_STR(m_P0) + ' ' + m_name + ' '
    + ANY_TO_STR(m_color) + ' ' + ANY_TO_STR(m_u_Ht) + ' ' + ANY_TO_STR(m_u_V) + ' ' + ANY_TO_STR(m_u_T) + ' '
    + ANY_TO_STR(m_u_P) + ' ' + ANY_TO_STR(m_cdaxis) + ' ' + ANY_TO_STR(m_cfd);
  if(m_cfd)
    string += ' ' + m_cfdname;
  else if(m_cdaxis)
    string += ANY_TO_STR(m_X1) + ' ' + ANY_TO_STR(m_Y1) + ' ' + ANY_TO_STR(m_H1) + ' '
    + ANY_TO_STR(m_X2) + ' ' + ANY_TO_STR(m_Y2) + ' ' + ANY_TO_STR(m_H2) + ' '
    + ANY_TO_STR(m_celldx) + ' ' + ANY_TO_STR(m_axialD) + ' ' + ANY_TO_STR(m_u_aD) + ' '
    + ANY_TO_STR(m_u_L);
  return string+'\n';
}

int ZoneImpl::nr() const
{
  return m_nr;
}

void ZoneImpl::setNr(const int nr)
{
  m_nr = nr;
}

unsigned int ZoneImpl::flags() const
{
  return m_flags;
}

void ZoneImpl::setFlags(const unsigned int flags)
{
  m_flags = flags;
}

int ZoneImpl::ps() const
{
  return m_ps;
}

void ZoneImpl::setPs(const int ps)
{
  m_ps = ps;
}

int ZoneImpl::pc() const
{
  return m_pc;
}

void ZoneImpl::setPc(const int pc)
{
  m_pc = pc;
}

int ZoneImpl::pk() const
{
  return m_pk;
}

void ZoneImpl::setPk(const int pk)
{
  m_pk = pk;
}

int ZoneImpl::pl() const
{
  return m_pl;
}

void ZoneImpl::setPl(const int pl)
{
  m_pl = pl;
}

double ZoneImpl::relHt() const
{
  return m_relHt.toDouble();
}

bool ZoneImpl::setRelHt(const double relHt)
{
  m_relHt = QString::number(relHt);
  return true;
}

bool ZoneImpl::setRelHt(const std::string &relHt)
{
  bool ok;
  FLOAT_CHECK(relHt,&ok);
  if(ok)
  {
    m_relHt = STR_TO_FLOAT(relHt);
    return true;
  }
  return false;
}

double ZoneImpl::Vol() const
{
  return m_Vol.toDouble();
}

bool ZoneImpl::setVol(const double Vol)
{
  m_Vol = QString::number(Vol);
  return true;
}

bool ZoneImpl::setVol(const std::string &Vol)
{
  bool ok;
  FLOAT_CHECK(Vol,&ok);
  if(ok)
  {
    m_Vol = STR_TO_FLOAT(Vol);
    return true;
  }
  return false;
}

double ZoneImpl::T0() const
{
  return m_T0.toDouble();
}

bool ZoneImpl::setT0(const double T0)
{
  m_T0 = QString::number(T0);
  return true;
}

bool ZoneImpl::setT0(const std::string &T0)
{
  bool ok;
  FLOAT_CHECK(T0,&ok);
  if(ok)
  {
    m_T0 = STR_TO_FLOAT(T0);
    return true;
  }
  return false;
}

double ZoneImpl::P0() const
{
  return m_P0.toDouble();
}

bool ZoneImpl::setP0(const double P0)
{
  m_P0 = QString::number(P0);
  return true;
}

bool ZoneImpl::setP0(const std::string P0)
{
  bool ok;
  FLOAT_CHECK(P0,&ok);
  if(ok)
  {
    m_P0 = STR_TO_FLOAT(P0);
    return true;
  }
  return false;
}

std::string ZoneImpl::name() const
{
  return m_name;
}

void ZoneImpl::setName(const std::string &name)
{
  m_name = name;
}

int ZoneImpl::color() const
{
  return m_color;
}

void ZoneImpl::setColor(const int color)
{
  m_color = color;
}

int ZoneImpl::u_Ht() const
{
  return m_u_Ht;
}

void ZoneImpl::setU_Ht(const int u_Ht)
{
  m_u_Ht = u_Ht;
}

int ZoneImpl::u_V() const
{
  return m_u_V;
}

void ZoneImpl::setU_V(const int u_V)
{
  m_u_V = u_V;
}

int ZoneImpl::u_T() const
{
  return m_u_T;
}

void ZoneImpl::setU_T(const int u_T)
{
  m_u_T = u_T;
}

int ZoneImpl::u_P() const
{
  return m_u_P;
}

void ZoneImpl::setU_P(const int u_P)
{
  m_u_P = u_P;
}

int ZoneImpl::cdaxis() const
{
  return m_cdaxis;
}

void ZoneImpl::setCdaxis(const int cdaxis)
{
  m_cdaxis = cdaxis;
}

int ZoneImpl::cfd() const
{
  return m_cfd;
}

void ZoneImpl::setCfd(const int cfd)
{
  m_cfd = cfd;
}

std::string ZoneImpl::cfdname() const
{
  return m_cfdname;
}

void ZoneImpl::setCfdname(const std::string cfdname)
{
  m_cfdname = cfdname;
}

double ZoneImpl::X1() const
{
  return m_X1.toDouble();
}

bool ZoneImpl::setX1(const double X1)
{
  m_X1 = QString::number(X1);
  return true;
}

bool ZoneImpl::setX1(const std::string &X1)
{
  bool ok;
  FLOAT_CHECK(X1,&ok);
  if(ok)
  {
    m_X1 = STR_TO_FLOAT(X1);
    return true;
  }
  return false;
}

double ZoneImpl::Y1() const
{
  return m_Y1.toDouble();
}

bool ZoneImpl::setY1(const double Y1)
{
  m_Y1 = QString::number(Y1);
  return true;
}

bool ZoneImpl::setY1(const std::string &Y1)
{
  bool ok;
  FLOAT_CHECK(Y1,&ok);
  if(ok)
  {
    m_Y1 = STR_TO_FLOAT(Y1);
    return true;
  }
  return false;
}

double ZoneImpl::H1() const
{
  return m_H1.toDouble();
}

bool ZoneImpl::setH1(const double H1)
{
  m_H1 = QString::number(H1);
  return true;
}

bool ZoneImpl::setH1(const std::string &H1)
{
  bool ok;
  FLOAT_CHECK(H1,&ok);
  if(ok)
  {
    m_H1 = STR_TO_FLOAT(H1);
    return true;
  }
  return false;
}

double ZoneImpl::X2() const
{
  return m_X2.toDouble();
}

bool ZoneImpl::setX2(const double X2)
{
  m_X2 = QString::number(X2);
  return true;
}

bool ZoneImpl::setX2(const std::string &X2)
{
  bool ok;
  FLOAT_CHECK(X2,&ok);
  if(ok)
  {
    m_X2 = STR_TO_FLOAT(X2);
    return true;
  }
  return false;
}

double ZoneImpl::Y2() const
{
  return m_Y2.toDouble();
}

bool ZoneImpl::setY2(const double Y2)
{
  m_Y2 = QString::number(Y2);
  return true;
}

bool ZoneImpl::setY2(const std::string &Y2)
{
  bool ok;
  FLOAT_CHECK(Y2,&ok);
  if(ok)
  {
    m_Y2 = STR_TO_FLOAT(Y2);
    return true;
  }
  return false;
}

double ZoneImpl::H2() const
{
  return m_H2.toDouble();
}

bool ZoneImpl::setH2(const double H2)
{
  m_H2 = QString::number(H2);
  return true;
}

bool ZoneImpl::setH2(const std::string &H2)
{
  bool ok;
  FLOAT_CHECK(H2,&ok);
  if(ok)
  {
    m_H2 = STR_TO_FLOAT(H2);
    return true;
  }
  return false;
}

double ZoneImpl::celldx() const
{
  return m_celldx.toDouble();
}

bool ZoneImpl::setCelldx(const double celldx)
{
  m_celldx = QString::number(celldx);
  return true;
}

bool ZoneImpl::setCelldx(const std::string &celldx)
{
  bool ok;
  FLOAT_CHECK(celldx,&ok);
  if(ok)
  {
    m_celldx = STR_TO_FLOAT(celldx);
    return true;
  }
  return false;
}

double ZoneImpl::axialD() const
{
  return m_axialD.toDouble();
}

bool ZoneImpl::setAxialD(const double axialD)
{
  m_axialD = QString::number(axialD);
  return true;
}

bool ZoneImpl::setAxialD(const std::string &axialD)
{
  bool ok;
  FLOAT_CHECK(axialD,&ok);
  if(ok)
  {
    m_axialD = STR_TO_FLOAT(axialD);
    return true;
  }
  return false;
}

int ZoneImpl::u_aD() const
{
  return m_u_aD;
}

void ZoneImpl::setU_aD(const int u_aD)
{
  m_u_aD = u_aD;
}

int ZoneImpl::u_L() const
{
  return m_u_L;
}

void ZoneImpl::setU_L(const int u_L)
{
  m_u_L = u_L;
}

void ZoneImpl::setVariablePressure(bool b)
{
  if(b)
    m_flags |= VAR_P;
  else
    m_flags &= NVAR_P;
}

bool ZoneImpl::variablePressure() const
{
  return m_flags & VAR_P;
}

void ZoneImpl::setVariableContaminants(bool b)
{
  if(b)
    m_flags |= VAR_C;
  else
    m_flags &= NVAR_C;
}

bool ZoneImpl::variableContaminants() const
{
  return m_flags & VAR_C;
}

void ZoneImpl::setSystem(bool b)
{
  if(b)
    m_flags |= SYS_N;
  else
    m_flags &= NSYS_N;
}

bool ZoneImpl::system() const
{
  return m_flags & SYS_N;
}

double ZoneImpl::ic(const int i) const
{
  return m_ic[i].toDouble();
}

std::vector<double> ZoneImpl::ic() const
{
  std::vector<double> out;
  for(int i=0;i<m_ic.size();i++)
  {
    out.push_back(m_ic[i].toDouble());
  }
  return out;
}

bool ZoneImpl::setIc(const int i, const double value)
{
  m_ic[i] = QString::number(value);
  return true;
}

bool ZoneImpl::setIc(const int i, const std::string &value)
{
  bool ok;
  FLOAT_CHECK(value,&ok);
  if(ok)
  {
    m_ic[i] = STR_TO_FLOAT(value);
    return true;
  }
  return false;
}

bool ZoneImpl::setIc(std::vector<double> &ic)
{
  QVector<QString> in;
  for(unsigned i=0;i<ic.size();i++)
  {
    in << QString::number(ic[i]);
  }
  m_ic = in;
  return true;
}

bool ZoneImpl::setIc(std::vector<std::string> &ic)
{
  QVector<QString> in;
  for(unsigned i=0;i<ic.size();i++)
  {
    bool ok;
    FLOAT_CHECK(ic[i],&ok);
    if(!ok)
    {
      return false;
    }
    in << STR_TO_FLOAT(ic[i]);
  }
  m_ic = in;
  return true;
}

void SpeciesImpl::setDefaults()
{
  m_nr = 0;
  m_sflag = 0;
  m_ntflag = 0;
  m_molwt = PRJFLOAT("0.0");
  m_mdiam = PRJFLOAT("0.0");
  m_edens = PRJFLOAT("0.0");
  m_decay = PRJFLOAT("0.0");
  m_Dm = PRJFLOAT("0.0");
  m_ccdef = PRJFLOAT("0.0");
  m_Cp = PRJFLOAT("0.0");
  m_ucc = 0;
  m_umd = 0;
  m_ued = 0;
  m_udm = 0;
  m_ucp = 0;
}

SpeciesImpl::SpeciesImpl()
{
  setDefaults();
}

SpeciesImpl::SpeciesImpl(int nr,int sflag,int ntflag,std::string molwt,std::string mdiam,std::string edens,std::string decay,
  std::string Dm,std::string ccdef,std::string Cp,int ucc, int umd,int ued,int udm,int ucp,std::string name,
  std::string desc)
{
  setDefaults();
  setNr(nr);
  setSflag(sflag);
  setNtflag(ntflag);
  setMolwt(molwt);
  setMdiam(mdiam);
  setEdens(edens);
  setDecay(decay);
  setDm(Dm);
  setCcdef(ccdef);
  setCp(Cp);
  setUcc(ucc);
  setUmd(umd);
  setUed(ued);
  setUdm(udm);
  setUcp(ucp);
  setName(name);
  setDesc(desc);
}

SpeciesImpl::SpeciesImpl(int nr,int sflag,int ntflag,double molwt,double mdiam,double edens,double decay,
  double Dm,double ccdef,double Cp,int ucc,int umd,int ued,int udm,int ucp,std::string name,
  std::string desc)
{
  setDefaults();
  setNr(nr);
  setSflag(sflag);
  setNtflag(ntflag);
  setMolwt(molwt);
  setMdiam(mdiam);
  setEdens(edens);
  setDecay(decay);
  setDm(Dm);
  setCcdef(ccdef);
  setCp(Cp);
  setUcc(ucc);
  setUmd(umd);
  setUed(ued);
  setUdm(udm);
  setUcp(ucp);
  setName(name);
  setDesc(desc);
}

void SpeciesImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setSflag(input.read<int>());
  setNtflag(input.read<int>());
  setMolwt(input.read<std::string>());
  setMdiam(input.read<std::string>());
  setEdens(input.read<std::string>());
  setDecay(input.read<std::string>());
  setDm(input.read<std::string>());
  setCcdef(input.read<std::string>());
  setCp(input.read<std::string>());
  setUcc(input.read<int>());
  setUmd(input.read<int>());
  setUed(input.read<int>());
  setUdm(input.read<int>());
  setUcp(input.read<int>());
  setName(input.readString());
  setDesc(input.readLine());
}

std::string SpeciesImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_sflag) + ' ' + ANY_TO_STR(m_ntflag) + ' ' + ANY_TO_STR(m_molwt) + ' ' + ANY_TO_STR(m_mdiam) + ' ' + ANY_TO_STR(m_edens) + ' ' + ANY_TO_STR(m_decay) + ' ' + ANY_TO_STR(m_Dm) + ' ' + ANY_TO_STR(m_ccdef) + ' ' + ANY_TO_STR(m_Cp) + ' ' + ANY_TO_STR(m_ucc) + ' ' + ANY_TO_STR(m_umd) + ' ' + ANY_TO_STR(m_ued) + ' ' + ANY_TO_STR(m_udm) + ' ' + ANY_TO_STR(m_ucp) + ' ' + m_name + '\n';
  string += m_desc + '\n';
  return string;
}

int SpeciesImpl::nr() const
{
  return m_nr;
}

void SpeciesImpl::setNr(const int nr)
{
  m_nr = nr;
}

int SpeciesImpl::sflag() const
{
  return m_sflag;
}

void SpeciesImpl::setSflag(const int sflag)
{
  m_sflag = sflag;
}

int SpeciesImpl::ntflag() const
{
  return m_ntflag;
}

void SpeciesImpl::setNtflag(const int ntflag)
{
  m_ntflag = ntflag;
}

double SpeciesImpl::molwt() const
{
  return m_molwt.toDouble();
}

bool SpeciesImpl::setMolwt(const double molwt)
{
  m_molwt = QString::number(molwt);
  return true;
}

bool SpeciesImpl::setMolwt(const std::string &molwt)
{
  bool ok;
  FLOAT_CHECK(molwt,&ok);
  if(ok)
  {
    m_molwt = STR_TO_FLOAT(molwt);
    return true;
  }
  return false;
}

double SpeciesImpl::mdiam() const
{
  return m_mdiam.toDouble();
}

bool SpeciesImpl::setMdiam(const double mdiam)
{
  m_mdiam = QString::number(mdiam);
  return true;
}

bool SpeciesImpl::setMdiam(const std::string &mdiam)
{
  bool ok;
  FLOAT_CHECK(mdiam,&ok);
  if(ok)
  {
    m_mdiam = STR_TO_FLOAT(mdiam);
    return true;
  }
  return false;
}

double SpeciesImpl::edens() const
{
  return m_edens.toDouble();
}

bool SpeciesImpl::setEdens(const double edens)
{
  m_edens = QString::number(edens);
  return true;
}

bool SpeciesImpl::setEdens(const std::string &edens)
{
  bool ok;
  FLOAT_CHECK(edens,&ok);
  if(ok)
  {
    m_edens = STR_TO_FLOAT(edens);
    return true;
  }
  return false;
}

double SpeciesImpl::decay() const
{
  return m_decay.toDouble();
}

bool SpeciesImpl::setDecay(const double decay)
{
  m_decay = decay;
  return true;
}

bool SpeciesImpl::setDecay(const std::string &decay)
{
  bool ok;
  FLOAT_CHECK(decay,&ok);
  if(ok)
  {
    m_decay = STR_TO_FLOAT(decay);
    return true;
  }
  return false;
}

double SpeciesImpl::Dm() const
{
  return m_Dm.toDouble();
}

bool SpeciesImpl::setDm(const double Dm)
{
  m_Dm = QString::number(Dm);
  return true;
}

bool SpeciesImpl::setDm(const std::string &Dm)
{
  bool ok;
  FLOAT_CHECK(Dm,&ok);
  if(ok)
  {
    m_Dm = STR_TO_FLOAT(Dm);
    return true;
  }
  return false;
}

double SpeciesImpl::ccdef() const
{
  return m_ccdef.toDouble();
}

bool SpeciesImpl::setCcdef(const double ccdef)
{
  m_ccdef = QString::number(ccdef);
  return true;
}

bool SpeciesImpl::setCcdef(const std::string &ccdef)
{
  bool ok;
  FLOAT_CHECK(ccdef,&ok);
  if(ok)
  {
    m_ccdef = STR_TO_FLOAT(ccdef);
    return true;
  }
  return false;
}

double SpeciesImpl::Cp() const
{
  return m_Cp.toDouble();
}

bool SpeciesImpl::setCp(const double Cp)
{
  m_Cp = Cp;
  return true;
}

bool SpeciesImpl::setCp(const std::string &Cp)
{
  bool ok;
  FLOAT_CHECK(Cp,&ok);
  if(ok)
  {
    m_Cp = STR_TO_FLOAT(Cp);
    return true;
  }
  return false;
}

int SpeciesImpl::ucc() const
{
  return m_ucc;
}

void SpeciesImpl::setUcc(const int ucc)
{
  m_ucc = ucc;
}

int SpeciesImpl::umd() const
{
  return m_umd;
}

void SpeciesImpl::setUmd(const int umd)
{
  m_umd = umd;
}

int SpeciesImpl::ued() const
{
  return m_ued;
}

void SpeciesImpl::setUed(const int ued)
{
  m_ued = ued;
}

int SpeciesImpl::udm() const
{
  return m_udm;
}

void SpeciesImpl::setUdm(const int udm)
{
  m_udm = udm;
}

int SpeciesImpl::ucp() const
{
  return m_ucp;
}

void SpeciesImpl::setUcp(const int ucp)
{
  m_ucp = ucp;
}

std::string SpeciesImpl::name() const
{
  return m_name;
}

void SpeciesImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string SpeciesImpl::desc() const
{
  return m_desc;
}

void SpeciesImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

void AhsImpl::setDefaults()
{
  m_nr = 0;
  m_zone_r = 0;
  m_zone_s = 0;
  m_path_r = 0;
  m_path_s = 0;
  m_path_x = 0;
}

AhsImpl::AhsImpl()
{
  setDefaults();
}

AhsImpl::AhsImpl(int nr,int zone_r,int zone_s,int path_r,int path_s,int path_x,
  std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setZone_r(zone_r);
  setZone_s(zone_s);
  setPath_r(path_r);
  setPath_s(path_s);
  setPath_x(path_x);
  setName(name);
  setDesc(desc);
}

void AhsImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setZone_r(input.read<int>());
  setZone_s(input.read<int>());
  setPath_r(input.read<int>());
  setPath_s(input.read<int>());
  setPath_x(input.read<int>());
  setName(input.readString());
  setDesc(input.readLine());
}

std::string AhsImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_zone_r) + ' ' + ANY_TO_STR(m_zone_s) + ' ' + ANY_TO_STR(m_path_r) + ' ' + ANY_TO_STR(m_path_s) + ' ' + ANY_TO_STR(m_path_x) + ' ' + m_name + '\n';
  string += m_desc + '\n';
  return string;
}

int AhsImpl::nr() const
{
  return m_nr;
}

void AhsImpl::setNr(const int nr)
{
  m_nr = nr;
}

int AhsImpl::zone_r() const
{
  return m_zone_r;
}

void AhsImpl::setZone_r(const int zone_r)
{
  m_zone_r = zone_r;
}

int AhsImpl::zone_s() const
{
  return m_zone_s;
}

void AhsImpl::setZone_s(const int zone_s)
{
  m_zone_s = zone_s;
}

int AhsImpl::path_r() const
{
  return m_path_r;
}

void AhsImpl::setPath_r(const int path_r)
{
  m_path_r = path_r;
}

int AhsImpl::path_s() const
{
  return m_path_s;
}

void AhsImpl::setPath_s(const int path_s)
{
  m_path_s = path_s;
}

int AhsImpl::path_x() const
{
  return m_path_x;
}

void AhsImpl::setPath_x(const int path_x)
{
  m_path_x = path_x;
}

std::string AhsImpl::name() const
{
  return m_name;
}

void AhsImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string AhsImpl::desc() const
{
  return m_desc;
}

void AhsImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

void AirflowPathImpl::setDefaults()
{
  m_nr = 0;
  m_flags = 0;
  m_pzn = 0;
  m_pzm = 0;
  m_pe = 0;
  m_pf = 0;
  m_pw = 0;
  m_pa = 0;
  m_ps = 0;
  m_pc = 0;
  m_pld = 0;
  m_X = PRJFLOAT("0.0");
  m_Y = PRJFLOAT("0.0");
  m_relHt = PRJFLOAT("0.0");
  m_mult = PRJFLOAT("0.0");
  m_wPset = PRJFLOAT("0.0");
  m_wPmod = PRJFLOAT("0.0");
  m_wazm = PRJFLOAT("0.0");
  m_Fahs = PRJFLOAT("0.0");
  m_Xmax = PRJFLOAT("0.0");
  m_Xmin = PRJFLOAT("0.0");
  m_icon = 0;
  m_dir = 0;
  m_u_Ht = 0;
  m_u_XY = 0;
  m_u_dP = 0;
  m_u_F = 0;
  m_cfd = 0;
  m_cfd_ptype = 0;
  m_cfd_btype = 0;
  m_cfd_capp = 0;
}

AirflowPathImpl::AirflowPathImpl()
{
  setDefaults();
}

AirflowPathImpl::AirflowPathImpl(int nr,int flags,int pzn,int pzm,int pe,int pf,int pw,int pa,int ps,int pc,int pld,
  std::string X,std::string Y,std::string relHt,std::string mult,std::string wPset,std::string wPmod,std::string wazm,
  std::string Fahs,std::string Xmax,std::string Xmin,unsigned int icon,unsigned int dir,int u_Ht,int u_XY,
  int u_dP,int u_F,int cfd,std::string cfd_name,int cfd_ptype,int cfd_btype,int cfd_capp)
{
  setDefaults();
  setNr(nr);
  setFlags(flags);
  setPzn(pzn);
  setPzm(pzm);
  setPe(pe);
  setPf(pf);
  setPw(pw);
  setPa(pa);
  setPs(ps);
  setPc(pc);
  setPld(pld);
  setX(X);
  setY(Y);
  setRelHt(relHt);
  setMult(mult);
  setWPset(wPset);
  setWPmod(wPmod);
  setWazm(wazm);
  setFahs(Fahs);
  setXmax(Xmax);
  setXmin(Xmin);
  setIcon(icon);
  setDir(dir);
  setU_Ht(u_Ht);
  setU_XY(u_XY);
  setU_dP(u_dP);
  setU_F(u_F);
  setCfd(cfd);
  setCfd_name(cfd_name);
  setCfd_ptype(cfd_ptype);
  setCfd_btype(cfd_btype);
  setCfd_capp(cfd_capp);
}

AirflowPathImpl::AirflowPathImpl(int nr,int flags,int pzn,int pzm,int pe,int pf,int pw,int pa,int ps,int pc,int pld,
  double X,double Y,double relHt,double mult,double wPset,double wPmod,double wazm,
  double Fahs,double Xmax,double Xmin,unsigned int icon,unsigned int dir,int u_Ht,int u_XY,
  int u_dP,int u_F,int cfd,std::string cfd_name,int cfd_ptype,int cfd_btype,int cfd_capp)
{
  setDefaults();
  setNr(nr);
  setFlags(flags);
  setPzn(pzn);
  setPzm(pzm);
  setPe(pe);
  setPf(pf);
  setPw(pw);
  setPa(pa);
  setPs(ps);
  setPc(pc);
  setPld(pld);
  setX(X);
  setY(Y);
  setRelHt(relHt);
  setMult(mult);
  setWPset(wPset);
  setWPmod(wPmod);
  setWazm(wazm);
  setFahs(Fahs);
  setXmax(Xmax);
  setXmin(Xmin);
  setIcon(icon);
  setDir(dir);
  setU_Ht(u_Ht);
  setU_XY(u_XY);
  setU_dP(u_dP);
  setU_F(u_F);
  setCfd(cfd);
  setCfd_name(cfd_name);
  setCfd_ptype(cfd_ptype);
  setCfd_btype(cfd_btype);
  setCfd_capp(cfd_capp);
}

void AirflowPathImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setFlags(input.read<int>());
  setPzn(input.read<int>());
  setPzm(input.read<int>());
  setPe(input.read<int>());
  setPf(input.read<int>());
  setPw(input.read<int>());
  setPa(input.read<int>());
  setPs(input.read<int>());
  setPc(input.read<int>());
  setPld(input.read<int>());
  setX(input.readNumber<std::string>());
  setY(input.readNumber<std::string>());
  setRelHt(input.readNumber<std::string>());
  setMult(input.readNumber<std::string>());
  setWPset(input.readNumber<std::string>());
  setWPmod(input.readNumber<std::string>());
  setWazm(input.readNumber<std::string>());
  setFahs(input.readNumber<std::string>());
  setXmax(input.readNumber<std::string>());
  setXmin(input.readNumber<std::string>());
  setIcon(input.read<unsigned int>());
  setDir(input.read<unsigned int>());
  setU_Ht(input.read<int>());
  setU_XY(input.read<int>());
  setU_dP(input.read<int>());
  setU_F(input.read<int>());
  setCfd(input.read<int>());
  if(m_cfd)
  {
    setCfd_name(input.readString());
    setCfd_ptype(input.read<int>());
    setCfd_btype(input.read<int>());
    setCfd_capp(input.read<int>());
  }
}

std::string AirflowPathImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_flags) + ' ' + ANY_TO_STR(m_pzn) + ' ' + ANY_TO_STR(m_pzm) + ' '
    + ANY_TO_STR(m_pe) + ' ' + ANY_TO_STR(m_pf) + ' ' + ANY_TO_STR(m_pw) + ' ' + ANY_TO_STR(m_pa) + ' '
    + ANY_TO_STR(m_ps) + ' ' + ANY_TO_STR(m_pc) + ' ' + ANY_TO_STR(m_pld) + ' ' + ANY_TO_STR(m_X) + ' '
    + ANY_TO_STR(m_Y) + ' ' + ANY_TO_STR(m_relHt) + ' ' + ANY_TO_STR(m_mult) + ' ' + ANY_TO_STR(m_wPset) + ' '
    + ANY_TO_STR(m_wPmod) + ' ' + ANY_TO_STR(m_wazm) + ' ' + ANY_TO_STR(m_Fahs) + ' ' + ANY_TO_STR(m_Xmax) + ' '
    + ANY_TO_STR(m_Xmin) + ' ' + ANY_TO_STR(m_icon) + ' ' + ANY_TO_STR(m_dir) + ' ' + ANY_TO_STR(m_u_Ht) + ' '
    + ANY_TO_STR(m_u_XY) + ' ' + ANY_TO_STR(m_u_dP) + ' ' + ANY_TO_STR(m_u_F) + ' ' + ANY_TO_STR(m_cfd);
  if(m_cfd)
  {
    string += ' ' + m_cfd_name + ' ' + ANY_TO_STR(m_cfd_ptype) + ' ' + ANY_TO_STR(m_cfd_btype) + ' '
      + ANY_TO_STR(m_cfd_capp);
  }
  return string + '\n';
}

int AirflowPathImpl::nr() const
{
  return m_nr;
}

void AirflowPathImpl::setNr(const int nr)
{
  m_nr = nr;
}

int AirflowPathImpl::flags() const
{
  return m_flags;
}

void AirflowPathImpl::setFlags(const int flags)
{
  m_flags = flags;
}

int AirflowPathImpl::pzn() const
{
  return m_pzn;
}

void AirflowPathImpl::setPzn(const int pzn)
{
  m_pzn = pzn;
}

int AirflowPathImpl::pzm() const
{
  return m_pzm;
}

void AirflowPathImpl::setPzm(const int pzm)
{
  m_pzm = pzm;
}

int AirflowPathImpl::pe() const
{
  return m_pe;
}

void AirflowPathImpl::setPe(const int pe)
{
  m_pe = pe;
}

int AirflowPathImpl::pf() const
{
  return m_pf;
}

void AirflowPathImpl::setPf(const int pf)
{
  m_pf = pf;
}

int AirflowPathImpl::pw() const
{
  return m_pw;
}

void AirflowPathImpl::setPw(const int pw)
{
  m_pw = pw;
}

int AirflowPathImpl::pa() const
{
  return m_pa;
}

void AirflowPathImpl::setPa(const int pa)
{
  m_pa = pa;
}

int AirflowPathImpl::ps() const
{
  return m_ps;
}

void AirflowPathImpl::setPs(const int ps)
{
  m_ps = ps;
}

int AirflowPathImpl::pc() const
{
  return m_pc;
}

void AirflowPathImpl::setPc(const int pc)
{
  m_pc = pc;
}

int AirflowPathImpl::pld() const
{
  return m_pld;
}

void AirflowPathImpl::setPld(const int pld)
{
  m_pld = pld;
}

double AirflowPathImpl::X() const
{
  return m_X.toDouble();
}

bool AirflowPathImpl::setX(const double X)
{
  m_X = QString::number(X);
  return true;
}

bool AirflowPathImpl::setX(const std::string &X)
{
  bool ok;
  FLOAT_CHECK(X,&ok);
  if(ok)
  {
    m_X = STR_TO_FLOAT(X);
    return true;
  }
  return false;
}

double AirflowPathImpl::Y() const
{
  return m_Y.toDouble();
}

bool AirflowPathImpl::setY(const double Y)
{
  m_Y = QString::number(Y);
  return true;
}

bool AirflowPathImpl::setY(const std::string &Y)
{
  bool ok;
  FLOAT_CHECK(Y,&ok);
  if(ok)
  {
    m_Y = STR_TO_FLOAT(Y);
    return true;
  }
  return false;
}

double AirflowPathImpl::relHt() const
{
  return m_relHt.toDouble();
}

bool AirflowPathImpl::setRelHt(const double relHt)
{
  m_relHt = QString::number(relHt);
  return true;
}

bool AirflowPathImpl::setRelHt(const std::string &relHt)
{
  bool ok;
  FLOAT_CHECK(relHt,&ok);
  if(ok)
  {
    m_relHt = STR_TO_FLOAT(relHt);
    return true;
  }
  return false;
}

double AirflowPathImpl::mult() const
{
  return m_mult.toDouble();
}

bool AirflowPathImpl::setMult(const double mult)
{
  m_mult = QString::number(mult);
  return true;
}

bool AirflowPathImpl::setMult(const std::string &mult)
{
  bool ok;
  FLOAT_CHECK(mult,&ok);
  if(ok)
  {
    m_mult = STR_TO_FLOAT(mult);
    return true;
  }
  return false;
}

double AirflowPathImpl::wPset() const
{
  return m_wPset.toDouble();
}

bool AirflowPathImpl::setWPset(const double wPset)
{
  m_wPset = QString::number(wPset);
  return true;
}

bool AirflowPathImpl::setWPset(const std::string &wPset)
{
  bool ok;
  FLOAT_CHECK(wPset,&ok);
  if(ok)
  {
    m_wPset = STR_TO_FLOAT(wPset);
    return true;
  }
  return false;
}

double AirflowPathImpl::wPmod() const
{
  return m_wPmod.toDouble();
}

bool AirflowPathImpl::setWPmod(const double wPmod)
{
  m_wPmod = QString::number(wPmod);
  return true;
}

bool AirflowPathImpl::setWPmod(const std::string &wPmod)
{
  bool ok;
  FLOAT_CHECK(wPmod,&ok);
  if(ok)
  {
    m_wPmod = STR_TO_FLOAT(wPmod);
    return true;
  }
  return false;
}

double AirflowPathImpl::wazm() const
{
  return m_wazm.toDouble();
}

bool AirflowPathImpl::setWazm(const double wazm)
{
  m_wazm = QString::number(wazm);
  return true;
}

bool AirflowPathImpl::setWazm(const std::string &wazm)
{
  bool ok;
  FLOAT_CHECK(wazm,&ok);
  if(ok)
  {
    m_wazm = STR_TO_FLOAT(wazm);
    return true;
  }
  return false;
}

double AirflowPathImpl::Fahs() const
{
  return m_Fahs.toDouble();
}

bool AirflowPathImpl::setFahs(const double Fahs)
{
  m_Fahs = QString::number(Fahs);
  return true;
}

bool AirflowPathImpl::setFahs(const std::string &Fahs)
{
  bool ok;
  FLOAT_CHECK(Fahs,&ok);
  if(ok)
  {
    m_Fahs = STR_TO_FLOAT(Fahs);
    return true;
  }
  return false;
}

double AirflowPathImpl::Xmax() const
{
  return m_Xmax.toDouble();
}

bool AirflowPathImpl::setXmax(const double Xmax)
{
  m_Xmax = QString::number(Xmax);
  return true;
}

bool AirflowPathImpl::setXmax(const std::string &Xmax)
{
  bool ok;
  FLOAT_CHECK(Xmax,&ok);
  if(ok)
  {
    m_Xmax = STR_TO_FLOAT(Xmax);
    return true;
  }
  return false;
}

double AirflowPathImpl::Xmin() const
{
  return m_Xmin.toDouble();
}

bool AirflowPathImpl::setXmin(const double Xmin)
{
  m_Xmin = QString::number(Xmin);
  return true;
}

bool AirflowPathImpl::setXmin(const std::string &Xmin)
{
  bool ok;
  FLOAT_CHECK(Xmin,&ok);
  if(ok)
  {
    m_Xmin = STR_TO_FLOAT(Xmin);
    return true;
  }
  return false;
}

unsigned int AirflowPathImpl::icon() const
{
  return m_icon;
}

void AirflowPathImpl::setIcon(const unsigned int icon)
{
  m_icon = icon;
}

unsigned int AirflowPathImpl::dir() const
{
  return m_dir;
}

void AirflowPathImpl::setDir(const unsigned int dir)
{
  m_dir = dir;
}

int AirflowPathImpl::u_Ht() const
{
  return m_u_Ht;
}

void AirflowPathImpl::setU_Ht(const int u_Ht)
{
  m_u_Ht = u_Ht;
}

int AirflowPathImpl::u_XY() const
{
  return m_u_XY;
}

void AirflowPathImpl::setU_XY(const int u_XY)
{
  m_u_XY = u_XY;
}

int AirflowPathImpl::u_dP() const
{
  return m_u_dP;
}

void AirflowPathImpl::setU_dP(const int u_dP)
{
  m_u_dP = u_dP;
}

int AirflowPathImpl::u_F() const
{
  return m_u_F;
}

void AirflowPathImpl::setU_F(const int u_F)
{
  m_u_F = u_F;
}

int AirflowPathImpl::cfd() const
{
  return m_cfd;
}

void AirflowPathImpl::setCfd(const int cfd)
{
  m_cfd = cfd;
}

std::string AirflowPathImpl::cfd_name() const
{
  return m_cfd_name;
}

void AirflowPathImpl::setCfd_name(const std::string &cfd_name)
{
  m_cfd_name = cfd_name;
}

int AirflowPathImpl::cfd_ptype() const
{
  return m_cfd_ptype;
}

void AirflowPathImpl::setCfd_ptype(const int cfd_ptype)
{
  m_cfd_ptype = cfd_ptype;
}

int AirflowPathImpl::cfd_btype() const
{
  return m_cfd_btype;
}

void AirflowPathImpl::setCfd_btype(const int cfd_btype)
{
  m_cfd_btype = cfd_btype;
}

int AirflowPathImpl::cfd_capp() const
{
  return m_cfd_capp;
}

void AirflowPathImpl::setCfd_capp(const int cfd_capp)
{
  m_cfd_capp = cfd_capp;
}

void AirflowPathImpl::setWindPressure(bool b)
{
  if(b)
    m_flags |= WIND;
  else
    m_flags &= 0xFFFE;
}

bool AirflowPathImpl::windPressure()
{
  return m_flags & WIND;
}

void AirflowPathImpl::setSystem(bool b)
{
  if(b)
    m_flags |= AHS_S;
  else
    m_flags &= 0xFFF7;
}

bool AirflowPathImpl::system()
{
  return m_flags & AHS_S;
}

void AirflowPathImpl::setExhaust(bool b)
{
  if(b)
    m_flags |= AHS_X;
  else
    m_flags &= 0xFFBF;
}

bool AirflowPathImpl::exhaust()
{
  return m_flags & AHS_X;
}


void AirflowPathImpl::setRecirculation(bool b)
{
  if(b)
    m_flags |= AHS_R;
  else
    m_flags &= 0xFFEF;
}

bool AirflowPathImpl::recirculation()
{
  return m_flags & AHS_R;
}

void AirflowPathImpl::setOutsideAir(bool b)
{
  if(b)
    m_flags |= AHS_O;
  else
    m_flags &= 0xFFDF;
}

bool AirflowPathImpl::outsideAir()
{
  return m_flags & AHS_O;
}

void RunControlImpl::setDefaults()
{
  m_sim_af = 0;
  m_afcalc = 0;
  m_afmaxi = 0;
  m_afrcnvg = PRJFLOAT("0.0");
  m_afacnvg = PRJFLOAT("0.0");
  m_afrelax = PRJFLOAT("0.0");
  m_uac2 = 0;
  m_Pres = PRJFLOAT("0.0");
  m_uPres = 0;
  m_afslae = 0;
  m_afrseq = 0;
  m_aflmaxi = 0;
  m_aflcnvg = PRJFLOAT("0.0");
  m_aflinit = 0;
  m_Tadj = 0;
  m_sim_mf = 0;
  m_ccmaxi = 0;
  m_ccrcnvg = PRJFLOAT("0.0");
  m_ccacnvg = PRJFLOAT("0.0");
  m_ccrelax = PRJFLOAT("0.0");
  m_uccc = 0;
  m_mfnmthd = 0;
  m_mfnrseq = 0;
  m_mfnmaxi = 0;
  m_mfnrcnvg = PRJFLOAT("0.0");
  m_mfnacnvg = PRJFLOAT("0.0");
  m_mfnrelax = PRJFLOAT("0.0");
  m_mfngamma = PRJFLOAT("0.0");
  m_uccn = 0;
  m_mftmthd = 0;
  m_mftrseq = 0;
  m_mftmaxi = 0;
  m_mftrcnvg = PRJFLOAT("0.0");
  m_mftacnvg = PRJFLOAT("0.0");
  m_mftrelax = PRJFLOAT("0.0");
  m_mftgamma = PRJFLOAT("0.0");
  m_ucct = 0;
  m_mfvmthd = 0;
  m_mfvrseq = 0;
  m_mfvmaxi = 0;
  m_mfvrcnvg = PRJFLOAT("0.0");
  m_mfvacnvg = PRJFLOAT("0.0");
  m_mfvrelax = PRJFLOAT("0.0");
  m_uccv = 0;
  m_mf_solver = 0;
  m_sim_1dz = 0;
  m_sim_1dd = 0;
  m_celldx = PRJFLOAT("0.0");
  m_sim_vjt = 0;
  m_udx = 0;
  m_cvode_mth = 0;
  m_cvode_rcnvg = PRJFLOAT("0.0");
  m_cvode_acnvg = PRJFLOAT("0.0");
  m_cvode_dtmax = PRJFLOAT("0.0");
  m_tsdens = 0;
  m_tsrelax = PRJFLOAT("0.0");
  m_tsmaxi = 0;
  m_cnvgSS = 0;
  m_densZP = 0;
  m_stackD = 0;
  m_dodMdt = 0;
  m_restart = 0;
  m_list = 0;
  m_doDlg = 0;
  m_pfsave = 0;
  m_zfsave = 0;
  m_zcsave = 0;
  m_achvol = 0;
  m_achsave = 0;
  m_abwsave = 0;
  m_cbwsave = 0;
  m_expsave = 0;
  m_ebwsave = 0;
  m_zaasave = 0;
  m_zbwsave = 0;
  m_rzfsave = 0;
  m_rzmsave = 0;
  m_rz1save = 0;
  m_csmsave = 0;
  m_srfsave = 0;
  m_logsave = 0;
  m_BldgFlowZ = 0;
  m_BldgFlowD = 0;
  m_BldgFlowC = 0;
  m_cfd_ctype = 0;
  m_cfd_convcpl = PRJFLOAT("0.0");
  m_cfd_var = 0;
  m_cfd_zref = 0;
  m_cfd_imax = 0;
  m_cfd_dtcmo = 0;
}

RunControlImpl::RunControlImpl()
{
  setDefaults();
}

RunControlImpl::RunControlImpl(int sim_af,int afcalc,int afmaxi,double afrcnvg,double afacnvg,double afrelax,
  int uac2,double Pres,int uPres,int afslae,int afrseq,int aflmaxi,
  double aflcnvg,int aflinit,int Tadj,int sim_mf,int ccmaxi,double ccrcnvg,
  double ccacnvg,double ccrelax,int uccc,int mfnmthd,int mfnrseq,int mfnmaxi,
  double mfnrcnvg,double mfnacnvg,double mfnrelax,double mfngamma,int uccn,
  int mftmthd,int mftrseq,int mftmaxi,double mftrcnvg,double mftacnvg,
  double mftrelax,double mftgamma,int ucct,int mfvmthd,int mfvrseq,
  int mfvmaxi,double mfvrcnvg,double mfvacnvg,double mfvrelax,int uccv,
  int mf_solver,int sim_1dz,int sim_1dd,double celldx,int sim_vjt,int udx,
  int cvode_mth,double cvode_rcnvg,double cvode_acnvg,double cvode_dtmax,
  int tsdens,double tsrelax,int tsmaxi,int cnvgSS,int densZP,int stackD,
  int dodMdt,std::string date_st,std::string time_st,std::string date_0,std::string time_0,
  std::string date_1,std::string time_1,std::string time_step,std::string time_list,
  std::string time_scrn,int restart,std::string rstdate,std::string rsttime,int list,
  int doDlg,int pfsave,int zfsave,int zcsave,int achvol,int achsave,
  int abwsave,int cbwsave,int expsave,int ebwsave,int zaasave,int zbwsave,
  int rzfsave,int rzmsave,int rz1save,int csmsave,int srfsave,int logsave,
  std::vector<int> save,std::vector<double> rvals,int BldgFlowZ,int BldgFlowD,
  int BldgFlowC,int cfd_ctype,double cfd_convcpl,int cfd_var,int cfd_zref,
  int cfd_imax,int cfd_dtcmo)
{
  setDefaults();
  setSim_af(sim_af);
  setAfcalc(afcalc);
  setAfmaxi(afmaxi);
  setAfrcnvg(afrcnvg);
  setAfacnvg(afacnvg);
  setAfrelax(afrelax);
  setUac2(uac2);
  setPres(Pres);
  setUPres(uPres);
  setAfslae(afslae);
  setAfrseq(afrseq);
  setAflmaxi(aflmaxi);
  setAflcnvg(aflcnvg);
  setAflinit(aflinit);
  setTadj(Tadj);
  setSim_mf(sim_mf);
  setCcmaxi(ccmaxi);
  setCcrcnvg(ccrcnvg);
  setCcacnvg(ccacnvg);
  setCcrelax(ccrelax);
  setUccc(uccc);
  setMfnmthd(mfnmthd);
  setMfnrseq(mfnrseq);
  setMfnmaxi(mfnmaxi);
  setMfnrcnvg(mfnrcnvg);
  setMfnacnvg(mfnacnvg);
  setMfnrelax(mfnrelax);
  setMfngamma(mfngamma);
  setUccn(uccn);
  setMftmthd(mftmthd);
  setMftrseq(mftrseq);
  setMftmaxi(mftmaxi);
  setMftrcnvg(mftrcnvg);
  setMftacnvg(mftacnvg);
  setMftrelax(mftrelax);
  setMftgamma(mftgamma);
  setUcct(ucct);
  setMfvmthd(mfvmthd);
  setMfvrseq(mfvrseq);
  setMfvmaxi(mfvmaxi);
  setMfvrcnvg(mfvrcnvg);
  setMfvacnvg(mfvacnvg);
  setMfvrelax(mfvrelax);
  setUccv(uccv);
  setMf_solver(mf_solver);
  setSim_1dz(sim_1dz);
  setSim_1dd(sim_1dd);
  setCelldx(celldx);
  setSim_vjt(sim_vjt);
  setUdx(udx);
  setCvode_mth(cvode_mth);
  setCvode_rcnvg(cvode_rcnvg);
  setCvode_acnvg(cvode_acnvg);
  setCvode_dtmax(cvode_dtmax);
  setTsdens(tsdens);
  setTsrelax(tsrelax);
  setTsmaxi(tsmaxi);
  setCnvgSS(cnvgSS);
  setDensZP(densZP);
  setStackD(stackD);
  setDodMdt(dodMdt);
  setDate_st(date_st);
  setTime_st(time_st);
  setDate_0(date_0);
  setTime_0(time_0);
  setDate_1(date_1);
  setTime_1(time_1);
  setTime_step(time_step);
  setTime_list(time_list);
  setTime_scrn(time_scrn);
  setRestart(restart);
  setRstdate(rstdate);
  setRsttime(rsttime);
  setList(list);
  setDoDlg(doDlg);
  setPfsave(pfsave);
  setZfsave(zfsave);
  setZcsave(zcsave);
  setAchvol(achvol);
  setAchsave(achsave);
  setAbwsave(abwsave);
  setCbwsave(cbwsave);
  setExpsave(expsave);
  setEbwsave(ebwsave);
  setZaasave(zaasave);
  setZbwsave(zbwsave);
  setRzfsave(rzfsave);
  setRzmsave(rzmsave);
  setRz1save(rz1save);
  setCsmsave(csmsave);
  setSrfsave(srfsave);
  setLogsave(logsave);
  setSave(save);
  setRvals(rvals);
  setBldgFlowZ(BldgFlowZ);
  setBldgFlowD(BldgFlowD);
  setBldgFlowC(BldgFlowC);
  setCfd_ctype(cfd_ctype);
  setCfd_convcpl(cfd_convcpl);
  setCfd_var(cfd_var);
  setCfd_zref(cfd_zref);
  setCfd_imax(cfd_imax);
  setCfd_dtcmo(cfd_dtcmo);
}

RunControlImpl::RunControlImpl(int sim_af,int afcalc,int afmaxi,std::string afrcnvg,std::string afacnvg,
  std::string afrelax,int uac2,std::string Pres,int uPres,int afslae,int afrseq,
  int aflmaxi,std::string aflcnvg,int aflinit,int Tadj,int sim_mf,int ccmaxi,
  std::string ccrcnvg,std::string ccacnvg,std::string ccrelax,int uccc,int mfnmthd,
  int mfnrseq,int mfnmaxi,std::string mfnrcnvg,std::string mfnacnvg,std::string mfnrelax,
  std::string mfngamma,int uccn,int mftmthd,int mftrseq,int mftmaxi,
  std::string mftrcnvg,std::string mftacnvg,std::string mftrelax,std::string mftgamma,
  int ucct,int mfvmthd,int mfvrseq,int mfvmaxi,std::string mfvrcnvg,
  std::string mfvacnvg,std::string mfvrelax,int uccv,int mf_solver,int sim_1dz,
  int sim_1dd,std::string celldx,int sim_vjt,int udx,int cvode_mth,
  std::string cvode_rcnvg,std::string cvode_acnvg,std::string cvode_dtmax,int tsdens,
  std::string tsrelax,int tsmaxi,int cnvgSS,int densZP,int stackD,int dodMdt,
  std::string date_st,std::string time_st,std::string date_0,std::string time_0,std::string date_1,
  std::string time_1,std::string time_step,std::string time_list,std::string time_scrn,
  int restart,std::string rstdate,std::string rsttime,int list,int doDlg,int pfsave,
  int zfsave,int zcsave,int achvol,int achsave,int abwsave,int cbwsave,
  int expsave,int ebwsave,int zaasave,int zbwsave,int rzfsave,int rzmsave,
  int rz1save,int csmsave,int srfsave,int logsave,std::vector<int> save,
  std::vector<std::string> rvals,int BldgFlowZ,int BldgFlowD,int BldgFlowC,
  int cfd_ctype,std::string cfd_convcpl,int cfd_var,int cfd_zref,int cfd_imax,
  int cfd_dtcmo)
{
  setDefaults();
  setSim_af(sim_af);
  setAfcalc(afcalc);
  setAfmaxi(afmaxi);
  setAfrcnvg(afrcnvg);
  setAfacnvg(afacnvg);
  setAfrelax(afrelax);
  setUac2(uac2);
  setPres(Pres);
  setUPres(uPres);
  setAfslae(afslae);
  setAfrseq(afrseq);
  setAflmaxi(aflmaxi);
  setAflcnvg(aflcnvg);
  setAflinit(aflinit);
  setTadj(Tadj);
  setSim_mf(sim_mf);
  setCcmaxi(ccmaxi);
  setCcrcnvg(ccrcnvg);
  setCcacnvg(ccacnvg);
  setCcrelax(ccrelax);
  setUccc(uccc);
  setMfnmthd(mfnmthd);
  setMfnrseq(mfnrseq);
  setMfnmaxi(mfnmaxi);
  setMfnrcnvg(mfnrcnvg);
  setMfnacnvg(mfnacnvg);
  setMfnrelax(mfnrelax);
  setMfngamma(mfngamma);
  setUccn(uccn);
  setMftmthd(mftmthd);
  setMftrseq(mftrseq);
  setMftmaxi(mftmaxi);
  setMftrcnvg(mftrcnvg);
  setMftacnvg(mftacnvg);
  setMftrelax(mftrelax);
  setMftgamma(mftgamma);
  setUcct(ucct);
  setMfvmthd(mfvmthd);
  setMfvrseq(mfvrseq);
  setMfvmaxi(mfvmaxi);
  setMfvrcnvg(mfvrcnvg);
  setMfvacnvg(mfvacnvg);
  setMfvrelax(mfvrelax);
  setUccv(uccv);
  setMf_solver(mf_solver);
  setSim_1dz(sim_1dz);
  setSim_1dd(sim_1dd);
  setCelldx(celldx);
  setSim_vjt(sim_vjt);
  setUdx(udx);
  setCvode_mth(cvode_mth);
  setCvode_rcnvg(cvode_rcnvg);
  setCvode_acnvg(cvode_acnvg);
  setCvode_dtmax(cvode_dtmax);
  setTsdens(tsdens);
  setTsrelax(tsrelax);
  setTsmaxi(tsmaxi);
  setCnvgSS(cnvgSS);
  setDensZP(densZP);
  setStackD(stackD);
  setDodMdt(dodMdt);
  setDate_st(date_st);
  setTime_st(time_st);
  setDate_0(date_0);
  setTime_0(time_0);
  setDate_1(date_1);
  setTime_1(time_1);
  setTime_step(time_step);
  setTime_list(time_list);
  setTime_scrn(time_scrn);
  setRestart(restart);
  setRstdate(rstdate);
  setRsttime(rsttime);
  setList(list);
  setDoDlg(doDlg);
  setPfsave(pfsave);
  setZfsave(zfsave);
  setZcsave(zcsave);
  setAchvol(achvol);
  setAchsave(achsave);
  setAbwsave(abwsave);
  setCbwsave(cbwsave);
  setExpsave(expsave);
  setEbwsave(ebwsave);
  setZaasave(zaasave);
  setZbwsave(zbwsave);
  setRzfsave(rzfsave);
  setRzmsave(rzmsave);
  setRz1save(rz1save);
  setCsmsave(csmsave);
  setSrfsave(srfsave);
  setLogsave(logsave);
  setSave(save);
  setRvals(rvals);
  setBldgFlowZ(BldgFlowZ);
  setBldgFlowD(BldgFlowD);
  setBldgFlowC(BldgFlowC);
  setCfd_ctype(cfd_ctype);
  setCfd_convcpl(cfd_convcpl);
  setCfd_var(cfd_var);
  setCfd_zref(cfd_zref);
  setCfd_imax(cfd_imax);
  setCfd_dtcmo(cfd_dtcmo);
}

void RunControlImpl::read(Reader &input)
{
  setSim_af(input.read<int>());
  setAfcalc(input.read<int>());
  setAfmaxi(input.read<int>());
  setAfrcnvg(input.readNumber<std::string>());
  setAfacnvg(input.readNumber<std::string>());
  setAfrelax(input.readNumber<std::string>());
  setUac2(input.read<int>());
  setPres(input.readNumber<std::string>());
  setUPres(input.read<int>());
  setAfslae(input.read<int>());
  setAfrseq(input.read<int>());
  setAflmaxi(input.read<int>());
  setAflcnvg(input.readNumber<std::string>());
  setAflinit(input.read<int>());
  setTadj(input.read<int>());
  setSim_mf(input.read<int>());
  setCcmaxi(input.read<int>());
  setCcrcnvg(input.readNumber<std::string>());
  setCcacnvg(input.readNumber<std::string>());
  setCcrelax(input.readNumber<std::string>());
  setUccc(input.read<int>());
  setMfnmthd(input.read<int>());
  setMfnrseq(input.read<int>());
  setMfnmaxi(input.read<int>());
  setMfnrcnvg(input.readNumber<std::string>());
  setMfnacnvg(input.readNumber<std::string>());
  setMfnrelax(input.readNumber<std::string>());
  setMfngamma(input.readNumber<std::string>());
  setUccn(input.read<int>());
  setMftmthd(input.read<int>());
  setMftrseq(input.read<int>());
  setMftmaxi(input.read<int>());
  setMftrcnvg(input.readNumber<std::string>());
  setMftacnvg(input.readNumber<std::string>());
  setMftrelax(input.readNumber<std::string>());
  setMftgamma(input.readNumber<std::string>());
  setUcct(input.read<int>());
  setMfvmthd(input.read<int>());
  setMfvrseq(input.read<int>());
  setMfvmaxi(input.read<int>());
  setMfvrcnvg(input.readNumber<std::string>());
  setMfvacnvg(input.readNumber<std::string>());
  setMfvrelax(input.readNumber<std::string>());
  setUccv(input.read<int>());
  setMf_solver(input.read<int>());
  setSim_1dz(input.read<int>());
  setSim_1dd(input.read<int>());
  setCelldx(input.readNumber<std::string>());
  setSim_vjt(input.read<int>());
  setUdx(input.read<int>());
  setCvode_mth(input.read<int>());
  setCvode_rcnvg(input.readNumber<std::string>());
  setCvode_acnvg(input.readNumber<std::string>());
  setCvode_dtmax(input.readNumber<std::string>());
  setTsdens(input.read<int>());
  setTsrelax(input.readNumber<std::string>());
  setTsmaxi(input.read<int>());
  setCnvgSS(input.read<int>());
  setDensZP(input.read<int>());
  setStackD(input.read<int>());
  setDodMdt(input.read<int>());
  setDate_st(input.readString());
  setTime_st(input.readString());
  setDate_0(input.readString());
  setTime_0(input.readString());
  setDate_1(input.readString());
  setTime_1(input.readString());
  setTime_step(input.readString());
  setTime_list(input.readString());
  setTime_scrn(input.readString());
  setRestart(input.read<int>());
  setRstdate(input.readString());
  setRsttime(input.readString());
  setList(input.read<int>());
  setDoDlg(input.read<int>());
  setPfsave(input.read<int>());
  setZfsave(input.read<int>());
  setZcsave(input.read<int>());
  setAchvol(input.read<int>());
  setAchsave(input.read<int>());
  setAbwsave(input.read<int>());
  setCbwsave(input.read<int>());
  setExpsave(input.read<int>());
  setEbwsave(input.read<int>());
  setZaasave(input.read<int>());
  setZbwsave(input.read<int>());
  setRzfsave(input.read<int>());
  setRzmsave(input.read<int>());
  setRz1save(input.read<int>());
  setCsmsave(input.read<int>());
  setSrfsave(input.read<int>());
  setLogsave(input.read<int>());
  std::vector<int> save;
  for(int i=0;i<16;i++)
  {
    save.push_back(input.read<int>());
  }
  setSave(save);
  int nrvals = input.read<int>();
  std::vector<std::string> rvals;
  for(int i=0;i<nrvals;i++)
  {
    rvals.push_back(input.read<std::string>());
  }
  setRvals(rvals);
  setBldgFlowZ(input.read<int>());
  setBldgFlowD(input.read<int>());
  setBldgFlowC(input.read<int>());
  setCfd_ctype(input.read<int>());
  setCfd_convcpl(input.readNumber<std::string>());
  setCfd_var(input.read<int>());
  setCfd_zref(input.read<int>());
  setCfd_imax(input.read<int>());
  setCfd_dtcmo(input.read<int>());
}

std::string RunControlImpl::write()
{
  std::string string;    
  string += ANY_TO_STR(m_sim_af) + ' ' + ANY_TO_STR(m_afcalc) + ' ' + ANY_TO_STR(m_afmaxi) + ' '
    + ANY_TO_STR(m_afrcnvg) + ' ' + ANY_TO_STR(m_afacnvg) + ' ' + ANY_TO_STR(m_afrelax) + ' '
    + ANY_TO_STR(m_uac2) + ' ' + ANY_TO_STR(m_Pres) + ' ' + ANY_TO_STR(m_uPres) + '\n';
  string += ANY_TO_STR(m_afslae) + ' ' + ANY_TO_STR(m_afrseq) + ' ' + ANY_TO_STR(m_aflmaxi) + ' '
    + ANY_TO_STR(m_aflcnvg) + ' ' + ANY_TO_STR(m_aflinit) + ' ' + ANY_TO_STR(m_Tadj) + '\n';
  string += ANY_TO_STR(m_sim_mf) + ' ' + ANY_TO_STR(m_ccmaxi) + ' ' + ANY_TO_STR(m_ccrcnvg) + ' '
    + ANY_TO_STR(m_ccacnvg) + ' ' + ANY_TO_STR(m_ccrelax) + ' ' + ANY_TO_STR(m_uccc) + '\n';
  string += ANY_TO_STR(m_mfnmthd) + ' ' + ANY_TO_STR(m_mfnrseq) + ' ' + ANY_TO_STR(m_mfnmaxi) + ' '
    + ANY_TO_STR(m_mfnrcnvg) + ' ' + ANY_TO_STR(m_mfnacnvg) + ' ' + ANY_TO_STR(m_mfnrelax) + ' '
    + ANY_TO_STR(m_mfngamma) + ' ' + ANY_TO_STR(m_uccn) + '\n';
  string += ANY_TO_STR(m_mftmthd) + ' ' + ANY_TO_STR(m_mftrseq) + ' ' + ANY_TO_STR(m_mftmaxi) + ' '
    + ANY_TO_STR(m_mftrcnvg) + ' ' + ANY_TO_STR(m_mftacnvg) + ' ' + ANY_TO_STR(m_mftrelax) + ' '
    + ANY_TO_STR(m_mftgamma) + ' ' + ANY_TO_STR(m_ucct) + '\n';
  string += ANY_TO_STR(m_mfvmthd) + ' ' + ANY_TO_STR(m_mfvrseq) + ' ' + ANY_TO_STR(m_mfvmaxi) + ' '
    + ANY_TO_STR(m_mfvrcnvg) + ' ' + ANY_TO_STR(m_mfvacnvg) + ' ' + ANY_TO_STR(m_mfvrelax) + ' '
    + ANY_TO_STR(m_uccv) + '\n';
  string += ANY_TO_STR(m_mf_solver) + ' ' + ANY_TO_STR(m_sim_1dz) + ' ' + ANY_TO_STR(m_sim_1dd) + ' '
    + ANY_TO_STR(m_celldx) + ' ' + ANY_TO_STR(m_sim_vjt) + ' ' + ANY_TO_STR(m_udx) + '\n';
  string += ANY_TO_STR(m_cvode_mth) + ' ' + ANY_TO_STR(m_cvode_rcnvg) + ' ' + ANY_TO_STR(m_cvode_acnvg) + ' '
    + ANY_TO_STR(m_cvode_dtmax) + '\n';
  string += ANY_TO_STR(m_tsdens) + ' ' + ANY_TO_STR(m_tsrelax) + ' ' + ANY_TO_STR(m_tsmaxi) + ' '
    + ANY_TO_STR(m_cnvgSS) + ' ' + ANY_TO_STR(m_densZP) + ' ' + ANY_TO_STR(m_stackD) + ' '
    + ANY_TO_STR(m_dodMdt) + '\n';
  string += m_date_st + ' ' + m_time_st + ' ' + m_date_0 + ' ' + m_time_0 + ' ' + m_date_1 + ' '
    + m_time_1 + ' ' + m_time_step + ' ' + m_time_list + ' ' + m_time_scrn + '\n';
  string += ANY_TO_STR(m_restart) + ' ' + m_rstdate + ' ' + m_rsttime + '\n';
  string += ANY_TO_STR(m_list) + ' ' + ANY_TO_STR(m_doDlg) + ' ' + ANY_TO_STR(m_pfsave) + ' '
    + ANY_TO_STR(m_zfsave) + ' ' + ANY_TO_STR(m_zcsave) + '\n';
  string += ANY_TO_STR(m_achvol) + ' ' + ANY_TO_STR(m_achsave) + ' ' + ANY_TO_STR(m_abwsave) + ' '
    + ANY_TO_STR(m_cbwsave) + ' ' + ANY_TO_STR(m_expsave) + ' ' + ANY_TO_STR(m_ebwsave) + ' '
    + ANY_TO_STR(m_zaasave) + ' ' + ANY_TO_STR(m_zbwsave) + '\n';
  string += ANY_TO_STR(m_rzfsave) + ' ' + ANY_TO_STR(m_rzmsave) + ' ' + ANY_TO_STR(m_rz1save) + ' '
    + ANY_TO_STR(m_csmsave) + ' ' + ANY_TO_STR(m_srfsave) + ' ' + ANY_TO_STR(m_logsave) + '\n';
  for(int i=0;i<16;i++)
    string += ANY_TO_STR(m_save[i]) + ' ';
  string += '\n';
  string += ANY_TO_STR(m_rvals.size()) + '\n';
  for(unsigned int i=0;i<m_rvals.size();i++)
    string += ANY_TO_STR(m_rvals[i]) + ' ';
  string += '\n';
  string += ANY_TO_STR(m_BldgFlowZ) + ' ' + ANY_TO_STR(m_BldgFlowD) + ' ' + ANY_TO_STR(m_BldgFlowC) + '\n';
  string += ANY_TO_STR(m_cfd_ctype) + ' ' + ANY_TO_STR(m_cfd_convcpl) + ' ' + ANY_TO_STR(m_cfd_var) + ' '
    + ANY_TO_STR(m_cfd_zref) + ' ' + ANY_TO_STR(m_cfd_imax) + ' ' + ANY_TO_STR(m_cfd_dtcmo) + '\n';
  return string;
}

int RunControlImpl::sim_af() const
{
  return m_sim_af;
}

void RunControlImpl::setSim_af(const int sim_af)
{
  m_sim_af = sim_af;
}

int RunControlImpl::afcalc() const
{
  return m_afcalc;
}

void RunControlImpl::setAfcalc(const int afcalc)
{
  m_afcalc = afcalc;
}

int RunControlImpl::afmaxi() const
{
  return m_afmaxi;
}

void RunControlImpl::setAfmaxi(const int afmaxi)
{
  m_afmaxi = afmaxi;
}

double RunControlImpl::afrcnvg() const
{
  return m_afrcnvg.toDouble();
}

bool RunControlImpl::setAfrcnvg(const double afrcnvg)
{
  m_afrcnvg = QString::number(afrcnvg);
  return true;
}

bool RunControlImpl::setAfrcnvg(const std::string &afrcnvg)
{
  bool ok;
  FLOAT_CHECK(afrcnvg,&ok);
  if(ok)
  {
    m_afrcnvg = STR_TO_FLOAT(afrcnvg);
    return true;
  }
  return false;
}

double RunControlImpl::afacnvg() const
{
  return m_afacnvg.toDouble();
}

bool RunControlImpl::setAfacnvg(const double afacnvg)
{
  m_afacnvg = QString::number(afacnvg);
  return true;
}

bool RunControlImpl::setAfacnvg(const std::string &afacnvg)
{
  bool ok;
  FLOAT_CHECK(afacnvg,&ok);
  if(ok)
  {
    m_afacnvg = STR_TO_FLOAT(afacnvg);
    return true;
  }
  return false;
}

double RunControlImpl::afrelax() const
{
  return m_afrelax.toDouble();
}

bool RunControlImpl::setAfrelax(const double afrelax)
{
  m_afrelax = QString::number(afrelax);
  return true;
}

bool RunControlImpl::setAfrelax(const std::string &afrelax)
{
  bool ok;
  FLOAT_CHECK(afrelax,&ok);
  if(ok)
  {
    m_afrelax = STR_TO_FLOAT(afrelax);
    return true;
  }
  return false;
}

int RunControlImpl::uac2() const
{
  return m_uac2;
}

void RunControlImpl::setUac2(const int uac2)
{
  m_uac2 = uac2;
}

double RunControlImpl::Pres() const
{
  return m_Pres.toDouble();
}

bool RunControlImpl::setPres(const double Pres)
{
  m_Pres = QString::number(Pres);
  return true;
}

bool RunControlImpl::setPres(const std::string &Pres)
{
  bool ok;
  FLOAT_CHECK(Pres,&ok);
  if(ok)
  {
    m_Pres = STR_TO_FLOAT(Pres);
    return true;
  }
  return false;
}

int RunControlImpl::uPres() const
{
  return m_uPres;
}

void RunControlImpl::setUPres(const int uPres)
{
  m_uPres = uPres;
}

int RunControlImpl::afslae() const
{
  return m_afslae;
}

void RunControlImpl::setAfslae(const int afslae)
{
  m_afslae = afslae;
}

int RunControlImpl::afrseq() const
{
  return m_afrseq;
}

void RunControlImpl::setAfrseq(const int afrseq)
{
  m_afrseq = afrseq;
}

int RunControlImpl::aflmaxi() const
{
  return m_aflmaxi;
}

void RunControlImpl::setAflmaxi(const int aflmaxi)
{
  m_aflmaxi = aflmaxi;
}

double RunControlImpl::aflcnvg() const
{
  return m_aflcnvg.toDouble();
}

bool RunControlImpl::setAflcnvg(const double aflcnvg)
{
  m_aflcnvg = QString::number(aflcnvg);
  return true;
}

bool RunControlImpl::setAflcnvg(const std::string &aflcnvg)
{
  bool ok;
  FLOAT_CHECK(aflcnvg,&ok);
  if(ok)
  {
    m_aflcnvg = STR_TO_FLOAT(aflcnvg);
    return true;
  }
  return false;
}

int RunControlImpl::aflinit() const
{
  return m_aflinit;
}

void RunControlImpl::setAflinit(const int aflinit)
{
  m_aflinit = aflinit;
}

int RunControlImpl::Tadj() const
{
  return m_Tadj;
}

void RunControlImpl::setTadj(const int Tadj)
{
  m_Tadj = Tadj;
}

int RunControlImpl::sim_mf() const
{
  return m_sim_mf;
}

void RunControlImpl::setSim_mf(const int sim_mf)
{
  m_sim_mf = sim_mf;
}

int RunControlImpl::ccmaxi() const
{
  return m_ccmaxi;
}

void RunControlImpl::setCcmaxi(const int ccmaxi)
{
  m_ccmaxi = ccmaxi;
}

double RunControlImpl::ccrcnvg() const
{
  return m_ccrcnvg.toDouble();
}

bool RunControlImpl::setCcrcnvg(const double ccrcnvg)
{
  m_ccrcnvg = QString::number(ccrcnvg);
  return true;
}

bool RunControlImpl::setCcrcnvg(const std::string &ccrcnvg)
{
  bool ok;
  FLOAT_CHECK(ccrcnvg,&ok);
  if(ok)
  {
    m_ccrcnvg = STR_TO_FLOAT(ccrcnvg);
    return true;
  }
  return false;
}

double RunControlImpl::ccacnvg() const
{
  return m_ccacnvg.toDouble();
}

bool RunControlImpl::setCcacnvg(const double ccacnvg)
{
  m_ccacnvg = QString::number(ccacnvg);
  return true;
}

bool RunControlImpl::setCcacnvg(const std::string &ccacnvg)
{
  bool ok;
  FLOAT_CHECK(ccacnvg,&ok);
  if(ok)
  {
    m_ccacnvg = STR_TO_FLOAT(ccacnvg);
    return true;
  }
  return false;
}

double RunControlImpl::ccrelax() const
{
  return m_ccrelax.toDouble();
}

bool RunControlImpl::setCcrelax(const double ccrelax)
{
  m_ccrelax = QString::number(ccrelax);
  return true;
}

bool RunControlImpl::setCcrelax(const std::string &ccrelax)
{
  bool ok;
  FLOAT_CHECK(ccrelax,&ok);
  if(ok)
  {
    m_ccrelax = STR_TO_FLOAT(ccrelax);
    return true;
  }
  return false;
}

int RunControlImpl::uccc() const
{
  return m_uccc;
}

void RunControlImpl::setUccc(const int uccc)
{
  m_uccc = uccc;
}

int RunControlImpl::mfnmthd() const
{
  return m_mfnmthd;
}

void RunControlImpl::setMfnmthd(const int mfnmthd)
{
  m_mfnmthd = mfnmthd;
}

int RunControlImpl::mfnrseq() const
{
  return m_mfnrseq;
}

void RunControlImpl::setMfnrseq(const int mfnrseq)
{
  m_mfnrseq = mfnrseq;
}

int RunControlImpl::mfnmaxi() const
{
  return m_mfnmaxi;
}

void RunControlImpl::setMfnmaxi(const int mfnmaxi)
{
  m_mfnmaxi = mfnmaxi;
}

double RunControlImpl::mfnrcnvg() const
{
  return m_mfnrcnvg.toDouble();
}

bool RunControlImpl::setMfnrcnvg(const double mfnrcnvg)
{
  m_mfnrcnvg = QString::number(mfnrcnvg);
  return true;
}

bool RunControlImpl::setMfnrcnvg(const std::string &mfnrcnvg)
{
  bool ok;
  FLOAT_CHECK(mfnrcnvg,&ok);
  if(ok)
  {
    m_mfnrcnvg = STR_TO_FLOAT(mfnrcnvg);
    return true;
  }
  return false;
}

double RunControlImpl::mfnacnvg() const
{
  return m_mfnacnvg.toDouble();
}

bool RunControlImpl::setMfnacnvg(const double mfnacnvg)
{
  m_mfnacnvg = QString::number(mfnacnvg);
  return true;
}

bool RunControlImpl::setMfnacnvg(const std::string &mfnacnvg)
{
  bool ok;
  FLOAT_CHECK(mfnacnvg,&ok);
  if(ok)
  {
    m_mfnacnvg = STR_TO_FLOAT(mfnacnvg);
    return true;
  }
  return false;
}

double RunControlImpl::mfnrelax() const
{
  return m_mfnrelax.toDouble();
}

bool RunControlImpl::setMfnrelax(const double mfnrelax)
{
  m_mfnrelax = QString::number(mfnrelax);
  return true;
}

bool RunControlImpl::setMfnrelax(const std::string &mfnrelax)
{
  bool ok;
  FLOAT_CHECK(mfnrelax,&ok);
  if(ok)
  {
    m_mfnrelax = STR_TO_FLOAT(mfnrelax);
    return true;
  }
  return false;
}

double RunControlImpl::mfngamma() const
{
  return m_mfngamma.toDouble();
}

bool RunControlImpl::setMfngamma(const double mfngamma)
{
  m_mfngamma = QString::number(mfngamma);
  return true;
}

bool RunControlImpl::setMfngamma(const std::string &mfngamma)
{
  bool ok;
  FLOAT_CHECK(mfngamma,&ok);
  if(ok)
  {
    m_mfngamma = STR_TO_FLOAT(mfngamma);
    return true;
  }
  return false;
}

int RunControlImpl::uccn() const
{
  return m_uccn;
}

void RunControlImpl::setUccn(const int uccn)
{
  m_uccn = uccn;
}

int RunControlImpl::mftmthd() const
{
  return m_mftmthd;
}

void RunControlImpl::setMftmthd(const int mftmthd)
{
  m_mftmthd = mftmthd;
}

int RunControlImpl::mftrseq() const
{
  return m_mftrseq;
}

void RunControlImpl::setMftrseq(const int mftrseq)
{
  m_mftrseq = mftrseq;
}

int RunControlImpl::mftmaxi() const
{
  return m_mftmaxi;
}

void RunControlImpl::setMftmaxi(const int mftmaxi)
{
  m_mftmaxi = mftmaxi;
}

double RunControlImpl::mftrcnvg() const
{
  return m_mftrcnvg.toDouble();
}

bool RunControlImpl::setMftrcnvg(const double mftrcnvg)
{
  m_mftrcnvg = QString::number(mftrcnvg);
  return true;
}

bool RunControlImpl::setMftrcnvg(const std::string &mftrcnvg)
{
  bool ok;
  FLOAT_CHECK(mftrcnvg,&ok);
  if(ok)
  {
    m_mftrcnvg = STR_TO_FLOAT(mftrcnvg);
    return true;
  }
  return false;
}

double RunControlImpl::mftacnvg() const
{
  return m_mftacnvg.toDouble();
}

bool RunControlImpl::setMftacnvg(const double mftacnvg)
{
  m_mftacnvg = QString::number(mftacnvg);
  return true;
}

bool RunControlImpl::setMftacnvg(const std::string &mftacnvg)
{
  bool ok;
  FLOAT_CHECK(mftacnvg,&ok);
  if(ok)
  {
    m_mftacnvg = STR_TO_FLOAT(mftacnvg);
    return true;
  }
  return false;
}

double RunControlImpl::mftrelax() const
{
  return m_mftrelax.toDouble();
}

bool RunControlImpl::setMftrelax(const double mftrelax)
{
  m_mftrelax = QString::number(mftrelax);
  return true;
}

bool RunControlImpl::setMftrelax(const std::string &mftrelax)
{
  bool ok;
  FLOAT_CHECK(mftrelax,&ok);
  if(ok)
  {
    m_mftrelax = STR_TO_FLOAT(mftrelax);
    return true;
  }
  return false;
}

double RunControlImpl::mftgamma() const
{
  return m_mftgamma.toDouble();
}

bool RunControlImpl::setMftgamma(const double mftgamma)
{
  m_mftgamma = QString::number(mftgamma);
  return true;
}

bool RunControlImpl::setMftgamma(const std::string &mftgamma)
{
  bool ok;
  FLOAT_CHECK(mftgamma,&ok);
  if(ok)
  {
    m_mftgamma = STR_TO_FLOAT(mftgamma);
    return true;
  }
  return false;
}

int RunControlImpl::ucct() const
{
  return m_ucct;
}

void RunControlImpl::setUcct(const int ucct)
{
  m_ucct = ucct;
}

int RunControlImpl::mfvmthd() const
{
  return m_mfvmthd;
}

void RunControlImpl::setMfvmthd(const int mfvmthd)
{
  m_mfvmthd = mfvmthd;
}

int RunControlImpl::mfvrseq() const
{
  return m_mfvrseq;
}

void RunControlImpl::setMfvrseq(const int mfvrseq)
{
  m_mfvrseq = mfvrseq;
}

int RunControlImpl::mfvmaxi() const
{
  return m_mfvmaxi;
}

void RunControlImpl::setMfvmaxi(const int mfvmaxi)
{
  m_mfvmaxi = mfvmaxi;
}

double RunControlImpl::mfvrcnvg() const
{
  return m_mfvrcnvg.toDouble();
}

bool RunControlImpl::setMfvrcnvg(const double mfvrcnvg)
{
  m_mfvrcnvg = QString::number(mfvrcnvg);
  return true;
}

bool RunControlImpl::setMfvrcnvg(const std::string &mfvrcnvg)
{
  bool ok;
  FLOAT_CHECK(mfvrcnvg,&ok);
  if(ok)
  {
    m_mfvrcnvg = STR_TO_FLOAT(mfvrcnvg);
    return true;
  }
  return false;
}

double RunControlImpl::mfvacnvg() const
{
  return m_mfvacnvg.toDouble();
}

bool RunControlImpl::setMfvacnvg(const double mfvacnvg)
{
  m_mfvacnvg = QString::number(mfvacnvg);
  return true;
}

bool RunControlImpl::setMfvacnvg(const std::string &mfvacnvg)
{
  bool ok;
  FLOAT_CHECK(mfvacnvg,&ok);
  if(ok)
  {
    m_mfvacnvg = STR_TO_FLOAT(mfvacnvg);
    return true;
  }
  return false;
}

double RunControlImpl::mfvrelax() const
{
  return m_mfvrelax.toDouble();
}

bool RunControlImpl::setMfvrelax(const double mfvrelax)
{
  m_mfvrelax = QString::number(mfvrelax);
  return true;
}

bool RunControlImpl::setMfvrelax(const std::string &mfvrelax)
{
  bool ok;
  FLOAT_CHECK(mfvrelax,&ok);
  if(ok)
  {
    m_mfvrelax = STR_TO_FLOAT(mfvrelax);
    return true;
  }
  return false;
}

int RunControlImpl::uccv() const
{
  return m_uccv;
}

void RunControlImpl::setUccv(const int uccv)
{
  m_uccv = uccv;
}

int RunControlImpl::mf_solver() const
{
  return m_mf_solver;
}

void RunControlImpl::setMf_solver(const int mf_solver)
{
  m_mf_solver = mf_solver;
}

int RunControlImpl::sim_1dz() const
{
  return m_sim_1dz;
}

void RunControlImpl::setSim_1dz(const int sim_1dz)
{
  m_sim_1dz = sim_1dz;
}

int RunControlImpl::sim_1dd() const
{
  return m_sim_1dd;
}

void RunControlImpl::setSim_1dd(const int sim_1dd)
{
  m_sim_1dd = sim_1dd;
}

double RunControlImpl::celldx() const
{
  return m_celldx.toDouble();
}

bool RunControlImpl::setCelldx(const double celldx)
{
  m_celldx = QString::number(celldx);
  return true;
}

bool RunControlImpl::setCelldx(const std::string &celldx)
{
  bool ok;
  FLOAT_CHECK(celldx,&ok);
  if(ok)
  {
    m_celldx = STR_TO_FLOAT(celldx);
    return true;
  }
  return false;
}

int RunControlImpl::sim_vjt() const
{
  return m_sim_vjt;
}

void RunControlImpl::setSim_vjt(const int sim_vjt)
{
  m_sim_vjt = sim_vjt;
}

int RunControlImpl::udx() const
{
  return m_udx;
}

void RunControlImpl::setUdx(const int udx)
{
  m_udx = udx;
}

int RunControlImpl::cvode_mth() const
{
  return m_cvode_mth;
}

void RunControlImpl::setCvode_mth(const int cvode_mth)
{
  m_cvode_mth = cvode_mth;
}

double RunControlImpl::cvode_rcnvg() const
{
  return m_cvode_rcnvg.toDouble();
}

bool RunControlImpl::setCvode_rcnvg(const double cvode_rcnvg)
{
  m_cvode_rcnvg = QString::number(cvode_rcnvg);
  return true;
}

bool RunControlImpl::setCvode_rcnvg(const std::string &cvode_rcnvg)
{
  bool ok;
  FLOAT_CHECK(cvode_rcnvg,&ok);
  if(ok)
  {
    m_cvode_rcnvg = STR_TO_FLOAT(cvode_rcnvg);
    return true;
  }
  return false;
}

double RunControlImpl::cvode_acnvg() const
{
  return m_cvode_acnvg.toDouble();
}

bool RunControlImpl::setCvode_acnvg(const double cvode_acnvg)
{
  m_cvode_acnvg = QString::number(cvode_acnvg);
  return true;
}

bool RunControlImpl::setCvode_acnvg(const std::string &cvode_acnvg)
{
  bool ok;
  FLOAT_CHECK(cvode_acnvg,&ok);
  if(ok)
  {
    m_cvode_acnvg = STR_TO_FLOAT(cvode_acnvg);
    return true;
  }
  return false;
}

double RunControlImpl::cvode_dtmax() const
{
  return m_cvode_dtmax.toDouble();
}

bool RunControlImpl::setCvode_dtmax(const double cvode_dtmax)
{
  m_cvode_dtmax = QString::number(cvode_dtmax);
  return true;
}

bool RunControlImpl::setCvode_dtmax(const std::string &cvode_dtmax)
{
  bool ok;
  FLOAT_CHECK(cvode_dtmax,&ok);
  if(ok)
  {
    m_cvode_dtmax = STR_TO_FLOAT(cvode_dtmax);
    return true;
  }
  return false;
}

int RunControlImpl::tsdens() const
{
  return m_tsdens;
}

void RunControlImpl::setTsdens(const int tsdens)
{
  m_tsdens = tsdens;
}

double RunControlImpl::tsrelax() const
{
  return m_tsrelax.toDouble();
}

bool RunControlImpl::setTsrelax(const double tsrelax)
{
  m_tsrelax = QString::number(tsrelax);
  return true;
}

bool RunControlImpl::setTsrelax(const std::string &tsrelax)
{
  bool ok;
  FLOAT_CHECK(tsrelax,&ok);
  if(ok)
  {
    m_tsrelax = STR_TO_FLOAT(tsrelax);
    return true;
  }
  return false;
}

int RunControlImpl::tsmaxi() const
{
  return m_tsmaxi;
}

void RunControlImpl::setTsmaxi(const int tsmaxi)
{
  m_tsmaxi = tsmaxi;
}

int RunControlImpl::cnvgSS() const
{
  return m_cnvgSS;
}

void RunControlImpl::setCnvgSS(const int cnvgSS)
{
  m_cnvgSS = cnvgSS;
}

int RunControlImpl::densZP() const
{
  return m_densZP;
}

void RunControlImpl::setDensZP(const int densZP)
{
  m_densZP = densZP;
}

int RunControlImpl::stackD() const
{
  return m_stackD;
}

void RunControlImpl::setStackD(const int stackD)
{
  m_stackD = stackD;
}

int RunControlImpl::dodMdt() const
{
  return m_dodMdt;
}

void RunControlImpl::setDodMdt(const int dodMdt)
{
  m_dodMdt = dodMdt;
}

std::string RunControlImpl::date_st() const
{
  return m_date_st;
}

void RunControlImpl::setDate_st(const std::string &date_st)
{
  m_date_st = date_st;
}

std::string RunControlImpl::time_st() const
{
  return m_time_st;
}

void RunControlImpl::setTime_st(const std::string &time_st)
{
  m_time_st = time_st;
}

std::string RunControlImpl::date_0() const
{
  return m_date_0;
}

void RunControlImpl::setDate_0(const std::string &date_0)
{
  m_date_0 = date_0;
}

std::string RunControlImpl::time_0() const
{
  return m_time_0;
}

void RunControlImpl::setTime_0(const std::string &time_0)
{
  m_time_0 = time_0;
}

std::string RunControlImpl::date_1() const
{
  return m_date_1;
}

void RunControlImpl::setDate_1(const std::string &date_1)
{
  m_date_1 = date_1;
}

std::string RunControlImpl::time_1() const
{
  return m_time_1;
}

void RunControlImpl::setTime_1(const std::string &time_1)
{
  m_time_1 = time_1;
}

std::string RunControlImpl::time_step() const
{
  return m_time_step;
}

void RunControlImpl::setTime_step(const std::string &time_step)
{
  m_time_step = time_step;
}

std::string RunControlImpl::time_list() const
{
  return m_time_list;
}

void RunControlImpl::setTime_list(const std::string &time_list)
{
  m_time_list = time_list;
}

std::string RunControlImpl::time_scrn() const
{
  return m_time_scrn;
}

void RunControlImpl::setTime_scrn(const std::string &time_scrn)
{
  m_time_scrn = time_scrn;
}

int RunControlImpl::restart() const
{
  return m_restart;
}

void RunControlImpl::setRestart(const int restart)
{
  m_restart = restart;
}

std::string RunControlImpl::rstdate() const
{
  return m_rstdate;
}

void RunControlImpl::setRstdate(const std::string &rstdate)
{
  m_rstdate = rstdate;
}

std::string RunControlImpl::rsttime() const
{
  return m_rsttime;
}

void RunControlImpl::setRsttime(const std::string &rsttime)
{
  m_rsttime = rsttime;
}

int RunControlImpl::list() const
{
  return m_list;
}

void RunControlImpl::setList(const int list)
{
  m_list = list;
}

int RunControlImpl::doDlg() const
{
  return m_doDlg;
}

void RunControlImpl::setDoDlg(const int doDlg)
{
  m_doDlg = doDlg;
}

int RunControlImpl::pfsave() const
{
  return m_pfsave;
}

void RunControlImpl::setPfsave(const int pfsave)
{
  m_pfsave = pfsave;
}

int RunControlImpl::zfsave() const
{
  return m_zfsave;
}

void RunControlImpl::setZfsave(const int zfsave)
{
  m_zfsave = zfsave;
}

int RunControlImpl::zcsave() const
{
  return m_zcsave;
}

void RunControlImpl::setZcsave(const int zcsave)
{
  m_zcsave = zcsave;
}

int RunControlImpl::achvol() const
{
  return m_achvol;
}

void RunControlImpl::setAchvol(const int achvol)
{
  m_achvol = achvol;
}

int RunControlImpl::achsave() const
{
  return m_achsave;
}

void RunControlImpl::setAchsave(const int achsave)
{
  m_achsave = achsave;
}

int RunControlImpl::abwsave() const
{
  return m_abwsave;
}

void RunControlImpl::setAbwsave(const int abwsave)
{
  m_abwsave = abwsave;
}

int RunControlImpl::cbwsave() const
{
  return m_cbwsave;
}

void RunControlImpl::setCbwsave(const int cbwsave)
{
  m_cbwsave = cbwsave;
}

int RunControlImpl::expsave() const
{
  return m_expsave;
}

void RunControlImpl::setExpsave(const int expsave)
{
  m_expsave = expsave;
}

int RunControlImpl::ebwsave() const
{
  return m_ebwsave;
}

void RunControlImpl::setEbwsave(const int ebwsave)
{
  m_ebwsave = ebwsave;
}

int RunControlImpl::zaasave() const
{
  return m_zaasave;
}

void RunControlImpl::setZaasave(const int zaasave)
{
  m_zaasave = zaasave;
}

int RunControlImpl::zbwsave() const
{
  return m_zbwsave;
}

void RunControlImpl::setZbwsave(const int zbwsave)
{
  m_zbwsave = zbwsave;
}

int RunControlImpl::rzfsave() const
{
  return m_rzfsave;
}

void RunControlImpl::setRzfsave(const int rzfsave)
{
  m_rzfsave = rzfsave;
}

int RunControlImpl::rzmsave() const
{
  return m_rzmsave;
}

void RunControlImpl::setRzmsave(const int rzmsave)
{
  m_rzmsave = rzmsave;
}

int RunControlImpl::rz1save() const
{
  return m_rz1save;
}

void RunControlImpl::setRz1save(const int rz1save)
{
  m_rz1save = rz1save;
}

int RunControlImpl::csmsave() const
{
  return m_csmsave;
}

void RunControlImpl::setCsmsave(const int csmsave)
{
  m_csmsave = csmsave;
}

int RunControlImpl::srfsave() const
{
  return m_srfsave;
}

void RunControlImpl::setSrfsave(const int srfsave)
{
  m_srfsave = srfsave;
}

int RunControlImpl::logsave() const
{
  return m_logsave;
}

void RunControlImpl::setLogsave(const int logsave)
{
  m_logsave = logsave;
}

std::vector<int> RunControlImpl::save() const
{
  return m_save;
}

void RunControlImpl::setSave(const std::vector<int> &save)
{
  m_save = save;
}

std::vector<double> RunControlImpl::rvals() const
{
  std::vector<double> out;
  for(unsigned i=0;i<m_rvals.size();i++)
  {
    out.push_back(m_rvals[i].toDouble());
  }
  return out;
}

bool RunControlImpl::setRvals(const std::vector<double> &rvals)
{
  for(unsigned i=0;i<rvals.size();i++)
  {
    m_rvals.push_back(QString::number(rvals[i]));
  }
  return true;
}

bool RunControlImpl::setRvals(const std::vector<std::string> &rvals)
{
  std::vector<PRJFLOAT> in;
  for(unsigned i=0;i<rvals.size();i++)
  {
    bool ok;
    FLOAT_CHECK(rvals[i],&ok);
    if(!ok)
    {
      return false;
    }
    in.push_back(STR_TO_FLOAT(rvals[i]));
  }
  m_rvals = in;
  return true;
}

int RunControlImpl::BldgFlowZ() const
{
  return m_BldgFlowZ;
}

void RunControlImpl::setBldgFlowZ(const int BldgFlowZ)
{
  m_BldgFlowZ = BldgFlowZ;
}

int RunControlImpl::BldgFlowD() const
{
  return m_BldgFlowD;
}

void RunControlImpl::setBldgFlowD(const int BldgFlowD)
{
  m_BldgFlowD = BldgFlowD;
}

int RunControlImpl::BldgFlowC() const
{
  return m_BldgFlowC;
}

void RunControlImpl::setBldgFlowC(const int BldgFlowC)
{
  m_BldgFlowC = BldgFlowC;
}

int RunControlImpl::cfd_ctype() const
{
  return m_cfd_ctype;
}

void RunControlImpl::setCfd_ctype(const int cfd_ctype)
{
  m_cfd_ctype = cfd_ctype;
}

double RunControlImpl::cfd_convcpl() const
{
  return m_cfd_convcpl.toDouble();
}

bool RunControlImpl::setCfd_convcpl(const double cfd_convcpl)
{
  m_cfd_convcpl = QString::number(cfd_convcpl);
  return true;
}

bool RunControlImpl::setCfd_convcpl(const std::string &cfd_convcpl)
{
  bool ok;
  FLOAT_CHECK(cfd_convcpl,&ok);
  if(ok)
  {
    m_cfd_convcpl = STR_TO_FLOAT(cfd_convcpl);
    return true;
  }
  return false;
}

int RunControlImpl::cfd_var() const
{
  return m_cfd_var;
}

void RunControlImpl::setCfd_var(const int cfd_var)
{
  m_cfd_var = cfd_var;
}

int RunControlImpl::cfd_zref() const
{
  return m_cfd_zref;
}

void RunControlImpl::setCfd_zref(const int cfd_zref)
{
  m_cfd_zref = cfd_zref;
}

int RunControlImpl::cfd_imax() const
{
  return m_cfd_imax;
}

void RunControlImpl::setCfd_imax(const int cfd_imax)
{
  m_cfd_imax = cfd_imax;
}

int RunControlImpl::cfd_dtcmo() const
{
  return m_cfd_dtcmo;
}

void RunControlImpl::setCfd_dtcmo(const int cfd_dtcmo)
{
  m_cfd_dtcmo = cfd_dtcmo;
}

void LevelImpl::setDefaults()
{
  m_nr = 0;
  m_refht = PRJFLOAT("0.0");
  m_delht = PRJFLOAT("0.0");
  m_u_rfht = 0;
  m_u_dlht = 0;
}

LevelImpl::LevelImpl()
{
  setDefaults();
}

LevelImpl::LevelImpl(int nr,double refht,double delht,int u_rfht,int u_dlht,std::string name,std::vector<Icon> icons)
{
  setDefaults();
  setNr(nr);
  setRefht(refht);
  setDelht(delht);
  setU_rfht(u_rfht);
  setU_dlht(u_dlht);
  setName(name);
  setIcons(icons);
}

LevelImpl::LevelImpl(int nr,std::string refht,std::string delht,int u_rfht,int u_dlht,std::string name,std::vector<Icon> icons)
{
  setDefaults();
  setNr(nr);
  setRefht(refht);
  setDelht(delht);
  setU_rfht(u_rfht);
  setU_dlht(u_dlht);
  setName(name);
  setIcons(icons);
}

void LevelImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setRefht(input.readNumber<std::string>());
  setDelht(input.readNumber<std::string>());
  int nicon = input.read<int>();
  setU_rfht(input.read<int>());
  setU_dlht(input.read<int>());
  setName(input.readString());
  std::vector<Icon> icons;
  for(int i=0;i<nicon;i++)
  {
    Icon object;
    object.read(input);
    icons.push_back(object);
  }
  setIcons(icons);
}

std::string LevelImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_refht) + ' ' + ANY_TO_STR(m_delht) + ' ' + ANY_TO_STR(m_icons.size())
    + ' ' + ANY_TO_STR(m_u_rfht) + ' ' + ANY_TO_STR(m_u_dlht) + ' ' + ANY_TO_STR(m_name) + '\n';
  for(unsigned i=0;i<m_icons.size();i++)
  {
    string += m_icons[i].write();
  }
  return string;
}

int LevelImpl::nr() const
{
  return m_nr;
}

void LevelImpl::setNr(const int nr)
{
  m_nr = nr;
}

double LevelImpl::refht() const
{
  return m_refht.toDouble();
}

bool LevelImpl::setRefht(const double refht)
{
  m_refht = QString::number(refht);
  return true;
}

bool LevelImpl::setRefht(const std::string &refht)
{
  bool ok;
  FLOAT_CHECK(refht,&ok);
  if(ok)
  {
    m_refht = STR_TO_FLOAT(refht);
    return true;
  }
  return false;
}

double LevelImpl::delht() const
{
  return m_delht.toDouble();
}

bool LevelImpl::setDelht(const double delht)
{
  m_delht = QString::number(delht);
  return true;
}

bool LevelImpl::setDelht(const std::string &delht)
{
  bool ok;
  FLOAT_CHECK(delht,&ok);
  if(ok)
  {
    m_delht = STR_TO_FLOAT(delht);
    return true;
  }
  return false;
}

int LevelImpl::u_rfht() const
{
  return m_u_rfht;
}

void LevelImpl::setU_rfht(const int u_rfht)
{
  m_u_rfht = u_rfht;
}

int LevelImpl::u_dlht() const
{
  return m_u_dlht;
}

void LevelImpl::setU_dlht(const int u_dlht)
{
  m_u_dlht = u_dlht;
}

std::string LevelImpl::name() const
{
  return m_name;
}

void LevelImpl::setName(const std::string &name)
{
  m_name = name;
}

std::vector<Icon> LevelImpl::icons() const
{
  return m_icons;
}

void LevelImpl::setIcons(const std::vector<Icon> &icons)
{
  m_icons = icons;
}

void DayScheduleImpl::setDefaults()
{
  m_nr = 0;
  m_shape = 0;
  m_utyp = 0;
  m_ucnv = 0;
}

DayScheduleImpl::DayScheduleImpl()
{
  setDefaults();
}

DayScheduleImpl::DayScheduleImpl(int nr,int shape,int utyp,int ucnv,std::string name,std::string desc,
  std::vector<SchedulePoint> points)
{
  setDefaults();
  setNr(nr);
  setShape(shape);
  setUtyp(utyp);
  setUcnv(ucnv);
  setName(name);
  setDesc(desc);
  setPoints(points);
}

void DayScheduleImpl::read(Reader &input)
{
  setNr(input.read<int>());
  int npts = input.read<int>();
  setShape(input.read<int>());
  setUtyp(input.read<int>());
  setUcnv(input.read<int>());
  setName(input.readString());
  setDesc(input.readLine());
  std::vector<SchedulePoint> points;
  for(int i=0;i<npts;i++)
  {
    SchedulePoint object;
    object.read(input);
    points.push_back(object);
  }
  setPoints(points);

}

std::string DayScheduleImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_points.size()) + ' ' + ANY_TO_STR(m_shape) + ' ' + ANY_TO_STR(m_utyp) + ' ' + ANY_TO_STR(m_ucnv) + ' ' + m_name + '\n';
  string += m_desc + '\n';
  for(unsigned i=0;i<m_points.size();i++)
  {
    string += m_points[i].write();
  }
  return string;
}

int DayScheduleImpl::nr() const
{
  return m_nr;
}

void DayScheduleImpl::setNr(const int nr)
{
  m_nr = nr;
}

int DayScheduleImpl::shape() const
{
  return m_shape;
}

void DayScheduleImpl::setShape(const int shape)
{
  m_shape = shape;
}

int DayScheduleImpl::utyp() const
{
  return m_utyp;
}

void DayScheduleImpl::setUtyp(const int utyp)
{
  m_utyp = utyp;
}

int DayScheduleImpl::ucnv() const
{
  return m_ucnv;
}

void DayScheduleImpl::setUcnv(const int ucnv)
{
  m_ucnv = ucnv;
}

std::string DayScheduleImpl::name() const
{
  return m_name;
}

void DayScheduleImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string DayScheduleImpl::desc() const
{
  return m_desc;
}

void DayScheduleImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

std::vector<SchedulePoint> DayScheduleImpl::points() const
{
  return m_points;
}

void DayScheduleImpl::setPoints(const std::vector<SchedulePoint> &points)
{
  m_points = points;
}

void WeekScheduleImpl::setDefaults()
{
  m_nr = 0;
  m_utyp = 0;
  m_ucnv = 0;
}

WeekScheduleImpl::WeekScheduleImpl()
{
  setDefaults();
}

WeekScheduleImpl::WeekScheduleImpl(int nr,int utyp,int ucnv,std::string name,std::string desc,std::vector<int> j)
{
  setDefaults();
  setNr(nr);
  setUtyp(utyp);
  setUcnv(ucnv);
  setName(name);
  setDesc(desc);
  setJ(j);
}

void WeekScheduleImpl::read(Reader &input)
{
  setNr(input.read<int>());
  setUtyp(input.read<int>());
  setUcnv(input.read<int>());
  setName(input.readString());
  setDesc(input.readLine());
  std::vector<int> j;
  for(int i=0;i<12;i++)
  {
    j.push_back(input.read<int>());
  }
  setJ(j);
}

std::string WeekScheduleImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_utyp) + ' ' + ANY_TO_STR(m_ucnv) + ' ' + m_name + '\n';
  string += m_desc + '\n';
  for(int i=0;i<12;i++)
  {
    string += ANY_TO_STR(m_j[i]) + ' ';
  }
  string += '\n';
  return string;
}

int WeekScheduleImpl::nr() const
{
  return m_nr;
}

void WeekScheduleImpl::setNr(const int nr)
{
  m_nr = nr;
}

int WeekScheduleImpl::utyp() const
{
  return m_utyp;
}

void WeekScheduleImpl::setUtyp(const int utyp)
{
  m_utyp = utyp;
}

int WeekScheduleImpl::ucnv() const
{
  return m_ucnv;
}

void WeekScheduleImpl::setUcnv(const int ucnv)
{
  m_ucnv = ucnv;
}

std::string WeekScheduleImpl::name() const
{
  return m_name;
}

void WeekScheduleImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string WeekScheduleImpl::desc() const
{
  return m_desc;
}

void WeekScheduleImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

std::vector<int> WeekScheduleImpl::j() const
{
  return m_j;
}

void WeekScheduleImpl::setJ(const std::vector<int> &j)
{
  m_j = j;
}

void WindPressureProfileImpl::setDefaults()
{
  m_nr=0;
  m_type=0;
}

WindPressureProfileImpl::WindPressureProfileImpl()
{
  setDefaults();
}

WindPressureProfileImpl::WindPressureProfileImpl(int nr,int type,std::string name,std::string desc,
  std::vector<PressureCoefficientPoint> coeffs)
{
  setDefaults();
  setNr(nr);
  setType(type);
  setName(name);
  setDesc(desc);
  setCoeffs(coeffs);
}

void WindPressureProfileImpl::read(Reader &input)
{
  setNr(input.read<int>());
  int npts = input.read<int>();
  setType(input.read<int>());
  setName(input.readString());
  setDesc(input.readLine());
  std::vector<PressureCoefficientPoint> coeffs;
  for(int i=0;i<npts;i++)
  {
    PressureCoefficientPoint object;
    object.read(input);
    coeffs.push_back(object);
  }
  setCoeffs(coeffs);
}

std::string WindPressureProfileImpl::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_coeffs.size()) + ' ' + ANY_TO_STR(m_type) + ' ' + m_name + '\n';
  string += m_desc + '\n';
  for(unsigned i=0;i<m_coeffs.size();i++)
  {
    string += m_coeffs[i].write();
  }
  return string;
}

int WindPressureProfileImpl::nr() const
{
  return m_nr;
}

void WindPressureProfileImpl::setNr(const int nr)
{
  m_nr = nr;
}

int WindPressureProfileImpl::type() const
{
  return m_type;
}

void WindPressureProfileImpl::setType(const int type)
{
  m_type = type;
}

std::string WindPressureProfileImpl::name() const
{
  return m_name;
}

void WindPressureProfileImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string WindPressureProfileImpl::desc() const
{
  return m_desc;
}

void WindPressureProfileImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

std::vector<PressureCoefficientPoint> WindPressureProfileImpl::coeffs() const
{
  return m_coeffs;
}

void WindPressureProfileImpl::setCoeffs(const std::vector<PressureCoefficientPoint> &coeffs)
{
  m_coeffs = coeffs;
}

void CdvDatImpl::setDefaults()
{
  m_nr = 0;
  m_seqnr = 0;
  m_flags = 0;
  m_inreq = 0;
  m_n1 = 0;
  m_n2 = 0;
}

CdvDatImpl::CdvDatImpl()
{
  setDefaults();
}

CdvDatImpl::CdvDatImpl(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc)
{
  setDefaults();
  setNr(nr);
  setSeqnr(seqnr);
  setFlags(flags);
  setInreq(inreq);
  setN1(n1);
  setN2(n2);
  setName(name);
  setDesc(desc);
}

CdvDatImpl::CdvDatImpl(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,
  std::string valuename)
{
  setDefaults();
  setNr(nr);
  setSeqnr(seqnr);
  setFlags(flags);
  setInreq(inreq);
  setN1(n1);
  setN2(n2);
  setName(name);
  setDesc(desc);
  setValuename(valuename);
}

void CdvDatImpl::read(Reader &input)
{
  setNr(input.read<int>());
  std::string dataType = input.readString(); // Should really check this
  setSeqnr(input.read<int>());
  setFlags(input.read<unsigned int>());
  setInreq(input.read<int>());
  setN1(input.read<int>());
  setN2(input.read<int>());
  setName(input.readString());
  setDesc(input.readLine());
  setValuename(input.readLine());
}

std::string CdvDatImpl::write(std::string datatype)
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + datatype + '\n';
  string += ANY_TO_STR(m_seqnr) + ' ' + ANY_TO_STR(m_flags) + ' ' + ANY_TO_STR(m_inreq) + ' '
    + ANY_TO_STR(m_n1) + ' ' + ANY_TO_STR(m_n2) + ' ' + m_name + '\n';
  string += m_desc + '\n';
  string += m_valuename + '\n';
  return string;
}

void CdvDatImpl::readDetails(Reader &input)
{
  setValuename(input.readLine());
}

int CdvDatImpl::nr() const
{
  return m_nr;
}

void CdvDatImpl::setNr(const int nr)
{
  m_nr = nr;
}

int CdvDatImpl::seqnr() const
{
  return m_seqnr;
}

void CdvDatImpl::setSeqnr(const int seqnr)
{
  m_seqnr = seqnr;
}

unsigned int CdvDatImpl::flags() const
{
  return m_flags;
}

void CdvDatImpl::setFlags(const unsigned int flags)
{
  m_flags = flags;
}

int CdvDatImpl::inreq() const
{
  return m_inreq;
}

void CdvDatImpl::setInreq(const int inreq)
{
  m_inreq = inreq;
}

int CdvDatImpl::n1() const
{
  return m_n1;
}

void CdvDatImpl::setN1(const int n1)
{
  m_n1 = n1;
}

int CdvDatImpl::n2() const
{
  return m_n2;
}

void CdvDatImpl::setN2(const int n2)
{
  m_n2 = n2;
}

std::string CdvDatImpl::name() const
{
  return m_name;
}

void CdvDatImpl::setName(const std::string &name)
{
  m_name = name;
}

std::string CdvDatImpl::desc() const
{
  return m_desc;
}

void CdvDatImpl::setDesc(const std::string &desc)
{
  m_desc = desc;
}

std::string CdvDatImpl::valuename() const
{
  return m_valuename;
}

void CdvDatImpl::setValuename(const std::string &valuename)
{
  m_valuename = valuename;
}

} // detail
} // contam
} // openstudio
