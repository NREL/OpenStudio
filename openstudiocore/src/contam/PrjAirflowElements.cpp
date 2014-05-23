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
#include "PrjAirflowElements.hpp"
#include "PrjAirflowElementsImpl.hpp"

#include <utilities/core/Logger.hpp>

namespace openstudio {
namespace contam {

AirflowElement::Type AirflowElement::convertTag(std::string string)
{
  std::string tags[28] = {std::string("plr_orfc"),std::string("plr_leak1"),
    std::string("plr_leak2"),std::string("plr_leak3"),std::string("plr_conn"),
    std::string("plr_qcn"),std::string("plr_fcn"),std::string("plr_test1"),
    std::string("plr_test2"),std::string("plr_crack"),std::string("plr_stair"),
    std::string("plr_shaft"),std::string("plr_bdq"),std::string("plr_bdf"),
    std::string("qfr_qab"),std::string("qfr_fab"),std::string("qfr_crack"),
    std::string("qfr_test2"),std::string("dor_door"),std::string("dor_pl2"),
    std::string("fan_cmf"),std::string("fan_cvf"),std::string("fan_fan"),
    std::string("csf_fsp"),std::string("csf_qsp"),std::string("csf_psf"),
    std::string("csf_psq"),std::string("sup_afe")};
  AirflowElement::Type type[28] = {PL_ORFC,PL_LEAK1,PL_LEAK2,PL_LEAK3,PL_CONN,PL_QCN,PL_FCN,
    PL_TEST1,PL_TEST2,PL_CRACK,PL_STAIR,PL_SHAFT,PL_BDQ,PL_BDF,QFR_QAB,QFR_QAF,QFR_CRACK,
    QFR_TEST2,DR_DOOR,DR_PL2,FN_CMF,FN_CVF,FN_FAN,CS_FSP,CS_QSP,CS_PSF,CS_PSQ,AF_SUP};
  for(int i=0;i<28;i++)
    if(string==tags[i])
      return type[i];
  return AirflowElement::UNKNOWN;
}

AirflowElement* AirflowElement::readElement(Reader &input)
{
  AirflowElement *out=NULL;
  int nr = input.read<int>();
  int icon = input.read<int>();
  std::string dataType = input.readString();
  std::string name = input.readString();
  std::string desc = input.readLine();
  int kind = convertTag(dataType);
  switch(kind)
  {
  case AirflowElement::PL_ORFC:
    {
      PlrOrf *obj = new PlrOrf(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_LEAK1:
    {
      PlrLeak1 *obj = new PlrLeak1(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_LEAK2:
    {
      PlrLeak2 *obj = new PlrLeak2(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_LEAK3:
    {
      PlrLeak3 *obj = new PlrLeak3(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_CONN:
    {
      PlrConn *obj = new PlrConn(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_QCN:
    {
      PlrQcn *obj = new PlrQcn(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_FCN:
    {
      PlrFcn *obj = new PlrFcn(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_TEST1:
    {
      PlrTest1 *obj = new PlrTest1(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_TEST2:
    {
      PlrTest2 *obj = new PlrTest2(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_CRACK:
    {
      PlrCrack *obj = new PlrCrack(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_STAIR:
    {
      PlrStair *obj = new PlrStair(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_SHAFT:
    {
      PlrShaft *obj = new PlrShaft(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_BDQ:
    {
      PlrBdq *obj = new PlrBdq(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::PL_BDF:
    {
      PlrBdf *obj = new PlrBdf(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::QFR_QAB:
    {
      QfrQab *obj = new QfrQab(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::QFR_QAF:
    {
      QfrFab *obj = new QfrFab(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::QFR_CRACK:
    {
      QfrCrack *obj = new QfrCrack(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::QFR_TEST2:
    {
      QfrTest2 *obj = new QfrTest2(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::DR_DOOR:
    {
      AfeDor *obj = new AfeDor(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::DR_PL2:
    {
      DrPl2 *obj = new DrPl2(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::FN_CMF:
    {
      AfeCmf *obj = new AfeCmf(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::FN_CVF:
    {
      AfeCvf *obj = new AfeCvf(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::FN_FAN:
    {
      AfeFan *obj = new AfeFan(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::CS_FSP:
    {
      AfeFsp *obj = new AfeFsp(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::CS_QSP:
    {
      AfeQsp *obj = new AfeQsp(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::CS_PSF:
    {
      AfePsf *obj = new AfePsf(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::CS_PSQ:
    {
      AfePsq *obj = new AfePsq(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::AF_SUP:
    {
      AfeSup *obj = new AfeSup(nr,icon,name,desc);
      obj->readDetails(input);
      out = static_cast<AirflowElement*>(obj);
      break;
    }
  case AirflowElement::UNKNOWN:
  default:
    std::string mesg = "Unknown airflow element type '" + dataType + "' at line " + openstudio::toString(input.lineNumber());
    LOG_FREE_AND_THROW("openstudio.contam.Reader",mesg);
  }
  return out;
}

PlrOrf::PlrOrf()
{
  m_impl = boost::shared_ptr<detail::PlrOrfImpl>(new detail::PlrOrfImpl);
}

PlrOrf::PlrOrf(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::PlrOrfImpl>(new detail::PlrOrfImpl(nr,icon,name,desc));
}

PlrOrf::PlrOrf(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double area,
               double dia,double coef,double Re,int u_A,int u_D)
{
  m_impl = boost::shared_ptr<detail::PlrOrfImpl>(new detail::PlrOrfImpl(nr,icon,name,desc,lam,turb,expt,area,dia,coef,Re,u_A,u_D));
}

PlrOrf::PlrOrf(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string area,
               std::string dia,std::string coef,std::string Re,int u_A,int u_D)
{
  m_impl = boost::shared_ptr<detail::PlrOrfImpl>(new detail::PlrOrfImpl(nr,icon,name,desc,lam,turb,expt,area,dia,coef,Re,u_A,u_D));
}

PlrOrf::PlrOrf(const PlrOrf &other) : m_impl(other.m_impl)
{}

PlrOrf::~PlrOrf()
{}

PlrOrf& PlrOrf::operator=(const PlrOrf &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PlrOrf::operator==(const PlrOrf &other) const
{
  return m_impl == other.m_impl;
}

bool PlrOrf::operator!=(const PlrOrf &other) const
{
  return m_impl != other.m_impl;
}

std::string PlrOrf::write()
{
  return m_impl->write();
}

void PlrOrf::read(Reader &input)
{
  m_impl->read(input);
}

void PlrOrf::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int PlrOrf::nr() const
{
  return m_impl->nr();
}

void PlrOrf::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int PlrOrf::icon() const
{
  return m_impl->icon();
}

void PlrOrf::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string PlrOrf::name() const
{
  return m_impl->name();
}

void PlrOrf::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string PlrOrf::desc() const
{
  return m_impl->desc();
}

void PlrOrf::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double PlrOrf::lam() const
{
  return m_impl->lam();
}

bool PlrOrf::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool PlrOrf::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double PlrOrf::turb() const
{
  return m_impl->turb();
}

bool PlrOrf::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool PlrOrf::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double PlrOrf::expt() const
{
  return m_impl->expt();
}

bool PlrOrf::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool PlrOrf::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double PlrOrf::area() const
{
  return m_impl->area();
}

bool PlrOrf::setArea(const double area)
{
  return m_impl->setArea(area);
}

bool PlrOrf::setArea(const std::string &area)
{
  return m_impl->setArea(area);
}

double PlrOrf::dia() const
{
  return m_impl->dia();
}

bool PlrOrf::setDia(const double dia)
{
  return m_impl->setDia(dia);
}

bool PlrOrf::setDia(const std::string &dia)
{
  return m_impl->setDia(dia);
}

double PlrOrf::coef() const
{
  return m_impl->coef();
}

bool PlrOrf::setCoef(const double coef)
{
  return m_impl->setCoef(coef);
}

bool PlrOrf::setCoef(const std::string &coef)
{
  return m_impl->setCoef(coef);
}

double PlrOrf::Re() const
{
  return m_impl->Re();
}

bool PlrOrf::setRe(const double Re)
{
  return m_impl->setRe(Re);
}

bool PlrOrf::setRe(const std::string &Re)
{
  return m_impl->setRe(Re);
}

int PlrOrf::u_A() const
{
  return m_impl->u_A();
}

void PlrOrf::setU_A(const int u_A)
{
  m_impl->setU_A(u_A);
}

int PlrOrf::u_D() const
{
  return m_impl->u_D();
}

void PlrOrf::setU_D(const int u_D)
{
  m_impl->setU_D(u_D);
}

PlrLeak::PlrLeak()
{
  m_impl = boost::shared_ptr<detail::PlrLeakImpl>(new detail::PlrLeakImpl());
}

PlrLeak::PlrLeak(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::PlrLeakImpl>(new detail::PlrLeakImpl(nr,icon,name,desc));
}

PlrLeak::PlrLeak(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double coef,
                 double pres,double area1,double area2,double area3,int u_A1,int u_A2,int u_A3,int u_dP)
{
  m_impl = boost::shared_ptr<detail::PlrLeakImpl>(new detail::PlrLeakImpl(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,area3,u_A1,u_A2,u_A3,u_dP));
}

PlrLeak::PlrLeak(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string coef,
                 std::string pres,std::string area1,std::string area2,std::string area3,int u_A1,int u_A2,int u_A3,int u_dP)
{
  m_impl = boost::shared_ptr<detail::PlrLeakImpl>(new detail::PlrLeakImpl(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,area3,u_A1,u_A2,u_A3,u_dP));
}

PlrLeak::PlrLeak(const PlrLeak &other) : m_impl(other.m_impl)
{}

PlrLeak::~PlrLeak()
{}

PlrLeak& PlrLeak::operator=(const PlrLeak &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PlrLeak::operator==(const PlrLeak &other) const
{
  return m_impl == other.m_impl;
}

bool PlrLeak::operator!=(const PlrLeak &other) const
{
  return m_impl != other.m_impl;
}

std::string PlrLeak::write()
{
  return m_impl->write(dataType());
}

void PlrLeak::read(Reader &input)
{
  m_impl->read(input);
}

void PlrLeak::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int PlrLeak::nr() const
{
  return m_impl->nr();
}

void PlrLeak::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int PlrLeak::icon() const
{
  return m_impl->icon();
}

void PlrLeak::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string PlrLeak::name() const
{
  return m_impl->name();
}

void PlrLeak::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string PlrLeak::desc() const
{
  return m_impl->desc();
}

void PlrLeak::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double PlrLeak::lam() const
{
  return m_impl->lam();
}

bool PlrLeak::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool PlrLeak::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double PlrLeak::turb() const
{
  return m_impl->turb();
}

bool PlrLeak::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool PlrLeak::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double PlrLeak::expt() const
{
  return m_impl->expt();
}

bool PlrLeak::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool PlrLeak::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double PlrLeak::coef() const
{
  return m_impl->coef();
}

bool PlrLeak::setCoef(const double coef)
{
  return m_impl->setCoef(coef);
}

bool PlrLeak::setCoef(const std::string &coef)
{
  return m_impl->setCoef(coef);
}

double PlrLeak::pres() const
{
  return m_impl->pres();
}

bool PlrLeak::setPres(const double pres)
{
  return m_impl->setPres(pres);
}

bool PlrLeak::setPres(const std::string &pres)
{
  return m_impl->setPres(pres);
}

double PlrLeak::area1() const
{
  return m_impl->area1();
}

bool PlrLeak::setArea1(const double area1)
{
  return m_impl->setArea1(area1);
}

bool PlrLeak::setArea1(const std::string &area1)
{
  return m_impl->setArea1(area1);
}

double PlrLeak::area2() const
{
  return m_impl->area2();
}

bool PlrLeak::setArea2(const double area2)
{
  return m_impl->setArea2(area2);
}

bool PlrLeak::setArea2(const std::string &area2)
{
  return m_impl->setArea2(area2);
}

double PlrLeak::area3() const
{
  return m_impl->area3();
}

bool PlrLeak::setArea3(const double area3)
{
  return m_impl->setArea3(area3);
}

bool PlrLeak::setArea3(const std::string &area3)
{
  return m_impl->setArea3(area3);
}

int PlrLeak::u_A1() const
{
  return m_impl->u_A1();
}

void PlrLeak::setU_A1(const int u_A1)
{
  m_impl->setU_A1(u_A1);
}

int PlrLeak::u_A2() const
{
  return m_impl->u_A2();
}

void PlrLeak::setU_A2(const int u_A2)
{
  m_impl->setU_A2(u_A2);
}

int PlrLeak::u_A3() const
{
  return m_impl->u_A3();
}

void PlrLeak::setU_A3(const int u_A3)
{
  m_impl->setU_A3(u_A3);
}

int PlrLeak::u_dP() const
{
  return m_impl->u_dP();
}

void PlrLeak::setU_dP(const int u_dP)
{
  m_impl->setU_dP(u_dP);
}

PlrLeak1::PlrLeak1() : PlrLeak()
{}

PlrLeak1::PlrLeak1(int nr,int icon,std::string name,std::string desc) : PlrLeak(nr,icon,name,desc)
{}

PlrLeak1::PlrLeak1(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double coef,
                   double pres,double area1,double area2,double area3,int u_A1,int u_A2,int u_A3,int u_dP)
                   : PlrLeak(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,area3,u_A1,u_A2,u_A3,u_dP)
{}

PlrLeak1::PlrLeak1(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string coef,
                   std::string pres,std::string area1,std::string area2,std::string area3,int u_A1,int u_A2,int u_A3,int u_dP)
                   : PlrLeak(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,area3,u_A1,u_A2,u_A3,u_dP)
{}

PlrLeak1::~PlrLeak1()
{}

PlrLeak2::PlrLeak2() : PlrLeak()
{}

PlrLeak2::PlrLeak2(int nr,int icon,std::string name,std::string desc) : PlrLeak(nr,icon,name,desc)
{}

PlrLeak2::PlrLeak2(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double coef,
                   double pres,double area1,double area2,double area3,int u_A1,int u_A2,int u_A3,int u_dP)
                   : PlrLeak(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,area3,u_A1,u_A2,u_A3,u_dP)
{}

PlrLeak2::PlrLeak2(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string coef,
                   std::string pres,std::string area1,std::string area2,std::string area3,int u_A1,int u_A2,int u_A3,int u_dP)
                   : PlrLeak(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,area3,u_A1,u_A2,u_A3,u_dP)
{}

PlrLeak2::~PlrLeak2()
{}

PlrLeak3::PlrLeak3() : PlrLeak()
{}

PlrLeak3::PlrLeak3(int nr,int icon,std::string name,std::string desc) : PlrLeak(nr,icon,name,desc)
{}

PlrLeak3::PlrLeak3(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double coef,
                   double pres,double area1,double area2,double area3,int u_A1,int u_A2,int u_A3,int u_dP)
                   : PlrLeak(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,area3,u_A1,u_A2,u_A3,u_dP)
{}

PlrLeak3::PlrLeak3(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string coef,
                   std::string pres,std::string area1,std::string area2,std::string area3,int u_A1,int u_A2,int u_A3,int u_dP)
                   : PlrLeak(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,area3,u_A1,u_A2,u_A3,u_dP)
{}

PlrLeak3::~PlrLeak3()
{}

PlrConn::PlrConn()
{
  m_impl = boost::shared_ptr<detail::PlrConnImpl>(new detail::PlrConnImpl);
}

PlrConn::PlrConn(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::PlrConnImpl>(new detail::PlrConnImpl(nr,icon,name,desc));
}

PlrConn::PlrConn(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double area,
                 double coef,int u_A)
{
  m_impl = boost::shared_ptr<detail::PlrConnImpl>(new detail::PlrConnImpl(nr,icon,name,desc,lam,turb,expt,area,coef,u_A));
}

PlrConn::PlrConn(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string area,
                 std::string coef,int u_A)
{
  m_impl = boost::shared_ptr<detail::PlrConnImpl>(new detail::PlrConnImpl(nr,icon,name,desc,lam,turb,expt,area,coef,u_A));
}

PlrConn::PlrConn(const PlrConn &other) : m_impl(other.m_impl)
{}

PlrConn::~PlrConn()
{}

PlrConn& PlrConn::operator=(const PlrConn &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PlrConn::operator==(const PlrConn &other) const
{
  return m_impl == other.m_impl;
}

bool PlrConn::operator!=(const PlrConn &other) const
{
  return m_impl != other.m_impl;
}

std::string PlrConn::write()
{
  return m_impl->write();
}

void PlrConn::read(Reader &input)
{
  m_impl->read(input);
}

void PlrConn::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int PlrConn::nr() const
{
  return m_impl->nr();
}

void PlrConn::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int PlrConn::icon() const
{
  return m_impl->icon();
}

void PlrConn::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string PlrConn::name() const
{
  return m_impl->name();
}

void PlrConn::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string PlrConn::desc() const
{
  return m_impl->desc();
}

void PlrConn::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double PlrConn::lam() const
{
  return m_impl->lam();
}

bool PlrConn::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool PlrConn::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double PlrConn::turb() const
{
  return m_impl->turb();
}

bool PlrConn::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool PlrConn::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double PlrConn::expt() const
{
  return m_impl->expt();
}

bool PlrConn::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool PlrConn::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double PlrConn::area() const
{
  return m_impl->area();
}

bool PlrConn::setArea(const double area)
{
  return m_impl->setArea(area);
}

bool PlrConn::setArea(const std::string &area)
{
  return m_impl->setArea(area);
}

double PlrConn::coef() const
{
  return m_impl->coef();
}

bool PlrConn::setCoef(const double coef)
{
  return m_impl->setCoef(coef);
}

bool PlrConn::setCoef(const std::string &coef)
{
  return m_impl->setCoef(coef);
}

int PlrConn::u_A() const
{
  return m_impl->u_A();
}

void PlrConn::setU_A(const int u_A)
{
  m_impl->setU_A(u_A);
}

PlrGeneral::PlrGeneral()
{
  m_impl = boost::shared_ptr<detail::PlrGeneralImpl>(new detail::PlrGeneralImpl);
}

PlrGeneral::PlrGeneral(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::PlrGeneralImpl>(new detail::PlrGeneralImpl(nr,icon,name,desc));
}

PlrGeneral::PlrGeneral(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt)
{
  m_impl = boost::shared_ptr<detail::PlrGeneralImpl>(new detail::PlrGeneralImpl(nr,icon,name,desc,lam,turb,expt));
}

PlrGeneral::PlrGeneral(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt)
{
  m_impl = boost::shared_ptr<detail::PlrGeneralImpl>(new detail::PlrGeneralImpl(nr,icon,name,desc,lam,turb,expt));
}

PlrGeneral::PlrGeneral(const PlrGeneral &other) : m_impl(other.m_impl)
{}

PlrGeneral::~PlrGeneral()
{}

PlrGeneral& PlrGeneral::operator=(const PlrGeneral &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PlrGeneral::operator==(const PlrGeneral &other) const
{
  return m_impl == other.m_impl;
}

bool PlrGeneral::operator!=(const PlrGeneral &other) const
{
  return m_impl != other.m_impl;
}

std::string PlrGeneral::write()
{
  return m_impl->write(dataType());
}

void PlrGeneral::read(Reader &input)
{
  m_impl->read(input);
}

void PlrGeneral::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int PlrGeneral::nr() const
{
  return m_impl->nr();
}

void PlrGeneral::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int PlrGeneral::icon() const
{
  return m_impl->icon();
}

void PlrGeneral::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string PlrGeneral::name() const
{
  return m_impl->name();
}

void PlrGeneral::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string PlrGeneral::desc() const
{
  return m_impl->desc();
}

void PlrGeneral::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double PlrGeneral::lam() const
{
  return m_impl->lam();
}

bool PlrGeneral::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool PlrGeneral::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double PlrGeneral::turb() const
{
  return m_impl->turb();
}

bool PlrGeneral::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool PlrGeneral::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double PlrGeneral::expt() const
{
  return m_impl->expt();
}

bool PlrGeneral::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool PlrGeneral::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

PlrQcn::PlrQcn() : PlrGeneral()
{}

PlrQcn::PlrQcn(int nr,int icon,std::string name,std::string desc) : PlrGeneral(nr,icon,name,desc)
{}

PlrQcn::PlrQcn(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt)
: PlrGeneral(nr,icon,name,desc,lam,turb,expt)
{}

PlrQcn::PlrQcn(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt)
: PlrGeneral(nr,icon,name,desc,lam,turb,expt)
{}

PlrQcn::~PlrQcn()
{}

PlrFcn::PlrFcn() : PlrGeneral()
{}

PlrFcn::PlrFcn(int nr,int icon,std::string name,std::string desc) : PlrGeneral(nr,icon,name,desc)
{}

PlrFcn::PlrFcn(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt)
: PlrGeneral(nr,icon,name,desc,lam,turb,expt)
{}

PlrFcn::PlrFcn(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt)
: PlrGeneral(nr,icon,name,desc,lam,turb,expt)
{}

PlrFcn::~PlrFcn()
{}

PlrTest1::PlrTest1()
{
  m_impl = boost::shared_ptr<detail::PlrTest1Impl>(new detail::PlrTest1Impl);
}

PlrTest1::PlrTest1(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::PlrTest1Impl>(new detail::PlrTest1Impl(nr,icon,name,desc));
}

PlrTest1::PlrTest1(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dP,
                   double Flow,int u_P,int u_F)
{
  m_impl = boost::shared_ptr<detail::PlrTest1Impl>(new detail::PlrTest1Impl(nr,icon,name,desc,lam,turb,expt,dP,Flow,u_P,u_F));
}

PlrTest1::PlrTest1(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dP,
                   std::string Flow,int u_P,int u_F)
{
  m_impl = boost::shared_ptr<detail::PlrTest1Impl>(new detail::PlrTest1Impl(nr,icon,name,desc,lam,turb,expt,dP,Flow,u_P,u_F));
}

PlrTest1::PlrTest1(const PlrTest1 &other) : m_impl(other.m_impl)
{}

PlrTest1::~PlrTest1()
{}

PlrTest1& PlrTest1::operator=(const PlrTest1 &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PlrTest1::operator==(const PlrTest1 &other) const
{
  return m_impl == other.m_impl;
}

bool PlrTest1::operator!=(const PlrTest1 &other) const
{
  return m_impl != other.m_impl;
}

std::string PlrTest1::write()
{
  return m_impl->write();
}

void PlrTest1::read(Reader &input)
{
  m_impl->read(input);
}

void PlrTest1::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int PlrTest1::nr() const
{
  return m_impl->nr();
}

void PlrTest1::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int PlrTest1::icon() const
{
  return m_impl->icon();
}

void PlrTest1::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string PlrTest1::name() const
{
  return m_impl->name();
}

void PlrTest1::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string PlrTest1::desc() const
{
  return m_impl->desc();
}

void PlrTest1::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double PlrTest1::lam() const
{
  return m_impl->lam();
}

bool PlrTest1::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool PlrTest1::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double PlrTest1::turb() const
{
  return m_impl->turb();
}

bool PlrTest1::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool PlrTest1::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double PlrTest1::expt() const
{
  return m_impl->expt();
}

bool PlrTest1::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool PlrTest1::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double PlrTest1::dP() const
{
  return m_impl->dP();
}

bool PlrTest1::setDP(const double dP)
{
  return m_impl->setDP(dP);
}

bool PlrTest1::setDP(const std::string &dP)
{
  return m_impl->setDP(dP);
}

double PlrTest1::Flow() const
{
  return m_impl->Flow();
}

bool PlrTest1::setFlow(const double Flow)
{
  return m_impl->setFlow(Flow);
}

bool PlrTest1::setFlow(const std::string &Flow)
{
  return m_impl->setFlow(Flow);
}

int PlrTest1::u_P() const
{
  return m_impl->u_P();
}

void PlrTest1::setU_P(const int u_P)
{
  m_impl->setU_P(u_P);
}

int PlrTest1::u_F() const
{
  return m_impl->u_F();
}

void PlrTest1::setU_F(const int u_F)
{
  m_impl->setU_F(u_F);
}

PlrTest2::PlrTest2()
{
  m_impl = boost::shared_ptr<detail::PlrTest2Impl>(new detail::PlrTest2Impl);
}

PlrTest2::PlrTest2(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::PlrTest2Impl>(new detail::PlrTest2Impl(nr,icon,name,desc));
}

PlrTest2::PlrTest2(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dP1,
                   double F1,double dP2,double F2,int u_P1,int u_F1,int u_P2,int u_F2)
{
  m_impl = boost::shared_ptr<detail::PlrTest2Impl>(new detail::PlrTest2Impl(nr,icon,name,desc,lam,turb,expt,dP1,F1,dP2,F2,u_P1,u_F1,u_P2,u_F2));
}

PlrTest2::PlrTest2(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dP1,
                   std::string F1,std::string dP2,std::string F2,int u_P1,int u_F1,int u_P2,int u_F2)
{
  m_impl = boost::shared_ptr<detail::PlrTest2Impl>(new detail::PlrTest2Impl(nr,icon,name,desc,lam,turb,expt,dP1,F1,dP2,F2,u_P1,u_F1,u_P2,u_F2));
}

PlrTest2::PlrTest2(const PlrTest2 &other) : m_impl(other.m_impl)
{}

PlrTest2::~PlrTest2()
{}

PlrTest2& PlrTest2::operator=(const PlrTest2 &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PlrTest2::operator==(const PlrTest2 &other) const
{
  return m_impl == other.m_impl;
}

bool PlrTest2::operator!=(const PlrTest2 &other) const
{
  return m_impl != other.m_impl;
}

std::string PlrTest2::write()
{
  return m_impl->write();
}

void PlrTest2::read(Reader &input)
{
  m_impl->read(input);
}

void PlrTest2::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int PlrTest2::nr() const
{
  return m_impl->nr();
}

void PlrTest2::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int PlrTest2::icon() const
{
  return m_impl->icon();
}

void PlrTest2::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string PlrTest2::name() const
{
  return m_impl->name();
}

void PlrTest2::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string PlrTest2::desc() const
{
  return m_impl->desc();
}

void PlrTest2::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double PlrTest2::lam() const
{
  return m_impl->lam();
}

bool PlrTest2::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool PlrTest2::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double PlrTest2::turb() const
{
  return m_impl->turb();
}

bool PlrTest2::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool PlrTest2::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double PlrTest2::expt() const
{
  return m_impl->expt();
}

bool PlrTest2::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool PlrTest2::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double PlrTest2::dP1() const
{
  return m_impl->dP1();
}

bool PlrTest2::setDP1(const double dP1)
{
  return m_impl->setDP1(dP1);
}

bool PlrTest2::setDP1(const std::string &dP1)
{
  return m_impl->setDP1(dP1);
}

double PlrTest2::F1() const
{
  return m_impl->F1();
}

bool PlrTest2::setF1(const double F1)
{
  return m_impl->setF1(F1);
}

bool PlrTest2::setF1(const std::string &F1)
{
  return m_impl->setF1(F1);
}

double PlrTest2::dP2() const
{
  return m_impl->dP2();
}

bool PlrTest2::setDP2(const double dP2)
{
  return m_impl->setDP2(dP2);
}

bool PlrTest2::setDP2(const std::string &dP2)
{
  return m_impl->setDP2(dP2);
}

double PlrTest2::F2() const
{
  return m_impl->F2();
}

bool PlrTest2::setF2(const double F2)
{
  return m_impl->setF2(F2);
}

bool PlrTest2::setF2(const std::string &F2)
{
  return m_impl->setF2(F2);
}

int PlrTest2::u_P1() const
{
  return m_impl->u_P1();
}

void PlrTest2::setU_P1(const int u_P1)
{
  m_impl->setU_P1(u_P1);
}

int PlrTest2::u_F1() const
{
  return m_impl->u_F1();
}

void PlrTest2::setU_F1(const int u_F1)
{
  m_impl->setU_F1(u_F1);
}

int PlrTest2::u_P2() const
{
  return m_impl->u_P2();
}

void PlrTest2::setU_P2(const int u_P2)
{
  m_impl->setU_P2(u_P2);
}

int PlrTest2::u_F2() const
{
  return m_impl->u_F2();
}

void PlrTest2::setU_F2(const int u_F2)
{
  m_impl->setU_F2(u_F2);
}

PlrCrack::PlrCrack()
{
  m_impl = boost::shared_ptr<detail::PlrCrackImpl>(new detail::PlrCrackImpl);
}

PlrCrack::PlrCrack(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::PlrCrackImpl>(new detail::PlrCrackImpl(nr,icon,name,desc));
}

PlrCrack::PlrCrack(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double length,
                   double width,int u_L,int u_W)
{
  m_impl = boost::shared_ptr<detail::PlrCrackImpl>(new detail::PlrCrackImpl(nr,icon,name,desc,lam,turb,expt,length,width,u_L,u_W));
}

PlrCrack::PlrCrack(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string length,
                   std::string width,int u_L,int u_W)
{
  m_impl = boost::shared_ptr<detail::PlrCrackImpl>(new detail::PlrCrackImpl(nr,icon,name,desc,lam,turb,expt,length,width,u_L,u_W));
}

PlrCrack::PlrCrack(const PlrCrack &other) : m_impl(other.m_impl)
{}

PlrCrack::~PlrCrack()
{}

PlrCrack& PlrCrack::operator=(const PlrCrack &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PlrCrack::operator==(const PlrCrack &other) const
{
  return m_impl == other.m_impl;
}

bool PlrCrack::operator!=(const PlrCrack &other) const
{
  return m_impl != other.m_impl;
}

std::string PlrCrack::write()
{
  return m_impl->write();
}

void PlrCrack::read(Reader &input)
{
  m_impl->read(input);
}

void PlrCrack::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int PlrCrack::nr() const
{
  return m_impl->nr();
}

void PlrCrack::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int PlrCrack::icon() const
{
  return m_impl->icon();
}

void PlrCrack::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string PlrCrack::name() const
{
  return m_impl->name();
}

void PlrCrack::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string PlrCrack::desc() const
{
  return m_impl->desc();
}

void PlrCrack::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double PlrCrack::lam() const
{
  return m_impl->lam();
}

bool PlrCrack::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool PlrCrack::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double PlrCrack::turb() const
{
  return m_impl->turb();
}

bool PlrCrack::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool PlrCrack::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double PlrCrack::expt() const
{
  return m_impl->expt();
}

bool PlrCrack::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool PlrCrack::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double PlrCrack::length() const
{
  return m_impl->length();
}

bool PlrCrack::setLength(const double length)
{
  return m_impl->setLength(length);
}

bool PlrCrack::setLength(const std::string &length)
{
  return m_impl->setLength(length);
}

double PlrCrack::width() const
{
  return m_impl->width();
}

bool PlrCrack::setWidth(const double width)
{
  return m_impl->setWidth(width);
}

bool PlrCrack::setWidth(const std::string &width)
{
  return m_impl->setWidth(width);
}

int PlrCrack::u_L() const
{
  return m_impl->u_L();
}

void PlrCrack::setU_L(const int u_L)
{
  m_impl->setU_L(u_L);
}

int PlrCrack::u_W() const
{
  return m_impl->u_W();
}

void PlrCrack::setU_W(const int u_W)
{
  m_impl->setU_W(u_W);
}

PlrStair::PlrStair()
{
  m_impl = boost::shared_ptr<detail::PlrStairImpl>(new detail::PlrStairImpl);
}

PlrStair::PlrStair(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::PlrStairImpl>(new detail::PlrStairImpl(nr,icon,name,desc));
}

PlrStair::PlrStair(int nr,int icon,std::string name,std::string desc,double lam,double turb,
                   double expt,double Ht,double Area,double peo,int tread,int u_A,int u_D)
{
  m_impl = boost::shared_ptr<detail::PlrStairImpl>(new detail::PlrStairImpl(nr,icon,name,desc,lam,turb,expt,Ht,Area,peo,tread,u_A,u_D));
}

PlrStair::PlrStair(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,
                   std::string expt,std::string Ht,std::string Area,std::string peo,int tread,int u_A,int u_D)
{
  m_impl = boost::shared_ptr<detail::PlrStairImpl>(new detail::PlrStairImpl(nr,icon,name,desc,lam,turb,expt,Ht,Area,peo,tread,u_A,u_D));
}

PlrStair::PlrStair(const PlrStair &other) : m_impl(other.m_impl)
{}

PlrStair::~PlrStair()
{}

PlrStair& PlrStair::operator=(const PlrStair &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PlrStair::operator==(const PlrStair &other) const
{
  return m_impl == other.m_impl;
}

bool PlrStair::operator!=(const PlrStair &other) const
{
  return m_impl != other.m_impl;
}

std::string PlrStair::write()
{
  return m_impl->write();
}

void PlrStair::read(Reader &input)
{
  m_impl->read(input);
}

void PlrStair::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int PlrStair::nr() const
{
  return m_impl->nr();
}

void PlrStair::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int PlrStair::icon() const
{
  return m_impl->icon();
}

void PlrStair::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string PlrStair::name() const
{
  return m_impl->name();
}

void PlrStair::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string PlrStair::desc() const
{
  return m_impl->desc();
}

void PlrStair::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double PlrStair::lam() const
{
  return m_impl->lam();
}

bool PlrStair::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool PlrStair::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double PlrStair::turb() const
{
  return m_impl->turb();
}

bool PlrStair::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool PlrStair::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double PlrStair::expt() const
{
  return m_impl->expt();
}

bool PlrStair::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool PlrStair::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double PlrStair::Ht() const
{
  return m_impl->Ht();
}

bool PlrStair::setHt(const double Ht)
{
  return m_impl->setHt(Ht);
}

bool PlrStair::setHt(const std::string &Ht)
{
  return m_impl->setHt(Ht);
}

double PlrStair::area() const
{
  return m_impl->area();
}

bool PlrStair::setArea(const double Area)
{
  return m_impl->setArea(Area);
}

bool PlrStair::setArea(const std::string &Area)
{
  return m_impl->setArea(Area);
}

double PlrStair::people() const
{
  return m_impl->people();
}

bool PlrStair::setPeople(const double peo)
{
  return m_impl->setPeople(peo);
}

bool PlrStair::setPeople(const std::string &peo)
{
  return m_impl->setPeople(peo);
}

int PlrStair::tread() const
{
  return m_impl->tread();
}

void PlrStair::setTread(const int tread)
{
  m_impl->setTread(tread);
}

int PlrStair::u_A() const
{
  return m_impl->u_A();
}

void PlrStair::setU_A(const int u_A)
{
  m_impl->setU_A(u_A);
}

int PlrStair::u_D() const
{
  return m_impl->u_D();
}

void PlrStair::setU_D(const int u_D)
{
  m_impl->setU_D(u_D);
}

PlrShaft::PlrShaft()
{
  m_impl = boost::shared_ptr<detail::PlrShaftImpl>(new detail::PlrShaftImpl);
}

PlrShaft::PlrShaft(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::PlrShaftImpl>(new detail::PlrShaftImpl(nr,icon,name,desc));
}

PlrShaft::PlrShaft(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double Ht,
                   double area,double perim,double rough,int u_A,int u_D,int u_P,int u_R)
{
  m_impl = boost::shared_ptr<detail::PlrShaftImpl>(new detail::PlrShaftImpl(nr,icon,name,desc,lam,turb,expt,Ht,area,perim,rough,u_A,u_D,u_P,u_R));
}

PlrShaft::PlrShaft(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string Ht,
                   std::string area,std::string perim,std::string rough,int u_A,int u_D,int u_P,int u_R)
{
  m_impl = boost::shared_ptr<detail::PlrShaftImpl>(new detail::PlrShaftImpl(nr,icon,name,desc,lam,turb,expt,Ht,area,perim,rough,u_A,u_D,u_P,u_R));
}

PlrShaft::PlrShaft(const PlrShaft &other) : m_impl(other.m_impl)
{}

PlrShaft::~PlrShaft()
{}

PlrShaft& PlrShaft::operator=(const PlrShaft &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PlrShaft::operator==(const PlrShaft &other) const
{
  return m_impl == other.m_impl;
}

bool PlrShaft::operator!=(const PlrShaft &other) const
{
  return m_impl != other.m_impl;
}

std::string PlrShaft::write()
{
  return m_impl->write();
}

void PlrShaft::read(Reader &input)
{
  m_impl->read(input);
}

void PlrShaft::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int PlrShaft::nr() const
{
  return m_impl->nr();
}

void PlrShaft::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int PlrShaft::icon() const
{
  return m_impl->icon();
}

void PlrShaft::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string PlrShaft::name() const
{
  return m_impl->name();
}

void PlrShaft::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string PlrShaft::desc() const
{
  return m_impl->desc();
}

void PlrShaft::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double PlrShaft::lam() const
{
  return m_impl->lam();
}

bool PlrShaft::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool PlrShaft::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double PlrShaft::turb() const
{
  return m_impl->turb();
}

bool PlrShaft::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool PlrShaft::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double PlrShaft::expt() const
{
  return m_impl->expt();
}

bool PlrShaft::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool PlrShaft::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double PlrShaft::Ht() const
{
  return m_impl->Ht();
}

bool PlrShaft::setHt(const double Ht)
{
  return m_impl->setHt(Ht);
}

bool PlrShaft::setHt(const std::string &Ht)
{
  return m_impl->setHt(Ht);
}

double PlrShaft::area() const
{
  return m_impl->area();
}

bool PlrShaft::setArea(const double area)
{
  return m_impl->setArea(area);
}

bool PlrShaft::setArea(const std::string &area)
{
  return m_impl->setArea(area);
}

double PlrShaft::perim() const
{
  return m_impl->perim();
}

bool PlrShaft::setPerim(const double perim)
{
  return m_impl->setPerim(perim);
}

bool PlrShaft::setPerim(const std::string &perim)
{
  return m_impl->setPerim(perim);
}

double PlrShaft::rough() const
{
  return m_impl->rough();
}

bool PlrShaft::setRough(const double rough)
{
  return m_impl->setRough(rough);
}

bool PlrShaft::setRough(const std::string &rough)
{
  return m_impl->setRough(rough);
}

int PlrShaft::u_A() const
{
  return m_impl->u_A();
}

void PlrShaft::setU_A(const int u_A)
{
  m_impl->setU_A(u_A);
}

int PlrShaft::u_D() const
{
  return m_impl->u_D();
}

void PlrShaft::setU_D(const int u_D)
{
  m_impl->setU_D(u_D);
}

int PlrShaft::u_P() const
{
  return m_impl->u_P();
}

void PlrShaft::setU_P(const int u_P)
{
  m_impl->setU_P(u_P);
}

int PlrShaft::u_R() const
{
  return m_impl->u_R();
}

void PlrShaft::setU_R(const int u_R)
{
  m_impl->setU_R(u_R);
}

PlrBackDamper::PlrBackDamper()
{
  m_impl = boost::shared_ptr<detail::PlrBackDamperImpl>(new detail::PlrBackDamperImpl);
}

PlrBackDamper::PlrBackDamper(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::PlrBackDamperImpl>(new detail::PlrBackDamperImpl(nr,icon,name,desc));
}

PlrBackDamper::PlrBackDamper(int nr,int icon,std::string name,std::string desc,double lam,double Cp,double xp,double Cn,double xn)
{
  m_impl = boost::shared_ptr<detail::PlrBackDamperImpl>(new detail::PlrBackDamperImpl(nr,icon,name,desc,lam,Cp,xp,Cn,xn));
}

PlrBackDamper::PlrBackDamper(int nr,int icon,std::string name,std::string desc,std::string lam,std::string Cp,std::string xp,std::string Cn,std::string xn)
{
  m_impl = boost::shared_ptr<detail::PlrBackDamperImpl>(new detail::PlrBackDamperImpl(nr,icon,name,desc,lam,Cp,xp,Cn,xn));
}

PlrBackDamper::PlrBackDamper(const PlrBackDamper &other) : m_impl(other.m_impl)
{}

PlrBackDamper::~PlrBackDamper()
{}

PlrBackDamper& PlrBackDamper::operator=(const PlrBackDamper &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool PlrBackDamper::operator==(const PlrBackDamper &other) const
{
  return m_impl == other.m_impl;
}

bool PlrBackDamper::operator!=(const PlrBackDamper &other) const
{
  return m_impl != other.m_impl;
}

std::string PlrBackDamper::write()
{
  return m_impl->write("plr_bdq");
}

void PlrBackDamper::read(Reader &input)
{
  m_impl->read(input);
}

void PlrBackDamper::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int PlrBackDamper::nr() const
{
  return m_impl->nr();
}

void PlrBackDamper::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int PlrBackDamper::icon() const
{
  return m_impl->icon();
}

void PlrBackDamper::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string PlrBackDamper::name() const
{
  return m_impl->name();
}

void PlrBackDamper::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string PlrBackDamper::desc() const
{
  return m_impl->desc();
}

void PlrBackDamper::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double PlrBackDamper::lam() const
{
  return m_impl->lam();
}

bool PlrBackDamper::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool PlrBackDamper::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double PlrBackDamper::Cp() const
{
  return m_impl->Cp();
}

bool PlrBackDamper::setCp(const double Cp)
{
  return m_impl->setCp(Cp);
}

bool PlrBackDamper::setCp(const std::string &Cp)
{
  return m_impl->setCp(Cp);
}

double PlrBackDamper::xp() const
{
  return m_impl->xp();
}

bool PlrBackDamper::setXp(const double xp)
{
  return m_impl->setXp(xp);
}

bool PlrBackDamper::setXp(const std::string &xp)
{
  return m_impl->setXp(xp);
}

double PlrBackDamper::Cn() const
{
  return m_impl->Cn();
}

bool PlrBackDamper::setCn(const double Cn)
{
  return m_impl->setCn(Cn);
}

bool PlrBackDamper::setCn(const std::string &Cn)
{
  return m_impl->setCn(Cn);
}

double PlrBackDamper::xn() const
{
  return m_impl->xn();
}

bool PlrBackDamper::setXn(const double xn)
{
  return m_impl->setXn(xn);
}

bool PlrBackDamper::setXn(const std::string &xn)
{
  return m_impl->setXn(xn);
}

PlrBdq::PlrBdq() : PlrBackDamper()
{}

PlrBdq::PlrBdq(int nr,int icon,std::string name,std::string desc)
: PlrBackDamper(nr,icon,name,desc)
{}

PlrBdq::PlrBdq(int nr,int icon,std::string name,std::string desc,double lam,double Cp,double xp,double Cn,double xn)
: PlrBackDamper(nr,icon,name,desc,lam,Cp,xp,Cn,xn)
{}

PlrBdq::PlrBdq(int nr,int icon,std::string name,std::string desc,std::string lam,std::string Cp,std::string xp,std::string Cn,std::string xn)
: PlrBackDamper(nr,icon,name,desc,lam,Cp,xp,Cn,xn)
{}

PlrBdq::~PlrBdq()
{}

PlrBdf::PlrBdf() : PlrBackDamper()
{}

PlrBdf::PlrBdf(int nr,int icon,std::string name,std::string desc)
: PlrBackDamper(nr,icon,name,desc)
{}

PlrBdf::PlrBdf(int nr,int icon,std::string name,std::string desc,double lam,double Cp,double xp,double Cn,double xn)
: PlrBackDamper(nr,icon,name,desc,lam,Cp,xp,Cn,xn)
{}

PlrBdf::PlrBdf(int nr,int icon,std::string name,std::string desc,std::string lam,std::string Cp,std::string xp,std::string Cn,std::string xn)
: PlrBackDamper(nr,icon,name,desc,lam,Cp,xp,Cn,xn)
{}

PlrBdf::~PlrBdf()
{}

QfrGeneral::QfrGeneral()
{
  m_impl = boost::shared_ptr<detail::QfrQuadraticImpl>(new detail::QfrQuadraticImpl);
}

QfrGeneral::QfrGeneral(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::QfrQuadraticImpl>(new detail::QfrQuadraticImpl(nr,icon,name,desc));
}

QfrGeneral::QfrGeneral(int nr,int icon,std::string name,std::string desc,double a,double b)
{
  m_impl = boost::shared_ptr<detail::QfrQuadraticImpl>(new detail::QfrQuadraticImpl(nr,icon,name,desc,a,b));
}

QfrGeneral::QfrGeneral(int nr,int icon,std::string name,std::string desc,std::string a,std::string b)
{
  m_impl = boost::shared_ptr<detail::QfrQuadraticImpl>(new detail::QfrQuadraticImpl(nr,icon,name,desc,a,b));
}

QfrGeneral::QfrGeneral(const QfrGeneral &other) : m_impl(other.m_impl)
{}

QfrGeneral::~QfrGeneral()
{}

QfrGeneral& QfrGeneral::operator=(const QfrGeneral &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool QfrGeneral::operator==(const QfrGeneral &other) const
{
  return m_impl == other.m_impl;
}

bool QfrGeneral::operator!=(const QfrGeneral &other) const
{
  return m_impl != other.m_impl;
}

std::string QfrGeneral::write()
{
  return m_impl->write(dataType());
}

void QfrGeneral::read(Reader &input)
{
  m_impl->read(input);
}

void QfrGeneral::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int QfrGeneral::nr() const
{
  return m_impl->nr();
}

void QfrGeneral::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int QfrGeneral::icon() const
{
  return m_impl->icon();
}

void QfrGeneral::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string QfrGeneral::name() const
{
  return m_impl->name();
}

void QfrGeneral::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string QfrGeneral::desc() const
{
  return m_impl->desc();
}

void QfrGeneral::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double QfrGeneral::a() const
{
  return m_impl->a();
}

bool QfrGeneral::setA(const double a)
{
  return m_impl->setA(a);
}

bool QfrGeneral::setA(const std::string &a)
{
  return m_impl->setA(a);
}

double QfrGeneral::b() const
{
  return m_impl->b();
}

bool QfrGeneral::setB(const double b)
{
  return m_impl->setB(b);
}

bool QfrGeneral::setB(const std::string &b)
{
  return m_impl->setB(b);
}

QfrQab::QfrQab() : QfrGeneral()
{}

QfrQab::QfrQab(int nr,int icon,std::string name,std::string desc) : QfrGeneral(nr,icon,name,desc)
{}

QfrQab::QfrQab(int nr,int icon,std::string name,std::string desc,double a,double b)
: QfrGeneral(nr,icon,name,desc,a,b)
{}

QfrQab::QfrQab(int nr,int icon,std::string name,std::string desc,std::string a,std::string b)
: QfrGeneral(nr,icon,name,desc,a,b)
{}

QfrQab::~QfrQab()
{}

QfrFab::QfrFab() : QfrGeneral()
{}

QfrFab::QfrFab(int nr,int icon,std::string name,std::string desc) : QfrGeneral(nr,icon,name,desc)
{}

QfrFab::QfrFab(int nr,int icon,std::string name,std::string desc,double a,double b)
: QfrGeneral(nr,icon,name,desc,a,b)
{}

QfrFab::QfrFab(int nr,int icon,std::string name,std::string desc,std::string a,std::string b)
: QfrGeneral(nr,icon,name,desc,a,b)
{}

QfrFab::~QfrFab()
{}

QfrCrack::QfrCrack()
{
  m_impl = boost::shared_ptr<detail::QfrCrackImpl>(new detail::QfrCrackImpl);
}

QfrCrack::QfrCrack(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::QfrCrackImpl>(new detail::QfrCrackImpl(nr,icon,name,desc));
}

QfrCrack::QfrCrack(int nr,int icon,std::string name,std::string desc,double a,double b,double length,double width,
                   double depth,int nB,int u_L,int u_W,int u_D)
{
  m_impl = boost::shared_ptr<detail::QfrCrackImpl>(new detail::QfrCrackImpl(nr,icon,name,desc,a,b,length,width,depth,nB,u_L,u_W,u_D));
}

QfrCrack::QfrCrack(int nr,int icon,std::string name,std::string desc,std::string a,std::string b,std::string length,std::string width,
                   std::string depth,int nB,int u_L,int u_W,int u_D)
{
  m_impl = boost::shared_ptr<detail::QfrCrackImpl>(new detail::QfrCrackImpl(nr,icon,name,desc,a,b,length,width,depth,nB,u_L,u_W,u_D));
}

QfrCrack::QfrCrack(const QfrCrack &other) : m_impl(other.m_impl)
{}

QfrCrack::~QfrCrack()
{}

QfrCrack& QfrCrack::operator=(const QfrCrack &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool QfrCrack::operator==(const QfrCrack &other) const
{
  return m_impl == other.m_impl;
}

bool QfrCrack::operator!=(const QfrCrack &other) const
{
  return m_impl != other.m_impl;
}

std::string QfrCrack::write()
{
  return m_impl->write();
}

void QfrCrack::read(Reader &input)
{
  m_impl->read(input);
}

void QfrCrack::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int QfrCrack::nr() const
{
  return m_impl->nr();
}

void QfrCrack::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int QfrCrack::icon() const
{
  return m_impl->icon();
}

void QfrCrack::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string QfrCrack::name() const
{
  return m_impl->name();
}

void QfrCrack::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string QfrCrack::desc() const
{
  return m_impl->desc();
}

void QfrCrack::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double QfrCrack::a() const
{
  return m_impl->a();
}

bool QfrCrack::setA(const double a)
{
  return m_impl->setA(a);
}

bool QfrCrack::setA(const std::string &a)
{
  return m_impl->setA(a);
}

double QfrCrack::b() const
{
  return m_impl->b();
}

bool QfrCrack::setB(const double b)
{
  return m_impl->setB(b);
}

bool QfrCrack::setB(const std::string &b)
{
  return m_impl->setB(b);
}

double QfrCrack::length() const
{
  return m_impl->length();
}

bool QfrCrack::setLength(const double length)
{
  return m_impl->setLength(length);
}

bool QfrCrack::setLength(const std::string &length)
{
  return m_impl->setLength(length);
}

double QfrCrack::width() const
{
  return m_impl->width();
}

bool QfrCrack::setWidth(const double width)
{
  return m_impl->setWidth(width);
}

bool QfrCrack::setWidth(const std::string &width)
{
  return m_impl->setWidth(width);
}

double QfrCrack::depth() const
{
  return m_impl->depth();
}

bool QfrCrack::setDepth(const double depth)
{
  return m_impl->setDepth(depth);
}

bool QfrCrack::setDepth(const std::string &depth)
{
  return m_impl->setDepth(depth);
}

int QfrCrack::nB() const
{
  return m_impl->nB();
}

void QfrCrack::setNB(const int nB)
{
  m_impl->setNB(nB);
}

int QfrCrack::u_L() const
{
  return m_impl->u_L();
}

void QfrCrack::setU_L(const int u_L)
{
  m_impl->setU_L(u_L);
}

int QfrCrack::u_W() const
{
  return m_impl->u_W();
}

void QfrCrack::setU_W(const int u_W)
{
  m_impl->setU_W(u_W);
}

int QfrCrack::u_D() const
{
  return m_impl->u_D();
}

void QfrCrack::setU_D(const int u_D)
{
  m_impl->setU_D(u_D);
}

QfrTest2::QfrTest2()
{
  m_impl = boost::shared_ptr<detail::QfrTest2Impl>(new detail::QfrTest2Impl);
}

QfrTest2::QfrTest2(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::QfrTest2Impl>(new detail::QfrTest2Impl(nr,icon,name,desc));
}

QfrTest2::QfrTest2(int nr,int icon,std::string name,std::string desc,double a,double b,double dP1,double F1,
                   double dP2,double F2,int u_P1,int u_F1,int u_P2,int u_F2)
{
  m_impl = boost::shared_ptr<detail::QfrTest2Impl>(new detail::QfrTest2Impl(nr,icon,name,desc,a,b,dP1,F1,dP2,F2,u_P1,u_F1,u_P2,u_F2));
}

QfrTest2::QfrTest2(int nr,int icon,std::string name,std::string desc,std::string a,std::string b,std::string dP1,std::string F1,
                   std::string dP2,std::string F2,int u_P1,int u_F1,int u_P2,int u_F2)
{
  m_impl = boost::shared_ptr<detail::QfrTest2Impl>(new detail::QfrTest2Impl(nr,icon,name,desc,a,b,dP1,F1,dP2,F2,u_P1,u_F1,u_P2,u_F2));
}

QfrTest2::QfrTest2(const QfrTest2 &other) : m_impl(other.m_impl)
{}

QfrTest2::~QfrTest2()
{}

QfrTest2& QfrTest2::operator=(const QfrTest2 &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool QfrTest2::operator==(const QfrTest2 &other) const
{
  return m_impl == other.m_impl;
}

bool QfrTest2::operator!=(const QfrTest2 &other) const
{
  return m_impl != other.m_impl;
}

std::string QfrTest2::write()
{
  return m_impl->write();
}

void QfrTest2::read(Reader &input)
{
  m_impl->read(input);
}

void QfrTest2::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int QfrTest2::nr() const
{
  return m_impl->nr();
}

void QfrTest2::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int QfrTest2::icon() const
{
  return m_impl->icon();
}

void QfrTest2::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string QfrTest2::name() const
{
  return m_impl->name();
}

void QfrTest2::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string QfrTest2::desc() const
{
  return m_impl->desc();
}

void QfrTest2::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double QfrTest2::a() const
{
  return m_impl->a();
}

bool QfrTest2::setA(const double a)
{
  return m_impl->setA(a);
}

bool QfrTest2::setA(const std::string &a)
{
  return m_impl->setA(a);
}

double QfrTest2::b() const
{
  return m_impl->b();
}

bool QfrTest2::setB(const double b)
{
  return m_impl->setB(b);
}

bool QfrTest2::setB(const std::string &b)
{
  return m_impl->setB(b);
}

double QfrTest2::dP1() const
{
  return m_impl->dP1();
}

bool QfrTest2::setDP1(const double dP1)
{
  return m_impl->setDP1(dP1);
}

bool QfrTest2::setDP1(const std::string &dP1)
{
  return m_impl->setDP1(dP1);
}

double QfrTest2::F1() const
{
  return m_impl->F1();
}

bool QfrTest2::setF1(const double F1)
{
  return m_impl->setF1(F1);
}

bool QfrTest2::setF1(const std::string &F1)
{
  return m_impl->setF1(F1);
}

double QfrTest2::dP2() const
{
  return m_impl->dP2();
}

bool QfrTest2::setDP2(const double dP2)
{
  return m_impl->setDP2(dP2);
}

bool QfrTest2::setDP2(const std::string &dP2)
{
  return m_impl->setDP2(dP2);
}

double QfrTest2::F2() const
{
  return m_impl->F2();
}

bool QfrTest2::setF2(const double F2)
{
  return m_impl->setF2(F2);
}

bool QfrTest2::setF2(const std::string &F2)
{
  return m_impl->setF2(F2);
}

int QfrTest2::u_P1() const
{
  return m_impl->u_P1();
}

void QfrTest2::setU_P1(const int u_P1)
{
  m_impl->setU_P1(u_P1);
}

int QfrTest2::u_F1() const
{
  return m_impl->u_F1();
}

void QfrTest2::setU_F1(const int u_F1)
{
  m_impl->setU_F1(u_F1);
}

int QfrTest2::u_P2() const
{
  return m_impl->u_P2();
}

void QfrTest2::setU_P2(const int u_P2)
{
  m_impl->setU_P2(u_P2);
}

int QfrTest2::u_F2() const
{
  return m_impl->u_F2();
}

void QfrTest2::setU_F2(const int u_F2)
{
  m_impl->setU_F2(u_F2);
}

AfeDor::AfeDor()
{
  m_impl = boost::shared_ptr<detail::AfeDorImpl>(new detail::AfeDorImpl);
}

AfeDor::AfeDor(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::AfeDorImpl>(new detail::AfeDorImpl(nr,icon,name,desc));
}

AfeDor::AfeDor(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dTmin,
               double ht,double wd,double cd,int u_T,int u_H,int u_W)
{
  m_impl = boost::shared_ptr<detail::AfeDorImpl>(new detail::AfeDorImpl(nr,icon,name,desc,lam,turb,expt,dTmin,ht,wd,cd,u_T,u_H,u_W));
}

AfeDor::AfeDor(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dTmin,
               std::string ht,std::string wd,std::string cd,int u_T,int u_H,int u_W)
{
  m_impl = boost::shared_ptr<detail::AfeDorImpl>(new detail::AfeDorImpl(nr,icon,name,desc,lam,turb,expt,dTmin,ht,wd,cd,u_T,u_H,u_W));
}

AfeDor::AfeDor(const AfeDor &other) : m_impl(other.m_impl)
{}

AfeDor::~AfeDor()
{}

AfeDor& AfeDor::operator=(const AfeDor &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool AfeDor::operator==(const AfeDor &other) const
{
  return m_impl == other.m_impl;
}

bool AfeDor::operator!=(const AfeDor &other) const
{
  return m_impl != other.m_impl;
}

std::string AfeDor::write()
{
  return m_impl->write();
}

void AfeDor::read(Reader &input)
{
  m_impl->read(input);
}

void AfeDor::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int AfeDor::nr() const
{
  return m_impl->nr();
}

void AfeDor::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int AfeDor::icon() const
{
  return m_impl->icon();
}

void AfeDor::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string AfeDor::name() const
{
  return m_impl->name();
}

void AfeDor::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string AfeDor::desc() const
{
  return m_impl->desc();
}

void AfeDor::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double AfeDor::lam() const
{
  return m_impl->lam();
}

bool AfeDor::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool AfeDor::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double AfeDor::turb() const
{
  return m_impl->turb();
}

bool AfeDor::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool AfeDor::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double AfeDor::expt() const
{
  return m_impl->expt();
}

bool AfeDor::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool AfeDor::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double AfeDor::dTmin() const
{
  return m_impl->dTmin();
}

bool AfeDor::setDTmin(const double dTmin)
{
  return m_impl->setDTmin(dTmin);
}

bool AfeDor::setDTmin(const std::string &dTmin)
{
  return m_impl->setDTmin(dTmin);
}

double AfeDor::height() const
{
  return m_impl->height();
}

bool AfeDor::setHeight(const double ht)
{
  return m_impl->setHeight(ht);
}

bool AfeDor::setHeight(const std::string &ht)
{
  return m_impl->setHeight(ht);
}

double AfeDor::width() const
{
  return m_impl->width();
}

bool AfeDor::setWidth(const double wd)
{
  return m_impl->setWidth(wd);
}

bool AfeDor::setWidth(const std::string &wd)
{
  return m_impl->setWidth(wd);
}

double AfeDor::cd() const
{
  return m_impl->cd();
}

bool AfeDor::setCd(const double cd)
{
  return m_impl->setCd(cd);
}

bool AfeDor::setCd(const std::string &cd)
{
  return m_impl->setCd(cd);
}

int AfeDor::u_T() const
{
  return m_impl->u_T();
}

void AfeDor::setU_T(const int u_T)
{
  m_impl->setU_T(u_T);
}

int AfeDor::u_H() const
{
  return m_impl->u_H();
}

void AfeDor::setU_H(const int u_H)
{
  m_impl->setU_H(u_H);
}

int AfeDor::u_W() const
{
  return m_impl->u_W();
}

void AfeDor::setU_W(const int u_W)
{
  m_impl->setU_W(u_W);
}

DrPl2::DrPl2()
{
  m_impl = boost::shared_ptr<detail::DrPl2Impl>(new detail::DrPl2Impl);
}

DrPl2::DrPl2(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::DrPl2Impl>(new detail::DrPl2Impl(nr,icon,name,desc));
}

DrPl2::DrPl2(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dH,double ht,
             double wd,double cd,int u_H,int u_W)
{
  m_impl = boost::shared_ptr<detail::DrPl2Impl>(new detail::DrPl2Impl(nr,icon,name,desc,lam,turb,expt,dH,ht,wd,cd,u_H,u_W));
}

DrPl2::DrPl2(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dH,std::string ht,
             std::string wd,std::string cd,int u_H,int u_W)
{
  m_impl = boost::shared_ptr<detail::DrPl2Impl>(new detail::DrPl2Impl(nr,icon,name,desc,lam,turb,expt,dH,ht,wd,cd,u_H,u_W));
}

DrPl2::DrPl2(const DrPl2 &other) : m_impl(other.m_impl)
{}

DrPl2::~DrPl2()
{}

DrPl2& DrPl2::operator=(const DrPl2 &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool DrPl2::operator==(const DrPl2 &other) const
{
  return m_impl == other.m_impl;
}

bool DrPl2::operator!=(const DrPl2 &other) const
{
  return m_impl != other.m_impl;
}

std::string DrPl2::write()
{
  return m_impl->write();
}

void DrPl2::read(Reader &input)
{
  m_impl->read(input);
}

void DrPl2::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int DrPl2::nr() const
{
  return m_impl->nr();
}

void DrPl2::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int DrPl2::icon() const
{
  return m_impl->icon();
}

void DrPl2::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string DrPl2::name() const
{
  return m_impl->name();
}

void DrPl2::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string DrPl2::desc() const
{
  return m_impl->desc();
}

void DrPl2::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double DrPl2::lam() const
{
  return m_impl->lam();
}

bool DrPl2::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool DrPl2::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double DrPl2::turb() const
{
  return m_impl->turb();
}

bool DrPl2::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool DrPl2::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double DrPl2::expt() const
{
  return m_impl->expt();
}

bool DrPl2::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool DrPl2::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double DrPl2::dH() const
{
  return m_impl->dH();
}

bool DrPl2::setDH(const double dH)
{
  return m_impl->setDH(dH);
}

bool DrPl2::setDH(const std::string &dH)
{
  return m_impl->setDH(dH);
}

double DrPl2::height() const
{
  return m_impl->height();
}

bool DrPl2::setHeight(const double ht)
{
  return m_impl->setHeight(ht);
}

bool DrPl2::setHeight(const std::string &ht)
{
  return m_impl->setHeight(ht);
}

double DrPl2::width() const
{
  return m_impl->width();
}

bool DrPl2::setWidth(const double wd)
{
  return m_impl->setWidth(wd);
}

bool DrPl2::setWidth(const std::string &wd)
{
  return m_impl->setWidth(wd);
}

double DrPl2::cd() const
{
  return m_impl->cd();
}

bool DrPl2::setCd(const double cd)
{
  return m_impl->setCd(cd);
}

bool DrPl2::setCd(const std::string &cd)
{
  return m_impl->setCd(cd);
}

int DrPl2::u_H() const
{
  return m_impl->u_H();
}

void DrPl2::setU_H(const int u_H)
{
  m_impl->setU_H(u_H);
}

int DrPl2::u_W() const
{
  return m_impl->u_W();
}

void DrPl2::setU_W(const int u_W)
{
  m_impl->setU_W(u_W);
}

AfeFlow::AfeFlow()
{
  m_impl = boost::shared_ptr<detail::AfeFlowImpl>(new detail::AfeFlowImpl);
}

AfeFlow::AfeFlow(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::AfeFlowImpl>(new detail::AfeFlowImpl(nr,icon,name,desc));
}

AfeFlow::AfeFlow(int nr,int icon,std::string name,std::string desc,double Flow,int u_F)
{
  m_impl = boost::shared_ptr<detail::AfeFlowImpl>(new detail::AfeFlowImpl(nr,icon,name,desc,Flow,u_F));
}

AfeFlow::AfeFlow(int nr,int icon,std::string name,std::string desc,std::string Flow,int u_F)
{
  m_impl = boost::shared_ptr<detail::AfeFlowImpl>(new detail::AfeFlowImpl(nr,icon,name,desc,Flow,u_F));
}

AfeFlow::AfeFlow(const AfeFlow &other) : m_impl(other.m_impl)
{}

AfeFlow::~AfeFlow()
{}

AfeFlow& AfeFlow::operator=(const AfeFlow &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool AfeFlow::operator==(const AfeFlow &other) const
{
  return m_impl == other.m_impl;
}

bool AfeFlow::operator!=(const AfeFlow &other) const
{
  return m_impl != other.m_impl;
}

std::string AfeFlow::write()
{
  return m_impl->write(dataType());
}

void AfeFlow::read(Reader &input)
{
  m_impl->read(input);
}

void AfeFlow::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int AfeFlow::nr() const
{
  return m_impl->nr();
}

void AfeFlow::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int AfeFlow::icon() const
{
  return m_impl->icon();
}

void AfeFlow::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string AfeFlow::name() const
{
  return m_impl->name();
}

void AfeFlow::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string AfeFlow::desc() const
{
  return m_impl->desc();
}

void AfeFlow::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double AfeFlow::Flow() const
{
  return m_impl->Flow();
}

bool AfeFlow::setFlow(const double Flow)
{
  return m_impl->setFlow(Flow);
}

bool AfeFlow::setFlow(const std::string &Flow)
{
  return m_impl->setFlow(Flow);
}

int AfeFlow::u_F() const
{
  return m_impl->u_F();
}

void AfeFlow::setU_F(const int u_F)
{
  m_impl->setU_F(u_F);
}

AfeCmf::AfeCmf() : AfeFlow()
{}

AfeCmf::AfeCmf(int nr,int icon,std::string name,std::string desc) : AfeFlow(nr,icon,name,desc)
{}

AfeCmf::AfeCmf(int nr,int icon,std::string name,std::string desc,double Flow,int u_F)
: AfeFlow(nr,icon,name,desc,Flow,u_F)
{}

AfeCmf::AfeCmf(int nr,int icon,std::string name,std::string desc,std::string Flow,int u_F)
: AfeFlow(nr,icon,name,desc,Flow,u_F)
{}

AfeCmf::~AfeCmf()
{}

AfeCvf::AfeCvf() : AfeFlow()
{}

AfeCvf::AfeCvf(int nr,int icon,std::string name,std::string desc) : AfeFlow(nr,icon,name,desc)
{}

AfeCvf::AfeCvf(int nr,int icon,std::string name,std::string desc,double Flow,int u_F)
: AfeFlow(nr,icon,name,desc,Flow,u_F)
{}

AfeCvf::AfeCvf(int nr,int icon,std::string name,std::string desc,std::string Flow,int u_F)
: AfeFlow(nr,icon,name,desc,Flow,u_F)
{}

AfeCvf::~AfeCvf()
{}

AfeFan::AfeFan()
{
  m_impl = boost::shared_ptr<detail::AfeFanImpl>(new detail::AfeFanImpl);
}

AfeFan::AfeFan(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::AfeFanImpl>(new detail::AfeFanImpl(nr,icon,name,desc));
}

AfeFan::AfeFan(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double rdens,
               double fdf,double sop,double off,std::vector<double> fpc,double Sarea,int u_Sa,
               std::vector<FanDataPoint> data)
{
  m_impl = boost::shared_ptr<detail::AfeFanImpl>(new detail::AfeFanImpl(nr,icon,name,desc,lam,turb,expt,rdens,fdf,sop,off,fpc,Sarea,u_Sa,data));
}

AfeFan::AfeFan(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string rdens,
               std::string fdf,std::string sop,std::string off,std::vector<std::string> fpc,std::string Sarea,int u_Sa,
               std::vector<FanDataPoint> data)
{
  m_impl = boost::shared_ptr<detail::AfeFanImpl>(new detail::AfeFanImpl(nr,icon,name,desc,lam,turb,expt,rdens,fdf,sop,off,fpc,Sarea,u_Sa,data));
}

AfeFan::AfeFan(const AfeFan &other) : m_impl(other.m_impl)
{}

AfeFan::~AfeFan()
{}

AfeFan& AfeFan::operator=(const AfeFan &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool AfeFan::operator==(const AfeFan &other) const
{
  return m_impl == other.m_impl;
}

bool AfeFan::operator!=(const AfeFan &other) const
{
  return m_impl != other.m_impl;
}

std::string AfeFan::write()
{
  return m_impl->write();
}

void AfeFan::read(Reader &input)
{
  m_impl->read(input);
}

void AfeFan::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int AfeFan::nr() const
{
  return m_impl->nr();
}

void AfeFan::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int AfeFan::icon() const
{
  return m_impl->icon();
}

void AfeFan::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string AfeFan::name() const
{
  return m_impl->name();
}

void AfeFan::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string AfeFan::desc() const
{
  return m_impl->desc();
}

void AfeFan::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

double AfeFan::lam() const
{
  return m_impl->lam();
}

bool AfeFan::setLam(const double lam)
{
  return m_impl->setLam(lam);
}

bool AfeFan::setLam(const std::string &lam)
{
  return m_impl->setLam(lam);
}

double AfeFan::turb() const
{
  return m_impl->turb();
}

bool AfeFan::setTurb(const double turb)
{
  return m_impl->setTurb(turb);
}

bool AfeFan::setTurb(const std::string &turb)
{
  return m_impl->setTurb(turb);
}

double AfeFan::expt() const
{
  return m_impl->expt();
}

bool AfeFan::setExpt(const double expt)
{
  return m_impl->setExpt(expt);
}

bool AfeFan::setExpt(const std::string &expt)
{
  return m_impl->setExpt(expt);
}

double AfeFan::rdens() const
{
  return m_impl->rdens();
}

bool AfeFan::setRdens(const double rdens)
{
  return m_impl->setRdens(rdens);
}

bool AfeFan::setRdens(const std::string &rdens)
{
  return m_impl->setRdens(rdens);
}

double AfeFan::fdf() const
{
  return m_impl->fdf();
}

bool AfeFan::setFdf(const double fdf)
{
  return m_impl->setFdf(fdf);
}

bool AfeFan::setFdf(const std::string &fdf)
{
  return m_impl->setFdf(fdf);
}

double AfeFan::sop() const
{
  return m_impl->sop();
}

bool AfeFan::setSop(const double sop)
{
  return m_impl->setSop(sop);
}

bool AfeFan::setSop(const std::string &sop)
{
  return m_impl->setSop(sop);
}

double AfeFan::off() const
{
  return m_impl->off();
}

bool AfeFan::setOff(const double off)
{
  return m_impl->setOff(off);
}

bool AfeFan::setOff(const std::string &off)
{
  return m_impl->setOff(off);
}

std::vector<double> AfeFan::fpc() const
{
  return m_impl->fpc();
}

bool AfeFan::setFpc(const std::vector<double> &fpc)
{
  return m_impl->setFpc(fpc);
}

bool AfeFan::setFpc(const std::vector<std::string> &fpc)
{
  return m_impl->setFpc(fpc);
}

double AfeFan::Sarea() const
{
  return m_impl->Sarea();
}

bool AfeFan::setSarea(const double Sarea)
{
  return m_impl->setSarea(Sarea);
}

bool AfeFan::setSarea(const std::string &Sarea)
{
  return m_impl->setSarea(Sarea);
}

int AfeFan::u_Sa() const
{
  return m_impl->u_Sa();
}

void AfeFan::setU_Sa(const int u_Sa)
{
  m_impl->setU_Sa(u_Sa);
}

std::vector<FanDataPoint> AfeFan::data() const
{
  return m_impl->data();
}

void AfeFan::setData(const std::vector<FanDataPoint> &data)
{
  m_impl->setData(data);
}

AfeCsf::AfeCsf()
{
  m_impl = boost::shared_ptr<detail::AfeCsfImpl>(new detail::AfeCsfImpl());
}

AfeCsf::AfeCsf(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::AfeCsfImpl>(new detail::AfeCsfImpl(nr,icon,name,desc));
}

AfeCsf::AfeCsf(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data)
{
  m_impl = boost::shared_ptr<detail::AfeCsfImpl>(new detail::AfeCsfImpl(nr,icon,name,desc,u_x,u_y,data));
}

AfeCsf::AfeCsf(const AfeCsf &other) : m_impl(other.m_impl)
{}

AfeCsf::~AfeCsf()
{}

AfeCsf& AfeCsf::operator=(const AfeCsf &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool AfeCsf::operator==(const AfeCsf &other) const
{
  return m_impl == other.m_impl;
}

bool AfeCsf::operator!=(const AfeCsf &other) const
{
  return m_impl != other.m_impl;
}

std::string AfeCsf::write()
{
  return m_impl->write(dataType());
}

void AfeCsf::read(Reader &input)
{
  m_impl->read(input);
}

void AfeCsf::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int AfeCsf::nr() const
{
  return m_impl->nr();
}

void AfeCsf::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int AfeCsf::icon() const
{
  return m_impl->icon();
}

void AfeCsf::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string AfeCsf::name() const
{
  return m_impl->name();
}

void AfeCsf::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string AfeCsf::desc() const
{
  return m_impl->desc();
}

void AfeCsf::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

int AfeCsf::u_x() const
{
  return m_impl->u_x();
}

void AfeCsf::setU_x(const int u_x)
{
  m_impl->setU_x(u_x);
}

int AfeCsf::u_y() const
{
  return m_impl->u_y();
}

void AfeCsf::setU_y(const int u_y)
{
  m_impl->setU_y(u_y);
}

std::vector<XyDataPoint> AfeCsf::data() const
{
  return m_impl->data();
}

void AfeCsf::setData(const std::vector<XyDataPoint> &data)
{
  m_impl->setData(data);
}

AfeFsp::AfeFsp() : AfeCsf()
{}

AfeFsp::AfeFsp(int nr,int icon,std::string name,std::string desc) : AfeCsf(nr,icon,name,desc)
{}

AfeFsp::AfeFsp(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data)
: AfeCsf(nr,icon,name,desc,u_x,u_y,data)
{}

AfeFsp::~AfeFsp()
{}

AfeQsp::AfeQsp() : AfeCsf()
{}

AfeQsp::AfeQsp(int nr,int icon,std::string name,std::string desc) : AfeCsf(nr,icon,name,desc)
{}

AfeQsp::AfeQsp(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data)
: AfeCsf(nr,icon,name,desc,u_x,u_y,data)
{}

AfeQsp::~AfeQsp()
{}

AfePsf::AfePsf() : AfeCsf()
{}

AfePsf::AfePsf(int nr,int icon,std::string name,std::string desc) : AfeCsf(nr,icon,name,desc)
{}

AfePsf::AfePsf(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data)
: AfeCsf(nr,icon,name,desc,u_x,u_y,data)
{}

AfePsf::~AfePsf()
{}

AfePsq::AfePsq() : AfeCsf()
{}

AfePsq::AfePsq(int nr,int icon,std::string name,std::string desc) : AfeCsf(nr,icon,name,desc)
{}

AfePsq::AfePsq(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data)
: AfeCsf(nr,icon,name,desc,u_x,u_y,data)
{}

AfePsq::~AfePsq()
{}

AfeSup::AfeSup()
{
  m_impl = boost::shared_ptr<detail::AfeSupImpl>(new detail::AfeSupImpl);
}

AfeSup::AfeSup(int nr,int icon,std::string name,std::string desc)
{
  m_impl = boost::shared_ptr<detail::AfeSupImpl>(new detail::AfeSupImpl(nr,icon,name,desc));
}

AfeSup::AfeSup(int nr,int icon,std::string name,std::string desc,int sched,int u_H,
               std::vector<AirflowSubelementData> subelements)
{
  m_impl = boost::shared_ptr<detail::AfeSupImpl>(new detail::AfeSupImpl(nr,icon,name,desc,sched,u_H,subelements));
}

AfeSup::AfeSup(const AfeSup &other) : m_impl(other.m_impl)
{}

AfeSup::~AfeSup()
{}

AfeSup& AfeSup::operator=(const AfeSup &other)
{
  m_impl = other.m_impl;
  return *this;
}

bool AfeSup::operator==(const AfeSup &other) const
{
  return m_impl == other.m_impl;
}

bool AfeSup::operator!=(const AfeSup &other) const
{
  return m_impl != other.m_impl;
}

std::string AfeSup::write()
{
  return m_impl->write();
}

void AfeSup::read(Reader &input)
{
  m_impl->read(input);
}

void AfeSup::readDetails(Reader &input)
{
  m_impl->readDetails(input);
}

int AfeSup::nr() const
{
  return m_impl->nr();
}

void AfeSup::setNr(const int nr)
{
  m_impl->setNr(nr);
}

int AfeSup::icon() const
{
  return m_impl->icon();
}

void AfeSup::setIcon(const int icon)
{
  m_impl->setIcon(icon);
}

std::string AfeSup::name() const
{
  return m_impl->name();
}

void AfeSup::setName(const std::string &name)
{
  m_impl->setName(name);
}

std::string AfeSup::desc() const
{
  return m_impl->desc();
}

void AfeSup::setDesc(const std::string &desc)
{
  m_impl->setDesc(desc);
}

int AfeSup::sched() const
{
  return m_impl->sched();
}

void AfeSup::setSched(const int sched)
{
  m_impl->setSched(sched);
}

int AfeSup::u_H() const
{
  return m_impl->u_H();
}

void AfeSup::setU_H(const int u_H)
{
  m_impl->setU_H(u_H);
}

std::vector<AirflowSubelementData> AfeSup::subelements() const
{
  return m_impl->subelements();
}

void AfeSup::setSubelements(const std::vector<AirflowSubelementData> &subelements)
{
  m_impl->setSubelements(subelements);
}

} // contam
} // openstudio
