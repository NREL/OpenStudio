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

namespace openstudio{

EpwFile::EpwFile(const openstudio::path& p)
  : m_path(p), m_latitude(0), m_longitude(0), m_timeZone(0), m_elevation(0)
{
  if (!parse()){
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

bool EpwFile::parse()
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
  bool timeReversal = false;
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
            double totalDays = delta.totalDays();
            realYear = false;
            break;
          }else if (delta.totalDays() < 0){
            timeReversal = false;
            break;
          }
        }
        lastDate = date;
      }catch(...){
        realYear = false;
        startDate.reset();
        endDate.reset();
        break;
      }
    }
  }

  // close file
  ifs.close();

  if (timeReversal){
    LOG(Error, "Date reversal detected, wrap around years not supported for TMY data.");
    return false;
  }

  if (realYear && startDate && endDate){
    m_startDayOfWeek = startDate->dayOfWeek();
    m_startDate = startDate.get();
    m_startDateActualYear = startDate->year();

    m_endDate = endDate.get();
    m_endDateActualYear = endDate->year();
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
    std::string timeStep = std::string(matches[2].first, matches[2].second); boost::trim(timeStep);
    std::string startDayOfWeek = std::string(matches[4].first, matches[4].second); boost::trim(startDayOfWeek);
    std::string startDate = std::string(matches[5].first, matches[5].second); boost::trim(startDate);
    std::string endDate = std::string(matches[6].first, matches[6].second); boost::trim(endDate);

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
