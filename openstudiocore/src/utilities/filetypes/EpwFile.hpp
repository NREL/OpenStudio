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
  /** Creata an EpwDataPoint from a list of EPW data as strings. The pedantic argument controls how strict the conversion is. 
      If pedantic is true, the list should have 35 elements. If pedantic is false, lists with more or fewer elements may
      still result in an EpwDataPoint */
  static boost::optional<EpwDataPoint> fromEpwStrings(const std::vector<std::string> &list, bool pedantic=true);
  /** Creata an EpwDataPoint from a list of EPW data as strings, overriding the date and time with the specified arguments. 
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

  bool parse(bool storeData=false);
  bool parseLocation(const std::string& line);
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

  bool m_isActual;

  // Error/warning flags to store how well the input matches what we think it should
  bool m_minutesMatch; // No disagreement between the data period and the minutes field
};

UTILITIES_API IdfObject toIdfObject(const EpwFile& epwFile);

} // openstudio

#endif //UTILITIES_FILETYPES_EPWFILE_HPP
