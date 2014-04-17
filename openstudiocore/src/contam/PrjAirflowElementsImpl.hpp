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
#ifndef PRJAIRFLOWELEMENTSIMPL_H
#define PRJAIRFLOWELEMENTSIMPL_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "PrjSubobjects.hpp"

namespace openstudio {
namespace contam {
namespace detail {

class PlrOrfImpl
{
public:
  PlrOrfImpl();
  PlrOrfImpl(int nr,int icon,std::string name,std::string desc);
  PlrOrfImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double area,
    double dia,double coef,double Re,int u_A,int u_D);
  PlrOrfImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string area,
    std::string dia,std::string coef,std::string Re,int u_A,int u_D);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
  PRJFLOAT m_area;  // actual area [m^2] (R4) {X}
  PRJFLOAT m_dia;  // hydraulic diameter [m] (R4) {X}
  PRJFLOAT m_coef;  // flow coefficient (R4) {X}
  PRJFLOAT m_Re;  // laminar/turbulet transition Reynolds number [-](R4) {X}
  int m_u_A;  // units of area (I2) {X}
  int m_u_D;  // units of diameter (I2) {X}
};

class PlrLeakImpl
{
public:
  PlrLeakImpl();
  PlrLeakImpl(int nr,int icon,std::string name,std::string desc);
  PlrLeakImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double coef,
    double pres,double area1,double area2,double area3,int u_A1,int u_A2,int u_A3,int u_dP);
  PlrLeakImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string coef,
    std::string pres,std::string area1,std::string area2,std::string area3,int u_A1,int u_A2,int u_A3,int u_dP);
  void read(Reader &reader);
  std::string write(std::string datatype);
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
  PRJFLOAT m_coef;  // flow coefficient (R4) {W}
  PRJFLOAT m_pres;  // reference pressure drop [Pa] (R4) {W}
  PRJFLOAT m_area1;  // leakage area per item [m^2] (R4) {W}
  PRJFLOAT m_area2;  // leakage area per unit length [m^2/m] (R4) {W}
  PRJFLOAT m_area3;  // leakage area per unit area [m^2/m^2] (R4) {W}
  int m_u_A1;  // units of area1 [m^2] (I2) {W}
  int m_u_A2;  // units of area2 [m^2/m] (I2) {W}
  int m_u_A3;  // units of area3 [m^2/m^2] (I2) {W}
  int m_u_dP;  // units of pressure (I2) {W}
};

class PlrConnImpl
{
public:
  PlrConnImpl();
  PlrConnImpl(int nr,int icon,std::string name,std::string desc);
  PlrConnImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double area,
    double coef,int u_A);
  PlrConnImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string area,
    std::string coef,int u_A);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent - 0.5 (R4)
  PRJFLOAT m_area;  // actual area [m^2] (R4) {W}
  PRJFLOAT m_coef;  // flow coefficient (R4) {W}
  int m_u_A;  // units of area (I2) {W}
};

class PlrGeneralImpl
{
public:
  PlrGeneralImpl();
  PlrGeneralImpl(int nr,int icon,std::string name,std::string desc);
  PlrGeneralImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt);
  PlrGeneralImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt);
  void read(Reader &reader);
  std::string write(std::string datatype);
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
};

class PlrTest1Impl
{
public:
  PlrTest1Impl();
  PlrTest1Impl(int nr,int icon,std::string name,std::string desc);
  PlrTest1Impl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dP,
    double Flow,int u_P,int u_F);
  PlrTest1Impl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dP,
    std::string Flow,int u_P,int u_F);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
  PRJFLOAT m_dP;  // pressure drop [Pa] (R4) {W}
  PRJFLOAT m_Flow;  // flow rate [kg/s] (R4) {W}
  int m_u_P;  // units of pressure drop (I2) {W}
  int m_u_F;  // units of flow (I2) {W}
};

class PlrTest2Impl
{
public:
  PlrTest2Impl();
  PlrTest2Impl(int nr,int icon,std::string name,std::string desc);
  PlrTest2Impl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dP1,
    double F1,double dP2,double F2,int u_P1,int u_F1,int u_P2,int u_F2);
  PlrTest2Impl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dP1,
    std::string F1,std::string dP2,std::string F2,int u_P1,int u_F1,int u_P2,int u_F2);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
  PRJFLOAT m_dP1;  // point 1 pressure drop [Pa] (R4) {W}
  PRJFLOAT m_F1;  // point 1 flow rate [kg/s] (R4) {W}
  PRJFLOAT m_dP2;  // point 2 pressure drop [Pa] (R4) {W}
  PRJFLOAT m_F2;  // point 2 flow rate [kg/s] (R4) {W}
  int m_u_P1;  // units of pressure drop (I2) {W}
  int m_u_F1;  // units of flow (I2) {W}
  int m_u_P2;  // units of pressure drop (I2) {W}
  int m_u_F2;  // units of flow (I2) {W}
};

class PlrCrackImpl
{
public:
  PlrCrackImpl();
  PlrCrackImpl(int nr,int icon,std::string name,std::string desc);
  PlrCrackImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double length,
    double width,int u_L,int u_W);
  PlrCrackImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string length,
    std::string width,int u_L,int u_W);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
  PRJFLOAT m_length;  // crack length [m] (R4) {W}
  PRJFLOAT m_width;  // crack width [m] (R4) {W}
  int m_u_L;  // units of length (I2) {W}
  int m_u_W;  // units of width (I2) {W}
};

class PlrStairImpl
{
public:
  PlrStairImpl();
  PlrStairImpl(int nr,int icon,std::string name,std::string desc);
  PlrStairImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,
    double expt,double Ht,double Area,double peo,int tread,int u_A,int u_D);
  PlrStairImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,
    std::string expt,std::string Ht,std::string Area,std::string peo,int tread,int u_A,int u_D);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
  PRJFLOAT m_Ht;  // distance between levels [m] (R4) {W}
  PRJFLOAT m_Area;  // cross-sectional area [m^2] (R4) {W}
  PRJFLOAT m_peo;  // density of people [pers/m^2] (R4) {W}
  int m_tread;  // 1 = open tread 0 = closed (IX) {W}
  int m_u_A;  // units of area (I2) {W}
  int m_u_D;  // units of distance (I2) {W}
};

class PlrShaftImpl
{
public:
  PlrShaftImpl();
  PlrShaftImpl(int nr,int icon,std::string name,std::string desc);
  PlrShaftImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,
    double Ht,double area,double perim,double rough,int u_A,int u_D,int u_P,int u_R);
  PlrShaftImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,
    std::string Ht,std::string area,std::string perim,std::string rough,int u_A,int u_D,int u_P,int u_R);

  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
  PRJFLOAT m_Ht;  // distance between levels [m] (R4) {W}
  PRJFLOAT m_area;  // cross-sectional area [m^2] (R4) {W}
  PRJFLOAT m_perim;  // perimeter [m] (R4) {W}
  PRJFLOAT m_rough;  // roughness [m] (R4) {W}
  int m_u_A;  // units of area (I2) {W}
  int m_u_D;  // units of distance (I2) {W}
  int m_u_P;  // units of perimeter (I2) {W}
  int m_u_R;  // units of roughness (I2) {W}
};

class PlrBackDamperImpl
{
public:
  PlrBackDamperImpl();
  PlrBackDamperImpl(int nr,int icon,std::string name,std::string desc);
  PlrBackDamperImpl(int nr,int icon,std::string name,std::string desc,double lam,double Cp,double xp,double Cn,double xn);
  PlrBackDamperImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string Cp,std::string xp,std::string Cn,std::string xn);
  void read(Reader &reader);
  std::string write(std::string dataType);
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient {R4}
  PRJFLOAT m_Cp;  // turbulent flow coefficient ( dP > 0 ) {R4}
  PRJFLOAT m_xp;  // pressure exponent ( dP > 0 ) {R4}
  PRJFLOAT m_Cn;  // turbulent flow coefficient ( dP < 0 ) {R4}
  PRJFLOAT m_xn;  // pressure exponent ( dP < 0 ) {R4}
};

