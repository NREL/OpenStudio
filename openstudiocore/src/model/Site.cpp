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

#include <model/Site.hpp>
#include <model/Site_Impl.hpp>

#include <model/Model.hpp>
#include <model/Model_Impl.hpp>

#include <model/SimulationControl.hpp>
#include <model/SimulationControl_Impl.hpp>
#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>

#include <model/ClimateZones.hpp>
#include <model/ClimateZones_Impl.hpp>
#include <model/DesignDay.hpp>
#include <model/DesignDay_Impl.hpp>
#include <model/LightingDesignDay.hpp>
#include <model/LightingDesignDay_Impl.hpp>
#include <model/ShadingSurfaceGroup.hpp>
#include <model/ShadingSurfaceGroup_Impl.hpp>
#include <model/SkyTemperature.hpp>
#include <model/SkyTemperature_Impl.hpp>
#include <model/SizingPeriod.hpp>
#include <model/SizingPeriod_Impl.hpp>
#include <model/TimeDependentValuation.hpp>
#include <model/TimeDependentValuation_Impl.hpp>
#include <model/WeatherFile.hpp>
#include <model/WeatherFile_Impl.hpp>
#include <model/WeatherFileConditionType.hpp>
#include <model/WeatherFileConditionType_Impl.hpp>
#include <model/WeatherFileDays.hpp>
#include <model/WeatherFileDays_Impl.hpp>
#include <model/SiteWaterMainsTemperature.hpp>
#include <model/SiteWaterMainsTemperature_Impl.hpp>
#include <model/SiteGroundReflectance.hpp>
#include <model/SiteGroundReflectance_Impl.hpp>
#include <model/SiteGroundTemperatureBuildingSurface.hpp>
#include <model/SiteGroundTemperatureBuildingSurface_Impl.hpp>

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/OS_Site_FieldEnums.hxx>

#include <utilities/core/Assert.hpp>

#include <boost/foreach.hpp>

namespace openstudio {
namespace model {

namespace detail {

  Site_Impl::Site_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ParentObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == Site::iddObjectType());
  }

  Site_Impl::Site_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                       Model_Impl* model,
                       bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == Site::iddObjectType());
  }

  Site_Impl::Site_Impl(const Site_Impl& other,
                       Model_Impl* model,
                       bool keepHandle)
    : ParentObject_Impl(other,model,keepHandle)
  {}

  IddObjectType Site_Impl::iddObjectType() const {
    return Site::iddObjectType();
  }

  boost::optional<ParentObject> Site_Impl::parent() const
  {
    return boost::optional<ParentObject>();
  }

  std::vector<ModelObject> Site_Impl::children() const
  {
    std::vector<ModelObject> result;

    // weather file
    OptionalWeatherFile weatherFile = this->weatherFile();
    if (weatherFile){ result.push_back(*weatherFile); }

    // site ground reflectance
    OptionalSiteGroundReflectance siteGroundReflectance = this->siteGroundReflectance();
    if (siteGroundReflectance){ result.push_back(*siteGroundReflectance); }

    // site ground temperature building surface
    OptionalSiteGroundTemperatureBuildingSurface siteGroundTemperatureBuildingSurface = this->siteGroundTemperatureBuildingSurface();
    if (siteGroundTemperatureBuildingSurface){ result.push_back(*siteGroundTemperatureBuildingSurface); }

    // site water mains temperature
    OptionalSiteWaterMainsTemperature siteWaterMainsTemperature = this->siteWaterMainsTemperature();
    if (siteWaterMainsTemperature){ result.push_back(*siteWaterMainsTemperature); }

    // climate zones
    OptionalClimateZones climateZones = this->climateZones();
    if (climateZones) { result.push_back(*climateZones); }

    // sizing periods
    SizingPeriodVector sizingPeriods = model().getModelObjects<SizingPeriod>();
    result.insert(result.end(),sizingPeriods.begin(),sizingPeriods.end());

    // lighting design days
    LightingDesignDayVector lightingDesignDays = model().getModelObjects<LightingDesignDay>();
    result.insert(result.end(),lightingDesignDays.begin(),lightingDesignDays.end());

    // time dependent valuation
    OptionalTimeDependentValuation tdv = this->timeDependentValuation();
    if (tdv) { result.push_back(*tdv); }

    // some SkyTemperatures are children (those that do not explicitly point to something else)
    SkyTemperatureVector skyTemperatures = model().getModelObjects<SkyTemperature>();
    ParentObject siteAsParent = getObject<ParentObject>();
    BOOST_FOREACH(const SkyTemperature& st,skyTemperatures) {
      OptionalParentObject opo = st.parent();
      if (opo && (opo.get() == siteAsParent)) { result.push_back(st.cast<ModelObject>()); }
    }

    // shading surface groups
    std::vector<ShadingSurfaceGroup> shadingSurfaceGroups = this->shadingSurfaceGroups();
    result.insert(result.end(),shadingSurfaceGroups.begin(),shadingSurfaceGroups.end());

    return result;
  }

  std::vector<IddObjectType> Site_Impl::allowableChildTypes() const
  {
    std::vector<IddObjectType> result;
    result.push_back(ClimateZones::iddObjectType());
    result.push_back(DesignDay::iddObjectType());
    result.push_back(SkyTemperature::iddObjectType());
    result.push_back(TimeDependentValuation::iddObjectType());
    result.push_back(WeatherFile::iddObjectType());
    result.push_back(WeatherFileConditionType::iddObjectType());
    result.push_back(WeatherFileDays::iddObjectType());
    result.push_back(SiteGroundReflectance::iddObjectType());
    result.push_back(SiteGroundTemperatureBuildingSurface::iddObjectType());
    result.push_back(SiteWaterMainsTemperature::iddObjectType());
    result.push_back(ShadingSurfaceGroup::iddObjectType());
    return result;
  }

  const std::vector<std::string>& Site_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  double Site_Impl::latitude() const {
    boost::optional<double> value = getDouble(OS_SiteFields::Latitude,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Site_Impl::isLatitudeDefaulted() const {
    return isEmpty(OS_SiteFields::Latitude);
  }

  double Site_Impl::longitude() const {
    boost::optional<double> value = getDouble(OS_SiteFields::Longitude,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Site_Impl::isLongitudeDefaulted() const {
    return isEmpty(OS_SiteFields::Longitude);
  }

  double Site_Impl::timeZone() const {
    boost::optional<double> value = getDouble(OS_SiteFields::TimeZone,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Site_Impl::isTimeZoneDefaulted() const {
    return isEmpty(OS_SiteFields::TimeZone);
  }

  double Site_Impl::elevation() const {
    boost::optional<double> value = getDouble(OS_SiteFields::Elevation,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Site_Impl::isElevationDefaulted() const {
    return isEmpty(OS_SiteFields::Elevation);
  }

  std::string Site_Impl::terrain() const {
    boost::optional<std::string> value = getString(OS_SiteFields::Terrain,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool Site_Impl::isTerrainDefaulted() const {
    return isEmpty(OS_SiteFields::Terrain);
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

  bool Site_Impl::setTerrain(std::string terrain) {
    bool result = setString(OS_SiteFields::Terrain, terrain);
    return result;
  }

  void Site_Impl::resetTerrain() {
    bool result = setString(OS_SiteFields::Terrain, "");
    OS_ASSERT(result);
  }

  boost::optional<WeatherFile> Site_Impl::weatherFile() const
  {
    return this->model().getOptionalUniqueModelObject<WeatherFile>();
  }

  boost::optional<SiteGroundReflectance> Site_Impl::siteGroundReflectance() const
  {
    return this->model().getOptionalUniqueModelObject<SiteGroundReflectance>();
  }

  boost::optional<SiteGroundTemperatureBuildingSurface> Site_Impl::siteGroundTemperatureBuildingSurface() const
  {
    return this->model().getOptionalUniqueModelObject<SiteGroundTemperatureBuildingSurface>();
  }

  boost::optional<SiteWaterMainsTemperature> Site_Impl::siteWaterMainsTemperature() const
  {
    return this->model().getOptionalUniqueModelObject<SiteWaterMainsTemperature>();
  } 

  boost::optional<ClimateZones> Site_Impl::climateZones() const {
    return this->model().getOptionalUniqueModelObject<ClimateZones>();
  }

  boost::optional<TimeDependentValuation> Site_Impl::timeDependentValuation() const {
    return this->model().getOptionalUniqueModelObject<TimeDependentValuation>();
  }

  ShadingSurfaceGroupVector Site_Impl::shadingSurfaceGroups() const
  {
    ShadingSurfaceGroupVector result;
    BOOST_FOREACH(ShadingSurfaceGroup shadingGroup, this->model().getModelObjects<ShadingSurfaceGroup>()){
      if (istringEqual(shadingGroup.shadingSurfaceType(), "Site")){
        result.push_back(shadingGroup);
      }
    }
    return result;
  }

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

} // detail

IddObjectType Site::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Site);
  return result;
}

std::vector<std::string> Site::validTerrainValues() {
  return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(),
                        OS_SiteFields::Terrain);
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

bool Site::setTerrain(std::string terrain) {
  return getImpl<detail::Site_Impl>()->setTerrain(terrain);
}

void Site::resetTerrain() {
  getImpl<detail::Site_Impl>()->resetTerrain();
}

boost::optional<WeatherFile> Site::weatherFile() const
{
  return getImpl<detail::Site_Impl>()->weatherFile();
}

boost::optional<SiteGroundReflectance> Site::siteGroundReflectance() const
{
  return getImpl<detail::Site_Impl>()->siteGroundReflectance();
}

boost::optional<SiteGroundTemperatureBuildingSurface> Site::siteGroundTemperatureBuildingSurface() const
{
  return getImpl<detail::Site_Impl>()->siteGroundTemperatureBuildingSurface();
}

boost::optional<SiteWaterMainsTemperature> Site::siteWaterMainsTemperature() const
{
  return getImpl<detail::Site_Impl>()->siteWaterMainsTemperature();
} 

boost::optional<ClimateZones> Site::climateZones() const {
  return getImpl<detail::Site_Impl>()->climateZones();
}

boost::optional<TimeDependentValuation> Site::timeDependentValuation() const {
  return getImpl<detail::Site_Impl>()->timeDependentValuation();
}

ShadingSurfaceGroupVector Site::shadingSurfaceGroups() const
{
  return getImpl<detail::Site_Impl>()->shadingSurfaceGroups();
}

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

/// @cond
Site::Site(boost::shared_ptr<detail::Site_Impl> impl)
  : ParentObject(impl)
{}

Site::Site(Model& model)
  : ParentObject(Site::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio

