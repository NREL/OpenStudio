/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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
#include "../core/StringHelpers.hpp"
#include "../core/Assert.hpp"
#include "../units/QuantityConverter.hpp"

#include <QStringList>
#include <QFile>
#include <QTextStream>

#include <cmath>
#include <string>
#include <fstream>

namespace openstudio{

static double psat(double T)
{
  // Compute water vapor saturation pressure, eqns 5 and 6 from ASHRAE Fundamentals 2009 Ch. 1
  // This version takes T in C rather than Kelvin since most of the other eqns use C
  T += 273.15;
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
  if(T<273.15) {
    rhs = C1/T + C2 + T*(C3 + T*(C4 + T*(C5 + T*C6))) + C7*std::log(T);
  } else {
    rhs = C8/T + C9 + T*(C10 + T*(C11 + T*C12)) + C13*std::log(T);
  }
  return exp(rhs);
}

static double psatp(double T, double psat)
{
  // Compute the derivative of the water vapor saturation pressure function (eqns 5 and 6 from
  // ASHRAE Fundamentals 2009 Ch. 1)
  // This version takes T in C rather than Kelvin since most of the other eqns use C
  T += 273.15;
  double C1 = -5.6745359e+03;
  double C3 = -9.6778430e-03;
  double C4 = 6.2215701e-07;
  double C5 = 2.0747825e-09;
  double C6 = -9.4840240e-13;
  double C7 = 4.1635019e+00;
  double C8 = -5.8002206e+03;
  double C10 = -4.8640239e-02;
  double C11 = 4.1764768e-05;
  double C12 = -1.4452093e-08;
  double C13 = 6.5459673e+00;
  double fp;
  double T2 = T*T;
  double T3 = T*T2;
  if (T<273.15) {
    fp = -C1 / T2 + C3 + 2 * T*C4 + 3 * T2*C5 + 4 * T3*C6 + C7 / T;
  }
  else {
    fp = -C8 / T2 + C10 + 2 * T*C11 + 3 * T2*C12 + C13 / T;
  }
  //std::cout << "psatp: " << T - 273.15 << " " << fp << " " << fp*psat << std::endl;
  return fp*psat;
}

static double enthalpy(double T, double p, double phi)
{
  // Compute moist air enthalpy, eqns 5, 6, 22, 24, and 32 from ASHRAE Fundamentals 2009 Ch. 1
  double pw = phi*psat(T); // Partial pressure of water vapor, eqn 24 (uses eqns 5 and 6)
  double W = 0.621945*pw / (p - pw); // Humidity ratio, eqn 22
  return 1.006*T + W*(2501 + 1.86*T); // Moist air specific enthalpy, eqn 32
}

static double enthalpyFromDewPoint(double T, double p, double Tdewpoint)
{
  // Compute moist air enthalpy, eqns 5, 6, 22, 32, and 38 from ASHRAE Fundamentals 2009 Ch. 1
  double pw = openstudio::psat(Tdewpoint); // Partial pressure of water vapor, eqn 38 (uses eqns 5 and 6)
  double W = 0.621945*pw / (p - pw); // Humidity ratio, eqn 22
  return 1.006*T + W*(2501 + 1.86*T); // Moist air specific enthalpy, eqn 32
}

// Using equation 35/37 in ASHRAE Fundamentals 2009 Ch. 1:
//
//  W = (A W*_s - B)/C
//
// where A, B, C, and W*_s are all function of wet bulb t*
// Solve for the root of
//
//  f = W C - A W*_s + B
//
// To make things even more opaque, lets use
//
//  A = a0 + a1 t*
//  B = b (t - t*)
//  C = c0 + c1 t + c2 t*
//
static boost::optional<double> solveForWetBulb(double drybulb, double p, double W, double deltaLimit, int itermax)
{
  double tstar = drybulb;
  double Ap, Bp, Cp;
  double a0, a1, b, c0, c1t, c2;
  int i = 0;
  a0 = 2501;
  a1 = -2.326;
  b = 1.006;
  c0 = 2501;
  c1t = 1.86*drybulb;
  c2 = -4.186;
  Ap = -2.326;
  Bp = -1.006;
  Cp = -4.186;
  if (drybulb < 0) {
    a0 = 2830;
    a1 = -0.24;
    c0 = 2830;
    c2 = -2.1;
    Ap = -0.24;
    Cp = -2.1;
  }

  while (i < itermax) {
    i++;
    double A = a0 + a1*tstar;
    double B = b*(drybulb - tstar);
    double C = c0 + c1t + c2*tstar;
    double pwsstar = psat(tstar);
    double pwsstarp = psatp(tstar, pwsstar);
    double deltap = p - pwsstar;
    double Wsstar = 0.621945*pwsstar / deltap;
    double Wsstarp = (0.621945*pwsstarp*deltap + 0.621945*pwsstar*pwsstarp) / (deltap*deltap);
    double f = W*C - A*Wsstar + B;
    double fp = W*Cp - A*Wsstarp - Ap*Wsstar + Bp;
    double delta = -f / fp;
    tstar += delta;
    // std::cout << i << " " << tstar << " " << delta / (273.15 + tstar) << std::endl;
    if (fabs(delta / (273.15 + tstar)) <= deltaLimit) {
      return boost::optional<double>(tstar);
    }
  }
  return boost::none;
}

// Using equation 38 in ASHRAE Fundamentals 2009 Ch. 1:
//
//  psat(td) = pw
//
static boost::optional<double> solveForDewPoint(double drybulb, double pw, double deltaLimit, int itermax)
{
  //double deltaLimit = percentChange*0.01;
  double tdew = drybulb;
  int i = 0;

  while (i < itermax) {
    i++;
    double pws = psat(tdew);
    double f = pws - pw;
    double fp = psatp(tdew, pws);
    double delta = -f / fp;
    tdew += delta;
    // std::cout << i << " " << tdew << " " << delta / (273.15 + tdew) << std::endl;
    if (fabs(delta / (273.15 + tdew)) <= deltaLimit) {
      return boost::optional<double>(tdew);
    }
  }
  return boost::none;
}

AirState::AirState()
{
  // Set parameters
  m_drybulb = 20.0;
  m_pressure = 101325.0;
  m_phi = 0.5;
  // Compute moist air properties, eqns from ASHRAE Fundamentals 2009 Ch. 1, should probably just set all of these
  m_psat = psat(m_drybulb); // Water vapor saturation pressure (uses eqns 5 and 6)
  double pw = m_phi * m_psat; // Relative humidity, eqn 24
  m_W = 0.621945 * pw / (m_pressure - pw); // Humidity ratio, eqn 22
  m_h = 1.006*m_drybulb + m_W*(2501 + 1.86*m_drybulb); // Moist air specific enthalpy, eqn 32
  m_v = 0.287042*(m_drybulb + 273.15)*(1 + 1.607858*m_W) / m_pressure; // Specific volume, eqn 28
  // Compute the dew point temperature here
  boost::optional<double> dewpoint = solveForDewPoint(m_drybulb, pw, 1e-4, 100);
  OS_ASSERT(dewpoint);
  m_dewpoint = dewpoint.get();
  // Compute the wet bulb temperature here
  boost::optional<double> wetbulb = solveForWetBulb(m_drybulb, m_pressure, m_W, 1e-4, 100);
  OS_ASSERT(wetbulb);
  m_wetbulb = wetbulb.get();
}

boost::optional<AirState> AirState::fromDryBulbDewPointPressure(double drybulb, double dewpoint, double pressure)
{
  AirState state;
  if (drybulb < -100.0 || drybulb > 200.0) {
    // Out of the range of our current psat function
    return boost::none;
  }
  if (dewpoint < -100.0 || dewpoint > 200.0) {
    // Out of the range of our current psat function
    return boost::none;
  }
  state.m_drybulb = drybulb;
  state.m_dewpoint = dewpoint;
  state.m_pressure = pressure;
  // Compute moist air properties, eqns from ASHRAE Fundamentals 2009 Ch. 1
  double pw = psat(dewpoint); // // Partial pressure of water vapor, eqn 38 (uses eqns 5 and 6)
  state.m_W = 0.621945 * pw / (pressure - pw); // Humidity ratio, eqn 22
  state.m_psat = psat(drybulb); // Water vapor saturation pressure (uses eqns 5 and 6)
  //double Ws = 0.621945 * state.m_psat / (pressure - state.m_psat);
  state.m_phi = pw / state.m_psat; // Relative humidity, eqn 24
  state.m_h = 1.006*drybulb + state.m_W*(2501 + 1.86*drybulb); // Moist air specific enthalpy, eqn 32
  state.m_v = 0.287042*(drybulb + 273.15)*(1 + 1.607858*state.m_W) / pressure; // Specific volume, eqn 28
  // Compute the wet bulb temperature here
  boost::optional<double> wetbulb = solveForWetBulb(drybulb, pressure, state.m_W, 1e-4, 100);
  if (!wetbulb) {
    return boost::none;
  }
  state.m_wetbulb = wetbulb.get();
  return boost::optional<AirState>(state);
}

boost::optional<AirState> AirState::fromDryBulbRelativeHumidityPressure(double drybulb, double RH, double pressure)
{
  AirState state;
  if (drybulb < -100.0 || drybulb > 200.0) {
    // Out of the range of our current psat function
    return boost::none;
  }
  if (RH > 100.0 || RH < 0.0) {
    // Out of the range
    return boost::none;
  }
  state.m_drybulb = drybulb;
  state.m_phi = 0.01*RH;
  state.m_pressure = pressure;
  // Compute moist air properties, eqns from ASHRAE Fundamentals 2009 Ch. 1
  state.m_psat = psat(drybulb); // Water vapor saturation pressure (uses eqns 5 and 6)
  double pw = state.m_phi * state.m_psat; // Relative humidity, eqn 24
  state.m_W = 0.621945 * pw / (pressure - pw); // Humidity ratio, eqn 22
  state.m_h = 1.006*drybulb + state.m_W*(2501 + 1.86*drybulb); // Moist air specific enthalpy, eqn 32
  state.m_v = 0.287042*(drybulb + 273.15)*(1 + 1.607858*state.m_W) / pressure; // Specific volume, eqn 28
  // Compute the dew point temperature here
  boost::optional<double> dewpoint = solveForDewPoint(drybulb, pw, 1e-4, 100);
  if (!dewpoint) {
    return boost::none;
  }
  state.m_dewpoint = dewpoint.get();
  // Compute the wet bulb temperature here
  boost::optional<double> wetbulb = solveForWetBulb(drybulb, pressure, state.m_W, 1e-4, 100);
  if (!wetbulb) {
    return boost::none;
  }
  state.m_wetbulb = wetbulb.get();
  return boost::optional<AirState>(state);
}


double AirState::drybulb() const
{
  return m_drybulb;
}

double AirState::dewpoint() const
{
  return m_dewpoint;
}

double AirState::wetbulb() const
{
  return m_wetbulb;
}

double AirState::relativeHumidity() const
{
  return 100.0*m_phi;
}

double AirState::pressure() const
{
  return m_pressure;
}

double AirState::enthalpy() const
{
  return m_h;
}

double AirState::saturationPressure() const
{
  return m_psat;
}

double AirState::density() const
{
  return 1.0 / m_v;
}

double AirState::specificVolume() const
{
  return m_v;
}

double AirState::humidityRatio() const
{
  return m_W;
}

double AirState::R()
{
  return 8314.472/28.966; // eqn 1 from ASHRAE Fundamentals 2009 Ch. 1
}

EpwDataPoint::EpwDataPoint() :
  m_year(1),
  m_month(1),
  m_day(1),
  m_hour(1),
  m_minute(0),
  m_dataSourceandUncertaintyFlags(""),
  m_dryBulbTemperature("99.9"),
  m_dewPointTemperature("99.9"),
  m_relativeHumidity("999"),
  m_atmosphericStationPressure("999999"),
  m_extraterrestrialHorizontalRadiation("9999"),
  m_extraterrestrialDirectNormalRadiation("9999"),
  m_horizontalInfraredRadiationIntensity("9999"),
  m_globalHorizontalRadiation("9999"),
  m_directNormalRadiation("9999"),
  m_diffuseHorizontalRadiation("9999"),
  m_globalHorizontalIlluminance("999999"),
  m_directNormalIlluminance("999999"),
  m_diffuseHorizontalIlluminance("999999"),
  m_zenithLuminance("9999"),
  m_windDirection("999"),
  m_windSpeed("999"),
  m_totalSkyCover(99),
  m_opaqueSkyCover(99),
  m_visibility("9999"),
  m_ceilingHeight("99999"),
  m_presentWeatherObservation(0),
  m_presentWeatherCodes(0),
  m_precipitableWater("999"),
  m_aerosolOpticalDepth(".999"),
  m_snowDepth("999"),
  m_daysSinceLastSnowfall("99"),
  m_albedo("999"),
  m_liquidPrecipitationDepth("999"),
  m_liquidPrecipitationQuantity("99")
{}

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
  std::vector<std::string> list = splitString(line, ',');
  return fromEpwStrings(list);
}

