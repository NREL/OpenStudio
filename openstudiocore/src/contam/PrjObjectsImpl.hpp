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
#ifndef PRJOBJECTSIMPL_H
#define PRJOBJECTSIMPL_H

#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "PrjSubobjects.hpp"

namespace openstudio {
namespace contam {
namespace detail {

class ZoneImpl
{
public:
  ZoneImpl();
  ZoneImpl(int nr,unsigned int flags,int ps,int pc,int pk,int pl,std::string relHt,std::string Vol,std::string T0,std::string P0,
    std::string name,int color,int u_Ht,int u_V,int u_T,int u_P,int cdaxis,int cfd,std::string cfdname,std::string X1,
    std::string Y1,std::string H1,std::string X2,std::string Y2,std::string H2,std::string celldx,std::string axialD,int u_aD,int u_L);
  ZoneImpl(int nr,unsigned int flags,int ps,int pc,int pk,int pl,double relHt,double Vol,double T0,double P0,
    std::string name,int color,int u_Ht,int u_V,int u_T,int u_P,int cdaxis,int cfd,std::string cfdname,double X1,
    double Y1,double H1,double X2,double Y2,double H2,double celldx,double axialD,int u_aD,int u_L);

  void read(Reader &input);
  std::string write();

  /** Returns the zone number, in order from 1 to the number of zones. */
  int nr() const;
  /** Sets the zone number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the zones flags. See the ZoneFlags enum for more information. */
  unsigned int flags() const;
  /** Set the zones flags. See the ZoneFlags enum for more information. */
  void setFlags(const unsigned int flags);
  /** Returns the week schedule index. It is converted to pointer by CONTAM. */
  int ps() const;
  /** Sets the week schedule index. It is converted to pointer by CONTAM. */
  void setPs(const int ps);
  /** Returns the control node index. It is converted to pointer by CONTAM. */
  int pc() const;
  /** Sets the control node index. It is converted to pointer by CONTAM. */
  void setPc(const int pc);
  /** Returns the kinetic reaction index. It is converted to pointer by CONTAM. */
  int pk() const;
  /** Sets the kinetic reaction index. It is converted to pointer by CONTAM. */
  void setPk(const int pk);
  /** Returns the building level index. It is converted to pointer by CONTAM. */
  int pl() const;
  /** Sets the building level index. It is converted to pointer by CONTAM. */
  void setPl(const int pl);
  /** Returns the zone height [m]. */
  double relHt() const;
  /** Sets the zone height [m]. */
  bool setRelHt(const double relHt);
  /** Sets the zone height [m]. */
  bool setRelHt(const std::string &relHt);
  /** Returns the zone volume [m^3]. */
  double Vol() const;
  /** Sets the zone volume [m^3]. */
  bool setVol(const double Vol);
  /** Sets the zone volume [m^3]. */
  bool setVol(const std::string &Vol);
  /** Returns the initial zone temperature [K]. */
  double T0() const;
  /** Sets the initial zone temperature [K]. */
  bool setT0(const double T0);
  /** Sets the initial zone temperature [K]. */
  bool setT0(const std::string &T0);
  /** Returns the initial zone pressure [Pa]. */
  double P0() const;
  /** Sets the initial zone pressure [Pa]. */
  bool setP0(const double P0);
  /** Sets the initial zone pressure [Pa]. */
  bool setP0(const std::string P0);
  /** Returns the zone name. */
  std::string name() const;
  /** Sets the zone name. */
  void setName(const std::string &name);
  /** Returns the zone fill color. */
  int color() const;
  /** Sets the zone fill color. */
  void setColor(const int color);
  /** Returns the units of height. */
  int u_Ht() const;
  /** Sets the units of height. */
  void setU_Ht(const int u_Ht);
  /** Returns the units of volume. */
  int u_V() const;
  /** Sets the units of volume. */
  void setU_V(const int u_V);
  /** Returns the units of temperature. */
  int u_T() const;
  /** Sets the units of temperature. */
  void setU_T(const int u_T);
  /** Returns the units of pressure. */
  int u_P() const;
  /** sets the units of pressure. */
  void setU_P(const int u_P);
  /** Returns the convection/diffusion axis flag (0=no cd, 1-4 => cd axis direction). */
  int cdaxis() const;
  /** Sets the convection/diffusion axis flag (0=no cd, 1-4 => cd axis direction). */
  void setCdaxis(const int cdaxis);
  /** Returns the CFD zone flag (0=no, 1=yes). */
  int cfd() const;
  /** Sets the cfd zone flag (0=no, 1=yes). */
  void setCfd(const int cfd);
  /** Returns the CFD zone name. */
  std::string cfdname() const;
  /** Sets the CFD zone name. */
  void setCfdname(const std::string cfdname);
  /** Returns the X coordinate of one end of the cdaxis. */
  double X1() const;
  /** Sets the X coordinate of one end of the cdaxis. */
  bool setX1(const double X1);
  /** Sets the X coordinate of one end of the cdaxis. */
  bool setX1(const std::string &X1);
  /** Returns the Y coordinate of one end of the cdaxis. */
  double Y1() const;
  /** Sets the Y coordinate of one end of the cdaxis. */
  bool setY1(const double Y1);
  /** Sets the Y coordinate of one end of the cdaxis. */
  bool setY1(const std::string &Y1);
  /** Returns the relative height of one end of the cdaxis. */
  double H1() const;
  /** Sets the relative height of one end of the cdaxis. */
  bool setH1(const double H1);
  /** Sets the relative height of one end of the cdaxis. */
  bool setH1(const std::string &H1);
  /** Returns the X coordinate of the other end of the cdaxis. */
  double X2() const;
  /** Sets the X coordinate of the other end of the cdaxis. */
  bool setX2(const double X2);
  /** Sets the X coordinate of the other end of the cdaxis. */
  bool setX2(const std::string &X2);
  /** Returns the Y coordinate of the other end of the cdaxis. */
  double Y2() const;
  /** Sets the Y coordinate of the other end of the cdaxis. */
  bool setY2(const double Y2);
  /** Sets the Y coordinate of the other end of the cdaxis. */
  bool setY2(const std::string &Y2);
  /** Returns the relative height of the other end of the cdaxis. */
  double H2() const;
  /** Sets the relative height of the other end of the cdaxis. */
  bool setH2(const double H2);
  /** Sets the relative height of the other end of the cdaxis. */
  bool setH2(const std::string &H2);
  /** Returns the convection/diffusion cell length [m]. */
  double celldx() const;
  /** Returns the convection/diffusion cell length [m]. */
  bool setCelldx(const double celldx);
  /** Returns the convection/diffusion cell length [m]. */
  bool setCelldx(const std::string &celldx);
  /** Returns the axial diffusion coeff [m^2/s]. */
  double axialD() const;
  /** Sets the axial diffusion coeff [m^2/s]. */
  bool setAxialD(const double axialD);
  /** Sets the axial diffusion coeff [m^2/s]. */
  bool setAxialD(const std::string &axialD);
  /** Returns the display units of axial diffusion. */
  int u_aD() const;
  /** Sets the display units of axial diffusion. */
  void setU_aD(const int u_aD);
  /** Returns the c/d axis limit display units. */
  int u_L() const;
  /** Sets the c/d axis limit display units. */
  void setU_L(const int u_L);
  /** Returns true if the zone is a variable pressure zone. */
  bool variablePressure() const;
  /** Set the zone pressure behavior. */
  void setVariablePressure(bool b);
  /** Returns true if the zone is a variable contaminant zone. */
  bool variableContaminants() const;
  /** Set the zone contaminant behavior. */
  void setVariableContaminants(bool b);
  /** Returns true if the zone is a system zone. */
  bool system() const;
  /** Set the zone system flag. */
  void setSystem(bool b);
  /** Returns the initial condition of contaminant i. */
  double ic(const int i) const;
  /** Returns the contaminant initial conditions as a vector. */
  std::vector<double> ic() const;
  /** Sets the initial condition of contaminant i. */
  bool setIc(const int i, const double value);
  /** Sets the initial condition of contaminant i. */
  bool setIc(const int i, const std::string &value);
  /** Sets the contaminant initial condition vector. */
  bool setIc(std::vector<double> &ic);
  /** Sets the contaminant initial condition vector. */
  bool setIc(std::vector<std::string> &ic);

private:
  void setDefaults();

  int m_nr;  // zone number (IX); in order from 1 to _nzone
  unsigned int m_flags;  // zone flags - bits defined in contam.h (U2)
  int m_ps;  // week schedule index (IX); converted to pointer
  int m_pc;  // control node index (IX); converted to pointer
  int m_pk;  // kinetic reaction index (IX); converted to pointer
  int m_pl;  // building level index (IX); converted to pointer
  PRJFLOAT m_relHt;  // zone height [m] (R4)
  PRJFLOAT m_Vol;  // zone volume [m^3] (R4)
  PRJFLOAT m_T0;  // initial zone temperature [K] (R4)
  PRJFLOAT m_P0;  // initial zone pressure [Pa] (R4)
  std::string m_name;  // zone name (CS) {W}
  int m_color;  // zone fill color (I2) {W} {Contam 2.4}
  int m_u_Ht;  // units of height (I2) {W}
  int m_u_V;  // units of volume (I2) {W}
  int m_u_T;  // units of temperature (I2) {W}
  int m_u_P;  // units of pressure (I2) {W}
  int m_cdaxis;  // conv/diff axis (0=no cd, 1-4 => cd axis direction) (I2)
  int m_cfd;  // cfd zone (0=no, 1=yes) (I2)
  std::string m_cfdname;  // cfd zone id (CS)
  PRJFLOAT m_X1;  // X coordinate of one end of cdaxis (RX)
  PRJFLOAT m_Y1;  // Y coordinate of one end of cdaxis (RX)
  PRJFLOAT m_H1;  // Relative Height of one end of cdaxis (RX)
  PRJFLOAT m_X2;  // X coordinate of other end of cdaxis (RX)
  PRJFLOAT m_Y2;  // Y coordinate of other end of cdaxis (RX)
  PRJFLOAT m_H2;  // Relative Height of other end of cdaxis (RX)
  PRJFLOAT m_celldx;  // length of c/d cell [m] (R4)
  PRJFLOAT m_axialD;  // axial diffusion coeff [m^2/s] (R4)
  int m_u_aD;  // units of axial diffusion (I2)
  int m_u_L;  // units of c/d axis limits (I2)

  QVector<PRJFLOAT> m_ic;

};

class SpeciesImpl
{
public:
  /** @name Constructors */
  //@{
  SpeciesImpl();
  SpeciesImpl(int nr,int sflag,int ntflag,std::string molwt,std::string mdiam,std::string edens,std::string decay,std::string Dm,
    std::string ccdef,std::string Cp,int uc,int umd,int ued,int udm,int ucp,std::string name,std::string desc);
  SpeciesImpl(int nr,int sflag,int ntflag,double molwt,double mdiam,double edens,double decay,
    double Dm,double ccdef,double Cp,int ucc,int umd,int ued,int udm,int ucp,std::string name,
    std::string desc);

  //@}
  /** @name Input and Output */
  //@{

  /** Read an object from a Reader. */
  void read(Reader &input);
  /** Write the object to a string. */
  std::string write();

  //@}
  /** @name Getters and Setters */
  //@{

  /** Returns the species number, in order from 1 to the number of species. */
  int nr() const;
  /** Sets the species number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the simulation flag: 1 = simulated, 0 = unsimulated species. */
  int sflag() const;
  /** Sets the simulation flag: 1 = simulated, 0 = unsimulated species. */
  void setSflag(const int sflag);
  /** Returns the non-trace contaminant flag: 1 = non-trace, 0 = trace species. */
  int ntflag() const;
  /** Sets the non-trace contaminant flag: 1 = non-trace, 0 = trace species. */
  void setNtflag(const int ntflag);
  /** Returns the molar mass [kg/kmol] for gas contaminants. */
  double molwt() const;
  /** Sets the molar mass [kg/kmol] for gas contaminants. */
  bool setMolwt(const double molwt);
  /** Sets the molar mass [kg/kmol] for gas contaminants. */
  bool setMolwt(const std::string &molwt);
  /** Returns the mean diameter [m] for particle contaminants. */
  double mdiam() const;
  /** Sets the mean diameter [m] for particle contaminants. */
  bool setMdiam(const double mdiam);
  /** Sets the mean diameter [m] for particle contaminants. */
  bool setMdiam(const std::string &mdiam);
  /** Returns the effective density [kg/m^3] for particle contaminants. */
  double edens() const;
  /** Sets the effective density [kg/m^3] for particle contaminants. */
  bool setEdens(const double edens);
  /** Sets the effective density [kg/m^3] for particle contaminants. */
  bool setEdens(const std::string &edens);
  /** Returns the contaminant decay constant [1/s]. */
  double decay() const;
  /** Sets the contaminant decay constant [1/s]. */
  bool setDecay(const double decay);
  /** Sets the contaminant decay constant [1/s]. */
  bool setDecay(const std::string &decay);
  /** Returns the contaminant molecular diffusion coefficient [m^2/s]. */
  double Dm() const;
  /** Sets the contaminant molecular diffusion coefficient [m^2/s]. */
  bool setDm(const double Dm);
  /** Sets the contaminant molecular diffusion coefficient [m^2/s]. */
  bool setDm(const std::string &Dm);
  /** Returns the default contaminant concentration [kg/kg air]. */
  double ccdef() const;
  /** Sets the default contaminant concentration [kg/kg air]. */
  bool setCcdef(const double ccdef);
  /** Sets the default contaminant concentration [kg/kg air]. */
  bool setCcdef(const std::string &ccdef);
  /** Returns the contaminant specific heat at constant pressure [J/kgK]. This is not used by CONTAM. */
  double Cp() const;
  /** Sets the contaminant specific heat at constant pressure [J/kgK]. This is not used by CONTAM. */
  bool setCp(const double Cp);
  /** Sets the contaminant specific heat at constant pressure [J/kgK]. This is not used by CONTAM. */
  bool setCp(const std::string &Cp);
  /** Returns the concentration display units. */
  int ucc() const;
  /** Sets the concentration display units. */
  void setUcc(const int ucc);
  /** Returns the mean diameter display units. */
  int umd() const;
  /** Sets the mean diameter display units. */
  void setUmd(const int umd);
  /** Returns the effective density display units. */
  int ued() const;
  /** Sets the effective density display units. */
  void setUed(const int ued);
  /** Returns the diffusion coefficient display units. */
  int udm() const;
  /** Sets the diffusion coefficient display units. */
  void setUdm(const int udm);
  /** Returns the specific heat display units. */
  int ucp() const;
  /** Sets the specific heat display units. */
  void setUcp(const int ucp);
  /** Returns the species name. */
  std::string name() const;
  /** Sets the species name. */
  void setName(const std::string &name);
  /** Returns the species description. */
  std::string desc() const;
  /** Sets the species description. */
  void setDesc(const std::string &desc);

private:
  void setDefaults();

  int m_nr;  // species number (IX), in order from 1 to _nspcs
  int m_sflag;  // 1 = simulated, 0 = unsimulated species (I2) {W}
  int m_ntflag;  // 1 = non-trace, 0 = trace species (I2) {W}
  PRJFLOAT m_molwt;  // molar mass [kg/kmol] - gas (R4)
  PRJFLOAT m_mdiam;  // mean diameter - particle [m] (R4)
  PRJFLOAT m_edens;  // effective density - particle [kg/m^3] (R4)
  PRJFLOAT m_decay;  // decay constant [1/s] (R4) {W}
  PRJFLOAT m_Dm;  // molecular diffusion coefficient [m2/s] (R4)
  PRJFLOAT m_ccdef;  // default concentration [kg/kg air] (R4)
  PRJFLOAT m_Cp;  // (unused) specific heat at constant pressure [J/kgK] (R4)
  int m_ucc;  // units to display concentration (I2) {W}
  int m_umd;  // units to display mean diameter (I2) {W}
  int m_ued;  // units to display effective density (I2) {W}
  int m_udm;  // units to display diffusion coefficient (I2) {W}
  int m_ucp;  // units to display specific heat (I2) {W}
  std::string m_name;  // species name (CS)
  std::string m_desc;  // species description (CS) {W}
};

class AhsImpl
{
public:
  AhsImpl();
  AhsImpl(int nr,int zone_r,int zone_s,int path_r,int path_s,int path_x,std::string name,std::string desc);
  void read(Reader &reader);
  std::string write();

  /** Returns the AHS number, in order from 1 to the number of systems. */
  int nr() const;
  /** Sets the AHS number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the return zone number. */
  int zone_r() const;
  /** Sets the return zone number. */
  void setZone_r(const int zone_r);
  /** Returns the supply zone number. */
  int zone_s() const;
  /** Sets the supply zone number. */
  void setZone_s(const int zone_s);
  /** Returns the recirculation air path number. */
  int path_r() const;
  /** Sets the recirculation air path number. */
  void setPath_r(const int path_r);
  /** Returns the outdoor air path number. */
  int path_s() const;
  /** Sets the outdoor air path number. */
  void setPath_s(const int path_s);
  /** Returns the exhaust path number. */
  int path_x() const;
  /** Sets the exhaust path number. */
  void setPath_x(const int path_x);
  /** Returns the AHS name. */
  std::string name() const;
  /** Sets the AHS name. */
  void setName(const std::string &name);
  /** Returns the AHS description. */
  std::string desc() const;
  /** Sets the AHS description. */
  void setDesc(const std::string &desc);

private:
  void setDefaults();

  int m_nr;  // AHS number (IX); in order from 1 to _nahs
  int m_zone_r;  // return zone number (IX)
  int m_zone_s;  // supply zone number (IX)
  int m_path_r;  // recircultaion air path number (IX)
  int m_path_s;  // outdoor air path number (IX)
  int m_path_x;  // exhaust path number (IX)
  std::string m_name;  // AHS name (CS)
  std::string m_desc;  // AHS description (CS) {W} may be blank
};

class AirflowPathImpl
{
public:
  AirflowPathImpl();
  AirflowPathImpl(int nr,int flags,int pzn,int pzm,int pe,int pf,int pw,int pa,int ps,int pc,int pld,std::string X,
    std::string Y,std::string relHt,std::string mult,std::string wPset,std::string wPmod,std::string wazm,std::string Fahs,std::string Xmax,
    std::string Xmin,unsigned int icon,unsigned int dir,int u_Ht,int u_XY,int u_dP,int u_F,int cfd,
    std::string cfd_name,int cfd_ptype,int cfd_btype,int cfd_capp);
  AirflowPathImpl(int nr,int flags,int pzn,int pzm,int pe,int pf,int pw,int pa,int ps,int pc,int pld,double X,
    double Y,double relHt,double mult,double wPset,double wPmod,double wazm,double Fahs,double Xmax,
    double Xmin,unsigned int icon,unsigned int dir,int u_Ht,int u_XY,int u_dP,int u_F,int cfd,
    std::string cfd_name,int cfd_ptype,int cfd_btype,int cfd_capp);
  void read(Reader &reader);
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

