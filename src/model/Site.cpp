/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Site.hpp"
#include "Site_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "SimulationControl.hpp"
#include "SimulationControl_Impl.hpp"
#include "Facility.hpp"
#include "Facility_Impl.hpp"

#include "ClimateZones.hpp"
#include "ClimateZones_Impl.hpp"
#include "DesignDay.hpp"
#include "DesignDay_Impl.hpp"
#include "LightingDesignDay.hpp"
#include "LightingDesignDay_Impl.hpp"
#include "ShadingSurfaceGroup.hpp"
#include "ShadingSurfaceGroup_Impl.hpp"
#include "SkyTemperature.hpp"
#include "SkyTemperature_Impl.hpp"
#include "SizingPeriod.hpp"
#include "SizingPeriod_Impl.hpp"
#include "WeatherFile.hpp"
#include "WeatherFile_Impl.hpp"
#include "WeatherFileConditionType.hpp"
#include "WeatherFileConditionType_Impl.hpp"
#include "WeatherFileDays.hpp"
#include "WeatherFileDays_Impl.hpp"
#include "SiteWaterMainsTemperature.hpp"
#include "SiteWaterMainsTemperature_Impl.hpp"
#include "SiteGroundReflectance.hpp"
#include "SiteGroundReflectance_Impl.hpp"
#include "SiteGroundTemperatureBuildingSurface.hpp"
#include "SiteGroundTemperatureBuildingSurface_Impl.hpp"
#include "SiteGroundTemperatureDeep.hpp"
#include "SiteGroundTemperatureDeep_Impl.hpp"
#include "SiteGroundTemperatureShallow.hpp"
#include "SiteGroundTemperatureShallow_Impl.hpp"
#include "SiteGroundTemperatureFCfactorMethod.hpp"
#include "SiteGroundTemperatureFCfactorMethod_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>

#include <utilities/idd/OS_Site_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/ContainersMove.hpp"

#include <algorithm>

namespace openstudio {
namespace model {

  namespace detail {

    Site_Impl::Site_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle) : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == Site::iddObjectType());
    }

    Site_Impl::Site_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == Site::iddObjectType());
    }

    Site_Impl::Site_Impl(const Site_Impl& other, Model_Impl* model, bool keepHandle) : ParentObject_Impl(other, model, keepHandle) {}

    IddObjectType Site_Impl::iddObjectType() const {
      return Site::iddObjectType();
    }

    boost::optional<ParentObject> Site_Impl::parent() const {
      return {};
    }

    std::vector<ModelObject> Site_Impl::children() const {
      std::vector<ModelObject> result;

      // weather file
      OptionalWeatherFile weatherFile = this->weatherFile();
      if (weatherFile) {
        result.push_back(*weatherFile);
      }

      // site ground reflectance
      OptionalSiteGroundReflectance siteGroundReflectance = this->siteGroundReflectance();
      if (siteGroundReflectance) {
        result.push_back(*siteGroundReflectance);
      }

      // site ground temperature building surface
      OptionalSiteGroundTemperatureBuildingSurface siteGroundTemperatureBuildingSurface = this->siteGroundTemperatureBuildingSurface();
      if (siteGroundTemperatureBuildingSurface) {
        result.push_back(*siteGroundTemperatureBuildingSurface);
      }

      // site ground temperature deep
      OptionalSiteGroundTemperatureDeep siteGroundTemperatureDeep = this->siteGroundTemperatureDeep();
      if (siteGroundTemperatureDeep) {
        result.push_back(*siteGroundTemperatureDeep);
      }

      // site ground temperature shallow
      OptionalSiteGroundTemperatureShallow siteGroundTemperatureShallow = this->siteGroundTemperatureShallow();
      if (siteGroundTemperatureShallow) {
        result.push_back(*siteGroundTemperatureShallow);
      }

      // site ground temperature FCfactorMethod
      OptionalSiteGroundTemperatureFCfactorMethod siteGroundTemperatureFCfactorMethod = this->siteGroundTemperatureFCfactorMethod();
      if (siteGroundTemperatureFCfactorMethod) {
        result.push_back(*siteGroundTemperatureFCfactorMethod);
      }

      // site water mains temperature
      OptionalSiteWaterMainsTemperature siteWaterMainsTemperature = this->siteWaterMainsTemperature();
      if (siteWaterMainsTemperature) {
        result.push_back(*siteWaterMainsTemperature);
      }

      // climate zones
      OptionalClimateZones climateZones = this->climateZones();
      if (climateZones) {
        result.push_back(*climateZones);
      }

      // sizing periods
      openstudio::detail::concat_helper(result, model().getModelObjects<SizingPeriod>());

      // lighting design days
      openstudio::detail::concat_helper(result, model().getConcreteModelObjects<LightingDesignDay>());

      // some SkyTemperatures are children (those that do not explicitly point to something else)
      SkyTemperatureVector skyTemperatures = model().getConcreteModelObjects<SkyTemperature>();
      auto siteAsParent = getObject<ParentObject>();
      for (SkyTemperature& st : skyTemperatures) {
        OptionalParentObject opo = st.parent();
        if (opo && (opo.get() == siteAsParent)) {
          // result.emplace_back(st.cast<ModelObject>());
          result.emplace_back(std::move(st));
        }
      }

      // shading surface groups
      openstudio::detail::concat_helper(result, this->shadingSurfaceGroups());

      return result;
    }

    std::vector<IddObjectType> Site_Impl::allowableChildTypes() const {
      return std::vector<IddObjectType>{ClimateZones::iddObjectType(),
                                        DesignDay::iddObjectType(),
                                        SkyTemperature::iddObjectType(),
                                        WeatherFile::iddObjectType(),
                                        WeatherFileConditionType::iddObjectType(),
                                        WeatherFileDays::iddObjectType(),
                                        SiteGroundReflectance::iddObjectType(),
                                        SiteGroundTemperatureBuildingSurface::iddObjectType(),
                                        SiteGroundTemperatureDeep::iddObjectType(),
                                        SiteGroundTemperatureShallow::iddObjectType(),
                                        SiteGroundTemperatureFCfactorMethod::iddObjectType(),
                                        SiteWaterMainsTemperature::iddObjectType(),
                                        ShadingSurfaceGroup::iddObjectType()};
    }

    const std::vector<std::string>& Site_Impl::outputVariableNames() const {
      static const std::vector<std::string> result{"Site Outdoor Air Drybulb Temperature",
                                                   "Site Outdoor Air Wetbulb Temperature",
                                                   "Site Direct Solar Radiation Rate per Area",
                                                   "Site Diffuse Solar Radiation Rate per Area",
                                                   "Site Exterior Beam Normal Illuminance",
                                                   "Site Exterior Horizontal Beam Illuminance",
                                                   "Site Exterior Horizontal Sky Illuminance",
                                                   "Site Beam Solar Radiation Luminous Efficacy",
                                                   "Site Sky Diffuse Solar Radiation Luminous Efficacy",
                                                   "Site Daylighting Model Sky Clearness",
                                                   "Site Daylighting Model Sky Brightness"};
      return result;
    }

    double Site_Impl::latitude() const {
      boost::optional<double> value = getDouble(OS_SiteFields::Latitude, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Site_Impl::isLatitudeDefaulted() const {
      return isEmpty(OS_SiteFields::Latitude);
    }

    double Site_Impl::longitude() const {
      boost::optional<double> value = getDouble(OS_SiteFields::Longitude, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Site_Impl::isLongitudeDefaulted() const {
      return isEmpty(OS_SiteFields::Longitude);
    }

    double Site_Impl::timeZone() const {
      boost::optional<double> value = getDouble(OS_SiteFields::TimeZone, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Site_Impl::isTimeZoneDefaulted() const {
      return isEmpty(OS_SiteFields::TimeZone);
    }

    double Site_Impl::elevation() const {
      boost::optional<double> value = getDouble(OS_SiteFields::Elevation, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Site_Impl::isElevationDefaulted() const {
      return isEmpty(OS_SiteFields::Elevation);
    }

    std::string Site_Impl::terrain() const {
      boost::optional<std::string> value = getString(OS_SiteFields::Terrain, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool Site_Impl::isTerrainDefaulted() const {
      return isEmpty(OS_SiteFields::Terrain);
    }

    bool Site_Impl::keepSiteLocationInformation() const {
      return getBooleanFieldValue(OS_SiteFields::KeepSiteLocationInformation);
    }

    bool Site_Impl::isKeepSiteLocationInformationDefaulted() const {
      return isEmpty(OS_SiteFields::KeepSiteLocationInformation);
    }

    bool Site_Impl::setLatitude(double latitude) {
      bool result = setDouble(OS_SiteFields::Latitude, latitude);
      return result;
    }

    void Site_Impl::resetLatitude() {
      bool result = setString(OS_SiteFields::Latitude, "");
      OS_ASSERT(result);
    }

    bool Site_Impl::setLongitude(double longitude) {
      bool result = setDouble(OS_SiteFields::Longitude, longitude);
      return result;
    }

    void Site_Impl::resetLongitude() {
      bool result = setString(OS_SiteFields::Longitude, "");
      OS_ASSERT(result);
    }

    bool Site_Impl::setTimeZone(double timeZone) {
      bool result = setDouble(OS_SiteFields::TimeZone, timeZone);
      return result;
    }

    void Site_Impl::resetTimeZone() {
      bool result = setString(OS_SiteFields::TimeZone, "");
      OS_ASSERT(result);
    }

    bool Site_Impl::setElevation(double elevation) {
      bool result = setDouble(OS_SiteFields::Elevation, elevation);
      return result;
    }

    void Site_Impl::resetElevation() {
      bool result = setString(OS_SiteFields::Elevation, "");
      OS_ASSERT(result);
    }

    bool Site_Impl::setTerrain(const std::string& terrain) {
      bool result = setString(OS_SiteFields::Terrain, terrain);
      return result;
    }

    void Site_Impl::resetTerrain() {
      bool result = setString(OS_SiteFields::Terrain, "");
      OS_ASSERT(result);
    }

    bool Site_Impl::setKeepSiteLocationInformation(bool keepSiteLocationInformation) {
      return setBooleanFieldValue(OS_SiteFields::KeepSiteLocationInformation, keepSiteLocationInformation);
    }

    void Site_Impl::resetKeepSiteLocationInformation() {
      bool result = setString(OS_SiteFields::KeepSiteLocationInformation, "");
      OS_ASSERT(result);
    }

    boost::optional<WeatherFile> Site_Impl::weatherFile() const {
      return this->model().getOptionalUniqueModelObject<WeatherFile>();
    }

    boost::optional<SiteGroundReflectance> Site_Impl::siteGroundReflectance() const {
      return this->model().getOptionalUniqueModelObject<SiteGroundReflectance>();
    }

    boost::optional<SiteGroundTemperatureBuildingSurface> Site_Impl::siteGroundTemperatureBuildingSurface() const {
      return this->model().getOptionalUniqueModelObject<SiteGroundTemperatureBuildingSurface>();
    }

    boost::optional<SiteGroundTemperatureDeep> Site_Impl::siteGroundTemperatureDeep() const {
      return this->model().getOptionalUniqueModelObject<SiteGroundTemperatureDeep>();
    }

    boost::optional<SiteGroundTemperatureShallow> Site_Impl::siteGroundTemperatureShallow() const {
      return this->model().getOptionalUniqueModelObject<SiteGroundTemperatureShallow>();
    }

    boost::optional<SiteGroundTemperatureFCfactorMethod> Site_Impl::siteGroundTemperatureFCfactorMethod() const {
      return this->model().getOptionalUniqueModelObject<SiteGroundTemperatureFCfactorMethod>();
    }

    boost::optional<SiteWaterMainsTemperature> Site_Impl::siteWaterMainsTemperature() const {
      return this->model().getOptionalUniqueModelObject<SiteWaterMainsTemperature>();
    }

    boost::optional<ClimateZones> Site_Impl::climateZones() const {
      return this->model().getOptionalUniqueModelObject<ClimateZones>();
    }

    ShadingSurfaceGroupVector Site_Impl::shadingSurfaceGroups() const {
      ShadingSurfaceGroupVector result = this->model().getConcreteModelObjects<ShadingSurfaceGroup>();
      result.erase(std::remove_if(result.begin(), result.end(),
                                  [](const auto& shadingGroup) { return !istringEqual(shadingGroup.shadingSurfaceType(), "Site"); }),
                   result.end());
      return result;
    }
    /*
  std::string Site_Impl::activeClimateZoneValue() const {
    std::string result;
    OptionalClimateZones oClimateZones = climateZones();
    if (oClimateZones) {
      result = oClimateZones->activeClimateZoneValue();
    }
    return result;
  }

  std::string Site_Impl::activeClimateZoneInstitution() const {
    std::string result;
    OptionalClimateZones oClimateZones = climateZones();
    if (oClimateZones) {
      ClimateZone cz = oClimateZones->activeClimateZone();
      if (!cz.empty()) { result = cz.institution(); }
    }
    return result;
  }

  bool Site_Impl::setActiveClimateZoneValue(const std::string& value) {
    ClimateZones climateZones = this->model().getUniqueModelObject<ClimateZones>();
    ClimateZone acz = climateZones.activeClimateZone();
    if (!acz.empty()) {
      return acz.setValue(value);
    }
    return false;
  }

  bool Site_Impl::setActiveClimateZoneInstitution(const std::string& institution) {
    ClimateZones climateZones = this->model().getUniqueModelObject<ClimateZones>();
    return !climateZones.setActiveClimateZone(institution).empty();
  }
*/

    std::vector<EMSActuatorNames> Site_Impl::emsActuatorNames() const {
      std::vector<EMSActuatorNames> actuators{
        {"Weather Data", "Outdoor Dry Bulb"}, {"Weather Data", "Outdoor Dew Point"}, {"Weather Data", "Outdoor Relative Humidity"},
        {"Weather Data", "Diffuse Solar"},    {"Weather Data", "Direct Solar"},      {"Weather Data", "Wind Speed"},
        {"Weather Data", "Wind Direction"},
      };
      return actuators;
    }

    std::vector<std::string> Site_Impl::emsInternalVariableNames() const {
      std::vector<std::string> types;
      return types;
    }
  }  // namespace detail

  IddObjectType Site::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Site);
    return result;
  }

  std::vector<std::string> Site::validTerrainValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_SiteFields::Terrain);
  }

  double Site::latitude() const {
    return getImpl<detail::Site_Impl>()->latitude();
  }

  bool Site::isLatitudeDefaulted() const {
    return getImpl<detail::Site_Impl>()->isLatitudeDefaulted();
  }

  double Site::longitude() const {
    return getImpl<detail::Site_Impl>()->longitude();
  }

  bool Site::isLongitudeDefaulted() const {
    return getImpl<detail::Site_Impl>()->isLongitudeDefaulted();
  }

  double Site::timeZone() const {
    return getImpl<detail::Site_Impl>()->timeZone();
  }

  bool Site::isTimeZoneDefaulted() const {
    return getImpl<detail::Site_Impl>()->isTimeZoneDefaulted();
  }

  double Site::elevation() const {
    return getImpl<detail::Site_Impl>()->elevation();
  }

  bool Site::isElevationDefaulted() const {
    return getImpl<detail::Site_Impl>()->isElevationDefaulted();
  }

  std::string Site::terrain() const {
    return getImpl<detail::Site_Impl>()->terrain();
  }

  bool Site::isTerrainDefaulted() const {
    return getImpl<detail::Site_Impl>()->isTerrainDefaulted();
  }

  bool Site::keepSiteLocationInformation() const {
    return getImpl<detail::Site_Impl>()->keepSiteLocationInformation();
  }

  bool Site::isKeepSiteLocationInformationDefaulted() const {
    return getImpl<detail::Site_Impl>()->isKeepSiteLocationInformationDefaulted();
  }

  bool Site::setLatitude(double latitude) {
    return getImpl<detail::Site_Impl>()->setLatitude(latitude);
  }

  void Site::resetLatitude() {
    getImpl<detail::Site_Impl>()->resetLatitude();
  }

  bool Site::setLongitude(double longitude) {
    return getImpl<detail::Site_Impl>()->setLongitude(longitude);
  }

  void Site::resetLongitude() {
    getImpl<detail::Site_Impl>()->resetLongitude();
  }

  bool Site::setTimeZone(double timeZone) {
    return getImpl<detail::Site_Impl>()->setTimeZone(timeZone);
  }

  void Site::resetTimeZone() {
    getImpl<detail::Site_Impl>()->resetTimeZone();
  }

  bool Site::setElevation(double elevation) {
    return getImpl<detail::Site_Impl>()->setElevation(elevation);
  }

  void Site::resetElevation() {
    getImpl<detail::Site_Impl>()->resetElevation();
  }

  bool Site::setTerrain(const std::string& terrain) {
    return getImpl<detail::Site_Impl>()->setTerrain(terrain);
  }

  void Site::resetTerrain() {
    getImpl<detail::Site_Impl>()->resetTerrain();
  }

  bool Site::setKeepSiteLocationInformation(bool keepSiteLocationInformation) {
    return getImpl<detail::Site_Impl>()->setKeepSiteLocationInformation(keepSiteLocationInformation);
  }

  void Site::resetKeepSiteLocationInformation() {
    getImpl<detail::Site_Impl>()->resetKeepSiteLocationInformation();
  }

  boost::optional<WeatherFile> Site::weatherFile() const {
    return getImpl<detail::Site_Impl>()->weatherFile();
  }

  boost::optional<SiteGroundReflectance> Site::siteGroundReflectance() const {
    return getImpl<detail::Site_Impl>()->siteGroundReflectance();
  }

  boost::optional<SiteGroundTemperatureBuildingSurface> Site::siteGroundTemperatureBuildingSurface() const {
    return getImpl<detail::Site_Impl>()->siteGroundTemperatureBuildingSurface();
  }

  boost::optional<SiteGroundTemperatureDeep> Site::siteGroundTemperatureDeep() const {
    return getImpl<detail::Site_Impl>()->siteGroundTemperatureDeep();
  }

  boost::optional<SiteGroundTemperatureShallow> Site::siteGroundTemperatureShallow() const {
    return getImpl<detail::Site_Impl>()->siteGroundTemperatureShallow();
  }

  boost::optional<SiteGroundTemperatureFCfactorMethod> Site::siteGroundTemperatureFCfactorMethod() const {
    return getImpl<detail::Site_Impl>()->siteGroundTemperatureFCfactorMethod();
  }

  boost::optional<SiteWaterMainsTemperature> Site::siteWaterMainsTemperature() const {
    return getImpl<detail::Site_Impl>()->siteWaterMainsTemperature();
  }

  boost::optional<ClimateZones> Site::climateZones() const {
    return getImpl<detail::Site_Impl>()->climateZones();
  }

  ShadingSurfaceGroupVector Site::shadingSurfaceGroups() const {
    return getImpl<detail::Site_Impl>()->shadingSurfaceGroups();
  }
  /*
std::string Site::activeClimateZoneValue() const {
  return getImpl<detail::Site_Impl>()->activeClimateZoneValue();
}

std::string Site::activeClimateZoneInstitution() const {
  return getImpl<detail::Site_Impl>()->activeClimateZoneInstitution();
}

bool Site::setActiveClimateZoneValue(const std::string& value) {
  return getImpl<detail::Site_Impl>()->setActiveClimateZoneValue(value);
}

bool Site::setActiveClimateZoneInstitution(const std::string& institution) {
  return getImpl<detail::Site_Impl>()->setActiveClimateZoneInstitution(institution);
}
*/

  /// @cond
  Site::Site(std::shared_ptr<detail::Site_Impl> impl) : ParentObject(std::move(impl)) {}

  Site::Site(Model& model) : ParentObject(Site::iddObjectType(), model) {}

  /// @endcond

}  // namespace model
}  // namespace openstudio
