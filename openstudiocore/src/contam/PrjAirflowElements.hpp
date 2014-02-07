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
#ifndef PRJAIRFLOWELEMENTS_H
#define PRJAIRFLOWELEMENTS_H

#include "PrjDefines.hpp"
#include "PrjAirflowElementsImpl.hpp"
#include <QExplicitlySharedDataPointer>
#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {

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
