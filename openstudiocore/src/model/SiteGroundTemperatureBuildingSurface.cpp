/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include <vector>
#include <string>
#include "SiteGroundTemperatureBuildingSurface.hpp"
#include "SiteGroundTemperatureBuildingSurface_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_Site_GroundTemperature_BuildingSurface_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/time/Date.hpp"

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

  double SiteGroundTemperatureBuildingSurface_Impl::getTemperatureByMonth(int month) const {
    double retval;
    switch(month) {
      case 1 : retval = januaryGroundTemperature(); break;
      case 2 : retval = februaryGroundTemperature(); break;
      case 3 : retval = marchGroundTemperature(); break;
      case 4 : retval = aprilGroundTemperature(); break;
      case 5 : retval = mayGroundTemperature(); break;
      case 6 : retval = juneGroundTemperature(); break;
      case 7 : retval = julyGroundTemperature(); break;
      case 8 : retval = augustGroundTemperature(); break;
      case 9 : retval = septemberGroundTemperature(); break;
      case 10 : retval = octoberGroundTemperature(); break;
      case 11 : retval = novemberGroundTemperature(); break;
      case 12 : retval = decemberGroundTemperature(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
    return retval;
  }

  double SiteGroundTemperatureBuildingSurface_Impl::getTemperatureByMonth(const openstudio::MonthOfYear & month) const {
    return getTemperatureByMonth(month.value());
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isMonthDefaulted(int month) const {
    bool is_defaulted;
    switch(month) {
      case 1 : is_defaulted = isJanuaryGroundTemperatureDefaulted(); break;
      case 2 : is_defaulted = isFebruaryGroundTemperatureDefaulted(); break;
      case 3 : is_defaulted = isMarchGroundTemperatureDefaulted(); break;
      case 4 : is_defaulted = isAprilGroundTemperatureDefaulted(); break;
      case 5 : is_defaulted = isMayGroundTemperatureDefaulted(); break;
      case 6 : is_defaulted = isJuneGroundTemperatureDefaulted(); break;
      case 7 : is_defaulted = isJulyGroundTemperatureDefaulted(); break;
      case 8 : is_defaulted = isAugustGroundTemperatureDefaulted(); break;
      case 9 : is_defaulted = isSeptemberGroundTemperatureDefaulted(); break;
      case 10 : is_defaulted = isOctoberGroundTemperatureDefaulted(); break;
      case 11 : is_defaulted = isNovemberGroundTemperatureDefaulted(); break;
      case 12 : is_defaulted = isDecemberGroundTemperatureDefaulted(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
    return is_defaulted;
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::isMonthDefaulted(const openstudio::MonthOfYear & month) const {
    return isMonthDefaulted(month.value());
  }

  std::vector<double> SiteGroundTemperatureBuildingSurface_Impl::getAllMonthlyTemperatures() const {
    std::vector<double> monthly_temperatures;
    monthly_temperatures.reserve(12);
    for (int i=1; i <= 12; ++i) {
      monthly_temperatures.push_back(getTemperatureByMonth(i));
    }
    return monthly_temperatures;
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

  void SiteGroundTemperatureBuildingSurface_Impl::setTemperatureByMonth(int month, double temperature) {
    switch(month) {
      case 1 : setJanuaryGroundTemperature(temperature); break;
      case 2 : setFebruaryGroundTemperature(temperature); break;
      case 3 : setMarchGroundTemperature(temperature); break;
      case 4 : setAprilGroundTemperature(temperature); break;
      case 5 : setMayGroundTemperature(temperature); break;
      case 6 : setJuneGroundTemperature(temperature); break;
      case 7 : setJulyGroundTemperature(temperature); break;
      case 8 : setAugustGroundTemperature(temperature); break;
      case 9 : setSeptemberGroundTemperature(temperature); break;
      case 10 : setOctoberGroundTemperature(temperature); break;
      case 11 : setNovemberGroundTemperature(temperature); break;
      case 12 : setDecemberGroundTemperature(temperature); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
  }

  void SiteGroundTemperatureBuildingSurface_Impl::setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature) {
    return setTemperatureByMonth(month.value(), temperature);
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetTemperatureByMonth(int month) {
    switch(month) {
      case 1 : resetJanuaryGroundTemperature(); break;
      case 2 : resetFebruaryGroundTemperature(); break;
      case 3 : resetMarchGroundTemperature(); break;
      case 4 : resetAprilGroundTemperature(); break;
      case 5 : resetMayGroundTemperature(); break;
      case 6 : resetJuneGroundTemperature(); break;
      case 7 : resetJulyGroundTemperature(); break;
      case 8 : resetAugustGroundTemperature(); break;
      case 9 : resetSeptemberGroundTemperature(); break;
      case 10 : resetOctoberGroundTemperature(); break;
      case 11 : resetNovemberGroundTemperature(); break;
      case 12 : resetDecemberGroundTemperature(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetTemperatureByMonth(const openstudio::MonthOfYear & month) {
    return resetTemperatureByMonth(month.value());
  }

  void SiteGroundTemperatureBuildingSurface_Impl::resetAllMonths() {
    for (int i = 1; i <= 12; ++i) {
      resetTemperatureByMonth(i);
    }
  }

  bool SiteGroundTemperatureBuildingSurface_Impl::setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures) {
    if (monthly_temperatures.size() == 12) {
      for (int i = 1; i <= 12; ++i) {
        setTemperatureByMonth(i, monthly_temperatures[i-1]);
      }
      return true;
    } else {
      return false;
    }
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

double SiteGroundTemperatureBuildingSurface::getTemperatureByMonth(int month) const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->getTemperatureByMonth(month);
}

double SiteGroundTemperatureBuildingSurface::getTemperatureByMonth(const openstudio::MonthOfYear & month) const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->getTemperatureByMonth(month);
}

bool SiteGroundTemperatureBuildingSurface::isMonthDefaulted(int month) const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isMonthDefaulted(month);
}

bool SiteGroundTemperatureBuildingSurface::isMonthDefaulted(const openstudio::MonthOfYear & month) const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->isMonthDefaulted(month);
}

std::vector<double> SiteGroundTemperatureBuildingSurface::getAllMonthlyTemperatures() const {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->getAllMonthlyTemperatures();
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

void SiteGroundTemperatureBuildingSurface::setTemperatureByMonth(int month, double temperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setTemperatureByMonth(month, temperature);
}

void SiteGroundTemperatureBuildingSurface::setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setTemperatureByMonth(month, temperature);
}

void SiteGroundTemperatureBuildingSurface::resetTemperatureByMonth(int month) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetTemperatureByMonth(month);
}

void SiteGroundTemperatureBuildingSurface::resetTemperatureByMonth(const openstudio::MonthOfYear & month) {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetTemperatureByMonth(month);
}

void SiteGroundTemperatureBuildingSurface::resetAllMonths() {
  getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->resetAllMonths();
}

bool SiteGroundTemperatureBuildingSurface::setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures) {
  return getImpl<detail::SiteGroundTemperatureBuildingSurface_Impl>()->setAllMonthlyTemperatures(monthly_temperatures);
}

/// @cond
SiteGroundTemperatureBuildingSurface::SiteGroundTemperatureBuildingSurface(std::shared_ptr<detail::SiteGroundTemperatureBuildingSurface_Impl> impl)
  : ModelObject(std::move(impl))
{}
SiteGroundTemperatureBuildingSurface::SiteGroundTemperatureBuildingSurface(Model& model)
  : ModelObject(SiteGroundTemperatureBuildingSurface::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio
