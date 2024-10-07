/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WEATHERFILE_HPP
#define MODEL_WEATHERFILE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "../utilities/core/Path.hpp"

namespace openstudio {

class EpwFile;
class DayOfWeek;

namespace model {

  class Site;

  namespace detail {

    class WeatherFile_Impl;

  }  // namespace detail

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
  class MODEL_API WeatherFile : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    virtual ~WeatherFile() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    WeatherFile(const WeatherFile& other) = default;
    WeatherFile(WeatherFile&& other) = default;
    WeatherFile& operator=(const WeatherFile&) = default;
    WeatherFile& operator=(WeatherFile&&) = default;

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

    boost::optional<int> startDateActualYear() const;

    boost::optional<DayOfWeek> startDayOfWeek() const;

    //@}
    /** @name Setters */
    //@{

    bool setCity(const std::string& city);

    bool setStateProvinceRegion(const std::string& stateProvinceRegion);

    bool setCountry(const std::string& country);

    bool setDataSource(const std::string& dataSource);

    bool setWMONumber(const std::string& wMONumber);

    bool setLatitude(double latitude);

    bool setLongitude(double longitude);

    bool setTimeZone(double timeZone);

    bool setElevation(double elevation);

    void resetElevation();

    //bool setUrl(boost::optional<std::string> url);

    //bool setUrl(const std::string& url);

    //void resetUrl();

    //bool setChecksum(boost::optional<std::string> checksum);

    //bool setChecksum(const std::string& checksum);

    //void resetChecksum();

    //@}

    // Returns the parent Site.
    boost::optional<Site> site() const;

    /** Load and return the EpwFile.  Optional argument dir can be used as a base path
  if url field is relative. */
    boost::optional<EpwFile> file(const openstudio::path& dir = openstudio::path()) const;

    /** Save the url as relative to basePath, or just keep the file name and extension if
  *  basePath.empty(). */
    bool makeUrlRelative(const openstudio::path& basePath = openstudio::path());

    /** Search for file and save the absolute url if successful. */
    bool makeUrlAbsolute(const openstudio::path& searchDirectory);

    // Returns the environment name used if the RunPeriod object has no name,
    // environmentName = "City StateProvinceRegion Country DataSource WMO#=WMONumber"
    boost::optional<std::string> environmentName() const;

   protected:
    explicit WeatherFile(Model& model);

    /// @cond
    using ImplType = detail::WeatherFile_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit WeatherFile(std::shared_ptr<detail::WeatherFile_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.WeatherFile");
  };

  /** \relates WeatherFile*/
  using OptionalWeatherFile = boost::optional<WeatherFile>;

  /** \relates WeatherFile*/
  using WeatherFileVector = std::vector<WeatherFile>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WEATHERFILE_HPP
