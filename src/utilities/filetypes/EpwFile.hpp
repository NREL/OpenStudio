/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_EPWFILE_HPP
#define UTILITIES_FILETYPES_EPWFILE_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Path.hpp"
#include "../core/Enum.hpp"
#include "../core/Logger.hpp"
#include "../time/Time.hpp"
#include "../time/Date.hpp"
#include "../time/DateTime.hpp"
#include "../data/TimeSeries.hpp"

namespace openstudio {

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
  double m_drybulb;   // Dry bulb temperature in C
  double m_dewpoint;  // Dew point temperature in C
  double m_pressure;  // Atmospheric pressure in Pa
  double m_wetbulb;   // Thermodynamic wet bulb temperature in C

  // These are always computed
  double m_psat;
  double m_W;
  double m_h;
  double m_phi;
  double m_v;
};

// clang-format off

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
// cppcheck-suppress [unknownMacro, syntaxError]
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
  ((CoolingDehumidificationHumidityRatio0pt4)(Cooling Dehumidification Humidity Ratio 0.4%))
  ((CoolingDehumidificationMeanCoincidentDryBulb0pt4)(Cooling Dehumidification Mean Coincident Dry Bulb 0.4%))
  ((CoolingDehumidificationDewPoint1)(Cooling Dehumidification Dew Point 1%))
  ((CoolingDehumidificationHumidityRatio1)(Cooling Dehumidification Humidity Ratio 1%))
  ((CoolingDehumidificationMeanCoincidentDryBulb1)(Cooling Dehumidification Mean Coincident Dry Bulb 1%))
  ((CoolingDehumidificationDewPoint2)(Cooling Dehumidification Dew Point 2%))
  ((CoolingDehumidificationHumidityRatio2)(Cooling Dehumidification Humidity Ratio 2%))
  ((CoolingDehumidificationMeanCoincidentDryBulb2)(Cooling Dehumidification Mean Coincident Dry Bulb 2%))
  ((CoolingEnthalpy0pt4)(Cooling Enthalpy 0.4%))
  ((CoolingEnthalpyMeanCoincidentDryBulb0pt4)(Cooling Enthalpy Mean Coincident Dry Bulb 0.4%))
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
  ((ExtremeN10YearsMaxDryBulb)(Extreme N10 Years Max Dry Bulb))
  ((ExtremeN20YearsMinDryBulb)(Extreme N20 Years Min Dry Bulb))
  ((ExtremeN20YearsMaxDryBulb)(Extreme N20 Years Max Dry Bulb))
  ((ExtremeN50YearsMinDryBulb)(Extreme N50 Years Min Dry Bulb))
  ((ExtremeN50YearsMaxDryBulb)(Extreme N50 Years Max Dry Bulb))
);
// cppcheck-suppress [unknownMacro, syntaxError]
OPENSTUDIO_ENUM(EpwDepthField,
  ((GroundTemperatureDepth)(Ground Temperature Depth)(0))
  ((SoilConductivity)(Soil Conductivity))
  ((SoilDensity)(Soil Density))
  ((SoilSpecificHeat)(Soil Specific Heat))
  ((JanGroundTemperature)(January Ground Temperature))
  ((FebGroundTemperature)(February Ground Temperature))
  ((MarGroundTemperature)(March Ground Temperature))
  ((AprGroundTemperature)(April Ground Temperature))
  ((MayGroundTemperature)(May Ground Temperature))
  ((JunGroundTemperature)(June Ground Temperature))
  ((JulGroundTemperature)(July Ground Temperature))
  ((AugGroundTemperature)(August Ground Temperature))
  ((SepGroundTemperature)(September Ground Temperature))
  ((OctGroundTemperature)(October Ground Temperature))
  ((NovGroundTemperature)(November Ground Temperature))
  ((DecGroundTemperature)(December Ground Temperature))
);

// clang-format on

/** EpwDataPoint is one line from the EPW file. All floating point numbers are stored as strings,
 * but are checked as numbers.
 */
class UTILITIES_API EpwDataPoint
{
 public:
  /** Create an empty EpwDataPoint object */
  EpwDataPoint();
  /** Create an EpwDataPoint object with specified properties */
  EpwDataPoint(int year, int month, int day, int hour, int minute, const std::string& dataSourceandUncertaintyFlags, double dryBulbTemperature,
               double dewPointTemperature, double relativeHumidity, double atmosphericStationPressure, double extraterrestrialHorizontalRadiation,
               double extraterrestrialDirectNormalRadiation, double horizontalInfraredRadiationIntensity, double globalHorizontalRadiation,
               double directNormalRadiation, double diffuseHorizontalRadiation, double globalHorizontalIlluminance, double directNormalIlluminance,
               double diffuseHorizontalIlluminance, double zenithLuminance, double windDirection, double windSpeed, int totalSkyCover,
               int opaqueSkyCover, double visibility, double ceilingHeight, int presentWeatherObservation, int presentWeatherCodes,
               double precipitableWater, double aerosolOpticalDepth, double snowDepth, double daysSinceLastSnowfall, double albedo,
               double liquidPrecipitationDepth, double liquidPrecipitationQuantity);
  // Static
  /** Returns the units of the named field */
  static boost::optional<std::string> getUnitsByName(const std::string& name);
  /** Returns the units of the field specified by enumeration value */
  static std::string getUnits(EpwDataField field);
  /** Returns the units of the computed value specified by enumeration value */
  static std::string getUnits(EpwComputedField field);
  // Data retrieval
  /** Returns the double value of the named field if possible */
  boost::optional<double> getFieldByName(const std::string& name);
  /** Returns the dobule value of the field specified by enumeration value */
  boost::optional<double> getField(EpwDataField id);
  /** Returns the air state specified by the EPW data. If dry bulb, pressure, and relative humidity are available,
      then those values will be used to compute the air state. Otherwise, unless dry bulb, pressure, and dew point are
      available, then an empty optional will be returned. Note that the air state may not be consistend with the EPW
      data if all 4 parameters are in the EPW data. */
  boost::optional<AirState> airState() const;
  // Conversion
  /** Create an EpwDataPoint from an EPW-formatted string */
  static boost::optional<EpwDataPoint> fromEpwString(const std::string& line);
  /** Create an EpwDataPoint from a list of EPW data as strings. The pedantic argument controls how strict the conversion is.
      If pedantic is true, the list should have 35 elements. If pedantic is false, lists with more or fewer elements may
      still result in an EpwDataPoint */
  static boost::optional<EpwDataPoint> fromEpwStrings(const std::vector<std::string>& list, bool pedantic = true);
  /** Create an EpwDataPoint from a list of EPW data as strings, overriding the date and time with the specified arguments.
      The pedantic argument controls how strict the conversion is. If pedantic is true, the list should have 35 elements.
      If pedantic is false, lists with more or fewer elements may still result in an EpwDataPoint */
  static boost::optional<EpwDataPoint> fromEpwStrings(int year, int month, int day, int hour, int minute, const std::vector<std::string>& list,
                                                      bool pedantic = true);
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
  bool setYear(const std::string& year);
  bool setMonth(int month);
  bool setMonth(const std::string& month);
  bool setDay(int day);
  bool setDay(const std::string& day);
  bool setHour(int hour);
  bool setHour(const std::string& hour);
  bool setMinute(int minute);
  bool setMinute(const std::string& minute);
  void setDataSourceandUncertaintyFlags(const std::string& dataSourceandUncertaintyFlags);
  bool setDryBulbTemperature(double dryBulbTemperature);
  bool setDryBulbTemperature(const std::string& dryBulbTemperature);
  bool setDewPointTemperature(double dewPointTemperature);
  bool setDewPointTemperature(const std::string& dewPointTemperature);
  bool setRelativeHumidity(double relativeHumidity);
  bool setRelativeHumidity(const std::string& relativeHumidity);
  bool setAtmosphericStationPressure(double atmosphericStationPressure);
  bool setAtmosphericStationPressure(const std::string& atmosphericStationPressure);
  bool setExtraterrestrialHorizontalRadiation(double extraterrestrialHorizontalRadiation);
  bool setExtraterrestrialHorizontalRadiation(const std::string& extraterrestrialHorizontalRadiation);
  bool setExtraterrestrialDirectNormalRadiation(double extraterrestrialDirectNormalRadiation);
  bool setExtraterrestrialDirectNormalRadiation(const std::string& extraterrestrialDirectNormalRadiation);
  bool setHorizontalInfraredRadiationIntensity(double horizontalInfraredRadiationIntensity);
  bool setHorizontalInfraredRadiationIntensity(const std::string& horizontalInfraredRadiationIntensity);
  bool setGlobalHorizontalRadiation(double globalHorizontalRadiation);
  bool setGlobalHorizontalRadiation(const std::string& globalHorizontalRadiation);
  bool setDirectNormalRadiation(double directNormalRadiation);
  bool setDirectNormalRadiation(const std::string& directNormalRadiation);
  bool setDiffuseHorizontalRadiation(double diffuseHorizontalRadiation);
  bool setDiffuseHorizontalRadiation(const std::string& diffuseHorizontalRadiation);
  bool setGlobalHorizontalIlluminance(double globalHorizontalIlluminance);
  bool setGlobalHorizontalIlluminance(const std::string& globalHorizontalIlluminance);
  bool setDirectNormalIlluminance(double directNormalIlluminance);
  bool setDirectNormalIlluminance(const std::string& directNormalIlluminance);
  bool setDiffuseHorizontalIlluminance(double diffuseHorizontalIlluminance);
  bool setDiffuseHorizontalIlluminance(const std::string& diffuseHorizontalIlluminance);
  bool setZenithLuminance(double zenithLuminance);
  bool setZenithLuminance(const std::string& zenithLuminance);
  bool setWindDirection(double windDirection);
  bool setWindDirection(const std::string& windDirection);
  bool setWindSpeed(double windSpeed);
  bool setWindSpeed(const std::string& windSpeed);
  bool setTotalSkyCover(int totalSkyCover);
  bool setTotalSkyCover(const std::string& totalSkyCover);
  bool setOpaqueSkyCover(int opaqueSkyCover);
  bool setOpaqueSkyCover(const std::string& opaqueSkyCover);
  bool setVisibility(double visibility);
  bool setVisibility(const std::string& visibility);
  void setCeilingHeight(double ceilingHeight);
  bool setCeilingHeight(const std::string& ceilingHeight);
  void setPresentWeatherObservation(int presentWeatherObservation);
  bool setPresentWeatherObservation(const std::string& presentWeatherObservation);
  void setPresentWeatherCodes(int presentWeatherCodes);
  bool setPresentWeatherCodes(const std::string& presentWeatherCodes);
  void setPrecipitableWater(double precipitableWater);
  bool setPrecipitableWater(const std::string& precipitableWater);
  void setAerosolOpticalDepth(double aerosolOpticalDepth);
  bool setAerosolOpticalDepth(const std::string& aerosolOpticalDepth);
  void setSnowDepth(double snowDepth);
  bool setSnowDepth(const std::string& snowDepth);
  void setDaysSinceLastSnowfall(double daysSinceLastSnowfall);
  bool setDaysSinceLastSnowfall(const std::string& daysSinceLastSnowfall);
  void setAlbedo(double albedo);
  bool setAlbedo(const std::string& albedo);
  void setLiquidPrecipitationDepth(double liquidPrecipitationDepth);
  bool setLiquidPrecipitationDepth(const std::string& liquidPrecipitationDepth);
  void setLiquidPrecipitationQuantity(double liquidPrecipitationQuantity);
  bool setLiquidPrecipitationQuantity(const std::string& liquidPrecipitationQuantity);

  int m_year;
  int m_month;
  int m_day;
  int m_hour;
  int m_minute;
  std::string m_dataSourceandUncertaintyFlags;
  std::string m_dryBulbTemperature;                     // units C, minimum> -70, maximum< 70, missing 99.9
  std::string m_dewPointTemperature;                    // units C, minimum> -70, maximum< 70, missing 99.9
  std::string m_relativeHumidity;                       // missing 999., minimum 0, maximum 110
  std::string m_atmosphericStationPressure;             // units Pa, missing 999999.,  minimum> 31000, maximum< 120000
  std::string m_extraterrestrialHorizontalRadiation;    // units Wh/m2, missing 9999., minimum 0
  std::string m_extraterrestrialDirectNormalRadiation;  //units Wh/m2, missing 9999., minimum 0
  std::string m_horizontalInfraredRadiationIntensity;   // units Wh/m2, missing 9999., minimum 0
  std::string m_globalHorizontalRadiation;              // units Wh/m2, missing 9999., minimum 0
  std::string m_directNormalRadiation;                  // units Wh/m2, missing 9999., minimum 0
  std::string m_diffuseHorizontalRadiation;             // units Wh/m2, missing 9999., minimum 0
  std::string m_globalHorizontalIlluminance;            // units lux, missing 999999., will be missing if >= 999900, minimum 0
  std::string m_directNormalIlluminance;                // units lux, missing 999999., will be missing if >= 999900, minimum 0
  std::string m_diffuseHorizontalIlluminance;           // units lux, missing 999999., will be missing if >= 999900, minimum 0
  std::string m_zenithLuminance;                        // units Cd/m2, missing 9999., will be missing if >= 9999, minimum 0
  std::string m_windDirection;                          // units degrees, missing 999., minimum 0, maximum 360
  std::string m_windSpeed;                              // units m/s, missing 999., minimum 0, maximum 40
  int m_totalSkyCover;                                  // missing 99, minimum 0, maximum 10
  int m_opaqueSkyCover;                                 // used if Horizontal IR Intensity missing, missing 99, minimum 0, maximum 10
  std::string m_visibility;                             // units km, missing 9999
  std::string m_ceilingHeight;                          // units m, missing 99999
  int m_presentWeatherObservation;
  int m_presentWeatherCodes;
  std::string m_precipitableWater;            // units mm, missing 999
  std::string m_aerosolOpticalDepth;          // units thousandths, missing .999
  std::string m_snowDepth;                    // units cm, missing 999
  std::string m_daysSinceLastSnowfall;        // missing 99
  std::string m_albedo;                       //missing 999
  std::string m_liquidPrecipitationDepth;     // units mm, missing 999
  std::string m_liquidPrecipitationQuantity;  // units hr, missing 99
};

