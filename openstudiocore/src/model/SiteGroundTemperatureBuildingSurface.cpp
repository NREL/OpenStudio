/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "SiteGroundTemperatureBuildingSurface.hpp"
#include "SiteGroundTemperatureBuildingSurface_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_Site_GroundTemperature_BuildingSurface_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SiteGroundTemperatureBuildingSurface_Impl::SiteGroundTemperatureBuildingSurface_Impl(
      const IdfObject& idfObject, 
      Model_Impl* model, 
      bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SiteGroundTemperatureBuildingSurface::iddObjectType());
  }

  SiteGroundTemperatureBuildingSurface_Impl::SiteGroundTemperatureBuildingSurface_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SiteGroundTemperatureBuildingSurface::iddObjectType());
  }

  SiteGroundTemperatureBuildingSurface_Impl::SiteGroundTemperatureBuildingSurface_Impl(const SiteGroundTemperatureBuildingSurface_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> SiteGroundTemperatureBuildingSurface_Impl::parent() const
  {
    boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
    return boost::optional<ParentObject>(result);
  }

  const std::vector<std::string>& SiteGroundTemperatureBuildingSurface_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SiteGroundTemperatureBuildingSurface_Impl::iddObjectType() const {
    return SiteGroundTemperatureBuildingSurface::iddObjectType();
  }

  double SiteGroundTemperatureBuildingSurface_Impl::januaryGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::JanuaryGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isJanuaryGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::JanuaryGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::februaryGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::FebruaryGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isFebruaryGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::FebruaryGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::marchGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::MarchGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isMarchGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::MarchGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::aprilGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::AprilGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isAprilGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::AprilGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::mayGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::MayGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isMayGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::MayGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::juneGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::JuneGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isJuneGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::JuneGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::julyGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::JulyGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isJulyGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::JulyGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::augustGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::AugustGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isAugustGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::AugustGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::septemberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::SeptemberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isSeptemberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::SeptemberGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::octoberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::OctoberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isOctoberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::OctoberGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::novemberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::NovemberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isNovemberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::NovemberGroundTemperature);
  }

  double SiteGroundTemperatureBuildingSurface_Impl::decemberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::DecemberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isDecemberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_BuildingSurfaceFields::DecemberGroundTemperature);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setJanuaryGroundTemperature(double januaryGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::JanuaryGroundTemperature, januaryGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetJanuaryGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::JanuaryGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setFebruaryGroundTemperature(double februaryGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::FebruaryGroundTemperature, februaryGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetFebruaryGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::FebruaryGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setMarchGroundTemperature(double marchGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::MarchGroundTemperature, marchGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetMarchGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::MarchGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setAprilGroundTemperature(double aprilGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::AprilGroundTemperature, aprilGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetAprilGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::AprilGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setMayGroundTemperature(double mayGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::MayGroundTemperature, mayGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetMayGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::MayGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setJuneGroundTemperature(double juneGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::JuneGroundTemperature, juneGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetJuneGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::JuneGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setJulyGroundTemperature(double julyGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::JulyGroundTemperature, julyGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetJulyGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::JulyGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setAugustGroundTemperature(double augustGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::AugustGroundTemperature, augustGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetAugustGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::AugustGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setSeptemberGroundTemperature(double septemberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::SeptemberGroundTemperature, septemberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetSeptemberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::SeptemberGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setOctoberGroundTemperature(double octoberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::OctoberGroundTemperature, octoberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetOctoberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::OctoberGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setNovemberGroundTemperature(double novemberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::NovemberGroundTemperature, novemberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetNovemberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::NovemberGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setDecemberGroundTemperature(double decemberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_BuildingSurfaceFields::DecemberGroundTemperature, decemberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetDecemberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_BuildingSurfaceFields::DecemberGroundTemperature, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType SiteGroundTemperatureBuildingSurface::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Site_GroundTemperature_BuildingSurface);
  return result;
}

double SiteGroundTemperatureBuildingSurface::januaryGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->januaryGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isJanuaryGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isJanuaryGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::februaryGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->februaryGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isFebruaryGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isFebruaryGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::marchGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->marchGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isMarchGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isMarchGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::aprilGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->aprilGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isAprilGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isAprilGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::mayGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->mayGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isMayGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isMayGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::juneGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->juneGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isJuneGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isJuneGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::julyGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->julyGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isJulyGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isJulyGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::augustGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->augustGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isAugustGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isAugustGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::septemberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->septemberGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isSeptemberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isSeptemberGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::octoberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->octoberGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isOctoberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isOctoberGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::novemberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->novemberGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isNovemberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isNovemberGroundTemperatureDefaulted();
}

double SiteGroundTemperatureBuildingSurface::decemberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->decemberGroundTemperature();
}

bool SiteGroundTemperatureBuildingSurface::isDecemberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isDecemberGroundTemperatureDefaulted();
}

void SiteGroundTemperatureBuildingSurface::setJanuaryGroundTemperature(double januaryGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setJanuaryGroundTemperature(januaryGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetJanuaryGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetJanuaryGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setFebruaryGroundTemperature(double februaryGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setFebruaryGroundTemperature(februaryGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetFebruaryGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetFebruaryGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setMarchGroundTemperature(double marchGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setMarchGroundTemperature(marchGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetMarchGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetMarchGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setAprilGroundTemperature(double aprilGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setAprilGroundTemperature(aprilGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetAprilGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetAprilGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setMayGroundTemperature(double mayGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setMayGroundTemperature(mayGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetMayGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetMayGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setJuneGroundTemperature(double juneGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setJuneGroundTemperature(juneGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetJuneGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetJuneGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setJulyGroundTemperature(double julyGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setJulyGroundTemperature(julyGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetJulyGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetJulyGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setAugustGroundTemperature(double augustGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setAugustGroundTemperature(augustGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetAugustGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetAugustGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setSeptemberGroundTemperature(double septemberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setSeptemberGroundTemperature(septemberGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetSeptemberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetSeptemberGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setOctoberGroundTemperature(double octoberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setOctoberGroundTemperature(octoberGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetOctoberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetOctoberGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setNovemberGroundTemperature(double novemberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setNovemberGroundTemperature(novemberGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetNovemberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetNovemberGroundTemperature();
}

void SiteGroundTemperatureBuildingSurface::setDecemberGroundTemperature(double decemberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setDecemberGroundTemperature(decemberGroundTemperature);
}

void SiteGroundTemperatureBuildingSurface::resetDecemberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetDecemberGroundTemperature();
}

/// @cond
SiteGroundTemperatureBuildingSurface::SiteGroundTemperatureBuildingSurface(std::shared_ptr<detail::SiteGroundTemperatureBuildingSurface_Impl> impl)
  : ModelObject(impl)
{}
SiteGroundTemperatureBuildingSurface::SiteGroundTemperatureBuildingSurface(Model& model)
  : ModelObject(SiteGroundTemperatureBuildingSurface::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio

