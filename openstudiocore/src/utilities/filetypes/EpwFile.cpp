/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "EpwFile.hpp"
#include "../idf/IdfObject.hpp"
#include "../idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../core/Checksum.hpp"
#include "../core/Assert.hpp"
#include "../units/QuantityConverter.hpp"

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include <boost/filesystem/fstream.hpp>

#include <QStringList>
#include <QFile>
#include <QTextStream>

#include <cmath>

namespace openstudio{

EpwDataPoint::EpwDataPoint()
{
  m_year=1;
  m_month=1;
  m_day=1;
  m_hour=1;
  m_minute=0,
  m_dataSourceandUncertaintyFlags="";
  m_dryBulbTemperature="99.9";
  m_dewPointTemperature="99.9",
  m_relativeHumidity="999";
  m_atmosphericStationPressure="999999";
  m_extraterrestrialHorizontalRadiation="9999";
  m_extraterrestrialDirectNormalRadiation="9999";
  m_horizontalInfraredRadiationIntensity="9999",
  m_globalHorizontalRadiation="9999";
  m_directNormalRadiation="9999";
  m_diffuseHorizontalRadiation="9999",
  m_globalHorizontalIlluminance="999999";
  m_directNormalIlluminance="999999";
  m_diffuseHorizontalIlluminance="999999";
  m_zenithLuminance="9999";
  m_windDirection="999";
  m_windSpeed="999";
  m_totalSkyCover=99;
  m_opaqueSkyCover=99,
  m_visibility="9999";
  m_ceilingHeight="99999";
  m_presentWeatherObservation=0;
  m_presentWeatherCodes=0;
  m_precipitableWater="999";
  m_aerosolOpticalDepth=".999";
  m_snowDepth="999";
  m_daysSinceLastSnowfall="99",
  m_albedo="999";
  m_liquidPrecipitationDepth="999";
  m_liquidPrecipitationQuantity="99";
}

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

boost::optional<EpwDataPoint> EpwDataPoint::fromEpwString(const std::string &line)
{
  std::vector<std::string> list;
  boost::split(list, line, boost::is_any_of(","));
  return fromEpwStrings(list);
}

boost::optional<EpwDataPoint> EpwDataPoint::fromEpwStrings(const std::vector<std::string> &list, bool pedantic)
{
  EpwDataPoint pt;
  // Expect 35 items in the list
  if (list.size() < 35) {
    if (pedantic) {
      LOG_FREE(Error, "openstudio.EpwFile", "Expected 35 fields in EPW data, got " << list.size());
      return boost::none;
    } else {
      LOG_FREE(Warn, "openstudio.EpwFile", "Expected 35 fields in EPW data, got " << list.size() << ", remaining fields will not be available");
    }
  }
  else if (list.size() > 35) {
    LOG_FREE(Warn, "openstudio.EpwFile", "Expected 35 fields in EPW data, got " << list.size() << ", additional data will be ignored");
  }
  // Use the appropriate setter on each field
  if (!pt.setYear(list[EpwDataField::Year])) {
    return boost::none;
  }
  if (!pt.setMonth(list[EpwDataField::Month])) {
    return boost::none;
  }
  if (!pt.setDay(list[EpwDataField::Day])) {
    return boost::none;
  }
  if (!pt.setHour(list[EpwDataField::Hour])) {
    return boost::none;
  }
  if(!pt.setMinute(list[EpwDataField::Minute])) {
    return boost::none;
  }
  pt.setDataSourceandUncertaintyFlags(list[EpwDataField::DataSourceandUncertaintyFlags]);
  pt.setDryBulbTemperature(list[EpwDataField::DryBulbTemperature]);
  pt.setDewPointTemperature(list[EpwDataField::DewPointTemperature]);
  pt.setRelativeHumidity(list[EpwDataField::RelativeHumidity]);
  pt.setAtmosphericStationPressure(list[EpwDataField::AtmosphericStationPressure]);
  pt.setExtraterrestrialHorizontalRadiation(list[EpwDataField::ExtraterrestrialHorizontalRadiation]);
  pt.setExtraterrestrialDirectNormalRadiation(list[EpwDataField::ExtraterrestrialDirectNormalRadiation]);
  pt.setHorizontalInfraredRadiationIntensity(list[EpwDataField::HorizontalInfraredRadiationIntensity]);
  pt.setGlobalHorizontalRadiation(list[EpwDataField::GlobalHorizontalRadiation]);
  pt.setDirectNormalRadiation(list[EpwDataField::DirectNormalRadiation]);
  pt.setDiffuseHorizontalRadiation(list[EpwDataField::DiffuseHorizontalRadiation]);
  pt.setGlobalHorizontalIlluminance(list[EpwDataField::GlobalHorizontalIlluminance]);
  pt.setDirectNormalIlluminance(list[EpwDataField::DirectNormalIlluminance]);
  pt.setDiffuseHorizontalIlluminance(list[EpwDataField::DiffuseHorizontalIlluminance]);
  pt.setZenithLuminance(list[EpwDataField::ZenithLuminance]);
  pt.setWindDirection(list[EpwDataField::WindDirection]);
  pt.setWindSpeed(list[EpwDataField::WindSpeed]);
  pt.setTotalSkyCover(list[EpwDataField::TotalSkyCover]);
  pt.setOpaqueSkyCover(list[EpwDataField::OpaqueSkyCover]);
  pt.setVisibility(list[EpwDataField::Visibility]);
  pt.setCeilingHeight(list[EpwDataField::CeilingHeight]);
  pt.setPresentWeatherObservation(list[EpwDataField::PresentWeatherObservation]);
  pt.setPresentWeatherCodes(list[EpwDataField::PresentWeatherCodes]);
  pt.setPrecipitableWater(list[EpwDataField::PrecipitableWater]);
  pt.setAerosolOpticalDepth(list[EpwDataField::AerosolOpticalDepth]);
  pt.setSnowDepth(list[EpwDataField::SnowDepth]);
  pt.setDaysSinceLastSnowfall(list[EpwDataField::DaysSinceLastSnowfall]);
  pt.setAlbedo(list[EpwDataField::Albedo]);
  pt.setLiquidPrecipitationDepth(list[EpwDataField::LiquidPrecipitationDepth]);
  pt.setLiquidPrecipitationQuantity(list[EpwDataField::LiquidPrecipitationQuantity]);
  return boost::optional<EpwDataPoint>(pt);
}

std::vector<std::string> EpwDataPoint::toEpwStrings() const
{
  std::vector<std::string> list;
  list.reserve(35);
  list.push_back(std::to_string(m_year));
  list.push_back(std::to_string(m_month));
  list.push_back(std::to_string(m_day));
  list.push_back(std::to_string(m_hour));
  list.push_back(std::to_string(m_minute));
  list.push_back(m_dataSourceandUncertaintyFlags);
  list.push_back(m_dryBulbTemperature);
  list.push_back(m_dewPointTemperature);
  list.push_back(m_relativeHumidity);
  list.push_back(m_atmosphericStationPressure);
  list.push_back(m_extraterrestrialHorizontalRadiation);
  list.push_back(m_extraterrestrialDirectNormalRadiation);
  list.push_back(m_horizontalInfraredRadiationIntensity);
  list.push_back(m_globalHorizontalRadiation);
  list.push_back(m_directNormalRadiation);
  list.push_back(m_diffuseHorizontalRadiation);
  list.push_back(m_globalHorizontalIlluminance);
  list.push_back(m_directNormalIlluminance);
  list.push_back(m_diffuseHorizontalIlluminance);
  list.push_back(m_zenithLuminance);
  list.push_back(m_windDirection);
  list.push_back(m_windSpeed);
  list.push_back(std::to_string(m_totalSkyCover));
  list.push_back(std::to_string(m_opaqueSkyCover));
  list.push_back(m_visibility);
  list.push_back(m_ceilingHeight);
  list.push_back(std::to_string(m_presentWeatherObservation));
  list.push_back(std::to_string(m_presentWeatherCodes));
  list.push_back(m_precipitableWater);
  list.push_back(m_aerosolOpticalDepth);
  list.push_back(m_snowDepth);
  list.push_back(m_daysSinceLastSnowfall);
  list.push_back(m_albedo);
  list.push_back(m_liquidPrecipitationDepth);
  list.push_back(m_liquidPrecipitationQuantity);
  return list;
}

boost::optional<std::string> EpwDataPoint::unitsByName(const std::string &name)
{
  EpwDataField id;
  try {
    id = EpwDataField(name);
  } catch(...) {
    // Could do a warning message here
    return boost::optional<std::string>();
  }
  return boost::optional<std::string>(units(id));
}

std::string EpwDataPoint::units(EpwDataField field)
{
  std::string string;
  switch(field.value())
  {
  case EpwDataField::Year:
    //string = "None";
    break;
  case EpwDataField::Month:
    //string = "None";
    break;
  case EpwDataField::Day:
    //string = "None";
    break;
  case EpwDataField::Hour:
    //string = "None";
    break;
  case EpwDataField::Minute:
    //string = "None";
    break;
  case EpwDataField::DataSourceandUncertaintyFlags:
    //string = "None";
    break;
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
  default:
    //string = "None";
    break;
  }
  return string;
}

boost::optional<double> EpwDataPoint::fieldByName(const std::string &name)
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

boost::optional<std::string> EpwDataPoint::toWthString() const
{
  QStringList output;
  std::string date = QString("%1/%2").arg(m_month).arg(m_day).toStdString();
  std::string string = date;
  QString qhms = QString().sprintf("%02d:%02d:00", m_hour, m_minute);
  std::string hms = qhms.toStdString();
  string += '\t' + hms;
  boost::optional<double> value = dryBulbTemperature();
  if(!value) {
    LOG_FREE(Error,"openstudio.EpwFile","Missing dry bulb temperature on " << date << " at " << hms)
    return boost::none;
  }
  boost::optional<double> optdrybulb = openstudio::convert(value.get(), "C", "K");
  OS_ASSERT(optdrybulb);
  double drybulb = optdrybulb.get();
  string += '\t' + std::to_string(drybulb);
  value = atmosphericStationPressure();
  if(!value) {
    LOG_FREE(Error,"openstudio.EpwFile", "Missing atmospheric station pressure on " << date << " at " << hms);
    return boost::none;
  }
  double p = value.get();
  string += '\t' + m_atmosphericStationPressure;
  if(!windSpeed()) {
    LOG_FREE(Error,"openstudio.EpwFile", "Missing wind speed on " << date << " at " << hms);
    return boost::none;
  }
  string += '\t' + m_windSpeed;
  if(!windDirection()) {
    LOG_FREE(Error,"openstudio.EpwFile", "Missing wind direction on " << date << " at " << hms);
    return boost::none;
  }
  string += '\t' + m_windDirection;
  double pw;
  value = relativeHumidity();
  if(!value) { // Don't have relative humidity - this has not been tested
    value = dewPointTemperature();
    if(!value) {
      LOG_FREE(Error,"openstudio.EpwFile", "Cannot compute humidity ratio on " << date << " at " << hms);
      return boost::none;
    }
    boost::optional<double> optdewpoint = openstudio::convert(value.get(), "C", "K");
    OS_ASSERT(optdewpoint);
    double dewpoint = optdewpoint.get();
    pw = psat(dewpoint);
  } else  { // Have relative humidity
    double pws = psat(drybulb);
    pw = 0.01*value.get()*pws;
  }
  double W = 0.621945*pw/(p-pw);
  string += "\t" + std::to_string(W*1000); // need g/kg
  // Pass on solar flux quantities
  string +=  "\t0\t0";
  // Pass on Tsky
  string += "\t0";
  // Pass on snow and rain
  string += "\t0\t0";
  return boost::optional<std::string>(string);
}

// Local convenience functions
static int stringToInteger(const std::string &string, bool *ok)
{
  int value = 0;
  *ok = true;
  try {
    value = std::stoi(string);
  }
  catch (const std::invalid_argument) {
    *ok = false;
  }
  catch (const std::out_of_range) {
    *ok = false;
  }
  return value;
}

static double stringToDouble(const std::string &string, bool *ok)
{
  double value = 0;
  *ok = true;
  try {
    value = std::stod(string);
  }
  catch (const std::invalid_argument) {
    *ok = false;
  }
  catch (const std::out_of_range) {
    *ok = false;
  }
  return value;
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

bool EpwDataPoint::setYear(const std::string &year)
{
  bool ok;
  int value = stringToInteger(year, &ok);
  if(!ok) {
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
  if(1 > month || 12 < month) {
    LOG_FREE(Error,"openstudio.EpwFile","Month value " << month << " out of range");
    return false;
  }
  m_month = month;
  return true;
}

bool EpwDataPoint::setMonth(const std::string &month)
{
  bool ok;
  int value = stringToInteger(month, &ok);
  if(!ok) {
    LOG_FREE(Error,"openstudio.EpwFile","Month value '" << month << "' cannot be converted into an integer");
    return false;
  }
  return setMonth(value);
}

int EpwDataPoint::day() const
{
  return m_day;
}

bool EpwDataPoint::setDay(int day)
{
  if(1 > day || 31 < day) {
    LOG_FREE(Error,"openstudio.EpwFile","Day value " << day << " out of range");
    return false;
  }
  m_day = day;
  return true;
}

bool EpwDataPoint::setDay(const std::string &day)
{
  bool ok;
  int value = stringToInteger(day, &ok);
  if(!ok) {
    LOG_FREE(Error,"openstudio.EpwFile","Day value '" << day << "' cannot be converted into an integer");
    return false;
  } 
  return setDay(value);
}

int EpwDataPoint::hour() const
{
  return m_hour;
}

bool EpwDataPoint::setHour(int hour)
{
  if(1 > hour || 24 < hour) {
    LOG_FREE(Error,"openstudio.EpwFile","Hour value " << hour << " out of range");
    return false;
  }
  m_hour = hour;
  return true;
}

bool EpwDataPoint::setHour(const std::string &hour)
{
  bool ok;
  int value = stringToInteger(hour, &ok);
  if(!ok) {
    LOG_FREE(Error,"openstudio.EpwFile","Hour value '" << hour << "' cannot be converted into an integer");
    return false;
  }
  return setHour(value);
}

int EpwDataPoint::minute() const
{
  return m_minute;
}

bool EpwDataPoint::setMinute(int minute)
{
  if(0 > minute || 59 < minute) {
    LOG_FREE(Error,"openstudio.EpwFile","Minute value " << minute << " out of range");
    return false;
  }
  m_minute = minute;
  return true;
}

bool EpwDataPoint::setMinute(const std::string &minute)
{
  bool ok;
  int value = stringToInteger(minute, &ok);
  if(!ok) {
    LOG_FREE(Error,"openstudio.EpwFile","Minute value '" << minute << "' cannot be converted into an integer");
    return false;
  }
  return setMinute(value);
}

std::string EpwDataPoint::dataSourceandUncertaintyFlags() const
{
  return m_dataSourceandUncertaintyFlags;
}

void EpwDataPoint::setDataSourceandUncertaintyFlags(const std::string &dataSourceandUncertaintyFlags)
{
  m_dataSourceandUncertaintyFlags = dataSourceandUncertaintyFlags;
}

boost::optional<double> EpwDataPoint::dryBulbTemperature() const
{
  if(m_dryBulbTemperature == "99.9") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_dryBulbTemperature));
}

bool EpwDataPoint::setDryBulbTemperature(double dryBulbTemperature)
{
  if(-70 >= dryBulbTemperature || 70 <= dryBulbTemperature) {
    m_dryBulbTemperature = "99.9";
    return false;
  }
  m_dryBulbTemperature = std::to_string(dryBulbTemperature);
  return true;
}

bool EpwDataPoint::setDryBulbTemperature(const std::string &dryBulbTemperature)
{
  bool ok;
  double value = stringToDouble(dryBulbTemperature, &ok);
  if(!ok || -70 >= value || 70 <= value) {
    m_dryBulbTemperature = "99.9";
    return false;
  }
  m_dryBulbTemperature = dryBulbTemperature;
  return true;
}

boost::optional<double> EpwDataPoint::dewPointTemperature() const
{
  if(m_dewPointTemperature == "99.9") {
    return boost::optional<double>();
  }
  return boost::optional<double>(std::stod(m_dewPointTemperature));
}

bool EpwDataPoint::setDewPointTemperature(double dewPointTemperature)
{
  if(-70 >= dewPointTemperature || 70 <= dewPointTemperature) {
    m_dewPointTemperature = "99.9";
    return false;
  }
  m_dewPointTemperature = std::to_string(dewPointTemperature);
  return true;
}

bool EpwDataPoint::setDewPointTemperature(const std::string &dewPointTemperature)
{
  bool ok;
  double value = stringToDouble(dewPointTemperature, &ok);
  if(!ok || -70 >= value || 70 <= value) {
    m_dewPointTemperature = "99.9";
    return false;
  }
  m_dewPointTemperature = dewPointTemperature;
  return true;
}

boost::optional<double> EpwDataPoint::relativeHumidity() const
{
  if(m_relativeHumidity == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_relativeHumidity));
}

bool EpwDataPoint::setRelativeHumidity(double relativeHumidity)
{
  if(0 > relativeHumidity || 110 < relativeHumidity) {
    m_relativeHumidity = "999";
    return false;
  }
  m_relativeHumidity = std::to_string(relativeHumidity);
  return true;
}

bool EpwDataPoint::setRelativeHumidity(const std::string &relativeHumidity)
{
  bool ok;
  double value = stringToDouble(relativeHumidity, &ok);
  if(!ok || 0 > value || 110 < value) {
    m_relativeHumidity = "999";
    return false;
  }
  m_relativeHumidity = relativeHumidity;
  return true;
}

boost::optional<double> EpwDataPoint::atmosphericStationPressure() const
{
  if(m_atmosphericStationPressure == "999999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_atmosphericStationPressure));
}

bool EpwDataPoint::setAtmosphericStationPressure(double atmosphericStationPressure)
{
  if(31000 >= atmosphericStationPressure || 120000 <= atmosphericStationPressure) {
    m_atmosphericStationPressure = "999999";
    return false;
  }
  m_atmosphericStationPressure = std::to_string(atmosphericStationPressure);
  return true;
}

bool EpwDataPoint::setAtmosphericStationPressure(const std::string &atmosphericStationPressure)
{
  bool ok;
  double value = stringToDouble(atmosphericStationPressure, &ok);
  if(!ok || 31000 >= value || 120000 <= value) {
    m_atmosphericStationPressure = "999999";
    return false;
  }
  m_atmosphericStationPressure = atmosphericStationPressure;
  return true;
}

boost::optional<double> EpwDataPoint::extraterrestrialHorizontalRadiation() const
{
  if(m_extraterrestrialHorizontalRadiation == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_extraterrestrialHorizontalRadiation));
}

bool EpwDataPoint::setExtraterrestrialHorizontalRadiation(double extraterrestrialHorizontalRadiation)
{
  if(0 > extraterrestrialHorizontalRadiation) {
    m_extraterrestrialHorizontalRadiation = "9999";
    return false;
  }
  m_extraterrestrialHorizontalRadiation = std::to_string(extraterrestrialHorizontalRadiation);
  return true;
}

bool EpwDataPoint::setExtraterrestrialHorizontalRadiation(const std::string &extraterrestrialHorizontalRadiation)
{
  bool ok;
  double value = stringToDouble(extraterrestrialHorizontalRadiation, &ok);
  if(!ok || 0 > value) {
    m_extraterrestrialHorizontalRadiation = "9999";
    return false;
  }
  m_extraterrestrialHorizontalRadiation = extraterrestrialHorizontalRadiation;
  return true;
}

boost::optional<double> EpwDataPoint::extraterrestrialDirectNormalRadiation() const
{
  if(m_extraterrestrialDirectNormalRadiation == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_extraterrestrialDirectNormalRadiation));
}

bool EpwDataPoint::setExtraterrestrialDirectNormalRadiation(double extraterrestrialDirectNormalRadiation)
{
  if(0 > extraterrestrialDirectNormalRadiation) {
    m_extraterrestrialDirectNormalRadiation = "9999";
    return false;
  }
  m_extraterrestrialDirectNormalRadiation = std::to_string(extraterrestrialDirectNormalRadiation);
  return true;
}

bool EpwDataPoint::setExtraterrestrialDirectNormalRadiation(const std::string &extraterrestrialDirectNormalRadiation)
{
  bool ok;
  double value = stringToDouble(extraterrestrialDirectNormalRadiation, &ok);
  if(!ok || 0 > value) {
    m_extraterrestrialDirectNormalRadiation = "9999";
    return false;
  }
  m_extraterrestrialDirectNormalRadiation = extraterrestrialDirectNormalRadiation;
  return true;
}

boost::optional<double> EpwDataPoint::horizontalInfraredRadiationIntensity() const
{
  if(m_horizontalInfraredRadiationIntensity == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_horizontalInfraredRadiationIntensity));
}

bool EpwDataPoint::setHorizontalInfraredRadiationIntensity(double horizontalInfraredRadiationIntensity)
{
  if(0 > horizontalInfraredRadiationIntensity) {
    m_horizontalInfraredRadiationIntensity = "9999";
    return false;
  }
  m_horizontalInfraredRadiationIntensity = std::to_string(horizontalInfraredRadiationIntensity);
  return true;
}

bool EpwDataPoint::setHorizontalInfraredRadiationIntensity(const std::string &horizontalInfraredRadiationIntensity)
{
  bool ok;
  double value = stringToDouble(horizontalInfraredRadiationIntensity, &ok);
  if(!ok || 0 > value) {
    m_horizontalInfraredRadiationIntensity = "9999";
    return false;
  }
  m_horizontalInfraredRadiationIntensity = horizontalInfraredRadiationIntensity;
  return true;
}

boost::optional<double> EpwDataPoint::globalHorizontalRadiation() const
{
  if(m_globalHorizontalRadiation == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_globalHorizontalRadiation));
}

bool EpwDataPoint::setGlobalHorizontalRadiation(double globalHorizontalRadiation)
{
  if(0 > globalHorizontalRadiation)
  {
    m_globalHorizontalRadiation = "9999";
    return false;
  }
  m_globalHorizontalRadiation = std::to_string(globalHorizontalRadiation);
  return true;
}

bool EpwDataPoint::setGlobalHorizontalRadiation(const std::string &globalHorizontalRadiation)
{
  bool ok;
  double value = QString().fromStdString(globalHorizontalRadiation).toDouble(&ok);
  if(!ok) {
    m_globalHorizontalRadiation = "9999";
    return false;
  }
  return setGlobalHorizontalRadiation(value);
}

boost::optional<double> EpwDataPoint::directNormalRadiation() const
{
  if(m_directNormalRadiation == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_directNormalRadiation));
}

bool EpwDataPoint::setDirectNormalRadiation(double directNormalRadiation)
{
  if(0 > directNormalRadiation) {
    m_directNormalRadiation = "9999";
    return false;
  }
  m_directNormalRadiation = std::to_string(directNormalRadiation);
  return true;
}

bool EpwDataPoint::setDirectNormalRadiation(const std::string &directNormalRadiation)
{
  bool ok;
  double value = stringToDouble(directNormalRadiation, &ok);
  if(!ok || 0 > value) {
    m_directNormalRadiation = "9999";
    return false;
  }
  m_directNormalRadiation = directNormalRadiation;
  return true;
}

boost::optional<double> EpwDataPoint::diffuseHorizontalRadiation() const
{
  if(m_diffuseHorizontalRadiation == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_diffuseHorizontalRadiation));
}

bool EpwDataPoint::setDiffuseHorizontalRadiation(double diffuseHorizontalRadiation)
{
  if(0 > diffuseHorizontalRadiation) {
    m_diffuseHorizontalRadiation = "9999";
    return false;
  }
  m_diffuseHorizontalRadiation = std::to_string(diffuseHorizontalRadiation);
  return true;
}

bool EpwDataPoint::setDiffuseHorizontalRadiation(const std::string &diffuseHorizontalRadiation)
{
  bool ok;
  double value = stringToDouble(diffuseHorizontalRadiation, &ok);
  if(!ok) {
    m_diffuseHorizontalRadiation = "9999";
    return false;
  }
  m_diffuseHorizontalRadiation = diffuseHorizontalRadiation;
  return true;
}

boost::optional<double> EpwDataPoint::globalHorizontalIlluminance() const
{
  if(m_globalHorizontalIlluminance == "999999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_globalHorizontalIlluminance));
}

bool EpwDataPoint::setGlobalHorizontalIlluminance(double globalHorizontalIlluminance)
{
  if(0 > globalHorizontalIlluminance || 999900 < globalHorizontalIlluminance) {
    m_globalHorizontalIlluminance = "999999";
    return false;
  }
  m_globalHorizontalIlluminance = std::to_string(globalHorizontalIlluminance);
  return true;
}

bool EpwDataPoint::setGlobalHorizontalIlluminance(const std::string &globalHorizontalIlluminance)
{
  bool ok;
  double value = stringToDouble(globalHorizontalIlluminance, &ok);
  if(!ok || 0 > value || 999900 < value) {
    m_globalHorizontalIlluminance = "999999";
    return false;
  }
  m_globalHorizontalIlluminance = globalHorizontalIlluminance;
  return true;
}

boost::optional<double> EpwDataPoint::directNormalIlluminance() const
{
  if(m_directNormalIlluminance == "999999") {
    return boost::optional<double>();
  }
  return boost::optional<double>(std::stod(m_directNormalIlluminance));
}

bool EpwDataPoint::setDirectNormalIlluminance(double directNormalIlluminance)
{
  if(0 > directNormalIlluminance || 999900 < directNormalIlluminance) {
    m_directNormalIlluminance = "999999";
    return false;
  }
  m_directNormalIlluminance = std::to_string(directNormalIlluminance);
  return true;
}

bool EpwDataPoint::setDirectNormalIlluminance(const std::string &directNormalIlluminance)
{
  bool ok;
  double value = stringToDouble(directNormalIlluminance, &ok);
  if(!ok || 0 > value || 999900 < value) {
    m_directNormalIlluminance = "999999";
    return false;
  }
  m_directNormalIlluminance = directNormalIlluminance;
  return true;
}

boost::optional<double> EpwDataPoint::diffuseHorizontalIlluminance() const
{
  if(m_diffuseHorizontalIlluminance == "999999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_diffuseHorizontalIlluminance));
}

bool EpwDataPoint::setDiffuseHorizontalIlluminance(double diffuseHorizontalIlluminance)
{
  if(0 > diffuseHorizontalIlluminance || 999900 < diffuseHorizontalIlluminance) {
    m_diffuseHorizontalIlluminance = "999999";
    return false;
  }
  m_diffuseHorizontalIlluminance = std::to_string(diffuseHorizontalIlluminance);
  return true;
}

bool EpwDataPoint::setDiffuseHorizontalIlluminance(const std::string &diffuseHorizontalIlluminance)
{
  bool ok;
  double value = stringToDouble(diffuseHorizontalIlluminance, &ok);
  if(!ok || 0 > value || 999900 < value) {
    m_diffuseHorizontalIlluminance = "999999";
    return false;
  }
  m_diffuseHorizontalIlluminance = diffuseHorizontalIlluminance;
  return true;
}

boost::optional<double> EpwDataPoint::zenithLuminance() const
{
  if(m_zenithLuminance == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_zenithLuminance));
}

bool EpwDataPoint::setZenithLuminance(double zenithLuminance)
{
  if(0 > zenithLuminance || 9999 < zenithLuminance) {
    m_zenithLuminance = "9999";
    return false;
  }
  m_zenithLuminance = std::to_string(zenithLuminance);
  return true;
}

bool EpwDataPoint::setZenithLuminance(const std::string &zenithLuminance)
{
  bool ok;
  double value = stringToDouble(zenithLuminance, &ok);
  if(!ok || 0 > value || 9999 < value) {
    m_zenithLuminance = "9999";
    return false;
  }
  m_zenithLuminance = zenithLuminance;
  return true;
}

boost::optional<double> EpwDataPoint::windDirection() const
{
  if(m_windDirection == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_windDirection));
}

bool EpwDataPoint::setWindDirection(double windDirection)
{
  if(0 > windDirection || 360 < windDirection) {
    m_windDirection = "999";
    return false;
  }
  m_windDirection = std::to_string(windDirection);
  return true;
}

bool EpwDataPoint::setWindDirection(const std::string &windDirection)
{
  bool ok;
  double value = stringToDouble(windDirection, &ok);
  if(!ok || 0 > value || 360 < value) {
    m_windDirection = "999";
    return false;
  }
  m_windDirection = windDirection;
  return true;
}

boost::optional<double> EpwDataPoint::windSpeed() const
{
  if(m_windSpeed == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_windSpeed));
}

bool EpwDataPoint::setWindSpeed(double windSpeed)
{
  if(0 > windSpeed || 40 < windSpeed) {
    m_windSpeed = "999";
    return false;
  }
  m_windSpeed = std::to_string(windSpeed);
  return true;
}

bool EpwDataPoint::setWindSpeed(const std::string &windSpeed)
{
  bool ok;
  double value = stringToDouble(windSpeed, &ok);
  if(!ok || 0 > value || 40 < value) {
    m_windSpeed = "999";
    return false;
  }
  return setWindSpeed(value);
}

int EpwDataPoint::totalSkyCover() const
{
  return m_totalSkyCover;
}

bool EpwDataPoint::setTotalSkyCover(int totalSkyCover)
{
  if(0 > totalSkyCover || 10 < totalSkyCover) {
    m_totalSkyCover = 99;
    return false;
  }
  m_totalSkyCover = totalSkyCover;
  return true;
}

bool EpwDataPoint::setTotalSkyCover(const std::string &totalSkyCover)
{
  bool ok;
  int value = stringToInteger(totalSkyCover, &ok);
  if(!ok || 0 > value || 10 < value) {
    m_totalSkyCover = 99;
    return false;
  }
  return setTotalSkyCover(value);
}

int EpwDataPoint::opaqueSkyCover() const
{
  return m_opaqueSkyCover;
}

bool EpwDataPoint::setOpaqueSkyCover(int opaqueSkyCover)
{
  if(0 > opaqueSkyCover || 10 < opaqueSkyCover) {
    m_opaqueSkyCover = 99;
    return false;
  }
  m_opaqueSkyCover = opaqueSkyCover;
  return true;
}

bool EpwDataPoint::setOpaqueSkyCover(const std::string &opaqueSkyCover)
{
  bool ok;
  int value = stringToInteger(opaqueSkyCover, &ok);
  if(!ok || 0 > value || 10 < value) {
    m_opaqueSkyCover = 99;
    return false;
  }
  return setOpaqueSkyCover(value);
}

boost::optional<double> EpwDataPoint::visibility() const
{
  if(m_visibility == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_visibility));
}

void EpwDataPoint::setVisibility(double visibility)
{
  m_visibility = std::to_string(visibility);
}

bool EpwDataPoint::setVisibility(const std::string &visibility)
{
  bool ok;
  stringToDouble(visibility, &ok);
  if(!ok) {
    m_visibility = "9999";
    return false;
  }
  m_visibility = visibility;
  return true;
}