  /** Returns the path number, in order from 1 to the number of paths. */
  int nr() const;
  /** Sets the path number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the airflow path flag value. */
  int flags() const;
  /** Sets the airflow path flag value. */
  void setFlags(const int flags);
  /** Returns the zone N index, with positive flow from N to M. It is converted to pointer by CONTAM. */
  int pzn() const;
  /** Sets the zone N index, with positive flow from N to M. It is converted to pointer by CONTAM. */
  void setPzn(const int pzn);
  /** Returns the zone M index, with positive flow from N to M. It is converted to pointer by CONTAM. */
  int pzm() const;
  /** Sets the zone M index, with positive flow from N to M. It is converted to pointer by CONTAM. */
  void setPzm(const int pzm);
  /** Returns the flow element index. It is converted to pointer by CONTAM. */
  int pe() const;
  /** Sets the flow element index. It is converted to pointer by CONTAM. */
  void setPe(const int pe);
  /** Returns the filter index. It is converted to pointer by CONTAM. */
  int pf() const;
  /** Sets the filter index. It is converted to pointer by CONTAM. */
  void setPf(const int pf);
  /** Returns the wind coefficients index. It is converted to pointer by CONTAM. */
  int pw() const;
  /** Sets the wind coefficients index. It is converted to pointer by CONTAM. */
  void setPw(const int pw);
  /** Returns the AHS index. It is converted to pointer by CONTAM. */
  int pa() const;
  /** Sets the AHS index. It is converted to pointer by CONTAM. */
  void setPa(const int pa);
  /** Returns the schedule index. It is converted to pointer by CONTAM. */
  int ps() const;
  /** Sets the schedule index. It is converted to pointer by CONTAM. */
  void setPs(const int ps);
  /** Returns the control node index. It is converted to pointer by CONTAM. */
  int pc() const;
  /** Sets the control node index. It is converted to pointer by CONTAM. */
  void setPc(const int pc);
  /** Returns the level index. It is converted to pointer by CONTAM. */
  int pld() const;
  /** Sets the level index. It is converted to pointer by CONTAM. */
  void setPld(const int pld);
  /** Returns the X-coordinate of an envelope path [m] if set. */
  double X() const;
  /** Sets the X-coordinate of an envelope path [m]. */
  bool setX(const double X);
  /** Sets the X-coordinate of an envelope path [m]. */
  bool setX(const std::string &X);
  /** Returns the Y-coordinate of an envelope path [m] if set. */
  double Y() const;
  /** Sets the Y-coordinate of an envelope path [m]. */
  bool setY(const double Y);
  /** Sets the Y-coordinate of an envelope path [m]. */
  bool setY(const std::string &Y);
  /** Returns the height relative to current level [m]. */
  double relHt() const;
  /** Sets the height relative to current level [m]. */
  bool setRelHt(const double relHt);
  /** Sets the height relative to current level [m]. */
  bool setRelHt(const std::string &relHt);
  /** Return the element multiplier. */
  double mult() const;
  /** Sets the element multiplier. */
  bool setMult(const double mult);
  /** Sets the element multiplier. */
  bool setMult(const std::string &mult);
  /** Returns the constant wind pressure [Pa] (pw==NULL). */
  double wPset() const;
  /** Sets the constant wind pressure [Pa] (pw==NULL). */
  bool setWPset(const double wPset);
  /** Sets the constant wind pressure [Pa] (pw==NULL). */
  bool setWPset(const std::string &wPset);
  /** Returns the wind speed modifier (pw!=NULL). */
  double wPmod() const;
  /** Sets the wind speed modifier (pw!=NULL). */
  bool setWPmod(const double wPmod);
  /** Sets the wind speed modifier (pw!=NULL). */
  bool setWPmod(const std::string &wPmod);
  /** Returns the wall azimuth angle in degrees (pw!=NULL). */
  double wazm() const;
  /** Sets the wall azimuth angle in degrees (pw!=NULL). */
  bool setWazm(const double wazm);
  /** Sets the wall azimuth angle in degrees (pw!=NULL). */
  bool setWazm(const std::string &wazm);
  /** Returns the AHS path flow rate [kg/s] (pw==NULL). */
  double Fahs() const;
  /** Sets the AHS path flow rate [kg/s] (pw==NULL). */
  bool setFahs(const double Fahs);
  /** Sets the AHS path flow rate [kg/s] (pw==NULL). */
  bool setFahs(const std::string &Fahs);
  /** Returns the flow or pressure maximum. */
  double Xmax() const;
  /** Sets the flow or pressure maximum. */
  bool setXmax(const double Xmax);
  /** Sets the flow or pressure maximum. */
  bool setXmax(const std::string &Xmax);
  /** Returns the flow or pressure minimum. */
  double Xmin() const;
  /** Sets the flow or pressure minimum. */
  bool setXmin(const double Xmin);
  /** Sets the flow or pressure minimum. */
  bool setXmin(const std::string &Xmin);
  /** Returns the icon used to represent flow path. */
  unsigned int icon() const;
  /** Sets the icon used to represent flow path. */
  void setIcon(const unsigned int icon);
  /** Returns the positive flow direction on the sketchpad. */
  unsigned int dir() const;
  /** Sets the positive flow direction on the sketchpad. */
  void setDir(const unsigned int dir);
  /** Returns the height display units. */
  int u_Ht() const;
  /** Sets the height display units. */
  void setU_Ht(const int u_Ht);
  /** Returns the X and Y display units. */
  int u_XY() const;
  /** Sets the X and Y display units. */
  void setU_XY(const int u_XY);
  /** Returns the pressure difference display units. */
  int u_dP() const;
  /** Sets the pressure difference display units. */
  void setU_dP(const int u_dP);
  /** Returns the flow display units. */
  int u_F() const;
  /** Sets the flow display units. */
  void setU_F(const int u_F);
  /** Returns the CFD path flag (0=no, 1=yes). */
  int cfd() const;
  /** Sets the CFD path flag (0=no, 1=yes). */
  void setCfd(const int cfd);
  /** Returns the CFD path id. */
  std::string cfd_name() const;
  /** Sets the CFD path id. */
  void setCfd_name(const std::string &cfd_name);
  /** Returns the CFD boundary condition type (0=mass flow, 1=pressure). */
  int cfd_ptype() const;
  /** Sets the CFD boundary condition type (0=mass flow, 1=pressure). */
  void setCfd_ptype(const int cfd_ptype);
  /** Returns the pressure bc type (if ptype = 1, 0=linear, 1=stagnation pressure). */
  int cfd_btype() const;
  /** Sets the pressure bc type (if ptype = 1, 0=linear, 1=stagnation pressure). */
  void setCfd_btype(const int cfd_btype);
  /** Returns the coupling approach (1=pressure-pressure). */
  int cfd_capp() const;
  /** Sets the coupling approach (1=pressure-pressure). */
  void setCfd_capp(const int cfd_capp);

private:
  void setDefaults();

  int m_nr;  // path number (IX); in order from 1 to _npath
  int m_flags;  // airflow path flag values (I2)
  int m_pzn;  // zone N index (IX); converted to pointer
  int m_pzm;  // zone M index (IX); converted to pointer
  int m_pe;  // flow element index (IX); converted to pointer
  int m_pf;  // filter index (IX); converted to pointer
  int m_pw;  // wind coefficients index (IX); converted to pointer
  int m_pa;  // AHS index (IX); converted to pointer
  int m_ps;  // schedule index (IX); converted to pointer
  int m_pc;  // control node index (IX); converted to pointer
  int m_pld;  // level index (IX); converted to pointer
  PRJFLOAT m_X;  // X-coordinate of envelope path [m] (R4) {Contam 2.1}
  PRJFLOAT m_Y;  // Y-coordinate of envelope path [m] (R4) {Contam 2.1}
  PRJFLOAT m_relHt;  // height relative to current level [m] (R4)
  PRJFLOAT m_mult;  // element multiplier (R4)
  PRJFLOAT m_wPset;  // constant wind pressure [Pa] (pw==NULL) (R4)
  PRJFLOAT m_wPmod;  // wind speed(?) modifier (pw!=NULL) (R4)
  PRJFLOAT m_wazm;  // wall azimuth angle (pw!=NULL) (R4)
  PRJFLOAT m_Fahs;  // AHS path flow rate [kg/s] (pw==NULL) (R4)
  PRJFLOAT m_Xmax;  // flow or pressure limit - maximum (R4) {W}
  PRJFLOAT m_Xmin;  // flow or pressure limit - minimum (R4) {W}
  unsigned int m_icon;  // icon used to represent flow path (U1) {W}
  unsigned int m_dir;  // positive flow direction on sketchpad (U1) {W}
  int m_u_Ht;  // units of height (I2) {W}
  int m_u_XY;  // units of X and Y (I2) {W}
  int m_u_dP;  // units of pressure difference (I2) {W}
  int m_u_F;  // units of flow (I2) {W}
  int m_cfd;  // cfd path (0=no, 1=yes) (I2) {CONTAM 3.0}
  std::string m_cfd_name;  // cfd path id (CS)
  int m_cfd_ptype;  // boundary condition type (0=mass flow, 1=pressure) (I2)
  int m_cfd_btype;  // pressure bc type (if ptype = 1, 0=linear, 1=stagnation pressure) (I2)
  int m_cfd_capp;  // coupling approach (1=pressure-pressure) (I2)
};

class RunControlImpl
{
public:
  RunControlImpl();
  RunControlImpl(int sim_af,int afcalc,int afmaxi,double afrcnvg,double afacnvg,double afrelax,int uac2,
    double Pres,int uPres,int afslae,int afrseq,int aflmaxi,double aflcnvg,int aflinit,int Tadj,
    int sim_mf,int ccmaxi,double ccrcnvg,double ccacnvg,double ccrelax,int uccc,int mfnmthd,
    int mfnrseq,int mfnmaxi,double mfnrcnvg,double mfnacnvg,double mfnrelax,double mfngamma,
    int uccn,int mftmthd,int mftrseq,int mftmaxi,double mftrcnvg,double mftacnvg,double mftrelax,
    double mftgamma,int ucct,int mfvmthd,int mfvrseq,int mfvmaxi,double mfvrcnvg,double mfvacnvg,
    double mfvrelax,int uccv,int mf_solver,int sim_1dz,int sim_1dd,double celldx,int sim_vjt,
    int udx,int cvode_mth,double cvode_rcnvg,double cvode_acnvg,double cvode_dtmax,int tsdens,
    double tsrelax,int tsmaxi,int cnvgSS,int densZP,int stackD,int dodMdt,std::string date_st,
    std::string time_st,std::string date_0,std::string time_0,std::string date_1,std::string time_1,std::string time_step,
    std::string time_list,std::string time_scrn,int restart,std::string rstdate,std::string rsttime,int list,
    int doDlg,int pfsave,int zfsave,int zcsave,int achvol,int achsave,int abwsave,int cbwsave,
    int expsave,int ebwsave,int zaasave,int zbwsave,int rzfsave,int rzmsave,int rz1save,
    int csmsave,int srfsave,int logsave,std::vector<int> save,std::vector<double> rvals,int BldgFlowZ,
    int BldgFlowD,int BldgFlowC,int cfd_ctype,double cfd_convcpl,int cfd_var,int cfd_zref,
    int cfd_imax,int cfd_dtcmo);
  RunControlImpl(int sim_af,int afcalc,int afmaxi,std::string afrcnvg,std::string afacnvg,std::string afrelax,int uac2,
    std::string Pres,int uPres,int afslae,int afrseq,int aflmaxi,std::string aflcnvg,int aflinit,int Tadj,
    int sim_mf,int ccmaxi,std::string ccrcnvg,std::string ccacnvg,std::string ccrelax,int uccc,int mfnmthd,
    int mfnrseq,int mfnmaxi,std::string mfnrcnvg,std::string mfnacnvg,std::string mfnrelax,std::string mfngamma,
    int uccn,int mftmthd,int mftrseq,int mftmaxi,std::string mftrcnvg,std::string mftacnvg,std::string mftrelax,
    std::string mftgamma,int ucct,int mfvmthd,int mfvrseq,int mfvmaxi,std::string mfvrcnvg,std::string mfvacnvg,
    std::string mfvrelax,int uccv,int mf_solver,int sim_1dz,int sim_1dd,std::string celldx,int sim_vjt,
    int udx,int cvode_mth,std::string cvode_rcnvg,std::string cvode_acnvg,std::string cvode_dtmax,int tsdens,
    std::string tsrelax,int tsmaxi,int cnvgSS,int densZP,int stackD,int dodMdt,std::string date_st,
    std::string time_st,std::string date_0,std::string time_0,std::string date_1,std::string time_1,std::string time_step,
    std::string time_list,std::string time_scrn,int restart,std::string rstdate,std::string rsttime,int list,
    int doDlg,int pfsave,int zfsave,int zcsave,int achvol,int achsave,int abwsave,int cbwsave,
    int expsave,int ebwsave,int zaasave,int zbwsave,int rzfsave,int rzmsave,int rz1save,
    int csmsave,int srfsave,int logsave,std::vector<int> save,std::vector<std::string> rvals,int BldgFlowZ,
    int BldgFlowD,int BldgFlowC,int cfd_ctype,std::string cfd_convcpl,int cfd_var,int cfd_zref,
    int cfd_imax,int cfd_dtcmo);
  void read(Reader &reader);
  std::string write();

  /** Returns the airflow simulation flag: 0 = steady, 1 = dynamic. */
  int sim_af() const;
  /** Sets the airflow simulation flag: 0 = steady, 1 = dynamic. */
  void setSim_af(const int sim_af);
  /** Returns the N-R method for non-linear eqns: 0 = SUR, 1 = STR. */
  int afcalc() const;
  /** Sets the N-R method for non-linear eqns: 0 = SUR, 1 = STR. */
  void setAfcalc(const int afcalc);
  /** Returns the maximum number of N-R iterations. */
  int afmaxi() const;
  /** Sets the maximum number of N-R iterations. */
  void setAfmaxi(const int afmaxi);
  /** Returns the relative airflow convergence factor. */
  double afrcnvg() const;
  /** Sets the relative airflow convergence factor. */
  bool setAfrcnvg(const double afrcnvg);
  /** Sets the relative airflow convergence factor. */
  bool setAfrcnvg(const std::string &afrcnvg);
  /** Returns the absolute airflow convergence factor [1/s]. */
  double afacnvg() const;
  /** Sets the absolute airflow convergence factor [1/s]. */
  bool setAfacnvg(const double afacnvg);
  /** Sets the absolute airflow convergence factor [1/s]. */
  bool setAfacnvg(const std::string &afacnvg);
  /** Returns the flow under-relaxation coefficient (for SUR). */
  double afrelax() const;
  /** Sets the flow under-relaxation coefficient (for SUR). */
  bool setAfrelax(const double afrelax);
  /** Sets the flow under-relaxation coefficient (for SUR). */
  bool setAfrelax(const std::string &afrelax);
  /** Returns the display units for afacnvg. */
  int uac2() const;
  /** Sets the display units for afacnvg. */
  void setUac2(const int uac2);
  /** Returns the . */
  double Pres() const;
  /** Sets the . */
  bool setPres(const double Pres);
  /** Sets the . */
  bool setPres(const std::string &Pres);
  /** Returns the pressure test pressure. */
  int uPres() const;
  /** Sets the pressure test pressure. */
  void setUPres(const int uPres);
  /** Returns the method for linear equations: 0 = SKY, 1 = PCG. */
  int afslae() const;
  /** Sets the method for linear equations: 0 = SKY, 1 = PCG. */
  void setAfslae(const int afslae);
  /** Returns the linear equation resequence flag: 0 = don't resequence, 1 = resequence. */
  int afrseq() const;
  /** Sets the linear equation resequence flag: 0 = don't resequence, 1 = resequence. */
  void setAfrseq(const int afrseq);
  /** Returns the maximum number of iterations (PCG). */
  int aflmaxi() const;
  /** Sets the maximum number of iterations (PCG). */
  void setAflmaxi(const int aflmaxi);
  /** Returns the relative convergence factor for (PCG). */
  double aflcnvg() const;
  /** Sets the relative convergence factor for (PCG). */
  bool setAflcnvg(const double aflcnvg);
  /** Sets the relative convergence factor for (PCG). */
  bool setAflcnvg(const std::string &aflcnvg);
  /** Returns the linear airflow initialization flag: 0 = don't use, 1 = use. */
  int aflinit() const;
  /** Sets the linear airflow initialization flag: 0 = don't use, 1 = use. */
  void setAflinit(const int aflinit);
  /** Returns the temperature adjustment flag: 0 = don't use, 1 = use. */
  int Tadj() const;
  /** Sets the temperature adjustment flag: 0 = don't use, 1 = use. */
  void setTadj(const int Tadj);
  /** Returns the mass fraction (contaminant) simulation method: 0 = none, 1 = steady, 2 = transient, 3 = cyclic. */
  int sim_mf() const;
  /** Sets the mass fraction (contaminant) simulation method: 0 = none, 1 = steady, 2 = transient, 3 = cyclic. */
  void setSim_mf(const int sim_mf);
  /** Returns the maximum number of cyclic iterations. */
  int ccmaxi() const;
  /** Sets the maximum number of cyclic iterations. */
  void setCcmaxi(const int ccmaxi);
  /** Returns the cyclic relative convergence factor. */
  double ccrcnvg() const;
  /** Sets the cyclic relative convergence factor. */
  bool setCcrcnvg(const double ccrcnvg);
  /** Sets the cyclic relative convergence factor. */
  bool setCcrcnvg(const std::string &ccrcnvg);
  /** Returns the cyclic absolute convergence factor [kg/kg]. */
  double ccacnvg() const;
  /** Sets the cyclic absolute convergence factor [kg/kg]. */
  bool setCcacnvg(const double ccacnvg);
  /** Sets the cyclic absolute convergence factor [kg/kg]. */
  bool setCcacnvg(const std::string &ccacnvg);
  /** Returns the cyclic (unused) over-relaxation coefficient. */
  double ccrelax() const;
  /** Sets the cyclic (unused) over-relaxation coefficient. */
  bool setCcrelax(const double ccrelax);
  /** Sets the cyclic (unused) over-relaxation coefficient. */
  bool setCcrelax(const std::string &ccrelax);
  /** Returns the display units for ccacnvg. */
  int uccc() const;
  /** Sets the display units for ccacnvg. */
  void setUccc(const int uccc);
  /** Returns the non-trace contaminant simulation method: 0 = SKY, 1 = BCG, 2 = SOR, 3 = LU. */
  int mfnmthd() const;
  /** Sets the non-trace contaminant simulation method: 0 = SKY, 1 = BCG, 2 = SOR, 3 = LU. */
  void setMfnmthd(const int mfnmthd);
  /** Returns the non-trace contaminant resequence flag: 0 = don't resequence, 1 = resequence. */
  int mfnrseq() const;
  /** Sets the non-trace contaminant resequence flag: 0 = don't resequence, 1 = resequence. */
  void setMfnrseq(const int mfnrseq);
  /** Returns the non-trace contaminant maximum iterations. */
  int mfnmaxi() const;
  /** Sets the non-trace contaminant maximum iterations. */
  void setMfnmaxi(const int mfnmaxi);
  /** Returns the non-trace contaminant relative convergence factor. */
  double mfnrcnvg() const;
  /** Sets the non-trace contaminant relative convergence factor. */
  bool setMfnrcnvg(const double mfnrcnvg);
  /** Sets the non-trace contaminant relative convergence factor. */
  bool setMfnrcnvg(const std::string &mfnrcnvg);
  /** Returns the non-trace contaminant absolute convergence factor. */
  double mfnacnvg() const;
  /** Sets the non-trace contaminant absolute convergence factor. */
  bool setMfnacnvg(const double mfnacnvg);
  /** Sets the non-trace contaminant absolute convergence factor. */
  bool setMfnacnvg(const std::string &mfnacnvg);
  /** Returns the non-trace contaminant relaxation coefficient. */
  double mfnrelax() const;
  /** Sets the non-trace contaminant relaxation coefficient. */
  bool setMfnrelax(const double mfnrelax);
  /** Sets the non-trace contaminant relaxation coefficient. */
  bool setMfnrelax(const std::string &mfnrelax);
  /** Returns the non-trace contaminant trapezoidal integration factor. */
  double mfngamma() const;
  /** Sets the non-trace contaminant trapezoidal integration factor. */
  bool setMfngamma(const double mfngamma);
  /** Sets the non-trace contaminant trapezoidal integration factor. */
  bool setMfngamma(const std::string &mfngamma);
  /** Returns the display units for mfnacnvg. */
  int uccn() const;
  /** Sets the display units for mfnacnvg. */
  void setUccn(const int uccn);
  /** Returns the trace contaminant simulation method: 0 = SKY, 1 = BCG, 2 = SOR, 3 = LU. */
  int mftmthd() const;
  /** Sets the trace contaminant simulation method: 0 = SKY, 1 = BCG, 2 = SOR, 3 = LU. */
  void setMftmthd(const int mftmthd);
  /** Returns the trace contaminant resequence flag: 0 = don't resequence, 1 = resequence. */
  int mftrseq() const;
  /** Sets the trace contaminant resequence flag: 0 = don't resequence, 1 = resequence. */
  void setMftrseq(const int mftrseq);
  /** Returns the trace contaminant maximum iterations. */
  int mftmaxi() const;
  /** Sets the trace contaminant maximum iterations. */
  void setMftmaxi(const int mftmaxi);
  /** Returns the trace contaminant relative convergence factor. */
  double mftrcnvg() const;
  /** Sets the trace contaminant relative convergence factor. */
  bool setMftrcnvg(const double mftrcnvg);
  /** Sets the trace contaminant relative convergence factor. */
  bool setMftrcnvg(const std::string &mftrcnvg);
  /** Returns the trace contaminant absolute convergence factor. */
  double mftacnvg() const;
  /** Sets the trace contaminant absolute convergence factor. */
  bool setMftacnvg(const double mftacnvg);
  /** Sets the trace contaminant absolute convergence factor. */
  bool setMftacnvg(const std::string &mftacnvg);
  /** Returns the trace contaminant relaxation coefficient. */
  double mftrelax() const;
  /** Sets the trace contaminant relaxation coefficient. */
  bool setMftrelax(const double mftrelax);
  /** Sets the trace contaminant relaxation coefficient. */
  bool setMftrelax(const std::string &mftrelax);
  /** Returns the trace contaminant trapezoidal integration factor. */
  double mftgamma() const;
  /** Sets the trace contaminant trapezoidal integration factor. */
  bool setMftgamma(const double mftgamma);
  /** Sets the trace contaminant trapezoidal integration factor. */
  bool setMftgamma(const std::string &mftgamma);
  /** Returns the display units for mfnacnvg. */
  int ucct() const;
  /** Sets the display units for mfnacnvg. */
  void setUcct(const int ucct);
  /** Returns the CVODE contaminant solver method: 0 = SKY, 2 = SOR, 3 = LU. */
  int mfvmthd() const;
  /** Sets the CVODE contaminant solver method: 0 = SKY, 2 = SOR, 3 = LU. */
  void setMfvmthd(const int mfvmthd);
  /** Returns the CVODE contaminant solver resequence flag: 0 = don't resequence, 1 = resequence. */
  int mfvrseq() const;
  /** Sets the CVODE contaminant solver resequence flag: 0 = don't resequence, 1 = resequence. */
  void setMfvrseq(const int mfvrseq);
  /** Returns the CVODE contaminant solver maximum iterations. */
  int mfvmaxi() const;
  /** Sets the CVODE contaminant solver maximum iterations. */
  void setMfvmaxi(const int mfvmaxi);
  /** Returns the CVODE contaminant solver relative convergence factor. */
  double mfvrcnvg() const;
  /** Sets the CVODE contaminant solver relative convergence factor. */
  bool setMfvrcnvg(const double mfvrcnvg);
  /** Sets the CVODE contaminant solver relative convergence factor. */
  bool setMfvrcnvg(const std::string &mfvrcnvg);
  /** Returns the CVODE contaminant solver absolute convergence factor. */
  double mfvacnvg() const;
  /** Sets the CVODE contaminant solver absolute convergence factor. */
  bool setMfvacnvg(const double mfvacnvg);
  /** Sets the CVODE contaminant solver absolute convergence factor. */
  bool setMfvacnvg(const std::string &mfvacnvg);
  /** Returns the CVODE contaminant solver relaxation coefficient. */
  double mfvrelax() const;
  /** Sets the CVODE contaminant solver relaxation coefficient. */
  bool setMfvrelax(const double mfvrelax);
  /** Sets the CVODE contaminant solver relaxation coefficient. */
  bool setMfvrelax(const std::string &mfvrelax);
  /** Returns the display units for mfvacnvg. */
  int uccv() const;
  /** Sets the display units for mfvacnvg. */
  void setUccv(const int uccv);
  /** Returns the mass fraction integration method: 0 = trapezoid, 1 = STS, 2 = CVODE. */
  int mf_solver() const;
  /** Sets the mass fraction integration method: 0 = trapezoid, 1 = STS, 2 = CVODE. */
  void setMf_solver(const int mf_solver);
  /** Returns the 1D zone flag: 0 = don't use, 1 = use. */
  int sim_1dz() const;
  /** Sets the 1D zone flag: 0 = don't use, 1 = use. */
  void setSim_1dz(const int sim_1dz);
  /** Returns the 1D duct flag: 0 = don't use, 1 = use. */
  int sim_1dd() const;
  /** Sets the 1D duct flag: 0 = don't use, 1 = use. */
  void setSim_1dd(const int sim_1dd);
  /** Returns the default length of duct cells for C-D model [m]. */
  double celldx() const;
  /** Sets the default length of duct cells for C-D model [m]. */
  bool setCelldx(const double celldx);
  /** Sets the default length of duct cells for C-D model [m]. */
  bool setCelldx(const std::string &celldx);
  /** Returns the variable junction temperature flag: 0 = don't compute, 1 = compute. */
  int sim_vjt() const;
  /** Sets the variable junction temperature flag: 0 = don't compute, 1 = compute. */
  void setSim_vjt(const int sim_vjt);
  /** Returns the display units of celldx. */
  int udx() const;
  /** Sets the display units of celldx. */
  void setUdx(const int udx);
  /** Returns the CVODE contaminant solver multi-step method: 0 = am, 1 = bdf. */
  int cvode_mth() const;
  /** Sets the CVODE contaminant solver multi-step method: 0 = am, 1 = bdf. */
  void setCvode_mth(const int cvode_mth);
  /** Returns the CVODE contaminant solver multi-step relative convergence factor. */
  double cvode_rcnvg() const;
  /** Sets the CVODE contaminant solver multi-step relative convergence factor. */
  bool setCvode_rcnvg(const double cvode_rcnvg);
  /** Sets the CVODE contaminant solver multi-step relative convergence factor. */
  bool setCvode_rcnvg(const std::string &cvode_rcnvg);
  /** Returns the CVODE contaminant solver multi-step absolute convergence factor. */
  double cvode_acnvg() const;
  /** Sets the CVODE contaminant solver multi-step absolute convergence factor. */
  bool setCvode_acnvg(const double cvode_acnvg);
  /** Sets the CVODE contaminant solver multi-step absolute convergence factor. */
  bool setCvode_acnvg(const std::string &cvode_acnvg);
  /** Returns the CVODE contaminant solver maximum time step. */
  double cvode_dtmax() const;
  /** Sets the CVODE contaminant solver maximum time step. */
  bool setCvode_dtmax(const double cvode_dtmax);
  /** Sets the CVODE contaminant solver maximum time step. */
  bool setCvode_dtmax(const std::string &cvode_dtmax);
  /** Returns the time-varying density flag: 0 = no variation during time step, 1 = vary density during time step. */
  int tsdens() const;
  /** Sets the time-varying density flag: 0 = no variation during time step, 1 = vary density during time step. */
  void setTsdens(const int tsdens);
  /** Returns the (inactive) under-relaxation factor for calculating dM/dt. */
  double tsrelax() const;
  /** Sets the (inactive) under-relaxation factor for calculating dM/dt. */
  bool setTsrelax(const double tsrelax);
  /** Sets the (inactive) under-relaxation factor for calculating dM/dt. */
  bool setTsrelax(const std::string &tsrelax);
  /** Returns the maximum number of iterations for density changes. */
  int tsmaxi() const;
  /** Sets the maximum number of iterations for density changes. */
  void setTsmaxi(const int tsmaxi);
  /** Returns the density treatment during steady state initialization: 0 = don't converge, 1 = converge. */
  int cnvgSS() const;
  /** Sets the density treatment during steady state initialization: 0 = don't converge, 1 = converge. */
  void setCnvgSS(const int cnvgSS);
  /** Returns the density/zone pressure treatment: 0 = no requirement, 1 = require density = f(zone pressure). */
  int densZP() const;
  /** Sets the density/zone pressure treatment: 0 = no requirement, 1 = require density = f(zone pressure). */
  void setDensZP(const int densZP);
  /** Returns the hydrostatic density treatment: 0 = no hydrostatic variation, 1 = density varies hydrostatically. */
  int stackD() const;
  /** Sets the hydrostatic density treatment: 0 = no hydrostatic variation, 1 = density varies hydrostatically. */
  void setStackD(const int stackD);
  /** Returns the dM/dt treatment: 0 = don't include in airflow calculation, 1 = include in airflow calculation. */
  int dodMdt() const;
  /** Sets the dM/dt treatment: 0 = don't include in airflow calculation, 1 = include in airflow calculation. */
  void setDodMdt(const int dodMdt);
  /** Returns the day-of-year to start steady simulation [mmmdd]. */
  std::string date_st() const;
  /** Sets the day-of-year to start steady simulation [mmmdd]. */
  void setDate_st(const std::string &date_st);
  /** Returns the time-of-day to start steady simulation [hh:mm:ss]. */
  std::string time_st() const;
  /** Sets the time-of-day to start steady simulation [hh:mm:ss]. */
  void setTime_st(const std::string &time_st);
  /** Returns the day-of-year to start transient simulation [mmmdd]. */
  std::string date_0() const;
  /** Sets the day-of-year to start transient simulation [mmmdd]. */
  void setDate_0(const std::string &date_0);
  /** Returns the time-of-day to start transient simulation [hh:mm:ss]. */
  std::string time_0() const;
  /** Sets the time-of-day to start transient simulation [hh:mm:ss]. */
  void setTime_0(const std::string &time_0);
  /** Returns the day-of-year to end transient simulation [mmmdd]. */
  std::string date_1() const;
  /** Sets the day-of-year to end transient simulation [mmmdd]. */
  void setDate_1(const std::string &date_1);
  /** Returns the time-of-day to end transient simulation [hh:mm:ss]. */
  std::string time_1() const;
  /** Sets the time-of-day to end transient simulation [hh:mm:ss]. */
  void setTime_1(const std::string &time_1);
  /** Returns the simulation time step [hh:mm:ss]. */
  std::string time_step() const;
  /** Sets the simulation time step [hh:mm:ss]. */
  void setTime_step(const std::string &time_step);
  /** Returns the simulation output (results) time step [hh:mm:ss]. */
  std::string time_list() const;
  /** Sets the simulation output (results) time step [hh:mm:ss]. */
  void setTime_list(const std::string &time_list);
  /** Returns simulation status time step (up to 1 day) [hh:mm:ss]. */
  std::string time_scrn() const;
  /** Sets the simulation status time step (up to 1 day) [hh:mm:ss]. */
  void setTime_scrn(const std::string &time_scrn);
  /** Returns the restart file flag: 0 = don't use, 1 = use. */
  int restart() const;
  /** Sets the restart file flag: 0 = don't use, 1 = use. */
  void setRestart(const int restart);
  /** Returns the restart date [mmmdd]. */
  std::string rstdate() const;
  /** Sets the restart date [mmmdd]. */
  void setRstdate(const std::string &rstdate);
  /** Returns the restart time [hh:mm:ss]. */
  std::string rsttime() const;
  /** Sets the restart time [hh:mm:ss]. */
  void setRsttime(const std::string &rsttime);
  /** Returns the data dump parameter: > 0 dump matrix analysis, = 2 dump SIM file output, > 2 dump lognotes. */
  int list() const;
  /** Sets the data dump parameter: > 0 dump matrix analysis, = 2 dump SIM file output, > 2 dump lognotes. */
  void setList(const int list);
  /** Returns the ContamX dialog box flag: 0 = don't display, 1 = display. */
  int doDlg() const;
  /** Sets the ContamX dialog box flag: 0 = don't display, 1 = display. */
  void setDoDlg(const int doDlg);
  /** Returns the path flow results save flag: 0 = don't save, 1 = save to SIM file. */
  int pfsave() const;
  /** Sets the path flow results save flag: 0 = don't save, 1 = save to SIM file. */
  void setPfsave(const int pfsave);
  /** Returns zone flow results the save flag: 0 = don't save, 1 = save to SIM file. */
  int zfsave() const;
  /** Sets the zone flow results save flag: 0 = don't save, 1 = save to SIM file. */
  void setZfsave(const int zfsave);
  /** Returns the mass fraction results save flag: 0 = don't save, 1 = save to SIM file. */
  int zcsave() const;
  /** Sets the mass fraction results save flag: 0 = don't save, 1 = save to SIM file. */
  void setZcsave(const int zcsave);
  /** Returns the ACH calculation behavior: 0 = use std volumes, 1 = use true volumes. */
  int achvol() const;
  /** Sets the ACH calculation behavior: 0 = use std volumes, 1 = use true volumes. */
  void setAchvol(const int achvol);
  /** Returns building air exchange rate transient data the save flag: 0 = don't save, 1 = save. */
  int achsave() const;
  /** Sets the building air exchange rate transient data save flag: 0 = don't save, 1 = save. */
  void setAchsave(const int achsave);
  /** Returns  the save flag: 0 = don't save, 1 = save. */
  int abwsave() const;
  /** Sets the  save flag: 0 = don't save, 1 = save. */
  void setAbwsave(const int abwsave);
  /** Returns the contaminant box-whisker data save flag: 0 = don't save, 1 = save. */
  int cbwsave() const;
  /** Sets the contaminant box-whisker data save flag: 0 = don't save, 1 = save. */
  void setCbwsave(const int cbwsave);
  /** Returns the exposure transient data save flag: 0 = don't save, 1 = save. */
  int expsave() const;
  /** Sets the exposure transient data save flag: 0 = don't save, 1 = save. */
  void setExpsave(const int expsave);
  /** Returns the exposure box-whisker data save flag: 0 = don't save, 1 = save. */
  int ebwsave() const;
  /** Sets the exposure box-whisker data save flag: 0 = don't save, 1 = save. */
  void setEbwsave(const int ebwsave);
  /** Returns the zones age-of-air transient data save flag: 0 = don't save, 1 = save. */
  int zaasave() const;
  /** Sets the zones age-of-air transient data save flag: 0 = don't save, 1 = save. */
  void setZaasave(const int zaasave);
  /** Returns the zones age-of-air box-whisker data save flag: 0 = don't save, 1 = save. */
  int zbwsave() const;
  /** Sets the zones age-of-air box-whisker data save flag: 0 = don't save, 1 = save. */
  void setZbwsave(const int zbwsave);
  /** Returns the binary zone flow file flag: 0 = don't save, 1 = save. */
  int rzfsave() const;
  /** Sets the binary zone flow file flag: 0 = don't save, 1 = save. */
  void setRzfsave(const int rzfsave);
  /** Returns the binary avg zone mass fraction file flag: 0 = don't save, 1 = save. */
  int rzmsave() const;
  /** Sets the binary avg zone mass fraction file flag: 0 = don't save, 1 = save. */
  void setRzmsave(const int rzmsave);
  /** Returns the binary 1D zone cell mass fraction file flag: 0 = don't save, 1 = save. */
  int rz1save() const;
  /** Sets the binary 1D zone cell mass fraction file flag: 0 = don't save, 1 = save. */
  void setRz1save(const int rz1save);
  /** Returns the text contaminant summary file flag: 0 = don't save, 1 = save. */
  int csmsave() const;
  /** Sets the text contaminant summary file flag: 0 = don't save, 1 = save. */
  void setCsmsave(const int csmsave);
  /** Returns the text surface result file flag: 0 = don't save, 1 = save. */
  int srfsave() const;
  /** Sets the text surface result file flag: 0 = don't save, 1 = save. */
  void setSrfsave(const int srfsave);
  /** Returns the text controls log file flag: 0 = don't save, 1 = save. */
  int logsave() const;
  /** Sets the text controls log file flag: 0 = don't save, 1 = save. */
  void setLogsave(const int logsave);
  /** Returns the save vector - unused by CONTAM and subject to change without notice. */
  std::vector<int> save() const;
  /** Sets the save vector - unused by CONTAM and subject to change without notice. */
  void setSave(const std::vector<int> &save);
  /** Returns the mysterious rvals vector. */
  std::vector<double> rvals() const;
  /** Sets the mysterious rvals vector. */
  bool setRvals(const std::vector<double> &rvals);
  /** Sets the mysterious rvals vector. */
  bool setRvals(const std::vector<std::string> &rvals);
  /** Returns the building airflow test (zones) flag: 0 = don't output, 1 = output. */
  int BldgFlowZ() const;
  /** Sets the building airflow test (zones) flag: 0 = don't output, 1 = output. */
  void setBldgFlowZ(const int BldgFlowZ);
  /** Returns the building airflow test (ducts) flag: 0 = don't output, 1 = output. */
  int BldgFlowD() const;
  /** Sets the building airflow test (ducts) flag: 0 = don't output, 1 = output. */
  void setBldgFlowD(const int BldgFlowD);
  /** Returns the building airflow test (classified flows) flag: 0 = don't output, 1 = output. */
  int BldgFlowC() const;
  /** Sets the building airflow test (classified flows) flag: 0 = don't output, 1 = output. */
  void setBldgFlowC(const int BldgFlowC);
  /** Returns the cfd coupling method: 0 = no cfd, 1 = post, 2 = quasi, 3 = dynamic. */
  int cfd_ctype() const;
  /** Sets the cfd coupling method: 0 = no cfd, 1 = post, 2 = quasi, 3 = dynamic. */
  void setCfd_ctype(const int cfd_ctype);
  /** Returns the convergence factor for dynamic coupling. */
  double cfd_convcpl() const;
  /** Sets the convergence factor for dynamic coupling. */
  bool setCfd_convcpl(const double cfd_convcpl);
  /** Sets the convergence factor for dynamic coupling. */
  bool setCfd_convcpl(const std::string &cfd_convcpl);
  /** Returns the .var file flag: 0 = don't use, 1 = use. */
  int cfd_var() const;
  /** Sets the .var file flag: 0 = don't use, 1 = use. */
  void setCfd_var(const int cfd_var);
  /** Returns the currently unused zref value. */
  int cfd_zref() const;
  /** Sets the the currently unused zref value. */
  void setCfd_zref(const int cfd_zref);
  /** Returns the max number of dynamic coupling iterations. */
  int cfd_imax() const;
  /** Sets the max number of dynamic coupling iterations. */
  void setCfd_imax(const int cfd_imax);
  /** Returns the number of iterations between outputs to .cmo file. */
  int cfd_dtcmo() const;
  /** Sets the number of iterations between outputs to .cmo file. */
  void setCfd_dtcmo(const int cfd_dtcmo);

private:
  void setDefaults();

