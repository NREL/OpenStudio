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

/** EpwDataPoint is one line from the EPW file. All floating point numbers are stored as strings,
 * but are checked as numbers.
 */
class UTILITIES_API EpwDataPoint
{
public:
  EpwDataPoint();
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
  static boost::optional<std::string> unitsByName(const std::string &name);
  static std::string units(EpwDataField field);
  // Data retrieval
  boost::optional<double> fieldByName(const std::string &name);
  boost::optional<double> field(EpwDataField id);
  // Conversion
  static boost::optional<EpwDataPoint> fromEpwString(const std::string &line);
  static boost::optional<EpwDataPoint> fromEpwStrings(const std::vector<std::string> &list, bool pedantic=true);
  std::vector<std::string> toEpwStrings() const;
  boost::optional<std::string> toWthString() const;
  // One billion getters
  Date date() const;
  Time time() const;
  openstudio::DateTime dateTime() const;
  int year() const;
  int month() const;
  int day() const;
  int hour() const;
  int minute() const;
  std::string dataSourceandUncertaintyFlags() const;
  boost::optional<double> dryBulbTemperature() const;
  boost::optional<double> dewPointTemperature() const;
  boost::optional<double> relativeHumidity() const;
  boost::optional<double> atmosphericStationPressure() const;
  boost::optional<double> extraterrestrialHorizontalRadiation() const;
  boost::optional<double> extraterrestrialDirectNormalRadiation() const;
  boost::optional<double> horizontalInfraredRadiationIntensity() const;
  boost::optional<double> globalHorizontalRadiation() const;
  boost::optional<double> directNormalRadiation() const;
  boost::optional<double> diffuseHorizontalRadiation() const;
  boost::optional<double> globalHorizontalIlluminance() const;
  boost::optional<double> directNormalIlluminance() const;
  boost::optional<double> diffuseHorizontalIlluminance() const;
  boost::optional<double> zenithLuminance() const;
  boost::optional<double> windDirection() const;
  boost::optional<double> windSpeed() const;
  int totalSkyCover() const;
  int opaqueSkyCover() const;
  boost::optional<double> visibility() const;
  boost::optional<double> ceilingHeight() const;
  int presentWeatherObservation() const;
  int presentWeatherCodes() const;
  boost::optional<double> precipitableWater() const;
  boost::optional<double> aerosolOpticalDepth() const;
  boost::optional<double> snowDepth() const;
  boost::optional<double> daysSinceLastSnowfall() const;
  boost::optional<double> albedo() const;
  boost::optional<double> liquidPrecipitationDepth() const;
  boost::optional<double> liquidPrecipitationQuantity() const;

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

  /// export to CONTAM WTH file
  bool translateToWth(openstudio::path path,std::string description=std::string());

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
};

UTILITIES_API IdfObject toIdfObject(const EpwFile& epwFile);

} // openstudio

#endif //UTILITIES_FILETYPES_EPWFILE_HPP
