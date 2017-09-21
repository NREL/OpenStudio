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
#include "SiteGroundTemperatureShallow.hpp"
#include "SiteGroundTemperatureShallow_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"

#include <utilities/idd/OS_Site_GroundTemperature_Shallow_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include "../utilities/core/Assert.hpp"
#include "../utilities/time/Date.hpp"

namespace openstudio {
namespace model {

namespace detail {

  SiteGroundTemperatureShallow_Impl::SiteGroundTemperatureShallow_Impl(
      const IdfObject& idfObject,
      Model_Impl* model,
      bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == SiteGroundTemperatureShallow::iddObjectType());
  }

  SiteGroundTemperatureShallow_Impl::SiteGroundTemperatureShallow_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == SiteGroundTemperatureShallow::iddObjectType());
  }

  SiteGroundTemperatureShallow_Impl::SiteGroundTemperatureShallow_Impl(const SiteGroundTemperatureShallow_Impl& other,
                                                                                       Model_Impl* model,
                                                                                       bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  boost::optional<ParentObject> SiteGroundTemperatureShallow_Impl::parent() const
  {
    boost::optional<Site> result = this->model().getOptionalUniqueModelObject<Site>();
    return boost::optional<ParentObject>(result);
  }

  const std::vector<std::string>& SiteGroundTemperatureShallow_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType SiteGroundTemperatureShallow_Impl::iddObjectType() const {
    return SiteGroundTemperatureShallow::iddObjectType();
  }

  double SiteGroundTemperatureShallow_Impl::januarySurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::JanuarySurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isJanuarySurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::JanuarySurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::februarySurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::FebruarySurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isFebruarySurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::FebruarySurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::marchSurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::MarchSurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isMarchSurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::MarchSurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::aprilSurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::AprilSurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isAprilSurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::AprilSurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::maySurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::MaySurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isMaySurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::MaySurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::juneSurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::JuneSurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isJuneSurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::JuneSurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::julySurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::JulySurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isJulySurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::JulySurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::augustSurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::AugustSurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isAugustSurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::AugustSurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::septemberSurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::SeptemberSurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isSeptemberSurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::SeptemberSurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::octoberSurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::OctoberSurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isOctoberSurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::OctoberSurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::novemberSurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::NovemberSurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isNovemberSurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::NovemberSurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::decemberSurfaceGroundTemperature() const {
    boost::optional<double> value = getDouble(OS_Site_GroundTemperature_ShallowFields::DecemberSurfaceGroundTemperature,true);
    OS_ASSERT(value);
    return value.get();
  }

  bool SiteGroundTemperatureShallow_Impl::isDecemberSurfaceGroundTemperatureDefaulted() const {
    return isEmpty(OS_Site_GroundTemperature_ShallowFields::DecemberSurfaceGroundTemperature);
  }

  double SiteGroundTemperatureShallow_Impl::getTemperatureByMonth(int month) const {
    double retval;
    switch(month) {
      case 1 : retval = januarySurfaceGroundTemperature(); break;
      case 2 : retval = februarySurfaceGroundTemperature(); break;
      case 3 : retval = marchSurfaceGroundTemperature(); break;
      case 4 : retval = aprilSurfaceGroundTemperature(); break;
      case 5 : retval = maySurfaceGroundTemperature(); break;
      case 6 : retval = juneSurfaceGroundTemperature(); break;
      case 7 : retval = julySurfaceGroundTemperature(); break;
      case 8 : retval = augustSurfaceGroundTemperature(); break;
      case 9 : retval = septemberSurfaceGroundTemperature(); break;
      case 10 : retval = octoberSurfaceGroundTemperature(); break;
      case 11 : retval = novemberSurfaceGroundTemperature(); break;
      case 12 : retval = decemberSurfaceGroundTemperature(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
    return retval;
  }

  double SiteGroundTemperatureShallow_Impl::getTemperatureByMonth(const openstudio::MonthOfYear & month) const {
    return getTemperatureByMonth(month.value());
  }

  bool SiteGroundTemperatureShallow_Impl::isMonthDefaulted(int month) const {
    bool is_defaulted;
    switch(month) {
      case 1 : is_defaulted = isJanuarySurfaceGroundTemperatureDefaulted(); break;
      case 2 : is_defaulted = isFebruarySurfaceGroundTemperatureDefaulted(); break;
      case 3 : is_defaulted = isMarchSurfaceGroundTemperatureDefaulted(); break;
      case 4 : is_defaulted = isAprilSurfaceGroundTemperatureDefaulted(); break;
      case 5 : is_defaulted = isMaySurfaceGroundTemperatureDefaulted(); break;
      case 6 : is_defaulted = isJuneSurfaceGroundTemperatureDefaulted(); break;
      case 7 : is_defaulted = isJulySurfaceGroundTemperatureDefaulted(); break;
      case 8 : is_defaulted = isAugustSurfaceGroundTemperatureDefaulted(); break;
      case 9 : is_defaulted = isSeptemberSurfaceGroundTemperatureDefaulted(); break;
      case 10 : is_defaulted = isOctoberSurfaceGroundTemperatureDefaulted(); break;
      case 11 : is_defaulted = isNovemberSurfaceGroundTemperatureDefaulted(); break;
      case 12 : is_defaulted = isDecemberSurfaceGroundTemperatureDefaulted(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
    return is_defaulted;
  }

  bool SiteGroundTemperatureShallow_Impl::isMonthDefaulted(const openstudio::MonthOfYear & month) const {
    return isMonthDefaulted(month.value());
  }

  std::vector<double> SiteGroundTemperatureShallow_Impl::getAllMonthlyTemperatures() const {
    std::vector<double> monthly_temperatures;
    monthly_temperatures.reserve(12);
    for (int i=1; i <= 12; ++i) {
      monthly_temperatures.push_back(getTemperatureByMonth(i));
    }
    return monthly_temperatures;
  }

  void SiteGroundTemperatureShallow_Impl::setJanuarySurfaceGroundTemperature(double januarySurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::JanuarySurfaceGroundTemperature, januarySurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetJanuarySurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::JanuarySurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setFebruarySurfaceGroundTemperature(double februarySurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::FebruarySurfaceGroundTemperature, februarySurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetFebruarySurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::FebruarySurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setMarchSurfaceGroundTemperature(double marchSurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::MarchSurfaceGroundTemperature, marchSurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetMarchSurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::MarchSurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setAprilSurfaceGroundTemperature(double aprilSurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::AprilSurfaceGroundTemperature, aprilSurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetAprilSurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::AprilSurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setMaySurfaceGroundTemperature(double maySurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::MaySurfaceGroundTemperature, maySurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetMaySurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::MaySurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setJuneSurfaceGroundTemperature(double juneSurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::JuneSurfaceGroundTemperature, juneSurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetJuneSurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::JuneSurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setJulySurfaceGroundTemperature(double julySurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::JulySurfaceGroundTemperature, julySurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetJulySurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::JulySurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setAugustSurfaceGroundTemperature(double augustSurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::AugustSurfaceGroundTemperature, augustSurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetAugustSurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::AugustSurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setSeptemberSurfaceGroundTemperature(double septemberSurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::SeptemberSurfaceGroundTemperature, septemberSurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetSeptemberSurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::SeptemberSurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setOctoberSurfaceGroundTemperature(double octoberSurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::OctoberSurfaceGroundTemperature, octoberSurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetOctoberSurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::OctoberSurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setNovemberSurfaceGroundTemperature(double novemberSurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::NovemberSurfaceGroundTemperature, novemberSurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetNovemberSurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::NovemberSurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setDecemberSurfaceGroundTemperature(double decemberSurfaceGroundTemperature) {
    bool result = setDouble(OS_Site_GroundTemperature_ShallowFields::DecemberSurfaceGroundTemperature, decemberSurfaceGroundTemperature);
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::resetDecemberSurfaceGroundTemperature() {
    bool result = setString(OS_Site_GroundTemperature_ShallowFields::DecemberSurfaceGroundTemperature, "");
    OS_ASSERT(result);
  }

  void SiteGroundTemperatureShallow_Impl::setTemperatureByMonth(int month, double temperature) {
    switch(month) {
      case 1 : setJanuarySurfaceGroundTemperature(temperature); break;
      case 2 : setFebruarySurfaceGroundTemperature(temperature); break;
      case 3 : setMarchSurfaceGroundTemperature(temperature); break;
      case 4 : setAprilSurfaceGroundTemperature(temperature); break;
      case 5 : setMaySurfaceGroundTemperature(temperature); break;
      case 6 : setJuneSurfaceGroundTemperature(temperature); break;
      case 7 : setJulySurfaceGroundTemperature(temperature); break;
      case 8 : setAugustSurfaceGroundTemperature(temperature); break;
      case 9 : setSeptemberSurfaceGroundTemperature(temperature); break;
      case 10 : setOctoberSurfaceGroundTemperature(temperature); break;
      case 11 : setNovemberSurfaceGroundTemperature(temperature); break;
      case 12 : setDecemberSurfaceGroundTemperature(temperature); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
  }

  void SiteGroundTemperatureShallow_Impl::setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature) {
    return setTemperatureByMonth(month.value(), temperature);
  }

  void SiteGroundTemperatureShallow_Impl::resetTemperatureByMonth(int month) {
    switch(month) {
      case 1 : resetJanuarySurfaceGroundTemperature(); break;
      case 2 : resetFebruarySurfaceGroundTemperature(); break;
      case 3 : resetMarchSurfaceGroundTemperature(); break;
      case 4 : resetAprilSurfaceGroundTemperature(); break;
      case 5 : resetMaySurfaceGroundTemperature(); break;
      case 6 : resetJuneSurfaceGroundTemperature(); break;
      case 7 : resetJulySurfaceGroundTemperature(); break;
      case 8 : resetAugustSurfaceGroundTemperature(); break;
      case 9 : resetSeptemberSurfaceGroundTemperature(); break;
      case 10 : resetOctoberSurfaceGroundTemperature(); break;
      case 11 : resetNovemberSurfaceGroundTemperature(); break;
      case 12 : resetDecemberSurfaceGroundTemperature(); break;
      default : LOG_AND_THROW("SiteGroundTemperature::getTemperatureByMonth: Invalid Month " + std::to_string(month)); break;
    }
  }

  void SiteGroundTemperatureShallow_Impl::resetTemperatureByMonth(const openstudio::MonthOfYear & month) {
    return resetTemperatureByMonth(month.value());
  }

  void SiteGroundTemperatureShallow_Impl::resetAllMonths() {
    for (int i = 1; i <= 12; ++i) {
      resetTemperatureByMonth(i);
    }
  }

  bool SiteGroundTemperatureShallow_Impl::setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures) {
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

IddObjectType SiteGroundTemperatureShallow::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Site_GroundTemperature_Shallow);
  return result;
}

double SiteGroundTemperatureShallow::januarySurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->januarySurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isJanuarySurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isJanuarySurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::februarySurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->februarySurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isFebruarySurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isFebruarySurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::marchSurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->marchSurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isMarchSurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isMarchSurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::aprilSurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->aprilSurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isAprilSurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isAprilSurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::maySurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->maySurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isMaySurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isMaySurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::juneSurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->juneSurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isJuneSurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isJuneSurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::julySurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->julySurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isJulySurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isJulySurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::augustSurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->augustSurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isAugustSurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isAugustSurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::septemberSurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->septemberSurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isSeptemberSurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isSeptemberSurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::octoberSurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->octoberSurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isOctoberSurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isOctoberSurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::novemberSurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->novemberSurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isNovemberSurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isNovemberSurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::decemberSurfaceGroundTemperature() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->decemberSurfaceGroundTemperature();
}

bool SiteGroundTemperatureShallow::isDecemberSurfaceGroundTemperatureDefaulted() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isDecemberSurfaceGroundTemperatureDefaulted();
}

double SiteGroundTemperatureShallow::getTemperatureByMonth(int month) const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->getTemperatureByMonth(month);
}

double SiteGroundTemperatureShallow::getTemperatureByMonth(const openstudio::MonthOfYear & month) const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->getTemperatureByMonth(month);
}

bool SiteGroundTemperatureShallow::isMonthDefaulted(int month) const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isMonthDefaulted(month);
}

bool SiteGroundTemperatureShallow::isMonthDefaulted(const openstudio::MonthOfYear & month) const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->isMonthDefaulted(month);
}

std::vector<double> SiteGroundTemperatureShallow::getAllMonthlyTemperatures() const {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->getAllMonthlyTemperatures();
}

void SiteGroundTemperatureShallow::setJanuarySurfaceGroundTemperature(double januarySurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setJanuarySurfaceGroundTemperature(januarySurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetJanuarySurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetJanuarySurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setFebruarySurfaceGroundTemperature(double februarySurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setFebruarySurfaceGroundTemperature(februarySurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetFebruarySurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetFebruarySurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setMarchSurfaceGroundTemperature(double marchSurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setMarchSurfaceGroundTemperature(marchSurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetMarchSurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetMarchSurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setAprilSurfaceGroundTemperature(double aprilSurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setAprilSurfaceGroundTemperature(aprilSurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetAprilSurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetAprilSurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setMaySurfaceGroundTemperature(double maySurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setMaySurfaceGroundTemperature(maySurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetMaySurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetMaySurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setJuneSurfaceGroundTemperature(double juneSurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setJuneSurfaceGroundTemperature(juneSurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetJuneSurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetJuneSurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setJulySurfaceGroundTemperature(double julySurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setJulySurfaceGroundTemperature(julySurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetJulySurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetJulySurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setAugustSurfaceGroundTemperature(double augustSurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setAugustSurfaceGroundTemperature(augustSurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetAugustSurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetAugustSurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setSeptemberSurfaceGroundTemperature(double septemberSurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setSeptemberSurfaceGroundTemperature(septemberSurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetSeptemberSurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetSeptemberSurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setOctoberSurfaceGroundTemperature(double octoberSurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setOctoberSurfaceGroundTemperature(octoberSurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetOctoberSurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetOctoberSurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setNovemberSurfaceGroundTemperature(double novemberSurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setNovemberSurfaceGroundTemperature(novemberSurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetNovemberSurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetNovemberSurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setDecemberSurfaceGroundTemperature(double decemberSurfaceGroundTemperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setDecemberSurfaceGroundTemperature(decemberSurfaceGroundTemperature);
}

void SiteGroundTemperatureShallow::resetDecemberSurfaceGroundTemperature() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetDecemberSurfaceGroundTemperature();
}

void SiteGroundTemperatureShallow::setTemperatureByMonth(int month, double temperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setTemperatureByMonth(month, temperature);
}

void SiteGroundTemperatureShallow::setTemperatureByMonth(const openstudio::MonthOfYear & month, double temperature) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setTemperatureByMonth(month, temperature);
}

void SiteGroundTemperatureShallow::resetTemperatureByMonth(int month) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetTemperatureByMonth(month);
}

void SiteGroundTemperatureShallow::resetTemperatureByMonth(const openstudio::MonthOfYear & month) {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetTemperatureByMonth(month);
}

void SiteGroundTemperatureShallow::resetAllMonths() {
  getImpl<detail::SiteGroundTemperatureShallow_Impl>()->resetAllMonths();
}

bool SiteGroundTemperatureShallow::setAllMonthlyTemperatures(const std::vector<double> &monthly_temperatures) {
  return getImpl<detail::SiteGroundTemperatureShallow_Impl>()->setAllMonthlyTemperatures(monthly_temperatures);
}

/// @cond
SiteGroundTemperatureShallow::SiteGroundTemperatureShallow(std::shared_ptr<detail::SiteGroundTemperatureShallow_Impl> impl)
  : ModelObject(std::move(impl))
{}
SiteGroundTemperatureShallow::SiteGroundTemperatureShallow(Model& model)
  : ModelObject(SiteGroundTemperatureShallow::iddObjectType(),model)
{}

/// @endcond


} // model
} // openstudio