  int m_sim_af;  // airflow simulation: 0 = steady, 1 = dynamic (I2)
  int m_afcalc;  // N-R method for non-linear eqns: 0 = SUR, 1 = STR (I2)
  int m_afmaxi;  // maximum number of N-R iterations (I2)
  PRJFLOAT m_afrcnvg;  // relative airflow convergence factor (R4)
  PRJFLOAT m_afacnvg;  // absolute airflow convergence factor [1/s] (R4)
  PRJFLOAT m_afrelax;  // flow under-relaxation coefficient (for SUR) (R4)
  int m_uac2;  // units for afacnvg (I2)
  PRJFLOAT m_Pres;  // pressure test pressure (R4) {Contam 2.4}
  int m_uPres;  // units of Pres (I2) {Contam 2.4}
  int m_afslae;  // method for linear equations: 0 = SKY, 1 = PCG (I2)
  int m_afrseq;  // if true, resequence the linear equations (I2)
  int m_aflmaxi;  // maximum number of iterations (PCG) (I2)
  PRJFLOAT m_aflcnvg;  // relative convergence factor for (PCG) (R4)
  int m_aflinit;  // if true, do linear airflow initialization (I2)
  int m_Tadj;  // if true, use temperature adjustment (I2)
  int m_sim_mf;  // mass fraction (contaminant) simulation: 0 = none, 1 = steady, 2 = transient, 3 = cyclic (I2)
  int m_ccmaxi;  // simulation: maximum number of cyclic iterations (I2)
  PRJFLOAT m_ccrcnvg;  // relative convergence factor (R4)
  PRJFLOAT m_ccacnvg;  // absolute convergence factor [kg/kg] (R4)
  PRJFLOAT m_ccrelax;  // (unused) over-relaxation coefficient (R4)
  int m_uccc;  // units for ccacnvg (I2)
  int m_mfnmthd;  // simulation: 0 = SKY, 1 = BCG, 2 = SOR, 3 = LU (I2)
  int m_mfnrseq;  // if true, resequence the linear equations (I2)
  int m_mfnmaxi;  // maximum iterations (I2)
  PRJFLOAT m_mfnrcnvg;  // desired relative convergence (R4)
  PRJFLOAT m_mfnacnvg;  // desired absolute convergence (R4)
  PRJFLOAT m_mfnrelax;  // relaxation coefficient (R4)
  PRJFLOAT m_mfngamma;  // trapezoidal integration factor (R4)
  int m_uccn;  // units for mfnacnvg (I2)
  int m_mftmthd;  // 0 = SKY, 1 = BCG, 2 = SOR, 3 = LU (I2)
  int m_mftrseq;  // if true, resequence the linear equations (I2)
  int m_mftmaxi;  // maximum iterations (I2)
  PRJFLOAT m_mftrcnvg;  // desired relative convergence (R4)
  PRJFLOAT m_mftacnvg;  // desired absolute convergence (R4)
  PRJFLOAT m_mftrelax;  // relaxation coefficient (R4)
  PRJFLOAT m_mftgamma;  // trapezoidal integration factor (R4)
  int m_ucct;  // units for mftacnvg (I2)
  int m_mfvmthd;  // 0 = SKY, 2 = SOR, 3 = LU (1 n/a) (I2)
  int m_mfvrseq;  // if true, resequence the linear equations (I2)
  int m_mfvmaxi;  // maximum iterations (I2)
  PRJFLOAT m_mfvrcnvg;  // desired relative convergence (R4)
  PRJFLOAT m_mfvacnvg;  // desired absolute convergence (R4)
  PRJFLOAT m_mfvrelax;  // relaxation coefficient (R4)
  int m_uccv;  // units for mfvacnvg (I2)
  int m_mf_solver;  // mass fraction integration method: {3.1} 0=trapezoid, 1=STS, 2=CVODE (I2)
  int m_sim_1dz;  // if true, use 1D zones (I2)
  int m_sim_1dd;  // if true, use 1D ducts (I2)
  PRJFLOAT m_celldx;  // default length of duct cells for C-D model [m] (R4)
  int m_sim_vjt;  // if true, compute variable junction temperatures (I2)
  int m_udx;  // units of celldx (I2)
  int m_cvode_mth;  // 0 = am, 1 = bdf (I2)
  PRJFLOAT m_cvode_rcnvg;  // relative convergence factor(R4)
  PRJFLOAT m_cvode_acnvg;  // absolute convergence factor(R4)
  PRJFLOAT m_cvode_dtmax;  // maximum time step (R4)
  int m_tsdens;  // (0/1) vary density during time step (I2)
  PRJFLOAT m_tsrelax;  // (inactive) under-relaxation factor for calculating dM/dt (R4)
  int m_tsmaxi;  // maximum number of iterations for density changes (I2)
  int m_cnvgSS;  // (0/1) converge density in steady state init (I1) {2.4b}
  int m_densZP;  // (0/1) density = f(zone pressure) (I1) {2.4b, 3.1}
  int m_stackD;  // (0/1) density varies hydrostatically (I1) {2.4b, 3.1}
  int m_dodMdt;  // (0/1) include dM/dt in airflow calc (I1) {2.4b, 3.1}
  std::string m_date_st;  // day-of-year to start steady simulation (mmmdd)
  std::string m_time_st;  // time-of-day to start steady simulation (hh:mm:ss)
  std::string m_date_0;  // day-of-year to start transient simulation (mmmdd)
  std::string m_time_0;  // time-of-day to start transient simulation (hh:mm:ss)
  std::string m_date_1;  // day-of-year to end transient simulation (mmmdd)
  std::string m_time_1;  // time-of-day to end transient simulation (hh:mm:ss)
  std::string m_time_step;  // simulation time step [s] (hh:mm:ss)
  std::string m_time_list;  // simulation output (results) time step [s] (hh:mm:ss)
  std::string m_time_scrn;  // simulation status time step [s] (up to 1 day) (hh:mm:ss)
  int m_restart;  // use restart file (I2)
  std::string m_rstdate;  // restart date (mmmdd)
  std::string m_rsttime;  // restart time (hh:mm:ss)
  int m_list;  // data dump parameter (I2) > 0 dump matrix analysis, = 2 dump SIM file output, > 2 dump lognotes.
  int m_doDlg;  // (0/1) ContamX will display dialog box (I1)
  int m_pfsave;  // (0/1) save path flow results to SIM file (I1)
  int m_zfsave;  // (0/1) save zone flow results to SIM file (I1)
  int m_zcsave;  // (0/1) save mass fraction results to SIM file (I1)
  int m_achvol;  // (0/1) ACH based on true volumes instead of std volumes (I1)
  int m_achsave;  // (0/1) save building air exchange rate transient data (I1)
  int m_abwsave;  // (0/1) save air exchange rate box-whisker data (I1)
  int m_cbwsave;  // (0/1) save contaminant box-whisker data (I1)
  int m_expsave;  // (0/1) save exposure transient data (I1)
  int m_ebwsave;  // (0/1) save exposure box-whisker data (I1)
  int m_zaasave;  // (0/1) save zones age-of-air transient data (I1)
  int m_zbwsave;  // (0/1) save zones age-of-air box-whisker data (I1)
  int m_rzfsave;  // (0/1) save binary zone flow file (I1)
  int m_rzmsave;  // (0/1) save binary avg zone mass fraction file (I1)
  int m_rz1save;  // (0/1) save binary 1D zone cell mass fraction file (I1)
  int m_csmsave;  // (0/1) save text contaminant summary file (I1)
  int m_srfsave;  // (0/1) save text surface result file (I1)
  int m_logsave;  // (0/1) save text controls log file (I1)
  std::vector<int> m_save;  // (unused by CONTAM; subject to change without notice) (I1)
  std::vector<PRJFLOAT> m_rvals;  // real values (R4)
  int m_BldgFlowZ;  // output building airflow test (zones) (IX)
  int m_BldgFlowD;  // output building airflow test (ducts) (IX)
  int m_BldgFlowC;  // output building airflow test (classified flows) (IX)
  int m_cfd_ctype;  // (0=no cfd, 1=post, 2=quasi, 3=dynamic) cfd coupling method (I2)
  PRJFLOAT m_cfd_convcpl;  // convergence factor for dynamic coupling (R4)
  int m_cfd_var;  // (0/1) use .var file (I2)
  int m_cfd_zref;  // currently not used (I2)
  int m_cfd_imax;  // max number of dynamic coupling iterations (I2)
  int m_cfd_dtcmo;  // number of iterations between outputs to .cmo file (I2)
};

class LevelImpl
{
public:
  LevelImpl();
  LevelImpl(int nr, double refht, double delht, int u_rfht, int u_dlht, std::string name, std::vector<Icon> icons);
  LevelImpl(int nr, std::string refht, std::string delht, int u_rfht, int u_dlht, std::string name, std::vector<Icon> icons);
  void read(Reader &reader);
  std::string write();

  /** Returns the level number, in order from 1 to the number of levels. */
  int nr() const;
  /** Sets the level number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the reference elevation of level [m]. */
  double refht() const;
  /** Sets the reference elevation of level [m]. */
  bool setRefht(const double refht);
  /** Sets the reference elevation of level [m]. */
  bool setRefht(const std::string &refht);
  /** Returns the delta elevation to next level [m]. */
  double delht() const;
  /** Sets the delta elevation to next level [m]. */
  bool setDelht(const double delht);
  /** Sets the delta elevation to next level [m]. */
  bool setDelht(const std::string &delht);
  /** Returns the units of reference elevation. */
  int u_rfht() const;
  /** Returns the units of reference elevation. */
  void setU_rfht(const int u_rfht);
  /** Returns the units of delta elevation. */
  int u_dlht() const;
  /** Sets the units of delta elevation. */
  void setU_dlht(const int u_dlht);
  /** Returns the level name. */
  std::string name() const;
  /** Sets the level name. */
  void setName(const std::string &name);
  /** Returns the level's icons in a vector. */
  std::vector<Icon> icons() const;
  /** Sets the level's icon vector. */
  void setIcons(const std::vector<Icon> &icons);

private:
  void setDefaults();

  int m_nr;  // level number (IX), in order from 1 to nlev
  PRJFLOAT m_refht;  // reference elevation of level [m] (R4)
  PRJFLOAT m_delht;  // delta elevation to next level [m] (R4) {W}
  //int m_nicon;  // number of icons on this level (IX)
  int m_u_rfht;  // units of reference elevation (I2) {W}
  int m_u_dlht;  // units of delta elevation (I2) {W}
  std::string m_name;  // level name (CS)
  std::vector<Icon> m_icons;  // level icons (Icon)
};

class DayScheduleImpl
{
public:
  DayScheduleImpl();
  DayScheduleImpl(int nr,int shape,int utyp,int ucnv,std::string name,std::string desc,std::vector<SchedulePoint> points);
  void read(Reader &reader);
  std::string write();

