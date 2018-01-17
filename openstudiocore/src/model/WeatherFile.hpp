/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  bool setCity(std::string city);

  bool setStateProvinceRegion(std::string stateProvinceRegion);

  bool setCountry(std::string country);

  bool setDataSource(std::string dataSource);

  bool setWMONumber(std::string wMONumber);

  bool setLatitude(double latitude);

  bool setLongitude(double longitude);

  bool setTimeZone(double timeZone);

  bool setElevation(double elevation);

  void resetElevation();

  //bool setUrl(boost::optional<std::string> url);

  //bool setUrl(std::string url);

  //void resetUrl();

  //bool setChecksum(boost::optional<std::string> checksum);

  //bool setChecksum(std::string checksum);

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
