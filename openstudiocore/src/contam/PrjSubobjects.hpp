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
#ifndef PRJSUBOBJECTS_H
#define PRJSUBOBJECTS_H
#include "PrjDefines.hpp"
#include "PrjReader.hpp"
#include "PrjSubobjectsPrivate.hpp"
#include "ContamAPI.hpp"

namespace openstudio {
namespace contam {

class CONTAM_API WeatherData
{
public:
  WeatherData(RX Tambt=RX_INIT(0),RX barpres=RX_INIT(0),RX windspd=RX_INIT(0),RX winddir=RX_INIT(0),
    RX relhum=RX_INIT(0),int daytyp=0,int uTa=0,int ubP=0,int uws=0,int uwd=0);
  void read(Reader &input);
  std::string write();
  RX Tambt() const;
  void setTambt(const RX Tambt);
  RX barpres() const;
  void setBarpres(const RX barpres);
  RX windspd() const;
  void setWindspd(const RX windspd);
  RX winddir() const;
  void setWinddir(const RX winddir);
  RX relhum() const;
  void setRelhum(const RX relhum);
  int daytyp() const;
  void setDaytyp(const int daytyp);
  int uTa() const;
  void setUTa(const int uTa);
  int ubP() const;
  void setUbP(const int ubP);
  int uws() const;
  void setUws(const int uws);
  int uwd() const;
  void setUwd(const int uwd);
private:
  QExplicitlySharedDataPointer<WeatherDataPrivate> d;
};

class CONTAM_API Icon
{
public:
  Icon(int icon=0,int col=0,int row=0,int nr=0);
  void read(Reader &input);
  std::string write();
  bool isWall();
  unsigned bits();
  int icon() const;
  void setIcon(const int icon);
  int col() const;
  void setCol(const int col);
  int row() const;
  void setRow(const int row);
  int nr() const;
  void setNr(const int nr);


private:
  QExplicitlySharedDataPointer<IconPrivate> d;
};

class CONTAM_API FanDataPoint
{
public:
  FanDataPoint(RX mF=RX_INIT(0),int u_mF=0,RX dP=RX_INIT(0),int u_dP=0,RX rP=RX_INIT(0),int u_rP=0);
  void read(Reader &reader);
  std::string write();
  RX mF() const;
  void setMF(const RX mF);
  int u_mF() const;
  void setU_mF(const int u_mF);
  RX dP() const;
  void setDP(const RX dP);
  int u_dP() const;
  void setU_dP(const int u_dP);
  RX rP() const;
  void setRP(const RX rP);
  int u_rP() const;
  void setU_rP(const int u_rP);
private:
  RX m_mF;  // measured flow rates [kg/s] (R4) {W}
  int m_u_mF;  // units of measured flows (I2) {W}
  RX m_dP;  // measured pressure rises [Pa] (R4) {W}
  int m_u_dP;  // units of pressure rises (I2) {W}
  RX m_rP;  // revised pressure rises [Pa] (R4) {W}
  int m_u_rP;  // units of revised pressures (I2) {W}
};

class CONTAM_API DataPoint
{
public:
  DataPoint(RX x=RX_INIT(0),RX y=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  RX x() const;
  void setX(const RX x);
  RX y() const;
  void setY(const RX y);
private:
  RX m_x;  // value of independent variable (R4)
  RX m_y;  // value of dependent variable (R4)
};

class CONTAM_API AirflowSubelementData
{
public:
  AirflowSubelementData(int nr=0,RX relHt=RX_INIT(0),int filt=0);
  void read(Reader &reader);
  std::string write();
  int nr() const;
  void setNr(const int nr);
  RX relHt() const;
  void setRelHt(const RX relHt);
  int filt() const;
  void setFilt(const int filt);
private:
  int m_nr;  // sub-element number (IX)
  RX m_relHt;  // relative height of sub-element (R4)
  int m_filt;  // 1= filtered, 0= not (I2)
};

class CONTAM_API PressureCoefficientPoint
{
public:
  PressureCoefficientPoint(RX azm=RX_INIT(0),RX coef=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  RX azm() const;
  void setAzm(const RX azm);
  RX coef() const;
  void setCoef(const RX coef);
private:
  RX m_azm;  // wind azimuth value {R4} [degrees]
  RX m_coef;  // normalized wind pressure coefficients {R4} [-]
};

class CONTAM_API SchedulePoint
{
public:
  SchedulePoint(std::string time=std::string(),RX ctrl=RX_INIT(0));
  void read(Reader &reader);
  std::string write();
  std::string time() const;
  void setTime(const std::string time);
  RX ctrl() const;
  void setCtrl(const RX ctrl);
private:
  std::string m_time;  // time-of-day [s] (hh:mm:ss)
  RX m_ctrl;  // corresponding control value (R4) [-]
};

} // contam
} // openstudio

#endif