class QfrQuadraticImpl
{
public:
  QfrQuadraticImpl();
  QfrQuadraticImpl(int nr,int icon,std::string name,std::string desc);
  QfrQuadraticImpl(int nr,int icon,std::string name,std::string desc,double a,double b);
  QfrQuadraticImpl(int nr,int icon,std::string name,std::string desc,std::string a,std::string b);
  void read(Reader &reader);
  std::string write(std::string dataType);
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();
  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_a;  // dP = a*Q + b*Q*Q {R4}
  PRJFLOAT m_b;  // {R4}
};

class QfrCrackImpl
{
public:
  QfrCrackImpl();
  /** Create a new object. */
  QfrCrackImpl(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  QfrCrackImpl(int nr,int icon,std::string name,std::string desc,double a,double b,double length,double width,
    double depth,int nB,int u_L,int u_W,int u_D);
  /** Create a new object. */
  QfrCrackImpl(int nr,int icon,std::string name,std::string desc,std::string a,std::string b,std::string length,std::string width,
    std::string depth,int nB,int u_L,int u_W,int u_D);

  void read(Reader &input);
  void readDetails(Reader &input);
  std::string write();

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_a;  // dP = a*F + b*F*F {R4}
  PRJFLOAT m_b;  // {R4}
  PRJFLOAT m_length;  // crack length [m] {R4}
  PRJFLOAT m_width;  // crack width [m] {R4}
  PRJFLOAT m_depth;  // crack depth [m] {R4}
  int m_nB;  // number of bends (IX)
  int m_u_L;  // units of length (IX)
  int m_u_W;  // units of width (IX)
  int m_u_D;  // units of depth (IX)
};

class QfrTest2Impl
{
public:
  QfrTest2Impl();
  QfrTest2Impl(int nr,int icon,std::string name,std::string desc);
  QfrTest2Impl(int nr,int icon,std::string name,std::string desc,double a,double b,double dP1,double F1,double dP2,
    double F2,int u_P1,int u_F1,int u_P2,int u_F2);
  QfrTest2Impl(int nr,int icon,std::string name,std::string desc,std::string a,std::string b,std::string dP1,std::string F1,std::string dP2,
    std::string F2,int u_P1,int u_F1,int u_P2,int u_F2);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_a;  // dP = a*F + b*F*F {R4}
  PRJFLOAT m_b;  // {R4}
  PRJFLOAT m_dP1;  // point 1 pressure drop [Pa] (R4) {W}
  PRJFLOAT m_F1;  // point 1 flow rate [kg/s] (R4) {W}
  PRJFLOAT m_dP2;  // point 2 pressure drop [Pa] (R4) {W}
  PRJFLOAT m_F2;  // point 2 flow rate [kg/s] (R4) {W}
  int m_u_P1;  // units of pressure drop (I2) {W}
  int m_u_F1;  // units of flow (I2) {W}
  int m_u_P2;  // units of pressure drop (I2) {W}
  int m_u_F2;  // units of flow (I2) {W}
};

class AfeDorImpl
{
public:
  AfeDorImpl();
  /** Create a new object. */
  AfeDorImpl(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  AfeDorImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dTmin,
    double ht,double wd,double cd,int u_T,int u_H,int u_W);
  /** Create a new object. */
  AfeDorImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dTmin,
    std::string ht,std::string wd,std::string cd,int u_T,int u_H,int u_W);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
  PRJFLOAT m_dTmin;  // minimum temperature difference for two-way flow [C] (R4)
  PRJFLOAT m_ht;  // height of doorway [m] (R4)
  PRJFLOAT m_wd;  // width of doorway [m] (R4)
  PRJFLOAT m_cd;  // discharge coefficient (R4)
  int m_u_T;  // units of temperature (I2) {W}
  int m_u_H;  // units of height (I2) {W}
  int m_u_W;  // units of width (I2) {W}
};

class DrPl2Impl
{
public:
  DrPl2Impl();
  /** Create a new object. */
  DrPl2Impl(int nr,int icon,std::string name,std::string desc);
  /** Create a new object. */
  DrPl2Impl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double dH,double ht,
    double wd,double cd,int u_H,int u_W);
  /** Create a new object. */
  DrPl2Impl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string dH,std::string ht,
    std::string wd,std::string cd,int u_H,int u_W);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
  PRJFLOAT m_dH;  // distance above | below midpoint [m] {R4}
  PRJFLOAT m_ht;  // height of doorway [m] (R4) {W in v. 2.0}
  PRJFLOAT m_wd;  // width of doorway [m] (R4) {W}
  PRJFLOAT m_cd;  // discharge coefficient [-] (R4) {W}
  int m_u_H;  // units of height (I2) {W}
  int m_u_W;  // units of width (I2) {W}
};

