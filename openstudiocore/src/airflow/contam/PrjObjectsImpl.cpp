/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../../utilities/core/StringHelpers.hpp"

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
  return openstudio::string_conversions::to<double>(m_relHt);
}

bool ZoneImpl::setRelHt(const double relHt)
{
  m_relHt = openstudio::string_conversions::number(relHt);
  return true;
}

bool ZoneImpl::setRelHt(const std::string &relHt)
{
  return assign_if_valid<double>(relHt, m_relHt);
}

double ZoneImpl::Vol() const
{
  return openstudio::string_conversions::to<double>(m_Vol);
}

bool ZoneImpl::setVol(const double Vol)
{
  m_Vol = openstudio::string_conversions::number(Vol);
  return true;
}

bool ZoneImpl::setVol(const std::string &Vol)
{
  return assign_if_valid<double>(Vol, m_Vol);
}

double ZoneImpl::T0() const
{
  return openstudio::string_conversions::to<double>(m_T0);
}

bool ZoneImpl::setT0(const double T0)
{
  m_T0 = openstudio::string_conversions::number(T0);
  return true;
}

bool ZoneImpl::setT0(const std::string &T0)
{
  return assign_if_valid<double>(T0, m_T0);
}

double ZoneImpl::P0() const
{
  return openstudio::string_conversions::to<double>(m_P0);
}

bool ZoneImpl::setP0(const double P0)
{
  m_P0 = openstudio::string_conversions::number(P0);
  return true;
}

