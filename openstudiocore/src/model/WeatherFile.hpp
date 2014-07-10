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

#ifndef MODEL_WEATHERFILE_HPP
#define MODEL_WEATHERFILE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class EpwFile;

namespace model {

class Site;

namespace detail {

  class WeatherFile_Impl;

} // detail

/** WeatherFile derives from ModelObject and is an interface to the OpenStudio IDD object named "OS:WeatherFile".
 *
 *  WeatherFile is a unique object which references an EPW format weather file to use for EnergyPlus simulation.
 *  EnergyPlus requires the weather file for simulation be named in.epw and located in the same directory as the input IDF file.
 *  The WeatherFile object provides a mechanism for an OpenStudio Model to reference a weather file in a more permanent way.
 *  The RunManager is able to locate the actual weather file needed and place it in the EnergyPlus run directory at simulation time.
 *  WeatherFile does not have a public constructor because it is a unique ModelObject.  
 *  To get the WeatherFile object for a Model or create one if it does not yet exist use model.getUniqueObject<WeatherFile>().  
 *  To get the WeatherFile object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<WeatherFile>().
 */
class MODEL_API WeatherFile : public ModelObject {
 public:
  
   /** @name Constructors and Destructors */
  //@{
  virtual ~WeatherFile() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  /// Sets WeatherFile in the Model based on EpwFile input.
  static boost::optional<WeatherFile> setWeatherFile(Model& model, const openstudio::EpwFile& epwFile);

  //@}
  /** @name Getters */
  //@{

  std::string city() const;

  std::string stateProvinceRegion() const;

  std::string country() const;

  std::string dataSource() const;

  std::string wMONumber() const;

  double latitude() const;

  double longitude() const;

  double timeZone() const;

  double elevation() const;

  bool isElevationDefaulted() const;

  boost::optional<std::string> url() const;

  boost::optional<openstudio::path> path() const;

  boost::optional<std::string> checksum() const;

  //@}
  /** @name Setters */
  //@{

  void setCity(std::string city);

  void setStateProvinceRegion(std::string stateProvinceRegion);

  void setCountry(std::string country);

  void setDataSource(std::string dataSource);

  void setWMONumber(std::string wMONumber);

  bool setLatitude(double latitude);

  bool setLongitude(double longitude);

  bool setTimeZone(double timeZone);

  bool setElevation(double elevation);

  void resetElevation();

  //void setUrl(boost::optional<std::string> url);

  //void setUrl(std::string url);

  //void resetUrl();

  //void setChecksum(boost::optional<std::string> checksum);

  //void setChecksum(std::string checksum);

  //void resetChecksum();

  //@}

  // Returns the parent Site.
  boost::optional<Site> site() const;

  /** Load and return the EpwFile.  Optional argument dir can be used as a base path
  if url field is relative. */
  boost::optional<EpwFile> file(const openstudio::path& dir=openstudio::path()) const;

  /** Save the url as relative to basePath, or just keep the file name and extension if 
  *  basePath.empty(). */
  bool makeUrlRelative(const openstudio::path& basePath=openstudio::path());

  /** Search for file and save the absolute url if successful. */
  bool makeUrlAbsolute(const openstudio::path& searchDirectory);

  // Returns the environment name used if the RunPeriod object has no name,
  // environmentName = "City StateProvinceRegion Country DataSource WMO#=WMONumber"
  boost::optional<std::string> environmentName() const;

protected:

  explicit WeatherFile(Model& model);

  /// @cond
  typedef detail::WeatherFile_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit WeatherFile(std::shared_ptr<detail::WeatherFile_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.WeatherFile");
};

/** \relates WeatherFile*/
typedef boost::optional<WeatherFile> OptionalWeatherFile;

/** \relates WeatherFile*/
typedef std::vector<WeatherFile> WeatherFileVector;

} // model
} // openstudio

#endif // MODEL_WEATHERFILE_HPP