  /** Returns the schedule number, in order from 1 to the number of schedules. */
  int nr() const;
  /** Sets the schedule number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the schedule shape: 0 = rectangular, 1 = trapezoidal. */
  int shape() const;
  /** Sets the schedule shape: 0 = rectangular, 1 = trapezoidal. */
  void setShape(const int shape);
  /** Returns the type of units. */
  int utyp() const;
  /** Sets the type of units. */
  void setUtyp(const int utyp);
  /** Returns the schedule unit conversion. */
  int ucnv() const;
  /** Sets the schedule unit conversion. */
  void setUcnv(const int ucnv);
  /** Returns the schedule name. */
  std::string name() const;
  /** Sets the schedule name. */
  void setName(const std::string &name);
  /** Returns the schedule description. */
  std::string desc() const;
  /** Sets the schedule description. */
  void setDesc(const std::string &desc);
  /** Returns the schedule points as a vector. */
  std::vector<SchedulePoint> points() const;
  /** Sets the schedule point vector. */
  void setPoints(const std::vector<SchedulePoint> &points);

private:
  void setDefaults();

  int m_nr;  // schedule number (IX); in order from 1 to _ndsch
  int m_npts;  // number of data points (I2)
  int m_shape;  // 0 = rectangular; 1 = trapezoidal (I2)
  int m_utyp;  // type of units (I2) {W}
  int m_ucnv;  // units conversion (I2) {W}
  std::string m_name;  // schedule name (CS) {W}
  std::string m_desc;  // schedule description (CS) {W} may be blank
  std::vector<SchedulePoint> m_points;  // control points (SchedulePoint)
};

class WeekScheduleImpl
{
public:
  WeekScheduleImpl();
  WeekScheduleImpl(int nr,int utyp,int ucnv,std::string name,std::string desc,std::vector<int> j);
  void read(Reader &reader);
  std::string write();

  /** Returns the schedule number, in order from 1 to the number of schedules. */
  int nr() const;
  /** Sets the schedule number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the type of units. */
  int utyp() const;
  /** Sets the type of units. */
  void setUtyp(const int utyp);
  /** Returns the schedule unit conversion. */
  int ucnv() const;
  /** Sets the schedule unit conversion. */
  void setUcnv(const int ucnv);
  /** Returns the schedule name. */
  std::string name() const;
  /** Sets the schedule name. */
  void setName(const std::string &name);
  /** Returns the schedule description. */
  std::string desc() const;
  /** Sets the schedule description. */
  void setDesc(const std::string &desc);
  /** Returns the vector of 12 day schedule indices. */
  std::vector<int> j() const;
  /** Sets the vector of day schedule indices. There should be 12 indices. */
  void setJ(const std::vector<int> &j);

private:
  void setDefaults();

  int m_nr;  // schedule number (IX); in order from 1 to _nwsch
  int m_utyp;  // type of units (I2) {W}
  int m_ucnv;  // units conversion (I2) {W}
  std::string m_name;  // schedule name (CS) {W}
  std::string m_desc;  // schedule description (CS) {W} may be blank
  std::vector<int> m_j;  // 12 day schedule indices (IX) - converted to pointers
};

class WindPressureProfileImpl
{
public:
  WindPressureProfileImpl();
  WindPressureProfileImpl(int nr,int type,std::string name,std::string desc,
    std::vector<PressureCoefficientPoint> coeffs);
  void read(Reader &reader);
  std::string write();

  /** Returns the profile number, in order from 1 to the number of profiles. */
  int nr() const;
  /** Sets the profile number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the profile type: 1 = linear, 2 = cubic spline, 3 = trigonometric. */
  int type() const;
  /** Sets the profile type: 1 = linear, 2 = cubic spline, 3 = trigonometric. */
  void setType(const int type);
  /** Returns the profile name. */
  std::string name() const;
  /** Sets the profile name. */
  void setName(const std::string &name);
  /** Returns the profile description. */
  std::string desc() const;
  /** Sets the profile description. */
  void setDesc(const std::string &desc);
  /** Returns the wind pressure coefficient data points as a vector. */
  std::vector<PressureCoefficientPoint> coeffs() const;
  /** Sets the wind pressure coefficient data point vector. */
  void setCoeffs(const std::vector<PressureCoefficientPoint> &coeffs);

private:
  void setDefaults();

  int m_nr;  // profile number (IX); in order from 1 to _nwpf
  int m_npts;  // number of data points (I2)
  int m_type;  // 1 = linear; 2 = cubic spline; 3 = trigonometric (I2)
  std::string m_name;  // schedule name (CS) {W}
  std::string m_desc;  // profile description (CS) {W} may be blank
  std::vector<PressureCoefficientPoint> m_coeffs;  // wind pressure coefficient objects (PressureCoefficientPoint)

};

class CdvDatImpl
{
public:
  CdvDatImpl();
  CdvDatImpl(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc);
  CdvDatImpl(int nr,int seqnr,unsigned int flags,int inreq,int n1,int n2,std::string name,std::string desc,
    std::string valuename);
  void read(Reader &reader);
  std::string write(std::string datatype);
  void readDetails(Reader &input);

  /** Returns the node number, in order from 1 to the number of control nodes. */
  int nr() const;
  /** Sets the element number. This should only be done with care. */
  void setNr(const int nr);
  /** Returns the computation sequence number, which is set in ContamW. */
  int seqnr() const;
  /** Sets the computation sequence number. */
  void setSeqnr(const int seqnr);
  /** Returns the flags for offset & scale, time constant, and 1D sensor. */
  unsigned int flags() const;
  /** Sets the flags for offset & scale, time constant, and 1D sensor. */
  void setFlags(const unsigned int flags);
  /** Returns the number of required inputs. */
  int inreq() const;
  /** Sets the number of required inputs. */
  void setInreq(const int inreq);
  /** Returns the SketchPad number of input node #1. */
  int n1() const;
  /** Sets the SketchPad number of input node #1. */
  void setN1(const int n1);
  /** Returns the SketchPad number of input node #2. */
  int n2() const;
  /** Sets the SketchPad number of input node #2. */
  void setN2(const int n2);
  /** Returns the element name. */
  std::string name() const;
  /** Sets the element name. */
  void setName(const std::string &name);
  /** Returns the element description. */
  std::string desc() const;
  /** Sets the element description. */
  void setDesc(const std::string &desc);
  /** Returns the name of the value read from the file. */
  std::string valuename() const;
  /** Sets the name of the value read from the file. */
  void setValuename(const std::string &valuename);

private:
  void setDefaults();

  int m_nr;  // node (SketchPad) number (IX); in order from 1 to _nctrl
  int m_seqnr;  // computation sequence number (IX); set in ContamW
  unsigned int m_flags;  // flags for offset & scale, time constant, and 1D sensor (U2)
  int m_inreq;  // number of required inputs (I2) {W}
  int m_n1;  // SketchPad number of input node #1 (IX)
  int m_n2;  // SketchPad number of input node #2 (IX)
  std::string m_name;  // element name (CS) {W}
  std::string m_desc;  // control node description (CS) {W} may be blank
  std::string m_valuename;  // name of the value read from the Discrete or Continuous Values file (CS)
};

} // detail
} // contam
} // openstudio

#endif