bool ZoneImpl::setP0(const std::string &P0)
{
  return assign_if_valid<double>(P0, m_P0);
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

void ZoneImpl::setCfdname(const std::string &cfdname)
{
  m_cfdname = cfdname;
}

double ZoneImpl::X1() const
{
  return openstudio::string_conversions::to<double>(m_X1);
}

bool ZoneImpl::setX1(const double X1)
{
  m_X1 = openstudio::string_conversions::number(X1);
  return true;
}

bool ZoneImpl::setX1(const std::string &X1)
{
  return assign_if_valid<double>(X1, m_X1);
}

double ZoneImpl::Y1() const
{
  return openstudio::string_conversions::to<double>(m_Y1);
}

bool ZoneImpl::setY1(const double Y1)
{
  m_Y1 = openstudio::string_conversions::number(Y1);
  return true;
}

bool ZoneImpl::setY1(const std::string &Y1)
{
  return assign_if_valid<double>(Y1, m_Y1);
}

double ZoneImpl::H1() const
{
  return openstudio::string_conversions::to<double>(m_H1);
}

bool ZoneImpl::setH1(const double H1)
{
  m_H1 = openstudio::string_conversions::number(H1);
  return true;
}

bool ZoneImpl::setH1(const std::string &H1)
{
  return assign_if_valid<double>(H1, m_H1);
}

double ZoneImpl::X2() const
{
  return openstudio::string_conversions::to<double>(m_X2);
}

bool ZoneImpl::setX2(const double X2)
{
  m_X2 = openstudio::string_conversions::number(X2);
  return true;
}

bool ZoneImpl::setX2(const std::string &X2)
{
  return assign_if_valid<double>(X2, m_X2);
}

double ZoneImpl::Y2() const
{
  return openstudio::string_conversions::to<double>(m_Y2);
}

bool ZoneImpl::setY2(const double Y2)
{
  m_Y2 = openstudio::string_conversions::number(Y2);
  return true;
}

bool ZoneImpl::setY2(const std::string &Y2)
{
  return assign_if_valid<double>(Y2, m_Y2);
}

double ZoneImpl::H2() const
{
  return openstudio::string_conversions::to<double>(m_H2);
}

bool ZoneImpl::setH2(const double H2)
{
  m_H2 = openstudio::string_conversions::number(H2);
  return true;
}

bool ZoneImpl::setH2(const std::string &H2)
{
  return assign_if_valid<double>(H2, m_H2);
}

double ZoneImpl::celldx() const
{
  return openstudio::string_conversions::to<double>(m_celldx);
}

bool ZoneImpl::setCelldx(const double celldx)
{
  m_celldx = openstudio::string_conversions::number(celldx);
  return true;
}

bool ZoneImpl::setCelldx(const std::string &celldx)
{
  return assign_if_valid<double>(celldx, m_celldx);
}

double ZoneImpl::axialD() const
{
  return openstudio::string_conversions::to<double>(m_axialD);
}

bool ZoneImpl::setAxialD(const double axialD)
{
  m_axialD = openstudio::string_conversions::number(axialD);
  return true;
}

bool ZoneImpl::setAxialD(const std::string &axialD)
{
  return assign_if_valid<double>(axialD, m_axialD);
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
    m_flags |= ZoneFlags::VAR_P;
  else
    m_flags &= ZoneFlags::NVAR_P;
}

bool ZoneImpl::variablePressure() const
{
  return m_flags & ZoneFlags::VAR_P;
}

void ZoneImpl::setVariableContaminants(bool b)
{
  if(b)
    m_flags |= ZoneFlags::VAR_C;
  else
    m_flags &= ZoneFlags::NVAR_C;
}

bool ZoneImpl::variableContaminants() const
{
  return m_flags & ZoneFlags::VAR_C;
}

void ZoneImpl::setSystem(bool b)
{
  if(b)
    m_flags |= ZoneFlags::SYS_N;
  else
    m_flags &= ZoneFlags::NSYS_N;
}

bool ZoneImpl::system() const
{
  return m_flags & ZoneFlags::SYS_N;
}

double ZoneImpl::ic(const int i) const
{
  return openstudio::string_conversions::to<double>(m_ic[i]);
}

std::vector<double> ZoneImpl::ic() const
{
  std::vector<double> out;
  for(std::size_t i=0;i<m_ic.size();i++)
  {
    out.push_back(openstudio::string_conversions::to<double>(m_ic[i]));
  }
  return out;
}

bool ZoneImpl::setIc(const int i, const double value)
{
  m_ic[i] = openstudio::string_conversions::number(value);
  return true;
}

bool ZoneImpl::setIc(const int i, const std::string &value)
{
  return assign_if_valid<double>(value, m_ic[i]);
}

bool ZoneImpl::setIc(const std::vector<double> &ic)
{
  std::vector<std::string> in;
  for(const auto &input : ic) {
    in.push_back(openstudio::string_conversions::number(input));
  }
  return true;
}

bool ZoneImpl::setIc(const std::vector<std::string> &ic)
{
  for(const auto &input : ic) {
    if (!is_valid<double>(input)) {
      return false;
    }
  }
  m_ic = ic;
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

bool SpeciesImpl::sflag() const
{
  return m_sflag == 1;
}

void SpeciesImpl::setSflag(const bool sflag)
{
  m_sflag = sflag?1:0;
}

bool SpeciesImpl::ntflag() const
{
  return m_ntflag == 1;
}

void SpeciesImpl::setNtflag(const bool ntflag)
{
  m_ntflag = (int)ntflag;
}

double SpeciesImpl::molwt() const
{
  return openstudio::string_conversions::to<double>(m_molwt);
}

bool SpeciesImpl::setMolwt(const double molwt)
{
  m_molwt = openstudio::string_conversions::number(molwt);
  return true;
}

bool SpeciesImpl::setMolwt(const std::string &molwt)
{
  return assign_if_valid<double>(molwt, m_molwt);
}

double SpeciesImpl::mdiam() const
{
  return openstudio::string_conversions::to<double>(m_mdiam);
}

bool SpeciesImpl::setMdiam(const double mdiam)
{
  m_mdiam = openstudio::string_conversions::number(mdiam);
  return true;
}

bool SpeciesImpl::setMdiam(const std::string &mdiam)
{
  return assign_if_valid<double>(mdiam, m_mdiam);
}

double SpeciesImpl::edens() const
{
  return openstudio::string_conversions::to<double>(m_edens);
}

bool SpeciesImpl::setEdens(const double edens)
{
  m_edens = openstudio::string_conversions::number(edens);
  return true;
}

bool SpeciesImpl::setEdens(const std::string &edens)
{
  return assign_if_valid<double>(edens, m_edens);
}

double SpeciesImpl::decay() const
{
  return openstudio::string_conversions::to<double>(m_decay);
}

bool SpeciesImpl::setDecay(const double decay)
{
  m_decay = decay;
  return true;
}

bool SpeciesImpl::setDecay(const std::string &decay)
{
  return assign_if_valid<double>(decay, m_decay);
}

double SpeciesImpl::Dm() const
{
  return openstudio::string_conversions::to<double>(m_Dm);
}

bool SpeciesImpl::setDm(const double Dm)
{
  m_Dm = openstudio::string_conversions::number(Dm);
  return true;
}

bool SpeciesImpl::setDm(const std::string &Dm)
{
  return assign_if_valid<double>(Dm, m_Dm);
}

double SpeciesImpl::ccdef() const
{
  return openstudio::string_conversions::to<double>(m_ccdef);
}

bool SpeciesImpl::setCcdef(const double ccdef)
{
  m_ccdef = openstudio::string_conversions::number(ccdef);
  return true;
}

bool SpeciesImpl::setCcdef(const std::string &ccdef)
{
  return assign_if_valid<double>(ccdef, m_ccdef);
}

double SpeciesImpl::Cp() const
{
  return openstudio::string_conversions::to<double>(m_Cp);
}

bool SpeciesImpl::setCp(const double Cp)
{
  m_Cp = openstudio::string_conversions::number(Cp);
  return true;
}

bool SpeciesImpl::setCp(const std::string &Cp)
{
  return assign_if_valid<double>(Cp, m_Cp);
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
  return openstudio::string_conversions::to<double>(m_X);
}

bool AirflowPathImpl::setX(const double X)
{
  m_X = openstudio::string_conversions::number(X);
  return true;
}

bool AirflowPathImpl::setX(const std::string &X)
{
  return assign_if_valid<double>(X, m_X);
}

double AirflowPathImpl::Y() const
{
  return openstudio::string_conversions::to<double>(m_Y);
}

bool AirflowPathImpl::setY(const double Y)
{
  m_Y = openstudio::string_conversions::number(Y);
  return true;
}

bool AirflowPathImpl::setY(const std::string &Y)
{
  return assign_if_valid<double>(Y, m_Y);
}

double AirflowPathImpl::relHt() const
{
  return openstudio::string_conversions::to<double>(m_relHt);
}

bool AirflowPathImpl::setRelHt(const double relHt)
{
  m_relHt = openstudio::string_conversions::number(relHt);
  return true;
}

bool AirflowPathImpl::setRelHt(const std::string &relHt)
{
  return assign_if_valid<double>(relHt, m_relHt);
}

double AirflowPathImpl::mult() const
{
  return openstudio::string_conversions::to<double>(m_mult);
}

bool AirflowPathImpl::setMult(const double mult)
{
  m_mult = openstudio::string_conversions::number(mult);
  return true;
}

bool AirflowPathImpl::setMult(const std::string &mult)
{
  return assign_if_valid<double>(mult, m_mult);
}

double AirflowPathImpl::wPset() const
{
  return openstudio::string_conversions::to<double>(m_wPset);
}

bool AirflowPathImpl::setWPset(const double wPset)
{
  m_wPset = openstudio::string_conversions::number(wPset);
  return true;
}

bool AirflowPathImpl::setWPset(const std::string &wPset)
{
  return assign_if_valid<double>(wPset, m_wPset);
}

double AirflowPathImpl::wPmod() const
{
  return openstudio::string_conversions::to<double>(m_wPmod);
}

bool AirflowPathImpl::setWPmod(const double wPmod)
{
  m_wPmod = openstudio::string_conversions::number(wPmod);
  return true;
}

bool AirflowPathImpl::setWPmod(const std::string &wPmod)
{
  return assign_if_valid<double>(wPmod, m_wPmod);
}

double AirflowPathImpl::wazm() const
{
  return openstudio::string_conversions::to<double>(m_wazm);
}

bool AirflowPathImpl::setWazm(const double wazm)
{
  m_wazm = openstudio::string_conversions::number(wazm);
  return true;
}

bool AirflowPathImpl::setWazm(const std::string &wazm)
{
  return assign_if_valid<double>(wazm, m_wazm);
}

double AirflowPathImpl::Fahs() const
{
  return openstudio::string_conversions::to<double>(m_Fahs);
}

bool AirflowPathImpl::setFahs(const double Fahs)
{
  m_Fahs = openstudio::string_conversions::number(Fahs);
  return true;
}

bool AirflowPathImpl::setFahs(const std::string &Fahs)
{
  return assign_if_valid<double>(Fahs, m_Fahs);
}

double AirflowPathImpl::Xmax() const
{
  return openstudio::string_conversions::to<double>(m_Xmax);
}

bool AirflowPathImpl::setXmax(const double Xmax)
{
  m_Xmax = openstudio::string_conversions::number(Xmax);
  return true;
}

bool AirflowPathImpl::setXmax(const std::string &Xmax)
{
  return assign_if_valid<double>(Xmax, m_Xmax);
}

double AirflowPathImpl::Xmin() const
{
  return openstudio::string_conversions::to<double>(m_Xmin);
}

bool AirflowPathImpl::setXmin(const double Xmin)
{
  m_Xmin = openstudio::string_conversions::number(Xmin);
  return true;
}

bool AirflowPathImpl::setXmin(const std::string &Xmin)
{
  return assign_if_valid<double>(Xmin, m_Xmin);
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
    m_flags |= PathFlags::WIND;
  else
    m_flags &= 0xFFFE;
}

bool AirflowPathImpl::windPressure()
{
  return m_flags & PathFlags::WIND;
}

void AirflowPathImpl::setSystem(bool b)
{
  if(b)
    m_flags |= PathFlags::AHS_S;
  else
    m_flags &= 0xFFF7;
}

bool AirflowPathImpl::system()
{
  return m_flags & PathFlags::AHS_S;
}

void AirflowPathImpl::setExhaust(bool b)
{
  if(b)
    m_flags |= PathFlags::AHS_X;
  else
    m_flags &= 0xFFBF;
}

bool AirflowPathImpl::exhaust()
{
  return m_flags & PathFlags::AHS_X;
}


void AirflowPathImpl::setRecirculation(bool b)
{
  if(b)
    m_flags |= PathFlags::AHS_R;
  else
    m_flags &= 0xFFEF;
}

bool AirflowPathImpl::recirculation()
{
  return m_flags & PathFlags::AHS_R;
}

void AirflowPathImpl::setOutsideAir(bool b)
{
  if(b)
    m_flags |= PathFlags::AHS_O;
  else
    m_flags &= 0xFFDF;
}

bool AirflowPathImpl::outsideAir()
{
  return m_flags & PathFlags::AHS_O;
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
  return openstudio::string_conversions::to<double>(m_afrcnvg);
}

bool RunControlImpl::setAfrcnvg(const double afrcnvg)
{
  m_afrcnvg = openstudio::string_conversions::number(afrcnvg);
  return true;
}

bool RunControlImpl::setAfrcnvg(const std::string &afrcnvg)
{
  return assign_if_valid<double>(afrcnvg, m_afrcnvg);
}

double RunControlImpl::afacnvg() const
{
  return openstudio::string_conversions::to<double>(m_afacnvg);
}

bool RunControlImpl::setAfacnvg(const double afacnvg)
{
  m_afacnvg = openstudio::string_conversions::number(afacnvg);
  return true;
}

bool RunControlImpl::setAfacnvg(const std::string &afacnvg)
{
  return assign_if_valid<double>(afacnvg, m_afacnvg);
}

double RunControlImpl::afrelax() const
{
  return openstudio::string_conversions::to<double>(m_afrelax);
}

bool RunControlImpl::setAfrelax(const double afrelax)
{
  m_afrelax = openstudio::string_conversions::number(afrelax);
  return true;
}

bool RunControlImpl::setAfrelax(const std::string &afrelax)
{
  return assign_if_valid<double>(afrelax, m_afrelax);
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
  return openstudio::string_conversions::to<double>(m_Pres);
}

bool RunControlImpl::setPres(const double Pres)
{
  m_Pres = openstudio::string_conversions::number(Pres);
  return true;
}

bool RunControlImpl::setPres(const std::string &Pres)
{
  return assign_if_valid<double>(Pres, m_Pres);
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
  return openstudio::string_conversions::to<double>(m_aflcnvg);
}

bool RunControlImpl::setAflcnvg(const double aflcnvg)
{
  m_aflcnvg = openstudio::string_conversions::number(aflcnvg);
  return true;
}

bool RunControlImpl::setAflcnvg(const std::string &aflcnvg)
{
  return assign_if_valid<double>(aflcnvg, m_aflcnvg);
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
  return openstudio::string_conversions::to<double>(m_ccrcnvg);
}

bool RunControlImpl::setCcrcnvg(const double ccrcnvg)
{
  m_ccrcnvg = openstudio::string_conversions::number(ccrcnvg);
  return true;
}

bool RunControlImpl::setCcrcnvg(const std::string &ccrcnvg)
{
  return assign_if_valid<double>(ccrcnvg, m_ccrcnvg);
}

double RunControlImpl::ccacnvg() const
{
  return openstudio::string_conversions::to<double>(m_ccacnvg);
}

bool RunControlImpl::setCcacnvg(const double ccacnvg)
{
  m_ccacnvg = openstudio::string_conversions::number(ccacnvg);
  return true;
}

bool RunControlImpl::setCcacnvg(const std::string &ccacnvg)
{
  return assign_if_valid<double>(ccacnvg, m_ccacnvg);
}

double RunControlImpl::ccrelax() const
{
  return openstudio::string_conversions::to<double>(m_ccrelax);
}

bool RunControlImpl::setCcrelax(const double ccrelax)
{
  m_ccrelax = openstudio::string_conversions::number(ccrelax);
  return true;
}

bool RunControlImpl::setCcrelax(const std::string &ccrelax)
{
  return assign_if_valid<double>(ccrelax, m_ccrelax);
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
  return openstudio::string_conversions::to<double>(m_mfnrcnvg);
}

bool RunControlImpl::setMfnrcnvg(const double mfnrcnvg)
{
  m_mfnrcnvg = openstudio::string_conversions::number(mfnrcnvg);
  return true;
}

bool RunControlImpl::setMfnrcnvg(const std::string &mfnrcnvg)
{
  return assign_if_valid<double>(mfnrcnvg, m_mfnrcnvg);
}

double RunControlImpl::mfnacnvg() const
{
  return openstudio::string_conversions::to<double>(m_mfnacnvg);
}

bool RunControlImpl::setMfnacnvg(const double mfnacnvg)
{
  m_mfnacnvg = openstudio::string_conversions::number(mfnacnvg);
  return true;
}

bool RunControlImpl::setMfnacnvg(const std::string &mfnacnvg)
{
  return assign_if_valid<double>(mfnacnvg, m_mfnacnvg);
}

double RunControlImpl::mfnrelax() const
{
  return openstudio::string_conversions::to<double>(m_mfnrelax);
}

bool RunControlImpl::setMfnrelax(const double mfnrelax)
{
  m_mfnrelax = openstudio::string_conversions::number(mfnrelax);
  return true;
}

bool RunControlImpl::setMfnrelax(const std::string &mfnrelax)
{
  return assign_if_valid<double>(mfnrelax, m_mfnrelax);
}

double RunControlImpl::mfngamma() const
{
  return openstudio::string_conversions::to<double>(m_mfngamma);
}

bool RunControlImpl::setMfngamma(const double mfngamma)
{
  m_mfngamma = openstudio::string_conversions::number(mfngamma);
  return true;
}

bool RunControlImpl::setMfngamma(const std::string &mfngamma)
{
  return assign_if_valid<double>(mfngamma, m_mfngamma);
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
  return openstudio::string_conversions::to<double>(m_mftrcnvg);
}

bool RunControlImpl::setMftrcnvg(const double mftrcnvg)
{
  m_mftrcnvg = openstudio::string_conversions::number(mftrcnvg);
  return true;
}

bool RunControlImpl::setMftrcnvg(const std::string &mftrcnvg)
{
  return assign_if_valid<double>(mftrcnvg, m_mftrcnvg);
}

double RunControlImpl::mftacnvg() const
{
  return openstudio::string_conversions::to<double>(m_mftacnvg);
}

bool RunControlImpl::setMftacnvg(const double mftacnvg)
{
  m_mftacnvg = openstudio::string_conversions::number(mftacnvg);
  return true;
}

bool RunControlImpl::setMftacnvg(const std::string &mftacnvg)
{
  return assign_if_valid<double>(mftacnvg, m_mftacnvg);
}

double RunControlImpl::mftrelax() const
{
  return openstudio::string_conversions::to<double>(m_mftrelax);
}

bool RunControlImpl::setMftrelax(const double mftrelax)
{
  m_mftrelax = openstudio::string_conversions::number(mftrelax);
  return true;
}

bool RunControlImpl::setMftrelax(const std::string &mftrelax)
{
  return assign_if_valid<double>(mftrelax, m_mftrelax);
}

double RunControlImpl::mftgamma() const
{
  return openstudio::string_conversions::to<double>(m_mftgamma);
}

bool RunControlImpl::setMftgamma(const double mftgamma)
{
  m_mftgamma = openstudio::string_conversions::number(mftgamma);
  return true;
}

bool RunControlImpl::setMftgamma(const std::string &mftgamma)
{
  return assign_if_valid<double>(mftgamma, m_mftgamma);
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
  return openstudio::string_conversions::to<double>(m_mfvrcnvg);
}

bool RunControlImpl::setMfvrcnvg(const double mfvrcnvg)
{
  m_mfvrcnvg = openstudio::string_conversions::number(mfvrcnvg);
  return true;
}

bool RunControlImpl::setMfvrcnvg(const std::string &mfvrcnvg)
{
  return assign_if_valid<double>(mfvrcnvg, m_mfvrcnvg);
}

double RunControlImpl::mfvacnvg() const
{
  return openstudio::string_conversions::to<double>(m_mfvacnvg);
}

bool RunControlImpl::setMfvacnvg(const double mfvacnvg)
{
  m_mfvacnvg = openstudio::string_conversions::number(mfvacnvg);
  return true;
}

bool RunControlImpl::setMfvacnvg(const std::string &mfvacnvg)
{
  return assign_if_valid<double>(mfvacnvg, m_mfvacnvg);
}

double RunControlImpl::mfvrelax() const
{
  return openstudio::string_conversions::to<double>(m_mfvrelax);
}

bool RunControlImpl::setMfvrelax(const double mfvrelax)
{
  m_mfvrelax = openstudio::string_conversions::number(mfvrelax);
  return true;
}

bool RunControlImpl::setMfvrelax(const std::string &mfvrelax)
{
  return assign_if_valid<double>(mfvrelax, m_mfvrelax);
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
  return openstudio::string_conversions::to<double>(m_celldx);
}

bool RunControlImpl::setCelldx(const double celldx)
{
  m_celldx = openstudio::string_conversions::number(celldx);
  return true;
}

bool RunControlImpl::setCelldx(const std::string &celldx)
{
  return assign_if_valid<double>(celldx, m_celldx);
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
  return openstudio::string_conversions::to<double>(m_cvode_rcnvg);
}

bool RunControlImpl::setCvode_rcnvg(const double cvode_rcnvg)
{
  m_cvode_rcnvg = openstudio::string_conversions::number(cvode_rcnvg);
  return true;
}

bool RunControlImpl::setCvode_rcnvg(const std::string &cvode_rcnvg)
{
  return assign_if_valid<double>(cvode_rcnvg, m_cvode_rcnvg);
}

double RunControlImpl::cvode_acnvg() const
{
  return openstudio::string_conversions::to<double>(m_cvode_acnvg);
}

bool RunControlImpl::setCvode_acnvg(const double cvode_acnvg)
{
  m_cvode_acnvg = openstudio::string_conversions::number(cvode_acnvg);
  return true;
}

bool RunControlImpl::setCvode_acnvg(const std::string &cvode_acnvg)
{
  return assign_if_valid<double>(cvode_acnvg, m_cvode_acnvg);
}

double RunControlImpl::cvode_dtmax() const
{
  return openstudio::string_conversions::to<double>(m_cvode_dtmax);
}

bool RunControlImpl::setCvode_dtmax(const double cvode_dtmax)
{
  m_cvode_dtmax = openstudio::string_conversions::number(cvode_dtmax);
  return true;
}

bool RunControlImpl::setCvode_dtmax(const std::string &cvode_dtmax)
{
  return assign_if_valid<double>(cvode_dtmax, m_cvode_dtmax);
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
  return openstudio::string_conversions::to<double>(m_tsrelax);
}

bool RunControlImpl::setTsrelax(const double tsrelax)
{
  m_tsrelax = openstudio::string_conversions::number(tsrelax);
  return true;
}

bool RunControlImpl::setTsrelax(const std::string &tsrelax)
{
  return assign_if_valid<double>(tsrelax, m_tsrelax);
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
  for(const auto &val : m_rvals)
  {
    out.push_back(openstudio::string_conversions::to<double>(val));
  }
  return out;
}

bool RunControlImpl::setRvals(const std::vector<double> &rvals)
{
  std::vector<std::string> new_vals;
  for(const auto &val : rvals)
  {
    new_vals.push_back(openstudio::string_conversions::number(val));
  }
  m_rvals = new_vals;
  return true;
}

bool RunControlImpl::setRvals(const std::vector<std::string> &rvals)
{
  if (std::all_of(rvals.begin(), rvals.end(), is_valid<double, std::string>)) {
    m_rvals = rvals;
    return true;
  } else {
    return false;
  }
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
  return openstudio::string_conversions::to<double>(m_cfd_convcpl);
}

bool RunControlImpl::setCfd_convcpl(const double cfd_convcpl)
{
  m_cfd_convcpl = openstudio::string_conversions::number(cfd_convcpl);
  return true;
}

bool RunControlImpl::setCfd_convcpl(const std::string &cfd_convcpl)
{
  return assign_if_valid<double>(cfd_convcpl, m_cfd_convcpl);
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
  return openstudio::string_conversions::to<double>(m_refht);
}

bool LevelImpl::setRefht(const double refht)
{
  m_refht = openstudio::string_conversions::number(refht);
  return true;
}

bool LevelImpl::setRefht(const std::string &refht)
{
  return assign_if_valid<double>(refht, m_refht);
}

double LevelImpl::delht() const
{
  return openstudio::string_conversions::to<double>(m_delht);
}

bool LevelImpl::setDelht(const double delht)
{
  m_delht = openstudio::string_conversions::number(delht);
  return true;
}

bool LevelImpl::setDelht(const std::string &delht)
{
  return assign_if_valid<double>(delht, m_delht);
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
