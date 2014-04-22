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
#ifndef PRJAIRFLOWELEMENTS_H
#define PRJAIRFLOWELEMENTS_H

#include "PrjDefines.hpp"
#include "PrjSubobjects.hpp"
#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {

class Reader;
namespace detail {
  class PlrOrfImpl;
  class PlrLeakImpl;
  class PlrConnImpl;
  class PlrGeneralImpl;
  class PlrTest1Impl;
  class PlrTest2Impl;
  class PlrCrackImpl;
  class PlrStairImpl;
  class PlrShaftImpl;
  class PlrBackDamperImpl;
  class QfrQuadraticImpl;
  class QfrCrackImpl;
  class QfrTest2Impl;
  class AfeDorImpl;
  class DrPl2Impl;
  class AfeFlowImpl;
  class AfeFanImpl;
  class AfeCsfImpl;
  class AfeSupImpl;
}

/** The AirflowElement object is the base class of all airflow elements. */
class CONTAM_API AirflowElement
{
public:
  /** The Type enumeration is used to identify the various airflow element types. */
  enum Type {PL_ORFC=0,PL_LEAK1=1,PL_LEAK2=2,PL_LEAK3=3,PL_CONN=4,PL_QCN=5,PL_FCN=6,PL_TEST1=7,PL_TEST2=8,
    PL_CRACK=9,PL_STAIR=10,PL_SHAFT=11,PL_BDQ=12,PL_BDF=13,QFR_QAB=14,QFR_QAF=15,QFR_CRACK=16,
    QFR_TEST2=17,DR_DOOR=18,DR_PL2=19,FN_CMF=20,FN_CVF=21,FN_FAN=22,CS_FSP=23,CS_QSP=24,CS_PSF=25,
    CS_PSQ=26,AF_SUP=27,UNKNOWN};
  /** Destroy the object. */
  virtual ~AirflowElement(){}
  /** Write the object to a string. */
  virtual std::string write()=0;
  /** Convert an input string into an airflow element type enum. */
  static Type convertTag(std::string string);
  /** Read an airflow element from a Reader object. */
  static AirflowElement* readElement(Reader &input);
  // virtual void recompute(){}
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  virtual int nr() const = 0;
  /** Sets the element number. This should only be done with care. */
  virtual void setNr(const int nr) = 0;
  /** Returns the icon used to represent flow path. */
  virtual int icon() const = 0;
  /** Sets the icon used to represent flow path. */
  virtual void setIcon(const int icon) = 0;
  /** Returns the element data type. */
  virtual std::string dataType() const = 0;
  /** Returns the element name. */
  virtual std::string name() const = 0;
  /** Sets the element name. */
  virtual void setName(const std::string &name) = 0;
  /** Returns the element description. */
  virtual std::string desc() const = 0;
  /** Sets the element description. */
  virtual void setDesc(const std::string &desc) = 0;

};

/** The PlrOrj object is an airflow element representing an orifice. */
class CONTAM_API PlrOrf : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrOrf();
  /** Create a new object. */
  PlrOrf(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrOrf(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double area,double dia,
    double coef,double Re,int u_A,int u_D);
  /** Create a new object. */
  PlrOrf(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string area,std::string dia,
    std::string coef,std::string Re,int u_A,int u_D);
  /** Create a new object from another object. */
  PlrOrf(const PlrOrf &other);
  /** Destroy the object. */
  ~PlrOrf();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  PlrOrf& operator=(const PlrOrf &other);
  /** Equality operator */
  bool operator==(const PlrOrf &other) const;
  /** Inequality operator */
  bool operator!=(const PlrOrf &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "plr_orfc";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the actual area [m^2]. */
  double area() const;
  /** Sets the actual area [m^2]. */
  bool setArea(const double area);
  /** Sets the actual area [m^2]. */
  bool setArea(const std::string &area);
  /** Returns the hydraulic diameter [m]. */
  double dia() const;
  /** Sets the hydraulic diameter [m]. */
  bool setDia(const double dia);
  /** Sets the hydraulic diameter [m]. */
  bool setDia(const std::string &dia);
  /** Returns the flow coefficient. */
  double coef() const;
  /** Sets the flow coefficient. */
  bool setCoef(const double coef);
  /** Sets the flow coefficient. */
  bool setCoef(const std::string &coef);
  /** Returns the laminar/turbulet transition Reynolds number. */
  double Re() const;
  /** Sets the laminar/turbulet transition Reynolds number. */
  bool setRe(const double Re);
  /** Sets the laminar/turbulet transition Reynolds number. */
  bool setRe(const std::string &Re);
  /** Returns the display units for area. */
  int u_A() const;
  /** Sets the display units for area. */
  void setU_A(const int u_A);
  /** Returns the display units for diameter. */
  int u_D() const;
  /** Sets the display units for diameter. */
  void setU_D(const int u_D);
  //@}

private:
  boost::shared_ptr<detail::PlrOrfImpl> m_impl;
};

/** The PlrLeak object is the base class of the leakage area airflow elements. */
class CONTAM_API PlrLeak : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrLeak();
  /** Create a new object. */
  PlrLeak(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrLeak(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double coef,
    double pres,double area1,double area2,double area3,int u_A1,int u_A2,int u_A3,int u_dP);
  /** Create a new object. */
  PlrLeak(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string coef,
    std::string pres,std::string area1,std::string area2,std::string area3,int u_A1,int u_A2,int u_A3,int u_dP);
  /** Create a new object from another object. */
  PlrLeak(const PlrLeak &other);
  /** Destroy the object. */
  ~PlrLeak();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  PlrLeak& operator=(const PlrLeak &other);
  /** Equality operator */
  bool operator==(const PlrLeak &other) const;
  /** Inequality operator */
  bool operator!=(const PlrLeak &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  virtual std::string dataType() const=0;
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the flow coefficient. */
  double coef() const;
  /** Sets the flow coefficient. */
  bool setCoef(const double coef);
  /** Sets the flow coefficient. */
  bool setCoef(const std::string &coef);
  /** Returns the reference pressure drop [Pa]. */
  double pres() const;
  /** Sets the reference pressure drop [Pa]. */
  bool setPres(const double pres);
  /** Sets the reference pressure drop [Pa]. */
  bool setPres(const std::string &pres);
  /** Returns the leakage area per item [m^2]. */
  double area1() const;
  /** Sets the leakage area per item [m^2]. */
  bool setArea1(const double area1);
  /** Sets the leakage area per item [m^2]. */
  bool setArea1(const std::string &area1);
  /** Returns the leakage area per unit length [m^2/m]. */
  double area2() const;
  /** Sets the leakage area per unit length [m^2/m]. */
  bool setArea2(const double area2);
  /** Sets the leakage area per unit length [m^2/m]. */
  bool setArea2(const std::string &area2);
  /** Returns the leakage area per unit area [m^2/m^2]. */
  double area3() const;
  /** Sets the leakage area per unit area [m^2/m^2]. */
  bool setArea3(const double area3);
  /** Sets the leakage area per unit area [m^2/m^2]. */
  bool setArea3(const std::string &area3);
  /** Returns the display units of area 1. */
  int u_A1() const;
  /** Sets the display units of area 1. */
  void setU_A1(const int u_A1);
  /** Returns the display units of area 2. */
  int u_A2() const;
  /** Sets the display units of area 2. */
  void setU_A2(const int u_A2);
  /** Returns the display units of area 3. */
  int u_A3() const;
  /** Sets the display units of area 3. */
  void setU_A3(const int u_A3);
  /** Returns the display units of pressure. */
  int u_dP() const;
  /** Sets the display units of pressure. */
  void setU_dP(const int u_dP);
  //@}
private:
  boost::shared_ptr<detail::PlrLeakImpl> m_impl;
};

/** The PlrLeak1 object is an airflow element representing a leakage area on a per-item basis. */
class CONTAM_API PlrLeak1 : public PlrLeak
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrLeak1();
  /** Create a new object. */
  PlrLeak1(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrLeak1(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double coef,
    double pres,double area1,double area2,double area3,int u_A1,int u_A2,int u_A3,int u_dP);
  /** Create a new object. */
  PlrLeak1(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string coef,
    std::string pres,std::string area1,std::string area2,std::string area3,int u_A1,int u_A2,int u_A3,int u_dP);
  //** Create a new object from another object. */
  //PlrLeak(const PlrLeak &other);
  /** Destroy the object. */
  ~PlrLeak1();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "plr_leak1";}
  //@}
};

/** The PlrLeak2 object is an airflow element representing a leakage area on a per-unit-length basis. */
class CONTAM_API PlrLeak2 : public PlrLeak
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrLeak2();
  /** Create a new object. */
  PlrLeak2(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrLeak2(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double coef,
    double pres,double area1,double area2,double area3,int u_A1,int u_A2,int u_A3,int u_dP);
  /** Create a new object. */
  PlrLeak2(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string coef,
    std::string pres,std::string area1,std::string area2,std::string area3,int u_A1,int u_A2,int u_A3,int u_dP);
  //** Create a new object from another object. */
  //PlrLeak(const PlrLeak &other);
  /** Destroy the object. */
  ~PlrLeak2();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "plr_leak2";}
  //@}
};

/** The PlrLeak3 object is an airflow element representing a leakage area on a per-unit-area basis. */
class CONTAM_API PlrLeak3 : public PlrLeak
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrLeak3();
  /** Create a new object. */
  PlrLeak3(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrLeak3(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double coef,
    double pres,double area1,double area2,double area3,int u_A1,int u_A2,int u_A3,int u_dP);
  /** Create a new object. */
  PlrLeak3(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string coef,
    std::string pres,std::string area1,std::string area2,std::string area3,int u_A1,int u_A2,int u_A3,int u_dP);
  //** Create a new object from another object. */
  //PlrLeak(const PlrLeak &other);
  /** Destroy the object. */
  ~PlrLeak3();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "plr_leak3";}
  //@}
};

/** The PlrConn object is an airflow element representing a ASCOS connection. */
class CONTAM_API PlrConn : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrConn();
  /** Create a new object. */
  PlrConn(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrConn(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double area,
    double coef,int u_A);
  /** Create a new object. */
  PlrConn(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string area,
    std::string coef,int u_A);
  /** Create a new object from another object. */
  PlrConn(const PlrConn &other);
  /** Destroy the object. */
  ~PlrConn();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  PlrConn& operator=(const PlrConn &other);
  /** Equality operator */
  bool operator==(const PlrConn &other) const;
  /** Inequality operator */
  bool operator!=(const PlrConn &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "plr_conn";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the actual area [m^2]. */
  double area() const;
  /** Sets the actual area [m^2]. */
  bool setArea(const double area1);
  /** Sets the actual area [m^2]. */
  bool setArea(const std::string &area1);
  /** Returns the flow coefficient. */
  double coef() const;
  /** Sets the flow coefficient. */
  bool setCoef(const double coef);
  /** Sets the flow coefficient. */
  bool setCoef(const std::string &coef);
  /** Returns the display units of area. */
  int u_A() const;
  /** Sets the display units of area. */
  void setU_A(const int u_A1);
  //@}

private:
  boost::shared_ptr<detail::PlrConnImpl> m_impl;
};

/** The PlrGeneral object is the base class for general power law elements. */
class CONTAM_API PlrGeneral : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrGeneral();
  /** Create a new object. */
  PlrGeneral(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrGeneral(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt);
  /** Create a new object. */
  PlrGeneral(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt);
  /** Create a new object from another object. */
  PlrGeneral(const PlrGeneral &other);
  /** Destroy the object. */
  ~PlrGeneral();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  PlrGeneral& operator=(const PlrGeneral &other);
  /** Equality operator */
  bool operator==(const PlrGeneral &other) const;
  /** Inequality operator */
  bool operator!=(const PlrGeneral &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  virtual std::string dataType() const=0;
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  //@}

private:
  boost::shared_ptr<detail::PlrGeneralImpl> m_impl;
};

/** The PlrQcn object is an airflow element representing volume flow rate power law leakage. */
class CONTAM_API PlrQcn : public PlrGeneral
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrQcn();
  /** Create a new object. */
  PlrQcn(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrQcn(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt);
  /** Create a new object. */
  PlrQcn(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt);
  //** Create a new object from another object. */
  //PlrQcn(const PlrQcn &other);
  /** Destroy the object. */
  ~PlrQcn();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "plr_qcn";}
  //@}
};

/** The PlrQcn object is an airflow element representing mass flow rate power law leakage. */
class CONTAM_API PlrFcn : public PlrGeneral
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrFcn();
  /** Create a new object. */
  PlrFcn(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrFcn(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt);
  /** Create a new object. */
  PlrFcn(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt);
  //** Create a new object from another object. */
  //PlrQcn(const PlrQcn &other);
  /** Destroy the object. */
  ~PlrFcn();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "plr_fcn";}
  //@}
};

/** The PlrTest1 object is an airflow element representing 1-point test data power law leakage. */
class CONTAM_API PlrTest1 : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrTest1();
  /** Create a new object. */
  PlrTest1(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrTest1(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dP,double Flow,
    int u_P,int u_F);
  /** Create a new object. */
  PlrTest1(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dP,std::string Flow,
    int u_P,int u_F);
  /** Create a new object from another object. */
  PlrTest1(const PlrTest1 &other);
  /** Destroy the object. */
  ~PlrTest1();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  PlrTest1& operator=(const PlrTest1 &other);
  /** Equality operator */
  bool operator==(const PlrTest1 &other) const;
  /** Inequality operator */
  bool operator!=(const PlrTest1 &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "plr_test1";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the pressure drop [Pa]. */
  double dP() const;
  /** Sets the pressure drop [Pa]. */
  bool setDP(const double dP);
  /** Sets the pressure drop [Pa]. */
  bool setDP(const std::string &dP);
  /** Returns the flow rate [kg/s].*/
  double Flow() const;
  /** Sets the flow rate [kg/s].*/
  bool setFlow(const double Flow);
  /** Sets the flow rate [kg/s].*/
  bool setFlow(const std::string &Flow);
  /** Returns the display units of pressure drop. */
  int u_P() const;
  /** Sets the display units of pressure drop. */
  void setU_P(const int u_P);
  /** Returns the display units of flow. */
  int u_F() const;
  /** Sets the display units of flow. */
  void setU_F(const int u_F);
  //@}

private:
  boost::shared_ptr<detail::PlrTest1Impl> m_impl;
};

/** The PlrTest2 object is an airflow element representing 2-point test data power law leakage. */
class CONTAM_API PlrTest2 : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrTest2();
  /** Create a new object. */
  PlrTest2(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrTest2(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dP1,double F1,
    double dP2,double F2,int u_P1,int u_F1,int u_P2,int u_F2);
  /** Create a new object. */
  PlrTest2(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dP1,std::string F1,
    std::string dP2,std::string F2,int u_P1,int u_F1,int u_P2,int u_F2);
  /** Create a new object from another object. */
  PlrTest2(const PlrTest2 &other);
  /** Destroy the object. */
  ~PlrTest2();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  PlrTest2& operator=(const PlrTest2 &other);
  /** Equality operator */
  bool operator==(const PlrTest2 &other) const;
  /** Inequality operator */
  bool operator!=(const PlrTest2 &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "plr_test2";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the point 1 pressure drop [Pa]. */
  double dP1() const;
  /** Sets the point 1 pressure drop [Pa]. */
  bool setDP1(const double dP);
  /** Sets the point 1 pressure drop [Pa]. */
  bool setDP1(const std::string &dP);
  /** Returns the point 1 flow rate [kg/s].*/
  double F1() const;
  /** Sets the point 1 flow rate [kg/s].*/
  bool setF1(const double Flow);
  /** Sets the point 1 flow rate [kg/s].*/
  bool setF1(const std::string &Flow);
  /** Returns the point 2 pressure drop [Pa]. */
  double dP2() const;
  /** Sets the point 2 pressure drop [Pa]. */
  bool setDP2(const double dP);
  /** Sets the point 2 pressure drop [Pa]. */
  bool setDP2(const std::string &dP);
  /** Returns the point 2 flow rate [kg/s].*/
  double F2() const;
  /** Sets the point 2 flow rate [kg/s].*/
  bool setF2(const double Flow);
  /** Sets the point 2 flow rate [kg/s].*/
  bool setF2(const std::string &Flow);
  /** Returns the display units of point 1 pressure drop. */
  int u_P1() const;
  /** Sets the display units of point 1 pressure drop. */
  void setU_P1(const int u_P);
  /** Returns the display units of point 1 flow. */
  int u_F1() const;
  /** Sets the display units of point 1 flow. */
  void setU_F1(const int u_F);
  /** Returns the display units of point 2 pressure drop. */
  int u_P2() const;
  /** Sets the display units of point 2 pressure drop. */
  void setU_P2(const int u_P);
  /** Returns the display units of point 2 flow. */
  int u_F2() const;
  /** Sets the display units of point 2 flow. */
  void setU_F2(const int u_F);
  //@}

private:
  boost::shared_ptr<detail::PlrTest2Impl> m_impl;
};

/** The PlrCrack object is an airflow element representing power law leakage through a crack. */
class CONTAM_API PlrCrack : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrCrack();
  /** Create a new object. */
  PlrCrack(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrCrack(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double length,
    double width,int u_L,int u_W);
  /** Create a new object. */
  PlrCrack(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string length,
    std::string width,int u_L,int u_W);
  /** Create a new object from another object. */
  PlrCrack(const PlrCrack &other);
  /** Destroy the object. */
  ~PlrCrack();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  PlrCrack& operator=(const PlrCrack &other);
  /** Equality operator */
  bool operator==(const PlrCrack &other) const;
  /** Inequality operator */
  bool operator!=(const PlrCrack &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "plr_crack";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the crack length [m]. */
  double length() const;
  /** Sets the crack length [m]. */
  bool setLength(const double length);
  /** Sets the crack length [m]. */
  bool setLength(const std::string &length);
  /** Returns the crack width [m].*/
  double width() const;
  /** Sets the crack width [m].*/
  bool setWidth(const double width);
  /** Sets the crack width [m].*/
  bool setWidth(const std::string &width);
  /** Returns the display units of length. */
  int u_L() const;
  /** Sets the display units of length. */
  void setU_L(const int u_L);
  /** Returns the display units of width. */
  int u_W() const;
  /** Sets the display units of width. */
  void setU_W(const int u_W);
  //@}

private:
  boost::shared_ptr<detail::PlrCrackImpl> m_impl;
};

/** The PlrStair object is an airflow element representing level-to-level power law leakage in stairwells. */
class CONTAM_API PlrStair : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrStair();
  /** Create a new object. */
  PlrStair(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrStair(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
    double Ht,double Area,double peo,int tread,int u_A,int u_D);
  /** Create a new object. */
  PlrStair(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
    std::string Ht,std::string Area,std::string peo,int tread,int u_A,int u_D);
  /** Create a new object from another object. */
  PlrStair(const PlrStair &other);
  /** Destroy the object. */
  ~PlrStair();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  PlrStair& operator=(const PlrStair &other);
  /** Equality operator */
  bool operator==(const PlrStair &other) const;
  /** Inequality operator */
  bool operator!=(const PlrStair &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "plr_stair";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the distance between levels [m]. */
  double Ht() const;
  /** Sets the distance between levels [m]. */
  bool setHt(const double Ht);
  /** Sets the distance between levels [m]. */
  bool setHt(const std::string &Ht);
  /** Returns the cross-sectional area [m^2]. */
  double area() const;
  /** Sets the cross-sectional area [m^2]. */
  bool setArea(const double Area);
  /** Sets the cross-sectional area [m^2]. */
  bool setArea(const std::string &Area);
  /** Returns the density of people [pers/m^2]. */
  double people() const;
  /** Sets the density of people [pers/m^2]. */
  bool setPeople(const double peo);
  /** Sets the density of people [pers/m^2]. */
  bool setPeople(const std::string &peo);
  /** Returns the tread type: 1 = open tread 0 = closed. */
  int tread() const;
  /** Sets the tread type: 1 = open tread 0 = closed. */
  void setTread(const int tread);
  /** Returns the display units of area. */
  int u_A() const;
  /** Sets the display units of area. */
  void setU_A(const int u_A);
  /** Returns the display units of distance. */
  int u_D() const;
  /** Sets the display units of distance. */
  void setU_D(const int u_D);
  //@}

private:
  boost::shared_ptr<detail::PlrStairImpl> m_impl;
};

/** The PlrShaft object is an airflow element representing level-to-level power law leakage in shafts. */
class CONTAM_API PlrShaft : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrShaft();
  /** Create a new object. */
  PlrShaft(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrShaft(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
    double Ht,double area,double perim,double rough,int u_A,int u_D,int u_P,
    int u_R);
  /** Create a new object. */
  PlrShaft(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
    std::string Ht,std::string area,std::string perim,std::string rough,int u_A,int u_D,int u_P,
    int u_R);
  /** Create a new object from another object. */
  PlrShaft(const PlrShaft &other);
  /** Destroy the object. */
  ~PlrShaft();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  PlrShaft& operator=(const PlrShaft &other);
  /** Equality operator */
  bool operator==(const PlrShaft &other) const;
  /** Inequality operator */
  bool operator!=(const PlrShaft &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "plr_shaft";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the distance between levels [m]. */
  double Ht() const;
  /** Sets the distance between levels [m]. */
  bool setHt(const double Ht);
  /** Sets the distance between levels [m]. */
  bool setHt(const std::string &Ht);
  /** Returns the cross-sectional area [m^2]. */
  double area() const;
  /** Sets the cross-sectional area [m^2]. */
  bool setArea(const double Area);
  /** Sets the cross-sectional area [m^2]. */
  bool setArea(const std::string &Area);
  /** Returns the perimeter [m]. */
  double perim() const;
  /** Sets the perimeter [m]. */
  bool setPerim(const double perim);
  /** Sets the perimeter [m]. */
  bool setPerim(const std::string &perim);
  /** Returns the roughness [m]. */
  double rough() const;
  /** Sets the roughness [m]. */
  bool setRough(const double rough);
  /** Sets the roughness [m]. */
  bool setRough(const std::string &rough);
  /** Returns the display units of area. */
  int u_A() const;
  /** Sets the display units of area. */
  void setU_A(const int u_A);
  /** Returns the display units of distance. */
  int u_D() const;
  /** Sets the display units of distance. */
  void setU_D(const int u_D);
  /** Returns the display units of perimeter. */
  int u_P() const;
  /** Sets the display units of perimeter. */
  void setU_P(const int u_P);
  /** Returns the display units of roughness. */
  int u_R() const;
  /** Sets the display units of roughness. */
  void setU_R(const int u_R);
  //@}

private:
  boost::shared_ptr<detail::PlrShaftImpl> m_impl;
};

/** The PlrBackDamper object is the base class of backdraft damper airflow elements. */
class CONTAM_API PlrBackDamper : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrBackDamper();
  /** Create a new object. */
  PlrBackDamper(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrBackDamper(int nr,int icon,std::string name,std::string desc,double lam,double Cp,double xp,double Cn,double xn);
  /** Create a new object. */
  PlrBackDamper(int nr,int icon,std::string name,std::string desc,std::string lam,std::string Cp,std::string xp,std::string Cn,std::string xn);
  /** Create a new object from another object. */
  PlrBackDamper(const PlrBackDamper &other);
  /** Destroy the object. */
  ~PlrBackDamper();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  PlrBackDamper& operator=(const PlrBackDamper &other);
  /** Equality operator */
  bool operator==(const PlrBackDamper &other) const;
  /** Inequality operator */
  bool operator!=(const PlrBackDamper &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  virtual std::string dataType() const = 0;
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient (dP > 0). */
  double Cp() const;
  /** Sets the turbulent flow coefficient (dP > 0). */
  bool setCp(const double Cp);
  /** Sets the turbulent flow coefficient (dP > 0). */
  bool setCp(const std::string &Cp);
  /** Returns the pressure exponent (dP > 0). */
  double xp() const;
  /** Sets the pressure exponent (dP > 0). */
  bool setXp(const double xp);
  /** Sets the pressure exponent (dP > 0). */
  bool setXp(const std::string &xp);
  /** Returns the turbulent flow coefficient (dP < 0). */
  double Cn() const;
  /** Sets the turbulent flow coefficient (dP < 0). */
  bool setCn(const double Cn);
  /** Sets the turbulent flow coefficient (dP < 0). */
  bool setCn(const std::string &Cn);
  /** Returns the pressure exponent (dP < 0). */
  double xn() const;
  /** Sets the pressure exponent (dP < 0). */
  bool setXn(const double xn);
  /** Sets the pressure exponent (dP < 0). */
  bool setXn(const std::string &xn);
  //@}
private:
  boost::shared_ptr<detail::PlrBackDamperImpl> m_impl;
};

/** The PlrBdq object is a backdraft damper airflow element in volumetric flow form. */
class CONTAM_API PlrBdq : public PlrBackDamper
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrBdq();
  /** Create a new object. */
  PlrBdq(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrBdq(int nr,int icon,std::string name,std::string desc,double lam,double Cp,double xp,double Cn,double xn);
  /** Create a new object. */
  PlrBdq(int nr,int icon,std::string name,std::string desc,std::string lam,std::string Cp,std::string xp,std::string Cn,std::string xn);
  /** Create a new object from another object. */
  //PlrBdq(const PlrBdq &other);
  /** Destroy the object. */
  ~PlrBdq();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element data type. */
  virtual std::string dataType() const {return "plr_bdq";}
  //@}
};

/** The PlrBdf object is a backdraft damper airflow element in mass flow form. */
class CONTAM_API PlrBdf : public PlrBackDamper
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  PlrBdf();
  /** Create a new object. */
  PlrBdf(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  PlrBdf(int nr,int icon,std::string name,std::string desc,double lam,double Cp,double xp,double Cn,double xn);
  /** Create a new object. */
  PlrBdf(int nr,int icon,std::string name,std::string desc,std::string lam,std::string Cp,std::string xp,std::string Cn,std::string xn);
  /** Create a new object from another object. */
  //PlrBdf(const PlrBdf &other);
  /** Destroy the object. */
  ~PlrBdf();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element data type. */
  std::string dataType() const {return "plr_bdf";}
  //@}
};

/** The QfrGeneral object is the base class for general quadratic airflow elements. */
class CONTAM_API QfrGeneral : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  QfrGeneral();
  /** Create a new object. */
  QfrGeneral(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  QfrGeneral(int nr,int icon,std::string name,std::string desc,double a,double b);
  /** Create a new object. */
  QfrGeneral(int nr,int icon,std::string name,std::string desc,std::string a,std::string b);
  /** Create a new object from another object. */
  QfrGeneral(const QfrGeneral &other);
  /** Destroy the object. */
  ~QfrGeneral();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  QfrGeneral& operator=(const QfrGeneral &other);
  /** Equality operator */
  bool operator==(const QfrGeneral &other) const;
  /** Inequality operator */
  bool operator!=(const QfrGeneral &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  virtual std::string dataType() const = 0;
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the flow rate coefficient. */
  double a() const;
  /** Sets the flow rate coefficient. */
  bool setA(const double a);
  /** Sets the flow rate coefficient. */
  bool setA(const std::string &a);
  /** Returns the flow rate^2 coefficient. */
  double b() const;
  /** Sets the flow rate^2 coefficient. */
  bool setB(const double b);
  /** Sets the flow rate^2 coefficient. */
  bool setB(const std::string &b);
  //@}
private:
  boost::shared_ptr<detail::QfrQuadraticImpl> m_impl;
};

/** The QfrQab object is an airflow element representing volume flow rate quadratic leakage. */
class CONTAM_API QfrQab : public QfrGeneral
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  QfrQab();
  /** Create a new object. */
  QfrQab(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  QfrQab(int nr,int icon,std::string name,std::string desc,double a,double b);
  /** Create a new object. */
  QfrQab(int nr,int icon,std::string name,std::string desc,std::string a,std::string b);
  /** Create a new object from another object. */
  //QfrQab(const QfrQab &other);
  /** Destroy the object. */
  ~QfrQab();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "qfr_qab";}
  //@}
};

/** The QfrFab object is an airflow element representing mass flow rate quadratic leakage. */
class CONTAM_API QfrFab : public QfrGeneral
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  QfrFab();
  /** Create a new object. */
  QfrFab(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  QfrFab(int nr,int icon,std::string name,std::string desc,double a,double b);
  /** Create a new object. */
  QfrFab(int nr,int icon,std::string name,std::string desc,std::string a,std::string b);
  /** Create a new object from another object. */
  //QfrFab(const QfrFab &other);
  /** Destroy the object. */
  ~QfrFab();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "qfr_fab";}
  //@}
};

/** The QfrCrack object is an airflow element representing quadratic leakage through a crack. */
class CONTAM_API QfrCrack : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  QfrCrack();
  /** Create a new object. */
  QfrCrack(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  QfrCrack(int nr,int icon,std::string name,std::string desc,double a,double b,double length,double width,
    double depth,int nB,int u_L,int u_W,int u_D);
  /** Create a new object. */
  QfrCrack(int nr,int icon,std::string name,std::string desc,std::string a,std::string b,std::string length,std::string width,
    std::string depth,int nB,int u_L,int u_W,int u_D);
  /** Create a new object from another object. */
  QfrCrack(const QfrCrack &other);
  /** Destroy the object. */
  ~QfrCrack();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  QfrCrack& operator=(const QfrCrack &other);
  /** Equality operator */
  bool operator==(const QfrCrack &other) const;
  /** Inequality operator */
  bool operator!=(const QfrCrack &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "qfr_crack";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the flow rate coefficient. */
  double a() const;
  /** Sets the flow rate coefficient. */
  bool setA(const double a);
  /** Sets the flow rate coefficient. */
  bool setA(const std::string &a);
  /** Returns the flow rate^2 coefficient. */
  double b() const;
  /** Sets the flow rate^2 coefficient. */
  bool setB(const double b);
  /** Sets the flow rate^2 coefficient. */
  bool setB(const std::string &b);
  /** Returns the crack length [m]. */
  double length() const;
  /** Sets the crack length [m]. */
  bool setLength(const double length);
  /** Sets the crack length [m]. */
  bool setLength(const std::string &length);
  /** Returns the crack width [m].*/
  double width() const;
  /** Sets the crack width [m].*/
  bool setWidth(const double width);
  /** Sets the crack width [m].*/
  bool setWidth(const std::string &width);
  /** Sets the crack depth [m].*/
  double depth() const;
  /** Sets the crack depth [m].*/
  bool setDepth(const double depth);
  /** Sets the crack depth [m].*/
  bool setDepth(const std::string &depth);
  /** Returns the number of bends. */
  int nB() const;
  /** Sets the number of bends. */
  void setNB(const int nB);
  /** Returns the display units of length. */
  int u_L() const;
  /** Sets the display units of length. */
  void setU_L(const int u_L);
  /** Returns the display units of width. */
  int u_W() const;
  /** Sets the display units of width. */
  void setU_W(const int u_W);
  /** Returns the display units of depth. */
  int u_D() const;
  /** Sets the display units of width. */
  void setU_D(const int u_D);
  //@}
private:
  boost::shared_ptr<detail::QfrCrackImpl> m_impl;
};

/** The QfrTest2 object is an airflow element representing 2-point test data quadratic leakage. */
class CONTAM_API QfrTest2 : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  QfrTest2();
  /** Create a new object. */
  QfrTest2(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  QfrTest2(int nr,int icon,std::string name,std::string desc,double a,double b,double dP1,double F1,double dP2,
    double F2,int u_P1,int u_F1,int u_P2,int u_F2);
  /** Create a new object. */
  QfrTest2(int nr,int icon,std::string name,std::string desc,std::string a,std::string b,std::string dP1,std::string F1,std::string dP2,
    std::string F2,int u_P1,int u_F1,int u_P2,int u_F2);
  /** Create a new object from another object. */
  QfrTest2(const QfrTest2 &other);
  /** Destroy the object. */
  ~QfrTest2();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  QfrTest2& operator=(const QfrTest2 &other);
  /** Equality operator */
  bool operator==(const QfrTest2 &other) const;
  /** Inequality operator */
  bool operator!=(const QfrTest2 &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "qfr_test2";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the flow rate coefficient. */
  double a() const;
  /** Sets the flow rate coefficient. */
  bool setA(const double a);
  /** Sets the flow rate coefficient. */
  bool setA(const std::string &a);
  /** Returns the flow rate^2 coefficient. */
  double b() const;
  /** Sets the flow rate^2 coefficient. */
  bool setB(const double b);
  /** Sets the flow rate^2 coefficient. */
  bool setB(const std::string &b);
  /** Returns the point 1 pressure drop [Pa]. */
  double dP1() const;
  /** Sets the point 1 pressure drop [Pa]. */
  bool setDP1(const double dP);
  /** Sets the point 1 pressure drop [Pa]. */
  bool setDP1(const std::string &dP);
  /** Returns the point 1 flow rate [kg/s].*/
  double F1() const;
  /** Sets the point 1 flow rate [kg/s].*/
  bool setF1(const double Flow);
  /** Sets the point 1 flow rate [kg/s].*/
  bool setF1(const std::string &Flow);
  /** Returns the point 2 pressure drop [Pa]. */
  double dP2() const;
  /** Sets the point 2 pressure drop [Pa]. */
  bool setDP2(const double dP);
  /** Sets the point 2 pressure drop [Pa]. */
  bool setDP2(const std::string &dP);
  /** Returns the point 2 flow rate [kg/s].*/
  double F2() const;
  /** Sets the point 2 flow rate [kg/s].*/
  bool setF2(const double Flow);
  /** Sets the point 2 flow rate [kg/s].*/
  bool setF2(const std::string &Flow);
  /** Returns the display units of point 1 pressure drop. */
  int u_P1() const;
  /** Sets the display units of point 1 pressure drop. */
  void setU_P1(const int u_P);
  /** Returns the display units of point 1 flow. */
  int u_F1() const;
  /** Sets the display units of point 1 flow. */
  void setU_F1(const int u_F);
  /** Returns the display units of point 2 pressure drop. */
  int u_P2() const;
  /** Sets the display units of point 2 pressure drop. */
  void setU_P2(const int u_P);
  /** Returns the display units of point 2 flow. */
  int u_F2() const;
  /** Sets the display units of point 2 flow. */
  void setU_F2(const int u_F);
  //@}
private:
  boost::shared_ptr<detail::QfrTest2Impl> m_impl;
};

/** The AfeDor object is an airflow element representing 2-way flow through a single opening. */
class CONTAM_API AfeDor : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfeDor();
  /** Create a new object. */
  AfeDor(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfeDor(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dTmin,double ht,
    double wd,double cd,int u_T,int u_H,int u_W);
  /** Create a new object. */
  AfeDor(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dTmin,std::string ht,
    std::string wd,std::string cd,int u_T,int u_H,int u_W);
  /** Create a new object from another object. */
  AfeDor(const AfeDor &other);
  /** Destroy the object. */
  ~AfeDor();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  AfeDor& operator=(const AfeDor &other);
  /** Equality operator */
  bool operator==(const AfeDor &other) const;
  /** Inequality operator */
  bool operator!=(const AfeDor &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "dor_door";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the minimum temperature difference for two-way flow [C]. */
  double dTmin() const;
  /** Sets the minimum temperature difference for two-way flow [C]. */
  bool setDTmin(const double dTmin);
  /** Sets the minimum temperature difference for two-way flow [C]. */
  bool setDTmin(const std::string &dTmin);
  /** Returns the height of doorway [m]. */
  double height() const;
  /** Sets the height of doorway [m]. */
  bool setHeight(const double ht);
  /** Sets the height of doorway [m]. */
  bool setHeight(const std::string &ht);
  /** Sets the width of doorway [m]. */
  double width() const;
  /** Sets the width of doorway [m]. */
  bool setWidth(const double wd);
  /** Sets the width of doorway [m]. */
  bool setWidth(const std::string &wd);
  /** Returns the discharge coefficient. */
  double cd() const;
  /** Sets the discharge coefficient. */
  bool setCd(const double cd);
  /** Sets the discharge coefficient. */
  bool setCd(const std::string &cd);
  /** Returns the display units of temperature. */
  int u_T() const;
  /** Sets the display units of temperature. */
  void setU_T(const int u_T);
  /** Returns the display units of height. */
  int u_H() const;
  /** Sets the display units of height. */
  void setU_H(const int u_H);
  /** Returns the display units of width. */
  int u_W() const;
  /** Sets the display units of width. */
  void setU_W(const int u_W);
  //@}
private:
  boost::shared_ptr<detail::AfeDorImpl> m_impl;
};

/** The DrPl2 object is an airflow element representing 2-way flow through an opening using two power law models. */
class CONTAM_API DrPl2 : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  DrPl2();
  /** Create a new object. */
  DrPl2(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  DrPl2(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dH,double ht,
    double wd,double cd,int u_H,int u_W);
  /** Create a new object. */
  DrPl2(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dH,std::string ht,
    std::string wd,std::string cd,int u_H,int u_W);
  /** Create a new object from another object. */
  DrPl2(const DrPl2 &other);
  /** Destroy the object. */
  ~DrPl2();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  DrPl2& operator=(const DrPl2 &other);
  /** Equality operator */
  bool operator==(const DrPl2 &other) const;
  /** Inequality operator */
  bool operator!=(const DrPl2 &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "dor_pl2";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the distance above/below midpoint [m]. */
  double dH() const;
  /** Sets the distance above/below midpoint [m]. */
  bool setDH(const double dH);
  /** Sets the distance above/below midpoint [m]. */
  bool setDH(const std::string &dH);
  /** Returns the height of doorway [m]. */
  double height() const;
  /** Sets the height of doorway [m]. */
  bool setHeight(const double ht);
  /** Sets the height of doorway [m]. */
  bool setHeight(const std::string &ht);
  /** Sets the width of doorway [m]. */
  double width() const;
  /** Sets the width of doorway [m]. */
  bool setWidth(const double wd);
  /** Sets the width of doorway [m]. */
  bool setWidth(const std::string &wd);
  /** Returns the discharge coefficient. */
  double cd() const;
  /** Sets the discharge coefficient. */
  bool setCd(const double cd);
  /** Sets the discharge coefficient. */
  bool setCd(const std::string &cd);
  /** Returns the display units of height. */
  int u_H() const;
  /** Sets the display units of height. */
  void setU_H(const int u_H);
  /** Returns the display units of width. */
  int u_W() const;
  /** Sets the display units of width. */
  void setU_W(const int u_W);
  //@}
private:
  boost::shared_ptr<detail::DrPl2Impl> m_impl;
};

/** The AfeFlow object is the base class of constant flow rate airflow elements. */
class CONTAM_API AfeFlow : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfeFlow();
  /** Create a new object. */
  AfeFlow(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfeFlow(int nr,int icon,std::string name,std::string desc,double Flow,int u_F);
  /** Create a new object. */
  AfeFlow(int nr,int icon,std::string name,std::string desc,std::string Flow,int u_F);
  /** Create a new object from another object. */
  AfeFlow(const AfeFlow &other);
  /** Destroy the object. */
  ~AfeFlow();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  AfeFlow& operator=(const AfeFlow &other);
  /** Equality operator */
  bool operator==(const AfeFlow &other) const;
  /** Inequality operator */
  bool operator!=(const AfeFlow &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const = 0;
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the design flow rate [kg/s or m^3/s]. */
  double Flow() const;
  /** Sets the design flow rate [kg/s or m^3/s]. */
  bool setFlow(const double Flow);
  /** Sets the design flow rate [kg/s or m^3/s]. */
  bool setFlow(const std::string &Flow);
  /** Returns the display units of design flow rate. */
  int u_F() const;
  /** Sets the display units of design flow rate. */
  void setU_F(const int u_F);
  //@}
private:
  boost::shared_ptr<detail::AfeFlowImpl> m_impl;
};

/** The AfeCmf object is an airflow element representing constant mass flow rate. */
class CONTAM_API AfeCmf : public AfeFlow
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfeCmf();
  /** Create a new object. */
  AfeCmf(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfeCmf(int nr,int icon,std::string name,std::string desc,double Flow,int u_F);
  /** Create a new object. */
  AfeCmf(int nr,int icon,std::string name,std::string desc,std::string Flow,int u_F);
  /** Create a new object from another object. */
  //AfeCmf(const AfeCmf &other);
  /** Destroy the object. */
  ~AfeCmf();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element data type. */
  std::string dataType() const {return "fan_cmf";}
  //@}
};

/** The AfeCvf object is an airflow element representing constant volume flow rate. */
class CONTAM_API AfeCvf : public AfeFlow
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfeCvf();
  /** Create a new object. */
  AfeCvf(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfeCvf(int nr,int icon,std::string name,std::string desc,double Flow,int u_F);
  /** Create a new object. */
  AfeCvf(int nr,int icon,std::string name,std::string desc,std::string Flow,int u_F);
  /** Create a new object from another object. */
  //AfeCvf(const AfeCvf &other);
  /** Destroy the object. */
  ~AfeCvf();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element data type. */
  std::string dataType() const {return "fan_cvf";}
  //@}
};

/** The AfeFan object is an airflow element representing a fan with performance data. */
class CONTAM_API AfeFan : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfeFan();
  /** Create a new object. */
  AfeFan(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfeFan(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double rdens,
    double fdf,double sop,double off,std::vector<double> fpc,double Sarea,int u_Sa,
    std::vector<FanDataPoint> data);
  /** Create a new object. */
  AfeFan(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string rdens,
    std::string fdf,std::string sop,std::string off,std::vector<std::string> fpc,std::string Sarea,int u_Sa,
    std::vector<FanDataPoint> data);
  /** Create a new object from another object. */
  AfeFan(const AfeFan &other);
  /** Destroy the object. */
  ~AfeFan();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  AfeFan& operator=(const AfeFan &other);
  /** Equality operator */
  bool operator==(const AfeFan &other) const;
  /** Inequality operator */
  bool operator!=(const AfeFan &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "fan_fan";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the laminar flow coefficient. */
  double lam() const;
  /** Sets the laminar flow coefficient. */
  bool setLam(const double lam);
  /** Sets the laminar flow coefficient. */
  bool setLam(const std::string &lam);
  /** Returns the turbulent flow coefficient. */
  double turb() const;
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const double turb);
  /** Sets the turbulent flow coefficient. */
  bool setTurb(const std::string &turb);
  /** Returns the pressure exponent. */
  double expt() const;
  /** Sets the pressure exponent. */
  bool setExpt(const double expt);
  /** Sets the pressure exponent. */
  bool setExpt(const std::string &expt);
  /** Returns the reference fluid density [kg/m^3]. */
  double rdens() const;
  /** Sets the reference fluid density [kg/m^3]. */
  bool setRdens(const double rdens);
  /** Sets the reference fluid density [kg/m^3]. */
  bool setRdens(const std::string &rdens);
  /** Returns the free delivery flow (prise = 0) [kg/s]. */
  double fdf() const;
  /** Returns the free delivery flow (prise = 0) [kg/s]. */
  bool setFdf(const double fdf);
  /** Returns the free delivery flow (prise = 0) [kg/s]. */
  bool setFdf(const std::string &fdf);
  /** Returns the shut-off pressure (flow = 0) [Pa]. */
  double sop() const;
  /** Sets the shut-off pressure (flow = 0) [Pa]. */
  bool setSop(const double sop);
  /** Sets the shut-off pressure (flow = 0) [Pa]. */
  bool setSop(const std::string &sop);
  /** Returns the minimum RPM fraction: fan is off if (RPM/rated RPM) < off. */
  double off() const;
  /** Returns the minimum RPM fraction: fan is off if (RPM/rated RPM) < off. */
  bool setOff(const double off);
  /** Returns the minimum RPM fraction: fan is off if (RPM/rated RPM) < off. */
  bool setOff(const std::string &off);
  /** Returns the fan performance polynomial coefficients. */
  std::vector<double> fpc() const;
  /** Sets the fan performance polynomial coefficients. */
  bool setFpc(const std::vector<double> &fpc);
  /** Sets the fan performance polynomial coefficients. */
  bool setFpc(const std::vector<std::string> &fpc);
  /** Returns the shut-off orifice area [m^2]. */
  double Sarea() const;
  /** Returns the shut-off orifice area [m^2]. */
  bool setSarea(const double Sarea);
  /** Returns the shut-off orifice area [m^2]. */
  bool setSarea(const std::string &Sarea);
  /** Returns the display units of shut-off orifice area. */
  int u_Sa() const;
  /** Sets the display units of shut-off orifice area. */
  void setU_Sa(const int u_Sa);
  /** Returns the vector of fan data points. */
  std::vector<FanDataPoint> data() const;
  /** Sets the vector of fan data points. */
  void setData(const std::vector<FanDataPoint> &data);
  //@}
private:
  boost::shared_ptr<detail::AfeFanImpl> m_impl;
};

/** The AfeCvf object is the base class of airflow element representing cubic spline leakage. */
class CONTAM_API AfeCsf : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfeCsf();
  /** Create a new object. */
  AfeCsf(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfeCsf(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data);
  /** Create a new object from another object. */
  AfeCsf(const AfeCsf &other);
  /** Destroy the object. */
  ~AfeCsf();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  AfeCsf& operator=(const AfeCsf &other);
  /** Equality operator */
  bool operator==(const AfeCsf &other) const;
  /** Inequality operator */
  bool operator!=(const AfeCsf &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the display units for x. */
  int u_x() const;
  /** Sets the display units for x. */
  void setU_x(const int u_x);
  /** Returns the display units for y. */
  int u_y() const;
  /** Sets the display units for y. */
  void setU_y(const int u_y);
  /** Returns the data point vector of (x,y) pairs. */
  std::vector<XyDataPoint> data() const;
  /** Sets the data point vector. */
  void setData(const std::vector<XyDataPoint> &data);
  //@}
private:
  boost::shared_ptr<detail::AfeCsfImpl> m_impl;
};

/** The AfeFsp object is an airflow element representing cubic spline leakage with mass flow rate as a function of pressure drop. */
class CONTAM_API AfeFsp : public AfeCsf
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfeFsp();
  /** Create a new object. */
  AfeFsp(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfeFsp(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data);
  /** Create a new object from another object. */
  //AfeFsp(const AfeCsf &other);
  /** Destroy the object. */
  ~AfeFsp();
  //@}

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "csf_fsp";}
  //@}
};

/** The AfeQsp object is an airflow element representing cubic spline leakage with volume flow rate as a function of pressure drop. */
class CONTAM_API AfeQsp : public AfeCsf
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfeQsp();
  /** Create a new object. */
  AfeQsp(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfeQsp(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data);
  /** Create a new object from another object. */
  //AfeQsp(const AfeCsf &other);
  /** Destroy the object. */
  ~AfeQsp();
  //@}

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "csf_qsp";}
  //@}
};

/** The AfePsf object is an airflow element representing cubic spline leakage with pressure drop as a function of mass flow rate. */
class CONTAM_API AfePsf : public AfeCsf
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfePsf();
  /** Create a new object. */
  AfePsf(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfePsf(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data);
  /** Create a new object from another object. */
  //AfePsf(const AfePsf &other);
  /** Destroy the object. */
  ~AfePsf();
  //@}

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "csf_psf";}
  //@}
};

/** The AfePsq object is an airflow element representing cubic spline leakage with pressure drop as a function of volume flow rate. */
class CONTAM_API AfePsq : public AfeCsf
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfePsq();
  /** Create a new object. */
  AfePsq(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfePsq(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data);
  /** Create a new object from another object. */
  //AfePsq(const AfePsq &other);
  /** Destroy the object. */
  ~AfePsq();
  //@}

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the element data type. */
  std::string dataType() const {return "csf_psq";}
  //@}
};

/** The AfeSup object is an airflow element that combines multiple elements into a single element. */
class CONTAM_API AfeSup : public AirflowElement
{
public:
  /** @name Constructors and Destructors */
  //@{

  /** Create a new object with default values. */
  AfeSup();
  /** Create a new object. */
  AfeSup(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfeSup(int nr,int icon,std::string name,std::string desc,int sched,int u_H,
    std::vector<AirflowSubelementData> subelements);
  /** Create a new object from another object. */
  AfeSup(const AfeSup &other);
  /** Destroy the object. */
  ~AfeSup();

  //@}
  /** @name Operators */
  //@{

  /** Copy operator */
  AfeSup& operator=(const AfeSup &other);
  /** Equality operator */
  bool operator==(const AfeSup &other) const;
  /** Inequality operator */
  bool operator!=(const AfeSup &other) const;

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Read object details from a Reader. */
  void readDetails(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{
  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the icon used to represent flow path. */
  int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const int icon);
  /** Returns the element data type. */
  std::string dataType() const {return "sup_afe";}
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the scheduled sub-element number (only one). */
  int sched() const;
  /** Sets the scheduled sub-element number (only one). */
  void setSched(const int sched);
  /** Returns the display units of relative height. */
  int u_H() const;
  /** Sets the display units of relative height. */
  void setU_H(const int u_H);
  /** Returns the sub-element data vector. */
  std::vector<AirflowSubelementData> subelements() const;
  /** Sets the sub-element data vector. */
  void setSubelements(const std::vector<AirflowSubelementData> &subelements);
  //@}
private:
  boost::shared_ptr<detail::AfeSupImpl> m_impl;
};

} // contam
} // openstudio

#endif
