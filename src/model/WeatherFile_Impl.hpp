/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WEATHERFILE_IMPL_HPP
#define MODEL_WEATHERFILE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

#include "../utilities/core/Path.hpp"

namespace openstudio {

class EpwFile;
class DayOfWeek;

namespace model {

  class Site;
  class WeatherFile;

  namespace detail {

    /** WeatherFile_Impl is a ModelObject_Impl that is the implementation class for WeatherFile.*/
    class MODEL_API WeatherFile_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      WeatherFile_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WeatherFile_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WeatherFile_Impl(const WeatherFile_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~WeatherFile_Impl() override = default;

      //@}

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

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

      // get the site
      boost::optional<Site> site() const;

      /** Load and return the EpwFile, if it can be located. */
      boost::optional<EpwFile> file(const openstudio::path& dir = openstudio::path()) const;

      /** Save the url as relative to basePath, or just keep the file name and extension if
    *  basePath.empty(). */
      bool makeUrlRelative(const openstudio::path& basePath = openstudio::path());

      /** Search for file and save the absolute url if successful. */
      bool makeUrlAbsolute(const openstudio::path& searchDirectory);

      // get environment name as reported in the sql file
      // environment name = "City StateProvinceRegion Country DataSource WMO#=WMONumber"
      boost::optional<std::string> environmentName() const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.WeatherFile");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WEATHERFILE_IMPL_HPP