boost::optional<double> EpwDataPoint::ceilingHeight() const
{
  if(m_ceilingHeight == "99999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_ceilingHeight));
}

void EpwDataPoint::setCeilingHeight(double ceilingHeight)
{
  m_ceilingHeight = std::to_string(ceilingHeight);
}

bool EpwDataPoint::setCeilingHeight(const std::string &ceilingHeight)
{
  bool ok;
  stringToDouble(ceilingHeight, &ok);
  if(!ok) {
    m_ceilingHeight = "99999";
    return false;
  }
  m_ceilingHeight = ceilingHeight;
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

bool EpwDataPoint::setPresentWeatherObservation(const std::string &presentWeatherObservation)
{
  bool ok;
  int value = stringToInteger(presentWeatherObservation, &ok);
  if(!ok) {
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

bool EpwDataPoint::setPresentWeatherCodes(const std::string &presentWeatherCodes)
{
  bool ok;
  int value = QString().fromStdString(presentWeatherCodes).toInt(&ok);
  if(!ok) {
    return false;
  }
  m_presentWeatherCodes = value;
  return true;
}

boost::optional<double> EpwDataPoint::precipitableWater() const
{
  if(m_precipitableWater == "999") {
    return boost::optional<double>();
  }
  return boost::optional<double>(std::stod(m_precipitableWater));
}

void EpwDataPoint::setPrecipitableWater(double precipitableWater)
{
  m_precipitableWater = std::to_string(precipitableWater);
}

bool EpwDataPoint::setPrecipitableWater(const std::string &precipitableWater)
{
  bool ok;
  stringToDouble(precipitableWater, &ok);
  if(!ok) {
    m_precipitableWater = "999";
    return false;
  }
  m_precipitableWater = precipitableWater;
  return true;
}

boost::optional<double> EpwDataPoint::aerosolOpticalDepth() const
{
  if(m_aerosolOpticalDepth == ".999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_aerosolOpticalDepth));
}

void EpwDataPoint::setAerosolOpticalDepth(double aerosolOpticalDepth)
{
  m_aerosolOpticalDepth = std::to_string(aerosolOpticalDepth);
}

bool EpwDataPoint::setAerosolOpticalDepth(const std::string &aerosolOpticalDepth)
{
  bool ok;
  stringToDouble(aerosolOpticalDepth, &ok);
  if(!ok) {
    m_aerosolOpticalDepth = ".999";
    return false;
  }
  m_aerosolOpticalDepth = aerosolOpticalDepth;
  return true;
}

boost::optional<double> EpwDataPoint::snowDepth() const
{
  if(m_snowDepth == "999") {
    return boost::optional<double>();
  }
  return boost::optional<double>(std::stod(m_snowDepth));
}

void EpwDataPoint::setSnowDepth(double snowDepth)
{
  m_snowDepth = std::to_string(snowDepth);
}

bool EpwDataPoint::setSnowDepth(const std::string &snowDepth)
{
  bool ok;
  stringToDouble(snowDepth, &ok);
  if(!ok) {
    m_snowDepth = "999";
    return false;
  }
  m_snowDepth = snowDepth;
  return true;
}

boost::optional<double> EpwDataPoint::daysSinceLastSnowfall() const
{
  if(m_daysSinceLastSnowfall == "99") {
    return boost::optional<double>();
  }
  return boost::optional<double>(std::stod(m_daysSinceLastSnowfall));
}

void EpwDataPoint::setDaysSinceLastSnowfall(double daysSinceLastSnowfall)
{
  m_daysSinceLastSnowfall = std::to_string(daysSinceLastSnowfall);
}

bool EpwDataPoint::setDaysSinceLastSnowfall(const std::string &daysSinceLastSnowfall)
{
  bool ok;
  stringToDouble(daysSinceLastSnowfall, &ok);
  if(!ok) {
    m_daysSinceLastSnowfall = "99";
    return false;
  }
  m_daysSinceLastSnowfall = daysSinceLastSnowfall;
  return true;
}

boost::optional<double> EpwDataPoint::albedo() const
{
  if(m_albedo == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_albedo));
}

void EpwDataPoint::setAlbedo(double albedo)
{
  m_albedo = std::to_string(albedo);
}

bool EpwDataPoint::setAlbedo(const std::string &albedo)
{
  bool ok;
  stringToDouble(albedo, &ok);
  if(!ok) {
    m_albedo = "999";
    return false;
  }
  m_albedo = albedo;
  return true;
}

boost::optional<double> EpwDataPoint::liquidPrecipitationDepth() const
{
  if(m_liquidPrecipitationDepth == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_liquidPrecipitationDepth));
}

void EpwDataPoint::setLiquidPrecipitationDepth(double liquidPrecipitationDepth)
{
  m_liquidPrecipitationDepth = std::to_string(liquidPrecipitationDepth);
}

bool EpwDataPoint::setLiquidPrecipitationDepth(const std::string &liquidPrecipitationDepth)
{
  bool ok;
  stringToDouble(liquidPrecipitationDepth, &ok);
  if(!ok) {
    m_liquidPrecipitationDepth = "999";
    return false;
  }
  m_liquidPrecipitationDepth = liquidPrecipitationDepth;
  return true;
}

boost::optional<double> EpwDataPoint::liquidPrecipitationQuantity() const
{
  if(m_liquidPrecipitationQuantity == "99") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_liquidPrecipitationQuantity));
}

void EpwDataPoint::setLiquidPrecipitationQuantity(double liquidPrecipitationQuantity)
{
  m_liquidPrecipitationQuantity = std::to_string(liquidPrecipitationQuantity);
}

bool EpwDataPoint::setLiquidPrecipitationQuantity(const std::string &liquidPrecipitationQuantity)
{
  bool ok;
  stringToDouble(liquidPrecipitationQuantity, &ok);
  if(!ok) {
    m_liquidPrecipitationQuantity = "99";
    return false;
  }
  m_liquidPrecipitationQuantity = liquidPrecipitationQuantity;
  return true;
}

EpwFile::EpwFile(const openstudio::path& p, bool storeData)
  : m_path(p), m_latitude(0), m_longitude(0), m_timeZone(0), m_elevation(0)
{
  if (!parse(storeData)){
    LOG_AND_THROW("EpwFile '" << toString(p) << "' cannot be processed");
  }
}

boost::optional<EpwFile> EpwFile::load(const openstudio::path& p, bool storeData)
{
  boost::optional<EpwFile> result;
  try{
    result = EpwFile(p, storeData);
  }catch(const std::exception&){
  }
  return result;
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
  OS_ASSERT((60 % m_recordsPerHour) == 0);
  return Time(0,0,60/m_recordsPerHour);
}

int EpwFile::recordsPerHour() const
{
  return m_recordsPerHour;
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

boost::optional<TimeSeries> EpwFile::getTimeSeries(const std::string &name)
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
//  firstPt.setDateTime(dateTime);

  stream <<"!Date\tTime\tTa [K]\tPb [Pa]\tWs [m/s]\tWd [deg]\tHr [g/kg]\tIth [kJ/m^2]\tIdn [kJ/m^2]\tTs [K]\tRn [-]\tSn [-]\n";
  boost::optional<std::string> output = firstPt.toWthString();
  if(!output) {
    LOG(Error, "Translation to WTH has failed");
    fp.close();
    return false;
  }
  stream << output.get() << '\n';
  for(unsigned int i=0;i<data().size();i++) {
    output = data()[i].toWthString();
    if(!output) {
      LOG(Error, "Translation to WTH has failed");
      fp.close();
      return false;
    }
    stream << output.get() << '\n';
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
  int lineNumber = 8;
  boost::optional<Date> startDate;
  boost::optional<Date> lastDate;
  boost::optional<Date> endDate;
  bool realYear = true;
  bool wrapAround = false;
  OS_ASSERT((60 % m_recordsPerHour) == 0);
  int minutesPerRecord = 60/m_recordsPerHour;
  int currentMinute = 0;
  bool warnedAboutMinutesAlready = false;
  while(std::getline(ifs, line)){
    lineNumber++;
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
        LOG(Error, "Could not read line " << lineNumber << " of EPW file '" << m_path << "'");
        ifs.close();
        return false;
      }
      if(storeData) {
        std::vector<std::string> list;
        boost::split(list, line, boost::is_any_of(","));
        // Due to issues with some EPW files, we need to check stuff here
        if (list.size() < 5) { // Not enough data, bail out
          LOG(Error, "Insufficient weather data on line " << lineNumber << " of EPW file '" << m_path << "'");
          ifs.close();
          return false;
        }
        int minutesInFile = std::stoi(list[4]);
        if(m_recordsPerHour!=1) {
          currentMinute += minutesPerRecord;
          if(currentMinute >= 60) { // This could really be ==, but >= is used for safety
            currentMinute = 0;
          }
        }
        // Check for agreement between the file value and the computed value
        if(currentMinute != minutesInFile) {
          if(!warnedAboutMinutesAlready) {
            LOG(Error, "Minutes field (" << minutesInFile << ") on line " << lineNumber << " of EPW file '" 
              << m_path << "' does not agree with computed value (" << currentMinute << "), using computed value");
          }
          // Override whatever is in the file
          list[3] = std::to_string(currentMinute);
        }
        boost::optional<EpwDataPoint> pt = EpwDataPoint::fromEpwStrings(list);
        if(pt) {
          m_data.push_back(pt.get());
        } else {
          LOG(Error,"Failed to parse line " << lineNumber << " of EPW file '" << m_path << "'");
          ifs.close();
          return false;
        }
      }
    }else{
      LOG(Error, "Could not read line " << lineNumber << " of EPW file '" << m_path << "'");
      ifs.close();
      return false;
    }
  }

  // close file
  ifs.close();

  if (!startDate){
    LOG(Error, "Could not find start date in data section of EPW file '" << m_path << "'");
    return false;
  }
  if (!endDate){
    LOG(Error, "Could not find end date in data section of EPW file '" << m_path << "'");
    return false;
  }

  if ((m_startDate.monthOfYear() != startDate->monthOfYear()) ||
      (m_startDate.dayOfMonth() != startDate->dayOfMonth()) ||
      (m_endDate.monthOfYear() != endDate->monthOfYear()) ||
      (m_endDate.dayOfMonth() != endDate->dayOfMonth())){
    LOG(Error, "Header start and end dates do not match data in EPW file '" << m_path << "'");
    return false;
  }

  if (realYear){
    if (m_startDayOfWeek != startDate->dayOfWeek()){
      LOG(Warn, "Header start day of the week and actual start day of the week do not match in EPW file '" << m_path << "', data will be treated as typical");
      // The flag needs to be changed so we can do the wrapAround check below
      realYear = false;
    } else {
      // set dates with years
      m_startDate = startDate.get();
      m_startDateActualYear = startDate->year();

      m_endDate = endDate.get();
      m_endDateActualYear = endDate->year();
    }
  }

  if(!realYear && wrapAround) {
    LOG(Error, "Wrap around years not supported for TMY data, EPW file '" << m_path << "'");
    return false;
  }

  return result;
}

bool EpwFile::parseLocation(const std::string& line)
{
  bool result = true;

  // LOCATION,Chicago Ohare Intl Ap,IL,USA,TMY3,725300,41.98,-87.92,-6.0,201.0
  boost::regex locationRegex("^LOCATION,(.*?),(.*?),(.*?),(.*?),(.*?),(.*?),(.*?),(.*?),([^,]*).*?$");
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
  boost::regex dataPeriodRegex("^DATA PERIODS,(.*?),(.*?),(.*?),(.*?),(.*?),([^,]*).*?$");
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
      m_recordsPerHour = boost::lexical_cast<int>(timeStep);
      if((60 % m_recordsPerHour) != 0) {
        LOG(Error, "Number of records per hour of " << m_recordsPerHour << " does not result in integral number of minutes between records in EPW file '" << m_path<<"'");
        result = false;
      }
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
