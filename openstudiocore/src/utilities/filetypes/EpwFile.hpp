/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_EPWFILE_HPP
#define UTILITIES_FILETYPES_EPWFILE_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Path.hpp"
#include "../core/Logger.hpp"
#include "../time/Time.hpp"
#include "../time/Date.hpp"
#include "../time/DateTime.hpp"
#include "../data/TimeSeries.hpp"

namespace openstudio{

// forward declaration
class IdfObject;
// Added these next four for SWIG
class Date;
class Time;
class DateTime;
class TimeSeries;

/** The AirState object represents a moist air state */
class UTILITIES_API AirState
{
public:
  /** Create a air state object at 25C, 101325 Pa, 50% RH */
  AirState();

  // Statics
  /** Attempt to create a moist air state for dry bulb temperature, dew point temperature, and pressure */
  static boost::optional<AirState> fromDryBulbDewPointPressure(double drybulb, double dewpoint, double pressure);
  /** Attempt to create a moist air state for dry bulb temperature, relative, and pressure */
  static boost::optional<AirState> fromDryBulbRelativeHumidityPressure(double drybulb, double RH, double pressure);

  /** Returns the dry bulb temperature in C*/
  double drybulb() const;
  /** Returns the dew point temperature in C*/
  double dewpoint() const;
  /** Returns the wet bulb temperature in C*/
  double wetbulb() const;
  /** Returns the relative humidity in percent*/
  double relativeHumidity() const;
  /** Returns the pressure in Pa*/
  double pressure() const;

  /** Returns the enthalpy kJ/kg*/
  double enthalpy() const;
  /** Returns the saturation pressure in Pa*/
  double saturationPressure() const;
  /** Returns the density in kg/m3*/
  double density() const;
  /** Returns the specific volume m3/kg*/
  double specificVolume() const;
  /** Returns the humidity ratio */
  double humidityRatio() const;

  /** Returns the air gas constant */
  static double R();

private:
  double m_drybulb; // Dry bulb temperature in C
  double m_dewpoint; // Dew point temperature in C
  double m_pressure; // Atmospheric pressure in Pa
  double m_wetbulb; // Thermodynamic wet bulb temperature in C

  // These are always computed
  double m_psat;
  double m_W;
  double m_h;
  double m_phi;
  double m_v;
};

OPENSTUDIO_ENUM(EpwDataField,
  ((Year)(Year)(0))
  ((Month)(Month))
  ((Day)(Day))
  ((Hour)(Hour))
  ((Minute)(Minute))
  ((DataSourceandUncertaintyFlags)(Data Source and Uncertainty Flags))
  ((DryBulbTemperature)(Dry Bulb Temperature))
  ((DewPointTemperature)(Dew Point Temperature))
  ((RelativeHumidity)(Relative Humidity))
  ((AtmosphericStationPressure)(Atmospheric Station Pressure))
  ((ExtraterrestrialHorizontalRadiation)(Extraterrestrial Horizontal Radiation))
  ((ExtraterrestrialDirectNormalRadiation)(Extraterrestrial Direct Normal Radiation))
  ((HorizontalInfraredRadiationIntensity)(Horizontal Infrared Radiation Intensity))
  ((GlobalHorizontalRadiation)(Global Horizontal Radiation))
  ((DirectNormalRadiation)(Direct Normal Radiation))
  ((DiffuseHorizontalRadiation)(Diffuse Horizontal Radiation))
  ((GlobalHorizontalIlluminance)(Global Horizontal Illuminance))
  ((DirectNormalIlluminance)(Direct Normal Illuminance))
  ((DiffuseHorizontalIlluminance)(Diffuse Horizontal Illuminance))
  ((ZenithLuminance)(Zenith Luminance))
  ((WindDirection)(Wind Direction))
  ((WindSpeed)(Wind Speed))
  ((TotalSkyCover)(Total Sky Cover))
  ((OpaqueSkyCover)(Opaque Sky Cover))
  ((Visibility))
  ((CeilingHeight)(Ceiling Height))
  ((PresentWeatherObservation)(Present Weather Observation))
  ((PresentWeatherCodes)(Present Weather Codes))
  ((PrecipitableWater)(Precipitable Water))
  ((AerosolOpticalDepth)(Aerosol Optical Depth))
  ((SnowDepth)(Snow Depth))
  ((DaysSinceLastSnowfall)(Days Since Last Snowfall))
  ((Albedo))
  ((LiquidPrecipitationDepth)(Liquid Precipitation Depth))
  ((LiquidPrecipitationQuantity)(Liquid Precipitation Quantity))
);

OPENSTUDIO_ENUM(EpwComputedField,
  ((SaturationPressure)(Saturation Pressure)(0))
  ((Enthalpy)(Enthalpy))
  ((HumidityRatio)(Humidity Ratio))
  ((WetBulbTemperature)(Wet Bulb Temperature))
  ((Density)(Density))
  ((SpecificVolume)(Specific Volume))
  );

OPENSTUDIO_ENUM(EpwDesignField,
  ((TitleOfDesignCondition)(Title of Design Condition)(0))
  ((Blank)(Blank))
  ((Heating)(Heating))
  ((HeatingColdestMonth)(Heating Coldest Month))
  ((HeatingDryBulb99pt6)(Heating Dry Bulb Temperature 99.6%))
  ((HeatingDryBulb99)(Heating Dry Bulb Temperature 99%))
  ((HeatingHumidificationDewPoint99pt6)(Heating Humidification Dew Point Temperature 99.6%))
  ((HeatingHumidificationHumidityRatio99pt6)(Heating Humidification Humidity Ratio 99.6%))
  ((HeatingHumidificationMeanCoincidentDryBulb99pt6)(Heating Humidification Mean Coincident Dry Bulb Temperature 99.6%))
  ((HeatingHumidificationDewPoint99)(Heating Humidification Dew Point Temperature 99%))
  ((HeatingHumidificationHumidityRatio99)(Heating Humidification Humidity Ratio 99%))
  ((HeatingHumidificationMeanCoincidentDryBulb99)(Heating Humidification Mean Coincident Dry Bulb 99%))
  ((HeatingColdestMonthWindSpeed0pt4)(Heating Coldest Month Wind Speed 0.4%))
  ((HeatingColdestMonthMeanCoincidentDryBulb0pt4)(Heating Coldest Month Mean Coincident Dry Bulb 0.4%))
  ((HeatingColdestMonthWindSpeed1)(Heating Coldest Month Wind Speed 1%))
  ((HeatingColdestMonthMeanCoincidentDryBulb1)(Heating Coldest Month Mean Coincident Dry Bulb 1%))
  ((HeatingMeanCoincidentWindSpeed99pt6)(Heating Mean Coincident Wind Speed))
  ((HeatingPrevailingCoincidentWindDirection99pt6)(Heating Prevailing Coincident Wind Direction 99.6%))
  ((Cooling)(Cooling))
  ((CoolingHottestMonth)(Cooling Hottest Month))
  ((CoolingDryBulbRange)(Cooling Dry Bulb Range))
  ((CoolingDryBulb0pt4)(Cooling Dry Bulb 0.4%))
  ((CoolingMeanCoincidentWetBulb0pt4)(Cooling Mean Coincident Wet Bulb 0.4%))
  ((CoolingDryBulb1)(Cooling Dry Bulb 1%))
  ((CoolingMeanCoincidentWetBulb1)(Cooling Mean Coincident Wet Bulb 1%))
  ((CoolingDryBulb2)(Cooling Dry Bulb 2%))
  ((CoolingMeanCoincidentWetBulb2)(Cooling Mean Coincident Wet Bulb 2%))
  ((CoolingEvaporationWetBulb0pt4)(Cooling Evaporation Wet Bulb 0.4%))
  ((CoolingEvaporationMeanCoincidentDryBulb0pt4)(Cooling Evaporation Mean Coincident Dry Bulb 0.4%))
  ((CoolingEvaporationWetBulb1)(Cooling Evaporation Wet Bulb 1%))
  ((CoolingEvaporationMeanCoincidentDryBulb1)(Cooling Evaporation Mean Coincident Dry Bulb 1%))
  ((CoolingEvaporationWetBulb2)(Cooling Evaporation Wet Bulb 2%))
  ((CoolingEvaporationMeanCoincidentDryBulb2)(Cooling Evaporation Mean Coincident Dry Bulb 2%))
  ((CoolingMeanCoincidentWindSpeed0pt4)(Cooling Mean Coincident Wind Speed 0.4%))
  ((CoolingPrevailingCoincidentWindDirection0pt4)(Cooling Prevailing Coincident Wind Direction 0.4%))
  ((CoolingDehumidificationDewPoint0pt4)(Cooling Dehumidification Dew Point 0.4%))
  ((CoolingDehumidificationHumidityRatio0pt4)(Cooling Dehumification Humidity Ratio 0.4%))
  ((CoolingDehumidificationMeanCoincidentDryBulb0pt4)(Cooling Dehumification Mean Coincident Dry Bulb 0.4%))
  ((CoolingDehumidificationDewPoint1)(Cooling Dehumification Dew Point 1%))
  ((CoolingDehumidificationHumidityRatio1)(Cooling Dehumification Humidity Ratio 1%))
  ((CoolingDehumidificationMeanCoincidentDryBulb1)(Cooling Dehumification Mean Coincident Dry Bulb 1%))
  ((CoolingDehumidificationDewPoint2)(Cooling Dehumidification Dew Point 2%))
  ((CoolingDehumidificationHumidityRatio2)(Cooling Dehumidification Humidity Ratio 2%))
  ((CoolingDehumidificationMeanCoincidentDryBulb2)(Cooling Dehumidification Mean Coincident Dry Bulb 2%))
  ((CoolingEnthalpy0pt4)(Cooling Enthalpy 0.4%))
  ((CoolingEnthalpyMeanCoincidentDryBulb0pt4)(Cooling Enthalpy Mean Coincident Dry Bulb 1%))
  ((CoolingEnthalpy1)(Cooling Enthalpy 1%))
  ((CoolingEnthalpyMeanCoincidentDryBulb1)(Cooling Enthalpy Mean Coincident Dry Bulb 1%))
  ((CoolingEnthalpy2)(Cooling Enthalpy 2%))
  ((CoolingEnthalpyMeanCoincidentDryBulb2)(Cooling Enthalpy Mean Coincident Dry Bulb 2%))
  ((CoolingHours8To4AndDryBulb12pt8To20pt6)(Cooling Hours 8 to 4 and Dry Bulb 12.8% to 20.6%))
  ((Extremes)(Extremes))
  ((ExtremeWindSpeed1)(Extreme Wind Speed 1%))
  ((ExtremeWindSpeed2pt5)(Extreme Wind Speed 2.5%))
  ((ExtremeWindSpeed5)(Extreme Wind Speed 5%))
  ((ExtremeMaxWetBulb)(Extreme Max Wet Bulb))
  ((ExtremeMeanMinDryBulb)(Extreme Mean Min Dry Bulb))
  ((ExtremeMeanMaxDryBulb)(Extreme Mean Max Dry Bulb))
  ((ExtremeStdDevMinDryBulb)(Extreme Std Dev Min Dry Bulb))
  ((ExtremeStdDevMaxDryBulb)(Extreme Std Dev Max Dry Bulb))
  ((ExtremeN5YearsMinDryBulb)(Extreme N5 Years Min Dry Bulb))
  ((ExtremeN5YearsMaxDryBulb)(Extreme N5 Years Max Dry Bulb))
  ((ExtremeN10YearsMinDryBulb)(Extreme N10 Years Min Dry Bulb))
  ((Extreme10YearsMaxDryBulb)(Extreme N10 Years Max Dry Bulb))
  ((ExtremeN20YearsMinDryBulb)(Extreme N20 Years Min Dry Bulb))
  ((ExtremeN20YearsMaxDryBulb)(Extreme N20 Years Max Dry Bulb))
  ((ExtremeN50YearsMinDryBulb)(Extreme N50 Years Min Dry Bulb))
  ((ExtremeN50YearsMaxDryBulb)(Extreme N50 Years Max Dry Bulb))
  );

/** EpwDataPoint is one line from the EPW file. All floating point numbers are stored as strings,
 * but are checked as numbers.
 */
class UTILITIES_API EpwDataPoint
{
public:
  /** Create an empty EpwDataPoint object */
  EpwDataPoint();
  /** Create an EpwDataPoint object with specified properties */
  EpwDataPoint(int year,int month,int day,int hour,int minute,
    std::string dataSourceandUncertaintyFlags,double dryBulbTemperature,double dewPointTemperature,
    double relativeHumidity,double atmosphericStationPressure,double extraterrestrialHorizontalRadiation,
    double extraterrestrialDirectNormalRadiation,double horizontalInfraredRadiationIntensity,
    double globalHorizontalRadiation,double directNormalRadiation,double diffuseHorizontalRadiation,
    double globalHorizontalIlluminance,double directNormalIlluminance,double diffuseHorizontalIlluminance,
    double zenithLuminance,double windDirection,double windSpeed,int totalSkyCover,int opaqueSkyCover,
    double visibility,double ceilingHeight,int presentWeatherObservation,int presentWeatherCodes,
    double precipitableWater,double aerosolOpticalDepth,double snowDepth,double daysSinceLastSnowfall,
    double albedo,double liquidPrecipitationDepth,double liquidPrecipitationQuantity);
  // Static
  /** Returns the units of the named field */
  static boost::optional<std::string> getUnitsByName(const std::string &name);
  /** Returns the units of the field specified by enumeration value */
  static std::string getUnits(EpwDataField field);
  /** Returns the units of the computed value specified by enumeration value */
  static std::string getUnits(EpwComputedField field);
  // Data retrieval
  /** Returns the double value of the named field if possible */
  boost::optional<double> getFieldByName(const std::string &name);
  /** Returns the dobule value of the field specified by enumeration value */
  boost::optional<double> getField(EpwDataField id);
  /** Returns the air state specified by the EPW data. If dry bulb, pressure, and relative humidity are available,
      then those values will be used to compute the air state. Otherwise, unless dry bulb, pressure, and dew point are
      available, then an empty optional will be returned. Note that the air state may not be consistend with the EPW
      data if all 4 parameters are in the EPW data. */
  boost::optional<AirState> airState() const;
  // Conversion
  /** Create an EpwDataPoint from an EPW-formatted string */
  static boost::optional<EpwDataPoint> fromEpwString(const std::string &line);
  /** Create an EpwDataPoint from a list of EPW data as strings. The pedantic argument controls how strict the conversion is.
      If pedantic is true, the list should have 35 elements. If pedantic is false, lists with more or fewer elements may
      still result in an EpwDataPoint */
  static boost::optional<EpwDataPoint> fromEpwStrings(const std::vector<std::string> &list, bool pedantic=true);
  /** Create an EpwDataPoint from a list of EPW data as strings, overriding the date and time with the specified arguments.
      The pedantic argument controls how strict the conversion is. If pedantic is true, the list should have 35 elements.
      If pedantic is false, lists with more or fewer elements may still result in an EpwDataPoint */
  static boost::optional<EpwDataPoint> fromEpwStrings(int year, int month, int day, int hour, int minute,
    const std::vector<std::string> &list, bool pedantic = true);
  /** Returns a list of strings containing the EPW data in the EpwDataPoint */
  std::vector<std::string> toEpwStrings() const;
  /** Convert the EPW data into CONTAM's WTH format */
  boost::optional<std::string> toWthString() const;
  // One billion getters
  /** Returns the date in a Date object */
  Date date() const;
  /** Returns the time in a Time object */
  Time time() const;
  /** Returns the date and time in a DateTime object */
  openstudio::DateTime dateTime() const;
  /** Returns the year as an integer */
  int year() const;
  /** Returns the month as an integer */
  int month() const;
  /** Returns the day as an integer */
  int day() const;
  /** Returns the hour as an integer */
  int hour() const;
  /** Returns the minute as an integer */
  int minute() const;
  /** Returns the data source and uncertainty flags */
  std::string dataSourceandUncertaintyFlags() const;
  /** If available, return the dry bulb temperature in degrees C */
  boost::optional<double> dryBulbTemperature() const;
  /** If available, return the dew point temperature in degrees C */
  boost::optional<double> dewPointTemperature() const;
  /** If available, return the relative humidity in % */
  boost::optional<double> relativeHumidity() const;
  /** If available, return the atmospheric station pressure in Pa */
  boost::optional<double> atmosphericStationPressure() const;
  /** If available, return the extraterrestrial horizontal radiation in Wh/m2*/
  boost::optional<double> extraterrestrialHorizontalRadiation() const;
  /** If available, return the extraterrestrial direct normal radiation in Wh/m2 */
  boost::optional<double> extraterrestrialDirectNormalRadiation() const;
  /** If available, return the horizontal infrared radiation intensity in Wh/m2*/
  boost::optional<double> horizontalInfraredRadiationIntensity() const;
  /** If available, return the global horizontal radiation in Wh/m2 */
  boost::optional<double> globalHorizontalRadiation() const;
  /** If available, return direct normal radiation in Wh/m2*/
  boost::optional<double> directNormalRadiation() const;
  /** If available, return the diffuse horizontal radiation in Wh/m2*/
  boost::optional<double> diffuseHorizontalRadiation() const;
  /** If available, return the global horizontal illuminance in lux*/
  boost::optional<double> globalHorizontalIlluminance() const;
  /** If available, return the direct normal illuminance in lux*/
  boost::optional<double> directNormalIlluminance() const;
  /** If available, return the diffuse horizontal illuminance in lux*/
  boost::optional<double> diffuseHorizontalIlluminance() const;
  /** If available, return the zenith luminances Cd/m2*/
  boost::optional<double> zenithLuminance() const;
  /** If available, return the wind direction in degrees*/
  boost::optional<double> windDirection() const;
  /** If available, return the wind speed in m/s */
  boost::optional<double> windSpeed() const;
  /** Returns the total sky cover */
  int totalSkyCover() const;
  /** Returns the opaque sky cover */
  int opaqueSkyCover() const;
  /** If available, return the visibility in km */
  boost::optional<double> visibility() const;
  /** If available, return the ceiling height in m*/
  boost::optional<double> ceilingHeight() const;
  /** Returns the present weather observation */
  int presentWeatherObservation() const;
  /** Returns the presetn weather codes */
  int presentWeatherCodes() const;
  /** If available, return the precipitable water in mm*/
  boost::optional<double> precipitableWater() const;
  /** If available, return the aerosol optical depth in thousandths*/
  boost::optional<double> aerosolOpticalDepth() const;
  /** If available, return the snow depth in cm */
  boost::optional<double> snowDepth() const;
  /** If available, return the days since last snowfall */
  boost::optional<double> daysSinceLastSnowfall() const;
  /** If available, return the albedo */
  boost::optional<double> albedo() const;
  /** If available, return the liquid precipitation depth in mm */
  boost::optional<double> liquidPrecipitationDepth() const;
  /** If available, return the liquid precipitation quantity in hr */
  boost::optional<double> liquidPrecipitationQuantity() const;

  // Computed quantities
  /** If possible, compute and return the saturation pressure in Pa */
  boost::optional<double> saturationPressure() const;
  /** If possible, compute and return the enthalpy in kJ/kg */
  boost::optional<double> enthalpy() const;
  /** If possible, compute and return the humidity ratio */
  boost::optional<double> humidityRatio() const;
  /** If possible, compute and return the density kg/m3 */
  boost::optional<double> density() const;
  /** If possible, compute and return the specific volume in m3/kg */
  boost::optional<double> specificVolume() const;
  /** If possible, compute and return the wet bulb temperature in C */
  boost::optional<double> wetbulb() const;

private:
  // One billion setters
  void setDate(Date date);
  void setTime(Time time);
  void setDateTime(openstudio::DateTime dateTime);
  void setYear(int year);
  bool setYear(const std::string &year);
  bool setMonth(int month);
  bool setMonth(const std::string &month);
  bool setDay(int day);
  bool setDay(const std::string &day);
  bool setHour(int hour);
  bool setHour(const std::string &hour);
  bool setMinute(int minute);
  bool setMinute(const std::string &minute);
  void setDataSourceandUncertaintyFlags(const std::string &dataSourceandUncertaintyFlags);
  bool setDryBulbTemperature(double dryBulbTemperature);
  bool setDryBulbTemperature(const std::string &dryBulbTemperature);
  bool setDewPointTemperature(double dewPointTemperature);
  bool setDewPointTemperature(const std::string &dewPointTemperature);
  bool setRelativeHumidity(double relativeHumidity);
  bool setRelativeHumidity(const std::string &relativeHumidity);
  bool setAtmosphericStationPressure(double atmosphericStationPressure);
  bool setAtmosphericStationPressure(const std::string &atmosphericStationPressure);
  bool setExtraterrestrialHorizontalRadiation(double extraterrestrialHorizontalRadiation);
  bool setExtraterrestrialHorizontalRadiation(const std::string &extraterrestrialHorizontalRadiation);
  bool setExtraterrestrialDirectNormalRadiation(double extraterrestrialDirectNormalRadiation);
  bool setExtraterrestrialDirectNormalRadiation(const std::string &extraterrestrialDirectNormalRadiation);
  bool setHorizontalInfraredRadiationIntensity(double horizontalInfraredRadiationIntensity);
  bool setHorizontalInfraredRadiationIntensity(const std::string &horizontalInfraredRadiationIntensity);
  bool setGlobalHorizontalRadiation(double globalHorizontalRadiation);
  bool setGlobalHorizontalRadiation(const std::string &globalHorizontalRadiation);
  bool setDirectNormalRadiation(double directNormalRadiation);
  bool setDirectNormalRadiation(const std::string &directNormalRadiation);
  bool setDiffuseHorizontalRadiation(double diffuseHorizontalRadiation);
  bool setDiffuseHorizontalRadiation(const std::string &diffuseHorizontalRadiation);
  bool setGlobalHorizontalIlluminance(double globalHorizontalIlluminance);
  bool setGlobalHorizontalIlluminance(const std::string &globalHorizontalIlluminance);
  bool setDirectNormalIlluminance(double directNormalIlluminance);
  bool setDirectNormalIlluminance(const std::string &directNormalIlluminance);
  bool setDiffuseHorizontalIlluminance(double diffuseHorizontalIlluminance);
  bool setDiffuseHorizontalIlluminance(const std::string &diffuseHorizontalIlluminance);
  bool setZenithLuminance(double zenithLuminance);
  bool setZenithLuminance(const std::string &zenithLuminance);
  bool setWindDirection(double windDirection);
  bool setWindDirection(const std::string &windDirection);
  bool setWindSpeed(double windSpeed);
  bool setWindSpeed(const std::string &windSpeed);
  bool setTotalSkyCover(int totalSkyCover);
  bool setTotalSkyCover(const std::string &totalSkyCover);
  bool setOpaqueSkyCover(int opaqueSkyCover);
  bool setOpaqueSkyCover(const std::string &opaqueSkyCover);
  bool setVisibility(double visibility);
  bool setVisibility(const std::string &visibility);
  void setCeilingHeight(double ceilingHeight);
  bool setCeilingHeight(const std::string &ceilingHeight);
  void setPresentWeatherObservation(int presentWeatherObservation);
  bool setPresentWeatherObservation(const std::string &presentWeatherObservation);
  void setPresentWeatherCodes(int presentWeatherCodes);
  bool setPresentWeatherCodes(const std::string &presentWeatherCodes);
  void setPrecipitableWater(double precipitableWater);
  bool setPrecipitableWater(const std::string &precipitableWater);
  void setAerosolOpticalDepth(double aerosolOpticalDepth);
  bool setAerosolOpticalDepth(const std::string &aerosolOpticalDepth);
  void setSnowDepth(double snowDepth);
  bool setSnowDepth(const std::string &snowDepth);
  void setDaysSinceLastSnowfall(double daysSinceLastSnowfall);
  bool setDaysSinceLastSnowfall(const std::string &daysSinceLastSnowfall);
  void setAlbedo(double albedo);
  bool setAlbedo(const std::string &albedo);
  void setLiquidPrecipitationDepth(double liquidPrecipitationDepth);
  bool setLiquidPrecipitationDepth(const std::string &liquidPrecipitationDepth);
  void setLiquidPrecipitationQuantity(double liquidPrecipitationQuantity);
  bool setLiquidPrecipitationQuantity(const std::string &liquidPrecipitationQuantity);

  int m_year;
  int m_month;
  int m_day;
  int m_hour;
  int m_minute;
  std::string m_dataSourceandUncertaintyFlags;
  std::string m_dryBulbTemperature; // units C, minimum> -70, maximum< 70, missing 99.9
  std::string m_dewPointTemperature; // units C, minimum> -70, maximum< 70, missing 99.9
  std::string m_relativeHumidity; // missing 999., minimum 0, maximum 110
  std::string m_atmosphericStationPressure; // units Pa, missing 999999.,  minimum> 31000, maximum< 120000
  std::string m_extraterrestrialHorizontalRadiation; // units Wh/m2, missing 9999., minimum 0
  std::string m_extraterrestrialDirectNormalRadiation; //units Wh/m2, missing 9999., minimum 0
  std::string m_horizontalInfraredRadiationIntensity; // units Wh/m2, missing 9999., minimum 0
  std::string m_globalHorizontalRadiation; // units Wh/m2, missing 9999., minimum 0
  std::string m_directNormalRadiation; // units Wh/m2, missing 9999., minimum 0
  std::string m_diffuseHorizontalRadiation; // units Wh/m2, missing 9999., minimum 0
  std::string m_globalHorizontalIlluminance; // units lux, missing 999999., will be missing if >= 999900, minimum 0
  std::string m_directNormalIlluminance; // units lux, missing 999999., will be missing if >= 999900, minimum 0
  std::string m_diffuseHorizontalIlluminance; // units lux, missing 999999., will be missing if >= 999900, minimum 0
  std::string m_zenithLuminance; // units Cd/m2, missing 9999., will be missing if >= 9999, minimum 0
  std::string m_windDirection; // units degrees, missing 999., minimum 0, maximum 360
  std::string m_windSpeed; // units m/s, missing 999., minimum 0, maximum 40
  int m_totalSkyCover; // missing 99, minimum 0, maximum 10
  int m_opaqueSkyCover; // used if Horizontal IR Intensity missing, missing 99, minimum 0, maximum 10
  std::string m_visibility; // units km, missing 9999
  std::string m_ceilingHeight; // units m, missing 99999
  int m_presentWeatherObservation;
  int m_presentWeatherCodes;
  std::string m_precipitableWater; // units mm, missing 999
  std::string m_aerosolOpticalDepth; // units thousandths, missing .999
  std::string m_snowDepth; // units cm, missing 999
  std::string m_daysSinceLastSnowfall; // missing 99
  std::string m_albedo; //missing 999
  std::string m_liquidPrecipitationDepth; // units mm, missing 999
  std::string m_liquidPrecipitationQuantity; // units hr, missing 99
};

/** EpwDesignCondition is one line from the EPW file. All floating point numbers are stored as strings,
* but are checked as numbers.
*/
class UTILITIES_API EpwDesignCondition
{
public:
  /** Create an empty EpwDesignCondition object */
  EpwDesignCondition();
  /** Create an EpwDesignCondition object with specified properties */
  EpwDesignCondition(std::string titleOfDesignCondition, int heatingColdestMonth, double heatingDryBulb99pt6, double heatingDryBulb99,
    double heatingHumidificationDewPoint99pt6, double heatingHumidificationHumidityRatio99pt6, double heatingHumidificationMeanCoincidentDryBulb99pt6,
    double heatingHumidificationDewPoint99, double heatingHumidificationHumidityRatio99, double heatingHumidificationMeanCoincidentDryBulb99,
    double heatingColdestMonthWindSpeed0pt4, double heatingColdestMonthMeanCoincidentDryBulb0pt4, double heatingColdestMonthWindSpeed1, 
    double heatingColdestMonthMeanCoincidentDryBulb1, double heatingMeanCoincidentWindSpeed99pt6, int heatingPrevailingCoincidentWindDirection99pt6,
    int coolingHottestMonth, double coolingDryBulbRange, double coolingDryBulb0pt4, double coolingMeanCoincidentWetBulb0pt4,
    double coolingDryBulb1, double coolingMeanCoincidentWetBulb1, double coolingDryBulb2, double coolingMeanCoincidentWetBulb2,
    double coolingEvaporationWetBulb0pt4, double coolingEvaporationMeanCoincidentDryBulb0pt4, double coolingEvaporationWetBulb1,
    double coolingEvaporationMeanCoincidentDryBulb1, double coolingEvaporationWetBulb2, double coolingEvaporationMeanCoincidentDryBulb2,
    double coolingMeanCoincidentWindSpeed0pt4, int coolingPrevailingCoincidentWindDirection0pt4, double coolingDehumidificationDewPoint0pt4,
    double coolingDehumidificationHumidityRatio0pt4, double coolingDehumidificationMeanCoincidentDryBulb0pt4,
    double coolingDehumificationDewPoint1, double coolingDehumificationHumidityRatio1, double coolingDehumidificationMeanCoincidentDryBulb1,
    double coolingDehumidificationDewPoint2, double coolingDehumidificationHumidityRatio2, double coolingDehumidificationMeanCoincidentDryBulb2,
    double coolingEnthalpy0pt4, double coolingEnthalpyMeanCoincidentDryBulb0pt4, double coolingEnthalpy1, double coolingEnthalpyMeanCoincidentDryBulb1,
    double coolingEnthalpy2, double coolingEnthalpyMeanCoincidentDryBulb2, int coolingHours8to4andDryBulb12pt8to20pt6, double extremeWindSpeed1,
    double extremeWindSpeed2pt5, double extremeWindSpeed5, double extremeMaxWetBulb, double extremeMeanMinDryBulb, double extremeMeanMaxDryBulb,
    double extremeStdDevMinDryBulb, double extremeStdDevMaxDryBulb, double extremeN5yearsMinDryBulb, double extremeN5yearsMaxDryBulb,
    double extremeN10yearsMinDryBulb, double extremeN10yearsMaxDryBulb, double extremeN20yearsMinDryBulb, double extremeN20yearsMaxDryBulb,
    double extremeN50yearsMinDryBulb, double extremeN50yeaarsMaxDryBulb);
  // Static
  /** Returns the units of the named field */
  static boost::optional<std::string> getUnitsByName(const std::string &name);
  /** Returns the units of the field specified by enumeration value */
  static std::string getUnits(EpwDesignField field);
  // Data retrieval
  /** Returns the double value of the named field if possible */
  boost::optional<double> getFieldByName(const std::string &name);
  /** Returns the dobule value of the field specified by enumeration value */
  boost::optional<double> getField(EpwDesignField id);
  // Conversion
  /** Create an EpwDesignCondition from an EPW-formatted string */
  static boost::optional<EpwDesignCondition> fromDesignConditionsString(const std::string &line);
  /** Create an EpwDesignCondition from a list of EPW designs as strings. The pedantic argument controls how strict the conversion is.
  If pedantic is true, the list should have 35 elements. If pedantic is false, lists with more or fewer elements may
  still result in an EpwDesignCondition */
  static boost::optional<EpwDesignCondition> fromDesignConditionsStrings(const std::vector<std::string> &list);
  
  // Getters
  /** Returns the title of design condition */
  std::string titleOfDesignCondition() const;
  /** Returns the heating coldest month */
  int heatingColdestMonth() const;
  /** Returns the heating dry bulb temperature 99.6% */
  double heatingDryBulb99pt6() const;

  heatingDryBulb99();
  heatingHumidificationDewPoint99pt6();
  heatingHumidificationHumidityRatio99pt6();
  heatingHumidificationMeanCoincidentDryBulb99pt6();
  heatingHumidificationDewPoint99();
  heatingHumidificationHumidityRatio99();
  heatingHumidificationMeanCoincidentDryBulb99();
  heatingColdestMonthWindSpeed0pt4();
  heatingColdestMonthMeanCoincidentDryBulb0pt4();
  heatingColdestMonthWindSpeed1();
  heatingColdestMonthMeanCoincidentDryBulb1();
  heatingMeanCoincidentWindSpeed99pt6();
  heatingPrevailingCoincidentWindDirection99pt6();
  coolingHottestMonth();
  coolingDryBulbRange();
  coolingDryBulb0pt4();
  coolingMeanCoincidentWetBulb0pt4();
  coolingDryBulb1();
  coolingMeanCoincidentWetBulb1();
  coolingDryBulb2();
  coolingMeanCoincidentWetBulb2();
  coolingEvaporationWetBulb0pt4();
  coolingEvaporationMeanCoincidentDryBulb0pt4();
  coolingEvaporationWetBulb1();
  coolingEvaporationMeanCoincidentDryBulb1();
  coolingEvaporationWetBulb2();
  coolingEvaporationMeanCoincidentDryBulb2();
  coolingMeanCoincidentWindSpeed0pt4();
  coolingPrevailingCoincidentWindDirection0pt4();
  coolingDehumidificationDewPoint0pt4();
  coolingDehumidificationHumidityRatio0pt4();
  coolingDehumidificationMeanCoincidentDryBulb0pt4();
  coolingDehumidificationDewPoint1();
  coolingDehumidificationHumidityRatio1();
  coolingDehumidificationMeanCoincidentDryBulb1();
  coolingDehumidificationDewPoint2();
  coolingDehumidificationHumidityRatio2();
  coolingDehumidificationMeanCoincidentDryBulb2();
  coolingEnthalpy0pt4();
  coolingEnthalpyMeanCoincidentDryBulb0pt4();
  coolingEnthalpy1();
  coolingEnthalpyMeanCoincidentDryBulb1();
  coolingEnthalpy2();
  coolingEnthalpyMeanCoincidentDryBulb2();
  coolingHours8To4AndDryBulb12pt8To20pt6();
  extremeWindSpeed1();
  extremeWindSpeed2pt5();
  extremeWindSpeed5();
  extremeMaxWetBulb();
  extremeMeanMinDryBulb();
  extremeMeanMaxDryBulb();
  extremeStdDevMinDryBulb();
  extremeStdDevMaxDryBulb();
  extremeN5YearsMinDryBulb();
  extremeN5YearsMaxDryBulb();
  extremeN10YearsMinDryBulb();
  extreme10YearsMaxDryBulb();
  extremeN20YearsMinDryBulb();
  extremeN20YearsMaxDryBulb();
  extremeN50YearsMinDryBulb();
  extremeN50YearsMaxDryBulb();

private:
  // Setters
  void setTitleOfDesignCondition(std::string titleOfDesignCondition);
  bool setHeatingColdestMonth(int heatingColdestMonth);
  bool setHeatingColdestMonth(const std::string &heatingColdestMonth);
  bool setHeatingDryBulb99pt6(double heatingDryBulb99pt6);
  bool setHeatingDryBulb99pt6(const std::string &heatingDryBulb99pt6);

  setHeatingDryBulb99();
  setHeatingHumidificationDewPoint99pt6();
  setHeatingHumidificationHumidityRatio99pt6();
  setHeatingHumidificationMeanCoincidentDryBulb99pt6();
  setHeatingHumidificationDewPoint99();
  setHeatingHumidificationHumidityRatio99();
  setHeatingHumidificationMeanCoincidentDryBulb99();
  setHeatingColdestMonthWindSpeed0pt4();
  setHeatingColdestMonthMeanCoincidentDryBulb0pt4();
  setHeatingColdestMonthWindSpeed1();
  setHeatingColdestMonthMeanCoincidentDryBulb1();
  setHeatingMeanCoincidentWindSpeed99pt6();
  setHeatingPrevailingCoincidentWindDirection99pt6();
  setCoolingHottestMonth();
  setCoolingDryBulbRange();
  setCoolingDryBulb0pt4();
  setCoolingMeanCoincidentWetBulb0pt4();
  setCoolingDryBulb1();
  setCoolingMeanCoincidentWetBulb1();
  setCoolingDryBulb2();
  setCoolingMeanCoincidentWetBulb2();
  setCoolingEvaporationWetBulb0pt4();
  setCoolingEvaporationMeanCoincidentDryBulb0pt4();
  setCoolingEvaporationWetBulb1();
  setCoolingEvaporationMeanCoincidentDryBulb1();
  setCoolingEvaporationWetBulb2();
  setCoolingEvaporationMeanCoincidentDryBulb2();
  setCoolingMeanCoincidentWindSpeed0pt4();
  setCoolingPrevailingCoincidentWindDirection0pt4();
  setCoolingDehumidificationDewPoint0pt4();
  setCoolingDehumidificationHumidityRatio0pt4();
  setCoolingDehumidificationMeanCoincidentDryBulb0pt4();
  setCoolingDehumidificationDewPoint1();
  setCoolingDehumidificationHumidityRatio1();
  setCoolingDehumidificationMeanCoincidentDryBulb1();
  setCoolingDehumidificationDewPoint2();
  setCoolingDehumidificationHumidityRatio2();
  setCoolingDehumidificationMeanCoincidentDryBulb2();
  setCoolingEnthalpy0pt4();
  setCoolingEnthalpyMeanCoincidentDryBulb0pt4();
  setCoolingEnthalpy1();
  setCoolingEnthalpyMeanCoincidentDryBulb1();
  setCoolingEnthalpy2();
  setCoolingEnthalpyMeanCoincidentDryBulb2();
  setCoolingHours8To4AndDryBulb12pt8To20pt6();
  setExtremeWindSpeed1();
  setExtremeWindSpeed2pt5();
  setExtremeWindSpeed5();
  setExtremeMaxWetBulb();
  setExtremeMeanMinDryBulb();
  setExtremeMeanMaxDryBulb();
  setExtremeStdDevMinDryBulb();
  setExtremeStdDevMaxDryBulb();
  setExtremeN5YearsMinDryBulb();
  setExtremeN5YearsMaxDryBulb();
  setExtremeN10YearsMinDryBulb();
  setExtreme10YearsMaxDryBulb();
  setExtremeN20YearsMinDryBulb();
  setExtremeN20YearsMaxDryBulb();
  setExtremeN50YearsMinDryBulb();
  setExtremeN50YearsMaxDryBulb();

  std::string m_titleOfDesignCondition;
  int m_heatingColdestMonth;
  double m_heatingDryBulb99pt6;

  m_heatingDryBulb99;
  m_heatingHumidificationDewPoint99pt6;
  m_heatingHumidificationHumidityRatio99pt6;
  m_heatingHumidificationMeanCoincidentDryBulb99pt6;
  m_heatingHumidificationDewPoint99;
  m_heatingHumidificationHumidityRatio99;
  m_heatingHumidificationMeanCoincidentDryBulb99;
  m_heatingColdestMonthWindSpeed0pt4;
  m_heatingColdestMonthMeanCoincidentDryBulb0pt4;
  m_heatingColdestMonthWindSpeed1;
  m_heatingColdestMonthMeanCoincidentDryBulb1;
  m_heatingMeanCoincidentWindSpeed99pt6;
  m_heatingPrevailingCoincidentWindDirection99pt6;
  m_coolingHottestMonth;
  m_coolingDryBulbRange;
  m_coolingDryBulb0pt4;
  m_coolingMeanCoincidentWetBulb0pt4;
  m_coolingDryBulb1;
  m_coolingMeanCoincidentWetBulb1;
  m_coolingDryBulb2;
  m_coolingMeanCoincidentWetBulb2;
  m_coolingEvaporationWetBulb0pt4;
  m_coolingEvaporationMeanCoincidentDryBulb0pt4;
  m_coolingEvaporationWetBulb1;
  m_coolingEvaporationMeanCoincidentDryBulb1;
  m_coolingEvaporationWetBulb2;
  m_coolingEvaporationMeanCoincidentDryBulb2;
  m_coolingMeanCoincidentWindSpeed0pt4;
  m_coolingPrevailingCoincidentWindDirection0pt4;
  m_coolingDehumidificationDewPoint0pt4;
  m_coolingDehumidificationHumidityRatio0pt4;
  m_coolingDehumidificationMeanCoincidentDryBulb0pt4;
  m_coolingDehumidificationDewPoint1;
  m_coolingDehumidificationHumidityRatio1;
  m_coolingDehumidificationMeanCoincidentDryBulb1;
  m_coolingDehumidificationDewPoint2;
  m_coolingDehumidificationHumidityRatio2;
  m_coolingDehumidificationMeanCoincidentDryBulb2;
  m_coolingEnthalpy0pt4;
  m_coolingEnthalpyMeanCoincidentDryBulb0pt4;
  m_coolingEnthalpy1;
  m_coolingEnthalpyMeanCoincidentDryBulb1;
  m_coolingEnthalpy2;
  m_coolingEnthalpyMeanCoincidentDryBulb2;
  m_coolingHours8To4AndDryBulb12pt8To20pt6;
  m_extremeWindSpeed1;
  m_extremeWindSpeed2pt5;
  m_extremeWindSpeed5;
  m_extremeMaxWetBulb;
  m_extremeMeanMinDryBulb;
  m_extremeMeanMaxDryBulb;
  m_extremeStdDevMinDryBulb;
  m_extremeStdDevMaxDryBulb;
  m_extremeN5YearsMinDryBulb;
  m_extremeN5YearsMaxDryBulb;
  m_extremeN10YearsMinDryBulb;
  m_extreme10YearsMaxDryBulb;
  m_extremeN20YearsMinDryBulb;
  m_extremeN20YearsMaxDryBulb;
  m_extremeN50YearsMinDryBulb;
  m_extremeN50YearsMaxDryBulb;
};

/** EpwFile parses a weather file in EPW format.  Later it may provide
 *   methods for writing and converting other weather files to EPW format.
 */
class UTILITIES_API EpwFile{
public:

  /// constructor with path
  /// will throw if path does not exist or file is incorrect
  EpwFile(const openstudio::path& p, bool storeData=false);

  /// static load method
  static boost::optional<EpwFile> load(const openstudio::path& p, bool storeData=false);

  /// static load method
  static boost::optional<EpwFile> loadFromString(const std::string& str, bool storeData=false);

  /// get the path
  openstudio::path path() const;

  /// get the file's checksum
  std::string checksum() const;

  /// get the city
  std::string city() const;

  /// get the state, province, or region
  std::string stateProvinceRegion() const;

  /// get the country
  std::string country() const;

  /// get the data source
  std::string dataSource() const;

  /// get the WMO number
  std::string wmoNumber() const;

  /// get the latitude in degrees
  double latitude() const;

  /// get the longitude in degrees
  double longitude() const;

  /// get the time zone relative to GMT
  double timeZone() const;

  /// get the elevation
  double elevation() const;

  /// get the time step
  Time timeStep() const;

  /// get the records per hour
  int recordsPerHour() const;

  /// get the start day of the week
  DayOfWeek startDayOfWeek() const;

  /// get the start date
  Date startDate() const;

  /// get the end date
  Date endDate() const;

  /// get the actual year of the start date if there is one
  boost::optional<int> startDateActualYear() const;

  /// get the actual year of the end date if there is one
  boost::optional<int> endDateActualYear() const;

  /// get the weather data
  std::vector<EpwDataPoint> data();

  /// get the design conditions
  std::vector<EpwDesignCondition> designConditions();

  /// get a time series of a particular weather field
  // This will probably need to include the period at some point, but for now just dump everything into a time series
  boost::optional<TimeSeries> getTimeSeries(const std::string &field);
  /// get a time series of a computed quantity
  boost::optional<TimeSeries> getComputedTimeSeries(const std::string &field);

  /// export to CONTAM WTH file
  bool translateToWth(openstudio::path path,std::string description=std::string());

  // Data status (?) functions
  /// Returns true if the file appears to be AMY (as opposed to TMY)
  bool isActual() const;
  /// Returns true if the data period "records per hour" input matches the data point values
  bool minutesMatch() const;

private:

  EpwFile();
  bool parse(std::istream& is, bool storeData=false);
  bool parseLocation(const std::string& line);
  bool parseDesignConditions(const std::string& line);
  bool parseDataPeriod(const std::string& line);

  // configure logging
  REGISTER_LOGGER("openstudio.EpwFile");

  openstudio::path m_path;
  std::string m_checksum;
  std::string m_city;
  std::string m_stateProvinceRegion;
  std::string m_country;
  std::string m_dataSource;
  std::string m_wmoNumber;
  double m_latitude;
  double m_longitude;
  double m_timeZone;
  double m_elevation;
  int m_recordsPerHour;
  DayOfWeek m_startDayOfWeek;
  Date m_startDate;
  Date m_endDate;
  boost::optional<int> m_startDateActualYear;
  boost::optional<int> m_endDateActualYear;
  std::vector<EpwDataPoint> m_data;
  std::vector<EpwDesignCondition> m_designs;

  bool m_isActual;

  // Error/warning flags to store how well the input matches what we think it should
  bool m_minutesMatch; // No disagreement between the data period and the minutes field
};

UTILITIES_API IdfObject toIdfObject(const EpwFile& epwFile);

} // openstudio

#endif //UTILITIES_FILETYPES_EPWFILE_HPP
