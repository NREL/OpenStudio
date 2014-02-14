/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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
#include "PrjSubobjectsImpl.hpp"

namespace openstudio {
namespace contam {

WeatherData::WeatherData()
{
  d = new WeatherDataImpl;
}

WeatherData::WeatherData(std::string Tambt,std::string barpres,std::string windspd,std::string winddir,std::string relhum,int daytyp,
                         int uTa,int ubP,int uws,int uwd)
{
  d = new WeatherDataImpl(Tambt,barpres,windspd,winddir,relhum,daytyp,uTa,ubP,uws,uwd);
}

WeatherData::WeatherData(double Tambt,double barpres,double windspd,double winddir,double relhum,int daytyp,
                         int uTa,int ubP,int uws,int uwd)
{
  d = new WeatherDataImpl(Tambt,barpres,windspd,winddir,relhum,daytyp,uTa,ubP,uws,uwd);
}

WeatherData::WeatherData(const WeatherData &other) : d(other.d)
{}

WeatherData::~WeatherData()
{}

WeatherData& WeatherData::operator=(const WeatherData &other)
{
  d = other.d;
  return *this;
}

bool WeatherData::operator==(const WeatherData &other) const
{
  return d==other.d;
}

bool WeatherData::operator!=(const WeatherData &other) const
{
  return d!=other.d;
}

std::string WeatherData::write()
{
  return d->write();
}

void WeatherData::read(Reader &input)
{
  d->read(input);
}

double WeatherData::Tambt() const
{
  return d->Tambt();
}

bool WeatherData::setTambt(const double Tambt)
{
  return d->setTambt(Tambt);
}

bool WeatherData::setTambt(const std::string &Tambt)
{
  return d->setTambt(Tambt);
}

double WeatherData::barpres() const
{
  return d->barpres();
}

bool WeatherData::setBarpres(const double barpres)
{
  return d->setBarpres(barpres);
}

bool WeatherData::setBarpres(const std::string &barpres)
{
  return d->setBarpres(barpres);
}

double WeatherData::windspd() const
{
  return d->windspd();
}

bool WeatherData::setWindspd(const double windspd)
{
  return d->setWindspd(windspd);
}

bool WeatherData::setWindspd(const std::string &windspd)
{
  return d->setWindspd(windspd);
}

double WeatherData::winddir() const
{
  return d->winddir();
}

bool WeatherData::setWinddir(const double winddir)
{
  return d->setWinddir(winddir);
}

bool WeatherData::setWinddir(const std::string &winddir)
{
  return d->setWinddir(winddir);
}

double WeatherData::relhum() const
{
  return d->relhum();
}

bool WeatherData::setRelhum(const double relhum)
{
  return d->setRelhum(relhum);
}

bool WeatherData::setRelhum(const std::string &relhum)
{
  return d->setRelhum(relhum);
}

int WeatherData::daytyp() const
{
  return d->daytyp();
}

void WeatherData::setDaytyp(const int daytyp)
{
  d->setDaytyp(daytyp);
}

int WeatherData::uTa() const
{
  return d->uTa();
}

void WeatherData::setUTa(const int uTa)
{
  d->setUTa(uTa);
}

int WeatherData::ubP() const
{
  return d->ubP();
}

void WeatherData::setUbP(const int ubP)
{
  d->setUbP(ubP);
}

int WeatherData::uws() const
{
  return d->uws();
}

void WeatherData::setUws(const int uws)
{
  d->setUws(uws);
}

int WeatherData::uwd() const
{
  return d->uwd();
}

void WeatherData::setUwd(const int uwd)
{
  d->setUwd(uwd);
}

Icon::Icon()
{
  d = new IconImpl();
}

Icon::Icon(int icon,int col,int row,int nr)
{
  d = new IconImpl(icon,col,row,nr);
}

Icon::Icon(const Icon &other) : d(other.d)
{}

Icon::~Icon()
{}

Icon& Icon::operator=(const Icon &other)
{
  d = other.d;
  return *this;
}

bool Icon::operator==(const Icon &other) const
{
  return d==other.d;
}

bool Icon::operator!=(const Icon &other) const
{
  return d!=other.d;
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
  return d->icon();
}

void Icon::setIcon(const int icon)
{
  d->setIcon(icon);
}

int Icon::col() const
{
  return d->col();
}

void Icon::setCol(const int col)
{
  d->setCol(col);
}

int Icon::row() const
{
  return d->row();
}

void Icon::setRow(const int row)
{
  d->setRow(row);
}

int Icon::nr() const
{
  return d->nr();
}

void Icon::setNr(const int nr)
{
  d->setNr(nr);
}

bool Icon::isWall()
{
  return d->isWall();
}

uint Icon::bits()
{
  return d->bits();
}

void FanDataPoint::setDefaults()
{
  m_mF = RX7("0.0");
  m_u_mF = 0;
  m_dP = RX7("0.0");
  m_u_dP = 0;
  m_rP = RX7("0.0");
  m_u_rP = 0;
}

FanDataPoint::FanDataPoint()
{
  setDefaults();
}

FanDataPoint::FanDataPoint(double mF,int u_mF,double dP,int u_dP,double rP,int u_rP)
{
  setDefaults();
  setMF(mF);
  setU_mF(u_mF);
  setDP(dP);
  setU_dP(u_dP);
  setRP(rP);
  setU_rP(u_rP);
}

FanDataPoint::FanDataPoint(std::string mF,int u_mF,std::string dP,int u_dP,std::string rP,int u_rP)
{
  setDefaults();
  setMF(mF);
  setU_mF(u_mF);
  setDP(dP);
  setU_dP(u_dP);
  setRP(rP);
  setU_rP(u_rP);
}

void FanDataPoint::read(Reader &input)
{
  setMF(input.readNumber<std::string>(FILELINE));
  setU_mF(input.read<int>(FILELINE));
  setDP(input.readNumber<std::string>(FILELINE));
  setU_dP(input.read<int>(FILELINE));
  setRP(input.readNumber<std::string>(FILELINE));
  setU_rP(input.read<int>(FILELINE));
}

std::string FanDataPoint::write()
{
  std::string string;
  string += ANY_TO_STR(m_mF) + ' ' + ANY_TO_STR(m_u_mF) + ' ' + ANY_TO_STR(m_dP) + ' ' + ANY_TO_STR(m_u_dP) + ' ' + ANY_TO_STR(m_rP) + ' ' + ANY_TO_STR(m_u_rP) + '\n';
  return string;
}

double FanDataPoint::mF() const
{
  return m_mF.toDouble();
}

bool FanDataPoint::setMF(const double mF)
{
  m_mF = QString::number(mF);
  return true;
}

bool FanDataPoint::setMF(const std::string &mF)
{
  bool ok;
  STR_TO_RX7(mF).toDouble(&ok);
  if(ok)
  {
    m_mF = STR_TO_RX7(mF);
    return true;
  }
  return false;
}

int FanDataPoint::u_mF() const
{
  return m_u_mF;
}

void FanDataPoint::setU_mF(const int u_mF)
{
  m_u_mF = u_mF;
}

double FanDataPoint::dP() const
{
  return m_dP.toDouble();
}

bool FanDataPoint::setDP(const double dP)
{
  m_dP = QString::number(dP);
  return true;
}

bool FanDataPoint::setDP(const std::string &dP)
{
  bool ok;
  STR_TO_RX7(dP).toDouble(&ok);
  if(ok)
  {
    m_dP = STR_TO_RX7(dP);
    return true;
  }
  return false;
}

int FanDataPoint::u_dP() const
{
  return m_u_dP;
}

void FanDataPoint::setU_dP(const int u_dP)
{
  m_u_dP = u_dP;
}

double FanDataPoint::rP() const
{
  return m_rP.toDouble();
}

bool FanDataPoint::setRP(const double rP)
{
  m_rP = QString::number(rP);
  return true;
}

bool FanDataPoint::setRP(const std::string &rP)
{
  bool ok;
  STR_TO_RX7(rP).toDouble(&ok);
  if(ok)
  {
    m_rP = STR_TO_RX7(rP);
    return true;
  }
  return false;
}

int FanDataPoint::u_rP() const
{
  return m_u_rP;
}

void FanDataPoint::setU_rP(const int u_rP)
{
  m_u_rP = u_rP;
}

void DataPoint::setDefaults()
{
  m_x = RX7("0.0");
  m_y = RX7("0.0");
}

DataPoint::DataPoint()
{
  setDefaults();
}

DataPoint::DataPoint(double x,double y)
{
  setDefaults();
  setX(x);
  setY(y);
}

DataPoint::DataPoint(std::string x,std::string y)
{
  setDefaults();
  setX(x);
  setY(y);
}

void DataPoint::read(Reader &input)
{
  setX(input.readNumber<std::string>(FILELINE));
  setY(input.readNumber<std::string>(FILELINE));
}

std::string DataPoint::write()
{
  std::string string;
  string += ANY_TO_STR(m_x) + ' ' + ANY_TO_STR(m_y) + '\n';
  return string;
}

double DataPoint::x() const
{
  return m_x.toDouble();
}

bool DataPoint::setX(const double x)
{
  m_x = QString::number(x);
  return true;
}

bool DataPoint::setX(const std::string &x)
{
  bool ok;
  STR_TO_RX7(x).toDouble(&ok);
  if(ok)
  {
    m_x = STR_TO_RX7(x);
    return true;
  }
  return false;
}

double DataPoint::y() const
{
  return m_y.toDouble();
}

bool DataPoint::setY(const double y)
{
  m_y = QString::number(y);
  return true;
}

bool DataPoint::setY(const std::string &y)
{
  bool ok;
  STR_TO_RX7(y).toDouble(&ok);
  if(ok)
  {
    m_y = STR_TO_RX7(y);
    return true;
  }
  return false;
}

void AirflowSubelementData::setDefaults()
{
  m_nr = 0;
  m_relHt = RX7("0.0");
  m_filt = 0;
}

AirflowSubelementData::AirflowSubelementData()
{
  setDefaults();
}

AirflowSubelementData::AirflowSubelementData(int nr,double relHt,int filt)
{
  setDefaults();
  setNr(nr);
  setRelHt(relHt);
  setFilt(filt);
}

AirflowSubelementData::AirflowSubelementData(int nr,std::string relHt,int filt)
{
  setDefaults();
  setNr(nr);
  setRelHt(relHt);
  setFilt(filt);
}

void AirflowSubelementData::read(Reader &input)
{
  setNr(input.read<int>(FILELINE));
  setRelHt(input.readNumber<std::string>(FILELINE));
  setFilt(input.read<int>(FILELINE));
}

std::string AirflowSubelementData::write()
{
  std::string string;
  string += ANY_TO_STR(m_nr) + ' ' + ANY_TO_STR(m_relHt) + ' ' + ANY_TO_STR(m_filt) + '\n';
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

double AirflowSubelementData::relHt() const
{
  return m_relHt.toDouble();
}

bool AirflowSubelementData::setRelHt(const double relHt)
{
  m_relHt = QString::number(relHt);
  return true;
}

bool AirflowSubelementData::setRelHt(const std::string &relHt)
{
  bool ok;
  STR_TO_RX7(relHt).toDouble(&ok);
  if(ok)
  {
    m_relHt = STR_TO_RX7(relHt);
    return true;
  }
  return false;
}

int AirflowSubelementData::filt() const
{
  return m_filt;
}

void AirflowSubelementData::setFilt(const int filt)
{
  m_filt = filt;
}

void PressureCoefficientPoint::setDefaults()
{
  m_azm = RX7("0.0");
  m_coef = RX7("0.0");
}

PressureCoefficientPoint::PressureCoefficientPoint()
{
  setDefaults();
}

PressureCoefficientPoint::PressureCoefficientPoint(double azm,double coef)
{
  setDefaults();
  setAzm(azm);
  setCoef(coef);
}

PressureCoefficientPoint::PressureCoefficientPoint(std::string azm,std::string coef)
{
  setDefaults();
  setAzm(azm);
  setCoef(coef);
}

void PressureCoefficientPoint::read(Reader &input)
{
  setAzm(input.readNumber<std::string>(FILELINE));
  setCoef(input.readNumber<std::string>(FILELINE));
}

std::string PressureCoefficientPoint::write()
{
  std::string string;
  string += ANY_TO_STR(m_azm) + ' ' + ANY_TO_STR(m_coef) + '\n';
  return string;
}

double PressureCoefficientPoint::azm() const
{
  return m_azm.toDouble();
}

bool PressureCoefficientPoint::setAzm(const double azm)
{
  m_azm = QString::number(azm);
  return true;
}

bool PressureCoefficientPoint::setAzm(const std::string &azm)
{
  bool ok;
  STR_TO_RX7(azm).toDouble(&ok);
  if(ok)
  {
    m_azm = STR_TO_RX7(azm);
    return true;
  }
  return false;
}

double PressureCoefficientPoint::coef() const
{
  return m_coef.toDouble();
}

bool PressureCoefficientPoint::setCoef(const double coef)
{
  m_coef = QString::number(coef);
  return true;
}

bool PressureCoefficientPoint::setCoef(const std::string &coef)
{
  bool ok;
  STR_TO_RX7(coef).toDouble(&ok);
  if(ok)
  {
    m_coef = STR_TO_RX7(coef);
    return true;
  }
  return false;
}

void SchedulePoint::setDefaults()
{
  m_time = std::string("00:00:00");
  m_ctrl = RX7("0.0");
}

SchedulePoint::SchedulePoint()
{
  setDefaults();
}

SchedulePoint::SchedulePoint(std::string time,double ctrl)
{
  setDefaults();
  setTime(time);
  setCtrl(ctrl);
}

SchedulePoint::SchedulePoint(std::string time,std::string ctrl)
{
  setDefaults();
  setTime(time);
  setCtrl(ctrl);
}

void SchedulePoint::read(Reader &input)
{
  setTime(input.readString(FILELINE));
  setCtrl(input.readNumber<std::string>(FILELINE));
}

std::string SchedulePoint::write()
{
  std::string string;
  string += m_time + ' ' + ANY_TO_STR(m_ctrl) + '\n';
  return string;
}

std::string SchedulePoint::time() const
{
  return m_time;
}

void SchedulePoint::setTime(const std::string &time)
{
  m_time = time;
}

double SchedulePoint::ctrl() const
{
  return m_ctrl.toDouble();
}

bool SchedulePoint::setCtrl(const double ctrl)
{
  m_ctrl = QString::number(ctrl);
  return true;
}

bool SchedulePoint::setCtrl(const std::string &ctrl)
{
  bool ok;
  STR_TO_RX7(ctrl).toDouble(&ok);
  if(ok)
  {
    m_ctrl = STR_TO_RX7(ctrl);
    return true;
  }
  return false;
}

} // contam
} // openstudio
