/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "EpwFile.hpp"
#include "../idf/IdfObject.hpp"
#include "../idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include "../core/Checksum.hpp"
#include "../core/StringHelpers.hpp"
#include "../core/Assert.hpp"

#include <fmt/format.h>
#include <cmath>

namespace openstudio {

static double psat(double T) {
  // Compute water vapor saturation pressure, eqns 5 and 6 from ASHRAE Fundamentals 2009 Ch. 1
  // This version takes T in C rather than Kelvin since most of the other eqns use C
  T += 273.15;
  double C1 = -5.6745359e+03;
  double C2 = 6.3925247e+00;
  double C3 = -9.6778430e-03;
  double C4 = 6.2215701e-07;
  double C5 = 2.0747825e-09;
  double C6 = -9.4840240e-13;
  double C7 = 4.1635019e+00;
  double C8 = -5.8002206e+03;
  double C9 = 1.3914993e+00;
  double C10 = -4.8640239e-02;
  double C11 = 4.1764768e-05;
  double C12 = -1.4452093e-08;
  double C13 = 6.5459673e+00;
  double rhs;
  if (T < 273.15) {
    rhs = C1 / T + C2 + T * (C3 + T * (C4 + T * (C5 + T * C6))) + C7 * std::log(T);
  } else {
    rhs = C8 / T + C9 + T * (C10 + T * (C11 + T * C12)) + C13 * std::log(T);
  }
  return exp(rhs);
}

static double psatp(double T, double psat) {
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
  double T2 = T * T;
  double T3 = T * T2;
  if (T < 273.15) {
    fp = -C1 / T2 + C3 + 2 * T * C4 + 3 * T2 * C5 + 4 * T3 * C6 + C7 / T;
  } else {
    fp = -C8 / T2 + C10 + 2 * T * C11 + 3 * T2 * C12 + C13 / T;
  }
  //std::cout << "psatp: " << T - 273.15 << " " << fp << " " << fp*psat << '\n';
  return fp * psat;
}

// Note JM 2019-01-03: suppress -Wunused-function warning for this API function
[[maybe_unused]] static double enthalpy(double T, double p, double phi) {
  // Compute moist air enthalpy, eqns 5, 6, 22, 24, and 32 from ASHRAE Fundamentals 2009 Ch. 1
  double pw = phi * psat(T);                 // Partial pressure of water vapor, eqn 24 (uses eqns 5 and 6)
  double W = 0.621945 * pw / (p - pw);       // Humidity ratio, eqn 22
  return 1.006 * T + W * (2501 + 1.86 * T);  // Moist air specific enthalpy, eqn 32
}

[[maybe_unused]] static double enthalpyFromDewPoint(double T, double p, double Tdewpoint) {
  // Compute moist air enthalpy, eqns 5, 6, 22, 32, and 38 from ASHRAE Fundamentals 2009 Ch. 1
  double pw = openstudio::psat(Tdewpoint);   // Partial pressure of water vapor, eqn 38 (uses eqns 5 and 6)
  double W = 0.621945 * pw / (p - pw);       // Humidity ratio, eqn 22
  return 1.006 * T + W * (2501 + 1.86 * T);  // Moist air specific enthalpy, eqn 32
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
static boost::optional<double> solveForWetBulb(double drybulb, double p, double W, double deltaLimit, int itermax) {
  double tstar = drybulb;
  double Ap;
  double Bp;
  double Cp;
  double a0;
  double a1;
  double b;
  double c0;
  double c1t;
  double c2;
  int i = 0;
  a0 = 2501;
  a1 = -2.326;
  b = 1.006;
  c0 = 2501;
  c1t = 1.86 * drybulb;
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
    double A = a0 + a1 * tstar;
    double B = b * (drybulb - tstar);
    double C = c0 + c1t + c2 * tstar;
    double pwsstar = psat(tstar);
    double pwsstarp = psatp(tstar, pwsstar);
    double deltap = p - pwsstar;
    double Wsstar = 0.621945 * pwsstar / deltap;
    double Wsstarp = (0.621945 * pwsstarp * deltap + 0.621945 * pwsstar * pwsstarp) / (deltap * deltap);
    double f = W * C - A * Wsstar + B;
    double fp = W * Cp - A * Wsstarp - Ap * Wsstar + Bp;
    double delta = -f / fp;
    tstar += delta;
    // std::cout << i << " " << tstar << " " << delta / (273.15 + tstar) << '\n';
    if (std::fabs(delta / (273.15 + tstar)) <= deltaLimit) {
      return boost::optional<double>(tstar);
    }
  }
  return boost::none;
}

// Using equation 38 in ASHRAE Fundamentals 2009 Ch. 1:
//
//  psat(td) = pw
//
static boost::optional<double> solveForDewPoint(double drybulb, double pw, double deltaLimit, int itermax) {
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
    // std::cout << i << " " << tdew << " " << delta / (273.15 + tdew) << '\n';
    if (std::fabs(delta / (273.15 + tdew)) <= deltaLimit) {
      return boost::optional<double>(tdew);
    }
  }
  return boost::none;
}

AirState::AirState() {
  // Set parameters
  m_drybulb = 20.0;
  m_pressure = 101325.0;
  m_phi = 0.5;
  // Compute moist air properties, eqns from ASHRAE Fundamentals 2009 Ch. 1, should probably just set all of these
  m_psat = psat(m_drybulb);                 // Water vapor saturation pressure (uses eqns 5 and 6)
  double pw = m_phi * m_psat;               // Relative humidity, eqn 24
  m_W = 0.621945 * pw / (m_pressure - pw);  // Humidity ratio, eqn 22
  // TODO: Use the function above
  m_h = 1.006 * m_drybulb + m_W * (2501 + 1.86 * m_drybulb);                  // Moist air specific enthalpy, eqn 32
  m_v = 0.287042 * (m_drybulb + 273.15) * (1 + 1.607858 * m_W) / m_pressure;  // Specific volume, eqn 28
  // Compute the dew point temperature here
  boost::optional<double> dewpoint = solveForDewPoint(m_drybulb, pw, 1e-4, 100);
  OS_ASSERT(dewpoint);
  m_dewpoint = dewpoint.get();
  // Compute the wet bulb temperature here
  boost::optional<double> wetbulb = solveForWetBulb(m_drybulb, m_pressure, m_W, 1e-4, 100);
  OS_ASSERT(wetbulb);
  m_wetbulb = wetbulb.get();
}

boost::optional<AirState> AirState::fromDryBulbDewPointPressure(double drybulb, double dewpoint, double pressure) {
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
  double pw = psat(dewpoint);                   // // Partial pressure of water vapor, eqn 38 (uses eqns 5 and 6)
  state.m_W = 0.621945 * pw / (pressure - pw);  // Humidity ratio, eqn 22
  state.m_psat = psat(drybulb);                 // Water vapor saturation pressure (uses eqns 5 and 6)
  //double Ws = 0.621945 * state.m_psat / (pressure - state.m_psat);
  state.m_phi = pw / state.m_psat;                                                    // Relative humidity, eqn 24
  state.m_h = 1.006 * drybulb + state.m_W * (2501 + 1.86 * drybulb);                  // Moist air specific enthalpy, eqn 32
  state.m_v = 0.287042 * (drybulb + 273.15) * (1 + 1.607858 * state.m_W) / pressure;  // Specific volume, eqn 28
  // Compute the wet bulb temperature here
  boost::optional<double> wetbulb = solveForWetBulb(drybulb, pressure, state.m_W, 1e-4, 100);
  if (!wetbulb) {
    return boost::none;
  }
  state.m_wetbulb = wetbulb.get();
  return boost::optional<AirState>(state);
}

boost::optional<AirState> AirState::fromDryBulbRelativeHumidityPressure(double drybulb, double RH, double pressure) {
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
  state.m_phi = 0.01 * RH;
  state.m_pressure = pressure;
  // Compute moist air properties, eqns from ASHRAE Fundamentals 2009 Ch. 1
  state.m_psat = psat(drybulb);                                                       // Water vapor saturation pressure (uses eqns 5 and 6)
  double pw = state.m_phi * state.m_psat;                                             // Relative humidity, eqn 24
  state.m_W = 0.621945 * pw / (pressure - pw);                                        // Humidity ratio, eqn 22
  state.m_h = 1.006 * drybulb + state.m_W * (2501 + 1.86 * drybulb);                  // Moist air specific enthalpy, eqn 32
  state.m_v = 0.287042 * (drybulb + 273.15) * (1 + 1.607858 * state.m_W) / pressure;  // Specific volume, eqn 28
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

double AirState::drybulb() const {
  return m_drybulb;
}

double AirState::dewpoint() const {
  return m_dewpoint;
}

double AirState::wetbulb() const {
  return m_wetbulb;
}

double AirState::relativeHumidity() const {
  return 100.0 * m_phi;
}

double AirState::pressure() const {
  return m_pressure;
}

double AirState::enthalpy() const {
  return m_h;
}

double AirState::saturationPressure() const {
  return m_psat;
}

double AirState::density() const {
  return 1.0 / m_v;
}

double AirState::specificVolume() const {
  return m_v;
}

double AirState::humidityRatio() const {
  return m_W;
}

double AirState::R() {
  return 8314.472 / 28.966;  // eqn 1 from ASHRAE Fundamentals 2009 Ch. 1
}

EpwDataPoint::EpwDataPoint()
  : m_year(1),
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
    m_liquidPrecipitationQuantity("99") {}

EpwDataPoint::EpwDataPoint(int year, int month, int day, int hour, int minute, const std::string& dataSourceandUncertaintyFlags,
                           double dryBulbTemperature, double dewPointTemperature, double relativeHumidity, double atmosphericStationPressure,
                           double extraterrestrialHorizontalRadiation, double extraterrestrialDirectNormalRadiation,
                           double horizontalInfraredRadiationIntensity, double globalHorizontalRadiation, double directNormalRadiation,
                           double diffuseHorizontalRadiation, double globalHorizontalIlluminance, double directNormalIlluminance,
                           double diffuseHorizontalIlluminance, double zenithLuminance, double windDirection, double windSpeed, int totalSkyCover,
                           int opaqueSkyCover, double visibility, double ceilingHeight, int presentWeatherObservation, int presentWeatherCodes,
                           double precipitableWater, double aerosolOpticalDepth, double snowDepth, double daysSinceLastSnowfall, double albedo,
                           double liquidPrecipitationDepth, double liquidPrecipitationQuantity) {
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

EpwDesignCondition::EpwDesignCondition()
  : m_titleOfDesignCondition(),
    m_heatingColdestMonth(),
    m_heatingDryBulb99pt6(),
    m_heatingDryBulb99(),
    m_heatingHumidificationDewPoint99pt6(),
    m_heatingHumidificationHumidityRatio99pt6(),
    m_heatingHumidificationMeanCoincidentDryBulb99pt6(),
    m_heatingHumidificationDewPoint99(),
    m_heatingHumidificationHumidityRatio99(),
    m_heatingHumidificationMeanCoincidentDryBulb99(),
    m_heatingColdestMonthWindSpeed0pt4(),
    m_heatingColdestMonthMeanCoincidentDryBulb0pt4(),
    m_heatingColdestMonthWindSpeed1(),
    m_heatingColdestMonthMeanCoincidentDryBulb1(),
    m_heatingMeanCoincidentWindSpeed99pt6(),
    m_heatingPrevailingCoincidentWindDirection99pt6(),
    m_coolingHottestMonth(),
    m_coolingDryBulbRange(),
    m_coolingDryBulb0pt4(),
    m_coolingMeanCoincidentWetBulb0pt4(),
    m_coolingDryBulb1(),
    m_coolingMeanCoincidentWetBulb1(),
    m_coolingDryBulb2(),
    m_coolingMeanCoincidentWetBulb2(),
    m_coolingEvaporationWetBulb0pt4(),
    m_coolingEvaporationMeanCoincidentDryBulb0pt4(),
    m_coolingEvaporationWetBulb1(),
    m_coolingEvaporationMeanCoincidentDryBulb1(),
    m_coolingEvaporationWetBulb2(),
    m_coolingEvaporationMeanCoincidentDryBulb2(),
    m_coolingMeanCoincidentWindSpeed0pt4(),
    m_coolingPrevailingCoincidentWindDirection0pt4(),
    m_coolingDehumidificationDewPoint0pt4(),
    m_coolingDehumidificationHumidityRatio0pt4(),
    m_coolingDehumidificationMeanCoincidentDryBulb0pt4(),
    m_coolingDehumidificationDewPoint1(),
    m_coolingDehumidificationHumidityRatio1(),
    m_coolingDehumidificationMeanCoincidentDryBulb1(),
    m_coolingDehumidificationDewPoint2(),
    m_coolingDehumidificationHumidityRatio2(),
    m_coolingDehumidificationMeanCoincidentDryBulb2(),
    m_coolingEnthalpy0pt4(),
    m_coolingEnthalpyMeanCoincidentDryBulb0pt4(),
    m_coolingEnthalpy1(),
    m_coolingEnthalpyMeanCoincidentDryBulb1(),
    m_coolingEnthalpy2(),
    m_coolingEnthalpyMeanCoincidentDryBulb2(),
    m_coolingHours8To4AndDryBulb12pt8To20pt6(),
    m_extremeWindSpeed1(),
    m_extremeWindSpeed2pt5(),
    m_extremeWindSpeed5(),
    m_extremeMaxWetBulb(),
    m_extremeMeanMinDryBulb(),
    m_extremeMeanMaxDryBulb(),
    m_extremeStdDevMinDryBulb(),
    m_extremeStdDevMaxDryBulb(),
    m_extremeN5YearsMinDryBulb(),
    m_extremeN5YearsMaxDryBulb(),
    m_extremeN10YearsMinDryBulb(),
    m_extremeN10YearsMaxDryBulb(),
    m_extremeN20YearsMinDryBulb(),
    m_extremeN20YearsMaxDryBulb(),
    m_extremeN50YearsMinDryBulb(),
    m_extremeN50YearsMaxDryBulb() {}

EpwDesignCondition::EpwDesignCondition(
  const std::string& titleOfDesignCondition, int heatingColdestMonth, double heatingDryBulb99pt6, double heatingDryBulb99,
  double heatingHumidificationDewPoint99pt6, double heatingHumidificationHumidityRatio99pt6, double heatingHumidificationMeanCoincidentDryBulb99pt6,
  double heatingHumidificationDewPoint99, double heatingHumidificationHumidityRatio99, double heatingHumidificationMeanCoincidentDryBulb99,
  double heatingColdestMonthWindSpeed0pt4, double heatingColdestMonthMeanCoincidentDryBulb0pt4, double heatingColdestMonthWindSpeed1,
  double heatingColdestMonthMeanCoincidentDryBulb1, double heatingMeanCoincidentWindSpeed99pt6, int heatingPrevailingCoincidentWindDirection99pt6,
  int coolingHottestMonth, double coolingDryBulbRange, double coolingDryBulb0pt4, double coolingMeanCoincidentWetBulb0pt4, double coolingDryBulb1,
  double coolingMeanCoincidentWetBulb1, double coolingDryBulb2, double coolingMeanCoincidentWetBulb2, double coolingEvaporationWetBulb0pt4,
  double coolingEvaporationMeanCoincidentDryBulb0pt4, double coolingEvaporationWetBulb1, double coolingEvaporationMeanCoincidentDryBulb1,
  double coolingEvaporationWetBulb2, double coolingEvaporationMeanCoincidentDryBulb2, double coolingMeanCoincidentWindSpeed0pt4,
  int coolingPrevailingCoincidentWindDirection0pt4, double coolingDehumidificationDewPoint0pt4, double coolingDehumidificationHumidityRatio0pt4,
  double coolingDehumidificationMeanCoincidentDryBulb0pt4, double coolingDehumidificationDewPoint1, double coolingDehumidificationHumidityRatio1,
  double coolingDehumidificationMeanCoincidentDryBulb1, double coolingDehumidificationDewPoint2, double coolingDehumidificationHumidityRatio2,
  double coolingDehumidificationMeanCoincidentDryBulb2, double coolingEnthalpy0pt4, double coolingEnthalpyMeanCoincidentDryBulb0pt4,
  double coolingEnthalpy1, double coolingEnthalpyMeanCoincidentDryBulb1, double coolingEnthalpy2, double coolingEnthalpyMeanCoincidentDryBulb2,
  int coolingHours8To4AndDryBulb12pt8To20pt6, double extremeWindSpeed1, double extremeWindSpeed2pt5, double extremeWindSpeed5,
  double extremeMaxWetBulb, double extremeMeanMinDryBulb, double extremeMeanMaxDryBulb, double extremeStdDevMinDryBulb,
  double extremeStdDevMaxDryBulb, double extremeN5YearsMinDryBulb, double extremeN5YearsMaxDryBulb, double extremeN10YearsMinDryBulb,
  double extremeN10YearsMaxDryBulb, double extremeN20YearsMinDryBulb, double extremeN20YearsMaxDryBulb, double extremeN50YearsMinDryBulb,
  double extremeN50YearsMaxDryBulb) {
  setTitleOfDesignCondition(titleOfDesignCondition);
  setHeatingColdestMonth(heatingColdestMonth);
  setHeatingDryBulb99pt6(heatingDryBulb99pt6);
  setHeatingDryBulb99(heatingDryBulb99);
  setHeatingHumidificationDewPoint99pt6(heatingHumidificationDewPoint99pt6);
  setHeatingHumidificationHumidityRatio99pt6(heatingHumidificationHumidityRatio99pt6);
  setHeatingHumidificationMeanCoincidentDryBulb99pt6(heatingHumidificationMeanCoincidentDryBulb99pt6);
  setHeatingHumidificationDewPoint99(heatingHumidificationDewPoint99);
  setHeatingHumidificationHumidityRatio99(heatingHumidificationHumidityRatio99);
  setHeatingHumidificationMeanCoincidentDryBulb99(heatingHumidificationMeanCoincidentDryBulb99);
  setHeatingColdestMonthWindSpeed0pt4(heatingColdestMonthWindSpeed0pt4);
  setHeatingColdestMonthMeanCoincidentDryBulb0pt4(heatingColdestMonthMeanCoincidentDryBulb0pt4);
  setHeatingColdestMonthWindSpeed1(heatingColdestMonthWindSpeed1);
  setHeatingColdestMonthMeanCoincidentDryBulb1(heatingColdestMonthMeanCoincidentDryBulb1);
  setHeatingMeanCoincidentWindSpeed99pt6(heatingMeanCoincidentWindSpeed99pt6);
  setHeatingPrevailingCoincidentWindDirection99pt6(heatingPrevailingCoincidentWindDirection99pt6);
  setCoolingHottestMonth(coolingHottestMonth);
  setCoolingDryBulbRange(coolingDryBulbRange);
  setCoolingDryBulb0pt4(coolingDryBulb0pt4);
  setCoolingMeanCoincidentWetBulb0pt4(coolingMeanCoincidentWetBulb0pt4);
  setCoolingDryBulb1(coolingDryBulb1);
  setCoolingMeanCoincidentWetBulb1(coolingMeanCoincidentWetBulb1);
  setCoolingDryBulb2(coolingDryBulb2);
  setCoolingMeanCoincidentWetBulb2(coolingMeanCoincidentWetBulb2);
  setCoolingEvaporationWetBulb0pt4(coolingEvaporationWetBulb0pt4);
  setCoolingEvaporationMeanCoincidentDryBulb0pt4(coolingEvaporationMeanCoincidentDryBulb0pt4);
  setCoolingEvaporationWetBulb1(coolingEvaporationWetBulb1);
  setCoolingEvaporationMeanCoincidentDryBulb1(coolingEvaporationMeanCoincidentDryBulb1);
  setCoolingEvaporationWetBulb2(coolingEvaporationWetBulb2);
  setCoolingEvaporationMeanCoincidentDryBulb2(coolingEvaporationMeanCoincidentDryBulb2);
  setCoolingMeanCoincidentWindSpeed0pt4(coolingMeanCoincidentWindSpeed0pt4);
  setCoolingPrevailingCoincidentWindDirection0pt4(coolingPrevailingCoincidentWindDirection0pt4);
  setCoolingDehumidificationDewPoint0pt4(coolingDehumidificationDewPoint0pt4);
  setCoolingDehumidificationHumidityRatio0pt4(coolingDehumidificationHumidityRatio0pt4);
  setCoolingDehumidificationMeanCoincidentDryBulb0pt4(coolingDehumidificationMeanCoincidentDryBulb0pt4);
  setCoolingDehumidificationDewPoint1(coolingDehumidificationDewPoint1);
  setCoolingDehumidificationHumidityRatio1(coolingDehumidificationHumidityRatio1);
  setCoolingDehumidificationMeanCoincidentDryBulb1(coolingDehumidificationMeanCoincidentDryBulb1);
  setCoolingDehumidificationDewPoint2(coolingDehumidificationDewPoint2);
  setCoolingDehumidificationHumidityRatio2(coolingDehumidificationHumidityRatio2);
  setCoolingDehumidificationMeanCoincidentDryBulb2(coolingDehumidificationMeanCoincidentDryBulb2);
  setCoolingEnthalpy0pt4(coolingEnthalpy0pt4);
  setCoolingEnthalpyMeanCoincidentDryBulb0pt4(coolingEnthalpyMeanCoincidentDryBulb0pt4);
  setCoolingEnthalpy1(coolingEnthalpy1);
  setCoolingEnthalpyMeanCoincidentDryBulb1(coolingEnthalpyMeanCoincidentDryBulb1);
  setCoolingEnthalpy2(coolingEnthalpy2);
  setCoolingEnthalpyMeanCoincidentDryBulb2(coolingEnthalpyMeanCoincidentDryBulb2);
  setCoolingHours8To4AndDryBulb12pt8To20pt6(coolingHours8To4AndDryBulb12pt8To20pt6);
  setExtremeWindSpeed1(extremeWindSpeed1);
  setExtremeWindSpeed2pt5(extremeWindSpeed2pt5);
  setExtremeWindSpeed5(extremeWindSpeed5);
  setExtremeMaxWetBulb(extremeMaxWetBulb);
  setExtremeMeanMinDryBulb(extremeMeanMinDryBulb);
  setExtremeMeanMaxDryBulb(extremeMeanMaxDryBulb);
  setExtremeStdDevMinDryBulb(extremeStdDevMinDryBulb);
  setExtremeStdDevMaxDryBulb(extremeStdDevMaxDryBulb);
  setExtremeN5YearsMinDryBulb(extremeN5YearsMinDryBulb);
  setExtremeN5YearsMaxDryBulb(extremeN5YearsMaxDryBulb);
  setExtremeN10YearsMinDryBulb(extremeN10YearsMinDryBulb);
  setExtremeN10YearsMaxDryBulb(extremeN10YearsMaxDryBulb);
  setExtremeN20YearsMinDryBulb(extremeN20YearsMinDryBulb);
  setExtremeN20YearsMaxDryBulb(extremeN20YearsMaxDryBulb);
  setExtremeN50YearsMinDryBulb(extremeN50YearsMinDryBulb);
  setExtremeN50YearsMaxDryBulb(extremeN50YearsMaxDryBulb);
}

boost::optional<EpwDataPoint> EpwDataPoint::fromEpwString(const std::string& line) {
  std::vector<std::string> list = splitString(line, ',');
  return fromEpwStrings(list);
}

boost::optional<EpwDataPoint> EpwDataPoint::fromEpwStrings(const std::vector<std::string>& list, bool pedantic) {
  EpwDataPoint pt;
  // Expect 35 items in the list
  if (list.size() < 35) {
    if (pedantic) {
      LOG_FREE(Error, "openstudio.EpwFile", "Expected 35 fields in EPW data instead of the " << list.size() << " received");
      return boost::none;
    } else {
      LOG_FREE(Warn, "openstudio.EpwFile",
               "Expected 35 fields in EPW data instead of the " << list.size() << " received. The remaining fields will not be available");
    }
  } else if (list.size() > 35) {
    LOG_FREE(Warn, "openstudio.EpwFile",
             "Expected 35 fields in EPW data instead of the " << list.size() << " received. The additional data will be ignored");
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
  if (!pt.setMinute(list[EpwDataField::Minute])) {
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

boost::optional<EpwDataPoint> EpwDataPoint::fromEpwStrings(int year, int month, int day, int hour, int minute, const std::vector<std::string>& list,
                                                           bool pedantic) {
  EpwDataPoint pt;
  // Expect 30 items in the list
  if (list.size() < 35) {
    if (pedantic) {
      LOG_FREE(Error, "openstudio.EpwFile", "Expected 35 fields in EPW data instead of the " << list.size() << " received");
      return boost::none;
    } else {
      LOG_FREE(Warn, "openstudio.EpwFile",
               "Expected 35 fields in EPW data instead of the " << list.size() << " received. The remaining fields will not be available");
    }
  } else if (list.size() > 35) {
    LOG_FREE(Warn, "openstudio.EpwFile",
             "Expected 35 fields in EPW data instead of the " << list.size() << " received. The additional data will be ignored");
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

boost::optional<EpwDesignCondition> EpwDesignCondition::fromDesignConditionsString(const std::string& line) {
  std::vector<std::string> list = splitString(line, ',');
  return fromDesignConditionsStrings(list);
}

boost::optional<EpwDesignCondition> EpwDesignCondition::fromDesignConditionsStrings(const std::vector<std::string>& list) {
  EpwDesignCondition dc;
  // Expect 68 items in the list
  if (list.size() < 68) {
    LOG_FREE(Error, "openstudio.EpwFile", "Expected 68 fields in EPW design condition instead of the " << list.size() << " received");
    return boost::none;
  }
  // Use the appropriate setter on each field
  dc.setTitleOfDesignCondition(list[EpwDesignField::TitleOfDesignCondition]);
  dc.setHeatingColdestMonth(list[EpwDesignField::HeatingColdestMonth]);
  dc.setHeatingDryBulb99pt6(list[EpwDesignField::HeatingDryBulb99pt6]);
  dc.setHeatingDryBulb99(list[EpwDesignField::HeatingDryBulb99]);
  dc.setHeatingHumidificationDewPoint99pt6(list[EpwDesignField::HeatingHumidificationDewPoint99pt6]);
  dc.setHeatingHumidificationHumidityRatio99pt6(list[EpwDesignField::HeatingHumidificationHumidityRatio99pt6]);
  dc.setHeatingHumidificationMeanCoincidentDryBulb99pt6(list[EpwDesignField::HeatingHumidificationMeanCoincidentDryBulb99pt6]);
  dc.setHeatingHumidificationDewPoint99(list[EpwDesignField::HeatingHumidificationDewPoint99]);
  dc.setHeatingHumidificationHumidityRatio99(list[EpwDesignField::HeatingHumidificationHumidityRatio99]);
  dc.setHeatingHumidificationMeanCoincidentDryBulb99(list[EpwDesignField::HeatingHumidificationMeanCoincidentDryBulb99]);
  dc.setHeatingColdestMonthWindSpeed0pt4(list[EpwDesignField::HeatingColdestMonthWindSpeed0pt4]);
  dc.setHeatingColdestMonthMeanCoincidentDryBulb0pt4(list[EpwDesignField::HeatingColdestMonthMeanCoincidentDryBulb0pt4]);
  dc.setHeatingColdestMonthWindSpeed1(list[EpwDesignField::HeatingColdestMonthWindSpeed1]);
  dc.setHeatingColdestMonthMeanCoincidentDryBulb1(list[EpwDesignField::HeatingColdestMonthMeanCoincidentDryBulb1]);
  dc.setHeatingMeanCoincidentWindSpeed99pt6(list[EpwDesignField::HeatingMeanCoincidentWindSpeed99pt6]);
  dc.setHeatingPrevailingCoincidentWindDirection99pt6(list[EpwDesignField::HeatingPrevailingCoincidentWindDirection99pt6]);
  dc.setCoolingHottestMonth(list[EpwDesignField::CoolingHottestMonth]);
  dc.setCoolingDryBulbRange(list[EpwDesignField::CoolingDryBulbRange]);
  dc.setCoolingDryBulb0pt4(list[EpwDesignField::CoolingDryBulb0pt4]);
  dc.setCoolingMeanCoincidentWetBulb0pt4(list[EpwDesignField::CoolingMeanCoincidentWetBulb0pt4]);
  dc.setCoolingDryBulb1(list[EpwDesignField::CoolingDryBulb1]);
  dc.setCoolingMeanCoincidentWetBulb1(list[EpwDesignField::CoolingMeanCoincidentWetBulb1]);
  dc.setCoolingDryBulb2(list[EpwDesignField::CoolingDryBulb2]);
  dc.setCoolingMeanCoincidentWetBulb2(list[EpwDesignField::CoolingMeanCoincidentWetBulb2]);
  dc.setCoolingEvaporationWetBulb0pt4(list[EpwDesignField::CoolingEvaporationWetBulb0pt4]);
  dc.setCoolingEvaporationMeanCoincidentDryBulb0pt4(list[EpwDesignField::CoolingEvaporationMeanCoincidentDryBulb0pt4]);
  dc.setCoolingEvaporationWetBulb1(list[EpwDesignField::CoolingEvaporationWetBulb1]);
  dc.setCoolingEvaporationMeanCoincidentDryBulb1(list[EpwDesignField::CoolingEvaporationMeanCoincidentDryBulb1]);
  dc.setCoolingEvaporationWetBulb2(list[EpwDesignField::CoolingEvaporationWetBulb2]);
  dc.setCoolingEvaporationMeanCoincidentDryBulb2(list[EpwDesignField::CoolingEvaporationMeanCoincidentDryBulb2]);
  dc.setCoolingMeanCoincidentWindSpeed0pt4(list[EpwDesignField::CoolingMeanCoincidentWindSpeed0pt4]);
  dc.setCoolingPrevailingCoincidentWindDirection0pt4(list[EpwDesignField::CoolingPrevailingCoincidentWindDirection0pt4]);
  dc.setCoolingDehumidificationDewPoint0pt4(list[EpwDesignField::CoolingDehumidificationDewPoint0pt4]);
  dc.setCoolingDehumidificationHumidityRatio0pt4(list[EpwDesignField::CoolingDehumidificationHumidityRatio0pt4]);
  dc.setCoolingDehumidificationMeanCoincidentDryBulb0pt4(list[EpwDesignField::CoolingDehumidificationMeanCoincidentDryBulb0pt4]);
  dc.setCoolingDehumidificationDewPoint1(list[EpwDesignField::CoolingDehumidificationDewPoint1]);
  dc.setCoolingDehumidificationHumidityRatio1(list[EpwDesignField::CoolingDehumidificationHumidityRatio1]);
  dc.setCoolingDehumidificationMeanCoincidentDryBulb1(list[EpwDesignField::CoolingDehumidificationMeanCoincidentDryBulb1]);
  dc.setCoolingDehumidificationDewPoint2(list[EpwDesignField::CoolingDehumidificationDewPoint2]);
  dc.setCoolingDehumidificationHumidityRatio2(list[EpwDesignField::CoolingDehumidificationHumidityRatio2]);
  dc.setCoolingDehumidificationMeanCoincidentDryBulb2(list[EpwDesignField::CoolingDehumidificationMeanCoincidentDryBulb2]);
  dc.setCoolingEnthalpy0pt4(list[EpwDesignField::CoolingEnthalpy0pt4]);
  dc.setCoolingEnthalpyMeanCoincidentDryBulb0pt4(list[EpwDesignField::CoolingEnthalpyMeanCoincidentDryBulb0pt4]);
  dc.setCoolingEnthalpy1(list[EpwDesignField::CoolingEnthalpy1]);
  dc.setCoolingEnthalpyMeanCoincidentDryBulb1(list[EpwDesignField::CoolingEnthalpyMeanCoincidentDryBulb1]);
  dc.setCoolingEnthalpy2(list[EpwDesignField::CoolingEnthalpy2]);
  dc.setCoolingEnthalpyMeanCoincidentDryBulb2(list[EpwDesignField::CoolingEnthalpyMeanCoincidentDryBulb2]);
  dc.setCoolingHours8To4AndDryBulb12pt8To20pt6(list[EpwDesignField::CoolingHours8To4AndDryBulb12pt8To20pt6]);
  dc.setExtremeWindSpeed1(list[EpwDesignField::ExtremeWindSpeed1]);
  dc.setExtremeWindSpeed2pt5(list[EpwDesignField::ExtremeWindSpeed2pt5]);
  dc.setExtremeWindSpeed5(list[EpwDesignField::ExtremeWindSpeed5]);
  dc.setExtremeMaxWetBulb(list[EpwDesignField::ExtremeMaxWetBulb]);
  dc.setExtremeMeanMinDryBulb(list[EpwDesignField::ExtremeMeanMinDryBulb]);
  dc.setExtremeMeanMaxDryBulb(list[EpwDesignField::ExtremeMeanMaxDryBulb]);
  dc.setExtremeStdDevMinDryBulb(list[EpwDesignField::ExtremeStdDevMinDryBulb]);
  dc.setExtremeStdDevMaxDryBulb(list[EpwDesignField::ExtremeStdDevMaxDryBulb]);
  dc.setExtremeN5YearsMinDryBulb(list[EpwDesignField::ExtremeN5YearsMinDryBulb]);
  dc.setExtremeN5YearsMaxDryBulb(list[EpwDesignField::ExtremeN5YearsMaxDryBulb]);
  dc.setExtremeN10YearsMinDryBulb(list[EpwDesignField::ExtremeN10YearsMinDryBulb]);
  dc.setExtremeN10YearsMaxDryBulb(list[EpwDesignField::ExtremeN10YearsMaxDryBulb]);
  dc.setExtremeN20YearsMinDryBulb(list[EpwDesignField::ExtremeN20YearsMinDryBulb]);
  dc.setExtremeN20YearsMaxDryBulb(list[EpwDesignField::ExtremeN20YearsMaxDryBulb]);
  dc.setExtremeN50YearsMinDryBulb(list[EpwDesignField::ExtremeN50YearsMinDryBulb]);
  dc.setExtremeN50YearsMaxDryBulb(list[EpwDesignField::ExtremeN50YearsMaxDryBulb]);
  return boost::optional<EpwDesignCondition>(dc);
}

std::vector<std::string> EpwDataPoint::toEpwStrings() const {
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

boost::optional<std::string> EpwDataPoint::getUnitsByName(const std::string& name) {
  EpwDataField id;
  try {
    id = EpwDataField(name);
  } catch (...) {
    // Could do a warning message here
    return boost::none;
  }
  return boost::optional<std::string>(getUnits(id));
}

std::string EpwDataPoint::getUnits(EpwDataField field) {
  std::string string;
  switch (field.value()) {
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

boost::optional<std::string> EpwDesignCondition::getUnitsByName(const std::string& name) {
  EpwDesignField id;
  try {
    id = EpwDesignField(name);
  } catch (...) {
    // Could do a warning message here
    return boost::none;
  }
  return getUnits(id);
}

std::string EpwDesignCondition::getUnits(EpwDesignField field) {
  std::string string;
  switch (field.value()) {
    case EpwDesignField::TitleOfDesignCondition:
      //string = "None";
      break;
    case EpwDesignField::HeatingColdestMonth:
      //string = "None";
      break;
    case EpwDesignField::HeatingDryBulb99pt6:
      string = "C";
      break;
    case EpwDesignField::HeatingDryBulb99:
      string = "C";
      break;
    case EpwDesignField::HeatingHumidificationDewPoint99pt6:
      string = "C";
      break;
    case EpwDesignField::HeatingHumidificationHumidityRatio99pt6:
      string = "g/kg";
      break;
    case EpwDesignField::HeatingHumidificationMeanCoincidentDryBulb99pt6:
      string = "C";
      break;
    case EpwDesignField::HeatingHumidificationDewPoint99:
      string = "C";
      break;
    case EpwDesignField::HeatingHumidificationHumidityRatio99:
      string = "g/kg";
      break;
    case EpwDesignField::HeatingHumidificationMeanCoincidentDryBulb99:
      string = "C";
      break;
    case EpwDesignField::HeatingColdestMonthWindSpeed0pt4:
      string = "m/s";
      break;
    case EpwDesignField::HeatingColdestMonthMeanCoincidentDryBulb0pt4:
      string = "C";
      break;
    case EpwDesignField::HeatingColdestMonthWindSpeed1:
      string = "m/s";
      break;
    case EpwDesignField::HeatingColdestMonthMeanCoincidentDryBulb1:
      string = "C";
      break;
    case EpwDesignField::HeatingMeanCoincidentWindSpeed99pt6:
      string = "m/s";
      break;
    case EpwDesignField::HeatingPrevailingCoincidentWindDirection99pt6:
      string = "degrees";
      break;
    case EpwDesignField::CoolingHottestMonth:
      //string = "None";
      break;
    case EpwDesignField::CoolingDryBulbRange:
      string = "C";
      break;
    case EpwDesignField::CoolingDryBulb0pt4:
      string = "C";
      break;
    case EpwDesignField::CoolingMeanCoincidentWetBulb0pt4:
      string = "C";
      break;
    case EpwDesignField::CoolingDryBulb1:
      string = "C";
      break;
    case EpwDesignField::CoolingMeanCoincidentWetBulb1:
      string = "C";
      break;
    case EpwDesignField::CoolingDryBulb2:
      string = "C";
      break;
    case EpwDesignField::CoolingMeanCoincidentWetBulb2:
      string = "C";
      break;
    case EpwDesignField::CoolingEvaporationWetBulb0pt4:
      string = "C";
      break;
    case EpwDesignField::CoolingEvaporationMeanCoincidentDryBulb0pt4:
      string = "C";
      break;
    case EpwDesignField::CoolingEvaporationWetBulb1:
      string = "C";
      break;
    case EpwDesignField::CoolingEvaporationMeanCoincidentDryBulb1:
      string = "C";
      break;
    case EpwDesignField::CoolingEvaporationWetBulb2:
      string = "C";
      break;
    case EpwDesignField::CoolingEvaporationMeanCoincidentDryBulb2:
      string = "C";
      break;
    case EpwDesignField::CoolingMeanCoincidentWindSpeed0pt4:
      string = "m/s";
      break;
    case EpwDesignField::CoolingPrevailingCoincidentWindDirection0pt4:
      string = "degrees";
      break;
    case EpwDesignField::CoolingDehumidificationDewPoint0pt4:
      string = "C";
      break;
    case EpwDesignField::CoolingDehumidificationHumidityRatio0pt4:
      string = "g/kg";
      break;
    case EpwDesignField::CoolingDehumidificationMeanCoincidentDryBulb0pt4:
      string = "C";
      break;
    case EpwDesignField::CoolingDehumidificationDewPoint1:
      string = "C";
      break;
    case EpwDesignField::CoolingDehumidificationHumidityRatio1:
      string = "g/kg";
      break;
    case EpwDesignField::CoolingDehumidificationMeanCoincidentDryBulb1:
      string = "C";
      break;
    case EpwDesignField::CoolingDehumidificationDewPoint2:
      string = "C";
      break;
    case EpwDesignField::CoolingDehumidificationHumidityRatio2:
      string = "g/kg";
      break;
    case EpwDesignField::CoolingDehumidificationMeanCoincidentDryBulb2:
      string = "C";
      break;
    case EpwDesignField::CoolingEnthalpy0pt4:
      string = "kJ/kg";
      break;
    case EpwDesignField::CoolingEnthalpyMeanCoincidentDryBulb0pt4:
      string = "C";
      break;
    case EpwDesignField::CoolingEnthalpy1:
      string = "kJ/kg";
      break;
    case EpwDesignField::CoolingEnthalpyMeanCoincidentDryBulb1:
      string = "C";
      break;
    case EpwDesignField::CoolingEnthalpy2:
      string = "kJ/kg";
      break;
    case EpwDesignField::CoolingEnthalpyMeanCoincidentDryBulb2:
      string = "C";
      break;
    case EpwDesignField::CoolingHours8To4AndDryBulb12pt8To20pt6:
      string = "hr";
      break;
    case EpwDesignField::ExtremeWindSpeed1:
      string = "m/s";
      break;
    case EpwDesignField::ExtremeWindSpeed2pt5:
      string = "m/s";
      break;
    case EpwDesignField::ExtremeWindSpeed5:
      string = "m/s";
      break;
    case EpwDesignField::ExtremeMaxWetBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeMeanMinDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeMeanMaxDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeStdDevMinDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeStdDevMaxDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeN5YearsMinDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeN5YearsMaxDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeN10YearsMinDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeN10YearsMaxDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeN20YearsMinDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeN20YearsMaxDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeN50YearsMinDryBulb:
      string = "C";
      break;
    case EpwDesignField::ExtremeN50YearsMaxDryBulb:
      string = "C";
      break;
    default:
      //string = "None";
      break;
  }
  return string;
}

std::string EpwDataPoint::getUnits(EpwComputedField field) {
  std::string string;
  switch (field.value()) {
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

boost::optional<double> EpwDataPoint::getFieldByName(const std::string& name) {
  EpwDataField id;
  try {
    id = EpwDataField(name);
  } catch (...) {
    // Could do a warning message here
    return boost::none;
  }
  return getField(id);
}

boost::optional<double> EpwDataPoint::getField(EpwDataField id) {
  boost::optional<int> ivalue;
  switch (id.value()) {
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
      if (ivalue) {
        return boost::optional<double>((double)ivalue.get());
      }
      break;
    case EpwDataField::OpaqueSkyCover:
      ivalue = opaqueSkyCover();
      if (ivalue) {
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
      if (ivalue) {
        return boost::optional<double>((double)ivalue.get());
      }
      break;
    case EpwDataField::PresentWeatherCodes:
      ivalue = presentWeatherCodes();
      if (ivalue) {
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

boost::optional<double> EpwDesignCondition::getFieldByName(const std::string& name) {
  EpwDesignField id;
  try {
    id = EpwDesignField(name);
  } catch (...) {
    // Could do a warning message here
    return boost::none;
  }
  return getField(id);
}

boost::optional<double> EpwDesignCondition::getField(EpwDesignField id) {
  auto optionaIntToOptionalDouble = [](boost::optional<int> val_) -> boost::optional<double> {
    if (val_) {
      return *val_;
    }
    return boost::none;
  };

  switch (id.value()) {
    case EpwDesignField::HeatingColdestMonth:
      return optionaIntToOptionalDouble(heatingColdestMonth());
      break;
    case EpwDesignField::HeatingDryBulb99pt6:
      return heatingDryBulb99pt6();
      break;
    case EpwDesignField::HeatingDryBulb99:
      return heatingDryBulb99();
      break;
    case EpwDesignField::HeatingHumidificationDewPoint99pt6:
      return heatingHumidificationDewPoint99pt6();
      break;
    case EpwDesignField::HeatingHumidificationHumidityRatio99pt6:
      return heatingHumidificationHumidityRatio99pt6();
      break;
    case EpwDesignField::HeatingHumidificationMeanCoincidentDryBulb99pt6:
      return heatingHumidificationMeanCoincidentDryBulb99pt6();
      break;
    case EpwDesignField::HeatingHumidificationDewPoint99:
      return heatingHumidificationDewPoint99();
      break;
    case EpwDesignField::HeatingHumidificationHumidityRatio99:
      return heatingHumidificationHumidityRatio99();
      break;
    case EpwDesignField::HeatingHumidificationMeanCoincidentDryBulb99:
      return heatingHumidificationMeanCoincidentDryBulb99();
      break;
    case EpwDesignField::HeatingColdestMonthWindSpeed0pt4:
      return heatingColdestMonthWindSpeed0pt4();
      break;
    case EpwDesignField::HeatingColdestMonthMeanCoincidentDryBulb0pt4:
      return heatingColdestMonthMeanCoincidentDryBulb0pt4();
      break;
    case EpwDesignField::HeatingColdestMonthWindSpeed1:
      return heatingColdestMonthWindSpeed1();
      break;
    case EpwDesignField::HeatingColdestMonthMeanCoincidentDryBulb1:
      return heatingColdestMonthMeanCoincidentDryBulb1();
      break;
    case EpwDesignField::HeatingMeanCoincidentWindSpeed99pt6:
      return heatingMeanCoincidentWindSpeed99pt6();
      break;
    case EpwDesignField::HeatingPrevailingCoincidentWindDirection99pt6:
      return optionaIntToOptionalDouble(heatingPrevailingCoincidentWindDirection99pt6());
      break;
    case EpwDesignField::CoolingHottestMonth:
      return optionaIntToOptionalDouble(coolingHottestMonth());
      break;
    case EpwDesignField::CoolingDryBulbRange:
      return coolingDryBulbRange();
      break;
    case EpwDesignField::CoolingDryBulb0pt4:
      return coolingDryBulb0pt4();
      break;
    case EpwDesignField::CoolingMeanCoincidentWetBulb0pt4:
      return coolingMeanCoincidentWetBulb0pt4();
      break;
    case EpwDesignField::CoolingDryBulb1:
      return coolingDryBulb1();
      break;
    case EpwDesignField::CoolingMeanCoincidentWetBulb1:
      return coolingMeanCoincidentWetBulb1();
      break;
    case EpwDesignField::CoolingDryBulb2:
      return coolingDryBulb2();
      break;
    case EpwDesignField::CoolingMeanCoincidentWetBulb2:
      return coolingMeanCoincidentWetBulb2();
      break;
    case EpwDesignField::CoolingEvaporationWetBulb0pt4:
      return coolingEvaporationWetBulb0pt4();
      break;
    case EpwDesignField::CoolingEvaporationMeanCoincidentDryBulb0pt4:
      return coolingEvaporationMeanCoincidentDryBulb0pt4();
      break;
    case EpwDesignField::CoolingEvaporationWetBulb1:
      return coolingEvaporationWetBulb1();
      break;
    case EpwDesignField::CoolingEvaporationMeanCoincidentDryBulb1:
      return coolingEvaporationMeanCoincidentDryBulb1();
      break;
    case EpwDesignField::CoolingEvaporationWetBulb2:
      return coolingEvaporationWetBulb2();
      break;
    case EpwDesignField::CoolingEvaporationMeanCoincidentDryBulb2:
      return coolingEvaporationMeanCoincidentDryBulb2();
      break;
    case EpwDesignField::CoolingMeanCoincidentWindSpeed0pt4:
      return coolingMeanCoincidentWindSpeed0pt4();
      break;
    case EpwDesignField::CoolingPrevailingCoincidentWindDirection0pt4:
      return optionaIntToOptionalDouble(coolingPrevailingCoincidentWindDirection0pt4());
      break;
    case EpwDesignField::CoolingDehumidificationDewPoint0pt4:
      return coolingDehumidificationDewPoint0pt4();
      break;
    case EpwDesignField::CoolingDehumidificationHumidityRatio0pt4:
      return coolingDehumidificationHumidityRatio0pt4();
      break;
    case EpwDesignField::CoolingDehumidificationMeanCoincidentDryBulb0pt4:
      return coolingDehumidificationMeanCoincidentDryBulb0pt4();
      break;
    case EpwDesignField::CoolingDehumidificationDewPoint1:
      return coolingDehumidificationDewPoint1();
      break;
    case EpwDesignField::CoolingDehumidificationHumidityRatio1:
      return coolingDehumidificationHumidityRatio1();
      break;
    case EpwDesignField::CoolingDehumidificationMeanCoincidentDryBulb1:
      return coolingDehumidificationMeanCoincidentDryBulb1();
      break;
    case EpwDesignField::CoolingDehumidificationDewPoint2:
      return coolingDehumidificationDewPoint2();
      break;
    case EpwDesignField::CoolingDehumidificationHumidityRatio2:
      return coolingDehumidificationHumidityRatio2();
      break;
    case EpwDesignField::CoolingDehumidificationMeanCoincidentDryBulb2:
      return coolingDehumidificationMeanCoincidentDryBulb2();
      break;
    case EpwDesignField::CoolingEnthalpy0pt4:
      return coolingEnthalpy0pt4();
      break;
    case EpwDesignField::CoolingEnthalpyMeanCoincidentDryBulb0pt4:
      return coolingEnthalpyMeanCoincidentDryBulb0pt4();
      break;
    case EpwDesignField::CoolingEnthalpy1:
      return coolingEnthalpy1();
      break;
    case EpwDesignField::CoolingEnthalpyMeanCoincidentDryBulb1:
      return coolingEnthalpyMeanCoincidentDryBulb1();
      break;
    case EpwDesignField::CoolingEnthalpy2:
      return coolingEnthalpy2();
      break;
    case EpwDesignField::CoolingEnthalpyMeanCoincidentDryBulb2:
      return coolingEnthalpyMeanCoincidentDryBulb2();
      break;
    case EpwDesignField::CoolingHours8To4AndDryBulb12pt8To20pt6:
      return optionaIntToOptionalDouble(coolingHours8To4AndDryBulb12pt8To20pt6());
      break;
    case EpwDesignField::ExtremeWindSpeed1:
      return extremeWindSpeed1();
      break;
    case EpwDesignField::ExtremeWindSpeed2pt5:
      return extremeWindSpeed2pt5();
      break;
    case EpwDesignField::ExtremeWindSpeed5:
      return extremeWindSpeed5();
      break;
    case EpwDesignField::ExtremeMaxWetBulb:
      return extremeMaxWetBulb();
      break;
    case EpwDesignField::ExtremeMeanMinDryBulb:
      return extremeMeanMinDryBulb();
      break;
    case EpwDesignField::ExtremeMeanMaxDryBulb:
      return extremeMeanMaxDryBulb();
      break;
    case EpwDesignField::ExtremeStdDevMinDryBulb:
      return extremeStdDevMinDryBulb();
      break;
    case EpwDesignField::ExtremeStdDevMaxDryBulb:
      return extremeStdDevMaxDryBulb();
      break;
    case EpwDesignField::ExtremeN5YearsMinDryBulb:
      return extremeN5YearsMinDryBulb();
      break;
    case EpwDesignField::ExtremeN5YearsMaxDryBulb:
      return extremeN5YearsMaxDryBulb();
      break;
    case EpwDesignField::ExtremeN10YearsMinDryBulb:
      return extremeN10YearsMinDryBulb();
      break;
    case EpwDesignField::ExtremeN10YearsMaxDryBulb:
      return extremeN10YearsMaxDryBulb();
      break;
    case EpwDesignField::ExtremeN20YearsMinDryBulb:
      return extremeN20YearsMinDryBulb();
      break;
    case EpwDesignField::ExtremeN20YearsMaxDryBulb:
      return extremeN20YearsMaxDryBulb();
      break;
    case EpwDesignField::ExtremeN50YearsMinDryBulb:
      return extremeN50YearsMinDryBulb();
      break;
    case EpwDesignField::ExtremeN50YearsMaxDryBulb:
      return extremeN50YearsMaxDryBulb();
      break;
    default:
      // Could do a warning message here
      return boost::none;
      break;
  }
  return boost::none;
}

boost::optional<std::string> EpwDataPoint::toWthString() const {
  std::string date = fmt::format("{}/{}", m_month, m_day);
  std::string string = date;
  std::string hms = fmt::format("{:02d}:{:02d}:00", m_hour, m_minute);
  string += '\t' + hms;
  boost::optional<double> value = dryBulbTemperature();
  if (!value) {
    LOG_FREE(Error, "openstudio.EpwFile", "Missing dry bulb temperature on " << date << " at " << hms)
    return boost::none;
  }
  //boost::optional<double> optdrybulb = openstudio::convert(value.get(), "C", "K");
  //OS_ASSERT(optdrybulb);
  double drybulb = value.get();
  string += '\t' + std::to_string(drybulb + 273.15);
  value = atmosphericStationPressure();
  if (!value) {
    LOG_FREE(Error, "openstudio.EpwFile", "Missing atmospheric station pressure on " << date << " at " << hms);
    return boost::none;
  }
  double p = value.get();
  string += '\t' + m_atmosphericStationPressure;
  if (!windSpeed()) {
    LOG_FREE(Error, "openstudio.EpwFile", "Missing wind speed on " << date << " at " << hms);
    return boost::none;
  }
  string += '\t' + m_windSpeed;
  if (!windDirection()) {
    LOG_FREE(Error, "openstudio.EpwFile", "Missing wind direction on " << date << " at " << hms);
    return boost::none;
  }
  string += '\t' + m_windDirection;
  double pw;
  value = relativeHumidity();
  if (!value) {  // Don't have relative humidity - this has not been tested
    value = dewPointTemperature();
    if (!value) {
      LOG_FREE(Error, "openstudio.EpwFile", "Cannot compute humidity ratio on " << date << " at " << hms);
      return boost::none;
    }
    //boost::optional<double> optdewpoint = openstudio::convert(value.get(), "C", "K");
    //OS_ASSERT(optdewpoint);
    double dewpoint = value.get();
    pw = openstudio::psat(dewpoint);
  } else {  // Have relative humidity
    double pws = openstudio::psat(drybulb);
    pw = 0.01 * value.get() * pws;
  }
  double W = 0.621945 * pw / (p - pw);
  string += "\t" + std::to_string(W * 1000);  // need g/kg
  // Pass on solar flux quantities
  string += "\t0\t0";
  // Pass on Tsky
  string += "\t0";
  // Pass on snow and rain
  string += "\t0\t0";
  return boost::optional<std::string>(string);
}

// Local convenience functions
static int stringToInteger(const std::string& string, bool* ok) {
  int value = 0;
  *ok = true;
  try {
    value = std::stoi(string);
  } catch (const std::invalid_argument&) {
    *ok = false;
  } catch (const std::out_of_range&) {
    *ok = false;
  }
  return value;
}

static double stringToDouble(const std::string& string, bool* ok) {
  double value = 0;
  *ok = true;
  try {
    value = std::stod(string);
  } catch (const std::invalid_argument&) {
    *ok = false;
  } catch (const std::out_of_range&) {
    *ok = false;
  }
  return value;
}

Date EpwDataPoint::date() const {
  return {MonthOfYear(m_month), static_cast<unsigned int>(m_day), m_year};
}

void EpwDataPoint::setDate(Date date) {
  m_month = openstudio::month(date.monthOfYear());
  m_day = date.dayOfMonth();
  m_year = date.year();
}

Time EpwDataPoint::time() const {
  return {0, m_hour, m_minute};
}

void EpwDataPoint::setTime(Time time) {
  m_hour = time.hours();
  m_minute = time.minutes();
}

DateTime EpwDataPoint::dateTime() const {
  return DateTime(date(), time());
}

void EpwDataPoint::setDateTime(DateTime dateTime) {
  setDate(dateTime.date());
  setTime(dateTime.time());
}

int EpwDataPoint::year() const {
  return m_year;
}

void EpwDataPoint::setYear(int year) {
  m_year = year;
}

bool EpwDataPoint::setYear(const std::string& year) {
  bool ok;
  int value = stringToInteger(year, &ok);
  if (!ok) {
    return false;
  }
  m_year = value;
  return true;
}

int EpwDataPoint::month() const {
  return m_month;
}

bool EpwDataPoint::setMonth(int month) {
  if (1 > month || 12 < month) {
    LOG_FREE(Error, "openstudio.EpwFile", "Month value " << month << " out of range");
    return false;
  }
  m_month = month;
  return true;
}

bool EpwDataPoint::setMonth(const std::string& month) {
  bool ok;
  int value = stringToInteger(month, &ok);
  if (!ok) {
    LOG_FREE(Error, "openstudio.EpwFile", "Month value '" << month << "' cannot be converted into an integer");
    return false;
  }
  return setMonth(value);
}

int EpwDataPoint::day() const {
  return m_day;
}

bool EpwDataPoint::setDay(int day) {
  if (1 > day || 31 < day) {
    LOG_FREE(Error, "openstudio.EpwFile", "Day value " << day << " out of range");
    return false;
  }
  m_day = day;
  return true;
}

bool EpwDataPoint::setDay(const std::string& day) {
  bool ok;
  int value = stringToInteger(day, &ok);
  if (!ok) {
    LOG_FREE(Error, "openstudio.EpwFile", "Day value '" << day << "' cannot be converted into an integer");
    return false;
  }
  return setDay(value);
}

int EpwDataPoint::hour() const {
  return m_hour;
}

bool EpwDataPoint::setHour(int hour) {
  if (1 > hour || 24 < hour) {
    LOG_FREE(Error, "openstudio.EpwFile", "Hour value " << hour << " out of range");
    return false;
  }
  m_hour = hour;
  return true;
}

bool EpwDataPoint::setHour(const std::string& hour) {
  bool ok;
  int value = stringToInteger(hour, &ok);
  if (!ok) {
    LOG_FREE(Error, "openstudio.EpwFile", "Hour value '" << hour << "' cannot be converted into an integer");
    return false;
  }
  return setHour(value);
}

int EpwDataPoint::minute() const {
  return m_minute;
}

bool EpwDataPoint::setMinute(int minute) {
  if (0 > minute || 59 < minute) {
    LOG_FREE(Error, "openstudio.EpwFile", "Minute value " << minute << " out of range");
    return false;
  }
  m_minute = minute;
  return true;
}

bool EpwDataPoint::setMinute(const std::string& minute) {
  bool ok;
  int value = stringToInteger(minute, &ok);
  if (!ok) {
    LOG_FREE(Error, "openstudio.EpwFile", "Minute value '" << minute << "' cannot be converted into an integer");
    return false;
  }
  return setMinute(value);
}

std::string EpwDataPoint::dataSourceandUncertaintyFlags() const {
  return m_dataSourceandUncertaintyFlags;
}

void EpwDataPoint::setDataSourceandUncertaintyFlags(const std::string& dataSourceandUncertaintyFlags) {
  m_dataSourceandUncertaintyFlags = dataSourceandUncertaintyFlags;
}

boost::optional<double> EpwDataPoint::dryBulbTemperature() const {
  if (m_dryBulbTemperature == "99.9") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_dryBulbTemperature));
}

bool EpwDataPoint::setDryBulbTemperature(double value) {
  if (-70 >= value || 70 <= value) {
    LOG_FREE(Warn, "openstudio.EpwFile", "DryBulbTemperature value '" << value << "' not within the expected limits");
  }
  m_dryBulbTemperature = std::to_string(value);
  return true;
}

bool EpwDataPoint::setDryBulbTemperature(const std::string& dryBulbTemperature) {
  bool ok;
  double value = stringToDouble(dryBulbTemperature, &ok);
  if (!ok) {
    m_dryBulbTemperature = "99.9";
    return false;
  } else if (-70 >= value || 70 <= value) {
    LOG_FREE(Warn, "openstudio.EpwFile", "DryBulbTemperature value '" << value << "' not within the expected limits");
  }
  m_dryBulbTemperature = dryBulbTemperature;
  return true;
}

boost::optional<double> EpwDataPoint::dewPointTemperature() const {
  if (m_dewPointTemperature == "99.9") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_dewPointTemperature));
}

bool EpwDataPoint::setDewPointTemperature(double value) {
  if (-70 >= value || 70 <= value) {
    LOG_FREE(Warn, "openstudio.EpwFile", "DewPointTemperature value '" << value << "' not within the expected limits");
  }
  m_dewPointTemperature = std::to_string(value);
  return true;
}

bool EpwDataPoint::setDewPointTemperature(const std::string& dewPointTemperature) {
  bool ok;
  double value = stringToDouble(dewPointTemperature, &ok);
  if (!ok) {
    m_dewPointTemperature = "99.9";
    return false;
  } else if (-70 >= value || 70 <= value) {
    LOG_FREE(Warn, "openstudio.EpwFile", "DewPointTemperature value '" << value << "' not within the expected limits");
  }
  m_dewPointTemperature = dewPointTemperature;
  return true;
}

boost::optional<double> EpwDataPoint::relativeHumidity() const {
  if (m_relativeHumidity == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_relativeHumidity));
}

bool EpwDataPoint::setRelativeHumidity(double value) {
  if (0 > value) {
    m_relativeHumidity = "999";
    return false;
  } else if (110 < value) {
    LOG_FREE(Warn, "openstudio.EpwFile", "RelativeHumidity value '" << value << "' not within the expected limits");
  }
  m_relativeHumidity = std::to_string(value);
  return true;
}

bool EpwDataPoint::setRelativeHumidity(const std::string& relativeHumidity) {
  bool ok;
  double value = stringToDouble(relativeHumidity, &ok);
  if (!ok || 0 > value) {
    m_relativeHumidity = "999";
    return false;
  } else if (110 < value) {
    LOG_FREE(Warn, "openstudio.EpwFile", "RelativeHumidity value '" << value << "' not within the expected limits");
  }
  m_relativeHumidity = relativeHumidity;
  return true;
}

boost::optional<double> EpwDataPoint::atmosphericStationPressure() const {
  if (m_atmosphericStationPressure == "999999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_atmosphericStationPressure));
}

bool EpwDataPoint::setAtmosphericStationPressure(double value) {
  if (31000 >= value || 120000 <= value) {
    LOG_FREE(Warn, "openstudio.EpwFile", "AtmosphericStationPressure value '" << value << "' not within the expected limits");
  }
  m_atmosphericStationPressure = std::to_string(value);
  return true;
}

bool EpwDataPoint::setAtmosphericStationPressure(const std::string& atmosphericStationPressure) {
  bool ok;
  double value = stringToDouble(atmosphericStationPressure, &ok);
  if (!ok) {
    m_atmosphericStationPressure = "999999";
    return false;
  } else if (31000 >= value || 120000 <= value) {
    LOG_FREE(Warn, "openstudio.EpwFile", "AtmosphericStationPressure value '" << value << "' not within the expected limits");
  }
  m_atmosphericStationPressure = atmosphericStationPressure;
  return true;
}

boost::optional<double> EpwDataPoint::extraterrestrialHorizontalRadiation() const {
  if (m_extraterrestrialHorizontalRadiation == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_extraterrestrialHorizontalRadiation));
}

bool EpwDataPoint::setExtraterrestrialHorizontalRadiation(double value) {
  if (0 > value || value == 9999) {
    m_extraterrestrialHorizontalRadiation = "9999";
    return false;
  }
  m_extraterrestrialHorizontalRadiation = std::to_string(value);
  return true;
}

bool EpwDataPoint::setExtraterrestrialHorizontalRadiation(const std::string& extraterrestrialHorizontalRadiation) {
  bool ok;
  double value = stringToDouble(extraterrestrialHorizontalRadiation, &ok);
  if (!ok || 0 > value || value == 9999) {
    m_extraterrestrialHorizontalRadiation = "9999";
    return false;
  }
  m_extraterrestrialHorizontalRadiation = extraterrestrialHorizontalRadiation;
  return true;
}

boost::optional<double> EpwDataPoint::extraterrestrialDirectNormalRadiation() const {
  if (m_extraterrestrialDirectNormalRadiation == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_extraterrestrialDirectNormalRadiation));
}

bool EpwDataPoint::setExtraterrestrialDirectNormalRadiation(double value) {
  if (0 > value || value == 9999) {
    m_extraterrestrialDirectNormalRadiation = "9999";
    return false;
  }
  m_extraterrestrialDirectNormalRadiation = std::to_string(value);
  return true;
}

bool EpwDataPoint::setExtraterrestrialDirectNormalRadiation(const std::string& extraterrestrialDirectNormalRadiation) {
  bool ok;
  double value = stringToDouble(extraterrestrialDirectNormalRadiation, &ok);
  if (!ok || 0 > value || value == 9999) {
    m_extraterrestrialDirectNormalRadiation = "9999";
    return false;
  }
  m_extraterrestrialDirectNormalRadiation = extraterrestrialDirectNormalRadiation;
  return true;
}

boost::optional<double> EpwDataPoint::horizontalInfraredRadiationIntensity() const {
  if (m_horizontalInfraredRadiationIntensity == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_horizontalInfraredRadiationIntensity));
}

bool EpwDataPoint::setHorizontalInfraredRadiationIntensity(double value) {
  if (0 > value || value == 9999) {
    m_horizontalInfraredRadiationIntensity = "9999";
    return false;
  }
  m_horizontalInfraredRadiationIntensity = std::to_string(value);
  return true;
}

bool EpwDataPoint::setHorizontalInfraredRadiationIntensity(const std::string& horizontalInfraredRadiationIntensity) {
  bool ok;
  double value = stringToDouble(horizontalInfraredRadiationIntensity, &ok);
  if (!ok || 0 > value || value == 9999) {
    m_horizontalInfraredRadiationIntensity = "9999";
    return false;
  }
  m_horizontalInfraredRadiationIntensity = horizontalInfraredRadiationIntensity;
  return true;
}

boost::optional<double> EpwDataPoint::globalHorizontalRadiation() const {
  if (m_globalHorizontalRadiation == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_globalHorizontalRadiation));
}

bool EpwDataPoint::setGlobalHorizontalRadiation(double value) {
  if (0 > value || value == 9999) {
    m_globalHorizontalRadiation = "9999";
    return false;
  }
  m_globalHorizontalRadiation = std::to_string(value);
  return true;
}

bool EpwDataPoint::setGlobalHorizontalRadiation(const std::string& globalHorizontalRadiation) {
  bool ok;
  double value = stringToDouble(globalHorizontalRadiation, &ok);
  if (!ok || 0 > value || value == 9999) {
    m_globalHorizontalRadiation = "9999";
    return false;
  }
  return setGlobalHorizontalRadiation(value);
}

boost::optional<double> EpwDataPoint::directNormalRadiation() const {
  if (m_directNormalRadiation == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_directNormalRadiation));
}

bool EpwDataPoint::setDirectNormalRadiation(double value) {
  if (0 > value || value == 9999) {
    m_directNormalRadiation = "9999";
    return false;
  }
  m_directNormalRadiation = std::to_string(value);
  return true;
}

bool EpwDataPoint::setDirectNormalRadiation(const std::string& directNormalRadiation) {
  bool ok;
  double value = stringToDouble(directNormalRadiation, &ok);
  if (!ok || 0 > value || value == 9999) {
    m_directNormalRadiation = "9999";
    return false;
  }
  m_directNormalRadiation = directNormalRadiation;
  return true;
}

boost::optional<double> EpwDataPoint::diffuseHorizontalRadiation() const {
  if (m_diffuseHorizontalRadiation == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_diffuseHorizontalRadiation));
}

bool EpwDataPoint::setDiffuseHorizontalRadiation(double value) {
  if (0 > value || value == 9999) {
    m_diffuseHorizontalRadiation = "9999";
    return false;
  }
  m_diffuseHorizontalRadiation = std::to_string(value);
  return true;
}

bool EpwDataPoint::setDiffuseHorizontalRadiation(const std::string& diffuseHorizontalRadiation) {
  bool ok;
  double value = stringToDouble(diffuseHorizontalRadiation, &ok);
  if (!ok || 0 > value || value == 9999) {
    m_diffuseHorizontalRadiation = "9999";
    return false;
  }
  m_diffuseHorizontalRadiation = diffuseHorizontalRadiation;
  return true;
}

boost::optional<double> EpwDataPoint::globalHorizontalIlluminance() const {
  if (m_globalHorizontalIlluminance == "999999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_globalHorizontalIlluminance));
}

bool EpwDataPoint::setGlobalHorizontalIlluminance(double value) {
  if (0 > value || 999900 < value) {
    m_globalHorizontalIlluminance = "999999";
    return false;
  }
  m_globalHorizontalIlluminance = std::to_string(value);
  return true;
}

bool EpwDataPoint::setGlobalHorizontalIlluminance(const std::string& globalHorizontalIlluminance) {
  bool ok;
  double value = stringToDouble(globalHorizontalIlluminance, &ok);
  if (!ok || 0 > value || 999900 < value) {
    m_globalHorizontalIlluminance = "999999";
    return false;
  }
  m_globalHorizontalIlluminance = globalHorizontalIlluminance;
  return true;
}

boost::optional<double> EpwDataPoint::directNormalIlluminance() const {
  if (m_directNormalIlluminance == "999999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_directNormalIlluminance));
}

bool EpwDataPoint::setDirectNormalIlluminance(double value) {
  if (0 > value || 999900 < value) {
    m_directNormalIlluminance = "999999";
    return false;
  }
  m_directNormalIlluminance = std::to_string(value);
  return true;
}

bool EpwDataPoint::setDirectNormalIlluminance(const std::string& directNormalIlluminance) {
  bool ok;
  double value = stringToDouble(directNormalIlluminance, &ok);
  if (!ok || 0 > value || 999900 < value) {
    m_directNormalIlluminance = "999999";
    return false;
  }
  m_directNormalIlluminance = directNormalIlluminance;
  return true;
}

boost::optional<double> EpwDataPoint::diffuseHorizontalIlluminance() const {
  if (m_diffuseHorizontalIlluminance == "999999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_diffuseHorizontalIlluminance));
}

bool EpwDataPoint::setDiffuseHorizontalIlluminance(double value) {
  if (0 > value || 999900 < value) {
    m_diffuseHorizontalIlluminance = "999999";
    return false;
  }
  m_diffuseHorizontalIlluminance = std::to_string(value);
  return true;
}

bool EpwDataPoint::setDiffuseHorizontalIlluminance(const std::string& diffuseHorizontalIlluminance) {
  bool ok;
  double value = stringToDouble(diffuseHorizontalIlluminance, &ok);
  if (!ok || 0 > value || 999900 < value) {
    m_diffuseHorizontalIlluminance = "999999";
    return false;
  }
  m_diffuseHorizontalIlluminance = diffuseHorizontalIlluminance;
  return true;
}

boost::optional<double> EpwDataPoint::zenithLuminance() const {
  if (m_zenithLuminance == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_zenithLuminance));
}

bool EpwDataPoint::setZenithLuminance(double value) {
  if (0 > value || 9999 <= value) {
    m_zenithLuminance = "9999";
    return false;
  }
  m_zenithLuminance = std::to_string(value);
  return true;
}

bool EpwDataPoint::setZenithLuminance(const std::string& zenithLuminance) {
  bool ok;
  double value = stringToDouble(zenithLuminance, &ok);
  if (!ok || 0 > value || 9999 <= value) {
    m_zenithLuminance = "9999";
    return false;
  }
  m_zenithLuminance = zenithLuminance;
  return true;
}

boost::optional<double> EpwDataPoint::windDirection() const {
  if (m_windDirection == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_windDirection));
}

bool EpwDataPoint::setWindDirection(double value) {
  if (0 > value || 360 < value) {
    m_windDirection = "999";
    return false;
  }
  m_windDirection = std::to_string(value);
  return true;
}

bool EpwDataPoint::setWindDirection(const std::string& windDirection) {
  bool ok;
  double value = stringToDouble(windDirection, &ok);
  if (!ok || 0 > value || 360 < value) {
    m_windDirection = "999";
    return false;
  }
  m_windDirection = windDirection;
  return true;
}

boost::optional<double> EpwDataPoint::windSpeed() const {
  if (m_windSpeed == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_windSpeed));
}

bool EpwDataPoint::setWindSpeed(double value) {
  if (0 > value) {
    m_windSpeed = "999";
    return false;
  } else if (40 < value) {
    LOG_FREE(Warn, "openstudio.EpwFile", "WindSpeed value '" << value << "' not within the expected limits");
  }
  m_windSpeed = std::to_string(value);
  return true;
}

bool EpwDataPoint::setWindSpeed(const std::string& windSpeed) {
  bool ok;
  double value = stringToDouble(windSpeed, &ok);
  if (!ok || 0 > value) {
    m_windSpeed = "999";
    return false;
  } else if (40 < value) {
    LOG_FREE(Warn, "openstudio.EpwFile", "WindSpeed value '" << value << "' not within the expected limits");
  }
  return setWindSpeed(value);
}

int EpwDataPoint::totalSkyCover() const {
  return m_totalSkyCover;
}

bool EpwDataPoint::setTotalSkyCover(int value) {
  if (0 > value || 10 < value) {
    m_totalSkyCover = 99;
    return false;
  }
  m_totalSkyCover = value;
  return true;
}

bool EpwDataPoint::setTotalSkyCover(const std::string& totalSkyCover) {
  bool ok;
  int value = stringToInteger(totalSkyCover, &ok);
  if (!ok || 0 > value || 10 < value) {
    m_totalSkyCover = 99;
    return false;
  }
  return setTotalSkyCover(value);
}

int EpwDataPoint::opaqueSkyCover() const {
  return m_opaqueSkyCover;
}

bool EpwDataPoint::setOpaqueSkyCover(int value) {
  if (0 > value || 10 < value) {
    m_opaqueSkyCover = 99;
    return false;
  }
  m_opaqueSkyCover = value;
  return true;
}

bool EpwDataPoint::setOpaqueSkyCover(const std::string& opaqueSkyCover) {
  bool ok;
  int value = stringToInteger(opaqueSkyCover, &ok);
  if (!ok || 0 > value || 10 < value) {
    m_opaqueSkyCover = 99;
    return false;
  }
  return setOpaqueSkyCover(value);
}

boost::optional<double> EpwDataPoint::visibility() const {
  if (m_visibility == "9999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_visibility));
}

bool EpwDataPoint::setVisibility(double value) {
  if (value == 9999) {
    m_visibility = "9999";
    return false;
  }
  m_visibility = std::to_string(value);
  return true;
}

bool EpwDataPoint::setVisibility(const std::string& visibility) {
  bool ok;
  double value = stringToDouble(visibility, &ok);
  if (!ok || value == 9999) {
    m_visibility = "9999";
    return false;
  }
  m_visibility = visibility;
  return true;
}

boost::optional<double> EpwDataPoint::ceilingHeight() const {
  if (m_ceilingHeight == "99999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_ceilingHeight));
}

void EpwDataPoint::setCeilingHeight(double ceilingHeight) {
  m_ceilingHeight = std::to_string(ceilingHeight);
}

bool EpwDataPoint::setCeilingHeight(const std::string& ceilingHeight) {
  bool ok;
  double value = stringToDouble(ceilingHeight, &ok);
  if (!ok || value == 99999) {
    m_ceilingHeight = "99999";
    return false;
  }
  m_ceilingHeight = ceilingHeight;
  return true;
}

int EpwDataPoint::presentWeatherObservation() const {
  return m_presentWeatherObservation;
}

void EpwDataPoint::setPresentWeatherObservation(int presentWeatherObservation) {
  m_presentWeatherObservation = presentWeatherObservation;
}

bool EpwDataPoint::setPresentWeatherObservation(const std::string& presentWeatherObservation) {
  bool ok;
  int value = stringToInteger(presentWeatherObservation, &ok);
  if (!ok) {
    return false;
  }
  m_presentWeatherObservation = value;
  return true;
}

int EpwDataPoint::presentWeatherCodes() const {
  return m_presentWeatherCodes;
}

void EpwDataPoint::setPresentWeatherCodes(int presentWeatherCodes) {
  m_presentWeatherCodes = presentWeatherCodes;
}

bool EpwDataPoint::setPresentWeatherCodes(const std::string& presentWeatherCodes) {
  bool ok;
  int value = stringToInteger(presentWeatherCodes, &ok);
  if (!ok) {
    return false;
  }
  m_presentWeatherCodes = value;
  return true;
}

boost::optional<double> EpwDataPoint::precipitableWater() const {
  if (m_precipitableWater == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_precipitableWater));
}

void EpwDataPoint::setPrecipitableWater(double precipitableWater) {
  m_precipitableWater = std::to_string(precipitableWater);
}

bool EpwDataPoint::setPrecipitableWater(const std::string& precipitableWater) {
  bool ok;
  double value = stringToDouble(precipitableWater, &ok);
  if (!ok || value == 999) {
    m_precipitableWater = "999";
    return false;
  }
  m_precipitableWater = precipitableWater;
  return true;
}

boost::optional<double> EpwDataPoint::aerosolOpticalDepth() const {
  if (m_aerosolOpticalDepth == ".999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_aerosolOpticalDepth));
}

void EpwDataPoint::setAerosolOpticalDepth(double aerosolOpticalDepth) {
  m_aerosolOpticalDepth = std::to_string(aerosolOpticalDepth);
}

bool EpwDataPoint::setAerosolOpticalDepth(const std::string& aerosolOpticalDepth) {
  bool ok;
  double value = stringToDouble(aerosolOpticalDepth, &ok);
  if (!ok || value == 0.999) {
    m_aerosolOpticalDepth = ".999";
    return false;
  }
  m_aerosolOpticalDepth = aerosolOpticalDepth;
  return true;
}

boost::optional<double> EpwDataPoint::snowDepth() const {
  if (m_snowDepth == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_snowDepth));
}

void EpwDataPoint::setSnowDepth(double snowDepth) {
  m_snowDepth = std::to_string(snowDepth);
}

bool EpwDataPoint::setSnowDepth(const std::string& snowDepth) {
  bool ok;
  double value = stringToDouble(snowDepth, &ok);
  if (!ok || value == 999) {
    m_snowDepth = "999";
    return false;
  }
  m_snowDepth = snowDepth;
  return true;
}

boost::optional<double> EpwDataPoint::daysSinceLastSnowfall() const {
  if (m_daysSinceLastSnowfall == "99") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_daysSinceLastSnowfall));
}

void EpwDataPoint::setDaysSinceLastSnowfall(double daysSinceLastSnowfall) {
  m_daysSinceLastSnowfall = std::to_string(daysSinceLastSnowfall);
}

bool EpwDataPoint::setDaysSinceLastSnowfall(const std::string& daysSinceLastSnowfall) {
  bool ok;
  double value = stringToDouble(daysSinceLastSnowfall, &ok);
  if (!ok || value == 99) {
    m_daysSinceLastSnowfall = "99";
    return false;
  }
  m_daysSinceLastSnowfall = daysSinceLastSnowfall;
  return true;
}

boost::optional<double> EpwDataPoint::albedo() const {
  if (m_albedo == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_albedo));
}

void EpwDataPoint::setAlbedo(double albedo) {
  m_albedo = std::to_string(albedo);
}

bool EpwDataPoint::setAlbedo(const std::string& albedo) {
  bool ok;
  double value = stringToDouble(albedo, &ok);
  if (!ok || value == 999) {
    m_albedo = "999";
    return false;
  }
  m_albedo = albedo;
  return true;
}

boost::optional<double> EpwDataPoint::liquidPrecipitationDepth() const {
  if (m_liquidPrecipitationDepth == "999") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_liquidPrecipitationDepth));
}

void EpwDataPoint::setLiquidPrecipitationDepth(double liquidPrecipitationDepth) {
  m_liquidPrecipitationDepth = std::to_string(liquidPrecipitationDepth);
}

bool EpwDataPoint::setLiquidPrecipitationDepth(const std::string& liquidPrecipitationDepth) {
  bool ok;
  double value = stringToDouble(liquidPrecipitationDepth, &ok);
  if (!ok || value == 999) {
    m_liquidPrecipitationDepth = "999";
    return false;
  }
  m_liquidPrecipitationDepth = liquidPrecipitationDepth;
  return true;
}

boost::optional<double> EpwDataPoint::liquidPrecipitationQuantity() const {
  if (m_liquidPrecipitationQuantity == "99") {
    return boost::none;
  }
  return boost::optional<double>(std::stod(m_liquidPrecipitationQuantity));
}

void EpwDataPoint::setLiquidPrecipitationQuantity(double liquidPrecipitationQuantity) {
  m_liquidPrecipitationQuantity = std::to_string(liquidPrecipitationQuantity);
}

bool EpwDataPoint::setLiquidPrecipitationQuantity(const std::string& liquidPrecipitationQuantity) {
  bool ok;
  double value = stringToDouble(liquidPrecipitationQuantity, &ok);
  if (!ok || value == 99) {
    m_liquidPrecipitationQuantity = "99";
    return false;
  }
  m_liquidPrecipitationQuantity = liquidPrecipitationQuantity;
  return true;
}

boost::optional<AirState> EpwDataPoint::airState() const {
  boost::optional<double> value = dryBulbTemperature();
  if (!value) {
    return boost::none;  // Have to have dry bulb
  }
  double drybulb = value.get();

  value = atmosphericStationPressure();
  if (!value) {
    return boost::none;  // Have to have pressure
  }
  double pressure = value.get();

  value = relativeHumidity();
  if (!value) {  // Don't have relative humidity
    value = dewPointTemperature();
    if (value) {
      double dewpoint = value.get();
      return AirState::fromDryBulbDewPointPressure(drybulb, dewpoint, pressure);
    }
  } else {  // Have relative humidity
    double RH = value.get();
    return AirState::fromDryBulbRelativeHumidityPressure(drybulb, RH, pressure);
  }

  return boost::none;
}

boost::optional<double> EpwDataPoint::saturationPressure() const {
  boost::optional<double> optdrybulb = dryBulbTemperature();
  if (optdrybulb) {
    double drybulb = optdrybulb.get();
    if (drybulb >= -100.0 && drybulb <= 200.0) {
      return boost::optional<double>(openstudio::psat(drybulb));
    }
  }
  return boost::none;
}

boost::optional<double> EpwDataPoint::enthalpy() const {
  boost::optional<AirState> state = airState();
  if (state) {
    return boost::optional<double>(state.get().enthalpy());
  }
  return boost::none;
}

boost::optional<double> EpwDataPoint::humidityRatio() const {
  boost::optional<AirState> state = airState();
  if (state) {
    return boost::optional<double>(state.get().humidityRatio());
  }
  return boost::none;
}

boost::optional<double> EpwDataPoint::density() const {
  boost::optional<AirState> state = airState();
  if (state) {
    return boost::optional<double>(state.get().density());
  }
  return boost::none;
}

boost::optional<double> EpwDataPoint::specificVolume() const {
  boost::optional<AirState> state = airState();
  if (state) {
    return boost::optional<double>(state.get().specificVolume());
  }
  return boost::none;
}

boost::optional<double> EpwDataPoint::wetbulb() const {
  boost::optional<AirState> state = airState();
  if (state) {
    return boost::optional<double>(state.get().wetbulb());
  }
  return boost::none;
}

EpwFile::EpwFile(const openstudio::path& p, bool storeData)
  : m_path(p), m_latitude(0), m_longitude(0), m_timeZone(0), m_elevation(0), m_isActual(false), m_minutesMatch(true) {
  if (!openstudio::filesystem::exists(m_path) || !openstudio::filesystem::is_regular_file(m_path)) {
    LOG_AND_THROW("Path '" << m_path << "' is not an EPW file");
  }

  // set checksum
  m_checksum = openstudio::checksum(m_path);

  // open file
  std::ifstream ifs(openstudio::toSystemFilename(m_path));

  if (!parse(ifs, storeData)) {
    ifs.close();
    LOG_AND_THROW("EpwFile '" << toString(p) << "' cannot be processed");
  }
  ifs.close();
}

EpwFile::EpwFile() : m_latitude(0), m_longitude(0), m_timeZone(0), m_elevation(0), m_isActual(false), m_minutesMatch(true) {}

boost::optional<EpwFile> EpwFile::load(const openstudio::path& p, bool storeData) {
  boost::optional<EpwFile> result;
  try {
    result = EpwFile(p, storeData);
  } catch (const std::exception&) {
  }
  return result;
}

boost::optional<EpwFile> EpwFile::loadFromString(const std::string& str, bool storeData) {
  EpwFile result;
  std::stringstream ss(str);
  if (result.parse(ss, storeData)) {
    result.m_checksum = openstudio::checksum(str);
  } else {
    return boost::none;
  }
  return result;
}

openstudio::path EpwFile::path() const {
  return m_path;
}

std::string EpwFile::checksum() const {
  return m_checksum;
}

std::string EpwFile::city() const {
  return m_city;
}

std::string EpwFile::stateProvinceRegion() const {
  return m_stateProvinceRegion;
}

std::string EpwFile::country() const {
  return m_country;
}

std::string EpwFile::dataSource() const {
  return m_dataSource;
}

std::string EpwFile::wmoNumber() const {
  return m_wmoNumber;
}

double EpwFile::latitude() const {
  return m_latitude;
}

double EpwFile::longitude() const {
  return m_longitude;
}

double EpwFile::timeZone() const {
  return m_timeZone;
}

double EpwFile::elevation() const {
  return m_elevation;
}

Time EpwFile::timeStep() const {
  OS_ASSERT((60 % m_recordsPerHour) == 0);
  return {0, 0, 60 / m_recordsPerHour};
}

int EpwFile::recordsPerHour() const {
  return m_recordsPerHour;
}

DayOfWeek EpwFile::startDayOfWeek() const {
  return m_startDayOfWeek;
}

Date EpwFile::startDate() const {
  return m_startDate;
}

Date EpwFile::endDate() const {
  return m_endDate;
}

boost::optional<int> EpwFile::startDateActualYear() const {
  return m_startDateActualYear;
}

boost::optional<int> EpwFile::endDateActualYear() const {
  return m_endDateActualYear;
}

std::vector<EpwDataPoint> EpwFile::data() {
  if (m_data.empty()) {
    if (!openstudio::filesystem::exists(m_path) || !openstudio::filesystem::is_regular_file(m_path)) {
      LOG_AND_THROW("Path '" << m_path << "' is not an EPW file");
    }

    // set checksum
    m_checksum = openstudio::checksum(m_path);

    // open file
    std::ifstream ifs(openstudio::toSystemFilename(m_path));

    if (!parse(ifs, true)) {
      ifs.close();
      LOG(Error, "EpwFile '" << toString(m_path) << "' cannot be processed");
    } else {
      ifs.close();
    }
  }
  return m_data;
}

std::string EpwDesignCondition::titleOfDesignCondition() const {
  return m_titleOfDesignCondition;
}

boost::optional<int> EpwDesignCondition::heatingColdestMonth() const {
  return m_heatingColdestMonth;
}

boost::optional<double> EpwDesignCondition::heatingDryBulb99pt6() const {
  return m_heatingDryBulb99pt6;
}

boost::optional<double> EpwDesignCondition::heatingDryBulb99() const {
  return m_heatingDryBulb99;
}

boost::optional<double> EpwDesignCondition::heatingHumidificationDewPoint99pt6() const {
  return m_heatingHumidificationDewPoint99pt6;
}

boost::optional<double> EpwDesignCondition::heatingHumidificationHumidityRatio99pt6() const {
  return m_heatingHumidificationHumidityRatio99pt6;
}

boost::optional<double> EpwDesignCondition::heatingHumidificationMeanCoincidentDryBulb99pt6() const {
  return m_heatingHumidificationMeanCoincidentDryBulb99pt6;
}

boost::optional<double> EpwDesignCondition::heatingHumidificationDewPoint99() const {
  return m_heatingHumidificationDewPoint99;
}

boost::optional<double> EpwDesignCondition::heatingHumidificationHumidityRatio99() const {
  return m_heatingHumidificationHumidityRatio99;
}

boost::optional<double> EpwDesignCondition::heatingHumidificationMeanCoincidentDryBulb99() const {
  return m_heatingHumidificationMeanCoincidentDryBulb99;
}

boost::optional<double> EpwDesignCondition::heatingColdestMonthWindSpeed0pt4() const {
  return m_heatingColdestMonthWindSpeed0pt4;
}

boost::optional<double> EpwDesignCondition::heatingColdestMonthMeanCoincidentDryBulb0pt4() const {
  return m_heatingColdestMonthMeanCoincidentDryBulb0pt4;
}

boost::optional<double> EpwDesignCondition::heatingColdestMonthWindSpeed1() const {
  return m_heatingColdestMonthWindSpeed1;
}

boost::optional<double> EpwDesignCondition::heatingColdestMonthMeanCoincidentDryBulb1() const {
  return m_heatingColdestMonthMeanCoincidentDryBulb1;
}

boost::optional<double> EpwDesignCondition::heatingMeanCoincidentWindSpeed99pt6() const {
  return m_heatingMeanCoincidentWindSpeed99pt6;
}

boost::optional<int> EpwDesignCondition::heatingPrevailingCoincidentWindDirection99pt6() const {
  return m_heatingPrevailingCoincidentWindDirection99pt6;
}

boost::optional<int> EpwDesignCondition::coolingHottestMonth() const {
  return m_coolingHottestMonth;
}

boost::optional<double> EpwDesignCondition::coolingDryBulbRange() const {
  return m_coolingDryBulbRange;
}

boost::optional<double> EpwDesignCondition::coolingDryBulb0pt4() const {
  return m_coolingDryBulb0pt4;
}

boost::optional<double> EpwDesignCondition::coolingMeanCoincidentWetBulb0pt4() const {
  return m_coolingMeanCoincidentWetBulb0pt4;
}

boost::optional<double> EpwDesignCondition::coolingDryBulb1() const {
  return m_coolingDryBulb1;
}

boost::optional<double> EpwDesignCondition::coolingMeanCoincidentWetBulb1() const {
  return m_coolingMeanCoincidentWetBulb1;
}

boost::optional<double> EpwDesignCondition::coolingDryBulb2() const {
  return m_coolingDryBulb2;
}

boost::optional<double> EpwDesignCondition::coolingMeanCoincidentWetBulb2() const {
  return m_coolingMeanCoincidentWetBulb2;
}

boost::optional<double> EpwDesignCondition::coolingEvaporationWetBulb0pt4() const {
  return m_coolingEvaporationWetBulb0pt4;
}

boost::optional<double> EpwDesignCondition::coolingEvaporationMeanCoincidentDryBulb0pt4() const {
  return m_coolingEvaporationMeanCoincidentDryBulb0pt4;
}

boost::optional<double> EpwDesignCondition::coolingEvaporationWetBulb1() const {
  return m_coolingEvaporationWetBulb1;
}

boost::optional<double> EpwDesignCondition::coolingEvaporationMeanCoincidentDryBulb1() const {
  return m_coolingEvaporationMeanCoincidentDryBulb1;
}

boost::optional<double> EpwDesignCondition::coolingEvaporationWetBulb2() const {
  return m_coolingEvaporationWetBulb2;
}

boost::optional<double> EpwDesignCondition::coolingEvaporationMeanCoincidentDryBulb2() const {
  return m_coolingEvaporationMeanCoincidentDryBulb2;
}

boost::optional<double> EpwDesignCondition::coolingMeanCoincidentWindSpeed0pt4() const {
  return m_coolingMeanCoincidentWindSpeed0pt4;
}

boost::optional<int> EpwDesignCondition::coolingPrevailingCoincidentWindDirection0pt4() const {
  return m_coolingPrevailingCoincidentWindDirection0pt4;
}

boost::optional<double> EpwDesignCondition::coolingDehumidificationDewPoint0pt4() const {
  return m_coolingDehumidificationDewPoint0pt4;
}

boost::optional<double> EpwDesignCondition::coolingDehumidificationHumidityRatio0pt4() const {
  return m_coolingDehumidificationHumidityRatio0pt4;
}

boost::optional<double> EpwDesignCondition::coolingDehumidificationMeanCoincidentDryBulb0pt4() const {
  return m_coolingDehumidificationMeanCoincidentDryBulb0pt4;
}

boost::optional<double> EpwDesignCondition::coolingDehumidificationDewPoint1() const {
  return m_coolingDehumidificationDewPoint1;
}

boost::optional<double> EpwDesignCondition::coolingDehumidificationHumidityRatio1() const {
  return m_coolingDehumidificationHumidityRatio1;
}

boost::optional<double> EpwDesignCondition::coolingDehumidificationMeanCoincidentDryBulb1() const {
  return m_coolingDehumidificationMeanCoincidentDryBulb1;
}

boost::optional<double> EpwDesignCondition::coolingDehumidificationDewPoint2() const {
  return m_coolingDehumidificationDewPoint2;
}

boost::optional<double> EpwDesignCondition::coolingDehumidificationHumidityRatio2() const {
  return m_coolingDehumidificationHumidityRatio2;
}

boost::optional<double> EpwDesignCondition::coolingDehumidificationMeanCoincidentDryBulb2() const {
  return m_coolingDehumidificationMeanCoincidentDryBulb2;
}

boost::optional<double> EpwDesignCondition::coolingEnthalpy0pt4() const {
  return m_coolingEnthalpy0pt4;
}

boost::optional<double> EpwDesignCondition::coolingEnthalpyMeanCoincidentDryBulb0pt4() const {
  return m_coolingEnthalpyMeanCoincidentDryBulb0pt4;
}

boost::optional<double> EpwDesignCondition::coolingEnthalpy1() const {
  return m_coolingEnthalpy1;
}

boost::optional<double> EpwDesignCondition::coolingEnthalpyMeanCoincidentDryBulb1() const {
  return m_coolingEnthalpyMeanCoincidentDryBulb1;
}

boost::optional<double> EpwDesignCondition::coolingEnthalpy2() const {
  return m_coolingEnthalpy2;
}

boost::optional<double> EpwDesignCondition::coolingEnthalpyMeanCoincidentDryBulb2() const {
  return m_coolingEnthalpyMeanCoincidentDryBulb2;
}

boost::optional<int> EpwDesignCondition::coolingHours8To4AndDryBulb12pt8To20pt6() const {
  return m_coolingHours8To4AndDryBulb12pt8To20pt6;
}

boost::optional<double> EpwDesignCondition::extremeWindSpeed1() const {
  return m_extremeWindSpeed1;
}

boost::optional<double> EpwDesignCondition::extremeWindSpeed2pt5() const {
  return m_extremeWindSpeed2pt5;
}

boost::optional<double> EpwDesignCondition::extremeWindSpeed5() const {
  return m_extremeWindSpeed5;
}

boost::optional<double> EpwDesignCondition::extremeMaxWetBulb() const {
  return m_extremeMaxWetBulb;
}

boost::optional<double> EpwDesignCondition::extremeMeanMinDryBulb() const {
  return m_extremeMeanMinDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeMeanMaxDryBulb() const {
  return m_extremeMeanMaxDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeStdDevMinDryBulb() const {
  return m_extremeStdDevMinDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeStdDevMaxDryBulb() const {
  return m_extremeStdDevMaxDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeN5YearsMinDryBulb() const {
  return m_extremeN5YearsMinDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeN5YearsMaxDryBulb() const {
  return m_extremeN5YearsMaxDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeN10YearsMinDryBulb() const {
  return m_extremeN10YearsMinDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeN10YearsMaxDryBulb() const {
  return m_extremeN10YearsMaxDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeN20YearsMinDryBulb() const {
  return m_extremeN20YearsMinDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeN20YearsMaxDryBulb() const {
  return m_extremeN20YearsMaxDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeN50YearsMinDryBulb() const {
  return m_extremeN50YearsMinDryBulb;
}

boost::optional<double> EpwDesignCondition::extremeN50YearsMaxDryBulb() const {
  return m_extremeN50YearsMaxDryBulb;
}

void EpwDesignCondition::setTitleOfDesignCondition(const std::string& titleOfDesignCondition) {
  m_titleOfDesignCondition = titleOfDesignCondition;
}

bool EpwDesignCondition::setHeatingColdestMonth(const std::string& heatingColdestMonth) {
  bool ok;
  int value = stringToInteger(heatingColdestMonth, &ok);
  if (!ok) {
    m_heatingColdestMonth = boost::none;
  } else {
    setHeatingColdestMonth(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingColdestMonth(int heatingColdestMonth) {
  m_heatingColdestMonth = heatingColdestMonth;
}

bool EpwDesignCondition::setHeatingDryBulb99pt6(const std::string& heatingDryBulb99pt6) {
  bool ok;
  double value = stringToDouble(heatingDryBulb99pt6, &ok);
  if (!ok) {
    m_heatingDryBulb99pt6 = boost::none;
  } else {
    setHeatingDryBulb99pt6(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingDryBulb99pt6(double heatingDryBulb99pt6) {
  m_heatingDryBulb99pt6 = heatingDryBulb99pt6;
}

bool EpwDesignCondition::setHeatingDryBulb99(const std::string& heatingDryBulb99) {
  bool ok;
  double value = stringToDouble(heatingDryBulb99, &ok);
  if (!ok) {
    m_heatingDryBulb99 = boost::none;
  } else {
    setHeatingDryBulb99(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingDryBulb99(double heatingDryBulb99) {
  m_heatingDryBulb99 = heatingDryBulb99;
}

bool EpwDesignCondition::setHeatingHumidificationDewPoint99pt6(const std::string& heatingHumidificationDewPoint99pt6) {
  bool ok;
  double value = stringToDouble(heatingHumidificationDewPoint99pt6, &ok);
  if (!ok) {
    m_heatingHumidificationDewPoint99pt6 = boost::none;
  } else {
    setHeatingHumidificationDewPoint99pt6(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingHumidificationDewPoint99pt6(double heatingHumidificationDewPoint99pt6) {
  m_heatingHumidificationDewPoint99pt6 = heatingHumidificationDewPoint99pt6;
}

bool EpwDesignCondition::setHeatingHumidificationHumidityRatio99pt6(const std::string& heatingHumidificationHumidityRatio99pt6) {
  bool ok;
  double value = stringToDouble(heatingHumidificationHumidityRatio99pt6, &ok);
  if (!ok) {
    m_heatingHumidificationHumidityRatio99pt6 = boost::none;
  } else {
    setHeatingHumidificationHumidityRatio99pt6(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingHumidificationHumidityRatio99pt6(double heatingHumidificationHumidityRatio99pt6) {
  m_heatingHumidificationHumidityRatio99pt6 = heatingHumidificationHumidityRatio99pt6;
}

bool EpwDesignCondition::setHeatingHumidificationMeanCoincidentDryBulb99pt6(const std::string& heatingHumidificationMeanCoincidentDryBulb99pt6) {
  bool ok;
  double value = stringToDouble(heatingHumidificationMeanCoincidentDryBulb99pt6, &ok);
  if (!ok) {
    m_heatingHumidificationMeanCoincidentDryBulb99pt6 = boost::none;
  } else {
    setHeatingHumidificationMeanCoincidentDryBulb99pt6(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingHumidificationMeanCoincidentDryBulb99pt6(double heatingHumidificationMeanCoincidentDryBulb99pt6) {
  m_heatingHumidificationMeanCoincidentDryBulb99pt6 = heatingHumidificationMeanCoincidentDryBulb99pt6;
}

bool EpwDesignCondition::setHeatingHumidificationDewPoint99(const std::string& heatingHumidificationDewPoint99) {
  bool ok;
  double value = stringToDouble(heatingHumidificationDewPoint99, &ok);
  if (!ok) {
    m_heatingHumidificationDewPoint99 = boost::none;
  } else {
    setHeatingHumidificationDewPoint99(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingHumidificationDewPoint99(double heatingHumidificationDewPoint99) {
  m_heatingHumidificationDewPoint99 = heatingHumidificationDewPoint99;
}

bool EpwDesignCondition::setHeatingHumidificationHumidityRatio99(const std::string& heatingHumidificationHumidityRatio99) {
  bool ok;
  double value = stringToDouble(heatingHumidificationHumidityRatio99, &ok);
  if (!ok) {
    m_heatingHumidificationHumidityRatio99 = boost::none;
  } else {
    setHeatingHumidificationHumidityRatio99(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingHumidificationHumidityRatio99(double heatingHumidificationHumidityRatio99) {
  m_heatingHumidificationHumidityRatio99 = heatingHumidificationHumidityRatio99;
}

bool EpwDesignCondition::setHeatingHumidificationMeanCoincidentDryBulb99(const std::string& heatingHumidificationMeanCoincidentDryBulb99) {
  bool ok;
  double value = stringToDouble(heatingHumidificationMeanCoincidentDryBulb99, &ok);
  if (!ok) {
    m_heatingHumidificationMeanCoincidentDryBulb99 = boost::none;
  } else {
    setHeatingHumidificationMeanCoincidentDryBulb99(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingHumidificationMeanCoincidentDryBulb99(double heatingHumidificationMeanCoincidentDryBulb99) {
  m_heatingHumidificationMeanCoincidentDryBulb99 = heatingHumidificationMeanCoincidentDryBulb99;
}

bool EpwDesignCondition::setHeatingColdestMonthWindSpeed0pt4(const std::string& heatingColdestMonthWindSpeed0pt4) {
  bool ok;
  double value = stringToDouble(heatingColdestMonthWindSpeed0pt4, &ok);
  if (!ok) {
    m_heatingColdestMonthWindSpeed0pt4 = boost::none;
  } else {
    setHeatingColdestMonthWindSpeed0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingColdestMonthWindSpeed0pt4(double heatingColdestMonthWindSpeed0pt4) {
  m_heatingColdestMonthWindSpeed0pt4 = heatingColdestMonthWindSpeed0pt4;
}

bool EpwDesignCondition::setHeatingColdestMonthMeanCoincidentDryBulb0pt4(const std::string& heatingColdestMonthMeanCoincidentDryBulb0pt4) {
  bool ok;
  double value = stringToDouble(heatingColdestMonthMeanCoincidentDryBulb0pt4, &ok);
  if (!ok) {
    m_heatingColdestMonthMeanCoincidentDryBulb0pt4 = boost::none;
  } else {
    setHeatingColdestMonthMeanCoincidentDryBulb0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingColdestMonthMeanCoincidentDryBulb0pt4(double heatingColdestMonthMeanCoincidentDryBulb0pt4) {
  m_heatingColdestMonthMeanCoincidentDryBulb0pt4 = heatingColdestMonthMeanCoincidentDryBulb0pt4;
}

bool EpwDesignCondition::setHeatingColdestMonthWindSpeed1(const std::string& heatingColdestMonthWindSpeed1) {
  bool ok;
  double value = stringToDouble(heatingColdestMonthWindSpeed1, &ok);
  if (!ok) {
    m_heatingColdestMonthWindSpeed1 = boost::none;
  } else {
    setHeatingColdestMonthWindSpeed1(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingColdestMonthWindSpeed1(double heatingColdestMonthWindSpeed1) {
  m_heatingColdestMonthWindSpeed1 = heatingColdestMonthWindSpeed1;
}

bool EpwDesignCondition::setHeatingColdestMonthMeanCoincidentDryBulb1(const std::string& heatingColdestMonthMeanCoincidentDryBulb1) {
  bool ok;
  double value = stringToDouble(heatingColdestMonthMeanCoincidentDryBulb1, &ok);
  if (!ok) {
    m_heatingColdestMonthMeanCoincidentDryBulb1 = boost::none;
  } else {
    setHeatingColdestMonthMeanCoincidentDryBulb1(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingColdestMonthMeanCoincidentDryBulb1(double heatingColdestMonthMeanCoincidentDryBulb1) {
  m_heatingColdestMonthMeanCoincidentDryBulb1 = heatingColdestMonthMeanCoincidentDryBulb1;
}

bool EpwDesignCondition::setHeatingMeanCoincidentWindSpeed99pt6(const std::string& heatingMeanCoincidentWindSpeed99pt6) {
  bool ok;
  double value = stringToDouble(heatingMeanCoincidentWindSpeed99pt6, &ok);
  if (!ok) {
    m_heatingMeanCoincidentWindSpeed99pt6 = boost::none;
  } else {
    setHeatingMeanCoincidentWindSpeed99pt6(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingMeanCoincidentWindSpeed99pt6(double heatingMeanCoincidentWindSpeed99pt6) {
  m_heatingMeanCoincidentWindSpeed99pt6 = heatingMeanCoincidentWindSpeed99pt6;
}

bool EpwDesignCondition::setHeatingPrevailingCoincidentWindDirection99pt6(const std::string& heatingPrevailingCoincidentWindDirection99pt6) {
  bool ok;
  int value = stringToInteger(heatingPrevailingCoincidentWindDirection99pt6, &ok);
  if (!ok) {
    m_heatingPrevailingCoincidentWindDirection99pt6 = boost::none;
  } else {
    setHeatingPrevailingCoincidentWindDirection99pt6(value);
  }
  return ok;
}

void EpwDesignCondition::setHeatingPrevailingCoincidentWindDirection99pt6(int heatingPrevailingCoincidentWindDirection99pt6) {
  m_heatingPrevailingCoincidentWindDirection99pt6 = heatingPrevailingCoincidentWindDirection99pt6;
}

bool EpwDesignCondition::setCoolingHottestMonth(const std::string& coolingHottestMonth) {
  bool ok;
  int value = stringToInteger(coolingHottestMonth, &ok);
  if (!ok) {
    m_coolingHottestMonth = boost::none;
  } else {
    setCoolingHottestMonth(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingHottestMonth(int coolingHottestMonth) {
  m_coolingHottestMonth = coolingHottestMonth;
}

bool EpwDesignCondition::setCoolingDryBulbRange(const std::string& coolingDryBulbRange) {
  bool ok;
  double value = stringToDouble(coolingDryBulbRange, &ok);
  if (!ok) {
    m_coolingDryBulbRange = boost::none;
  } else {
    setCoolingDryBulbRange(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDryBulbRange(double coolingDryBulbRange) {
  m_coolingDryBulbRange = coolingDryBulbRange;
}

bool EpwDesignCondition::setCoolingDryBulb0pt4(const std::string& coolingDryBulb0pt4) {
  bool ok;
  double value = stringToDouble(coolingDryBulb0pt4, &ok);
  if (!ok) {
    m_coolingDryBulb0pt4 = boost::none;
  } else {
    setCoolingDryBulb0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDryBulb0pt4(double coolingDryBulb0pt4) {
  m_coolingDryBulb0pt4 = coolingDryBulb0pt4;
}

bool EpwDesignCondition::setCoolingMeanCoincidentWetBulb0pt4(const std::string& coolingMeanCoincidentWetBulb0pt4) {
  bool ok;
  double value = stringToDouble(coolingMeanCoincidentWetBulb0pt4, &ok);
  if (!ok) {
    m_coolingMeanCoincidentWetBulb0pt4 = boost::none;
  } else {
    setCoolingMeanCoincidentWetBulb0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingMeanCoincidentWetBulb0pt4(double coolingMeanCoincidentWetBulb0pt4) {
  m_coolingMeanCoincidentWetBulb0pt4 = coolingMeanCoincidentWetBulb0pt4;
}

bool EpwDesignCondition::setCoolingDryBulb1(const std::string& coolingDryBulb1) {
  bool ok;
  double value = stringToDouble(coolingDryBulb1, &ok);
  if (!ok) {
    m_coolingDryBulb1 = boost::none;
  } else {
    setCoolingDryBulb1(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDryBulb1(double coolingDryBulb1) {
  m_coolingDryBulb1 = coolingDryBulb1;
}

bool EpwDesignCondition::setCoolingMeanCoincidentWetBulb1(const std::string& coolingMeanCoincidentWetBulb1) {
  bool ok;
  double value = stringToDouble(coolingMeanCoincidentWetBulb1, &ok);
  if (!ok) {
    m_coolingMeanCoincidentWetBulb1 = boost::none;
  } else {
    setCoolingMeanCoincidentWetBulb1(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingMeanCoincidentWetBulb1(double coolingMeanCoincidentWetBulb1) {
  m_coolingMeanCoincidentWetBulb1 = coolingMeanCoincidentWetBulb1;
}

bool EpwDesignCondition::setCoolingDryBulb2(const std::string& coolingDryBulb2) {
  bool ok;
  double value = stringToDouble(coolingDryBulb2, &ok);
  if (!ok) {
    m_coolingDryBulb2 = boost::none;
  } else {
    setCoolingDryBulb2(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDryBulb2(double coolingDryBulb2) {
  m_coolingDryBulb2 = coolingDryBulb2;
}

bool EpwDesignCondition::setCoolingMeanCoincidentWetBulb2(const std::string& coolingMeanCoincidentWetBulb2) {
  bool ok;
  double value = stringToDouble(coolingMeanCoincidentWetBulb2, &ok);
  if (!ok) {
    m_coolingMeanCoincidentWetBulb2 = boost::none;
  } else {
    setCoolingMeanCoincidentWetBulb2(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingMeanCoincidentWetBulb2(double coolingMeanCoincidentWetBulb2) {
  m_coolingMeanCoincidentWetBulb2 = coolingMeanCoincidentWetBulb2;
}

bool EpwDesignCondition::setCoolingEvaporationWetBulb0pt4(const std::string& coolingEvaporationWetBulb0pt4) {
  bool ok;
  double value = stringToDouble(coolingEvaporationWetBulb0pt4, &ok);
  if (!ok) {
    m_coolingEvaporationWetBulb0pt4 = boost::none;
  } else {
    setCoolingEvaporationWetBulb0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEvaporationWetBulb0pt4(double coolingEvaporationWetBulb0pt4) {
  m_coolingEvaporationWetBulb0pt4 = coolingEvaporationWetBulb0pt4;
}

bool EpwDesignCondition::setCoolingEvaporationMeanCoincidentDryBulb0pt4(const std::string& coolingEvaporationMeanCoincidentDryBulb0pt4) {
  bool ok;
  double value = stringToDouble(coolingEvaporationMeanCoincidentDryBulb0pt4, &ok);
  if (!ok) {
    m_coolingEvaporationMeanCoincidentDryBulb0pt4 = boost::none;
  } else {
    setCoolingEvaporationMeanCoincidentDryBulb0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEvaporationMeanCoincidentDryBulb0pt4(double coolingEvaporationMeanCoincidentDryBulb0pt4) {
  m_coolingEvaporationMeanCoincidentDryBulb0pt4 = coolingEvaporationMeanCoincidentDryBulb0pt4;
}

bool EpwDesignCondition::setCoolingEvaporationWetBulb1(const std::string& coolingEvaporationWetBulb1) {
  bool ok;
  double value = stringToDouble(coolingEvaporationWetBulb1, &ok);
  if (!ok) {
    m_coolingEvaporationWetBulb1 = boost::none;
  } else {
    setCoolingEvaporationWetBulb1(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEvaporationWetBulb1(double coolingEvaporationWetBulb1) {
  m_coolingEvaporationWetBulb1 = coolingEvaporationWetBulb1;
}

bool EpwDesignCondition::setCoolingEvaporationMeanCoincidentDryBulb1(const std::string& coolingEvaporationMeanCoincidentDryBulb1) {
  bool ok;
  double value = stringToDouble(coolingEvaporationMeanCoincidentDryBulb1, &ok);
  if (!ok) {
    m_coolingEvaporationMeanCoincidentDryBulb1 = boost::none;
  } else {
    setCoolingEvaporationMeanCoincidentDryBulb1(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEvaporationMeanCoincidentDryBulb1(double coolingEvaporationMeanCoincidentDryBulb1) {
  m_coolingEvaporationMeanCoincidentDryBulb1 = coolingEvaporationMeanCoincidentDryBulb1;
}

bool EpwDesignCondition::setCoolingEvaporationWetBulb2(const std::string& coolingEvaporationWetBulb2) {
  bool ok;
  double value = stringToDouble(coolingEvaporationWetBulb2, &ok);
  if (!ok) {
    m_coolingEvaporationWetBulb2 = boost::none;
  } else {
    setCoolingEvaporationWetBulb2(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEvaporationWetBulb2(double coolingEvaporationWetBulb2) {
  m_coolingEvaporationWetBulb2 = coolingEvaporationWetBulb2;
}

bool EpwDesignCondition::setCoolingEvaporationMeanCoincidentDryBulb2(const std::string& coolingEvaporationMeanCoincidentDryBulb2) {
  bool ok;
  double value = stringToDouble(coolingEvaporationMeanCoincidentDryBulb2, &ok);
  if (!ok) {
    m_coolingEvaporationMeanCoincidentDryBulb2 = boost::none;
  } else {
    setCoolingEvaporationMeanCoincidentDryBulb2(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEvaporationMeanCoincidentDryBulb2(double coolingEvaporationMeanCoincidentDryBulb2) {
  m_coolingEvaporationMeanCoincidentDryBulb2 = coolingEvaporationMeanCoincidentDryBulb2;
}

bool EpwDesignCondition::setCoolingMeanCoincidentWindSpeed0pt4(const std::string& coolingMeanCoincidentWindSpeed0pt4) {
  bool ok;
  double value = stringToDouble(coolingMeanCoincidentWindSpeed0pt4, &ok);
  if (!ok) {
    m_coolingMeanCoincidentWindSpeed0pt4 = boost::none;
  } else {
    setCoolingMeanCoincidentWindSpeed0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingMeanCoincidentWindSpeed0pt4(double coolingMeanCoincidentWindSpeed0pt4) {
  m_coolingMeanCoincidentWindSpeed0pt4 = coolingMeanCoincidentWindSpeed0pt4;
}

bool EpwDesignCondition::setCoolingPrevailingCoincidentWindDirection0pt4(const std::string& coolingPrevailingCoincidentWindDirection0pt4) {
  bool ok;
  int value = stringToInteger(coolingPrevailingCoincidentWindDirection0pt4, &ok);
  if (!ok) {
    m_coolingPrevailingCoincidentWindDirection0pt4 = boost::none;
  } else {
    setCoolingPrevailingCoincidentWindDirection0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingPrevailingCoincidentWindDirection0pt4(int coolingPrevailingCoincidentWindDirection0pt4) {
  m_coolingPrevailingCoincidentWindDirection0pt4 = coolingPrevailingCoincidentWindDirection0pt4;
}

bool EpwDesignCondition::setCoolingDehumidificationDewPoint0pt4(const std::string& coolingDehumidificationDewPoint0pt4) {
  bool ok;
  double value = stringToDouble(coolingDehumidificationDewPoint0pt4, &ok);
  if (!ok) {
    m_coolingDehumidificationDewPoint0pt4 = boost::none;
  } else {
    setCoolingDehumidificationDewPoint0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDehumidificationDewPoint0pt4(double coolingDehumidificationDewPoint0pt4) {
  m_coolingDehumidificationDewPoint0pt4 = coolingDehumidificationDewPoint0pt4;
}

bool EpwDesignCondition::setCoolingDehumidificationHumidityRatio0pt4(const std::string& coolingDehumidificationHumidityRatio0pt4) {
  bool ok;
  double value = stringToDouble(coolingDehumidificationHumidityRatio0pt4, &ok);
  if (!ok) {
    m_coolingDehumidificationHumidityRatio0pt4 = boost::none;
  } else {
    setCoolingDehumidificationHumidityRatio0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDehumidificationHumidityRatio0pt4(double coolingDehumidificationHumidityRatio0pt4) {
  m_coolingDehumidificationHumidityRatio0pt4 = coolingDehumidificationHumidityRatio0pt4;
}

bool EpwDesignCondition::setCoolingDehumidificationMeanCoincidentDryBulb0pt4(const std::string& coolingDehumidificationMeanCoincidentDryBulb0pt4) {
  bool ok;
  double value = stringToDouble(coolingDehumidificationMeanCoincidentDryBulb0pt4, &ok);
  if (!ok) {
    m_coolingDehumidificationMeanCoincidentDryBulb0pt4 = boost::none;
  } else {
    setCoolingDehumidificationMeanCoincidentDryBulb0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDehumidificationMeanCoincidentDryBulb0pt4(double coolingDehumidificationMeanCoincidentDryBulb0pt4) {
  m_coolingDehumidificationMeanCoincidentDryBulb0pt4 = coolingDehumidificationMeanCoincidentDryBulb0pt4;
}

bool EpwDesignCondition::setCoolingDehumidificationDewPoint1(const std::string& coolingDehumidificationDewPoint1) {
  bool ok;
  double value = stringToDouble(coolingDehumidificationDewPoint1, &ok);
  if (!ok) {
    m_coolingDehumidificationDewPoint1 = boost::none;
  } else {
    setCoolingDehumidificationDewPoint1(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDehumidificationDewPoint1(double coolingDehumidificationDewPoint1) {
  m_coolingDehumidificationDewPoint1 = coolingDehumidificationDewPoint1;
}

bool EpwDesignCondition::setCoolingDehumidificationHumidityRatio1(const std::string& coolingDehumidificationHumidityRatio1) {
  bool ok;
  double value = stringToDouble(coolingDehumidificationHumidityRatio1, &ok);
  if (!ok) {
    m_coolingDehumidificationHumidityRatio1 = boost::none;
  } else {
    setCoolingDehumidificationHumidityRatio1(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDehumidificationHumidityRatio1(double coolingDehumidificationHumidityRatio1) {
  m_coolingDehumidificationHumidityRatio1 = coolingDehumidificationHumidityRatio1;
}

bool EpwDesignCondition::setCoolingDehumidificationMeanCoincidentDryBulb1(const std::string& coolingDehumidificationMeanCoincidentDryBulb1) {
  bool ok;
  double value = stringToDouble(coolingDehumidificationMeanCoincidentDryBulb1, &ok);
  if (!ok) {
    m_coolingDehumidificationMeanCoincidentDryBulb1 = boost::none;
  } else {
    setCoolingDehumidificationMeanCoincidentDryBulb1(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDehumidificationMeanCoincidentDryBulb1(double coolingDehumidificationMeanCoincidentDryBulb1) {
  m_coolingDehumidificationMeanCoincidentDryBulb1 = coolingDehumidificationMeanCoincidentDryBulb1;
}

bool EpwDesignCondition::setCoolingDehumidificationDewPoint2(const std::string& coolingDehumidificationDewPoint2) {
  bool ok;
  double value = stringToDouble(coolingDehumidificationDewPoint2, &ok);
  if (!ok) {
    m_coolingDehumidificationDewPoint2 = boost::none;
  } else {
    setCoolingDehumidificationDewPoint2(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDehumidificationDewPoint2(double coolingDehumidificationDewPoint2) {
  m_coolingDehumidificationDewPoint2 = coolingDehumidificationDewPoint2;
}

bool EpwDesignCondition::setCoolingDehumidificationHumidityRatio2(const std::string& coolingDehumidificationHumidityRatio2) {
  bool ok;
  double value = stringToDouble(coolingDehumidificationHumidityRatio2, &ok);
  if (!ok) {
    m_coolingDehumidificationHumidityRatio2 = boost::none;
  } else {
    setCoolingDehumidificationHumidityRatio2(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDehumidificationHumidityRatio2(double coolingDehumidificationHumidityRatio2) {
  m_coolingDehumidificationHumidityRatio2 = coolingDehumidificationHumidityRatio2;
}

bool EpwDesignCondition::setCoolingDehumidificationMeanCoincidentDryBulb2(const std::string& coolingDehumidificationMeanCoincidentDryBulb2) {
  bool ok;
  double value = stringToDouble(coolingDehumidificationMeanCoincidentDryBulb2, &ok);
  if (!ok) {
    m_coolingDehumidificationMeanCoincidentDryBulb2 = boost::none;
  } else {
    setCoolingDehumidificationMeanCoincidentDryBulb2(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingDehumidificationMeanCoincidentDryBulb2(double coolingDehumidificationMeanCoincidentDryBulb2) {
  m_coolingDehumidificationMeanCoincidentDryBulb2 = coolingDehumidificationMeanCoincidentDryBulb2;
}

bool EpwDesignCondition::setCoolingEnthalpy0pt4(const std::string& coolingEnthalpy0pt4) {
  bool ok;
  double value = stringToDouble(coolingEnthalpy0pt4, &ok);
  if (!ok) {
    m_coolingEnthalpy0pt4 = boost::none;
  } else {
    setCoolingEnthalpy0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEnthalpy0pt4(double coolingEnthalpy0pt4) {
  m_coolingEnthalpy0pt4 = coolingEnthalpy0pt4;
}

bool EpwDesignCondition::setCoolingEnthalpyMeanCoincidentDryBulb0pt4(const std::string& coolingEnthalpyMeanCoincidentDryBulb0pt4) {
  bool ok;
  double value = stringToDouble(coolingEnthalpyMeanCoincidentDryBulb0pt4, &ok);
  if (!ok) {
    m_coolingEnthalpyMeanCoincidentDryBulb0pt4 = boost::none;
  } else {
    setCoolingEnthalpyMeanCoincidentDryBulb0pt4(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEnthalpyMeanCoincidentDryBulb0pt4(double coolingEnthalpyMeanCoincidentDryBulb0pt4) {
  m_coolingEnthalpyMeanCoincidentDryBulb0pt4 = coolingEnthalpyMeanCoincidentDryBulb0pt4;
}

bool EpwDesignCondition::setCoolingEnthalpy1(const std::string& coolingEnthalpy1) {
  bool ok;
  double value = stringToDouble(coolingEnthalpy1, &ok);
  if (!ok) {
    m_coolingEnthalpy1 = boost::none;
  } else {
    setCoolingEnthalpy1(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEnthalpy1(double coolingEnthalpy1) {
  m_coolingEnthalpy1 = coolingEnthalpy1;
}

bool EpwDesignCondition::setCoolingEnthalpyMeanCoincidentDryBulb1(const std::string& coolingEnthalpyMeanCoincidentDryBulb1) {
  bool ok;
  double value = stringToDouble(coolingEnthalpyMeanCoincidentDryBulb1, &ok);
  if (!ok) {
    m_coolingEnthalpyMeanCoincidentDryBulb1 = boost::none;
  } else {
    setCoolingEnthalpyMeanCoincidentDryBulb1(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEnthalpyMeanCoincidentDryBulb1(double coolingEnthalpyMeanCoincidentDryBulb1) {
  m_coolingEnthalpyMeanCoincidentDryBulb1 = coolingEnthalpyMeanCoincidentDryBulb1;
}

bool EpwDesignCondition::setCoolingEnthalpy2(const std::string& coolingEnthalpy2) {
  bool ok;
  double value = stringToDouble(coolingEnthalpy2, &ok);
  if (!ok) {
    m_coolingEnthalpy2 = boost::none;
  } else {
    setCoolingEnthalpy2(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEnthalpy2(double coolingEnthalpy2) {
  m_coolingEnthalpy2 = coolingEnthalpy2;
}

bool EpwDesignCondition::setCoolingEnthalpyMeanCoincidentDryBulb2(const std::string& coolingEnthalpyMeanCoincidentDryBulb2) {
  bool ok;
  double value = stringToDouble(coolingEnthalpyMeanCoincidentDryBulb2, &ok);
  if (!ok) {
    m_coolingEnthalpyMeanCoincidentDryBulb2 = boost::none;
  } else {
    setCoolingEnthalpyMeanCoincidentDryBulb2(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingEnthalpyMeanCoincidentDryBulb2(double coolingEnthalpyMeanCoincidentDryBulb2) {
  m_coolingEnthalpyMeanCoincidentDryBulb2 = coolingEnthalpyMeanCoincidentDryBulb2;
}

bool EpwDesignCondition::setCoolingHours8To4AndDryBulb12pt8To20pt6(const std::string& coolingHours8To4AndDryBulb12pt8To20pt6) {
  bool ok;
  int value = stringToInteger(coolingHours8To4AndDryBulb12pt8To20pt6, &ok);
  if (!ok) {
    m_coolingHours8To4AndDryBulb12pt8To20pt6 = boost::none;
  } else {
    setCoolingHours8To4AndDryBulb12pt8To20pt6(value);
  }
  return ok;
}

void EpwDesignCondition::setCoolingHours8To4AndDryBulb12pt8To20pt6(int coolingHours8To4AndDryBulb12pt8To20pt6) {
  m_coolingHours8To4AndDryBulb12pt8To20pt6 = coolingHours8To4AndDryBulb12pt8To20pt6;
}

bool EpwDesignCondition::setExtremeWindSpeed1(const std::string& extremeWindSpeed1) {
  bool ok;
  double value = stringToDouble(extremeWindSpeed1, &ok);
  if (!ok) {
    m_extremeWindSpeed1 = boost::none;
  } else {
    setExtremeWindSpeed1(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeWindSpeed1(double extremeWindSpeed1) {
  m_extremeWindSpeed1 = extremeWindSpeed1;
}

bool EpwDesignCondition::setExtremeWindSpeed2pt5(const std::string& extremeWindSpeed2pt5) {
  bool ok;
  double value = stringToDouble(extremeWindSpeed2pt5, &ok);
  if (!ok) {
    m_extremeWindSpeed2pt5 = boost::none;
  } else {
    setExtremeWindSpeed2pt5(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeWindSpeed2pt5(double extremeWindSpeed2pt5) {
  m_extremeWindSpeed2pt5 = extremeWindSpeed2pt5;
}

bool EpwDesignCondition::setExtremeWindSpeed5(const std::string& extremeWindSpeed5) {
  bool ok;
  double value = stringToDouble(extremeWindSpeed5, &ok);
  if (!ok) {
    m_extremeWindSpeed5 = boost::none;
  } else {
    setExtremeWindSpeed5(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeWindSpeed5(double extremeWindSpeed5) {
  m_extremeWindSpeed5 = extremeWindSpeed5;
}

bool EpwDesignCondition::setExtremeMaxWetBulb(const std::string& extremeMaxWetBulb) {
  bool ok;
  double value = stringToDouble(extremeMaxWetBulb, &ok);
  if (!ok) {
    m_extremeMaxWetBulb = boost::none;
  } else {
    setExtremeMaxWetBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeMaxWetBulb(double extremeMaxWetBulb) {
  m_extremeMaxWetBulb = extremeMaxWetBulb;
}

bool EpwDesignCondition::setExtremeMeanMinDryBulb(const std::string& extremeMeanMinDryBulb) {
  bool ok;
  double value = stringToDouble(extremeMeanMinDryBulb, &ok);
  if (!ok) {
    m_extremeMeanMinDryBulb = boost::none;
  } else {
    setExtremeMeanMinDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeMeanMinDryBulb(double extremeMeanMinDryBulb) {
  m_extremeMeanMinDryBulb = extremeMeanMinDryBulb;
}

bool EpwDesignCondition::setExtremeMeanMaxDryBulb(const std::string& extremeMeanMaxDryBulb) {
  bool ok;
  double value = stringToDouble(extremeMeanMaxDryBulb, &ok);
  if (!ok) {
    m_extremeMeanMaxDryBulb = boost::none;
  } else {
    setExtremeMeanMaxDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeMeanMaxDryBulb(double extremeMeanMaxDryBulb) {
  m_extremeMeanMaxDryBulb = extremeMeanMaxDryBulb;
}

bool EpwDesignCondition::setExtremeStdDevMinDryBulb(const std::string& extremeStdDevMinDryBulb) {
  bool ok;
  double value = stringToDouble(extremeStdDevMinDryBulb, &ok);
  if (!ok) {
    m_extremeStdDevMinDryBulb = boost::none;
  } else {
    setExtremeStdDevMinDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeStdDevMinDryBulb(double extremeStdDevMinDryBulb) {
  m_extremeStdDevMinDryBulb = extremeStdDevMinDryBulb;
}

bool EpwDesignCondition::setExtremeStdDevMaxDryBulb(const std::string& extremeStdDevMaxDryBulb) {
  bool ok;
  double value = stringToDouble(extremeStdDevMaxDryBulb, &ok);
  if (!ok) {
    m_extremeStdDevMaxDryBulb = boost::none;
  } else {
    setExtremeStdDevMaxDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeStdDevMaxDryBulb(double extremeStdDevMaxDryBulb) {
  m_extremeStdDevMaxDryBulb = extremeStdDevMaxDryBulb;
}

bool EpwDesignCondition::setExtremeN5YearsMinDryBulb(const std::string& extremeN5YearsMinDryBulb) {
  bool ok;
  double value = stringToDouble(extremeN5YearsMinDryBulb, &ok);
  if (!ok) {
    m_extremeN5YearsMinDryBulb = boost::none;
  } else {
    setExtremeN5YearsMinDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeN5YearsMinDryBulb(double extremeN5YearsMinDryBulb) {
  m_extremeN5YearsMinDryBulb = extremeN5YearsMinDryBulb;
}

bool EpwDesignCondition::setExtremeN5YearsMaxDryBulb(const std::string& extremeN5YearsMaxDryBulb) {
  bool ok;
  double value = stringToDouble(extremeN5YearsMaxDryBulb, &ok);
  if (!ok) {
    m_extremeN5YearsMaxDryBulb = boost::none;
  } else {
    setExtremeN5YearsMaxDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeN5YearsMaxDryBulb(double extremeN5YearsMaxDryBulb) {
  m_extremeN5YearsMaxDryBulb = extremeN5YearsMaxDryBulb;
}

bool EpwDesignCondition::setExtremeN10YearsMinDryBulb(const std::string& extremeN10YearsMinDryBulb) {
  bool ok;
  double value = stringToDouble(extremeN10YearsMinDryBulb, &ok);
  if (!ok) {
    m_extremeN10YearsMinDryBulb = boost::none;
  } else {
    setExtremeN10YearsMinDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeN10YearsMinDryBulb(double extremeN10YearsMinDryBulb) {
  m_extremeN10YearsMinDryBulb = extremeN10YearsMinDryBulb;
}

bool EpwDesignCondition::setExtremeN10YearsMaxDryBulb(const std::string& extremeN10YearsMaxDryBulb) {
  bool ok;
  double value = stringToDouble(extremeN10YearsMaxDryBulb, &ok);
  if (!ok) {
    m_extremeN10YearsMaxDryBulb = boost::none;
  } else {
    setExtremeN10YearsMaxDryBulb(value);
  }
  return ok;
}
void EpwDesignCondition::setExtremeN10YearsMaxDryBulb(double extremeN10YearsMaxDryBulb) {
  m_extremeN10YearsMaxDryBulb = extremeN10YearsMaxDryBulb;
}

bool EpwDesignCondition::setExtremeN20YearsMinDryBulb(const std::string& extremeN20YearsMinDryBulb) {
  bool ok;
  double value = stringToDouble(extremeN20YearsMinDryBulb, &ok);
  if (!ok) {
    m_extremeN20YearsMinDryBulb = boost::none;
  } else {
    setExtremeN20YearsMinDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeN20YearsMinDryBulb(double extremeN20YearsMinDryBulb) {
  m_extremeN20YearsMinDryBulb = extremeN20YearsMinDryBulb;
}

bool EpwDesignCondition::setExtremeN20YearsMaxDryBulb(const std::string& extremeN20YearsMaxDryBulb) {
  bool ok;
  double value = stringToDouble(extremeN20YearsMaxDryBulb, &ok);
  if (!ok) {
    m_extremeN20YearsMaxDryBulb = boost::none;
  } else {
    setExtremeN20YearsMaxDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeN20YearsMaxDryBulb(double extremeN20YearsMaxDryBulb) {
  m_extremeN20YearsMaxDryBulb = extremeN20YearsMaxDryBulb;
}

bool EpwDesignCondition::setExtremeN50YearsMinDryBulb(const std::string& extremeN50YearsMinDryBulb) {
  bool ok;
  double value = stringToDouble(extremeN50YearsMinDryBulb, &ok);
  if (!ok) {
    m_extremeN50YearsMinDryBulb = boost::none;
  } else {
    setExtremeN50YearsMinDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeN50YearsMinDryBulb(double extremeN50YearsMinDryBulb) {
  m_extremeN50YearsMinDryBulb = extremeN50YearsMinDryBulb;
}

bool EpwDesignCondition::setExtremeN50YearsMaxDryBulb(const std::string& extremeN50YearsMaxDryBulb) {
  bool ok;
  double value = stringToDouble(extremeN50YearsMaxDryBulb, &ok);
  if (!ok) {
    m_extremeN50YearsMaxDryBulb = boost::none;
  } else {
    setExtremeN50YearsMaxDryBulb(value);
  }
  return ok;
}

void EpwDesignCondition::setExtremeN50YearsMaxDryBulb(double extremeN50YearsMaxDryBulb) {
  m_extremeN50YearsMaxDryBulb = extremeN50YearsMaxDryBulb;
}

std::vector<EpwDesignCondition> EpwFile::designConditions() {
  if (m_designs.empty()) {
    if (!openstudio::filesystem::exists(m_path) || !openstudio::filesystem::is_regular_file(m_path)) {
      LOG_AND_THROW("Path '" << m_path << "' is not an EPW file");
    }

    // set checksum
    m_checksum = openstudio::checksum(m_path);

    // open file
    std::ifstream ifs(openstudio::toSystemFilename(m_path));

    if (!parse(ifs)) {
      ifs.close();
      LOG(Error, "EpwFile '" << toString(m_path) << "' cannot be processed");
    } else {
      ifs.close();
    }
  }
  return m_designs;
}

/*****************************************************************************************************************************************************
*                                           E P W     G R O U N D     T E M P E R A T U R E    D E P T H                                            *
*****************************************************************************************************************************************************/
static constexpr size_t NUMBER_GROUND_TEMP_STRINGS = 16;

EpwGroundTemperatureDepth::EpwGroundTemperatureDepth(double groundTemperatureDepth, double soilConductivity, double soilDensity,
                                                     double soilSpecificHeat, double janGroundTemperature, double febGroundTemperature,
                                                     double marGroundTemperature, double aprGroundTemperature, double mayGroundTemperature,
                                                     double junGroundTemperature, double julGroundTemperature, double augGroundTemperature,
                                                     double sepGroundTemperature, double octGroundTemperature, double novGroundTemperature,
                                                     double decGroundTemperature) {
  setGroundTemperatureDepth(groundTemperatureDepth);
  setSoilConductivity(soilConductivity);
  setSoilDensity(soilDensity);
  setSoilSpecificHeat(soilSpecificHeat);
  setJanGroundTemperature(janGroundTemperature);
  setFebGroundTemperature(febGroundTemperature);
  setMarGroundTemperature(marGroundTemperature);
  setAprGroundTemperature(aprGroundTemperature);
  setMayGroundTemperature(mayGroundTemperature);
  setJunGroundTemperature(junGroundTemperature);
  setJulGroundTemperature(julGroundTemperature);
  setAugGroundTemperature(augGroundTemperature);
  setSepGroundTemperature(sepGroundTemperature);
  setOctGroundTemperature(octGroundTemperature);
  setNovGroundTemperature(novGroundTemperature);
  setDecGroundTemperature(decGroundTemperature);
}

boost::optional<EpwGroundTemperatureDepth> EpwGroundTemperatureDepth::fromGroundTemperatureDepthsString(const std::string& line) {
  std::vector<std::string> strings = splitString(line, ',');
  return fromGroundTemperatureDepthsStrings(strings);
}

boost::optional<EpwGroundTemperatureDepth> EpwGroundTemperatureDepth::fromGroundTemperatureDepthsStrings(const std::vector<std::string>& strings) {
  EpwGroundTemperatureDepth gtd;
  if (strings.size() != NUMBER_GROUND_TEMP_STRINGS) {
    LOG_FREE(Error, "openstudio.EpwFile",
             "Expected " << NUMBER_GROUND_TEMP_STRINGS << " fields in EPW ground temperature depth instead of the " << strings.size() << " received");
    return boost::none;
  }
  // Use the appropriate setter on each field
  gtd.setGroundTemperatureDepth(strings[EpwDepthField::GroundTemperatureDepth]);
  gtd.setSoilConductivity(strings[EpwDepthField::SoilConductivity]);
  gtd.setSoilDensity(strings[EpwDepthField::SoilDensity]);
  gtd.setSoilSpecificHeat(strings[EpwDepthField::SoilSpecificHeat]);
  gtd.setJanGroundTemperature(strings[EpwDepthField::JanGroundTemperature]);
  gtd.setFebGroundTemperature(strings[EpwDepthField::FebGroundTemperature]);
  gtd.setMarGroundTemperature(strings[EpwDepthField::MarGroundTemperature]);
  gtd.setAprGroundTemperature(strings[EpwDepthField::AprGroundTemperature]);
  gtd.setMayGroundTemperature(strings[EpwDepthField::MayGroundTemperature]);
  gtd.setJunGroundTemperature(strings[EpwDepthField::JunGroundTemperature]);
  gtd.setJulGroundTemperature(strings[EpwDepthField::JulGroundTemperature]);
  gtd.setAugGroundTemperature(strings[EpwDepthField::AugGroundTemperature]);
  gtd.setSepGroundTemperature(strings[EpwDepthField::SepGroundTemperature]);
  gtd.setOctGroundTemperature(strings[EpwDepthField::OctGroundTemperature]);
  gtd.setNovGroundTemperature(strings[EpwDepthField::NovGroundTemperature]);
  gtd.setDecGroundTemperature(strings[EpwDepthField::DecGroundTemperature]);
  return std::move(gtd);
}

boost::optional<std::string> EpwGroundTemperatureDepth::getUnitsByName(const std::string& name) {
  EpwDepthField id;
  try {
    id = EpwDepthField(name);
  } catch (...) {
    // Could do a warning message here
    return boost::none;
  }
  return getUnits(id);
}

std::string EpwGroundTemperatureDepth::getUnits(EpwDepthField field) {
  switch (field.value()) {
    case EpwDepthField::GroundTemperatureDepth:
      return "m";
      break;
    case EpwDepthField::SoilConductivity:
      return "W/m-K";
      break;
    case EpwDepthField::SoilDensity:
      return "kg/m3";
      break;
    case EpwDepthField::SoilSpecificHeat:
      return "J/kg-K";
      break;
    case EpwDepthField::JanGroundTemperature:
      return "C";
      break;
    case EpwDepthField::FebGroundTemperature:
      return "C";
      break;
    case EpwDepthField::MarGroundTemperature:
      return "C";
      break;
    case EpwDepthField::AprGroundTemperature:
      return "C";
      break;
    case EpwDepthField::MayGroundTemperature:
      return "C";
      break;
    case EpwDepthField::JunGroundTemperature:
      return "C";
      break;
    case EpwDepthField::JulGroundTemperature:
      return "C";
      break;
    case EpwDepthField::AugGroundTemperature:
      return "C";
      break;
    case EpwDepthField::SepGroundTemperature:
      return "C";
      break;
    case EpwDepthField::OctGroundTemperature:
      return "C";
      break;
    case EpwDepthField::NovGroundTemperature:
      return "C";
      break;
    case EpwDepthField::DecGroundTemperature:
      return "C";
      break;
    default:
      //return "None";
      break;
  }
  return "";
}

boost::optional<double> EpwGroundTemperatureDepth::getFieldByName(const std::string& name) {
  EpwDepthField id;
  try {
    id = EpwDepthField(name);
  } catch (...) {
    // Could do a warning message here
    return boost::none;
  }
  return getField(id);
}

boost::optional<double> EpwGroundTemperatureDepth::getField(EpwDepthField id) {
  switch (id.value()) {
    case EpwDepthField::GroundTemperatureDepth:
      return groundTemperatureDepth();
      break;
    case EpwDepthField::SoilConductivity:
      return soilConductivity();
      break;
    case EpwDepthField::SoilDensity:
      return soilDensity();
      break;
    case EpwDepthField::SoilSpecificHeat:
      return soilSpecificHeat();
      break;
    case EpwDepthField::JanGroundTemperature:
      return janGroundTemperature();
      break;
    case EpwDepthField::FebGroundTemperature:
      return febGroundTemperature();
      break;
    case EpwDepthField::MarGroundTemperature:
      return marGroundTemperature();
      break;
    case EpwDepthField::AprGroundTemperature:
      return aprGroundTemperature();
      break;
    case EpwDepthField::MayGroundTemperature:
      return mayGroundTemperature();
      break;
    case EpwDepthField::JunGroundTemperature:
      return junGroundTemperature();
      break;
    case EpwDepthField::JulGroundTemperature:
      return julGroundTemperature();
      break;
    case EpwDepthField::AugGroundTemperature:
      return augGroundTemperature();
      break;
    case EpwDepthField::SepGroundTemperature:
      return sepGroundTemperature();
      break;
    case EpwDepthField::OctGroundTemperature:
      return octGroundTemperature();
      break;
    case EpwDepthField::NovGroundTemperature:
      return novGroundTemperature();
      break;
    case EpwDepthField::DecGroundTemperature:
      return decGroundTemperature();
      break;
    default:
      // Could do a warning message here
      return boost::none;
      break;
  }
  return boost::none;
}

double EpwGroundTemperatureDepth::groundTemperatureDepth() const {
  return m_groundTemperatureDepth;
}

double EpwGroundTemperatureDepth::soilConductivity() const {
  return m_soilConductivity;
}

double EpwGroundTemperatureDepth::soilDensity() const {
  return m_soilDensity;
}

double EpwGroundTemperatureDepth::soilSpecificHeat() const {
  return m_soilSpecificHeat;
}

double EpwGroundTemperatureDepth::janGroundTemperature() const {
  return m_janGroundTemperature;
}

double EpwGroundTemperatureDepth::febGroundTemperature() const {
  return m_febGroundTemperature;
}

double EpwGroundTemperatureDepth::marGroundTemperature() const {
  return m_marGroundTemperature;
}

double EpwGroundTemperatureDepth::aprGroundTemperature() const {
  return m_aprGroundTemperature;
}

double EpwGroundTemperatureDepth::mayGroundTemperature() const {
  return m_mayGroundTemperature;
}

double EpwGroundTemperatureDepth::junGroundTemperature() const {
  return m_junGroundTemperature;
}

double EpwGroundTemperatureDepth::julGroundTemperature() const {
  return m_julGroundTemperature;
}

double EpwGroundTemperatureDepth::augGroundTemperature() const {
  return m_augGroundTemperature;
}

double EpwGroundTemperatureDepth::sepGroundTemperature() const {
  return m_sepGroundTemperature;
}

double EpwGroundTemperatureDepth::octGroundTemperature() const {
  return m_octGroundTemperature;
}

double EpwGroundTemperatureDepth::novGroundTemperature() const {
  return m_novGroundTemperature;
}

double EpwGroundTemperatureDepth::decGroundTemperature() const {
  return m_decGroundTemperature;
}

bool EpwGroundTemperatureDepth::setGroundTemperatureDepth(const std::string& groundTemperatureDepth) {
  bool ok;
  double value = stringToDouble(groundTemperatureDepth, &ok);
  if (!ok) {
    return false;
  }
  setGroundTemperatureDepth(value);
  return ok;
}

void EpwGroundTemperatureDepth::setGroundTemperatureDepth(double groundTemperatureDepth) {
  m_groundTemperatureDepth = groundTemperatureDepth;
}

bool EpwGroundTemperatureDepth::setSoilConductivity(const std::string& soilConductivity) {
  bool ok;
  double value = stringToDouble(soilConductivity, &ok);
  if (!ok) {
    return false;
  }
  setSoilConductivity(value);
  return ok;
}

void EpwGroundTemperatureDepth::setSoilConductivity(double soilConductivity) {
  m_soilConductivity = soilConductivity;
}

bool EpwGroundTemperatureDepth::setSoilDensity(const std::string& soilDensity) {
  bool ok;
  double value = stringToDouble(soilDensity, &ok);
  if (!ok) {
    return false;
  }
  setSoilDensity(value);
  return ok;
}

void EpwGroundTemperatureDepth::setSoilDensity(double soilDensity) {
  m_soilDensity = soilDensity;
}

bool EpwGroundTemperatureDepth::setSoilSpecificHeat(const std::string& soilSpecificHeat) {
  bool ok;
  double value = stringToDouble(soilSpecificHeat, &ok);
  if (!ok) {
    return false;
  }
  setSoilSpecificHeat(value);
  return ok;
}

void EpwGroundTemperatureDepth::setSoilSpecificHeat(double soilSpecificHeat) {
  m_soilSpecificHeat = soilSpecificHeat;
}

bool EpwGroundTemperatureDepth::setJanGroundTemperature(const std::string& janGroundTemperature) {
  bool ok;
  double value = stringToDouble(janGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setJanGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setJanGroundTemperature(double janGroundTemperature) {
  m_janGroundTemperature = janGroundTemperature;
}

bool EpwGroundTemperatureDepth::setFebGroundTemperature(const std::string& febGroundTemperature) {
  bool ok;
  double value = stringToDouble(febGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setFebGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setFebGroundTemperature(double febGroundTemperature) {
  m_febGroundTemperature = febGroundTemperature;
}

bool EpwGroundTemperatureDepth::setMarGroundTemperature(const std::string& marGroundTemperature) {
  bool ok;
  double value = stringToDouble(marGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setMarGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setMarGroundTemperature(double marGroundTemperature) {
  m_marGroundTemperature = marGroundTemperature;
}

bool EpwGroundTemperatureDepth::setAprGroundTemperature(const std::string& aprGroundTemperature) {
  bool ok;
  double value = stringToDouble(aprGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setAprGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setAprGroundTemperature(double aprGroundTemperature) {
  m_aprGroundTemperature = aprGroundTemperature;
}

bool EpwGroundTemperatureDepth::setMayGroundTemperature(const std::string& mayGroundTemperature) {
  bool ok;
  double value = stringToDouble(mayGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setMayGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setMayGroundTemperature(double mayGroundTemperature) {
  m_mayGroundTemperature = mayGroundTemperature;
}

bool EpwGroundTemperatureDepth::setJunGroundTemperature(const std::string& junGroundTemperature) {
  bool ok;
  double value = stringToDouble(junGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setJunGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setJunGroundTemperature(double junGroundTemperature) {
  m_junGroundTemperature = junGroundTemperature;
}

bool EpwGroundTemperatureDepth::setJulGroundTemperature(const std::string& julGroundTemperature) {
  bool ok;
  double value = stringToDouble(julGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setJulGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setJulGroundTemperature(double julGroundTemperature) {
  m_julGroundTemperature = julGroundTemperature;
}

bool EpwGroundTemperatureDepth::setAugGroundTemperature(const std::string& augGroundTemperature) {
  bool ok;
  double value = stringToDouble(augGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setAugGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setAugGroundTemperature(double augGroundTemperature) {
  m_augGroundTemperature = augGroundTemperature;
}

bool EpwGroundTemperatureDepth::setSepGroundTemperature(const std::string& sepGroundTemperature) {
  bool ok;
  double value = stringToDouble(sepGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setSepGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setSepGroundTemperature(double sepGroundTemperature) {
  m_sepGroundTemperature = sepGroundTemperature;
}

bool EpwGroundTemperatureDepth::setOctGroundTemperature(const std::string& octGroundTemperature) {
  bool ok;
  double value = stringToDouble(octGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setOctGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setOctGroundTemperature(double octGroundTemperature) {
  m_octGroundTemperature = octGroundTemperature;
}

bool EpwGroundTemperatureDepth::setNovGroundTemperature(const std::string& novGroundTemperature) {
  bool ok;
  double value = stringToDouble(novGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setNovGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setNovGroundTemperature(double novGroundTemperature) {
  m_novGroundTemperature = novGroundTemperature;
}

bool EpwGroundTemperatureDepth::setDecGroundTemperature(const std::string& decGroundTemperature) {
  bool ok;
  double value = stringToDouble(decGroundTemperature, &ok);
  if (!ok) {
    return false;
  }
  setDecGroundTemperature(value);
  return ok;
}

void EpwGroundTemperatureDepth::setDecGroundTemperature(double decGroundTemperature) {
  m_decGroundTemperature = decGroundTemperature;
}

std::vector<EpwGroundTemperatureDepth> EpwFile::groundTemperatureDepths() {
  if (m_depths.empty()) {
    if (!openstudio::filesystem::exists(m_path) || !openstudio::filesystem::is_regular_file(m_path)) {
      LOG_AND_THROW("Path '" << m_path << "' is not an EPW file");
    }

    // set checksum
    m_checksum = openstudio::checksum(m_path);

    // open file
    std::ifstream ifs(openstudio::toSystemFilename(m_path));

    if (!parse(ifs)) {
      ifs.close();
      LOG(Error, "EpwFile '" << toString(m_path) << "' cannot be processed");
    } else {
      ifs.close();
    }
  }
  return m_depths;
}

boost::optional<TimeSeries> EpwFile::getTimeSeries(const std::string& name) {
  if (m_data.empty()) {
    if (!openstudio::filesystem::exists(m_path) || !openstudio::filesystem::is_regular_file(m_path)) {
      LOG_AND_THROW("Path '" << m_path << "' is not an EPW file");
    }

    // set checksum
    m_checksum = openstudio::checksum(m_path);

    // open file
    std::ifstream ifs(openstudio::toSystemFilename(m_path));

    if (!parse(ifs, true)) {
      ifs.close();
      LOG(Error, "EpwFile '" << toString(m_path) << "' cannot be processed");
      return boost::none;
    }
    ifs.close();
  }
  EpwDataField id;
  try {
    id = EpwDataField(name);
  } catch (...) {
    LOG(Warn, "Unrecognized EPW data field '" << name << "'");
    return boost::none;
  }
  if (!m_data.empty()) {
    std::string units = EpwDataPoint::getUnits(id);
    DateTimeVector dates;
    dates.push_back(DateTime());  // Use a placeholder to avoid an insert
    std::vector<double> values;
    for (unsigned int i = 0; i < m_data.size(); i++) {
      DateTime dateTime = m_data[i].dateTime();
      // Time time=m_data[i].time();
      boost::optional<double> value = m_data[i].getField(id);
      if (value) {
        if (isActual()) {
          dates.push_back(DateTime(dateTime));
        } else {
          // Strip year
          dates.push_back(DateTime(Date(dateTime.date().monthOfYear(), dateTime.date().dayOfMonth()), dateTime.time()));
        }
        values.push_back(value.get());
      }
    }
    if (!values.empty()) {
      DateTime start = dates[1] - Time(0, 0, 0, 3600 / m_recordsPerHour);
      dates[0] = start;  // Overwrite the placeholder
      return boost::optional<TimeSeries>(TimeSeries(dates, openstudio::createVector(values), units));
    }
  }
  return boost::none;
}

boost::optional<TimeSeries> EpwFile::getComputedTimeSeries(const std::string& name) {
  if (m_data.empty()) {
    if (!openstudio::filesystem::exists(m_path) || !openstudio::filesystem::is_regular_file(m_path)) {
      LOG_AND_THROW("Path '" << m_path << "' is not an EPW file");
    }

    // set checksum
    m_checksum = openstudio::checksum(m_path);

    // open file
    std::ifstream ifs(openstudio::toSystemFilename(m_path));

    if (!parse(ifs, true)) {
      ifs.close();
      LOG(Error, "EpwFile '" << toString(m_path) << "' cannot be processed");
      return boost::none;
    }
    ifs.close();
  }
  EpwComputedField id;
  try {
    id = EpwComputedField(name);
  } catch (...) {
    LOG(Warn, "Unrecognized computed data field '" << name << "'");
    return boost::none;
  }

  std::string units = EpwDataPoint::getUnits(id);
  boost::optional<double> (EpwDataPoint::*compute)() const;
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
  dates.push_back(DateTime());  // Use a placeholder to avoid an insert
  std::vector<double> values;
  for (unsigned int i = 0; i < m_data.size(); i++) {
    Date date = m_data[i].date();
    Time time = m_data[i].time();
    boost::optional<double> value = (m_data[i].*compute)();
    if (value) {
      dates.push_back(DateTime(date, time));
      values.push_back(value.get());
    }
  }
  if (!values.empty()) {
    DateTime start = dates[1] - Time(0, 0, 0, 3600 / m_recordsPerHour);
    dates[0] = start;  // Overwrite the placeholder
    return boost::optional<TimeSeries>(TimeSeries(dates, openstudio::createVector(values), units));
  }
  return boost::none;
}

bool EpwFile::translateToWth(openstudio::path path, std::string description) {
  if (m_data.empty()) {
    if (!openstudio::filesystem::exists(m_path) || !openstudio::filesystem::is_regular_file(m_path)) {
      LOG_AND_THROW("Path '" << m_path << "' is not an EPW file");
    }

    // set checksum
    m_checksum = openstudio::checksum(m_path);

    // open file
    std::ifstream ifs(openstudio::toSystemFilename(m_path));

    if (!parse(ifs, true)) {
      ifs.close();
      LOG(Error, "EpwFile '" << toString(m_path) << "' cannot be processed");
      return false;
    }
    ifs.close();
  }

  if (description.empty()) {
    description = "Translated from " + openstudio::toString(this->path());
  }

  if (data().empty()) {
    LOG(Error, "EPW file contains no data to translate");
    return false;
  }

  openstudio::filesystem::ofstream fp(path, std::ios_base::binary);
  if (!fp.is_open()) {
    LOG(Error, "Failed to open file '" + openstudio::toString(path) + "'");
    return false;
  }

  fp << "WeatherFile ContamW 2.0\n";
  fp << description << "\n";
  fp << fmt::format("{}/{}\t!start date\n", openstudio::month(startDate().monthOfYear()), startDate().dayOfMonth());
  fp << fmt::format("%1/%2\t!end date\n", openstudio::month(endDate().monthOfYear()), endDate().dayOfMonth());
  fp << "!Date\tDofW\tDtype\tDST\tTgrnd [K]\n";
  openstudio::Time delta(1, 0);
  int dayofweek = startDayOfWeek().value() + 1;
  for (openstudio::Date current = startDate(); current <= endDate(); current += delta) {
    fp << fmt::format("%1/%2\t%3\t%3\t0\t283.15\n", openstudio::month(current.monthOfYear()), current.dayOfMonth(), dayofweek);
    dayofweek++;
    if (dayofweek > 7) {
      dayofweek = 1;
    }
  }

  // Cheat to get data at the start time - this will need to change
  openstudio::EpwDataPoint lastPt = data()[data().size() - 1];
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

  fp << "!Date\tTime\tTa [K]\tPb [Pa]\tWs [m/s]\tWd [deg]\tHr [g/kg]\tIth [kJ/m^2]\tIdn [kJ/m^2]\tTs [K]\tRn [-]\tSn [-]\n";
  boost::optional<std::string> output = firstPtOpt.get().toWthString();
  if (!output) {
    LOG(Error, "Translation to WTH has failed on starting data point");
    fp.close();
    return false;
  }
  fp << output.get() << '\n';
  for (unsigned int i = 0; i < data().size(); i++) {
    output = data()[i].toWthString();
    if (!output) {
      LOG(Error, "Translation to WTH has failed on data point " << i);
      fp.close();
      return false;
    }
    fp << output.get() << '\n';
  }
  fp.close();
  return true;
}

bool EpwFile::parse(std::istream& ifs, bool storeData) {
  // read line by line
  std::string line;

  bool result = true;

  // read first 8 lines
  for (unsigned i = 0; i < 8; ++i) {

    if (!std::getline(ifs, line)) {
      LOG(Error, "Could not read line " << i + 1 << " of EPW file '" << m_path << "'");
      return false;
    }

    switch (i) {
      case 0:  // LOCATION,
        result = result && parseLocation(line);
        break;
      case 1:  // DESIGN CONDITIONS
        result = result && parseDesignConditions(line);
        break;
      case 2:  // TYPICAL/EXTREME PERIODS
        break;
      case 3:  // GROUND TEMPERATURES
        result = result && parseGroundTemperatures(line);
        break;
      case 4:  // HOLIDAYS/DAYLIGHT SAVINGS
        result = result && parseHolidaysDaylightSavings(line);
        break;
      case 5:  // COMMENTS 1
        break;
      case 6:  // COMMENTS 2
        break;
      case 7:  // DATA PERIODS
        result = result && parseDataPeriod(line);
        break;
      default:;
    }
  }

  if (!result) {
    LOG(Error, "Failed to parse EPW file header '" << m_path << "'");
    return false;
  }

  struct EPWString
  {
    const int lineNumber;
    const int year;
    const int month;
    const int day;
    const int hour;
    const int currentMinute;
    std::vector<std::string> strings;
  };

  // read rest of file
  int lineNumber = 8;
  boost::optional<Date> startDate;
  boost::optional<Date> lastDate;
  boost::optional<Date> endDate;
  bool realYear = true;
  bool wrapAround = false;
  OS_ASSERT((60 % m_recordsPerHour) == 0);
  int minutesPerRecord = 60 / m_recordsPerHour;
  int currentMinute = 0;
  std::vector<EPWString> epw_strings;
  while (std::getline(ifs, line)) {
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
            if (realYear) {  // Warn once
              LOG(Info, "Successive data points (" << lastDate.get() << " to " << endDate.get() << ", ending on line " << lineNumber
                                                   << ") are greater than 1 day apart in EPW file '" << m_path
                                                   << "'. Data will be treated as typical (TMY)");
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
            if (currentMinute >= 60) {  // This could really be ==, but >= is used for safety
              currentMinute = 0;
            }
          }
          // Check for agreement between the file value and the computed value
          if (currentMinute != minutesInFile) {
            if (m_minutesMatch) {  // Warn only once
              LOG(Error, "Minutes field (" << minutesInFile << ") on line " << lineNumber << " of EPW file '" << m_path
                                           << "' does not agree with computed value (" << currentMinute << "). Using computed value");
              m_minutesMatch = false;
            }
          }
          epw_strings.push_back({lineNumber, year, month, day, hour, currentMinute, strings});
        }

      } catch (...) {
        LOG(Error, "Could not read line " << lineNumber << " of EPW file '" << m_path << "'");
        return false;
      }
    } else {
      LOG(Error, "Insufficient weather data on line " << lineNumber << " of EPW file '" << m_path << "'");
      return false;
    }
  }

  for (const auto& epw_string : epw_strings) {
    int day = epw_string.day;

    // Fix for #5214: for TMY files where 28-day Feb is from a leap year, we need xxxx-Mar-01 00:00:00 and not xxxx-Feb-29 00:00:00
    // If the TMY file actually contained Feb 29, we still change xxxx-Feb-29 00:00:00 to xxxx-Mar-01 00:00:00, but it doesn't matter since you'd still get Bad Date on xxxx-Feb-29 01:00:00, etc.
    if ((!realYear) && (epw_string.month == 2) && (day == 28) && (epw_string.hour == 24) && (epw_string.currentMinute == 0)) {
      Date date(epw_string.month, day, epw_string.year);
      if (date.isLeapYear()) {
        day = 29;
      }
    }

    boost::optional<EpwDataPoint> pt =
      EpwDataPoint::fromEpwStrings(epw_string.year, epw_string.month, day, epw_string.hour, epw_string.currentMinute, epw_string.strings);
    if (pt) {
      m_data.push_back(pt.get());
    } else {
      LOG(Error, "Failed to parse line " << epw_string.lineNumber << " of EPW file '" << m_path << "'");
      return false;
    }
  }

  if (!startDate) {
    LOG(Error, "Could not find start date in data section of EPW file '" << m_path << "'");
    return false;
  }
  if (!endDate) {
    LOG(Error, "Could not find end date in data section of EPW file '" << m_path << "'");
    return false;
  }

  if ((m_startDate.monthOfYear() != startDate->monthOfYear()) || (m_startDate.dayOfMonth() != startDate->dayOfMonth())) {
    LOG(Error, "Header start date does not match data in EPW file '" << m_path << "'");
    return false;
  }

  if ((m_endDate.monthOfYear() != endDate->monthOfYear()) || (m_endDate.dayOfMonth() != endDate->dayOfMonth())) {
    LOG(Error, "Header end date does not match data in EPW file '" << m_path << "'");
    return false;
  }

  if (realYear) {
    if (m_startDayOfWeek != startDate->dayOfWeek()) {
      LOG(Info, "Header start day of the week and actual start day of the week do not match in EPW file '"
                  << m_path << "'. Data will be treated as typical (TMY)");
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

  if (!realYear && wrapAround) {
    LOG(Error, "Wrap around years not supported for TMY data, EPW file '" << m_path << "'");
    return false;
  }

  if (realYear) {
    m_isActual = true;
  }

  return result;
}

bool EpwFile::parseLocation(const std::string& line) {
  // LOCATION,Chicago Ohare Intl Ap,IL,USA,TMY3,725300,41.98,-87.92,-6.0,201.0
  // LOCATION, city, stateProvinceRegion, country, dataSource, wmoNumber, latitude, longitude, timeZone, elevation
  std::vector<std::string> split = splitString(line, ',');
  if (split.size() < 10) {
    LOG(Error, "Expected 10 location fields rather than the " << split.size() << " fields in EPW file '" << m_path << "'");
    return false;
  } else if (split.size() > 10) {
    LOG(Warn, "Expected 10 location fields rather than the " << split.size() << " fields in EPW file '" << m_path
                                                             << "', additional fields will be ignored");
  }

  if (split[0] != "LOCATION") {
    LOG(Error, "Missing LOCATION specifier in EPW file '" << m_path << "'");
    return false;
  }

  m_city = split[1];
  boost::trim(m_city);
  m_stateProvinceRegion = split[2];
  boost::trim(m_stateProvinceRegion);
  m_country = split[3];
  boost::trim(m_country);
  m_dataSource = split[4];
  boost::trim(m_dataSource);
  m_wmoNumber = split[5];
  boost::trim(m_wmoNumber);

  std::string latitude = split[6];
  boost::trim(latitude);
  std::string longitude = split[7];
  boost::trim(longitude);
  std::string timeZone = split[8];
  boost::trim(timeZone);
  std::string elevation = split[9];
  boost::trim(elevation);

  try {
    m_latitude = std::stod(latitude);
  } catch (...) {
    LOG(Error, "Non-numerical latitude in EPW file '" << m_path << "'");
    return false;
  }
  try {
    m_longitude = std::stod(longitude);
  } catch (...) {
    LOG(Error, "Non-numerical longitude in EPW file '" << m_path << "'");
    return false;
  }
  try {
    m_timeZone = std::stod(timeZone);
  } catch (...) {
    LOG(Error, "Non-numerical timezone in EPW file '" << m_path << "'");
    return false;
  }
  try {
    m_elevation = std::stod(elevation);
  } catch (...) {
    LOG(Error, "Non-numerical elevation in EPW file '" << m_path << "'");
    return false;
  }

  return true;
}

bool EpwFile::parseDesignConditions(const std::string& line) {
  // DESIGN CONDITIONS,1,Climate Design Data 2009 ASHRAE Handbook,,Heating,12,-17.4,-14,-21.5,0.7,-11.7,-18.9,0.9,-6.9,14.1,1.8,12,2.4,3.3,160,Cooling,7,15.2,34.6,15.7,33.2,15.6,31.8,15.4,18.3,27.3,17.6,27,17,26.5,4.2,80,16,14,19.9,15.2,13.2,19.7,14.1,12.3,19.6,58.3,27,55.9,26.9,53.8,26.3,722,Extremes,11.9,10.4,8.8,20.7,-22.7,37.1,2.8,1.3,-24.7,38,-26.3,38.8,-27.9,39.5,-29.9,40.5
  // DESIGN CONDITIONS,Number of Design Conditions,Title of Design Condition,Design Stat,HDB 99.6%,HDB 99%,X WS 1%,X WS 2.5%,X WS 5%,CM WS .4%,CM MDB .4%,CM WS 1%,CM MDB 1%,MWS 99.6%,PWD 99.6%,MWS .4%,PWD .4%,X MnDB Max,X MnDB Min,X StdDB Max,X StdDB Min,Design Stat,CDB .4%,C MWB .4%,CDB 1%,C MWB 1%,CDB 2%,C MWB 2%,E WB .4%,E MDB .4%,E WB 1%,E MDB 1%,E WB 2%,E MDB 2%,DP .4%,HR .4%,MDB .4%,DP 1%,HR 1%,MDB 1%,DP 2%,HR 2%,MDB 2%,DB Range
  // Bail out if the design conditions array already has contents
  if (!m_designs.empty()) {
    return true;
  }

  std::vector<std::string> split = splitString(line, ',');

  if (split[0] != "DESIGN CONDITIONS") {
    LOG(Error, "Missing DESIGN CONDITIONS specifier in EPW file '" << m_path << "'");
    return false;
  } else if (split[1] == "0") {
    LOG(Warn, "Appears there are no design condition fields in the EPW file '" << m_path << "'");
    return true;
  }

  int nDesignConditions = std::stoi(split[1]);

  double expected_split_size = 70;
  expected_split_size += (nDesignConditions - 1) * 68;

  if (split.size() != expected_split_size) {
    LOG(Warn, "Expected " << expected_split_size << " design condition fields rather than the " << split.size() << " fields in the EPW file '"
                          << m_path << "'. Design conditions will not be parsed.");
    nDesignConditions = 0;
  } else if (nDesignConditions > 1) {
    LOG(Warn, "Found " << nDesignConditions << " in the EPW file '" << m_path << "'");
  }

  for (int j = 0; j < nDesignConditions; j++) {
    std::vector<std::string> design_condition(68);
    for (int k = 0; k < 68; k++) {
      design_condition[k] = split[k + 2 + (68 * j)];
    }
    boost::optional<EpwDesignCondition> dc = EpwDesignCondition::fromDesignConditionsStrings(design_condition);
    if (dc) {
      m_designs.push_back(dc.get());
    } else {
      LOG(Error, "Failed to parse design condition " << j + 1 << " of EPW file '" << m_path << "'");
      return false;
    }
  }
  return true;
}

bool EpwFile::parseGroundTemperatures(const std::string& line) {
  // GROUND TEMPERATURES,3,.5,,,,-0.60,1.34,5.12,8.69,15.46,19.02,20.00,18.20,14.02,8.83,3.71,0.32,2,,,,2.08,2.55,4.70,7.10,12.30,15.62,17.28,16.91,14.53,10.94,6.90,3.72,4,,,,4.84,4.51,5.45,6.81,10.25,12.82,14.49,14.90,13.86,11.74,9.00,6.53
  // Number of Ground Temperature Depths,Ground Temperature Depth {m},Soil Conductivity {W/m-K},Soil Density {kg/m3},Soil Specific Heat {J/kg-K},Jan {C},Feb{C},Mar {C},Apr {C},May {C},Jun {C},Jul {C},Aug {C},Sep {C},Oct {C},Nov {C},Dec {C},<repeat to Number of temperature depths>
  // Bail out if the ground temperature depths array already has contents
  if (!m_depths.empty()) {
    return true;
  }

  std::vector<std::string> split = splitString(line, ',');

  if (split[0] != "GROUND TEMPERATURES") {
    LOG(Error, "Missing GROUND TEMPERATURES specifier in EPW file '" << m_path << "'");
    return false;
  } else if (split[1] == "0") {
    LOG(Warn, "Appears there are no ground temperature depth fields in the EPW file '" << m_path << "'");
    return true;
  }

  size_t nGroundTemperatureDepths = std::stoi(split[1]);
  size_t expected_split_size = 2 + (nGroundTemperatureDepths * NUMBER_GROUND_TEMP_STRINGS);

  if (split.size() != expected_split_size) {
    LOG(Error, "Expected " << expected_split_size << " ground temperature depth fields rather than the " << split.size()
                           << " fields in the EPW file '" << m_path << "'. Ground temperature depths will not be parsed.");
    return false;
  }
  LOG(Info, "Found " << nGroundTemperatureDepths << " ground temperature depths in the EPW file '" << m_path << "'");

  for (size_t i = 0; i < nGroundTemperatureDepths; ++i) {
    std::vector<std::string> ground_temperature_depth(std::next(split.begin(), 2 + NUMBER_GROUND_TEMP_STRINGS * i),
                                                      std::next(split.begin(), 2 + NUMBER_GROUND_TEMP_STRINGS * (i + 1)));
    boost::optional<EpwGroundTemperatureDepth> gtd = EpwGroundTemperatureDepth::fromGroundTemperatureDepthsStrings(ground_temperature_depth);
    if (gtd) {
      m_depths.push_back(std::move(*gtd));
    } else {
      LOG(Error, "Failed to parse ground temperature depth group " << i + 1 << " of EPW file '" << m_path << "'");
      return false;
    }
  }
  // Ensure sorted by lower to higher depths
  std::sort(m_depths.begin(), m_depths.end(),
            [](const auto& lhs, const auto& rhs) { return lhs.groundTemperatureDepth() < rhs.groundTemperatureDepth(); });
  return true;
}

bool EpwFile::parseHolidaysDaylightSavings(const std::string& line) {

  // HOLIDAYS/DAYLIGHT SAVINGS,No, 4/29,10/28,9,Hol:001, 1/ 1,Hol:002, 2/19,Hol:003, 5/28,Hol:004, 7/ 4,Hol:005, 9/ 3,Hol:006,10/ 8,Hol:007,11/12,Hol:008,11/22,Hol:009,12/25
  // specified, leapYearObserved, daylightSavingStartDay, daylightSavingEndDay, numberOfHolidays, FieldSet:<holidayName , holidayDay>

  std::vector<std::string> split = splitString(line, ',');
  std::string specifier = split[0];
  boost::trim(specifier);
  if (!istringEqual(specifier, "HOLIDAYS/DAYLIGHT SAVINGS")) {
    LOG(Error, "Missing 'HOLIDAYS/DAYLIGHT SAVINGS' specifier in EPW file '" << m_path << "'");
    return false;
  }

  std::string leapYearObserved = split[1];
  boost::trim(leapYearObserved);
  if (istringEqual("Yes", leapYearObserved)) {
    m_leapYearObserved = true;
  } else {
    m_leapYearObserved = false;
  }

  std::string daylightSavingStartDay = split[2];
  boost::trim(daylightSavingStartDay);
  std::string daylightSavingEndDay = split[3];
  boost::trim(daylightSavingEndDay);

  bool processDaylightDates = true;
  if (daylightSavingStartDay == "0") {
    LOG(Debug, "No Daylightings Saving start Date");
    processDaylightDates = false;
  } else {
    if (daylightSavingEndDay == "0") {
      LOG(Error, "No Daylightings Saving End Date, skipping.");
      processDaylightDates = false;
    } else {

      try {
        // Parse start month and day, optional year
        std::vector<std::string> dateSplit = splitString(daylightSavingStartDay, '/');
        if (dateSplit.size() != 2 && dateSplit.size() != 3) {
          LOG(Error, "Bad data Daylight Savings Start Day format '" << daylightSavingStartDay << "' in EPW file '" << m_path << "'");
          return false;
        }
        int month = std::stoi(dateSplit[0]);
        int day = std::stoi(dateSplit[1]);
        if (dateSplit.size() == 3) {
          int year = std::stoi(dateSplit[1]);
          m_daylightSavingStartDate = Date(monthOfYear(month), day, year);
        } else {
          m_daylightSavingStartDate = Date(monthOfYear(month), day);
        }
      } catch (...) {
        LOG(Error, "Failed to parse Daylight Savings Start Day '" << daylightSavingStartDay << "' in EPW file '" << m_path << "'");
        return false;
      }
    }
  }

  if (processDaylightDates) {

    try {
      // Parse end month and day, optional year
      std::vector<std::string> dateSplit = splitString(daylightSavingEndDay, '/');
      if (dateSplit.size() != 2 && dateSplit.size() != 3) {
        LOG(Error, "Bad data Daylight Savings End Day format '" << daylightSavingEndDay << "' in EPW file '" << m_path << "'");
        return false;
      }
      int month = std::stoi(dateSplit[0]);
      int day = std::stoi(dateSplit[1]);
      if (dateSplit.size() == 3) {
        int year = std::stoi(dateSplit[1]);
        m_daylightSavingEndDate = Date(monthOfYear(month), day, year);
      } else {
        m_daylightSavingEndDate = Date(monthOfYear(month), day);
      }
    } catch (...) {
      LOG(Error, "Failed to parse Daylight Savings End Day '" << daylightSavingEndDay << "' in EPW file '" << m_path << "'");
      return false;
    }
  }

  std::string numberOfHolidays = split[4];
  boost::trim(numberOfHolidays);
  int nHolidays = 0;
  try {
    nHolidays = std::stoi(numberOfHolidays);
  } catch (...) {
    LOG(Error, "Non-integral number of holidays in EPW file '" << m_path << "'");
    return false;
  }

  for (int i = 5; i < 5 + (2 * nHolidays); i += 2) {
    std::string holidayName = split[i];
    boost::trim(holidayName);
    std::string holidayDay = split[i + 1];
    boost::trim(holidayDay);
    // Need to parse the dates?

    if (holidayName.empty() || holidayDay.empty()) {
      LOG(Error,
          "Empty Holiday Day or Day for entry " << i << ": (holidayName, holidayDay) = (" << holidayName << ", " << holidayDay << "). Skipping.");
      continue;
    }
    m_holidays.push_back(EpwHoliday(holidayName, holidayDay));
  }

  return true;
}

boost::optional<Date> EpwFile::daylightSavingStartDate() const {
  return m_daylightSavingStartDate;
}

boost::optional<Date> EpwFile::daylightSavingEndDate() const {
  return m_daylightSavingEndDate;
}

std::vector<EpwHoliday> EpwFile::holidays() const {
  return m_holidays;
}

bool EpwFile::parseDataPeriod(const std::string& line) {
  // DATA PERIODS,1,1,Data,Sunday, 1/ 1,12/31
  // DATA PERIODS, nDataPeriods, timeStep, startDayOfWeek, startDate, endDate
  // NOTE THAT ONLY ONE DATA PERIOD IS SUPPORTED
  std::vector<std::string> split = splitString(line, ',');
  if (split.size() < 7) {
    LOG(Error, "Expected 7 data period fields rather than the " << split.size() << " fields in EPW file '" << m_path << "'");
    return false;
  } else if (split.size() > 7) {
    LOG(Warn, "Expected 7 data period fields rather than the " << split.size() << " fields in EPW file '" << m_path
                                                               << "', additional fields will be ignored");
  }

  if (split[0] != "DATA PERIODS") {
    LOG(Error, "Missing DATA PERIODS specifier in EPW file '" << m_path << "'");
    return false;
  }

  std::string nDataPeriods = split[1];
  boost::trim(nDataPeriods);
  std::string timeStep = split[2];
  boost::trim(timeStep);
  std::string startDayOfWeek = split[4];
  boost::trim(startDayOfWeek);
  std::string startDate = split[5];
  boost::trim(startDate);
  std::string endDate = split[6];
  boost::trim(endDate);

  try {
    int N = std::stoi(nDataPeriods);
    if (N > 1) {
      LOG(Error, "More than one data period in EPW file '" << m_path << "', which is not supported");
      return false;
    }
  } catch (...) {
    LOG(Error, "Non-integral number of data periods in EPW file '" << m_path << "'");
    return false;
  }

  try {
    m_recordsPerHour = std::stoi(timeStep);
    if ((60 % m_recordsPerHour) != 0) {
      LOG(Error, "Number of records per hour of " << m_recordsPerHour
                                                  << " does not result in integral number of minutes between records in EPW file '" << m_path << "'");
      return false;
    }
  } catch (...) {
    LOG(Error, "Non-integral timestep in EPW file '" << m_path << "'");
    return false;
  }

  try {
    m_startDayOfWeek = DayOfWeek(startDayOfWeek);
  } catch (...) {
    LOG(Error, "Bad start day of week in EPW file '" << m_path << "'");
    return false;
  }

  try {
    // Parse start month and day, optional year
    split = splitString(startDate, '/');
    if (split.size() != 2 && split.size() != 3) {
      LOG(Error, "Bad data period start date format '" << startDate << "' in EPW file '" << m_path << "'");
      return false;
    }
    int month = std::stoi(split[0]);
    int day = std::stoi(split[1]);
    if (split.size() == 3) {
      int year = std::stoi(split[2]);
      m_startDate = Date(monthOfYear(month), day, year);
      m_startDateActualYear = year;
    } else {
      m_startDate = Date(monthOfYear(month), day);
    }
  } catch (...) {
    LOG(Error, "Failed to parse data period start date from '" << startDate << "' in EPW file '" << m_path << "'");
    return false;
  }

  try {
    // Parse end month and day, optional year
    split = splitString(endDate, '/');
    if (split.size() != 2 && split.size() != 3) {
      LOG(Error, "Bad data period end date format '" << startDate << "' in EPW file '" << m_path << "'");
      return false;
    }
    int month = std::stoi(split[0]);
    int day = std::stoi(split[1]);
    if (split.size() == 3) {
      int year = std::stoi(split[2]);
      m_endDate = Date(monthOfYear(month), day, year);
      m_endDateActualYear = year;
    } else {
      m_endDate = Date(monthOfYear(month), day);
    }
  } catch (...) {
    LOG(Error, "Failed to parse data period end date from '" << startDate << "' in EPW file '" << m_path << "'");
    return false;
  }

  return true;
}

bool EpwFile::isActual() const {
  return m_isActual;
}

bool EpwFile::minutesMatch() const {
  return m_minutesMatch;
}

IdfObject toIdfObject(const EpwFile& epwFile) {
  IdfObject result(IddObjectType::OS_WeatherFile);

  bool success = true;

  success = success && result.setString(0, epwFile.city());
  success = success && result.setString(1, epwFile.stateProvinceRegion());
  success = success && result.setString(2, epwFile.country());
  success = success && result.setString(3, epwFile.dataSource());
  success = success && result.setString(4, epwFile.wmoNumber());
  success = success && result.setDouble(5, epwFile.latitude());
  success = success && result.setDouble(6, epwFile.longitude());
  success = success && result.setDouble(7, epwFile.timeZone());
  success = success && result.setDouble(8, epwFile.elevation());
  success = success && result.setString(9, toString(epwFile.path()));
  success = success && result.setString(10, epwFile.checksum());

  if (!success) {
    LOG_FREE(Error, "openstudio.EpwFile", "Unable to construct WeatherFile IdfObject from epwFile at path " << epwFile.path() << ".");
  }

  return result;
}

}  // namespace openstudio