class UTILITIES_API EpwHoliday
{

 public:
  EpwHoliday(const std::string& holidayName, const std::string& holidayDateString)
    : m_holidayName(holidayName), m_holidayDateString(holidayDateString){};

  std::string holidayName() const {
    return m_holidayName;
  };
  std::string holidayDateString() const {
    return m_holidayDateString;
  };

 private:
  std::string m_holidayName;
  std::string m_holidayDateString;
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
  EpwDesignCondition(
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
    double extremeN50YearsMaxDryBulb);
  // Static
  /** Returns the units of the named field */
  static boost::optional<std::string> getUnitsByName(const std::string& name);
  /** Returns the units of the field specified by enumeration value */
  static std::string getUnits(EpwDesignField field);
  // Data retrieval
  /** Returns the double value of the named field if possible */
  boost::optional<double> getFieldByName(const std::string& name);
  /** Returns the dobule value of the field specified by enumeration value */
  boost::optional<double> getField(EpwDesignField id);
  // Conversion
  /** Create an EpwDesignCondition from an EPW-formatted string */
  static boost::optional<EpwDesignCondition> fromDesignConditionsString(const std::string& line);
  /** Create an EpwDesignCondition from a list of EPW designs as strings */
  static boost::optional<EpwDesignCondition> fromDesignConditionsStrings(const std::vector<std::string>& list);
  // Getters
  /** Returns the title of design condition */
  std::string titleOfDesignCondition() const;
  /** Returns the heating coldest month */
  boost::optional<int> heatingColdestMonth() const;
  /** Returns the heating dry bulb temperature 99.6% in degrees C*/
  boost::optional<double> heatingDryBulb99pt6() const;
  /** Returns the heating dry bulb temperature 99% in degrees C*/
  boost::optional<double> heatingDryBulb99() const;
  /** Returns the heating humidification dew point temperature 99.6% in degrees C*/
  boost::optional<double> heatingHumidificationDewPoint99pt6() const;
  /** Returns the heating humidification humidity ratio 99.6% in g of moisture per kg of dry air */
  boost::optional<double> heatingHumidificationHumidityRatio99pt6() const;
  /** Returns the heating humidification mean coincident dry bulb temperature 99.6% in degrees C*/
  boost::optional<double> heatingHumidificationMeanCoincidentDryBulb99pt6() const;
  /** Returns the heating humidification dew point temperature 99% in degrees C*/
  boost::optional<double> heatingHumidificationDewPoint99() const;
  /** Returns the heating humidification humidity ratio 99% in g of moisture per kg of dry air */
  boost::optional<double> heatingHumidificationHumidityRatio99() const;
  /** Returns the heating humidification mean coincient dry bulb temperature 99% in degrees C */
  boost::optional<double> heatingHumidificationMeanCoincidentDryBulb99() const;
  /** Returns the heating coldest month wind speed 0.4% in m/s */
  boost::optional<double> heatingColdestMonthWindSpeed0pt4() const;
  /** Returns the heating coldest month mean coincident dry bulb temperature 0.4% in degrees C*/
  boost::optional<double> heatingColdestMonthMeanCoincidentDryBulb0pt4() const;
  /** Returns the heating coldest month wind speed 1% in m/s */
  boost::optional<double> heatingColdestMonthWindSpeed1() const;
  /** Returns the heating coldest month mean coincident dry bulb temperature 1% in degrees C */
  boost::optional<double> heatingColdestMonthMeanCoincidentDryBulb1() const;
  /** Returns the heating mean coincident wind speed 99.6% in m/s */
  boost::optional<double> heatingMeanCoincidentWindSpeed99pt6() const;
  /** Returns the heating prevailing coincident wind direction 99.6% in degrees */
  boost::optional<int> heatingPrevailingCoincidentWindDirection99pt6() const;
  /** Returns the cooling hottest month */
  boost::optional<int> coolingHottestMonth() const;
  /** Returns the cooling dry bulb temperature range in degrees C */
  boost::optional<double> coolingDryBulbRange() const;
  /** Returns the cooling dry bulb temperature 0.4% in degrees C */
  boost::optional<double> coolingDryBulb0pt4() const;
  /** Returns the cooling mean coincident wet bulb temperature in degrees C */
  boost::optional<double> coolingMeanCoincidentWetBulb0pt4() const;
  /** Returns the cooling dry bulb temperature 1% in degrees C */
  boost::optional<double> coolingDryBulb1() const;
  /** Returns the cooling mean coincident wet bulb temperature 1% in degrees C */
  boost::optional<double> coolingMeanCoincidentWetBulb1() const;
  /** Returns the cooling dry bulb temperature 2% in degrees C */
  boost::optional<double> coolingDryBulb2() const;
  /** Returns the cooling mean coincident wet bulb temperature 2% in degrees C */
  boost::optional<double> coolingMeanCoincidentWetBulb2() const;
  /** Returns the cooling evaporation wet bulb temperature 0.4% in degrees C */
  boost::optional<double> coolingEvaporationWetBulb0pt4() const;
  /** Returns the cooling evaporation mean coincident dry bulb temperature 0.4% in degrees C */
  boost::optional<double> coolingEvaporationMeanCoincidentDryBulb0pt4() const;
  /** Returns the cooling evaporation wet bulb temperature 1% in degrees C */
  boost::optional<double> coolingEvaporationWetBulb1() const;
  /** Returns the cooling evaporation mean coincident dry bulb temperature 1% in degrees C */
  boost::optional<double> coolingEvaporationMeanCoincidentDryBulb1() const;
  /** Returns the cooling evaporation wet bulb temperature 2% in degrees C */
  boost::optional<double> coolingEvaporationWetBulb2() const;
  /** Returns the cooling evaporation mean coincident dry bulb temperature 2% in degrees C */
  boost::optional<double> coolingEvaporationMeanCoincidentDryBulb2() const;
  /** Returns the cooling mean coincident wind speed 0.4% in m/s */
  boost::optional<double> coolingMeanCoincidentWindSpeed0pt4() const;
  /** Returns the cooling prevailing coincident wind direction 0.4% in degrees */
  boost::optional<int> coolingPrevailingCoincidentWindDirection0pt4() const;
  /** Returns the cooling dehumidification dew point temperature 0.4% in degrees C */
  boost::optional<double> coolingDehumidificationDewPoint0pt4() const;
  /** Returns the cooling dehumidification humidity ratio 0.4% in g of moisture per kg of dry air */
  boost::optional<double> coolingDehumidificationHumidityRatio0pt4() const;
  /** Returns the cooling dehumidification mean coincident dry bulb temperature 0.4% in degrees C */
  boost::optional<double> coolingDehumidificationMeanCoincidentDryBulb0pt4() const;
  /** Returns the cooling dehumidification dew point temperature 1% in degrees C */
  boost::optional<double> coolingDehumidificationDewPoint1() const;
  /** Returns the cooling dehumidification humidity ratio 1% in g of moisture per kg of dry air */
  boost::optional<double> coolingDehumidificationHumidityRatio1() const;
  /** Returns the cooling dehumidification mean coincident dry bulb temperature 1% in degrees C */
  boost::optional<double> coolingDehumidificationMeanCoincidentDryBulb1() const;
  /** Returns the cooling dehumidification dew point temperature 2% in degrees C */
  boost::optional<double> coolingDehumidificationDewPoint2() const;
  /** Returns the cooling dehumidification humidity ratio 2% in g of moisture per kg of dry air */
  boost::optional<double> coolingDehumidificationHumidityRatio2() const;
  /** Returns the cooling dehumidification mean coincident dry bulb temperature 2% in degrees C */
  boost::optional<double> coolingDehumidificationMeanCoincidentDryBulb2() const;
  /** Returns the cooling enthalpy 0.4% in kJ/kg */
  boost::optional<double> coolingEnthalpy0pt4() const;
  /** Returns the cooling enthalpy mean coincident dry bulb temperature 0.4% in degrees C */
  boost::optional<double> coolingEnthalpyMeanCoincidentDryBulb0pt4() const;
  /** Returns the cooling enthalpy 1% in kJ/kg */
  boost::optional<double> coolingEnthalpy1() const;
  /** Returns the cooling enthalpy mean coincident dry bulb temperature 1% in degrees C */
  boost::optional<double> coolingEnthalpyMeanCoincidentDryBulb1() const;
  /** Returns the cooling enthalpy 2% in kJ/kg */
  boost::optional<double> coolingEnthalpy2() const;
  /** Returns the cooling enthalpy mean coincident dry bulb temperature 2% in degrees C */
  boost::optional<double> coolingEnthalpyMeanCoincidentDryBulb2() const;
  /** Returns the number of cooling hours between 8am and 4pm with dry bulb temperature between 12.8 and 20.6 degrees C */
  boost::optional<int> coolingHours8To4AndDryBulb12pt8To20pt6() const;
  /** Returns the extreme wind speed 1% in m/s */
  boost::optional<double> extremeWindSpeed1() const;
  /** Returns the extreme wind speed 2.5% in m/s */
  boost::optional<double> extremeWindSpeed2pt5() const;
  /** Returns the extreme wind speed 5% in m/s */
  boost::optional<double> extremeWindSpeed5() const;
  /** Returns the extreme maximum wet bulb temperature in degrees C */
  boost::optional<double> extremeMaxWetBulb() const;
  /** Returns the extreme mean minimum dry bulb temperature in degrees C */
  boost::optional<double> extremeMeanMinDryBulb() const;
  /** Returns the extreme mean maximum dry bulb temperature in degrees C */
  boost::optional<double> extremeMeanMaxDryBulb() const;
  /** Returns the extreme standard deviation minimum dry bulb temperature in degrees C */
  boost::optional<double> extremeStdDevMinDryBulb() const;
  /** Returns the extreme standard deviation maximum dry bulb temperature in degrees C */
  boost::optional<double> extremeStdDevMaxDryBulb() const;
  /** Returns the extreme n=5 years minimum dry bulb temperature in degrees C */
  boost::optional<double> extremeN5YearsMinDryBulb() const;
  /** Returns the extreme n=5 years maximum dry bulb temperature in degrees C */
  boost::optional<double> extremeN5YearsMaxDryBulb() const;
  /** Returns the extreme n=10 years minimum dry bulb temperature in degrees C */
  boost::optional<double> extremeN10YearsMinDryBulb() const;
  /** Returns the extreme n=10 years maximum dry bulb temperature in degrees C */
  boost::optional<double> extremeN10YearsMaxDryBulb() const;
  /** Returns the extreme n=20 years minimum dry bulb temperature in degrees C */
  boost::optional<double> extremeN20YearsMinDryBulb() const;
  /** Returns the extreme n=20 years maximum dry bulb temperature in degrees C */
  boost::optional<double> extremeN20YearsMaxDryBulb() const;
  /** Returns the extreme n=50 years minimum dry bulb temperature in degrees C */
  boost::optional<double> extremeN50YearsMinDryBulb() const;
  /** Returns the extreme n=50 years maximum dry bulb temperature in degrees C */
  boost::optional<double> extremeN50YearsMaxDryBulb() const;

 private:
  // Setters
  void setTitleOfDesignCondition(const std::string& titleOfDesignCondition);
  bool setHeatingColdestMonth(const std::string& heatingColdestMonth);
  void setHeatingColdestMonth(int heatingColdestMonth);
  bool setHeatingDryBulb99pt6(const std::string& heatingDryBulb99pt6);
  void setHeatingDryBulb99pt6(double heatingDryBulb99pt6);
  bool setHeatingDryBulb99(const std::string& heatingDryBulb99);
  void setHeatingDryBulb99(double heatingDryBulb99);
  bool setHeatingHumidificationDewPoint99pt6(const std::string& heatingHumidificationDewPoint99pt6);
  void setHeatingHumidificationDewPoint99pt6(double heatingHumidificationDewPoint99pt6);
  bool setHeatingHumidificationHumidityRatio99pt6(const std::string& heatingHumidificationHumidityRatio99pt6);
  void setHeatingHumidificationHumidityRatio99pt6(double heatingHumidificationHumidityRatio99pt6);
  bool setHeatingHumidificationMeanCoincidentDryBulb99pt6(const std::string& heatingHumidificationMeanCoincidentDryBulb99pt6);
  void setHeatingHumidificationMeanCoincidentDryBulb99pt6(double heatingHumidificationMeanCoincidentDryBulb99pt6);
  bool setHeatingHumidificationDewPoint99(const std::string& heatingHumidificationDewPoint99);
  void setHeatingHumidificationDewPoint99(double heatingHumidificationDewPoint99);
  bool setHeatingHumidificationHumidityRatio99(const std::string& heatingHumidificationHumidityRatio99);
  void setHeatingHumidificationHumidityRatio99(double heatingHumidificationHumidityRatio99);
  bool setHeatingHumidificationMeanCoincidentDryBulb99(const std::string& heatingHumidificationMeanCoincidentDryBulb99);
  void setHeatingHumidificationMeanCoincidentDryBulb99(double heatingHumidificationMeanCoincidentDryBulb99);
  bool setHeatingColdestMonthWindSpeed0pt4(const std::string& heatingColdestMonthWindSpeed0pt4);
  void setHeatingColdestMonthWindSpeed0pt4(double heatingColdestMonthWindSpeed0pt4);
  bool setHeatingColdestMonthMeanCoincidentDryBulb0pt4(const std::string& heatingColdestMonthMeanCoincidentDryBulb0pt4);
  void setHeatingColdestMonthMeanCoincidentDryBulb0pt4(double heatingColdestMonthMeanCoincidentDryBulb0pt4);
  bool setHeatingColdestMonthWindSpeed1(const std::string& heatingColdestMonthWindSpeed1);
  void setHeatingColdestMonthWindSpeed1(double heatingColdestMonthWindSpeed1);
  bool setHeatingColdestMonthMeanCoincidentDryBulb1(const std::string& heatingColdestMonthMeanCoincidentDryBulb1);
  void setHeatingColdestMonthMeanCoincidentDryBulb1(double heatingColdestMonthMeanCoincidentDryBulb1);
  bool setHeatingMeanCoincidentWindSpeed99pt6(const std::string& heatingMeanCoincidentWindSpeed99pt6);
  void setHeatingMeanCoincidentWindSpeed99pt6(double heatingMeanCoincidentWindSpeed99pt6);
  bool setHeatingPrevailingCoincidentWindDirection99pt6(const std::string& heatingPrevailingCoincidentWindDirection99pt6);
  void setHeatingPrevailingCoincidentWindDirection99pt6(int heatingPrevailingCoincidentWindDirection99pt6);
  bool setCoolingHottestMonth(const std::string& coolingHottestMonth);
  void setCoolingHottestMonth(int coolingHottestMonth);
  bool setCoolingDryBulbRange(const std::string& coolingDryBulbRange);
  void setCoolingDryBulbRange(double coolingDryBulbRange);
  bool setCoolingDryBulb0pt4(const std::string& coolingDryBulb0pt4);
  void setCoolingDryBulb0pt4(double coolingDryBulb0pt4);
  bool setCoolingMeanCoincidentWetBulb0pt4(const std::string& coolingMeanCoincidentWetBulb0pt4);
  void setCoolingMeanCoincidentWetBulb0pt4(double coolingMeanCoincidentWetBulb0pt4);
  bool setCoolingDryBulb1(const std::string& coolingDryBulb1);
  void setCoolingDryBulb1(double coolingDryBulb1);
  bool setCoolingMeanCoincidentWetBulb1(const std::string& coolingMeanCoincidentWetBulb1);
  void setCoolingMeanCoincidentWetBulb1(double coolingMeanCoincidentWetBulb1);
  bool setCoolingDryBulb2(const std::string& coolingDryBulb2);
  void setCoolingDryBulb2(double coolingDryBulb2);
  bool setCoolingMeanCoincidentWetBulb2(const std::string& coolingMeanCoincidentWetBulb2);
  void setCoolingMeanCoincidentWetBulb2(double coolingMeanCoincidentWetBulb2);
  bool setCoolingEvaporationWetBulb0pt4(const std::string& coolingEvaporationWetBulb0pt4);
  void setCoolingEvaporationWetBulb0pt4(double coolingEvaporationWetBulb0pt4);
  bool setCoolingEvaporationMeanCoincidentDryBulb0pt4(const std::string& coolingEvaporationMeanCoincidentDryBulb0pt4);
  void setCoolingEvaporationMeanCoincidentDryBulb0pt4(double coolingEvaporationMeanCoincidentDryBulb0pt4);
  bool setCoolingEvaporationWetBulb1(const std::string& coolingEvaporationWetBulb1);
  void setCoolingEvaporationWetBulb1(double coolingEvaporationWetBulb1);
  bool setCoolingEvaporationMeanCoincidentDryBulb1(const std::string& coolingEvaporationMeanCoincidentDryBulb1);
  void setCoolingEvaporationMeanCoincidentDryBulb1(double coolingEvaporationMeanCoincidentDryBulb1);
  bool setCoolingEvaporationWetBulb2(const std::string& coolingEvaporationWetBulb2);
  void setCoolingEvaporationWetBulb2(double coolingEvaporationWetBulb2);
  bool setCoolingEvaporationMeanCoincidentDryBulb2(const std::string& coolingEvaporationMeanCoincidentDryBulb2);
  void setCoolingEvaporationMeanCoincidentDryBulb2(double coolingEvaporationMeanCoincidentDryBulb2);
  bool setCoolingMeanCoincidentWindSpeed0pt4(const std::string& coolingMeanCoincidentWindSpeed0pt4);
  void setCoolingMeanCoincidentWindSpeed0pt4(double coolingMeanCoincidentWindSpeed0pt4);
  bool setCoolingPrevailingCoincidentWindDirection0pt4(const std::string& coolingPrevailingCoincidentWindDirection0pt4);
  void setCoolingPrevailingCoincidentWindDirection0pt4(int coolingPrevailingCoincidentWindDirection0pt4);
  bool setCoolingDehumidificationDewPoint0pt4(const std::string& coolingDehumidificationDewPoint0pt4);
  void setCoolingDehumidificationDewPoint0pt4(double coolingDehumidificationDewPoint0pt4);
  bool setCoolingDehumidificationHumidityRatio0pt4(const std::string& coolingDehumidificationHumidityRatio0pt4);
  void setCoolingDehumidificationHumidityRatio0pt4(double coolingDehumidificationHumidityRatio0pt4);
  bool setCoolingDehumidificationMeanCoincidentDryBulb0pt4(const std::string& coolingDehumidificationMeanCoincidentDryBulb0pt4);
  void setCoolingDehumidificationMeanCoincidentDryBulb0pt4(double coolingDehumidificationMeanCoincidentDryBulb0pt4);
  bool setCoolingDehumidificationDewPoint1(const std::string& coolingDehumidificationDewPoint1);
  void setCoolingDehumidificationDewPoint1(double coolingDehumidificationDewPoint1);
  bool setCoolingDehumidificationHumidityRatio1(const std::string& coolingDehumidificationHumidityRatio1);
  void setCoolingDehumidificationHumidityRatio1(double coolingDehumidificationHumidityRatio1);
  bool setCoolingDehumidificationMeanCoincidentDryBulb1(const std::string& coolingDehumidificationMeanCoincidentDryBulb1);
  void setCoolingDehumidificationMeanCoincidentDryBulb1(double coolingDehumidificationMeanCoincidentDryBulb1);
  bool setCoolingDehumidificationDewPoint2(const std::string& coolingDehumidificationDewPoint2);
  void setCoolingDehumidificationDewPoint2(double coolingDehumidificationDewPoint2);
  bool setCoolingDehumidificationHumidityRatio2(const std::string& coolingDehumidificationHumidityRatio2);
  void setCoolingDehumidificationHumidityRatio2(double coolingDehumidificationHumidityRatio2);
  bool setCoolingDehumidificationMeanCoincidentDryBulb2(const std::string& coolingDehumidificationMeanCoincidentDryBulb2);
  void setCoolingDehumidificationMeanCoincidentDryBulb2(double coolingDehumidificationMeanCoincidentDryBulb2);
  bool setCoolingEnthalpy0pt4(const std::string& coolingEnthalpy0pt4);
  void setCoolingEnthalpy0pt4(double coolingEnthalpy0pt4);
  bool setCoolingEnthalpyMeanCoincidentDryBulb0pt4(const std::string& coolingEnthalpyMeanCoincidentDryBulb0pt4);
  void setCoolingEnthalpyMeanCoincidentDryBulb0pt4(double coolingEnthalpyMeanCoincidentDryBulb0pt4);
  bool setCoolingEnthalpy1(const std::string& coolingEnthalpy1);
  void setCoolingEnthalpy1(double coolingEnthalpy1);
  bool setCoolingEnthalpyMeanCoincidentDryBulb1(const std::string& coolingEnthalpyMeanCoincidentDryBulb1);
  void setCoolingEnthalpyMeanCoincidentDryBulb1(double coolingEnthalpyMeanCoincidentDryBulb1);
  bool setCoolingEnthalpy2(const std::string& coolingEnthalpy2);
  void setCoolingEnthalpy2(double coolingEnthalpy2);
  bool setCoolingEnthalpyMeanCoincidentDryBulb2(const std::string& coolingEnthalpyMeanCoincidentDryBulb2);
  void setCoolingEnthalpyMeanCoincidentDryBulb2(double coolingEnthalpyMeanCoincidentDryBulb2);
  bool setCoolingHours8To4AndDryBulb12pt8To20pt6(const std::string& coolingHours8To4AndDryBulb12pt8To20pt6);
  void setCoolingHours8To4AndDryBulb12pt8To20pt6(int coolingHours8To4AndDryBulb12pt8To20pt6);
  bool setExtremeWindSpeed1(const std::string& extremeWindSpeed1);
  void setExtremeWindSpeed1(double extremeWindSpeed1);
  bool setExtremeWindSpeed2pt5(const std::string& extremeWindSpeed2pt5);
  void setExtremeWindSpeed2pt5(double extremeWindSpeed2pt5);
  bool setExtremeWindSpeed5(const std::string& extremeWindSpeed5);
  void setExtremeWindSpeed5(double extremeWindSpeed5);
  bool setExtremeMaxWetBulb(const std::string& extremeMaxWetBulb);
  void setExtremeMaxWetBulb(double extremeMaxWetBulb);
  bool setExtremeMeanMinDryBulb(const std::string& extremeMeanMinDryBulb);
  void setExtremeMeanMinDryBulb(double extremeMeanMinDryBulb);
  bool setExtremeMeanMaxDryBulb(const std::string& extremeMeanMaxDryBulb);
  void setExtremeMeanMaxDryBulb(double extremeMeanMaxDryBulb);
  bool setExtremeStdDevMinDryBulb(const std::string& extremeStdDevMinDryBulb);
  void setExtremeStdDevMinDryBulb(double extremeStdDevMinDryBulb);
  bool setExtremeStdDevMaxDryBulb(const std::string& extremeStdDevMaxDryBulb);
  void setExtremeStdDevMaxDryBulb(double extremeStdDevMaxDryBulb);
  bool setExtremeN5YearsMinDryBulb(const std::string& extremeN5YearsMinDryBulb);
  void setExtremeN5YearsMinDryBulb(double extremeN5YearsMinDryBulb);
  bool setExtremeN5YearsMaxDryBulb(const std::string& extremeN5YearsMaxDryBulb);
  void setExtremeN5YearsMaxDryBulb(double extremeN5YearsMaxDryBulb);
  bool setExtremeN10YearsMinDryBulb(const std::string& extremeN10YearsMinDryBulb);
  void setExtremeN10YearsMinDryBulb(double extremeN10YearsMinDryBulb);
  bool setExtremeN10YearsMaxDryBulb(const std::string& extremeN10YearsMaxDryBulb);
  void setExtremeN10YearsMaxDryBulb(double extremeN10YearsMaxDryBulb);
  bool setExtremeN20YearsMinDryBulb(const std::string& extremeN20YearsMinDryBulb);
  void setExtremeN20YearsMinDryBulb(double extremeN20YearsMinDryBulb);
  bool setExtremeN20YearsMaxDryBulb(const std::string& extremeN20YearsMaxDryBulb);
  void setExtremeN20YearsMaxDryBulb(double extremeN20YearsMaxDryBulb);
  bool setExtremeN50YearsMinDryBulb(const std::string& extremeN50YearsMinDryBulb);
  void setExtremeN50YearsMinDryBulb(double extremeN50YearsMinDryBulb);
  bool setExtremeN50YearsMaxDryBulb(const std::string& extremeN50YearsMaxDryBulb);
  void setExtremeN50YearsMaxDryBulb(double extremeN50YearsMaxDryBulb);

  std::string m_titleOfDesignCondition;
  boost::optional<int> m_heatingColdestMonth;
  boost::optional<double> m_heatingDryBulb99pt6;
  boost::optional<double> m_heatingDryBulb99;
  boost::optional<double> m_heatingHumidificationDewPoint99pt6;
  boost::optional<double> m_heatingHumidificationHumidityRatio99pt6;
  boost::optional<double> m_heatingHumidificationMeanCoincidentDryBulb99pt6;
  boost::optional<double> m_heatingHumidificationDewPoint99;
  boost::optional<double> m_heatingHumidificationHumidityRatio99;
  boost::optional<double> m_heatingHumidificationMeanCoincidentDryBulb99;
  boost::optional<double> m_heatingColdestMonthWindSpeed0pt4;
  boost::optional<double> m_heatingColdestMonthMeanCoincidentDryBulb0pt4;
  boost::optional<double> m_heatingColdestMonthWindSpeed1;
  boost::optional<double> m_heatingColdestMonthMeanCoincidentDryBulb1;
  boost::optional<double> m_heatingMeanCoincidentWindSpeed99pt6;
  boost::optional<int> m_heatingPrevailingCoincidentWindDirection99pt6;
  boost::optional<int> m_coolingHottestMonth;
  boost::optional<double> m_coolingDryBulbRange;
  boost::optional<double> m_coolingDryBulb0pt4;
  boost::optional<double> m_coolingMeanCoincidentWetBulb0pt4;
  boost::optional<double> m_coolingDryBulb1;
  boost::optional<double> m_coolingMeanCoincidentWetBulb1;
  boost::optional<double> m_coolingDryBulb2;
  boost::optional<double> m_coolingMeanCoincidentWetBulb2;
  boost::optional<double> m_coolingEvaporationWetBulb0pt4;
  boost::optional<double> m_coolingEvaporationMeanCoincidentDryBulb0pt4;
  boost::optional<double> m_coolingEvaporationWetBulb1;
  boost::optional<double> m_coolingEvaporationMeanCoincidentDryBulb1;
  boost::optional<double> m_coolingEvaporationWetBulb2;
  boost::optional<double> m_coolingEvaporationMeanCoincidentDryBulb2;
  boost::optional<double> m_coolingMeanCoincidentWindSpeed0pt4;
  boost::optional<int> m_coolingPrevailingCoincidentWindDirection0pt4;
  boost::optional<double> m_coolingDehumidificationDewPoint0pt4;
  boost::optional<double> m_coolingDehumidificationHumidityRatio0pt4;
  boost::optional<double> m_coolingDehumidificationMeanCoincidentDryBulb0pt4;
  boost::optional<double> m_coolingDehumidificationDewPoint1;
  boost::optional<double> m_coolingDehumidificationHumidityRatio1;
  boost::optional<double> m_coolingDehumidificationMeanCoincidentDryBulb1;
  boost::optional<double> m_coolingDehumidificationDewPoint2;
  boost::optional<double> m_coolingDehumidificationHumidityRatio2;
  boost::optional<double> m_coolingDehumidificationMeanCoincidentDryBulb2;
  boost::optional<double> m_coolingEnthalpy0pt4;
  boost::optional<double> m_coolingEnthalpyMeanCoincidentDryBulb0pt4;
  boost::optional<double> m_coolingEnthalpy1;
  boost::optional<double> m_coolingEnthalpyMeanCoincidentDryBulb1;
  boost::optional<double> m_coolingEnthalpy2;
  boost::optional<double> m_coolingEnthalpyMeanCoincidentDryBulb2;
  boost::optional<int> m_coolingHours8To4AndDryBulb12pt8To20pt6;
  boost::optional<double> m_extremeWindSpeed1;
  boost::optional<double> m_extremeWindSpeed2pt5;
  boost::optional<double> m_extremeWindSpeed5;
  boost::optional<double> m_extremeMaxWetBulb;
  boost::optional<double> m_extremeMeanMinDryBulb;
  boost::optional<double> m_extremeMeanMaxDryBulb;
  boost::optional<double> m_extremeStdDevMinDryBulb;
  boost::optional<double> m_extremeStdDevMaxDryBulb;
  boost::optional<double> m_extremeN5YearsMinDryBulb;
  boost::optional<double> m_extremeN5YearsMaxDryBulb;
  boost::optional<double> m_extremeN10YearsMinDryBulb;
  boost::optional<double> m_extremeN10YearsMaxDryBulb;
  boost::optional<double> m_extremeN20YearsMinDryBulb;
  boost::optional<double> m_extremeN20YearsMaxDryBulb;
  boost::optional<double> m_extremeN50YearsMinDryBulb;
  boost::optional<double> m_extremeN50YearsMaxDryBulb;
};

/** EpwGroundTemperatureDepth is one line from the EPW file. All floating point numbers are stored as strings,
* but are checked as numbers.
*/
class UTILITIES_API EpwGroundTemperatureDepth
{
 public:
  /** Create an empty EpwGroundTemperatureDepth object */
  EpwGroundTemperatureDepth() = default;
  /** Create an EpwGroundTemperatureDepth object with specified properties */
  EpwGroundTemperatureDepth(double groundTemperatureDepth, double soilConductivity, double soilDensity, double soilSpecificHeat,
                            double janGroundTemperature, double febGroundTemperature, double marGroundTemperature, double aprGroundTemperature,
                            double mayGroundTemperature, double junGroundTemperature, double julGroundTemperature, double augGroundTemperature,
                            double sepGroundTemperature, double octGroundTemperature, double novGroundTemperature, double decGroundTemperature);
  // Static
  /** Returns the units of the named field */
  static boost::optional<std::string> getUnitsByName(const std::string& name);
  /** Returns the units of the field specified by enumeration value */
  static std::string getUnits(EpwDepthField field);
  // Data retrieval
  /** Returns the double value of the named field if possible */
  boost::optional<double> getFieldByName(const std::string& name);
  /** Returns the dobule value of the field specified by enumeration value */
  boost::optional<double> getField(EpwDepthField id);
  // Conversion
  /** Create an EpwGroundTemperatureDepth from an EPW-formatted string */
  static boost::optional<EpwGroundTemperatureDepth> fromGroundTemperatureDepthsString(const std::string& line);
  /** Create an EpwGroundTemperatureDepth from a list of EPW depths as strings */
  static boost::optional<EpwGroundTemperatureDepth> fromGroundTemperatureDepthsStrings(const std::vector<std::string>& strings);
  // Getters
  /** Returns the depth of ground temperature in m*/
  double groundTemperatureDepth() const;
  /** Returns the soil conductivity in W/m-K*/
  double soilConductivity() const;
  /** Returns the soil density in kg/m3*/
  double soilDensity() const;
  /** Returns the soil specific heat in J/kg-K*/
  double soilSpecificHeat() const;
  /** Returns the Jan undisturbed ground temperature in degrees C*/
  double janGroundTemperature() const;
  /** Returns the Feb undisturbed ground temperature in degrees C*/
  double febGroundTemperature() const;
  /** Returns the Mar undisturbed ground temperature in degrees C*/
  double marGroundTemperature() const;
  /** Returns the Apr undisturbed ground temperature in degrees C*/
  double aprGroundTemperature() const;
  /** Returns the May undisturbed ground temperature in degrees C*/
  double mayGroundTemperature() const;
  /** Returns the Jun undisturbed ground temperature in degrees C*/
  double junGroundTemperature() const;
  /** Returns the Jul undisturbed ground temperature in degrees C*/
  double julGroundTemperature() const;
  /** Returns the Aug undisturbed ground temperature in degrees C*/
  double augGroundTemperature() const;
  /** Returns the Sep undisturbed ground temperature in degrees C*/
  double sepGroundTemperature() const;
  /** Returns the Oct undisturbed ground temperature in degrees C*/
  double octGroundTemperature() const;
  /** Returns the Nov undisturbed ground temperature in degrees C*/
  double novGroundTemperature() const;
  /** Returns the Dec undisturbed ground temperature in degrees C*/
  double decGroundTemperature() const;

 private:
  // Setters
  void setGroundTemperatureDepth(double groundTemperatureDepth);
  void setSoilConductivity(double soilConductivity);
  void setSoilDensity(double soilDensity);
  void setSoilSpecificHeat(double soilSpecificHeat);
  void setJanGroundTemperature(double janGroundTemperature);
  void setFebGroundTemperature(double febGroundTemperature);
  void setMarGroundTemperature(double marGroundTemperature);
  void setAprGroundTemperature(double aprGroundTemperature);
  void setMayGroundTemperature(double mayGroundTemperature);
  void setJunGroundTemperature(double junGroundTemperature);
  void setJulGroundTemperature(double julGroundTemperature);
  void setAugGroundTemperature(double augGroundTemperature);
  void setSepGroundTemperature(double sepGroundTemperature);
  void setOctGroundTemperature(double octGroundTemperature);
  void setNovGroundTemperature(double novGroundTemperature);
  void setDecGroundTemperature(double decGroundTemperature);
  bool setGroundTemperatureDepth(const std::string& groundTemperatureDepth);
  bool setSoilConductivity(const std::string& soilConductivity);
  bool setSoilDensity(const std::string& soilDensity);
  bool setSoilSpecificHeat(const std::string& soilSpecificHeat);
  bool setJanGroundTemperature(const std::string& janGroundTemperature);
  bool setFebGroundTemperature(const std::string& febGroundTemperature);
  bool setMarGroundTemperature(const std::string& marGroundTemperature);
  bool setAprGroundTemperature(const std::string& aprGroundTemperature);
  bool setMayGroundTemperature(const std::string& mayGroundTemperature);
  bool setJunGroundTemperature(const std::string& junGroundTemperature);
  bool setJulGroundTemperature(const std::string& julGroundTemperature);
  bool setAugGroundTemperature(const std::string& augGroundTemperature);
  bool setSepGroundTemperature(const std::string& sepGroundTemperature);
  bool setOctGroundTemperature(const std::string& octGroundTemperature);
  bool setNovGroundTemperature(const std::string& novGroundTemperature);
  bool setDecGroundTemperature(const std::string& decGroundTemperature);

  double m_groundTemperatureDepth = -9999;
  double m_soilConductivity = -9999;
  double m_soilDensity = -9999;
  double m_soilSpecificHeat = -9999;
  double m_janGroundTemperature = -9999;
  double m_febGroundTemperature = -9999;
  double m_marGroundTemperature = -9999;
  double m_aprGroundTemperature = -9999;
  double m_mayGroundTemperature = -9999;
  double m_junGroundTemperature = -9999;
  double m_julGroundTemperature = -9999;
  double m_augGroundTemperature = -9999;
  double m_sepGroundTemperature = -9999;
  double m_octGroundTemperature = -9999;
  double m_novGroundTemperature = -9999;
  double m_decGroundTemperature = -9999;
};

/** EpwFile parses a weather file in EPW format.  Later it may provide
 *   methods for writing and converting other weather files to EPW format.
 */
class UTILITIES_API EpwFile
{
 public:
  /// constructor with path
  /// will throw if path does not exist or file is incorrect
  EpwFile(const openstudio::path& p, bool storeData = false);

  /// static load method
  static boost::optional<EpwFile> load(const openstudio::path& p, bool storeData = false);

  /// static load method
  static boost::optional<EpwFile> loadFromString(const std::string& str, bool storeData = false);

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

  /// get the ground temperature depths
  std::vector<EpwGroundTemperatureDepth> groundTemperatureDepths();

  /// get a time series of a particular weather field
  // This will probably need to include the period at some point, but for now just dump everything into a time series
  boost::optional<TimeSeries> getTimeSeries(const std::string& field);
  /// get a time series of a computed quantity
  boost::optional<TimeSeries> getComputedTimeSeries(const std::string& field);

  /// export to CONTAM WTH file
  bool translateToWth(openstudio::path path, std::string description = std::string());

  // Data status (?) functions
  /// Returns true if the file appears to be AMY (as opposed to TMY)
  bool isActual() const;
  /// Returns true if the data period "records per hour" input matches the data point values
  bool minutesMatch() const;

  boost::optional<Date> daylightSavingStartDate() const;
  boost::optional<Date> daylightSavingEndDate() const;
  std::vector<EpwHoliday> holidays() const;

 private:
  EpwFile();
  bool parse(std::istream& is, bool storeData = false);
  bool parseLocation(const std::string& line);
  bool parseDesignConditions(const std::string& line);
  bool parseDataPeriod(const std::string& line);
  bool parseHolidaysDaylightSavings(const std::string& line);
  bool parseGroundTemperatures(const std::string& line);

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
  std::vector<EpwGroundTemperatureDepth> m_depths;

  bool m_leapYearObserved;
  boost::optional<Date> m_daylightSavingStartDate;
  boost::optional<Date> m_daylightSavingEndDate;
  std::vector<EpwHoliday> m_holidays;

  bool m_isActual;

  // Error/warning flags to store how well the input matches what we think it should
  bool m_minutesMatch;  // No disagreement between the data period and the minutes field
};

UTILITIES_API IdfObject toIdfObject(const EpwFile& epwFile);

}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_EPWFILE_HPP
