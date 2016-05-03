/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "SiteGroundTemperatureDeep.hpp"
#include "SiteGroundTemperatureDeep_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_Site_GroundTemperature_Deep_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SiteGroundTemperatureDeep_Impl::SiteGroundTemperatureDeep_Impl(
      const IdfObject& idfObject, 
      Model_Impl* model, 
      bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SiteGroundTemperatureDeep::iddObjectType());
  }

  SiteGroundTemperatureDeep_Impl::SiteGroundTemperatureDeep_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SiteGroundTemperatureDeep::iddObjectType());
  }

  SiteGroundTemperatureDeep_Impl::SiteGroundTemperatureDeep_Impl(const SiteGroundTemperatureDeep_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> SiteGroundTemperatureDeep_Impl::parent() const
  {
    boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
    return boost::optional<ParentObject>(result);
  }

  const std::vector<std::string>& SiteGroundTemperatureDeep_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SiteGroundTemperatureDeep_Impl::iddObjectType() const {
    return SiteGroundTemperatureDeep::iddObjectType();
  }

  double SiteGroundTemperatureDeep_Impl::januaryGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::JanuaryGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isJanuaryGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::JanuaryGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::februaryGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::FebruaryGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isFebruaryGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::FebruaryGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::marchGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::MarchGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isMarchGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::MarchGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::aprilGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::AprilGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isAprilGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::AprilGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::mayGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::MayGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isMayGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::MayGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::juneGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::JuneGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isJuneGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::JuneGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::julyGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::JulyGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isJulyGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::JulyGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::augustGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::AugustGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isAugustGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::AugustGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::septemberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::SeptemberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isSeptemberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::SeptemberGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::octoberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::OctoberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isOctoberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::OctoberGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::novemberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::NovemberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isNovemberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::NovemberGroundTemperature);
  }

  double SiteGroundTemperatureDeep_Impl::decemberGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_DeepFields::DecemberGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureDeep_Impl::isDecemberGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_DeepFields::DecemberGroundTemperature);
  }

  void SiteGroundTemperatureDeep_Impl::setJanuaryGroundTemperature(double januaryGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::JanuaryGroundTemperature, januaryGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetJanuaryGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::JanuaryGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setFebruaryGroundTemperature(double februaryGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::FebruaryGroundTemperature, februaryGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetFebruaryGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::FebruaryGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setMarchGroundTemperature(double marchGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::MarchGroundTemperature, marchGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetMarchGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::MarchGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setAprilGroundTemperature(double aprilGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::AprilGroundTemperature, aprilGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetAprilGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::AprilGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setMayGroundTemperature(double mayGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::MayGroundTemperature, mayGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetMayGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::MayGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setJuneGroundTemperature(double juneGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::JuneGroundTemperature, juneGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetJuneGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::JuneGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setJulyGroundTemperature(double julyGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::JulyGroundTemperature, julyGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetJulyGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::JulyGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setAugustGroundTemperature(double augustGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::AugustGroundTemperature, augustGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetAugustGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::AugustGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setSeptemberGroundTemperature(double septemberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::SeptemberGroundTemperature, septemberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetSeptemberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::SeptemberGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setOctoberGroundTemperature(double octoberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::OctoberGroundTemperature, octoberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetOctoberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::OctoberGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setNovemberGroundTemperature(double novemberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::NovemberGroundTemperature, novemberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetNovemberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::NovemberGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::setDecemberGroundTemperature(double decemberGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_DeepFields::DecemberGroundTemperature, decemberGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureDeep_Impl::resetDecemberGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_DeepFields::DecemberGroundTemperature, "");
    OS_ASSERT(result);
  }

} // detail

IddObjectType SiteGroundTemperatureDeep::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Site_GroundTemperature_Deep);
  return result;
}

double SiteGroundTemperatureDeep::januaryGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->januaryGroundTemperature();
}

bool SiteGroundTemperatureDeep::isJanuaryGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isJanuaryGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::februaryGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->februaryGroundTemperature();
}

bool SiteGroundTemperatureDeep::isFebruaryGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isFebruaryGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::marchGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->marchGroundTemperature();
}

bool SiteGroundTemperatureDeep::isMarchGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isMarchGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::aprilGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->aprilGroundTemperature();
}

bool SiteGroundTemperatureDeep::isAprilGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isAprilGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::mayGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->mayGroundTemperature();
}

bool SiteGroundTemperatureDeep::isMayGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isMayGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::juneGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->juneGroundTemperature();
}

bool SiteGroundTemperatureDeep::isJuneGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isJuneGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::julyGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->julyGroundTemperature();
}

bool SiteGroundTemperatureDeep::isJulyGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isJulyGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::augustGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->augustGroundTemperature();
}

bool SiteGroundTemperatureDeep::isAugustGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isAugustGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::septemberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->septemberGroundTemperature();
}

bool SiteGroundTemperatureDeep::isSeptemberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isSeptemberGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::octoberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->octoberGroundTemperature();
}

bool SiteGroundTemperatureDeep::isOctoberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isOctoberGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::novemberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->novemberGroundTemperature();
}

bool SiteGroundTemperatureDeep::isNovemberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isNovemberGroundTemperatureDefaulted();
}

double SiteGroundTemperatureDeep::decemberGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->decemberGroundTemperature();
}

bool SiteGroundTemperatureDeep::isDecemberGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureDeep_Impl>()->isDecemberGroundTemperatureDefaulted();
}

void SiteGroundTemperatureDeep::setJanuaryGroundTemperature(double januaryGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setJanuaryGroundTemperature(januaryGroundTemperature);
}

void SiteGroundTemperatureDeep::resetJanuaryGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetJanuaryGroundTemperature();
}

void SiteGroundTemperatureDeep::setFebruaryGroundTemperature(double februaryGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setFebruaryGroundTemperature(februaryGroundTemperature);
}

void SiteGroundTemperatureDeep::resetFebruaryGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetFebruaryGroundTemperature();
}

void SiteGroundTemperatureDeep::setMarchGroundTemperature(double marchGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setMarchGroundTemperature(marchGroundTemperature);
}

void SiteGroundTemperatureDeep::resetMarchGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetMarchGroundTemperature();
}

void SiteGroundTemperatureDeep::setAprilGroundTemperature(double aprilGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setAprilGroundTemperature(aprilGroundTemperature);
}

void SiteGroundTemperatureDeep::resetAprilGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetAprilGroundTemperature();
}

void SiteGroundTemperatureDeep::setMayGroundTemperature(double mayGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setMayGroundTemperature(mayGroundTemperature);
}

void SiteGroundTemperatureDeep::resetMayGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetMayGroundTemperature();
}

void SiteGroundTemperatureDeep::setJuneGroundTemperature(double juneGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setJuneGroundTemperature(juneGroundTemperature);
}

void SiteGroundTemperatureDeep::resetJuneGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetJuneGroundTemperature();
}

void SiteGroundTemperatureDeep::setJulyGroundTemperature(double julyGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setJulyGroundTemperature(julyGroundTemperature);
}

void SiteGroundTemperatureDeep::resetJulyGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetJulyGroundTemperature();
}

void SiteGroundTemperatureDeep::setAugustGroundTemperature(double augustGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setAugustGroundTemperature(augustGroundTemperature);
}

void SiteGroundTemperatureDeep::resetAugustGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetAugustGroundTemperature();
}

void SiteGroundTemperatureDeep::setSeptemberGroundTemperature(double septemberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setSeptemberGroundTemperature(septemberGroundTemperature);
}

void SiteGroundTemperatureDeep::resetSeptemberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetSeptemberGroundTemperature();
}

void SiteGroundTemperatureDeep::setOctoberGroundTemperature(double octoberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setOctoberGroundTemperature(octoberGroundTemperature);
}

void SiteGroundTemperatureDeep::resetOctoberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetOctoberGroundTemperature();
}

void SiteGroundTemperatureDeep::setNovemberGroundTemperature(double novemberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setNovemberGroundTemperature(novemberGroundTemperature);
}

void SiteGroundTemperatureDeep::resetNovemberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetNovemberGroundTemperature();
}

void SiteGroundTemperatureDeep::setDecemberGroundTemperature(double decemberGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->setDecemberGroundTemperature(decemberGroundTemperature);
}

void SiteGroundTemperatureDeep::resetDecemberGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureDeep_Impl>()->resetDecemberGroundTemperature();
}

/// @cond
SiteGroundTemperatureDeep::SiteGroundTemperatureDeep(std::shared_ptr<detail::SiteGroundTemperatureDeep_Impl> impl)
  : ModelObject(impl)
{}
SiteGroundTemperatureDeep::SiteGroundTemperatureDeep(Model& model)
  : ModelObject(SiteGroundTemperatureDeep::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio

