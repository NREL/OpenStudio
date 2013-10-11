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

#ifndef UTILITIES_FILETYPES_EPWFILE_HPP
#define UTILITIES_FILETYPES_EPWFILE_HPP

#include <utilities/UtilitiesAPI.hpp>

#include <utilities/core/Path.hpp>
#include <utilities/core/Logger.hpp>
#include <utilities/time/Time.hpp>
#include <utilities/time/Date.hpp>

namespace openstudio{

// forward declaration
class IdfObject;

/** WeatherStringData is one line from the EPW file. All floating point numbers are stored as strings,
* but are checked as numbers.
*/
class UTILITIES_API WeatherStringData{
public:

private:
  int m_year;
	int m_month;
	int m_day;
	int m_hour;
	int m_minute;
  std::string m_sourceAndUncertainty;
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
	std::string m_globalHorizontalIlluminance; // units lux, missing 999999., note will be missing if >= 999900, minimum 0
  std::string m_directNormalIlluminance; // units lux, missing 999999., will be missing if >= 999900, minimum 0
	std::string m_diffuseHorizontalIlluminance; // units lux, missing 999999., will be missing if >= 999900, minimum 0
	std::string m_zenithLuminance; // units Cd/m2, missing 9999., will be missing if >= 9999, minimum 0
	std::string m_windDirection; // units degrees, missing 999., minimum 0, maximum 360
	std::string m_windSpeed; // units m/s, missing 999., minimum 0, maximum 40
	std::string m_totalSkyCover; // missing 99, minimum 0, maximum 10
	std::string m_opaqueSkyCover; // used if Horizontal IR Intensity missing, missing 99, minimum 0, maximum 10
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
  EpwFile(const openstudio::path& p);

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

private:

  bool parse();
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
  Time m_timeStep;
  DayOfWeek m_startDayOfWeek;
  Date m_startDate;
  Date m_endDate;
  boost::optional<int> m_startDateActualYear;
  boost::optional<int> m_endDateActualYear;
};

UTILITIES_API IdfObject toIdfObject(const EpwFile& epwFile);

} // openstudio

#endif //UTILITIES_FILETYPES_EPWFILE_HPP
