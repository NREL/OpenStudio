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
  static boost::optional<std::string> unitsByName(std::string name);
  static std::string units(EpwDataField field);
  // Data retrieval
  boost::optional<double> fieldByName(std::string name);
  boost::optional<double> field(EpwDataField id);
  // Conversion
  static boost::optional<EpwDataPoint> fromEpwString(std::string line);
  boost::optional<std::string> toWthString();
  // One billion getters and setters
  Date date() const;
  void setDate(Date date);
  Time time() const;
  void setTime(Time time);
  openstudio::DateTime dateTime() const;
  void setDateTime(openstudio::DateTime dateTime);
  int year() const;
  void setYear(int year);
  bool setYear(std::string year);
  int month() const;
  bool setMonth(int month);
  bool setMonth(std::string month);
  int day() const;
  bool setDay(int day);
  bool setDay(std::string day);
  int hour() const;
  bool setHour(int hour);
  bool setHour(std::string hour);
  int minute() const;
  bool setMinute(int minute);
  bool setMinute(std::string minute);
  std::string dataSourceandUncertaintyFlags() const;
  void setDataSourceandUncertaintyFlags(std::string dataSourceandUncertaintyFlags);
  boost::optional<double> dryBulbTemperature() const;
  bool setDryBulbTemperature(double dryBulbTemperature);
  bool setDryBulbTemperature(std::string dryBulbTemperature);
  boost::optional<double> dewPointTemperature() const;
  bool setDewPointTemperature(double dewPointTemperature);
  bool setDewPointTemperature(std::string dewPointTemperature);
  boost::optional<double> relativeHumidity() const;
  bool setRelativeHumidity(double relativeHumidity);
  bool setRelativeHumidity(std::string relativeHumidity);
  boost::optional<double> atmosphericStationPressure() const;
  bool setAtmosphericStationPressure(double atmosphericStationPressure);
  bool setAtmosphericStationPressure(std::string atmosphericStationPressure);
  boost::optional<double> extraterrestrialHorizontalRadiation() const;
  bool setExtraterrestrialHorizontalRadiation(double extraterrestrialHorizontalRadiation);
  bool setExtraterrestrialHorizontalRadiation(std::string extraterrestrialHorizontalRadiation);
  boost::optional<double> extraterrestrialDirectNormalRadiation() const;
  bool setExtraterrestrialDirectNormalRadiation(double extraterrestrialDirectNormalRadiation);
  bool setExtraterrestrialDirectNormalRadiation(std::string extraterrestrialDirectNormalRadiation);
  boost::optional<double> horizontalInfraredRadiationIntensity() const;
  bool setHorizontalInfraredRadiationIntensity(double horizontalInfraredRadiationIntensity);
  bool setHorizontalInfraredRadiationIntensity(std::string horizontalInfraredRadiationIntensity);
  boost::optional<double> globalHorizontalRadiation() const;
  bool setGlobalHorizontalRadiation(double globalHorizontalRadiation);
  bool setGlobalHorizontalRadiation(std::string globalHorizontalRadiation);
  boost::optional<double> directNormalRadiation() const;
  bool setDirectNormalRadiation(double directNormalRadiation);
  bool setDirectNormalRadiation(std::string directNormalRadiation);
  boost::optional<double> diffuseHorizontalRadiation() const;
  bool setDiffuseHorizontalRadiation(double diffuseHorizontalRadiation);
  bool setDiffuseHorizontalRadiation(std::string diffuseHorizontalRadiation);
  boost::optional<double> globalHorizontalIlluminance() const;
  bool setGlobalHorizontalIlluminance(double globalHorizontalIlluminance);
  bool setGlobalHorizontalIlluminance(std::string globalHorizontalIlluminance);
  boost::optional<double> directNormalIlluminance() const;
  bool setDirectNormalIlluminance(double directNormalIlluminance);
  bool setDirectNormalIlluminance(std::string directNormalIlluminance);
  boost::optional<double> diffuseHorizontalIlluminance() const;
  bool setDiffuseHorizontalIlluminance(double diffuseHorizontalIlluminance);
  bool setDiffuseHorizontalIlluminance(std::string diffuseHorizontalIlluminance);
  boost::optional<double> zenithLuminance() const;
  bool setZenithLuminance(double zenithLuminance);
  bool setZenithLuminance(std::string zenithLuminance);
  boost::optional<double> windDirection() const;
  bool setWindDirection(double windDirection);
  bool setWindDirection(std::string windDirection);
  boost::optional<double> windSpeed() const;
  bool setWindSpeed(double windSpeed);
  bool setWindSpeed(std::string windSpeed);
  int totalSkyCover() const;
  bool setTotalSkyCover(int totalSkyCover);
  bool setTotalSkyCover(std::string totalSkyCover);
  int opaqueSkyCover() const;
  bool setOpaqueSkyCover(int opaqueSkyCover);
  bool setOpaqueSkyCover(std::string opaqueSkyCover);
  boost::optional<double> visibility() const;
  void setVisibility(double visibility);
  bool setVisibility(std::string visibility);
  boost::optional<double> ceilingHeight() const;
  void setCeilingHeight(double ceilingHeight);
  bool setCeilingHeight(std::string ceilingHeight);
  int presentWeatherObservation() const;
  void setPresentWeatherObservation(int presentWeatherObservation);
  bool setPresentWeatherObservation(std::string presentWeatherObservation);
  int presentWeatherCodes() const;
  void setPresentWeatherCodes(int presentWeatherCodes);
  bool setPresentWeatherCodes(std::string presentWeatherCodes);
  boost::optional<double> precipitableWater() const;
  void setPrecipitableWater(double precipitableWater);
  bool setPrecipitableWater(std::string precipitableWater);
  boost::optional<double> aerosolOpticalDepth() const;
  void setAerosolOpticalDepth(double aerosolOpticalDepth);
  bool setAerosolOpticalDepth(std::string aerosolOpticalDepth);
  boost::optional<double> snowDepth() const;
  void setSnowDepth(double snowDepth);
  bool setSnowDepth(std::string snowDepth);
  boost::optional<double> daysSinceLastSnowfall() const;
  void setDaysSinceLastSnowfall(double daysSinceLastSnowfall);
  bool setDaysSinceLastSnowfall(std::string daysSinceLastSnowfall);
  boost::optional<double> albedo() const;
  void setAlbedo(double albedo);
  bool setAlbedo(std::string albedo);
  boost::optional<double> liquidPrecipitationDepth() const;
  void setLiquidPrecipitationDepth(double liquidPrecipitationDepth);
  bool setLiquidPrecipitationDepth(std::string liquidPrecipitationDepth);
  boost::optional<double> liquidPrecipitationQuantity() const;
  void setLiquidPrecipitationQuantity(double liquidPrecipitationQuantity);
  bool setLiquidPrecipitationQuantity(std::string liquidPrecipitationQuantity);

private:
  int m_year;
  int m_month;
  int m_day;
  int m_hour;
  int m_minute;
  std::string m_dataSourceandUncertaintyFlags;
  QString m_dryBulbTemperature; // units C, minimum> -70, maximum< 70, missing 99.9
  QString m_dewPointTemperature; // units C, minimum> -70, maximum< 70, missing 99.9
  QString m_relativeHumidity; // missing 999., minimum 0, maximum 110
  QString m_atmosphericStationPressure; // units Pa, missing 999999.,  minimum> 31000, maximum< 120000
  QString m_extraterrestrialHorizontalRadiation; // units Wh/m2, missing 9999., minimum 0
  QString m_extraterrestrialDirectNormalRadiation; //units Wh/m2, missing 9999., minimum 0
  QString m_horizontalInfraredRadiationIntensity; // units Wh/m2, missing 9999., minimum 0
  QString m_globalHorizontalRadiation; // units Wh/m2, missing 9999., minimum 0
  QString m_directNormalRadiation; // units Wh/m2, missing 9999., minimum 0
  QString m_diffuseHorizontalRadiation; // units Wh/m2, missing 9999., minimum 0
  QString m_globalHorizontalIlluminance; // units lux, missing 999999., note will be missing if >= 999900, minimum 0
  QString m_directNormalIlluminance; // units lux, missing 999999., will be missing if >= 999900, minimum 0
  QString m_diffuseHorizontalIlluminance; // units lux, missing 999999., will be missing if >= 999900, minimum 0
  QString m_zenithLuminance; // units Cd/m2, missing 9999., will be missing if >= 9999, minimum 0
  QString m_windDirection; // units degrees, missing 999., minimum 0, maximum 360
  QString m_windSpeed; // units m/s, missing 999., minimum 0, maximum 40
  int m_totalSkyCover; // missing 99, minimum 0, maximum 10
  int m_opaqueSkyCover; // used if Horizontal IR Intensity missing, missing 99, minimum 0, maximum 10
  QString m_visibility; // units km, missing 9999
  QString m_ceilingHeight; // units m, missing 99999
  int m_presentWeatherObservation;
  int m_presentWeatherCodes;
  QString m_precipitableWater; // units mm, missing 999
  QString m_aerosolOpticalDepth; // units thousandths, missing .999
  QString m_snowDepth; // units cm, missing 999
  QString m_daysSinceLastSnowfall; // missing 99
  QString m_albedo; //missing 999
  QString m_liquidPrecipitationDepth; // units mm, missing 999
  QString m_liquidPrecipitationQuantity; // units hr, missing 99
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
  boost::optional<TimeSeries> getTimeSeries(std::string field);

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