class AfeFlowImpl
{
public:
  AfeFlowImpl();
  AfeFlowImpl(int nr,int icon,std::string name,std::string desc);
  AfeFlowImpl(int nr,int icon,std::string name,std::string desc,double Flow,int u_F);
  AfeFlowImpl(int nr,int icon,std::string name,std::string desc,std::string Flow,int u_F);
  void read(Reader &reader);
  std::string write(std::string dataType);
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_Flow;  // design flow rate [kg/s] (R4)
  int m_u_F;  // units of flow (I2) {W}
};

class AfeFanImpl
{
public:
  AfeFanImpl();
  AfeFanImpl(int nr,int icon,std::string name,std::string desc);
  AfeFanImpl(int nr,int icon,std::string name,std::string desc,double lam,double turb,double expt,double rdens,
    double fdf,double sop,double off,std::vector<double> fpc,double Sarea,int u_Sa,
    std::vector<FanDataPoint> data);
  AfeFanImpl(int nr,int icon,std::string name,std::string desc,std::string lam,std::string turb,std::string expt,std::string rdens,
    std::string fdf,std::string sop,std::string off,std::vector<std::string> fpc,std::string Sarea,int u_Sa,
    std::vector<FanDataPoint> data);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  PRJFLOAT m_lam;  // laminar flow coefficient (R4)
  PRJFLOAT m_turb;  // turbulent flow coefficient (R4)
  PRJFLOAT m_expt;  // pressure exponent (R4)
  PRJFLOAT m_rdens;  // reference fluid density [kg/m^3] {R4}
  PRJFLOAT m_fdf;  // free delivery flow (prise = 0) [kg/s] {R4}
  PRJFLOAT m_sop;  // shut-off pressure (flow = 0) [Pa] {R4}
  PRJFLOAT m_off;  // fan is off if (RPM/rated RPM) < off {R4}
  std::vector<PRJFLOAT> m_fpc;  // fan performance polynomial coefficients {R4}
  PRJFLOAT m_Sarea;  // shut-off orifice area [m^2] {R4} {W}
  int m_u_Sa;  // units of shut-off area (I2) {W}
  std::vector<FanDataPoint> m_data;  // measured fan data points (FanDataPoint)
};

class AfeCsfImpl
{
public:
  AfeCsfImpl();
  AfeCsfImpl(int nr,int icon,std::string name,std::string desc);
  AfeCsfImpl(int nr,int icon,std::string name,std::string desc,int u_x,int u_y,std::vector<XyDataPoint> data);
  void read(Reader &reader);
  std::string write(std::string datatype);
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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
private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_dataType;  // element data type (CS) element type names are stored in _afe_dnames in dtype order.
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  int m_u_x;  // units for x (I2)
  int m_u_y;  // units for y (I2)
  std::vector<XyDataPoint> m_data;  // data points (XyDataPoint)
};

class AfeSupImpl
{
public:
  AfeSupImpl();
  AfeSupImpl(int nr,int icon,std::string name,std::string desc);
  AfeSupImpl(int nr,int icon,std::string name,std::string desc,int sched,int u_H,
    std::vector<AirflowSubelementData> subelements);
  void read(Reader &reader);
  std::string write();
  void readDetails(Reader &input);

  /** Returns the element number, in order from 1 to the number of airflow elements. */
  int nr() const;
  /** Sets the elemt number. This should only be done with care. */
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

private:
  void setDefaults();

  int m_nr;  // element number (IX); in order from 1 to _nafe
  int m_icon;  // icon used to represent flow path (I2) {W}
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // element description (CS) {W} may be blank
  int m_sched;  // scheduled sub-element number (IX) (only one)
  int m_u_H;  // units of relative height (I2)
  std::vector<AirflowSubelementData> m_subelements;  // Subelement data (AirflowSubelementData)
};

} // detail
} // contam
} // openstudio

#endif
