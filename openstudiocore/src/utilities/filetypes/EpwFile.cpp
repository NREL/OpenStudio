/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <utilities/filetypes/EpwFile.hpp>
#include <utilities/idf/IdfObject.hpp>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/core/Checksum.hpp>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/filesystem/fstream.hpp>

#include <QStringList>
#include <QFile>
#include <QTextStream>

#include <cmath>

namespace openstudio{

EpwDataPoint::EpwDataPoint(int year,int month,int day,int hour,int minute,
  std::string dataSourceandUncertaintyFlags,double dryBulbTemperature,
  double dewPointTemperature,double relativeHumidity,
  double atmosphericStationPressure,
  double extraterrestrialHorizontalRadiation,
  double extraterrestrialDirectNormalRadiation,
  double horizontalInfraredRadiationIntensity,
  double globalHorizontalRadiation,double directNormalRadiation,
  double diffuseHorizontalRadiation,double globalHorizontalIlluminance,
  double directNormalIlluminance,double diffuseHorizontalIlluminance,
  double zenithLuminance,double windDirection,double windSpeed,
  int totalSkyCover,int opaqueSkyCover,double visibility,
  double ceilingHeight,int presentWeatherObservation,
  int presentWeatherCodes,double precipitableWater,
  double aerosolOpticalDepth,double snowDepth,double daysSinceLastSnowfall,
  double albedo,double liquidPrecipitationDepth,
  double liquidPrecipitationQuantity)
{
  setYear(year);
  setMonth(month);
  setDay(day);
  setHour(hour);
  setMinute(minute);
  setDataSourceandUncertaintyFlags(dataSourceandUncertaintyFlags);
  setDryBulbTemperature(dryBulbTemperature);
  setDewPointTemperature(dewPointTemperature);
  setRelativeHumidity(relativeHumidity);
  setAtmosphericStationPressure(atmosphericStationPressure);
  setExtraterrestrialHorizontalRadiation(extraterrestrialHorizontalRadiation);
  setExtraterrestrialDirectNormalRadiation(extraterrestrialDirectNormalRadiation);
  setHorizontalInfraredRadiationIntensity(horizontalInfraredRadiationIntensity);
  setGlobalHorizontalRadiation(globalHorizontalRadiation);
  setDirectNormalRadiation(directNormalRadiation);
  setDiffuseHorizontalRadiation(diffuseHorizontalRadiation);
  setGlobalHorizontalIlluminance(globalHorizontalIlluminance);
  setDirectNormalIlluminance(directNormalIlluminance);
  setDiffuseHorizontalIlluminance(diffuseHorizontalIlluminance);
  setZenithLuminance(zenithLuminance);
  setWindDirection(windDirection);
  setWindSpeed(windSpeed);
  setTotalSkyCover(totalSkyCover);
  setOpaqueSkyCover(opaqueSkyCover);
  setVisibility(visibility);
  setCeilingHeight(ceilingHeight);
  setPresentWeatherObservation(presentWeatherObservation);
  setPresentWeatherCodes(presentWeatherCodes);
  setPrecipitableWater(precipitableWater);
  setAerosolOpticalDepth(aerosolOpticalDepth);
  setSnowDepth(snowDepth);
  setDaysSinceLastSnowfall(daysSinceLastSnowfall);
  setAlbedo(albedo);
  setLiquidPrecipitationDepth(liquidPrecipitationDepth);
  setLiquidPrecipitationQuantity(liquidPrecipitationQuantity);
}

EpwDataPoint::EpwDataPoint(std::string line)
{
  QStringList list = QString().fromStdString(line).split(',');
  // Require 35 items in the list
  if(list.size() != 35)
  {
    std::cout << "Bad input line" << std::endl;
    return;
  }
  // Use the appropriate setter on each field
  setYear(list[0].toStdString());
  setMonth(list[1].toStdString());
  setDay(list[2].toStdString());
  setHour(list[3].toStdString());
  setMinute(list[4].toStdString());
  setDataSourceandUncertaintyFlags(list[5].toStdString());
  setDryBulbTemperature(list[6].toStdString());
  setDewPointTemperature(list[7].toStdString());
  setRelativeHumidity(list[8].toStdString());
  setAtmosphericStationPressure(list[9].toStdString());
  setExtraterrestrialHorizontalRadiation(list[10].toStdString());
  setExtraterrestrialDirectNormalRadiation(list[11].toStdString());
  setHorizontalInfraredRadiationIntensity(list[12].toStdString());
  setGlobalHorizontalRadiation(list[13].toStdString());
  setDirectNormalRadiation(list[14].toStdString());
  setDiffuseHorizontalRadiation(list[15].toStdString());
  setGlobalHorizontalIlluminance(list[16].toStdString());
  setDirectNormalIlluminance(list[17].toStdString());
  setDiffuseHorizontalIlluminance(list[18].toStdString());
  setZenithLuminance(list[19].toStdString());
  setWindDirection(list[20].toStdString());
  setWindSpeed(list[21].toStdString());
  setTotalSkyCover(list[22].toStdString());
  setOpaqueSkyCover(list[23].toStdString());
  setVisibility(list[24].toStdString());
  setCeilingHeight(list[25].toStdString());
  setPresentWeatherObservation(list[26].toStdString());
  setPresentWeatherCodes(list[27].toStdString());
  setPrecipitableWater(list[28].toStdString());
  setAerosolOpticalDepth(list[29].toStdString());
  setSnowDepth(list[30].toStdString());
  setDaysSinceLastSnowfall(list[31].toStdString());
  setAlbedo(list[32].toStdString());
  setLiquidPrecipitationDepth(list[33].toStdString());
  setLiquidPrecipitationQuantity(list[34].toStdString());
}

std::string EpwDataPoint::unitsByName(std::string name)
{
  EpwDataField id;
  try
  {
    id = EpwDataField(name);
  }
  catch(...)
  {
    // Could do a warning message here
    return std::string();
  }
  return units(id);
}

std::string EpwDataPoint::units(EpwDataField field)
{
  std::string string;
  switch(field.value())
  {
  case EpwDataField::DryBulbTemperature:
    string = "C";
    break;
  case EpwDataField::DewPointTemperature:
    string = "C";
    break;
  case EpwDataField::RelativeHumidity:
    //string = "None";
    break;
  case EpwDataField::AtmosphericStationPressure:
    string = "Pa";
    break;
  case EpwDataField::ExtraterrestrialHorizontalRadiation:
    string = "Wh/m2";
    break;
  case EpwDataField::ExtraterrestrialDirectNormalRadiation:
    string = "Wh/m2";
    break;
  case EpwDataField::HorizontalInfraredRadiationIntensity:
    string = "Wh/m2";
    break;
  case EpwDataField::GlobalHorizontalRadiation:
    string = "Wh/m2";
    break;
  case EpwDataField::DirectNormalRadiation:
    string = "Wh/m2";
    break;
  case EpwDataField::DiffuseHorizontalRadiation:
    string = "Wh/m2";
    break;
  case EpwDataField::GlobalHorizontalIlluminance:
    string = "lux";
    break;
  case EpwDataField::DirectNormalIlluminance:
    string = "lux";
    break;
  case EpwDataField::DiffuseHorizontalIlluminance:
    string = "lux";
    break;
  case EpwDataField::ZenithLuminance:
    string = "Cd/m2";
    break;
  case EpwDataField::WindDirection:
    string = "degrees";
    break;
  case EpwDataField::WindSpeed:
    string = "m/s";
    break;
  case EpwDataField::TotalSkyCover:
    //string = "None";
    break;
  case EpwDataField::OpaqueSkyCover:
    //string = "None";
    break;
  case EpwDataField::Visibility:
    string = "km";
    break;
  case EpwDataField::CeilingHeight:
    string = "m";
    break;
  case EpwDataField::PresentWeatherObservation:
    //string = "None";
    break;
  case EpwDataField::PresentWeatherCodes:
    //string = "None";
    break;
  case EpwDataField::PrecipitableWater:
    string = "mm";
    break;
  case EpwDataField::AerosolOpticalDepth:
    string = "thousandths";
    break;
  case EpwDataField::SnowDepth:
    string = "cm";
    break;
  case EpwDataField::DaysSinceLastSnowfall:
    //string = "None";
    break;
  case EpwDataField::Albedo:
    //string = "None";
    break;
  case EpwDataField::LiquidPrecipitationDepth:
    string = "mm";
    break;
  case EpwDataField::LiquidPrecipitationQuantity:
    string = "hr";
    break;
  }
  return string;
}

boost::optional<double> EpwDataPoint::fieldByName(std::string name)
{
  EpwDataField id;
  try
  {
    id = EpwDataField(name);
  }
  catch(...)
  {
    // Could do a warning message here
    return boost::optional<double>();
  }
  return field(id);
}

boost::optional<double> EpwDataPoint::field(EpwDataField id)
{
  boost::optional<int> ivalue;
  switch(id.value())
  {
  case EpwDataField::DryBulbTemperature:
    return dryBulbTemperature();
    break;
  case EpwDataField::DewPointTemperature:
    return dewPointTemperature();
    break;
  case EpwDataField::RelativeHumidity:
    return relativeHumidity();
    break;
  case EpwDataField::AtmosphericStationPressure:
    return atmosphericStationPressure();
    break;
  case EpwDataField::ExtraterrestrialHorizontalRadiation:
    return extraterrestrialHorizontalRadiation();
    break;
  case EpwDataField::ExtraterrestrialDirectNormalRadiation:
    return extraterrestrialDirectNormalRadiation();
    break;
  case EpwDataField::HorizontalInfraredRadiationIntensity:
    return horizontalInfraredRadiationIntensity();
    break;
  case EpwDataField::GlobalHorizontalRadiation:
    return globalHorizontalRadiation();
    break;
  case EpwDataField::DirectNormalRadiation:
    return directNormalRadiation();
    break;
  case EpwDataField::DiffuseHorizontalRadiation:
    return diffuseHorizontalRadiation();
    break;
  case EpwDataField::GlobalHorizontalIlluminance:
    return globalHorizontalIlluminance();
    break;
  case EpwDataField::DirectNormalIlluminance:
    return directNormalIlluminance();
    break;
  case EpwDataField::DiffuseHorizontalIlluminance:
    return diffuseHorizontalIlluminance();
    break;
  case EpwDataField::ZenithLuminance:
    return zenithLuminance();
    break;
  case EpwDataField::WindDirection:
    return windDirection();
    break;
  case EpwDataField::WindSpeed:
    return windSpeed();
    break;
  case EpwDataField::TotalSkyCover:
    ivalue = totalSkyCover();
    if(ivalue)
    {
      return boost::optional<double>((double)ivalue.get());
    }
    break;
  case EpwDataField::OpaqueSkyCover:
    ivalue = opaqueSkyCover();
    if(ivalue)
    {
      return boost::optional<double>((double)ivalue.get());
    }
    break;
  case EpwDataField::Visibility:
    return visibility();
    break;
  case EpwDataField::CeilingHeight:
    return ceilingHeight();
    break;
  case EpwDataField::PresentWeatherObservation:
    ivalue = presentWeatherObservation();
    if(ivalue)
    {
      return boost::optional<double>((double)ivalue.get());
    }
    break;
  case EpwDataField::PresentWeatherCodes:
    ivalue = presentWeatherCodes();
    if(ivalue)
    {
      return boost::optional<double>((double)ivalue.get());
    }
    break;
  case EpwDataField::PrecipitableWater:
    return precipitableWater();
    break;
  case EpwDataField::AerosolOpticalDepth:
    return aerosolOpticalDepth();
    break;
  case EpwDataField::SnowDepth:
    return snowDepth();
    break;
  case EpwDataField::DaysSinceLastSnowfall:
    return daysSinceLastSnowfall();
    break;
  case EpwDataField::Albedo:
    return albedo();
    break;
  case EpwDataField::LiquidPrecipitationDepth:
    return liquidPrecipitationDepth();
    break;
  case EpwDataField::LiquidPrecipitationQuantity:
    return liquidPrecipitationQuantity();
    break;
  default:
    // Could do a warning message here
    return boost::optional<double>();
    break;
  }
  return boost::optional<double>();
}

static double psat(double T)
{
  // Compute water vapor saturation pressure, eqns 5 and 6 from ASHRAE Fundamentals 2009 Ch. 1
  double C1 =-5.6745359e+03;
  double C2 = 6.3925247e+00;
  double C3 =-9.6778430e-03;
  double C4 = 6.2215701e-07;
  double C5 = 2.0747825e-09;
  double C6 =-9.4840240e-13;
  double C7 = 4.1635019e+00;
  double C8 =-5.8002206e+03;
  double C9 = 1.3914993e+00;
  double C10=-4.8640239e-02;
  double C11= 4.1764768e-05;
  double C12=-1.4452093e-08;
  double C13= 6.5459673e+00;
  double rhs;
  if(T<273.15)
  {
    rhs = C1/T + C2 + T*(C3 + T*(C4 + T*(C5 + T*C6))) + C7*std::log(T);
  }
  else
  {
    rhs = C8/T + C9 + T*(C10 + T*(C11 + T*C12)) + C13*std::log(T);
  }
  return exp(rhs);
}

std::string EpwDataPoint::toWthString()
{
  QStringList output;
  QString date = QString("%1/%2").arg(m_month).arg(m_day);
  output << date;
  QString hms = QString().sprintf("%02d:%02d:00",m_hour,m_minute);
  output << hms;
  boost::optional<double> value = dryBulbTemperature();
  if(!value)
  {
    LOG_FREE_AND_THROW("openstudio.EpwFile",QString("Missing dry bulb temperature on %1 at %2").arg(date).arg(hms).toStdString());
  }
  double drybulb = value.get()+273.15;
  output << QString("%1").arg(drybulb);
  value = atmosphericStationPressure();
  if(!value)
  {
    LOG_FREE_AND_THROW("openstudio.EpwFile",QString("Missing atmospheric station pressure on %1 at %2").arg(date).arg(hms).toStdString());
  }
  double p = value.get();
  output << m_atmosphericStationPressure;
  if(!windSpeed())
  {
    LOG_FREE_AND_THROW("openstudio.EpwFile",QString("Missing wind speed on %1 at %2").arg(date).arg(hms).toStdString());
  }
  output << m_windSpeed;
  if(!windDirection())
  {
    LOG_FREE_AND_THROW("openstudio.EpwFile",QString("Missing wind direction on %1 at %2").arg(date).arg(hms).toStdString());
  }
  output << m_windDirection;
  double pw;
  value = relativeHumidity();
  if(!value) // Don't have relative humidity - this has not been tested
  {
    value = dewPointTemperature();
    if(!value)
    {
      LOG_FREE_AND_THROW("openstudio.EpwFile",QString("Cannot compute humidity ratio on %1 at %2").arg(date).arg(hms).toStdString());
    }
    double dewpoint = value.get()+273.15;
    pw = psat(dewpoint);
  }
  else // Have relative humidity
  {
    double pws = psat(drybulb);
    pw = 0.01*value.get()*pws;
  }
  double W = 0.621945*pw/(p-pw);
  output << QString("%1").arg(W*1000); // need g/kg
  // Pass on solar flux quantities
  output << "0" << "0";
  // Pass on Tsky
  output << "0";
  // Pass on snow and rain
  output << "0" << "0";
  return output.join("\t").toStdString();
}

Date EpwDataPoint::date() const
{
  return Date(MonthOfYear(m_month),m_day);//,m_year);
}

void EpwDataPoint::setDate(Date date)
{
  m_month = openstudio::month(date.monthOfYear());
  m_day = date.dayOfMonth();
  m_year = date.year();
}

Time EpwDataPoint::time() const
{
  return Time(0,m_hour,m_minute);
}

void EpwDataPoint::setTime(Time time)
{
  m_hour = time.hours();
  m_minute = time.minutes();
}

DateTime EpwDataPoint::dateTime() const
{
  return DateTime(date(),time());
}

void EpwDataPoint::setDateTime(DateTime dateTime)
{
  setDate(dateTime.date());
  setTime(dateTime.time());
}

int EpwDataPoint::year() const
{
  return m_year;
}

void EpwDataPoint::setYear(int year)
{
  m_year = year;
}

bool EpwDataPoint::setYear(std::string year)
{
  bool ok;
  int value = QString().fromStdString(year).toInt(&ok);
  if(!ok)
  {
    return false;
  }
  m_year = value;
  return true;
}

int EpwDataPoint::month() const
{
  return m_month;
}

bool EpwDataPoint::setMonth(int month)
{
  if(1 > month || 12 < month)
  {
    return false;
  }
  m_month = month;
  return true;
}

bool EpwDataPoint::setMonth(std::string month)
{
  bool ok;
  int value = QString().fromStdString(month).toInt(&ok);
  if(1 > value || 12 < value || !ok)
  {
    return false;
  }
  m_month = value;
  return true;
}

int EpwDataPoint::day() const
{
  return m_day;
}

bool EpwDataPoint::setDay(int day)
{
  if(1 > day || 31 < day)
  {
    return false;
  }
  m_day = day;
  return true;
}

bool EpwDataPoint::setDay(std::string day)
{
  bool ok;
  int value = QString().fromStdString(day).toInt(&ok);
  if(1 > value || 31 < value || !ok)
  {
    return false;
  }
  m_day = value;
  return true;
}

int EpwDataPoint::hour() const
{
  return m_hour;
}

bool EpwDataPoint::setHour(int hour)
{
  if(1 > hour || 24 < hour)
  {
    return false;
  }
  m_hour = hour;
  return true;
}

bool EpwDataPoint::setHour(std::string hour)
{
  bool ok;
  int value = QString().fromStdString(hour).toInt(&ok);
  if(1 > value || 24 < value || !ok)
  {
    return false;
  }
  m_hour = value;
  return true;
}

int EpwDataPoint::minute() const
{
  return m_minute;
}

bool EpwDataPoint::setMinute(int minute)
{
  if(0 > minute || 59 < minute)
  {
    return false;
  }
  m_minute = minute;
  return true;
}

bool EpwDataPoint::setMinute(std::string minute)
{
  bool ok;
  int value = QString().fromStdString(minute).toInt(&ok);
  if(0 > value || 59 < value || !ok)
  {
    return false;
  }
  m_minute = value;
  return true;
}

std::string EpwDataPoint::dataSourceandUncertaintyFlags() const
{
  return m_dataSourceandUncertaintyFlags;
}

void EpwDataPoint::setDataSourceandUncertaintyFlags(std::string dataSourceandUncertaintyFlags)
{
  m_dataSourceandUncertaintyFlags = dataSourceandUncertaintyFlags;
}

boost::optional<double> EpwDataPoint::dryBulbTemperature() const
{
  double value = m_dryBulbTemperature.toDouble();
  if(value == 99.9)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setDryBulbTemperature(double dryBulbTemperature)
{
  if(-70 >= dryBulbTemperature)
  {
    m_dryBulbTemperature = "99.9";
    return false;
  }
  m_dryBulbTemperature = QString("%1").arg(dryBulbTemperature);
  return true;
}

bool EpwDataPoint::setDryBulbTemperature(std::string dryBulbTemperature)
{
  bool ok;
  double value = QString().fromStdString(dryBulbTemperature).toDouble(&ok);
  if(-70 >= value || !ok)
  {
    m_dryBulbTemperature = "99.9";
    return false;
  }
  m_dryBulbTemperature = QString().fromStdString(dryBulbTemperature);
  return true;
}

boost::optional<double> EpwDataPoint::dewPointTemperature() const
{
  double value = m_dewPointTemperature.toDouble();
  if(value == 99.9)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setDewPointTemperature(double dewPointTemperature)
{
  if(-70 >= dewPointTemperature)
  {
    m_dewPointTemperature = "99.9";
    return false;
  }
  m_dewPointTemperature = QString("%1").arg(dewPointTemperature);
  return true;
}

bool EpwDataPoint::setDewPointTemperature(std::string dewPointTemperature)
{
  bool ok;
  double value = QString().fromStdString(dewPointTemperature).toDouble(&ok);
  if(-70 >= value || !ok)
  {
    m_dewPointTemperature = "99.9";
    return false;
  }
  m_dewPointTemperature = QString().fromStdString(dewPointTemperature);
  return true;
}

boost::optional<double> EpwDataPoint::relativeHumidity() const
{
  double value = m_relativeHumidity.toDouble();
  if(value == 999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setRelativeHumidity(double relativeHumidity)
{
  if(0 > relativeHumidity || 110 < relativeHumidity)
  {
    m_relativeHumidity = "999";
    return false;
  }
  m_relativeHumidity = QString("%1").arg(relativeHumidity);
  return true;
}

bool EpwDataPoint::setRelativeHumidity(std::string relativeHumidity)
{
  bool ok;
  double value = QString().fromStdString(relativeHumidity).toDouble(&ok);
  if(0 > value || 110 < value || !ok)
  {
    m_relativeHumidity = "999";
    return false;
  }
  m_relativeHumidity = QString().fromStdString(relativeHumidity);
  return true;
}

boost::optional<double> EpwDataPoint::atmosphericStationPressure() const
{
  double value = m_atmosphericStationPressure.toDouble();
  if(value == 999999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setAtmosphericStationPressure(double atmosphericStationPressure)
{
  if(31000 >= atmosphericStationPressure)
  {
    m_atmosphericStationPressure = "999999";
    return false;
  }
  m_atmosphericStationPressure = QString("%1").arg(atmosphericStationPressure);
  return true;
}

bool EpwDataPoint::setAtmosphericStationPressure(std::string atmosphericStationPressure)
{
  bool ok;
  double value = QString().fromStdString(atmosphericStationPressure).toDouble(&ok);
  if(31000 >= value || !ok)
  {
    m_atmosphericStationPressure = "999999";
    return false;
  }
  m_atmosphericStationPressure = QString().fromStdString(atmosphericStationPressure);
  return true;
}

boost::optional<double> EpwDataPoint::extraterrestrialHorizontalRadiation() const
{
  double value = m_extraterrestrialHorizontalRadiation.toDouble();
  if(value == 9999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setExtraterrestrialHorizontalRadiation(double extraterrestrialHorizontalRadiation)
{
  if(0 > extraterrestrialHorizontalRadiation)
  {
    m_extraterrestrialHorizontalRadiation = "9999";
    return false;
  }
  m_extraterrestrialHorizontalRadiation = QString("%1").arg(extraterrestrialHorizontalRadiation);
  return true;
}

bool EpwDataPoint::setExtraterrestrialHorizontalRadiation(std::string extraterrestrialHorizontalRadiation)
{
  bool ok;
  double value = QString().fromStdString(extraterrestrialHorizontalRadiation).toDouble(&ok);
  if(0 > value || !ok)
  {
    m_extraterrestrialHorizontalRadiation = "9999";
    return false;
  }
  m_extraterrestrialHorizontalRadiation = QString().fromStdString(extraterrestrialHorizontalRadiation);
  return true;
}

boost::optional<double> EpwDataPoint::extraterrestrialDirectNormalRadiation() const
{
  double value = m_extraterrestrialDirectNormalRadiation.toDouble();
  if(value == 9999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setExtraterrestrialDirectNormalRadiation(double extraterrestrialDirectNormalRadiation)
{
  if(0 > extraterrestrialDirectNormalRadiation)
  {
    m_extraterrestrialDirectNormalRadiation = "9999";
    return false;
  }
  m_extraterrestrialDirectNormalRadiation = QString("%1").arg(extraterrestrialDirectNormalRadiation);
  return true;
}

bool EpwDataPoint::setExtraterrestrialDirectNormalRadiation(std::string extraterrestrialDirectNormalRadiation)
{
  bool ok;
  double value = QString().fromStdString(extraterrestrialDirectNormalRadiation).toDouble(&ok);
  if(0 > value || !ok)
  {
    m_extraterrestrialDirectNormalRadiation = "9999";
    return false;
  }
  m_extraterrestrialDirectNormalRadiation = QString().fromStdString(extraterrestrialDirectNormalRadiation);
  return true;
}

boost::optional<double> EpwDataPoint::horizontalInfraredRadiationIntensity() const
{
  double value = m_horizontalInfraredRadiationIntensity.toDouble();
  if(value == 9999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setHorizontalInfraredRadiationIntensity(double horizontalInfraredRadiationIntensity)
{
  if(0 > horizontalInfraredRadiationIntensity)
  {
    m_horizontalInfraredRadiationIntensity = "9999";
    return false;
  }
  m_horizontalInfraredRadiationIntensity = QString("%1").arg(horizontalInfraredRadiationIntensity);
  return true;
}

bool EpwDataPoint::setHorizontalInfraredRadiationIntensity(std::string horizontalInfraredRadiationIntensity)
{
  bool ok;
  double value = QString().fromStdString(horizontalInfraredRadiationIntensity).toDouble(&ok);
  if(0 > value || !ok)
  {
    m_horizontalInfraredRadiationIntensity = "9999";
    return false;
  }
  m_horizontalInfraredRadiationIntensity = QString().fromStdString(horizontalInfraredRadiationIntensity);
  return true;
}

boost::optional<double> EpwDataPoint::globalHorizontalRadiation() const
{
  double value = m_globalHorizontalRadiation.toDouble();
  if(value == 9999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setGlobalHorizontalRadiation(double globalHorizontalRadiation)
{
  if(0 > globalHorizontalRadiation)
  {
    m_globalHorizontalRadiation = "9999";
    return false;
  }
  m_globalHorizontalRadiation = QString("%1").arg(globalHorizontalRadiation);
  return true;
}

bool EpwDataPoint::setGlobalHorizontalRadiation(std::string globalHorizontalRadiation)
{
  bool ok;
  double value = QString().fromStdString(globalHorizontalRadiation).toDouble(&ok);
  if(0 > value || !ok)
  {
    m_globalHorizontalRadiation = "9999";
    return false;
  }
  m_globalHorizontalRadiation = QString().fromStdString(globalHorizontalRadiation);
  return true;
}

boost::optional<double> EpwDataPoint::directNormalRadiation() const
{
  double value = m_directNormalRadiation.toDouble();
  if(value == 9999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setDirectNormalRadiation(double directNormalRadiation)
{
  if(0 > directNormalRadiation)
  {
    m_directNormalRadiation = "9999";
    return false;
  }
  m_directNormalRadiation = QString("%1").arg(directNormalRadiation);
  return true;
}

bool EpwDataPoint::setDirectNormalRadiation(std::string directNormalRadiation)
{
  bool ok;
  double value = QString().fromStdString(directNormalRadiation).toDouble(&ok);
  if(0 > value || !ok)
  {
    m_directNormalRadiation = "9999";
    return false;
  }
  m_directNormalRadiation = QString().fromStdString(directNormalRadiation);
  return true;
}

boost::optional<double> EpwDataPoint::diffuseHorizontalRadiation() const
{
  double value = m_diffuseHorizontalRadiation.toDouble();
  if(value == 9999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setDiffuseHorizontalRadiation(double diffuseHorizontalRadiation)
{
  if(0 > diffuseHorizontalRadiation)
  {
    m_diffuseHorizontalRadiation = "9999";
    return false;
  }
  m_diffuseHorizontalRadiation = QString("%1").arg(diffuseHorizontalRadiation);
  return true;
}

bool EpwDataPoint::setDiffuseHorizontalRadiation(std::string diffuseHorizontalRadiation)
{
  bool ok;
  double value = QString().fromStdString(diffuseHorizontalRadiation).toDouble(&ok);
  if(0 > value || !ok)
  {
    m_diffuseHorizontalRadiation = "9999";
    return false;
  }
  m_diffuseHorizontalRadiation = QString().fromStdString(diffuseHorizontalRadiation);
  return true;
}

boost::optional<double> EpwDataPoint::globalHorizontalIlluminance() const
{
  double value = m_globalHorizontalIlluminance.toDouble();
  if(value == 999999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setGlobalHorizontalIlluminance(double globalHorizontalIlluminance)
{
  if(0 > globalHorizontalIlluminance)
  {
    m_globalHorizontalIlluminance = "999999";
    return false;
  }
  m_globalHorizontalIlluminance = QString("%1").arg(globalHorizontalIlluminance);
  return true;
}

bool EpwDataPoint::setGlobalHorizontalIlluminance(std::string globalHorizontalIlluminance)
{
  bool ok;
  double value = QString().fromStdString(globalHorizontalIlluminance).toDouble(&ok);
  if(0 > value || !ok)
  {
    m_globalHorizontalIlluminance = "999999";
    return false;
  }
  m_globalHorizontalIlluminance = QString().fromStdString(globalHorizontalIlluminance);
  return true;
}

boost::optional<double> EpwDataPoint::directNormalIlluminance() const
{
  double value = m_directNormalIlluminance.toDouble();
  if(value == 999999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setDirectNormalIlluminance(double directNormalIlluminance)
{
  if(0 > directNormalIlluminance)
  {
    m_directNormalIlluminance = "999999";
    return false;
  }
  m_directNormalIlluminance = QString("%1").arg(directNormalIlluminance);
  return true;
}

bool EpwDataPoint::setDirectNormalIlluminance(std::string directNormalIlluminance)
{
  bool ok;
  double value = QString().fromStdString(directNormalIlluminance).toDouble(&ok);
  if(0 > value || !ok)
  {
    m_directNormalIlluminance = "999999";
    return false;
  }
  m_directNormalIlluminance = QString().fromStdString(directNormalIlluminance);
  return true;
}

boost::optional<double> EpwDataPoint::diffuseHorizontalIlluminance() const
{
  double value = m_diffuseHorizontalIlluminance.toDouble();
  if(value == 999999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setDiffuseHorizontalIlluminance(double diffuseHorizontalIlluminance)
{
  if(0 > diffuseHorizontalIlluminance)
  {
    m_diffuseHorizontalIlluminance = "999999";
    return false;
  }
  m_diffuseHorizontalIlluminance = QString("%1").arg(diffuseHorizontalIlluminance);
  return true;
}

bool EpwDataPoint::setDiffuseHorizontalIlluminance(std::string diffuseHorizontalIlluminance)
{
  bool ok;
  double value = QString().fromStdString(diffuseHorizontalIlluminance).toDouble(&ok);
  if(0 > value || !ok)
  {
    m_diffuseHorizontalIlluminance = "999999";
    return false;
  }
  m_diffuseHorizontalIlluminance = QString().fromStdString(diffuseHorizontalIlluminance);
  return true;
}

boost::optional<double> EpwDataPoint::zenithLuminance() const
{
  double value = m_zenithLuminance.toDouble();
  if(value == 9999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setZenithLuminance(double zenithLuminance)
{
  if(0 > zenithLuminance)
  {
    m_zenithLuminance = "9999";
    return false;
  }
  m_zenithLuminance = QString("%1").arg(zenithLuminance);
  return true;
}

bool EpwDataPoint::setZenithLuminance(std::string zenithLuminance)
{
  bool ok;
  double value = QString().fromStdString(zenithLuminance).toDouble(&ok);
  if(0 > value || !ok)
  {
    m_zenithLuminance = "9999";
    return false;
  }
  m_zenithLuminance = QString().fromStdString(zenithLuminance);
  return true;
}

boost::optional<double> EpwDataPoint::windDirection() const
{
  double value = m_windDirection.toDouble();
  if(value == 999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setWindDirection(double windDirection)
{
  if(0 > windDirection || 360 < windDirection)
  {
    m_windDirection = "999";
    return false;
  }
  m_windDirection = QString("%1").arg(windDirection);
  return true;
}

bool EpwDataPoint::setWindDirection(std::string windDirection)
{
  bool ok;
  double value = QString().fromStdString(windDirection).toDouble(&ok);
  if(0 > value || 360 < value || !ok)
  {
    m_windDirection = "999";
    return false;
  }
  m_windDirection = QString().fromStdString(windDirection);
  return true;
}

boost::optional<double> EpwDataPoint::windSpeed() const
{
  double value = m_windSpeed.toDouble();
  if(value == 999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

bool EpwDataPoint::setWindSpeed(double windSpeed)
{
  if(0 > windSpeed || 40 < windSpeed)
  {
    m_windSpeed = "999";
    return false;
  }
  m_windSpeed = QString("%1").arg(windSpeed);
  return true;
}

bool EpwDataPoint::setWindSpeed(std::string windSpeed)
{
  bool ok;
  double value = QString().fromStdString(windSpeed).toDouble(&ok);
  if(0 > value || 40 < value || !ok)
  {
    m_windSpeed = "999";
    return false;
  }
  m_windSpeed = QString().fromStdString(windSpeed);
  return true;
}

int EpwDataPoint::totalSkyCover() const
{
  return m_totalSkyCover;
}

bool EpwDataPoint::setTotalSkyCover(int totalSkyCover)
{
  if(0 > totalSkyCover || 10 < totalSkyCover)
  {
    m_totalSkyCover = 99;
    return false;
  }
  m_totalSkyCover = totalSkyCover;
  return true;
}

bool EpwDataPoint::setTotalSkyCover(std::string totalSkyCover)
{
  bool ok;
  int value = QString().fromStdString(totalSkyCover).toInt(&ok);
  if(0 > value || 10 < value || !ok)
  {
    m_totalSkyCover = 99;
    return false;
  }
  m_totalSkyCover = value;
  return true;
}

int EpwDataPoint::opaqueSkyCover() const
{
  return m_opaqueSkyCover;
}

bool EpwDataPoint::setOpaqueSkyCover(int opaqueSkyCover)
{
  if(0 > opaqueSkyCover || 10 < opaqueSkyCover)
  {
    m_opaqueSkyCover = 99;
    return false;
  }
  m_opaqueSkyCover = opaqueSkyCover;
  return true;
}

bool EpwDataPoint::setOpaqueSkyCover(std::string opaqueSkyCover)
{
  bool ok;
  int value = QString().fromStdString(opaqueSkyCover).toInt(&ok);
  if(0 > value || 10 < value || !ok)
  {
    m_opaqueSkyCover = 99;
    return false;
  }
  m_opaqueSkyCover = value;
  return true;
}

boost::optional<double> EpwDataPoint::visibility() const
{
  double value = m_visibility.toDouble();
  if(value == 9999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

void EpwDataPoint::setVisibility(double visibility)
{
  m_visibility = QString("%1").arg(visibility);
}

bool EpwDataPoint::setVisibility(std::string visibility)
{
  bool ok;
  QString().fromStdString(visibility).toDouble(&ok);
  if(!ok)
  {
    m_visibility = "9999";
    return false;
  }
  m_visibility = QString().fromStdString(visibility);
  return true;
}

boost::optional<double> EpwDataPoint::ceilingHeight() const
{
  double value = m_ceilingHeight.toDouble();
  if(value == 99999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

void EpwDataPoint::setCeilingHeight(double ceilingHeight)
{
  m_ceilingHeight = QString("%1").arg(ceilingHeight);
}

bool EpwDataPoint::setCeilingHeight(std::string ceilingHeight)
{
  bool ok;
  QString().fromStdString(ceilingHeight).toDouble(&ok);
  if(!ok)
  {
    m_ceilingHeight = "99999";
    return false;
  }
  m_ceilingHeight = QString().fromStdString(ceilingHeight);
  return true;
}

int EpwDataPoint::presentWeatherObservation() const
{
  return m_presentWeatherObservation;
}

void EpwDataPoint::setPresentWeatherObservation(int presentWeatherObservation)
{
  m_presentWeatherObservation = presentWeatherObservation;
}

bool EpwDataPoint::setPresentWeatherObservation(std::string presentWeatherObservation)
{
  bool ok;
  int value = QString().fromStdString(presentWeatherObservation).toInt(&ok);
  if(!ok)
  {
    return false;
  }
  m_presentWeatherObservation = value;
  return true;
}

int EpwDataPoint::presentWeatherCodes() const
{
  return m_presentWeatherCodes;
}

void EpwDataPoint::setPresentWeatherCodes(int presentWeatherCodes)
{
  m_presentWeatherCodes = presentWeatherCodes;
}

bool EpwDataPoint::setPresentWeatherCodes(std::string presentWeatherCodes)
{
  bool ok;
  int value = QString().fromStdString(presentWeatherCodes).toInt(&ok);
  if(!ok)
  {
    return false;
  }
  m_presentWeatherCodes = value;
  return true;
}

boost::optional<double> EpwDataPoint::precipitableWater() const
{
  double value = m_precipitableWater.toDouble();
  if(value == 999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

void EpwDataPoint::setPrecipitableWater(double precipitableWater)
{
  m_precipitableWater = QString("%1").arg(precipitableWater);
}

bool EpwDataPoint::setPrecipitableWater(std::string precipitableWater)
{
  bool ok;
  QString().fromStdString(precipitableWater).toDouble(&ok);
  if(!ok)
  {
    m_precipitableWater = "999";
    return false;
  }
  m_precipitableWater = QString().fromStdString(precipitableWater);
  return true;
}

boost::optional<double> EpwDataPoint::aerosolOpticalDepth() const
{
  double value = m_aerosolOpticalDepth.toDouble();
  if(value == .999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

void EpwDataPoint::setAerosolOpticalDepth(double aerosolOpticalDepth)
{
  m_aerosolOpticalDepth = QString("%1").arg(aerosolOpticalDepth);
}

bool EpwDataPoint::setAerosolOpticalDepth(std::string aerosolOpticalDepth)
{
  bool ok;
  QString().fromStdString(aerosolOpticalDepth).toDouble(&ok);
  if(!ok)
  {
    m_aerosolOpticalDepth = ".999";
    return false;
  }
  m_aerosolOpticalDepth = QString().fromStdString(aerosolOpticalDepth);
  return true;
}

boost::optional<double> EpwDataPoint::snowDepth() const
{
  double value = m_snowDepth.toDouble();
  if(value == 999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

void EpwDataPoint::setSnowDepth(double snowDepth)
{
  m_snowDepth = QString("%1").arg(snowDepth);
}

bool EpwDataPoint::setSnowDepth(std::string snowDepth)
{
  bool ok;
  QString().fromStdString(snowDepth).toDouble(&ok);
  if(!ok)
  {
    m_snowDepth = "999";
    return false;
  }
  m_snowDepth = QString().fromStdString(snowDepth);
  return true;
}

boost::optional<double> EpwDataPoint::daysSinceLastSnowfall() const
{
  double value = m_daysSinceLastSnowfall.toDouble();
  if(value == 99)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

void EpwDataPoint::setDaysSinceLastSnowfall(double daysSinceLastSnowfall)
{
  m_daysSinceLastSnowfall = QString("%1").arg(daysSinceLastSnowfall);
}

bool EpwDataPoint::setDaysSinceLastSnowfall(std::string daysSinceLastSnowfall)
{
  bool ok;
  QString().fromStdString(daysSinceLastSnowfall).toDouble(&ok);
  if(!ok)
  {
    m_daysSinceLastSnowfall = "99";
    return false;
  }
  m_daysSinceLastSnowfall = QString().fromStdString(daysSinceLastSnowfall);
  return true;
}

boost::optional<double> EpwDataPoint::albedo() const
{
  double value = m_albedo.toDouble();
  if(value == 999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

void EpwDataPoint::setAlbedo(double albedo)
{
  m_albedo = QString("%1").arg(albedo);
}

bool EpwDataPoint::setAlbedo(std::string albedo)
{
  bool ok;
  QString().fromStdString(albedo).toDouble(&ok);
  if(!ok)
  {
    m_albedo = "999";
    return false;
  }
  m_albedo = QString().fromStdString(albedo);
  return true;
}

boost::optional<double> EpwDataPoint::liquidPrecipitationDepth() const
{
  double value = m_liquidPrecipitationDepth.toDouble();
  if(value == 999)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

void EpwDataPoint::setLiquidPrecipitationDepth(double liquidPrecipitationDepth)
{
  m_liquidPrecipitationDepth = QString("%1").arg(liquidPrecipitationDepth);
}

bool EpwDataPoint::setLiquidPrecipitationDepth(std::string liquidPrecipitationDepth)
{
  bool ok;
  QString().fromStdString(liquidPrecipitationDepth).toDouble(&ok);
  if(!ok)
  {
    m_liquidPrecipitationDepth = "999";
    return false;
  }
  m_liquidPrecipitationDepth = QString().fromStdString(liquidPrecipitationDepth);
  return true;
}

boost::optional<double> EpwDataPoint::liquidPrecipitationQuantity() const
{
  double value = m_liquidPrecipitationQuantity.toDouble();
  if(value == 99)
  {
    return boost::optional<double>();
  }
  return boost::optional<double>(value);
}

void EpwDataPoint::setLiquidPrecipitationQuantity(double liquidPrecipitationQuantity)
{
  m_liquidPrecipitationQuantity = QString("%1").arg(liquidPrecipitationQuantity);
}

bool EpwDataPoint::setLiquidPrecipitationQuantity(std::string liquidPrecipitationQuantity)
{
  bool ok;
  QString().fromStdString(liquidPrecipitationQuantity).toDouble(&ok);
  if(!ok)
  {
    m_liquidPrecipitationQuantity = "99";
    return false;
  }
  m_liquidPrecipitationQuantity = QString().fromStdString(liquidPrecipitationQuantity);
  return true;
}

EpwFile::EpwFile(const openstudio::path& p, bool storeData)
  : m_path(p), m_latitude(0), m_longitude(0), m_timeZone(0), m_elevation(0)
{
  if (!parse(storeData)){
    LOG_AND_THROW("EpwFile '" << toString(p) << "' cannot be processed");
  }
}

openstudio::path EpwFile::path() const
{
  return m_path;
}

std::string EpwFile::checksum() const
{
  return m_checksum;
}

std::string EpwFile::city() const
{
  return m_city;
}

std::string EpwFile::stateProvinceRegion() const
{
  return m_stateProvinceRegion;
}

std::string EpwFile::country() const
{
  return m_country;
}

std::string EpwFile::dataSource() const
{
  return m_dataSource;
}

std::string EpwFile::wmoNumber() const
{
  return m_wmoNumber;
}

double EpwFile::latitude() const
{
  return m_latitude;
}

double EpwFile::longitude() const
{
  return m_longitude;
}

double EpwFile::timeZone() const
{
  return m_timeZone;
}

double EpwFile::elevation() const
{
  return m_elevation;
}

Time EpwFile::timeStep() const
{
  return m_timeStep;
}

DayOfWeek EpwFile::startDayOfWeek() const
{
  return m_startDayOfWeek;
}

Date EpwFile::startDate() const
{
  return m_startDate;
}

Date EpwFile::endDate() const
{
  return m_endDate;
}

boost::optional<int> EpwFile::startDateActualYear() const
{
  return m_startDateActualYear;
}

boost::optional<int> EpwFile::endDateActualYear() const
{
  return m_endDateActualYear;
}

std::vector<EpwDataPoint> EpwFile::data()
{
  if(m_data.size()==0){
    if (!parse(true)){
      LOG(Error,"EpwFile '" << toString(m_path) << "' cannot be processed");
    }
  }
  return m_data;
}

boost::optional<TimeSeries> EpwFile::getTimeSeries(std::string name)
{
  if(m_data.size()==0){
    if (!parse(true)){
      LOG(Error,"EpwFile '" << toString(m_path) << "' cannot be processed");
      return boost::optional<TimeSeries>();
    }
  }
  EpwDataField id;
  try
  {
    id = EpwDataField(name);
  }
  catch(...)
  {
    // Could do a warning message here
    return boost::optional<TimeSeries>();
  }
  if(m_data.size())
  {
    std::string units = EpwDataPoint::units(id);
    DateTimeVector dates;
    std::vector<double> values;
    for(unsigned int i=0;i<m_data.size();i++)
    {
      Date date=m_data[i].date();
      Time time=m_data[i].time();
      boost::optional<double> value = m_data[i].field(id);
      if(value)
      {
        dates.push_back(DateTime(date,time));
        values.push_back(value.get());
      }
    }
    if(dates.size())
    {
      return boost::optional<TimeSeries>(TimeSeries(dates,openstudio::createVector(values),units));
    }
  }
  return boost::optional<TimeSeries>();
}

bool EpwFile::translateToWth(openstudio::path path, std::string description)
{
  if(m_data.size()==0)
  {
    if (!parse(true))
    {
      LOG(Error,"EpwFile '" << toString(m_path) << "' cannot be processed");
      return false;
    }
  }

  if(description.empty())
  {
    description = "Translated from " + openstudio::toString(this->path());
  }

  if(!data().size())
  {
    LOG(Error, "EPW file contains no data to translate");
    return false;
  }

  QFile fp(openstudio::toQString(path));
  if(!fp.open(QFile::WriteOnly))
  {
    LOG(Error, "Failed to open file '" + openstudio::toString(path) + "'");
    return false;
  }

  QTextStream stream(&fp);

  stream << "WeatherFile ContamW 2.0\n";
  stream << openstudio::toQString(description) << "\n";
  stream << QString("%1/%2\t!start date\n").arg(openstudio::month(startDate().monthOfYear())).arg(startDate().dayOfMonth());
  stream << QString("%1/%2\t!end date\n").arg(openstudio::month(endDate().monthOfYear())).arg(endDate().dayOfMonth());
  stream << "!Date\tDofW\tDtype\tDST\tTgrnd [K]\n";
  openstudio::Time delta(1,0);
  int dayofweek = startDayOfWeek().value()+1;
  for(openstudio::Date current=startDate();current<=endDate();current += delta)
  {
    stream << QString("%1/%2\t%3\t%3\t0\t283.15\n")
      .arg(openstudio::month(current.monthOfYear()))
      .arg(current.dayOfMonth())
      .arg(dayofweek);
    dayofweek++;
    if(dayofweek > 7)
    {
      dayofweek=1;
    }
  }

  // Cheat to get data at the start time - this will need to change
  openstudio::EpwDataPoint firstPt = data()[data().size()-1];
  openstudio::DateTime dateTime = data()[0].dateTime();
  openstudio::Time dt = timeStep();
  dateTime -= dt;
  firstPt.setDateTime(dateTime);

  stream <<"!Date\tTime\tTa [K]\tPb [Pa]\tWs [m/s]\tWd [deg]\tHr [g/kg]\tIth [kJ/m^2]\tIdn [kJ/m^2]\tTs [K]\tRn [-]\tSn [-]\n";
  try
  {
    stream << firstPt.toWthString() << '\n';
    for(unsigned int i=0;i<data().size();i++)
    {
      stream << data()[i].toWthString() << '\n';
    }
  }
  catch(...)
  {
    LOG(Error, "Translation to WTH has failed");
    fp.close();
    return false;
  }
  fp.close();
  return true;
}

bool EpwFile::parse(bool storeData)
{
  if (!boost::filesystem::exists(m_path) || !boost::filesystem::is_regular_file(m_path)){
    LOG(Error, "Path '" << m_path << "' is not an EPW file");
    return false;
  }

  // set checksum
  m_checksum = openstudio::checksum(m_path);

  // open file
  boost::filesystem::ifstream ifs(m_path);

  // read line by line
  std::string line;

  bool result = true;

  // read first 8 lines
  for(unsigned i = 0; i < 8; ++i){

    if(!std::getline(ifs, line)){
      LOG(Error, "Could not read line " << i+1 << " of EPW file '" << m_path << "'");
      ifs.close();
      return false;
    }

    switch(i){
      case 0:
        result = result && parseLocation(line);
        break;
      case 1:
        break;
      case 2:
        break;
      case 3:
        break;
      case 4:
        break;
      case 5:
        break;
      case 6:
        break;
      case 7:
        result = result && parseDataPeriod(line);
        break;
      default:
        ;
    }
  }

  // read rest of file
  boost::optional<Date> startDate;
  boost::optional<Date> lastDate;
  boost::optional<Date> endDate;
  bool realYear = true;
  bool wrapAround = false;
  while(std::getline(ifs, line)){
    boost::regex dateRegex("^(.*?),(.*?),(.*?),.*");
    boost::smatch matches;
    if (boost::regex_search(line, matches, dateRegex)){
      std::string year = std::string(matches[1].first, matches[1].second); boost::trim(year);
      std::string month = std::string(matches[2].first, matches[2].second); boost::trim(month);
      std::string day = std::string(matches[3].first, matches[3].second); boost::trim(day);

      try{
        Date date(boost::lexical_cast<int>(month), boost::lexical_cast<int>(day), boost::lexical_cast<int>(year));
        
        if (!startDate){
          startDate = date;
        }
        endDate = date;

        if (endDate && lastDate){
          Time delta = endDate.get() - lastDate.get();
          if (std::abs(delta.totalDays()) > 1){
            realYear = false;
          }

          if (endDate->monthOfYear().value() < lastDate->monthOfYear().value()){
            wrapAround = true;
          }
        }
        lastDate = date;
      }catch(...){
        LOG(Error, "Could not read line " << line << ", EPW file '" << m_path << "'");
        ifs.close();
        return false;
      }
      if(storeData)
      {
        EpwDataPoint pt(line);
        m_data.push_back(pt);
      }
    }else{
      LOG(Error, "Could not read line " << line << ", EPW file '" << m_path << "'");
      ifs.close();
      return false;
    }
  }

  // close file
  ifs.close();

  if (!startDate){
    LOG(Error, "Could not find start date in data section, EPW file '" << m_path << "'");
    return false;
  }
  if (!endDate){
    LOG(Error, "Could not find end date in data section, EPW file '" << m_path << "'");
    return false;
  }

  if ((m_startDate.monthOfYear() != startDate->monthOfYear()) ||
      (m_startDate.dayOfMonth() != startDate->dayOfMonth()) ||
      (m_endDate.monthOfYear() != endDate->monthOfYear()) ||
      (m_endDate.dayOfMonth() != endDate->dayOfMonth())){
    LOG(Error, "Header start and end dates do not match data, EPW file '" << m_path << "'");
    return false;
  }

  if (realYear){
    if (m_startDayOfWeek != startDate->dayOfWeek()){
      LOG(Error, "Header start and end dates do not match data, EPW file '" << m_path << "'");
      return false;
    }

    // set dates with years
    m_startDate = startDate.get();
    m_startDateActualYear = startDate->year();

    m_endDate = endDate.get();
    m_endDateActualYear = endDate->year();
  }else{
    if (wrapAround){
      LOG(Error, "Wrap around years not supported for TMY data, EPW file '" << m_path << "'");
      return false;
    }
  }

  return result;
}

bool EpwFile::parseLocation(const std::string& line)
{
  bool result = true;

  // LOCATION,Chicago Ohare Intl Ap,IL,USA,TMY3,725300,41.98,-87.92,-6.0,201.0
  boost::regex locationRegex("^LOCATION,(.*?),(.*?),(.*?),(.*?),(.*?),(.*?),(.*?),(.*?),(.*?)$");
  boost::smatch matches;
  if (boost::regex_search(line, matches, locationRegex)){
    std::string city = std::string(matches[1].first, matches[1].second); boost::trim(city);
    std::string stateProvinceRegion = std::string(matches[2].first, matches[2].second); boost::trim(stateProvinceRegion);
    std::string country = std::string(matches[3].first, matches[3].second); boost::trim(country);
    std::string dataSource = std::string(matches[4].first, matches[4].second); boost::trim(dataSource);
    std::string wmoNumber = std::string(matches[5].first, matches[5].second); boost::trim(wmoNumber);
    std::string latitude = std::string(matches[6].first, matches[6].second); boost::trim(latitude);
    std::string longitude = std::string(matches[7].first, matches[7].second); boost::trim(longitude);
    std::string timeZone = std::string(matches[8].first, matches[8].second); boost::trim(timeZone);
    std::string elevation = std::string(matches[9].first, matches[9].second); boost::trim(elevation);

    m_city = city;
    m_stateProvinceRegion = stateProvinceRegion;
    m_country = country;
    m_dataSource = dataSource;
    m_wmoNumber = wmoNumber;
    try{
      m_latitude = boost::lexical_cast<double>(latitude);
    }catch(...){
      result = false;
    }
    try{
      m_longitude = boost::lexical_cast<double>(longitude);
    }catch(...){
      result = false;
    }
    try{
      m_timeZone = boost::lexical_cast<double>(timeZone);
    }catch(...){
      result = false;
    }
    try{
      m_elevation = boost::lexical_cast<double>(elevation);
    }catch(...){
      result = false;
    }

  }else{
    // can't read line
    LOG(Error, "Could not read location from EPW file '" << m_path << "'");
    result = false;
  }

  return(result);
}

bool EpwFile::parseDataPeriod(const std::string& line)
{
  bool result = true;

  // DATA PERIODS,1,1,Data,Sunday, 1/ 1,12/31
  boost::regex dataPeriodRegex("^DATA PERIODS,(.*?),(.*?),(.*?),(.*?),(.*?),(.*?)$");
  boost::smatch matches;
  if (boost::regex_search(line, matches, dataPeriodRegex)){
    std::string nDataPeriods =  std::string(matches[1].first, matches[1].second); boost::trim(nDataPeriods);
    std::string timeStep = std::string(matches[2].first, matches[2].second); boost::trim(timeStep);
    std::string startDayOfWeek = std::string(matches[4].first, matches[4].second); boost::trim(startDayOfWeek);
    std::string startDate = std::string(matches[5].first, matches[5].second); boost::trim(startDate);
    std::string endDate = std::string(matches[6].first, matches[6].second); boost::trim(endDate);

    try{
      int N = boost::lexical_cast<int>(nDataPeriods);
      if(N>1)
      {
        LOG(Error, "More than one data period in EPW file '" << m_path << "', which is not supported");
        result = false;
      }
    }catch(...){
      result = false;
    }
    try{
      m_timeStep = Time(boost::lexical_cast<double>(timeStep) / 24.0);
    }catch(...){
      result = false;
    }
    try{
      m_startDayOfWeek = DayOfWeek(startDayOfWeek);
    }catch(...){
      result = false;
    }
    try{
      boost::regex dateRegex("^(.*?)/(.*?)$");
      if (boost::regex_search(startDate, matches, dateRegex)){
        std::string month = std::string(matches[1].first, matches[1].second); boost::trim(month);
        std::string day = std::string(matches[2].first, matches[2].second); boost::trim(day);
        m_startDate = Date(monthOfYear(boost::lexical_cast<int>(month)), boost::lexical_cast<int>(day));
      }
    }catch(...){
      result = false;
    }
    try{
      boost::regex dateRegex("^(.*?)/(.*?)$");
      if (boost::regex_search(endDate, matches, dateRegex)){
        std::string month = std::string(matches[1].first, matches[1].second); boost::trim(month);
        std::string day = std::string(matches[2].first, matches[2].second); boost::trim(day);
        m_endDate = Date(monthOfYear(boost::lexical_cast<int>(month)), boost::lexical_cast<int>(day));
      }
    }catch(...){
      result = false;
    }

  }else{
    // can't read line
    LOG(Error, "Could not read data period from EPW file '" << m_path << "'");
    result = false;
  }

  return(result);
}

IdfObject toIdfObject(const EpwFile& epwFile) {
  IdfObject result(IddObjectType::OS_WeatherFile);

  bool success = true;

  success = success && result.setString(0,epwFile.city());
  success = success && result.setString(1,epwFile.stateProvinceRegion());
  success = success && result.setString(2,epwFile.country());
  success = success && result.setString(3,epwFile.dataSource());
  success = success && result.setString(4,epwFile.wmoNumber());
  success = success && result.setDouble(5,epwFile.latitude());
  success = success && result.setDouble(6,epwFile.longitude());
  success = success && result.setDouble(7,epwFile.timeZone());
  success = success && result.setDouble(8,epwFile.elevation());
  success = success && result.setString(9,toString(epwFile.path()));
  success = success && result.setString(10,epwFile.checksum());

  if (!success) {
    LOG_FREE(Error,"openstudio.EpwFile","Unable to construct WeatherFile IdfObject from epwFile at path "
             << epwFile.path() << ".");
  }

  return result;
}

} // openstudio
