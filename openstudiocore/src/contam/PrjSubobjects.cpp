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
#include "PrjSubobjects.hpp"

namespace openstudio {
namespace contam {

WeatherData::WeatherData(RX Tambt,RX barpres,RX windspd,RX winddir,RX relhum,int daytyp,int uTa,int ubP,int uws,int uwd)
{
  d = new WeatherDataPrivate(Tambt,barpres,windspd,winddir,relhum,daytyp,uTa,ubP,uws,uwd);
}

std::string WeatherData::write()
{
  return d->write();
}

void WeatherData::read(Reader &input)
{
  d->read(input);
}

RX WeatherData::Tambt() const
{
  return d->Tambt;
}

void WeatherData::setTambt(const RX Tambt)
{
  d->Tambt = Tambt;
}

RX WeatherData::barpres() const
{
  return d->barpres;
}

void WeatherData::setBarpres(const RX barpres)
{
  d->barpres = barpres;
}

RX WeatherData::windspd() const
{
  return d->windspd;
}

void WeatherData::setWindspd(const RX windspd)
{
  d->windspd = windspd;
}

RX WeatherData::winddir() const
{
  return d->winddir;
}

void WeatherData::setWinddir(const RX winddir)
{
  d->winddir = winddir;
}

RX WeatherData::relhum() const
{
  return d->relhum;
}

void WeatherData::setRelhum(const RX relhum)
{
  d->relhum = relhum;
}

int WeatherData::daytyp() const
{
  return d->daytyp;
}

void WeatherData::setDaytyp(const int daytyp)
{
  d->daytyp = daytyp;
}

int WeatherData::uTa() const
{
  return d->uTa;
}

void WeatherData::setUTa(const int uTa)
{
  d->uTa = uTa;
}

int WeatherData::ubP() const
{
  return d->ubP;
}

void WeatherData::setUbP(const int ubP)
{
  d->ubP = ubP;
}

int WeatherData::uws() const
{
  return d->uws;
}

void WeatherData::setUws(const int uws)
{
  d->uws = uws;
}

int WeatherData::uwd() const
{
  return d->uwd;
}

void WeatherData::setUwd(const int uwd)
{
  d->uwd = uwd;
}

Icon::Icon(int icon,int col,int row,int nr)
{
  d = new IconPrivate(icon,col,row,nr);
}

std::string Icon::write()
{
  return d->write();
}

void Icon::read(Reader &input)
{
  d->read(input);
}

int Icon::icon() const
{
  return d->icon;
}

void Icon::setIcon(const int icon)
{
  d->icon = icon;
}

int Icon::col() const
{
  return d->col;
}

void Icon::setCol(const int col)
{
  d->col = col;
}

int Icon::row() const
{
  return d->row;
}

void Icon::setRow(const int row)
{
  d->row = row;
}

int Icon::nr() const
{
  return d->nr;
}

void Icon::setNr(const int nr)
{
  d->nr = nr;
}



bool Icon::isWall()
{
  return (d->icon >= WL_EW) && (d->icon <= WL_NESW);
}

uint Icon::bits()
{
  if(isWall())
    switch(d->icon)
  {
    case WL_EW:
      return BIT_EW;
    case WL_NS:
      return BIT_NS;
    case WL_ES:
      return BIT_ES;
    case WL_SW:
      return BIT_SW;
    case WL_NW:
      return BIT_NW;
    case WL_NE:
      return BIT_NE;
    case WL_NES:
      return BIT_NES;
    case WL_ESW:
      return BIT_ESW;
    case WL_NSW:
      return BIT_NSW;
    case WL_NEW:
      return BIT_NEW;
    case WL_NESW:
      return BIT_NESW;
  }
  return 0;
}

FanDataPoint::FanDataPoint(RX mF,int u_mF,RX dP,int u_dP,RX rP,int u_rP):m_mF(mF),m_u_mF(u_mF),m_dP(dP),m_u_dP(u_dP),m_rP(rP),m_u_rP(u_rP)
{}

void FanDataPoint::read(Reader &input)
{
  m_mF = input.readNumber<RX>(FILELINE);
  m_u_mF = input.read<int>(FILELINE);
  m_dP = input.readNumber<RX>(FILELINE);
  m_u_dP = input.read<int>(FILELINE);
  m_rP = input.readNumber<RX>(FILELINE);
  m_u_rP = input.read<int>(FILELINE);
}

std::string FanDataPoint::write()
{
  std::string string;
  string += openstudio::toString(m_mF) + ' ' + openstudio::toString(m_u_mF) + ' ' + openstudio::toString(m_dP) + ' ' + openstudio::toString(m_u_dP) + ' ' + openstudio::toString(m_rP) + ' ' + openstudio::toString(m_u_rP) + '\n';
  return string;
}

RX FanDataPoint::mF() const
{
  return m_mF;
}

void FanDataPoint::setMF(const RX mF)
{
  m_mF = mF;
}

int FanDataPoint::u_mF() const
{
  return m_u_mF;
}

void FanDataPoint::setU_mF(const int u_mF)
{
  m_u_mF = u_mF;
}

RX FanDataPoint::dP() const
{
  return m_dP;
}

void FanDataPoint::setDP(const RX dP)
{
  m_dP = dP;
}

int FanDataPoint::u_dP() const
{
  return m_u_dP;
}

void FanDataPoint::setU_dP(const int u_dP)
{
  m_u_dP = u_dP;
}

RX FanDataPoint::rP() const
{
  return m_rP;
}

void FanDataPoint::setRP(const RX rP)
{
  m_rP = rP;
}

int FanDataPoint::u_rP() const
{
  return m_u_rP;
}

void FanDataPoint::setU_rP(const int u_rP)
{
  m_u_rP = u_rP;
}

DataPoint::DataPoint(RX x,RX y):m_x(x),m_y(y)
{}

void DataPoint::read(Reader &input)
{
  m_x = input.readNumber<RX>(FILELINE);
  m_y = input.readNumber<RX>(FILELINE);
}

std::string DataPoint::write()
{
  std::string string;
  string += openstudio::toString(m_x) + ' ' + openstudio::toString(m_y) + '\n';
  return string;
}

RX DataPoint::x() const
{
  return m_x;
}

void DataPoint::setX(const RX x)
{
  m_x = x;
}

RX DataPoint::y() const
{
  return m_y;
}

void DataPoint::setY(const RX y)
{
  m_y = y;
}

AirflowSubelementData::AirflowSubelementData(int nr,RX relHt,int filt):m_nr(nr),m_relHt(relHt),m_filt(filt)
{}

void AirflowSubelementData::read(Reader &input)
{
  m_nr = input.read<int>(FILELINE);
  m_relHt = input.readNumber<RX>(FILELINE);
  m_filt = input.read<int>(FILELINE);
}

std::string AirflowSubelementData::write()
{
  std::string string;
  string += openstudio::toString(m_nr) + ' ' + openstudio::toString(m_relHt) + ' ' + openstudio::toString(m_filt) + '\n';
  return string;
}

int AirflowSubelementData::nr() const
{
  return m_nr;
}

void AirflowSubelementData::setNr(const int nr)
{
  m_nr = nr;
}

RX AirflowSubelementData::relHt() const
{
  return m_relHt;
}

void AirflowSubelementData::setRelHt(const RX relHt)
{
  m_relHt = relHt;
}

int AirflowSubelementData::filt() const
{
  return m_filt;
}

void AirflowSubelementData::setFilt(const int filt)
{
  m_filt = filt;
}

PressureCoefficientPoint::PressureCoefficientPoint(RX azm,RX coef):m_azm(azm),m_coef(coef)
{}

void PressureCoefficientPoint::read(Reader &input)
{
  m_azm = input.readNumber<RX>(FILELINE);
  m_coef = input.readNumber<RX>(FILELINE);
}

std::string PressureCoefficientPoint::write()
{
  std::string string;
  string += openstudio::toString(m_azm) + ' ' + openstudio::toString(m_coef) + '\n';
  return string;
}

RX PressureCoefficientPoint::azm() const
{
  return m_azm;
}

void PressureCoefficientPoint::setAzm(const RX azm)
{
  m_azm = azm;
}

RX PressureCoefficientPoint::coef() const
{
  return m_coef;
}

void PressureCoefficientPoint::setCoef(const RX coef)
{
  m_coef = coef;
}

SchedulePoint::SchedulePoint(std::string time,RX ctrl):m_time(time),m_ctrl(ctrl)
{}

void SchedulePoint::read(Reader &input)
{
  m_time = input.readString(FILELINE);
  m_ctrl = input.readNumber<RX>(FILELINE);
}

std::string SchedulePoint::write()
{
  std::string string;
  string += m_time + ' ' + openstudio::toString(m_ctrl) + '\n';
  return string;
}

std::string SchedulePoint::time() const
{
  return m_time;
}

void SchedulePoint::setTime(const std::string time)
{
  m_time = time;
}

RX SchedulePoint::ctrl() const
{
  return m_ctrl;
}

void SchedulePoint::setCtrl(const RX ctrl)
{
  m_ctrl = ctrl;
}

} // contam
} // openstudio