boost::optional<EpwDataPoint> EpwDataPoint::fromEpwStrings(const std::vector<std::string> &list, bool pedantic)
{
  EpwDataPoint pt;
  // Expect 35 items in the list
  if (list.size() < 35) {
    if (pedantic) {
      LOG_FREE(Error, "openstudio.EpwFile", "Expected 35 fields in EPW data instead of the " << list.size() << " recieved");
      return boost::none;
    } else {
      LOG_FREE(Warn, "openstudio.EpwFile", "Expected 35 fields in EPW data instead of the " << list.size() << " recieved. The remaining fields will not be available");
    }
  }
  else if (list.size() > 35) {
    LOG_FREE(Warn, "openstudio.EpwFile", "Expected 35 fields in EPW data instead of the " << list.size() << " recieved. The additional data will be ignored");
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

boost::optional<EpwDataPoint> EpwDataPoint::fromEpwStrings(int year, int month, int day, int hour, int minute,
  const std::vector<std::string> &list, bool pedantic)
{
  EpwDataPoint pt;
  // Expect 30 items in the list
  if (list.size() < 35) {
    if (pedantic) {
      LOG_FREE(Error, "openstudio.EpwFile", "Expected 35 fields in EPW data instead of the " << list.size() << " recieved");
      return boost::none;
    } else {
      LOG_FREE(Warn, "openstudio.EpwFile", "Expected 35 fields in EPW data instead of the " << list.size() << " recieved. The remaining fields will not be available");
    }
  } else if (list.size() > 35) {
    LOG_FREE(Warn, "openstudio.EpwFile", "Expected 35 fields in EPW data instead of the " << list.size() << " recieved. The additional data will be ignored");
  }
  // Use the appropriate setter on each field
  pt.setYear(year);
  if (!pt.setMonth(month)) {
    return boost::none;
  }
  if (!pt.setDay(day)) {
    return boost::none;
  }
  if (!pt.setHour(hour)) {
    return boost::none;
  }
  if (!pt.setMinute(minute)) {
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

boost::optional<std::string> EpwDataPoint::getUnitsByName(const std::string &name)
{
  EpwDataField id;
  try {
    id = EpwDataField(name);
  } catch(...) {
    // Could do a warning message here
    return boost::none;
  }
  return boost::optional<std::string>(getUnits(id));
}

std::string EpwDataPoint::getUnits(EpwDataField field)
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

std::string EpwDataPoint::getUnits(EpwComputedField field)
{
  std::string string;
  switch (field.value())
  {
  case EpwComputedField::SaturationPressure:
    string = "Pa";
    break;
  case EpwComputedField::Enthalpy:
    string = "kJ/kg";
    break;
  case EpwComputedField::HumidityRatio:
    break;
  case EpwComputedField::WetBulbTemperature:
    string = "C";
    break;
  default:
    break;
  }
  return string;
}

boost::optional<double> EpwDataPoint::getFieldByName(const std::string &name)
{
  EpwDataField id;
  try {
    id = EpwDataField(name);
  } catch(...) {
    // Could do a warning message here
    return boost::none;
  }
  return getField(id);
}

boost::optional<double> EpwDataPoint::getField(EpwDataField id)
{
  boost::optional<int> ivalue;
  switch(id.value()) {
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
    if(ivalue) {
      return boost::optional<double>((double)ivalue.get());
    }
    break;
  case EpwDataField::OpaqueSkyCover:
    ivalue = opaqueSkyCover();
    if(ivalue) {
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
    if(ivalue) {
      return boost::optional<double>((double)ivalue.get());
    }
    break;
  case EpwDataField::PresentWeatherCodes:
    ivalue = presentWeatherCodes();
    if(ivalue) {
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
    return boost::none;
    break;
  }
  return boost::none;
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
  //boost::optional<double> optdrybulb = openstudio::convert(value.get(), "C", "K");
  //OS_ASSERT(optdrybulb);
  double drybulb = value.get();
  string += '\t' + std::to_string(drybulb + 273.15);
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
    //boost::optional<double> optdewpoint = openstudio::convert(value.get(), "C", "K");
    //OS_ASSERT(optdewpoint);
    double dewpoint = value.get(); 
    pw = openstudio::psat(dewpoint);
  } else  { // Have relative humidity
    double pws = openstudio::psat(drybulb);
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
  } catch (const std::invalid_argument) {
    *ok = false;
  } catch (const std::out_of_range) {
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
  } catch (const std::invalid_argument) {
    *ok = false;
  } catch (const std::out_of_range) {
    *ok = false;
  }
  return value;
}

Date EpwDataPoint::date() const
{
  return Date(MonthOfYear(m_month), m_day); // , m_year);
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

bool EpwDataPoint::setDryBulbTemperature(double value)
{
  if(-70 >= value || 70 <= value) {
    m_dryBulbTemperature = "99.9";
    return false;
  }
  m_dryBulbTemperature = std::to_string(value);
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
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_dewPointTemperature));
}

bool EpwDataPoint::setDewPointTemperature(double value)
{
  if(-70 >= value || 70 <= value) {
    m_dewPointTemperature = "99.9";
    return false;
  }
  m_dewPointTemperature = std::to_string(value);
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

bool EpwDataPoint::setRelativeHumidity(double value)
{
  if(0 > value || 110 < value) {
    m_relativeHumidity = "999";
    return false;
  }
  m_relativeHumidity = std::to_string(value);
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

bool EpwDataPoint::setAtmosphericStationPressure(double value)
{
  if(31000 >= value || 120000 <= value) {
    m_atmosphericStationPressure = "999999";
    return false;
  }
  m_atmosphericStationPressure = std::to_string(value);
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

bool EpwDataPoint::setExtraterrestrialHorizontalRadiation(double value)
{
  if(0 > value || value == 9999) {
    m_extraterrestrialHorizontalRadiation = "9999";
    return false;
  }
  m_extraterrestrialHorizontalRadiation = std::to_string(value);
  return true;
}

bool EpwDataPoint::setExtraterrestrialHorizontalRadiation(const std::string &extraterrestrialHorizontalRadiation)
{
  bool ok;
  double value = stringToDouble(extraterrestrialHorizontalRadiation, &ok);
  if(!ok || 0 > value || value == 9999) {
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

bool EpwDataPoint::setExtraterrestrialDirectNormalRadiation(double value)
{
  if(0 > value || value == 9999) {
    m_extraterrestrialDirectNormalRadiation = "9999";
    return false;
  }
  m_extraterrestrialDirectNormalRadiation = std::to_string(value);
  return true;
}

bool EpwDataPoint::setExtraterrestrialDirectNormalRadiation(const std::string &extraterrestrialDirectNormalRadiation)
{
  bool ok;
  double value = stringToDouble(extraterrestrialDirectNormalRadiation, &ok);
  if(!ok || 0 > value || value == 9999) {
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

bool EpwDataPoint::setHorizontalInfraredRadiationIntensity(double value)
{
  if(0 > value || value == 9999) {
    m_horizontalInfraredRadiationIntensity = "9999";
    return false;
  }
  m_horizontalInfraredRadiationIntensity = std::to_string(value);
  return true;
}

bool EpwDataPoint::setHorizontalInfraredRadiationIntensity(const std::string &horizontalInfraredRadiationIntensity)
{
  bool ok;
  double value = stringToDouble(horizontalInfraredRadiationIntensity, &ok);
  if(!ok || 0 > value || value == 9999) {
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

bool EpwDataPoint::setGlobalHorizontalRadiation(double value)
{
  if(0 > value || value == 9999) {
    m_globalHorizontalRadiation = "9999";
    return false;
  }
  m_globalHorizontalRadiation = std::to_string(value);
  return true;
}

bool EpwDataPoint::setGlobalHorizontalRadiation(const std::string &globalHorizontalRadiation)
{
  bool ok;
  double value = stringToDouble(globalHorizontalRadiation, &ok);
  if(!ok || 0 > value || value == 9999) {
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

bool EpwDataPoint::setDirectNormalRadiation(double value)
{
  if(0 > value || value == 9999) {
    m_directNormalRadiation = "9999";
    return false;
  }
  m_directNormalRadiation = std::to_string(value);
  return true;
}

bool EpwDataPoint::setDirectNormalRadiation(const std::string &directNormalRadiation)
{
  bool ok;
  double value = stringToDouble(directNormalRadiation, &ok);
  if(!ok || 0 > value || value == 9999) {
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

bool EpwDataPoint::setDiffuseHorizontalRadiation(double value)
{
  if(0 > value || value == 9999) {
    m_diffuseHorizontalRadiation = "9999";
    return false;
  }
  m_diffuseHorizontalRadiation = std::to_string(value);
  return true;
}

bool EpwDataPoint::setDiffuseHorizontalRadiation(const std::string &diffuseHorizontalRadiation)
{
  bool ok;
  double value = stringToDouble(diffuseHorizontalRadiation, &ok);
  if(!ok || 0 > value || value == 9999) {
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

bool EpwDataPoint::setGlobalHorizontalIlluminance(double value)
{
  if(0 > value || 999900 < value) {
    m_globalHorizontalIlluminance = "999999";
    return false;
  }
  m_globalHorizontalIlluminance = std::to_string(value);
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
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_directNormalIlluminance));
}

bool EpwDataPoint::setDirectNormalIlluminance(double value)
{
  if(0 > value || 999900 < value) {
    m_directNormalIlluminance = "999999";
    return false;
  }
  m_directNormalIlluminance = std::to_string(value);
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

bool EpwDataPoint::setDiffuseHorizontalIlluminance(double value)
{
  if(0 > value || 999900 < value) {
    m_diffuseHorizontalIlluminance = "999999";
    return false;
  }
  m_diffuseHorizontalIlluminance = std::to_string(value);
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

bool EpwDataPoint::setZenithLuminance(double value)
{
  if(0 > value || 9999 <= value) {
    m_zenithLuminance = "9999";
    return false;
  }
  m_zenithLuminance = std::to_string(value);
  return true;
}

bool EpwDataPoint::setZenithLuminance(const std::string &zenithLuminance)
{
  bool ok;
  double value = stringToDouble(zenithLuminance, &ok);
  if(!ok || 0 > value || 9999 <= value) {
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

bool EpwDataPoint::setWindDirection(double value)
{
  if(0 > value || 360 < value) {
    m_windDirection = "999";
    return false;
  }
  m_windDirection = std::to_string(value);
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

bool EpwDataPoint::setWindSpeed(double value)
{
  if(0 > value || 40 < value) {
    m_windSpeed = "999";
    return false;
  }
  m_windSpeed = std::to_string(value);
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

bool EpwDataPoint::setTotalSkyCover(int value)
{
  if(0 > value || 10 < value) {
    m_totalSkyCover = 99;
    return false;
  }
  m_totalSkyCover = value;
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

bool EpwDataPoint::setOpaqueSkyCover(int value)
{
  if(0 > value || 10 < value) {
    m_opaqueSkyCover = 99;
    return false;
  }
  m_opaqueSkyCover = value;
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

bool EpwDataPoint::setVisibility(double value)
{
  if(value == 9999) {
    m_visibility = "9999";
    return false;
  }
  m_visibility = std::to_string(value);
  return true;
}

bool EpwDataPoint::setVisibility(const std::string &visibility)
{
  bool ok;
  double value = stringToDouble(visibility, &ok);
  if(!ok || value == 9999) {
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
  double value = stringToDouble(ceilingHeight, &ok);
  if(!ok || value == 99999) {
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
  int value = stringToInteger(presentWeatherCodes, &ok);
  if(!ok) {
    return false;
  }
  m_presentWeatherCodes = value;
  return true;
}

boost::optional<double> EpwDataPoint::precipitableWater() const
{
  if(m_precipitableWater == "999") {
    return boost::none;
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
  double value = stringToDouble(precipitableWater, &ok);
  if(!ok || value == 999) {
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
  double value = stringToDouble(aerosolOpticalDepth, &ok);
  if(!ok || value == 0.999) {
    m_aerosolOpticalDepth = ".999";
    return false;
  }
  m_aerosolOpticalDepth = aerosolOpticalDepth;
  return true;
}

boost::optional<double> EpwDataPoint::snowDepth() const
{
  if(m_snowDepth == "999") {
    return boost::none;
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
  double value = stringToDouble(snowDepth, &ok);
  if(!ok || value == 999) {
    m_snowDepth = "999";
    return false;
  }
  m_snowDepth = snowDepth;
  return true;
}

boost::optional<double> EpwDataPoint::daysSinceLastSnowfall() const
{
  if(m_daysSinceLastSnowfall == "99") {
    return boost::none;
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
  double value = stringToDouble(daysSinceLastSnowfall, &ok);
  if(!ok || value == 99) {
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
  double value = stringToDouble(albedo, &ok);
  if(!ok || value == 999) {
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
  double value = stringToDouble(liquidPrecipitationDepth, &ok);
  if(!ok || value == 999) {
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
  double value = stringToDouble(liquidPrecipitationQuantity, &ok);
  if(!ok || value == 99) {
    m_liquidPrecipitationQuantity = "99";
    return false;
  }
  m_liquidPrecipitationQuantity = liquidPrecipitationQuantity;
  return true;
}

boost::optional<AirState> EpwDataPoint::airState() const
{
  boost::optional<double> value = dryBulbTemperature();
  if (!value) {
    return boost::none; // Have to have dry bulb
  }
  double drybulb = value.get();

  value = atmosphericStationPressure();
  if (!value) {
    return boost::none; // Have to have pressure
  }
  double pressure = value.get();

  value = relativeHumidity();
  if (!value) { // Don't have relative humidity
    value = dewPointTemperature();
    if (value) {
      double dewpoint = value.get();
      return AirState::fromDryBulbDewPointPressure(drybulb, dewpoint, pressure);
    }
  } else { // Have relative humidity
    double RH = value.get();
    return AirState::fromDryBulbRelativeHumidityPressure(drybulb, RH, pressure);
  }

  return boost::none;
}

boost::optional<double> EpwDataPoint::saturationPressure() const
{
  boost::optional<double> optdrybulb = dryBulbTemperature();
  if (optdrybulb) {
    double drybulb = optdrybulb.get();
    if (drybulb >= -100.0 && drybulb <= 200.0) {
      return boost::optional<double>(openstudio::psat(drybulb));
    }
  }
  return boost::none;
}

boost::optional<double> EpwDataPoint::enthalpy() const
{
  boost::optional<AirState> state = airState();
  if (state) {
    return boost::optional<double>(state.get().enthalpy());
  }
  return boost::none;
}

boost::optional<double> EpwDataPoint::humidityRatio() const
{
  boost::optional<AirState> state = airState();
  if (state) {
    return boost::optional<double>(state.get().humidityRatio());
  }
  return boost::none;
}

boost::optional<double> EpwDataPoint::density() const
{
  boost::optional<AirState> state = airState();
  if (state) {
    return boost::optional<double>(state.get().density());
  }
  return boost::none;
}

boost::optional<double> EpwDataPoint::specificVolume() const
{
  boost::optional<AirState> state = airState();
  if (state) {
    return boost::optional<double>(state.get().specificVolume());
  }
  return boost::none;
}

boost::optional<double> EpwDataPoint::wetbulb() const
{
  boost::optional<AirState> state = airState();
  if (state) {
    return boost::optional<double>(state.get().wetbulb());
  }
  return boost::none;
}

EpwFile::EpwFile(const openstudio::path& p, bool storeData)
    : m_path(p), m_latitude(0), m_longitude(0), m_timeZone(0), m_elevation(0), m_isActual(false), m_minutesMatch(true)
{
  if (!parse(storeData)){
    LOG_AND_THROW("EpwFile '" << toString(p) << "' cannot be processed");
  }
}

boost::optional<EpwFile> EpwFile::load(const openstudio::path& p, bool storeData)
{
  boost::optional<EpwFile> result;
  try {
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
  if(m_data.size()==0) {
    if (!parse(true)) {
      LOG(Error,"EpwFile '" << toString(m_path) << "' cannot be processed");
      return boost::none;
    } 
  }
  EpwDataField id;
  try {
    id = EpwDataField(name);
  } catch(...) {
    LOG(Warn, "Unrecognized EPW data field '" << name << "'");
    return boost::none;
  }
  if(m_data.size() > 0) {
    std::string units = EpwDataPoint::getUnits(id);
    DateTimeVector dates;
    dates.push_back(DateTime()); // Use a placeholder to avoid an insert
    std::vector<double> values;
    for(unsigned int i=0;i<m_data.size();i++) {
      DateTime dateTime=m_data[i].dateTime();
      Time time=m_data[i].time();
      boost::optional<double> value = m_data[i].getField(id);
      if(value) {
        dates.push_back(DateTime(dateTime));
        values.push_back(value.get());
      }
    }
    if(values.size()) {
      DateTime start = dates[1] - Time(0, 0, 0, 3600.0 / m_recordsPerHour);
      dates[0] = start; // Overwrite the placeholder
      return boost::optional<TimeSeries>(TimeSeries(dates,openstudio::createVector(values),units));
    }
  }
  return boost::none;
}

boost::optional<TimeSeries> EpwFile::getComputedTimeSeries(const std::string &name)
{
  if (m_data.size() == 0) {
    if (!parse(true)) {
      LOG(Error, "EpwFile '" << toString(m_path) << "' cannot be processed");
      return boost::none;
    }
  }
  EpwComputedField id;
  try {
    id = EpwComputedField(name);
  }
  catch (...) {
    LOG(Warn, "Unrecognized computed data field '" << name << "'");
    return boost::none;
  }

  std::string units = EpwDataPoint::getUnits(id);
  boost::optional<double>(EpwDataPoint::*compute)() const;
  switch (id.value()) {
  case EpwComputedField::SaturationPressure:
    compute = &EpwDataPoint::saturationPressure;
    break;
  case EpwComputedField::Enthalpy:
    compute = &EpwDataPoint::enthalpy;
    break;
  case EpwComputedField::HumidityRatio:
    compute = &EpwDataPoint::humidityRatio;
    break;
  case EpwComputedField::WetBulbTemperature:
    compute = &EpwDataPoint::wetbulb;
    break;
  case EpwComputedField::Density:
    compute = &EpwDataPoint::density;
    break;
  case EpwComputedField::SpecificVolume:
    compute = &EpwDataPoint::specificVolume;
    break;
  default:
    return boost::none;
  }
  DateTimeVector dates;
  dates.push_back(DateTime()); // Use a placeholder to avoid an insert
  std::vector<double> values;
  for (unsigned int i = 0; i<m_data.size(); i++) {
    Date date = m_data[i].date();
    Time time = m_data[i].time();
    boost::optional<double> value = (m_data[i].*compute)();
    if (value) {
      dates.push_back(DateTime(date, time));
      values.push_back(value.get());
    }
  }
  if (values.size()) {
    DateTime start = dates[1] - Time(0, 0, 0, 3600.0 / m_recordsPerHour);
    dates[0] = start; // Overwrite the placeholder
    return boost::optional<TimeSeries>(TimeSeries(dates, openstudio::createVector(values), units));
  }
  return boost::none;
}

bool EpwFile::translateToWth(openstudio::path path, std::string description)
{
  if(m_data.size()==0) {
    if (!parse(true)) {
      LOG(Error,"EpwFile '" << toString(m_path) << "' cannot be processed");
      return false;
    }
  }

  if(description.empty()) {
    description = "Translated from " + openstudio::toString(this->path());
  }

  if(!data().size()) {
    LOG(Error, "EPW file contains no data to translate");
    return false;
  }

  QFile fp(openstudio::toQString(path));
  if(!fp.open(QFile::WriteOnly)) {
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
  for(openstudio::Date current=startDate();current<=endDate();current += delta) {
    stream << QString("%1/%2\t%3\t%3\t0\t283.15\n")
      .arg(openstudio::month(current.monthOfYear()))
      .arg(current.dayOfMonth())
      .arg(dayofweek);
    dayofweek++;
    if(dayofweek > 7) {
      dayofweek=1;
    }
  }

  // Cheat to get data at the start time - this will need to change
  openstudio::EpwDataPoint lastPt = data()[data().size()-1];
  std::vector<std::string> epwstrings = lastPt.toEpwStrings();
  openstudio::DateTime dateTime = data()[0].dateTime();
  openstudio::Time dt = timeStep();
  dateTime -= dt;
  epwstrings[0] = std::to_string(dateTime.date().year());
  epwstrings[1] = std::to_string(month(dateTime.date().monthOfYear()));
  epwstrings[2] = std::to_string(dateTime.date().dayOfMonth());
  epwstrings[3] = std::to_string(dateTime.time().hours());
  epwstrings[4] = std::to_string(dateTime.time().minutes());
  boost::optional<openstudio::EpwDataPoint> firstPtOpt = EpwDataPoint::fromEpwStrings(epwstrings);
  if (!firstPtOpt) {
    LOG(Error, "Failed to create starting data point for WTH file");
    return false;
  }

  stream <<"!Date\tTime\tTa [K]\tPb [Pa]\tWs [m/s]\tWd [deg]\tHr [g/kg]\tIth [kJ/m^2]\tIdn [kJ/m^2]\tTs [K]\tRn [-]\tSn [-]\n";
  boost::optional<std::string> output = firstPtOpt.get().toWthString();
  if(!output) {
    LOG(Error, "Translation to WTH has failed on starting data point");
    fp.close();
    return false;
  }
  stream << output.get() << '\n';
  for(unsigned int i=0;i<data().size();i++) {
    output = data()[i].toWthString();
    if(!output) {
      LOG(Error, "Translation to WTH has failed on data point " << i);
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
  std::ifstream ifs(openstudio::toString(m_path));

  // read line by line
  std::string line;

  bool result = true;

  // read first 8 lines
  for(unsigned i = 0; i < 8; ++i) {

    if(!std::getline(ifs, line)) {
      LOG(Error, "Could not read line " << i+1 << " of EPW file '" << m_path << "'");
      ifs.close();
      return false;
    }

    switch(i) {
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

  if (!result){
    ifs.close();
    LOG(Error, "Failed to parse EPW file header '" << m_path << "'");
    return false;
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
  while(std::getline(ifs, line)) {
    lineNumber++;
    std::vector<std::string> strings = splitString(line, ',');
    if (strings.size() >= 5) {
      try {
        int year = std::stoi(strings[0]);
        int month = std::stoi(strings[1]);
        int day = std::stoi(strings[2]);

        Date date(month, day, year);
        if (!startDate) {
          startDate = date;
        }
        endDate = date;

        if (endDate && lastDate) {
          Time delta = endDate.get() - lastDate.get();
          if (std::abs(delta.totalDays()) > 1) {
            if (realYear) { // Warn once
              LOG(Warn, "Successive data points (" << lastDate.get() << " to " << endDate.get()
                << ", ending on line " << lineNumber << ") are greater than 1 day apart in EPW file '"
                << m_path << "'. Data will be treated as typical (TMY)");
            }
            realYear = false;
          }

          if (endDate->monthOfYear().value() < lastDate->monthOfYear().value()) {
            wrapAround = true;
          }
        }
        lastDate = date;

        // Store the data if requested
        if (storeData) {
          int hour = std::stoi(strings[3]);
          int minutesInFile = std::stoi(strings[4]);
          // Due to issues with some EPW files, we need to check stuff here
          if (m_recordsPerHour != 1) {
            currentMinute += minutesPerRecord;
            if (currentMinute >= 60) { // This could really be ==, but >= is used for safety
              currentMinute = 0;
            }
          }
          // Check for agreement between the file value and the computed value
          if (currentMinute != minutesInFile) {
            if (m_minutesMatch) { // Warn only once
              LOG(Error, "Minutes field (" << minutesInFile << ") on line " << lineNumber << " of EPW file '"
                << m_path << "' does not agree with computed value (" << currentMinute << "). Using computed value");
              m_minutesMatch = false;
            }
          }
          boost::optional<EpwDataPoint> pt = EpwDataPoint::fromEpwStrings(year, month, day, hour, currentMinute, strings);
          if (pt) {
            m_data.push_back(pt.get());
          } else {
            LOG(Error, "Failed to parse line " << lineNumber << " of EPW file '" << m_path << "'");
            ifs.close();
            return false;
          }
        }

      } catch(...) {
        LOG(Error, "Could not read line " << lineNumber << " of EPW file '" << m_path << "'");
        ifs.close();
        return false;
      }
    } else {
      LOG(Error, "Insufficient weather data on line " << lineNumber << " of EPW file '" << m_path << "'");
      ifs.close();
      return false;
    }
  }

  // close file
  ifs.close();

  if (!startDate) {
    LOG(Error, "Could not find start date in data section of EPW file '" << m_path << "'");
    return false;
  }
  if (!endDate) {
    LOG(Error, "Could not find end date in data section of EPW file '" << m_path << "'");
    return false;
  }

  if ((m_startDate.monthOfYear() != startDate->monthOfYear()) ||
      (m_startDate.dayOfMonth() != startDate->dayOfMonth())) {
    LOG(Error, "Header start date does not match data in EPW file '" << m_path << "'");
    return false;
  }

  if((m_endDate.monthOfYear() != endDate->monthOfYear()) ||
      (m_endDate.dayOfMonth() != endDate->dayOfMonth())) {
      LOG(Error, "Header end date does not match data in EPW file '" << m_path << "'");
      return false;
  }

  if (realYear) {
    if (m_startDayOfWeek != startDate->dayOfWeek()){
      LOG(Warn, "Header start day of the week and actual start day of the week do not match in EPW file '" << m_path << "'. Data will be treated as typical (TMY)");
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
  // LOCATION,Chicago Ohare Intl Ap,IL,USA,TMY3,725300,41.98,-87.92,-6.0,201.0
  // LOCATION, city, stateProvinceRegion, country, dataSource, wmoNumber, latitude, longitude, timeZone, elevation
  std::vector<std::string> split = splitString(line, ',');
  if(split.size() < 10) {
    LOG(Error, "Expected 10 location fields rather than the " << split.size() << " fields in EPW file '" << m_path << "'");
    return false;
  } else if(split.size() > 10) {
    LOG(Warn, "Expected 10 location fields rather than the " << split.size() << " fields in EPW file '" << m_path << "', additional fields will be ignored");
  }

  if(split[0] != "LOCATION") {
    LOG(Error, "Missing LOCATION specifier in EPW file '" << m_path << "'");
    return false;
  }

  m_city = split[1]; boost::trim(m_city);
  m_stateProvinceRegion = split[2]; boost::trim(m_stateProvinceRegion);
  m_country = split[3]; boost::trim(m_country);
  m_dataSource = split[4]; boost::trim(m_dataSource);
  m_wmoNumber = split[5]; boost::trim(m_wmoNumber);

  std::string latitude = split[6]; boost::trim(latitude);
  std::string longitude = split[7]; boost::trim(longitude);
  std::string timeZone = split[8]; boost::trim(timeZone);
  std::string elevation = split[9]; boost::trim(elevation);

  try{
    m_latitude = std::stod(latitude);
  } catch(...) {
    LOG(Error, "Non-numerical latitude in EPW file '" << m_path << "'");
    return false;
  }
  try{
    m_longitude = std::stod(longitude);
  } catch(...) {
    LOG(Error, "Non-numerical longitude in EPW file '" << m_path << "'");
    return false;
  }
  try{
    m_timeZone = std::stod(timeZone);
  } catch(...) {
    LOG(Error, "Non-numerical timezone in EPW file '" << m_path << "'");
    return false;
  }
  try{
    m_elevation = std::stod(elevation);
  } catch(...) {
    LOG(Error, "Non-numerical elevation in EPW file '" << m_path << "'");
    return false;
  }

  return true;
}

bool EpwFile::parseDataPeriod(const std::string& line)
{
  // DATA PERIODS,1,1,Data,Sunday, 1/ 1,12/31
  // DATA PERIODS, nDataPeriods, timeStep, startDayOfWeek, startDate, endDate
  // NOTE THAT ONLY ONE DATA PERIOD IS SUPPORTED
  std::vector<std::string> split = splitString(line, ',');
  if(split.size() < 7) {
    LOG(Error, "Expected 7 data period fields rather than the " << split.size() << " fields in EPW file '" << m_path << "'");
    return false;
  } else if(split.size() > 7) {
    LOG(Warn, "Expected 7 data period fields rather than the " << split.size() << " fields in EPW file '" << m_path << "', additional fields will be ignored");
  }

  if(split[0] != "DATA PERIODS") {
    LOG(Error, "Missing DATA PERIODS specifier in EPW file '" << m_path << "'");
    return false;
  }

  std::string nDataPeriods =  split[1]; boost::trim(nDataPeriods);
  std::string timeStep = split[2]; boost::trim(timeStep);
  std::string startDayOfWeek = split[4]; boost::trim(startDayOfWeek);
  std::string startDate = split[5]; boost::trim(startDate);
  std::string endDate = split[6]; boost::trim(endDate);

  try{
    int N = std::stoi(nDataPeriods);
    if(N > 1) {
      LOG(Error, "More than one data period in EPW file '" << m_path << "', which is not supported");
      return false;
    }
  } catch(...) {
    LOG(Error, "Non-integral number of data periods in EPW file '" << m_path << "'");
    return false;
  }

  try{
    m_recordsPerHour = std::stoi(timeStep);
    if((60 % m_recordsPerHour) != 0) {
      LOG(Error, "Number of records per hour of " << m_recordsPerHour << " does not result in integral number of minutes between records in EPW file '" << m_path << "'");
      return false;
    }
  } catch(...) {
    LOG(Error, "Non-integral timestep in EPW file '" << m_path << "'");
    return false;
  }

  try{
    m_startDayOfWeek = DayOfWeek(startDayOfWeek);
  } catch(...) {
    LOG(Error, "Bad start day of week in EPW file '" << m_path << "'");
    return false;
  }

  try{
    // Parse start month and day, optional year
    split = splitString(startDate, '/');
    if(split.size() != 2 && split.size() != 3) {
      LOG(Error, "Bad data period start date format '" << startDate << "' in EPW file '" << m_path << "'");
      return false;
    }
    int month = std::stoi(split[0]);
    int day = std::stoi(split[1]);
    if(split.size() == 3) {
      int year = std::stoi(split[1]);
      m_startDate = Date(monthOfYear(month), day, year);
      m_startDateActualYear = year;
    } else {
      m_startDate = Date(monthOfYear(month), day);
    }
  } catch(...) {
    LOG(Error, "Failed to parse data period start date from '" << startDate << "' in EPW file '" << m_path << "'");
    return false;
  }

  try{
    // Parse end month and day, optional year
    split = splitString(endDate, '/');
    if(split.size() != 2 && split.size() != 3) {
      LOG(Error, "Bad data period end date format '" << startDate << "' in EPW file '" << m_path << "'");
      return false;
    }
    int month = std::stoi(split[0]);
    int day = std::stoi(split[1]);
    if(split.size() == 3) {
      int year = std::stoi(split[1]);
      m_endDate = Date(monthOfYear(month), day, year);
      m_endDateActualYear = year;
    } else {
      m_endDate = Date(monthOfYear(month), day);
    }
  }
  catch(...) {
    LOG(Error, "Failed to parse data period end date from '" << startDate << "' in EPW file '" << m_path << "'");
    return false;
  }

  return true;
}

bool EpwFile::isActual() const
{
    return m_isActual;
}

bool EpwFile::minutesMatch() const
{
    return m_minutesMatch;
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
