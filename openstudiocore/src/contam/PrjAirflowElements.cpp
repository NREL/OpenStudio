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
#include "PrjAirflowElements.hpp"

#include <utilities/core/Logger.hpp>

namespace openstudio {
namespace contam {

AirflowElement::Type AirflowElement::convertTag(std::string string)
{
  std::string tags[28] = {std::string("plr_orfc"),std::string("plr_leak1"),std::string("plr_leak2"),std::string("plr_leak3"),std::string("plr_conn"),std::string("plr_qcn"),std::string("plr_fcn"),std::string("plr_test1"),std::string("plr_test2"),std::string("plr_crack"),std::string("plr_stair"),std::string("plr_shaft"),std::string("plr_bdq"),std::string("plr_bdf"),std::string("qfr_qab"),std::string("qfr_fab"),std::string("qfr_crack"),std::string("qfr_test2"),std::string("dor_door"),std::string("dor_pl2"),std::string("fan_cmf"),std::string("fan_cvf"),std::string("fan_fan"),std::string("csf_fsp"),std::string("csf_qsp"),std::string("csf_psf"),std::string("csf_psq"),std::string("sup_afe")};
  AirflowElement::Type type[28] = {PL_ORFC,PL_LEAK1,PL_LEAK2,PL_LEAK3,PL_CONN,PL_QCN,PL_FCN,PL_TEST1,PL_TEST2,PL_CRACK,PL_STAIR,PL_SHAFT,PL_BDQ,PL_BDF,QFR_QAB,QFR_QAF,QFR_CRACK,QFR_TEST2,DR_DOOR,DR_PL2,FN_CMF,FN_CVF,FN_FAN,CS_FSP,CS_QSP,CS_PSF,CS_PSQ,AF_SUP};
  for(int i=0;i<28;i++)
    if(string==tags[i])
      return type[i];
  return AirflowElement::UNKNOWN;
}

AirflowElement* AirflowElement::readElement(Reader &input)
{
  AirflowElement *out=NULL;
  int nr = input.read<int>(FILELINE);
  int icon = input.read<int>(FILELINE);
  std::string dataType = input.readString(FILELINE);
  std::string name = input.readString(FILELINE);
  std::string desc = input.readLine(FILELINE);
  int kind = convertTag(dataType);
  switch(kind)
  {
  case AirflowElement::PL_ORFC:
    {
      PlrOrf *obj = new PlrOrf(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_LEAK1:
    {
      PlrLeak1 *obj = new PlrLeak1(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_LEAK2:
    {
      PlrLeak2 *obj = new PlrLeak2(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_LEAK3:
    {
      PlrLeak3 *obj = new PlrLeak3(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_CONN:
    {
      PlrConn *obj = new PlrConn(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_QCN:
    {
      PlrQcn *obj = new PlrQcn(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_FCN:
    {
      PlrFcn *obj = new PlrFcn(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_TEST1:
    {
      PlrTest1 *obj = new PlrTest1(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_TEST2:
    {
      PlrTest2 *obj = new PlrTest2(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_CRACK:
    {
      PlrCrack *obj = new PlrCrack(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_STAIR:
    {
      PlrStair *obj = new PlrStair(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_SHAFT:
    {
      PlrShaft *obj = new PlrShaft(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_BDQ:
    {
      PlrBdq *obj = new PlrBdq(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::PL_BDF:
    {
      PlrBdf *obj = new PlrBdf(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::QFR_QAB:
    {
      QfrQab *obj = new QfrQab(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::QFR_QAF:
    {
      QfrFab *obj = new QfrFab(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::QFR_CRACK:
    {
      QfrCrack *obj = new QfrCrack(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::QFR_TEST2:
    {
      QfrTest2 *obj = new QfrTest2(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::DR_DOOR:
    {
      AfeDor *obj = new AfeDor(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::DR_PL2:
    {
      DrPl2 *obj = new DrPl2(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::FN_CMF:
    {
      AfeCmf *obj = new AfeCmf(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::FN_CVF:
    {
      AfeCvf *obj = new AfeCvf(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::FN_FAN:
    {
      AfeFan *obj = new AfeFan(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::CS_FSP:
    {
      AfeFsp *obj = new AfeFsp(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::CS_QSP:
    {
      AfeQsp *obj = new AfeQsp(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::CS_PSF:
    {
      AfePsf *obj = new AfePsf(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::CS_PSQ:
    {
      AfePsq *obj = new AfePsq(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::AF_SUP:
    {
      AfeSup *obj = new AfeSup(nr,icon,name,desc);
      obj->readDetails(input);
      out = (AirflowElement*)obj;
      break;
    }
  case AirflowElement::UNKNOWN:
  default:
    std::string mesg = "Unknown airflow element type '" + dataType + "' at line " + openstudio::toString(input.lineNumber());
#ifndef NOFILELINE
    mesg +=  QString(" (%1,%2)").arg(__FILE__).arg(__LINE__).toStdString();
#endif
    LOG_FREE_AND_THROW("openstudio.contam.Reader",mesg);
  }
  return out;
}

PlrOrf::PlrOrf(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX area,RX dia,RX coef,RX Re,int u_A,int u_D)
{
  d = new PlrOrfPrivate(nr,icon,name,desc,lam,turb,expt,area,dia,coef,Re,u_A,u_D);
}

std::string PlrOrf::write()
{
  return d->write();
}

void PlrOrf::read(Reader &input)
{
  d->read(input);
}

void PlrOrf::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrOrf::nr() const
{
  return d->nr;
}

void PlrOrf::setNr(const int nr)
{
  d->nr = nr;
}

int PlrOrf::icon() const
{
  return d->icon;
}

void PlrOrf::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrOrf::name() const
{
  return d->name;
}

void PlrOrf::setName(const std::string name)
{
  d->name = name;
}

std::string PlrOrf::desc() const
{
  return d->desc;
}

void PlrOrf::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrOrf::lam() const
{
  return d->lam;
}

void PlrOrf::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrOrf::turb() const
{
  return d->turb;
}

void PlrOrf::setTurb(const RX turb)
{
  d->turb = turb;
}

RX PlrOrf::expt() const
{
  return d->expt;
}

void PlrOrf::setExpt(const RX expt)
{
  d->expt = expt;
}

RX PlrOrf::area() const
{
  return d->area;
}

void PlrOrf::setArea(const RX area)
{
  d->area = area;
}

RX PlrOrf::dia() const
{
  return d->dia;
}

void PlrOrf::setDia(const RX dia)
{
  d->dia = dia;
}

RX PlrOrf::coef() const
{
  return d->coef;
}

void PlrOrf::setCoef(const RX coef)
{
  d->coef = coef;
}

RX PlrOrf::Re() const
{
  return d->Re;
}

void PlrOrf::setRe(const RX Re)
{
  d->Re = Re;
}

int PlrOrf::u_A() const
{
  return d->u_A;
}

void PlrOrf::setU_A(const int u_A)
{
  d->u_A = u_A;
}

int PlrOrf::u_D() const
{
  return d->u_D;
}

void PlrOrf::setU_D(const int u_D)
{
  d->u_D = u_D;
}


PlrLeak::PlrLeak(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX coef,RX pres,RX area1,RX area2,RX area3,int u_A1,int u_A2,int u_A3,int u_dP)
{
  d = new PlrLeakPrivate(nr,icon,name,desc,lam,turb,expt,coef,pres,area1,area2,area3,u_A1,u_A2,u_A3,u_dP);
}

std::string PlrLeak::write()
{
  return d->write(dataType());
}

void PlrLeak::read(Reader &input)
{
  d->read(input);
}

void PlrLeak::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrLeak::nr() const
{
  return d->nr;
}

void PlrLeak::setNr(const int nr)
{
  d->nr = nr;
}

int PlrLeak::icon() const
{
  return d->icon;
}

void PlrLeak::setIcon(const int icon)
{
  d->icon = icon;
}

//std::string PlrLeak::dataType() const
//{
//    return d->dataType;
//}

std::string PlrLeak::name() const
{
  return d->name;
}

void PlrLeak::setName(const std::string name)
{
  d->name = name;
}

std::string PlrLeak::desc() const
{
  return d->desc;
}

void PlrLeak::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrLeak::lam() const
{
  return d->lam;
}

void PlrLeak::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrLeak::turb() const
{
  return d->turb;
}

void PlrLeak::setTurb(const RX turb)
{
  d->turb = turb;
}

RX PlrLeak::expt() const
{
  return d->expt;
}

void PlrLeak::setExpt(const RX expt)
{
  d->expt = expt;
}

RX PlrLeak::coef() const
{
  return d->coef;
}

void PlrLeak::setCoef(const RX coef)
{
  d->coef = coef;
}

RX PlrLeak::pres() const
{
  return d->pres;
}

void PlrLeak::setPres(const RX pres)
{
  d->pres = pres;
}

RX PlrLeak::area1() const
{
  return d->area1;
}

void PlrLeak::setArea1(const RX area1)
{
  d->area1 = area1;
}

RX PlrLeak::area2() const
{
  return d->area2;
}

void PlrLeak::setArea2(const RX area2)
{
  d->area2 = area2;
}

RX PlrLeak::area3() const
{
  return d->area3;
}

void PlrLeak::setArea3(const RX area3)
{
  d->area3 = area3;
}

int PlrLeak::u_A1() const
{
  return d->u_A1;
}

void PlrLeak::setU_A1(const int u_A1)
{
  d->u_A1 = u_A1;
}

int PlrLeak::u_A2() const
{
  return d->u_A2;
}

void PlrLeak::setU_A2(const int u_A2)
{
  d->u_A2 = u_A2;
}

int PlrLeak::u_A3() const
{
  return d->u_A3;
}

void PlrLeak::setU_A3(const int u_A3)
{
  d->u_A3 = u_A3;
}

int PlrLeak::u_dP() const
{
  return d->u_dP;
}

void PlrLeak::setU_dP(const int u_dP)
{
  d->u_dP = u_dP;
}


PlrConn::PlrConn(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX area,RX coef,int u_A)
{
  d = new PlrConnPrivate(nr,icon,name,desc,lam,turb,expt,area,coef,u_A);
}

std::string PlrConn::write()
{
  return d->write();
}

void PlrConn::read(Reader &input)
{
  d->read(input);
}

void PlrConn::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrConn::nr() const
{
  return d->nr;
}

void PlrConn::setNr(const int nr)
{
  d->nr = nr;
}

int PlrConn::icon() const
{
  return d->icon;
}

void PlrConn::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrConn::name() const
{
  return d->name;
}

void PlrConn::setName(const std::string name)
{
  d->name = name;
}

std::string PlrConn::desc() const
{
  return d->desc;
}

void PlrConn::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrConn::lam() const
{
  return d->lam;
}

void PlrConn::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrConn::turb() const
{
  return d->turb;
}

void PlrConn::setTurb(const RX turb)
{
  d->turb = turb;
}

RX PlrConn::expt() const
{
  return d->expt;
}

void PlrConn::setExpt(const RX expt)
{
  d->expt = expt;
}

RX PlrConn::area() const
{
  return d->area;
}

void PlrConn::setArea(const RX area)
{
  d->area = area;
}

RX PlrConn::coef() const
{
  return d->coef;
}

void PlrConn::setCoef(const RX coef)
{
  d->coef = coef;
}

int PlrConn::u_A() const
{
  return d->u_A;
}

void PlrConn::setU_A(const int u_A)
{
  d->u_A = u_A;
}


PlrQcn::PlrQcn(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt)
{
  d = new PlrQcnPrivate(nr,icon,name,desc,lam,turb,expt);
}

std::string PlrQcn::write()
{
  return d->write();
}

void PlrQcn::read(Reader &input)
{
  d->read(input);
}

void PlrQcn::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrQcn::nr() const
{
  return d->nr;
}

void PlrQcn::setNr(const int nr)
{
  d->nr = nr;
}

int PlrQcn::icon() const
{
  return d->icon;
}

void PlrQcn::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrQcn::name() const
{
  return d->name;
}

void PlrQcn::setName(const std::string name)
{
  d->name = name;
}

std::string PlrQcn::desc() const
{
  return d->desc;
}

void PlrQcn::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrQcn::lam() const
{
  return d->lam;
}

void PlrQcn::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrQcn::turb() const
{
  return d->turb;
}

void PlrQcn::setTurb(const RX turb)
{
  d->turb = turb;
}

RX PlrQcn::expt() const
{
  return d->expt;
}

void PlrQcn::setExpt(const RX expt)
{
  d->expt = expt;
}


PlrFcn::PlrFcn(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt)
{
  d = new PlrFcnPrivate(nr,icon,name,desc,lam,turb,expt);
}

std::string PlrFcn::write()
{
  return d->write();
}

void PlrFcn::read(Reader &input)
{
  d->read(input);
}

void PlrFcn::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrFcn::nr() const
{
  return d->nr;
}

void PlrFcn::setNr(const int nr)
{
  d->nr = nr;
}

int PlrFcn::icon() const
{
  return d->icon;
}

void PlrFcn::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrFcn::name() const
{
  return d->name;
}

void PlrFcn::setName(const std::string name)
{
  d->name = name;
}

std::string PlrFcn::desc() const
{
  return d->desc;
}

void PlrFcn::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrFcn::lam() const
{
  return d->lam;
}

void PlrFcn::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrFcn::turb() const
{
  return d->turb;
}

void PlrFcn::setTurb(const RX turb)
{
  d->turb = turb;
}

RX PlrFcn::expt() const
{
  return d->expt;
}

void PlrFcn::setExpt(const RX expt)
{
  d->expt = expt;
}


PlrTest1::PlrTest1(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX dP,RX Flow,int u_P,int u_F)
{
  d = new PlrTest1Private(nr,icon,name,desc,lam,turb,expt,dP,Flow,u_P,u_F);
}

std::string PlrTest1::write()
{
  return d->write();
}

void PlrTest1::read(Reader &input)
{
  d->read(input);
}

void PlrTest1::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrTest1::nr() const
{
  return d->nr;
}

void PlrTest1::setNr(const int nr)
{
  d->nr = nr;
}

int PlrTest1::icon() const
{
  return d->icon;
}

void PlrTest1::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrTest1::name() const
{
  return d->name;
}

void PlrTest1::setName(const std::string name)
{
  d->name = name;
}

std::string PlrTest1::desc() const
{
  return d->desc;
}

void PlrTest1::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrTest1::lam() const
{
  return d->lam;
}

void PlrTest1::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrTest1::turb() const
{
  return d->turb;
}

void PlrTest1::setTurb(const RX turb)
{
  d->turb = turb;
}

RX PlrTest1::expt() const
{
  return d->expt;
}

void PlrTest1::setExpt(const RX expt)
{
  d->expt = expt;
}

RX PlrTest1::dP() const
{
  return d->dP;
}

void PlrTest1::setDP(const RX dP)
{
  d->dP = dP;
}

RX PlrTest1::Flow() const
{
  return d->Flow;
}

void PlrTest1::setFlow(const RX Flow)
{
  d->Flow = Flow;
}

int PlrTest1::u_P() const
{
  return d->u_P;
}

void PlrTest1::setU_P(const int u_P)
{
  d->u_P = u_P;
}

int PlrTest1::u_F() const
{
  return d->u_F;
}

void PlrTest1::setU_F(const int u_F)
{
  d->u_F = u_F;
}


PlrTest2::PlrTest2(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX dP1,RX F1,RX dP2,RX F2,int u_P1,int u_F1,int u_P2,int u_F2)
{
  d = new PlrTest2Private(nr,icon,name,desc,lam,turb,expt,dP1,F1,dP2,F2,u_P1,u_F1,u_P2,u_F2);
}

std::string PlrTest2::write()
{
  return d->write();
}

void PlrTest2::read(Reader &input)
{
  d->read(input);
}

void PlrTest2::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrTest2::nr() const
{
  return d->nr;
}

void PlrTest2::setNr(const int nr)
{
  d->nr = nr;
}

int PlrTest2::icon() const
{
  return d->icon;
}

void PlrTest2::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrTest2::name() const
{
  return d->name;
}

void PlrTest2::setName(const std::string name)
{
  d->name = name;
}

std::string PlrTest2::desc() const
{
  return d->desc;
}

void PlrTest2::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrTest2::lam() const
{
  return d->lam;
}

void PlrTest2::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrTest2::turb() const
{
  return d->turb;
}

void PlrTest2::setTurb(const RX turb)
{
  d->turb = turb;
}

RX PlrTest2::expt() const
{
  return d->expt;
}

void PlrTest2::setExpt(const RX expt)
{
  d->expt = expt;
}

RX PlrTest2::dP1() const
{
  return d->dP1;
}

void PlrTest2::setDP1(const RX dP1)
{
  d->dP1 = dP1;
}

RX PlrTest2::F1() const
{
  return d->F1;
}

void PlrTest2::setF1(const RX F1)
{
  d->F1 = F1;
}

RX PlrTest2::dP2() const
{
  return d->dP2;
}

void PlrTest2::setDP2(const RX dP2)
{
  d->dP2 = dP2;
}

RX PlrTest2::F2() const
{
  return d->F2;
}

void PlrTest2::setF2(const RX F2)
{
  d->F2 = F2;
}

int PlrTest2::u_P1() const
{
  return d->u_P1;
}

void PlrTest2::setU_P1(const int u_P1)
{
  d->u_P1 = u_P1;
}

int PlrTest2::u_F1() const
{
  return d->u_F1;
}

void PlrTest2::setU_F1(const int u_F1)
{
  d->u_F1 = u_F1;
}

int PlrTest2::u_P2() const
{
  return d->u_P2;
}

void PlrTest2::setU_P2(const int u_P2)
{
  d->u_P2 = u_P2;
}

int PlrTest2::u_F2() const
{
  return d->u_F2;
}

void PlrTest2::setU_F2(const int u_F2)
{
  d->u_F2 = u_F2;
}


PlrCrack::PlrCrack(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX length,RX width,int u_L,int u_W)
{
  d = new PlrCrackPrivate(nr,icon,name,desc,lam,turb,expt,length,width,u_L,u_W);
}

std::string PlrCrack::write()
{
  return d->write();
}

void PlrCrack::read(Reader &input)
{
  d->read(input);
}

void PlrCrack::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrCrack::nr() const
{
  return d->nr;
}

void PlrCrack::setNr(const int nr)
{
  d->nr = nr;
}

int PlrCrack::icon() const
{
  return d->icon;
}

void PlrCrack::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrCrack::name() const
{
  return d->name;
}

void PlrCrack::setName(const std::string name)
{
  d->name = name;
}

std::string PlrCrack::desc() const
{
  return d->desc;
}

void PlrCrack::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrCrack::lam() const
{
  return d->lam;
}

void PlrCrack::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrCrack::turb() const
{
  return d->turb;
}

void PlrCrack::setTurb(const RX turb)
{
  d->turb = turb;
}

RX PlrCrack::expt() const
{
  return d->expt;
}

void PlrCrack::setExpt(const RX expt)
{
  d->expt = expt;
}

RX PlrCrack::length() const
{
  return d->length;
}

void PlrCrack::setLength(const RX length)
{
  d->length = length;
}

RX PlrCrack::width() const
{
  return d->width;
}

void PlrCrack::setWidth(const RX width)
{
  d->width = width;
}

int PlrCrack::u_L() const
{
  return d->u_L;
}

void PlrCrack::setU_L(const int u_L)
{
  d->u_L = u_L;
}

int PlrCrack::u_W() const
{
  return d->u_W;
}

void PlrCrack::setU_W(const int u_W)
{
  d->u_W = u_W;
}


PlrStair::PlrStair(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX Ht,RX Area,RX peo,int tread,int u_A,int u_D)
{
  d = new PlrStairPrivate(nr,icon,name,desc,lam,turb,expt,Ht,Area,peo,tread,u_A,u_D);
}

std::string PlrStair::write()
{
  return d->write();
}

void PlrStair::read(Reader &input)
{
  d->read(input);
}

void PlrStair::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrStair::nr() const
{
  return d->nr;
}

void PlrStair::setNr(const int nr)
{
  d->nr = nr;
}

int PlrStair::icon() const
{
  return d->icon;
}

void PlrStair::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrStair::name() const
{
  return d->name;
}

void PlrStair::setName(const std::string name)
{
  d->name = name;
}

std::string PlrStair::desc() const
{
  return d->desc;
}

void PlrStair::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrStair::lam() const
{
  return d->lam;
}

void PlrStair::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrStair::turb() const
{
  return d->turb;
}

void PlrStair::setTurb(const RX turb)
{
  d->turb = turb;
}

RX PlrStair::expt() const
{
  return d->expt;
}

void PlrStair::setExpt(const RX expt)
{
  d->expt = expt;
}

RX PlrStair::Ht() const
{
  return d->Ht;
}

void PlrStair::setHt(const RX Ht)
{
  d->Ht = Ht;
}

RX PlrStair::Area() const
{
  return d->Area;
}

void PlrStair::setArea(const RX Area)
{
  d->Area = Area;
}

RX PlrStair::peo() const
{
  return d->peo;
}

void PlrStair::setPeo(const RX peo)
{
  d->peo = peo;
}

int PlrStair::tread() const
{
  return d->tread;
}

void PlrStair::setTread(const int tread)
{
  d->tread = tread;
}

int PlrStair::u_A() const
{
  return d->u_A;
}

void PlrStair::setU_A(const int u_A)
{
  d->u_A = u_A;
}

int PlrStair::u_D() const
{
  return d->u_D;
}

void PlrStair::setU_D(const int u_D)
{
  d->u_D = u_D;
}


PlrShaft::PlrShaft(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX Ht,RX area,RX perim,RX rough,int u_A,int u_D,int u_P,int u_R)
{
  d = new PlrShaftPrivate(nr,icon,name,desc,lam,turb,expt,Ht,area,perim,rough,u_A,u_D,u_P,u_R);
}

std::string PlrShaft::write()
{
  return d->write();
}

void PlrShaft::read(Reader &input)
{
  d->read(input);
}

void PlrShaft::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrShaft::nr() const
{
  return d->nr;
}

void PlrShaft::setNr(const int nr)
{
  d->nr = nr;
}

int PlrShaft::icon() const
{
  return d->icon;
}

void PlrShaft::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrShaft::name() const
{
  return d->name;
}

void PlrShaft::setName(const std::string name)
{
  d->name = name;
}

std::string PlrShaft::desc() const
{
  return d->desc;
}

void PlrShaft::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrShaft::lam() const
{
  return d->lam;
}

void PlrShaft::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrShaft::turb() const
{
  return d->turb;
}

void PlrShaft::setTurb(const RX turb)
{
  d->turb = turb;
}

RX PlrShaft::expt() const
{
  return d->expt;
}

void PlrShaft::setExpt(const RX expt)
{
  d->expt = expt;
}

RX PlrShaft::Ht() const
{
  return d->Ht;
}

void PlrShaft::setHt(const RX Ht)
{
  d->Ht = Ht;
}

RX PlrShaft::area() const
{
  return d->area;
}

void PlrShaft::setArea(const RX area)
{
  d->area = area;
}

RX PlrShaft::perim() const
{
  return d->perim;
}

void PlrShaft::setPerim(const RX perim)
{
  d->perim = perim;
}

RX PlrShaft::rough() const
{
  return d->rough;
}

void PlrShaft::setRough(const RX rough)
{
  d->rough = rough;
}

int PlrShaft::u_A() const
{
  return d->u_A;
}

void PlrShaft::setU_A(const int u_A)
{
  d->u_A = u_A;
}

int PlrShaft::u_D() const
{
  return d->u_D;
}

void PlrShaft::setU_D(const int u_D)
{
  d->u_D = u_D;
}

int PlrShaft::u_P() const
{
  return d->u_P;
}

void PlrShaft::setU_P(const int u_P)
{
  d->u_P = u_P;
}

int PlrShaft::u_R() const
{
  return d->u_R;
}

void PlrShaft::setU_R(const int u_R)
{
  d->u_R = u_R;
}


PlrBdq::PlrBdq(int nr,int icon,std::string name,std::string desc,RX lam,RX Cp,RX xp,RX Cn,RX xn)
{
  d = new PlrBdqPrivate(nr,icon,name,desc,lam,Cp,xp,Cn,xn);
}

std::string PlrBdq::write()
{
  return d->write();
}

void PlrBdq::read(Reader &input)
{
  d->read(input);
}

void PlrBdq::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrBdq::nr() const
{
  return d->nr;
}

void PlrBdq::setNr(const int nr)
{
  d->nr = nr;
}

int PlrBdq::icon() const
{
  return d->icon;
}

void PlrBdq::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrBdq::name() const
{
  return d->name;
}

void PlrBdq::setName(const std::string name)
{
  d->name = name;
}

std::string PlrBdq::desc() const
{
  return d->desc;
}

void PlrBdq::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrBdq::lam() const
{
  return d->lam;
}

void PlrBdq::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrBdq::Cp() const
{
  return d->Cp;
}

void PlrBdq::setCp(const RX Cp)
{
  d->Cp = Cp;
}

RX PlrBdq::xp() const
{
  return d->xp;
}

void PlrBdq::setXp(const RX xp)
{
  d->xp = xp;
}

RX PlrBdq::Cn() const
{
  return d->Cn;
}

void PlrBdq::setCn(const RX Cn)
{
  d->Cn = Cn;
}

RX PlrBdq::xn() const
{
  return d->xn;
}

void PlrBdq::setXn(const RX xn)
{
  d->xn = xn;
}


PlrBdf::PlrBdf(int nr,int icon,std::string name,std::string desc,RX lam,RX Cp,RX xp,RX Cn,RX xn)
{
  d = new PlrBdfPrivate(nr,icon,name,desc,lam,Cp,xp,Cn,xn);
}

std::string PlrBdf::write()
{
  return d->write();
}

void PlrBdf::read(Reader &input)
{
  d->read(input);
}

void PlrBdf::readDetails(Reader &input)
{
  d->readDetails(input);
}

int PlrBdf::nr() const
{
  return d->nr;
}

void PlrBdf::setNr(const int nr)
{
  d->nr = nr;
}

int PlrBdf::icon() const
{
  return d->icon;
}

void PlrBdf::setIcon(const int icon)
{
  d->icon = icon;
}

std::string PlrBdf::name() const
{
  return d->name;
}

void PlrBdf::setName(const std::string name)
{
  d->name = name;
}

std::string PlrBdf::desc() const
{
  return d->desc;
}

void PlrBdf::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX PlrBdf::lam() const
{
  return d->lam;
}

void PlrBdf::setLam(const RX lam)
{
  d->lam = lam;
}

RX PlrBdf::Cp() const
{
  return d->Cp;
}

void PlrBdf::setCp(const RX Cp)
{
  d->Cp = Cp;
}

RX PlrBdf::xp() const
{
  return d->xp;
}

void PlrBdf::setXp(const RX xp)
{
  d->xp = xp;
}

RX PlrBdf::Cn() const
{
  return d->Cn;
}

void PlrBdf::setCn(const RX Cn)
{
  d->Cn = Cn;
}

RX PlrBdf::xn() const
{
  return d->xn;
}

void PlrBdf::setXn(const RX xn)
{
  d->xn = xn;
}


QfrQab::QfrQab(int nr,int icon,std::string name,std::string desc,RX a,RX b)
{
  d = new QfrQabPrivate(nr,icon,name,desc,a,b);
}

std::string QfrQab::write()
{
  return d->write();
}

void QfrQab::read(Reader &input)
{
  d->read(input);
}

void QfrQab::readDetails(Reader &input)
{
  d->readDetails(input);
}

int QfrQab::nr() const
{
  return d->nr;
}

void QfrQab::setNr(const int nr)
{
  d->nr = nr;
}

int QfrQab::icon() const
{
  return d->icon;
}

void QfrQab::setIcon(const int icon)
{
  d->icon = icon;
}

std::string QfrQab::name() const
{
  return d->name;
}

void QfrQab::setName(const std::string name)
{
  d->name = name;
}

std::string QfrQab::desc() const
{
  return d->desc;
}

void QfrQab::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX QfrQab::a() const
{
  return d->a;
}

void QfrQab::setA(const RX a)
{
  d->a = a;
}

RX QfrQab::b() const
{
  return d->b;
}

void QfrQab::setB(const RX b)
{
  d->b = b;
}


QfrFab::QfrFab(int nr,int icon,std::string name,std::string desc,RX a,RX b)
{
  d = new QfrFabPrivate(nr,icon,name,desc,a,b);
}

std::string QfrFab::write()
{
  return d->write();
}

void QfrFab::read(Reader &input)
{
  d->read(input);
}

void QfrFab::readDetails(Reader &input)
{
  d->readDetails(input);
}

int QfrFab::nr() const
{
  return d->nr;
}

void QfrFab::setNr(const int nr)
{
  d->nr = nr;
}

int QfrFab::icon() const
{
  return d->icon;
}

void QfrFab::setIcon(const int icon)
{
  d->icon = icon;
}

std::string QfrFab::name() const
{
  return d->name;
}

void QfrFab::setName(const std::string name)
{
  d->name = name;
}

std::string QfrFab::desc() const
{
  return d->desc;
}

void QfrFab::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX QfrFab::a() const
{
  return d->a;
}

void QfrFab::setA(const RX a)
{
  d->a = a;
}

RX QfrFab::b() const
{
  return d->b;
}

void QfrFab::setB(const RX b)
{
  d->b = b;
}


QfrCrack::QfrCrack(int nr,int icon,std::string name,std::string desc,RX a,RX b,RX length,RX width,RX depth,int nB,int u_L,int u_W,int u_D)
{
  d = new QfrCrackPrivate(nr,icon,name,desc,a,b,length,width,depth,nB,u_L,u_W,u_D);
}

std::string QfrCrack::write()
{
  return d->write();
}

void QfrCrack::read(Reader &input)
{
  d->read(input);
}

void QfrCrack::readDetails(Reader &input)
{
  d->readDetails(input);
}

int QfrCrack::nr() const
{
  return d->nr;
}

void QfrCrack::setNr(const int nr)
{
  d->nr = nr;
}

int QfrCrack::icon() const
{
  return d->icon;
}

void QfrCrack::setIcon(const int icon)
{
  d->icon = icon;
}

std::string QfrCrack::name() const
{
  return d->name;
}

void QfrCrack::setName(const std::string name)
{
  d->name = name;
}

std::string QfrCrack::desc() const
{
  return d->desc;
}

void QfrCrack::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX QfrCrack::a() const
{
  return d->a;
}

void QfrCrack::setA(const RX a)
{
  d->a = a;
}

RX QfrCrack::b() const
{
  return d->b;
}

void QfrCrack::setB(const RX b)
{
  d->b = b;
}

RX QfrCrack::length() const
{
  return d->length;
}

void QfrCrack::setLength(const RX length)
{
  d->length = length;
}

RX QfrCrack::width() const
{
  return d->width;
}

void QfrCrack::setWidth(const RX width)
{
  d->width = width;
}

RX QfrCrack::depth() const
{
  return d->depth;
}

void QfrCrack::setDepth(const RX depth)
{
  d->depth = depth;
}

int QfrCrack::nB() const
{
  return d->nB;
}

void QfrCrack::setNB(const int nB)
{
  d->nB = nB;
}

int QfrCrack::u_L() const
{
  return d->u_L;
}

void QfrCrack::setU_L(const int u_L)
{
  d->u_L = u_L;
}

int QfrCrack::u_W() const
{
  return d->u_W;
}

void QfrCrack::setU_W(const int u_W)
{
  d->u_W = u_W;
}

int QfrCrack::u_D() const
{
  return d->u_D;
}

void QfrCrack::setU_D(const int u_D)
{
  d->u_D = u_D;
}


QfrTest2::QfrTest2(int nr,int icon,std::string name,std::string desc,RX a,RX b,RX dP1,RX F1,RX dP2,RX F2,int u_P1,int u_F1,int u_P2,int u_F2)
{
  d = new QfrTest2Private(nr,icon,name,desc,a,b,dP1,F1,dP2,F2,u_P1,u_F1,u_P2,u_F2);
}

std::string QfrTest2::write()
{
  return d->write();
}

void QfrTest2::read(Reader &input)
{
  d->read(input);
}

void QfrTest2::readDetails(Reader &input)
{
  d->readDetails(input);
}

int QfrTest2::nr() const
{
  return d->nr;
}

void QfrTest2::setNr(const int nr)
{
  d->nr = nr;
}

int QfrTest2::icon() const
{
  return d->icon;
}

void QfrTest2::setIcon(const int icon)
{
  d->icon = icon;
}

std::string QfrTest2::name() const
{
  return d->name;
}

void QfrTest2::setName(const std::string name)
{
  d->name = name;
}

std::string QfrTest2::desc() const
{
  return d->desc;
}

void QfrTest2::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX QfrTest2::a() const
{
  return d->a;
}

void QfrTest2::setA(const RX a)
{
  d->a = a;
}

RX QfrTest2::b() const
{
  return d->b;
}

void QfrTest2::setB(const RX b)
{
  d->b = b;
}

RX QfrTest2::dP1() const
{
  return d->dP1;
}

void QfrTest2::setDP1(const RX dP1)
{
  d->dP1 = dP1;
}

RX QfrTest2::F1() const
{
  return d->F1;
}

void QfrTest2::setF1(const RX F1)
{
  d->F1 = F1;
}

RX QfrTest2::dP2() const
{
  return d->dP2;
}

void QfrTest2::setDP2(const RX dP2)
{
  d->dP2 = dP2;
}

RX QfrTest2::F2() const
{
  return d->F2;
}

void QfrTest2::setF2(const RX F2)
{
  d->F2 = F2;
}

int QfrTest2::u_P1() const
{
  return d->u_P1;
}

void QfrTest2::setU_P1(const int u_P1)
{
  d->u_P1 = u_P1;
}

int QfrTest2::u_F1() const
{
  return d->u_F1;
}

void QfrTest2::setU_F1(const int u_F1)
{
  d->u_F1 = u_F1;
}

int QfrTest2::u_P2() const
{
  return d->u_P2;
}

void QfrTest2::setU_P2(const int u_P2)
{
  d->u_P2 = u_P2;
}

int QfrTest2::u_F2() const
{
  return d->u_F2;
}

void QfrTest2::setU_F2(const int u_F2)
{
  d->u_F2 = u_F2;
}


AfeDor::AfeDor(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX dTmin,RX ht,RX wd,RX cd,int u_T,int u_H,int u_W)
{
  d = new AfeDorPrivate(nr,icon,name,desc,lam,turb,expt,dTmin,ht,wd,cd,u_T,u_H,u_W);
}

std::string AfeDor::write()
{
  return d->write();
}

void AfeDor::read(Reader &input)
{
  d->read(input);
}

void AfeDor::readDetails(Reader &input)
{
  d->readDetails(input);
}

int AfeDor::nr() const
{
  return d->nr;
}

void AfeDor::setNr(const int nr)
{
  d->nr = nr;
}

int AfeDor::icon() const
{
  return d->icon;
}

void AfeDor::setIcon(const int icon)
{
  d->icon = icon;
}

std::string AfeDor::name() const
{
  return d->name;
}

void AfeDor::setName(const std::string name)
{
  d->name = name;
}

std::string AfeDor::desc() const
{
  return d->desc;
}

void AfeDor::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX AfeDor::lam() const
{
  return d->lam;
}

void AfeDor::setLam(const RX lam)
{
  d->lam = lam;
}

RX AfeDor::turb() const
{
  return d->turb;
}

void AfeDor::setTurb(const RX turb)
{
  d->turb = turb;
}

RX AfeDor::expt() const
{
  return d->expt;
}

void AfeDor::setExpt(const RX expt)
{
  d->expt = expt;
}

RX AfeDor::dTmin() const
{
  return d->dTmin;
}

void AfeDor::setDTmin(const RX dTmin)
{
  d->dTmin = dTmin;
}

RX AfeDor::ht() const
{
  return d->ht;
}

void AfeDor::setHt(const RX ht)
{
  d->ht = ht;
}

RX AfeDor::wd() const
{
  return d->wd;
}

void AfeDor::setWd(const RX wd)
{
  d->wd = wd;
}

RX AfeDor::cd() const
{
  return d->cd;
}

void AfeDor::setCd(const RX cd)
{
  d->cd = cd;
}

int AfeDor::u_T() const
{
  return d->u_T;
}

void AfeDor::setU_T(const int u_T)
{
  d->u_T = u_T;
}

int AfeDor::u_H() const
{
  return d->u_H;
}

void AfeDor::setU_H(const int u_H)
{
  d->u_H = u_H;
}

int AfeDor::u_W() const
{
  return d->u_W;
}

void AfeDor::setU_W(const int u_W)
{
  d->u_W = u_W;
}


DrPl2::DrPl2(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX dH,RX ht,RX wd,RX cd,int u_H,int u_W)
{
  d = new DrPl2Private(nr,icon,name,desc,lam,turb,expt,dH,ht,wd,cd,u_H,u_W);
}

std::string DrPl2::write()
{
  return d->write();
}

void DrPl2::read(Reader &input)
{
  d->read(input);
}

void DrPl2::readDetails(Reader &input)
{
  d->readDetails(input);
}

int DrPl2::nr() const
{
  return d->nr;
}

void DrPl2::setNr(const int nr)
{
  d->nr = nr;
}

int DrPl2::icon() const
{
  return d->icon;
}

void DrPl2::setIcon(const int icon)
{
  d->icon = icon;
}

std::string DrPl2::name() const
{
  return d->name;
}

void DrPl2::setName(const std::string name)
{
  d->name = name;
}

std::string DrPl2::desc() const
{
  return d->desc;
}

void DrPl2::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX DrPl2::lam() const
{
  return d->lam;
}

void DrPl2::setLam(const RX lam)
{
  d->lam = lam;
}

RX DrPl2::turb() const
{
  return d->turb;
}

void DrPl2::setTurb(const RX turb)
{
  d->turb = turb;
}

RX DrPl2::expt() const
{
  return d->expt;
}

void DrPl2::setExpt(const RX expt)
{
  d->expt = expt;
}

RX DrPl2::dH() const
{
  return d->dH;
}

void DrPl2::setDH(const RX dH)
{
  d->dH = dH;
}

RX DrPl2::ht() const
{
  return d->ht;
}

void DrPl2::setHt(const RX ht)
{
  d->ht = ht;
}

RX DrPl2::wd() const
{
  return d->wd;
}

void DrPl2::setWd(const RX wd)
{
  d->wd = wd;
}

RX DrPl2::cd() const
{
  return d->cd;
}

void DrPl2::setCd(const RX cd)
{
  d->cd = cd;
}

int DrPl2::u_H() const
{
  return d->u_H;
}

void DrPl2::setU_H(const int u_H)
{
  d->u_H = u_H;
}

int DrPl2::u_W() const
{
  return d->u_W;
}

void DrPl2::setU_W(const int u_W)
{
  d->u_W = u_W;
}


AfeCmf::AfeCmf(int nr,int icon,std::string name,std::string desc,RX Flow,int u_F)
{
  d = new AfeCmfPrivate(nr,icon,name,desc,Flow,u_F);
}

std::string AfeCmf::write()
{
  return d->write();
}

void AfeCmf::read(Reader &input)
{
  d->read(input);
}

void AfeCmf::readDetails(Reader &input)
{
  d->readDetails(input);
}

int AfeCmf::nr() const
{
  return d->nr;
}

void AfeCmf::setNr(const int nr)
{
  d->nr = nr;
}

int AfeCmf::icon() const
{
  return d->icon;
}

void AfeCmf::setIcon(const int icon)
{
  d->icon = icon;
}

std::string AfeCmf::name() const
{
  return d->name;
}

void AfeCmf::setName(const std::string name)
{
  d->name = name;
}

std::string AfeCmf::desc() const
{
  return d->desc;
}

void AfeCmf::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX AfeCmf::Flow() const
{
  return d->Flow;
}

void AfeCmf::setFlow(const RX Flow)
{
  d->Flow = Flow;
}

int AfeCmf::u_F() const
{
  return d->u_F;
}

void AfeCmf::setU_F(const int u_F)
{
  d->u_F = u_F;
}


AfeCvf::AfeCvf(int nr,int icon,std::string name,std::string desc,RX Flow,int u_F)
{
  d = new AfeCvfPrivate(nr,icon,name,desc,Flow,u_F);
}

std::string AfeCvf::write()
{
  return d->write();
}

void AfeCvf::read(Reader &input)
{
  d->read(input);
}

void AfeCvf::readDetails(Reader &input)
{
  d->readDetails(input);
}

int AfeCvf::nr() const
{
  return d->nr;
}

void AfeCvf::setNr(const int nr)
{
  d->nr = nr;
}

int AfeCvf::icon() const
{
  return d->icon;
}

void AfeCvf::setIcon(const int icon)
{
  d->icon = icon;
}

std::string AfeCvf::name() const
{
  return d->name;
}

void AfeCvf::setName(const std::string name)
{
  d->name = name;
}

std::string AfeCvf::desc() const
{
  return d->desc;
}

void AfeCvf::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX AfeCvf::Flow() const
{
  return d->Flow;
}

void AfeCvf::setFlow(const RX Flow)
{
  d->Flow = Flow;
}

int AfeCvf::u_F() const
{
  return d->u_F;
}

void AfeCvf::setU_F(const int u_F)
{
  d->u_F = u_F;
}


AfeFan::AfeFan(int nr,int icon,std::string name,std::string desc,RX lam,RX turb,RX expt,RX rdens,RX fdf,RX sop,RX off,std::vector<RX> fpc,RX Sarea,int u_Sa,std::vector<FanDataPoint> data)
{
  d = new AfeFanPrivate(nr,icon,name,desc,lam,turb,expt,rdens,fdf,sop,off,fpc,Sarea,u_Sa,data);
}

std::string AfeFan::write()
{
  return d->write();
}

void AfeFan::read(Reader &input)
{
  d->read(input);
}

void AfeFan::readDetails(Reader &input)
{
  d->readDetails(input);
}

int AfeFan::nr() const
{
  return d->nr;
}

void AfeFan::setNr(const int nr)
{
  d->nr = nr;
}

int AfeFan::icon() const
{
  return d->icon;
}

void AfeFan::setIcon(const int icon)
{
  d->icon = icon;
}

std::string AfeFan::name() const
{
  return d->name;
}

void AfeFan::setName(const std::string name)
{
  d->name = name;
}

std::string AfeFan::desc() const
{
  return d->desc;
}

void AfeFan::setDesc(const std::string desc)
{
  d->desc = desc;
}

RX AfeFan::lam() const
{
  return d->lam;
}

void AfeFan::setLam(const RX lam)
{
  d->lam = lam;
}

RX AfeFan::turb() const
{
  return d->turb;
}

void AfeFan::setTurb(const RX turb)
{
  d->turb = turb;
}

RX AfeFan::expt() const
{
  return d->expt;
}

void AfeFan::setExpt(const RX expt)
{
  d->expt = expt;
}

RX AfeFan::rdens() const
{
  return d->rdens;
}

void AfeFan::setRdens(const RX rdens)
{
  d->rdens = rdens;
}

RX AfeFan::fdf() const
{
  return d->fdf;
}

void AfeFan::setFdf(const RX fdf)
{
  d->fdf = fdf;
}

RX AfeFan::sop() const
{
  return d->sop;
}

void AfeFan::setSop(const RX sop)
{
  d->sop = sop;
}

RX AfeFan::off() const
{
  return d->off;
}

void AfeFan::setOff(const RX off)
{
  d->off = off;
}

std::vector<RX> AfeFan::fpc() const
{
  return d->fpc;
}

void AfeFan::setFpc(const std::vector<RX> fpc)
{
  d->fpc = fpc;
}

RX AfeFan::Sarea() const
{
  return d->Sarea;
}

void AfeFan::setSarea(const RX Sarea)
{
  d->Sarea = Sarea;
}

int AfeFan::u_Sa() const
{
  return d->u_Sa;
}

void AfeFan::setU_Sa(const int u_Sa)
{
  d->u_Sa = u_Sa;
}

std::vector<FanDataPoint> AfeFan::data() const
{
  return d->data;
}

void AfeFan::setData(const std::vector<FanDataPoint> data)
{
  d->data = data;
}


AfeCsf::AfeCsf(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<DataPoint> data)
{
  d = new AfeCsfPrivate(nr,icon,name,desc,u_x,u_y,data);
}

std::string AfeCsf::write()
{
  return d->write(dataType());
}

void AfeCsf::read(Reader &input)
{
  d->read(input);
}

void AfeCsf::readDetails(Reader &input)
{
  d->readDetails(input);
}

int AfeCsf::nr() const
{
  return d->nr;
}

void AfeCsf::setNr(const int nr)
{
  d->nr = nr;
}

int AfeCsf::icon() const
{
  return d->icon;
}

void AfeCsf::setIcon(const int icon)
{
  d->icon = icon;
}

std::string AfeCsf::name() const
{
  return d->name;
}

void AfeCsf::setName(const std::string name)
{
  d->name = name;
}

std::string AfeCsf::desc() const
{
  return d->desc;
}

void AfeCsf::setDesc(const std::string desc)
{
  d->desc = desc;
}

int AfeCsf::u_x() const
{
  return d->u_x;
}

void AfeCsf::setU_x(const int u_x)
{
  d->u_x = u_x;
}

int AfeCsf::u_y() const
{
  return d->u_y;
}

void AfeCsf::setU_y(const int u_y)
{
  d->u_y = u_y;
}

std::vector<DataPoint> AfeCsf::data() const
{
  return d->data;
}

void AfeCsf::setData(const std::vector<DataPoint> data)
{
  d->data = data;
}


AfeSup::AfeSup(int nr,int icon,std::string name,std::string desc,int sched,int u_H,std::vector<AirflowSubelementData> subelements)
{
  d = new AfeSupPrivate(nr,icon,name,desc,sched,u_H,subelements);
}

std::string AfeSup::write()
{
  return d->write();
}

void AfeSup::read(Reader &input)
{
  d->read(input);
}

void AfeSup::readDetails(Reader &input)
{
  d->readDetails(input);
}

int AfeSup::nr() const
{
  return d->nr;
}

void AfeSup::setNr(const int nr)
{
  d->nr = nr;
}

int AfeSup::icon() const
{
  return d->icon;
}

void AfeSup::setIcon(const int icon)
{
  d->icon = icon;
}

std::string AfeSup::name() const
{
  return d->name;
}

void AfeSup::setName(const std::string name)
{
  d->name = name;
}

std::string AfeSup::desc() const
{
  return d->desc;
}

void AfeSup::setDesc(const std::string desc)
{
  d->desc = desc;
}

int AfeSup::sched() const
{
  return d->sched;
}

void AfeSup::setSched(const int sched)
{
  d->sched = sched;
}

int AfeSup::u_H() const
{
  return d->u_H;
}

void AfeSup::setU_H(const int u_H)
{
  d->u_H = u_H;
}

std::vector<AirflowSubelementData> AfeSup::subelements() const
{
  return d->subelements;
}

void AfeSup::setSubelements(const std::vector<AirflowSubelementData> subelements)
{
  d->subelements = subelements;
}

} // contam
} // openstudio
