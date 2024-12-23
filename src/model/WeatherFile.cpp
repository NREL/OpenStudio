/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "WeatherFile.hpp"
#include "WeatherFile_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_WeatherFile_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/filetypes/EpwFile.hpp"
#include "../utilities/time/Date.hpp"
#include "../utilities/core/Assert.hpp"
#include "../utilities/core/PathHelpers.hpp"
#include "../utilities/core/String.hpp"

#include <boost/algorithm/string/replace.hpp>

namespace openstudio {
namespace model {

  namespace detail {

    WeatherFile_Impl::WeatherFile_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == WeatherFile::iddObjectType());
    }

    WeatherFile_Impl::WeatherFile_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == WeatherFile::iddObjectType());
    }

    WeatherFile_Impl::WeatherFile_Impl(const WeatherFile_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    boost::optional<ParentObject> WeatherFile_Impl::parent() const {
      boost::optional<ParentObject> result(this->site());
      return result;
    }

    bool WeatherFile_Impl::setParent(ParentObject& newParent) {
      if (newParent.optionalCast<Site>() && (newParent.model() == model())) {
        return true;
      }
      return false;
    }

    const std::vector<std::string>& WeatherFile_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType WeatherFile_Impl::iddObjectType() const {
      return WeatherFile::iddObjectType();
    }

    std::string WeatherFile_Impl::city() const {
      boost::optional<std::string> value = getString(OS_WeatherFileFields::City, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string WeatherFile_Impl::stateProvinceRegion() const {
      boost::optional<std::string> value = getString(OS_WeatherFileFields::StateProvinceRegion, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string WeatherFile_Impl::country() const {
      boost::optional<std::string> value = getString(OS_WeatherFileFields::Country, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string WeatherFile_Impl::dataSource() const {
      boost::optional<std::string> value = getString(OS_WeatherFileFields::DataSource, true);
      OS_ASSERT(value);
      return value.get();
    }

    std::string WeatherFile_Impl::wMONumber() const {
      boost::optional<std::string> value = getString(OS_WeatherFileFields::WMONumber, true);
      OS_ASSERT(value);
      return value.get();
    }

    double WeatherFile_Impl::latitude() const {
      boost::optional<double> value = getDouble(OS_WeatherFileFields::Latitude, true);
      OS_ASSERT(value);
      return value.get();
    }

    double WeatherFile_Impl::longitude() const {
      boost::optional<double> value = getDouble(OS_WeatherFileFields::Longitude, true);
      OS_ASSERT(value);
      return value.get();
    }

    double WeatherFile_Impl::timeZone() const {
      boost::optional<double> value = getDouble(OS_WeatherFileFields::TimeZone, true);
      OS_ASSERT(value);
      return value.get();
    }

    double WeatherFile_Impl::elevation() const {
      boost::optional<double> value = getDouble(OS_WeatherFileFields::Elevation, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool WeatherFile_Impl::isElevationDefaulted() const {
      return isEmpty(OS_WeatherFileFields::Elevation);
    }

    boost::optional<std::string> WeatherFile_Impl::url() const {
      return getString(OS_WeatherFileFields::Url, false, true);
    }

    boost::optional<openstudio::path> WeatherFile_Impl::path() const {
      boost::optional<openstudio::path> result;
      OptionalString oUrlStr = this->url();
      if (oUrlStr) {
        result = toPath(*oUrlStr);
      }
      return result;
    }

    boost::optional<std::string> WeatherFile_Impl::checksum() const {
      return getString(OS_WeatherFileFields::Checksum, true);
    }

    bool WeatherFile_Impl::setCity(const std::string& city) {
      bool result = setString(OS_WeatherFileFields::City, city);
      OS_ASSERT(result);
      return result;
    }

    bool WeatherFile_Impl::setStateProvinceRegion(const std::string& stateProvinceRegion) {
      bool result = setString(OS_WeatherFileFields::StateProvinceRegion, stateProvinceRegion);
      OS_ASSERT(result);
      return result;
    }

    bool WeatherFile_Impl::setCountry(const std::string& country) {
      bool result = setString(OS_WeatherFileFields::Country, country);
      OS_ASSERT(result);
      return result;
    }

    bool WeatherFile_Impl::setDataSource(const std::string& dataSource) {
      bool result = setString(OS_WeatherFileFields::DataSource, dataSource);
      OS_ASSERT(result);
      return result;
    }

    bool WeatherFile_Impl::setWMONumber(const std::string& wMONumber) {
      bool result = setString(OS_WeatherFileFields::WMONumber, wMONumber);
      OS_ASSERT(result);
      return result;
    }

    bool WeatherFile_Impl::setLatitude(double latitude) {
      bool result = setDouble(OS_WeatherFileFields::Latitude, latitude);
      return result;
    }

    bool WeatherFile_Impl::setLongitude(double longitude) {
      bool result = setDouble(OS_WeatherFileFields::Longitude, longitude);
      return result;
    }

    bool WeatherFile_Impl::setTimeZone(double timeZone) {
      bool result = setDouble(OS_WeatherFileFields::TimeZone, timeZone);
      return result;
    }

    bool WeatherFile_Impl::setElevation(double elevation) {
      bool result = setDouble(OS_WeatherFileFields::Elevation, elevation);
      return result;
    }

    void WeatherFile_Impl::resetElevation() {
      bool result = setString(OS_WeatherFileFields::Elevation, "");
      OS_ASSERT(result);
    }
    /*
  bool WeatherFile_Impl::setUrl(boost::optional<std::string> url) {
    bool result = false;
    if (url) {
      result = setString(OS_WeatherFileFields::Url, url.get());
    } else {
      result = setString(OS_WeatherFileFields::Url, "");
    }
    OS_ASSERT(result);
    return result;
  }

  bool WeatherFile_Impl::setUrl(const std::string& url) {
    bool result = setString(OS_WeatherFileFields::Url, url);
    OS_ASSERT(result);
    return result;
  }

  void WeatherFile_Impl::resetUrl() {
    bool result = setString(OS_WeatherFileFields::Url, "");
    OS_ASSERT(result);
  }

  bool WeatherFile_Impl::setChecksum(boost::optional<std::string> checksum) {
    bool result = false;
    if (checksum) {
      result = setString(OS_WeatherFileFields::Checksum, checksum.get());
    } else {
      result = setString(OS_WeatherFileFields::Checksum, "");
    }
    OS_ASSERT(result);
    return result;
  }

  bool WeatherFile_Impl::setChecksum(const std::string& checksum) {
    bool result = setString(OS_WeatherFileFields::Checksum, checksum);
    OS_ASSERT(result);
    return result;
  }

  void WeatherFile_Impl::resetChecksum() {
    bool result = setString(OS_WeatherFileFields::Checksum, "");
    OS_ASSERT(result);
  }
*/
    boost::optional<Site> WeatherFile_Impl::site() const {
      return this->model().getOptionalUniqueModelObject<Site>();
    }

    boost::optional<EpwFile> WeatherFile_Impl::file(const openstudio::path& dir) const {
      boost::optional<EpwFile> result;

      // get current path
      boost::optional<openstudio::path> currentPath = this->path();
      if (currentPath) {

        // try to load absolute path
        if (currentPath->is_complete() && openstudio::filesystem::exists(*currentPath)) {
          try {
            result = EpwFile(*currentPath);
            return result;
          } catch (...) {
          }

          // loading absolute path failed, try as relative path
          currentPath = currentPath->filename();
        }

        // try relative path
        if (!dir.empty()) {
          openstudio::path newPath = openstudio::filesystem::complete(*currentPath, dir);
          if (openstudio::filesystem::exists(newPath)) {
            try {
              result = EpwFile(newPath);
              return result;
            } catch (...) {
            }
          }
        }
      }

      return boost::none;
    }

    bool WeatherFile_Impl::makeUrlRelative(const openstudio::path& basePath) {
      boost::optional<openstudio::path> currentPath = this->path();
      if (currentPath) {
        openstudio::path newPath;
        if (basePath.empty()) {
          newPath = currentPath->filename();
        } else {
          newPath = relativePath(*currentPath, basePath);
        }
        if (!newPath.empty()) {
          std::string weatherFileUrl = toString(newPath);
          LOG(Debug, "Setting weather file url to " << weatherFileUrl);
          return setString(OS_WeatherFileFields::Url, weatherFileUrl);
        }
      }
      return false;
    }

    bool WeatherFile_Impl::makeUrlAbsolute(const openstudio::path& searchDirectory) {
      boost::optional<openstudio::path> currentPath = this->path();
      if (currentPath) {
        if (currentPath->is_complete() && openstudio::filesystem::exists(*currentPath)) {
          return true;
        }
        openstudio::path newPath;
        openstudio::path workingPath(*currentPath);
        if (!currentPath->is_complete()) {
          newPath = openstudio::filesystem::system_complete(workingPath);
          LOG(Debug, "Current path '" << toString(*currentPath) << "' not complete. " << "After calling system_complete have '" << toString(newPath)
                                      << "'.");
        }
        if (newPath.empty() || !openstudio::filesystem::exists(newPath)) {
          newPath = searchDirectory / *currentPath;
          LOG(Debug, "Going to look for '" << toString(newPath) << "'.");
        }
        if (newPath.empty() || !openstudio::filesystem::exists(newPath)) {
          workingPath = currentPath->filename();
          newPath = searchDirectory / workingPath;
          LOG(Debug, "Going to look for '" << toString(newPath) << "'.");
        }
        if (newPath.empty() || !openstudio::filesystem::exists(newPath)) {
          return false;
        }
        std::string weatherFileUrl = toString(newPath);
        LOG(Debug, "Setting weather file url to " << weatherFileUrl);
        return setString(OS_WeatherFileFields::Url, weatherFileUrl);
      }
      return false;
    }

    boost::optional<std::string> WeatherFile_Impl::environmentName() const {
      OptionalString result;

      OptionalString city = getString(OS_WeatherFileFields::City, true);
      OptionalString state = getString(OS_WeatherFileFields::StateProvinceRegion, true);
      OptionalString country = getString(OS_WeatherFileFields::Country, true);
      OptionalString dataSource = getString(OS_WeatherFileFields::DataSource, true);
      OptionalString wmoNumber = getString(OS_WeatherFileFields::WMONumber, true);

      if (city && state && country && dataSource && wmoNumber) {
        result = *city + " " + *state + " " + *country + " " + *dataSource + " WMO#=" + *wmoNumber;
      }

      return result;
    }

    std::vector<EMSActuatorNames> WeatherFile_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{
        {"Weather Data", "Outdoor Dry Bulb"}, {"Weather Data", "Outdoor Dew Point"}, {"Weather Data", "Outdoor Relative Humidity"},
        {"Weather Data", "Diffuse Solar"},    {"Weather Data", "Direct Solar"},      {"Weather Data", "Wind Speed"},
        {"Weather Data", "Wind Direction"}};
      return actuators;
    }

    std::vector<std::string> WeatherFile_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }

    boost::optional<int> WeatherFile_Impl::startDateActualYear() const {
      boost::optional<int> result = getInt(OS_WeatherFileFields::StartDateActualYear, true);
      return result;
    }

    boost::optional<DayOfWeek> WeatherFile_Impl::startDayOfWeek() const {
      boost::optional<std::string> result = getString(OS_WeatherFileFields::StartDayofWeek, false, true);
      if (!result) {
        return boost::none;
      }
      return openstudio::DayOfWeek{*result};
    }

  }  // namespace detail

  IddObjectType WeatherFile::iddObjectType() {
    IddObjectType result(IddObjectType::OS_WeatherFile);
    return result;
  }

  boost::optional<WeatherFile> WeatherFile::setWeatherFile(Model& model, const openstudio::EpwFile& epwFile) {
    WeatherFile weatherFile = model.getUniqueModelObject<WeatherFile>();
    weatherFile.setString(OS_WeatherFileFields::City, epwFile.city());
    weatherFile.setString(OS_WeatherFileFields::StateProvinceRegion, epwFile.stateProvinceRegion());
    weatherFile.setString(OS_WeatherFileFields::Country, epwFile.country());
    weatherFile.setString(OS_WeatherFileFields::DataSource, epwFile.dataSource());
    weatherFile.setString(OS_WeatherFileFields::WMONumber, epwFile.wmoNumber());
    weatherFile.setDouble(OS_WeatherFileFields::Latitude, epwFile.latitude());
    weatherFile.setDouble(OS_WeatherFileFields::Longitude, epwFile.longitude());
    weatherFile.setDouble(OS_WeatherFileFields::TimeZone, epwFile.timeZone());
    weatherFile.setDouble(OS_WeatherFileFields::Elevation, epwFile.elevation());
    weatherFile.setString(OS_WeatherFileFields::Url, toString(epwFile.path()));
    weatherFile.setString(OS_WeatherFileFields::Checksum, epwFile.checksum());
    if (auto s_ = epwFile.startDateActualYear()) {
      weatherFile.setInt(OS_WeatherFileFields::StartDateActualYear, s_.get());
    }

    weatherFile.setString(OS_WeatherFileFields::StartDayofWeek, epwFile.startDayOfWeek().valueName());

    return weatherFile;
  }

  std::string WeatherFile::city() const {
    return getImpl<detail::WeatherFile_Impl>()->city();
  }

  std::string WeatherFile::stateProvinceRegion() const {
    return getImpl<detail::WeatherFile_Impl>()->stateProvinceRegion();
  }

  std::string WeatherFile::country() const {
    return getImpl<detail::WeatherFile_Impl>()->country();
  }

  std::string WeatherFile::dataSource() const {
    return getImpl<detail::WeatherFile_Impl>()->dataSource();
  }

  std::string WeatherFile::wMONumber() const {
    return getImpl<detail::WeatherFile_Impl>()->wMONumber();
  }

  double WeatherFile::latitude() const {
    return getImpl<detail::WeatherFile_Impl>()->latitude();
  }

  double WeatherFile::longitude() const {
    return getImpl<detail::WeatherFile_Impl>()->longitude();
  }

  double WeatherFile::timeZone() const {
    return getImpl<detail::WeatherFile_Impl>()->timeZone();
  }

  double WeatherFile::elevation() const {
    return getImpl<detail::WeatherFile_Impl>()->elevation();
  }

  bool WeatherFile::isElevationDefaulted() const {
    return getImpl<detail::WeatherFile_Impl>()->isElevationDefaulted();
  }

  boost::optional<std::string> WeatherFile::url() const {
    return getImpl<detail::WeatherFile_Impl>()->url();
  }

  boost::optional<openstudio::path> WeatherFile::path() const {
    return getImpl<detail::WeatherFile_Impl>()->path();
  }

  boost::optional<std::string> WeatherFile::checksum() const {
    return getImpl<detail::WeatherFile_Impl>()->checksum();
  }

  bool WeatherFile::setCity(const std::string& city) {
    return getImpl<detail::WeatherFile_Impl>()->setCity(city);
  }

  bool WeatherFile::setStateProvinceRegion(const std::string& stateProvinceRegion) {
    return getImpl<detail::WeatherFile_Impl>()->setStateProvinceRegion(stateProvinceRegion);
  }

  bool WeatherFile::setCountry(const std::string& country) {
    return getImpl<detail::WeatherFile_Impl>()->setCountry(country);
  }

  bool WeatherFile::setDataSource(const std::string& dataSource) {
    return getImpl<detail::WeatherFile_Impl>()->setDataSource(dataSource);
  }

  bool WeatherFile::setWMONumber(const std::string& wMONumber) {
    return getImpl<detail::WeatherFile_Impl>()->setWMONumber(wMONumber);
  }

  bool WeatherFile::setLatitude(double latitude) {
    return getImpl<detail::WeatherFile_Impl>()->setLatitude(latitude);
  }

  bool WeatherFile::setLongitude(double longitude) {
    return getImpl<detail::WeatherFile_Impl>()->setLongitude(longitude);
  }

  bool WeatherFile::setTimeZone(double timeZone) {
    return getImpl<detail::WeatherFile_Impl>()->setTimeZone(timeZone);
  }

  bool WeatherFile::setElevation(double elevation) {
    return getImpl<detail::WeatherFile_Impl>()->setElevation(elevation);
  }

  void WeatherFile::resetElevation() {
    getImpl<detail::WeatherFile_Impl>()->resetElevation();
  }
  /*
bool WeatherFile::setUrl(boost::optional<std::string> url) {
  return getImpl<detail::WeatherFile_Impl>()->setUrl(url);
}

bool WeatherFile::setUrl(const std::string& url) {
  return getImpl<detail::WeatherFile_Impl>()->setUrl(url);
}

void WeatherFile::resetUrl() {
  getImpl<detail::WeatherFile_Impl>()->resetUrl();
}

bool WeatherFile::setChecksum(boost::optional<std::string> checksum) {
  return getImpl<detail::WeatherFile_Impl>()->setChecksum(checksum);
}

bool WeatherFile::setChecksum(const std::string& checksum) {
  return getImpl<detail::WeatherFile_Impl>()->setChecksum(checksum);
}

void WeatherFile::resetChecksum() {
  getImpl<detail::WeatherFile_Impl>()->resetChecksum();
}
*/

  boost::optional<Site> WeatherFile::site() const {
    return getImpl<detail::WeatherFile_Impl>()->site();
  }

  boost::optional<EpwFile> WeatherFile::file(const openstudio::path& dir) const {
    return getImpl<detail::WeatherFile_Impl>()->file(dir);
  }

  bool WeatherFile::makeUrlRelative(const openstudio::path& basePath) {
    return getImpl<detail::WeatherFile_Impl>()->makeUrlRelative(basePath);
  }

  bool WeatherFile::makeUrlAbsolute(const openstudio::path& searchDirectory) {
    return getImpl<detail::WeatherFile_Impl>()->makeUrlAbsolute(searchDirectory);
  }

  boost::optional<std::string> WeatherFile::environmentName() const {
    return getImpl<detail::WeatherFile_Impl>()->environmentName();
  }

  boost::optional<int> WeatherFile::startDateActualYear() const {
    return getImpl<detail::WeatherFile_Impl>()->startDateActualYear();
  }

  boost::optional<DayOfWeek> WeatherFile::startDayOfWeek() const {
    return getImpl<detail::WeatherFile_Impl>()->startDayOfWeek();
  }

  /// @cond
  WeatherFile::WeatherFile(std::shared_ptr<detail::WeatherFile_Impl> impl) : ModelObject(std::move(impl)) {}
  WeatherFile::WeatherFile(Model& model) : ModelObject(WeatherFile::iddObjectType(), model) {}

  /// @endcond

}  // namespace model
}  // namespace openstudio
